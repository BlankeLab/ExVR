
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

#include "ex_resource_w.hpp"

// Qt
#include <QStringBuilder>

// base
#include "utility/benchmark.hpp"

// local
#include "resources/resources_manager.hpp"
#include "experiment/global_signals.hpp"
#include "experiment/experiment.hpp"

using namespace tool::ex;

ExResourceW::ExResourceW(QString name) : ExItemW<QFrame>(UiType::Resource, name){

    w->setFrameShadow(QFrame::Raised);
    w->setFrameShape(QFrame::Shape::NoFrame);

    QHBoxLayout *l = new QHBoxLayout();
    w->setLayout(l);
    l->addWidget(m_icon = new QPushButton());
    l->addWidget(m_title = new QLabel());
    l->addWidget(m_resourcesAlias = new QComboBox());    
    l->addWidget(m_resources = new QPushButton());
    l->setStretch(0,1);
    l->setStretch(1,1);
    l->setStretch(2,50);
    l->setStretch(3,1);
    l->setContentsMargins(2,2,2,2);
    l->setSpacing(5);

    connect(m_resources, &QPushButton::clicked, this, [&]{
        emit GSignals::get()->show_resources_manager_dialog_signal(m_resourceType);
    });
    connect(m_resourcesAlias, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&]{

        m_currentKey = -1;
        if(m_resourcesAlias->currentIndex() > 0){

            const size_t id = to_unsigned(m_resourcesAlias->currentIndex()-1);
            if(auto resources = ExperimentManager::get()->current()->resM.get_resources(m_resourceType); id < resources.size()){
                m_currentKey =  resources[id]->key();
            }
        }
        trigger_ui_change();
    });
}

ExResourceW *ExResourceW::init_widget(Resource::Type resourceType, QString title, bool enabled){

    w->setEnabled(enabled);
    m_resourceType = resourceType;
    m_title->setText(title);

    m_icon->setText("");
    m_icon->setIcon(QIcon(from_view(Resource::get_icon_path(m_resourceType))));
    m_icon->setIconSize(QSize(30,30));
    m_icon->setMinimumWidth(35);

    m_resources->setText("");
    m_resources->setIcon(QIcon(":/icons/Resources"));
    m_resources->setIconSize(QSize(20,20));
    m_resources->setMinimumWidth(25);
    m_resources->setMinimumHeight(25);
    m_resources->setToolTip("Open resources manager");
    m_resourcesAlias->setMaxVisibleItems(50);

    return this;
}

void ExResourceW::update_from_arg(const Arg &arg){

    ExItemW::update_from_arg(arg);

    w->blockSignals(true);

    if(arg.generator.has_value()){
        if(auto info = arg.generator->info; info.has_value()){
            if(auto type = Resource::get_type(info.value().toStdString()); type.has_value()){
                m_resourceType = type.value();
                m_icon->setIcon(QIcon(from_view(Resource::get_icon_path(m_resourceType))));
                m_title->setText(info.value());
            }
        }
    }

    auto split = arg.split_value();
    m_currentKey = split[1].toInt();
    update_from_resources();

    w->blockSignals(false);
}

Arg ExResourceW::convert_to_arg() const{

    Arg arg = ExBaseW::convert_to_arg();
    QString alias = "";
    if(auto resource = ExperimentManager::get()->current()->resM.get_resource(m_resourceType, ResourceKey{m_currentKey}, false); resource != nullptr){
        alias = resource->alias;
    }
    arg.init_from({alias, std::to_string(m_currentKey).c_str()}, "%%%");

    if(hasGenerator){
        arg.generator->info = from_view(Resource::get_name(m_resourceType));
    }

    return arg;
}

void ExResourceW::update_from_resources(){

    blockSignals(true);

    Bench::start("ExResourceW::update_from_resources_1"sv);

    QString currentText = "";
    if(m_currentKey != -1){
        // check if component still available
        if(auto resource = ExperimentManager::get()->current()->resM.get_resource(m_resourceType, ResourceKey{m_currentKey}, false); resource != nullptr){
            currentText = resource->display_name();
        }
    }

    Bench::stop();
    Bench::start("ExResourceW::update_from_resources_2"sv);

    auto resources = ExperimentManager::get()->current()->resM.get_resources(m_resourceType);

    bool rebuild = false;
    if(resourcesId.size() != resources.size()){
        rebuild = true;
    }else{
        for(size_t ii = 0; ii < resources.size(); ++ii){
            if(resources[ii]->key() != resourcesId[ii]){
                rebuild = true;
                break;
            }
        }
    }


    Bench::stop();
    Bench::start("ExResourceW::update_from_resources_3"sv);

    if(rebuild){

        m_resourcesAlias->blockSignals(true);

        m_resourcesAlias->clear();

        resourcesId.clear();
        resources.reserve(resources.size());

        QStringList names;
        names.reserve(to_signed(resources.size()+1));
        names << "";
        for(const auto &resource : resources){
            names << resource->display_name();
            resourcesId.emplace_back(resource->key());
        }       
        m_resourcesAlias->addItems(names);
        m_resourcesAlias->blockSignals(false);
    }

    Bench::stop();
    Bench::start("ExResourceW::update_from_resources_4"sv);

    if(m_resourcesAlias->currentText() != currentText){
        m_resourcesAlias->setCurrentText(currentText);
    }

    Bench::stop();

    blockSignals(false);
}
