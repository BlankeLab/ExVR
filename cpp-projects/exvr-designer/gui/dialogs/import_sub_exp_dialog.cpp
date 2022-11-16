
/***********************************************************************************
** exvr-designer                                                                  **
** MIT License                                                                    **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                                **
** Permission is hereby granted, free of charge, to any person obtaining a copy   **
** of this software and associated documentation files (the "Software"), to deal  **
** in the Software without restriction, including without limitation the rights   **
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      **
** copies of the Software, and to permit persons to whom the Software is          **
** furnished to do so, subject to the following conditions:                       **
**                                                                                **
** The above copyright notice and this permission notice shall be included in all **
** copies or substantial portions of the Software.                                **
**                                                                                **
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     **
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       **
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    **
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         **
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  **
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  **
** SOFTWARE.                                                                      **
************************************************************************************/

#include "import_sub_exp_dialog.hpp"

// qt-utility
#include "qt_logger.hpp"
#include "gui/widgets/list_widget.hpp"

// local
#include "data/components_manager.hpp"
#include "experiment/experiment.hpp"
#include "IO/xml_io_manager.hpp"
#include "experiment/global_signals.hpp"

using namespace tool;
using namespace tool::ex;

ImportSubExpDialog::ImportSubExpDialog(QString path){

    validatedConfigs.clear();
    validatedResources.clear();

    setWindowTitle(QSL("Import experiment sub-parts"));
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);

    setLayout(new QVBoxLayout());

    auto exp = ExperimentManager::get()->current();

    ExperimentManager::get()->init_imported(exp->states.numVersion);
    auto importedExp = ExperimentManager::get()->imported();

    XmlIoManager xmlR(importedExp);
    QtLogger::message("exp read");

    IdKey::set_source(IdKey::Source::Imported);
    bool success = xmlR.load_experiment_file(path);
    IdKey::set_source(IdKey::Source::Current);

    if(!success){
        QtLogger::error(QSL("Cannot load exp ") % path);
        return;
    }

    QTabWidget *tw = new QTabWidget();
    layout()->addWidget(tw);

    QTabWidget *twResources = new QTabWidget();
    tw->addTab(twResources, "Resources");

    for(auto rType : Resource::get_types()){
        auto resources = importedExp->resM.get_resources(rType);
        if(resources.size() == 0){
            continue;
        }

        ui::ListWidget *lwResources = new ui::ListWidget();
        lwResources->set_margins(2,2,2,2,2);
        twResources->addTab(lwResources, from_view(Resource::get_name(rType)) % QSL("(") % QString::number(resources.size()) % QSL(")"));

        for(const auto &resource : resources){
            auto resourceCb = new QCheckBox(resource->alias % QSL(" [") % resource->path % QSL("] "));
            lwResources->add_widget(resourceCb);
            validatedResources[resource->r_key()] = resourceCb;
        }
    }

    QTabWidget *twComponents = new QTabWidget();
    tw->addTab(twComponents, "Components");

    for(auto category : Component::all_categories()){

        auto count = importedExp->compM.count(category);
        if(count == 0){
            continue;
        }

        ui::ListWidget *lwCategory = new ui::ListWidget();
        lwCategory->set_margins(2,2,2,2,2);
        twComponents->addTab(lwCategory, from_view(Component::get_display_name(category)) % QSL("(") % QString::number(count) % QSL(")"));

        std::for_each(importedExp->compM.category_begin(category), importedExp->compM.category_end(category), [&](std::pair<QStringView,Component*> p){

            lwCategory->add_widget(
                new QLabel(p.second->name() % QSL(" [") % from_view(Component::get_full_name(p.second->type)) % QSL("] ") % QSL("(") % QString::number(p.second->configs.size()) % QSL(")")));

            for(const auto &config : p.second->configs){
                auto configCb = new QCheckBox(config->name);
                lwCategory->add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("\t"), configCb}, LStretch{true}, LMargins{false},QFrame::NoFrame));
                validatedConfigs[p.second->c_key()][ConfigKey{config->key()}] = configCb;
            }
        });
    }

    auto pbValidate = new QPushButton("Import selection");
    auto pbCancel   = new QPushButton("Cancel");
    layout()->addWidget(ui::F::gen(ui::L::HB(),{pbValidate, pbCancel}, LStretch{true}, LMargins{false},QFrame::NoFrame));


    // settings dialog
    connect(pbValidate, &QPushButton::clicked, this,&ImportSubExpDialog::accept);
    connect(pbCancel,  &QPushButton::clicked, this,  &ImportSubExpDialog::reject);

    connect(this, &ImportSubExpDialog::finished, this, [&](int result){

        if(result != 1){
            return; }

        for(const auto &resource : validatedResources){
            if(resource.second->isChecked()){
                emit GSignals::get()->copy_resource_signal(
                    ExperimentManager::get()->imported()->resM.get_resource(resource.first)
                );
            }
        }



        for(const auto &componentConfigs : validatedConfigs){

            std::vector<ConfigKey> configsKeys;
            for(const auto &config : componentConfigs.second){
                if(config.second->isChecked()){
                    configsKeys.push_back(config.first);
                }
            }
            if(configsKeys.size() == 0){
                continue;
            }

            qDebug() << "copy signal";
            emit GSignals::get()->copy_component_signal(
                ExperimentManager::get()->imported()->compM.get_component(componentConfigs.first),
                std::move(configsKeys),
                RowId{0}
            );
            qDebug() << "end copy signal";
        }

        qDebug() << "clean";
        ExperimentManager::get()->clean_imported();
    });

}
