
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

#include "ex_resources_list_w.hpp"

// local
#include "experiment/experiment.hpp"

using namespace tool::ex;

ExResourcesListW::ExResourcesListW(QString name) : ExItemW<QFrame>(UiType::ResourcesList, name){

    w->setFrameShadow(QFrame::Raised);
    w->setFrameShape(QFrame::Shape::NoFrame);

    auto f1 = ui::F::gen_frame(ui::L::HB(), {
        {m_icon = new QPushButton(),1},
        {m_title = new QLabel(), 1},
        {m_resourcesAliases = new QComboBox(), 50},
        {m_add = new QPushButton("Add"), 1},
        {m_remove = new QPushButton("Rem"), 1}},
        0,
        LMarginsD{0,0,0,0,2}
    );

    auto l = ui::L::VB();
    l->addWidget(f1);
    l->addWidget(m_list = new ui::ListWidget());
    l->setStretch(0,1);
    l->setStretch(1,50);
    w->setLayout(l);

    m_list->set_margins(2,2,2,2,1);
    m_list->set_widget_selection(true);


    connect(m_add, &QPushButton::clicked, this, [&]{

        if(m_resourcesAliases->count() == 0){
            return;
        }

        const auto currTxt = m_resourcesAliases->currentText();
        if(auto resource = ExperimentManager::get()->current()->resM.get_resource(m_resourceType.value(), currTxt); resource != nullptr){
            m_resourcesKeys.emplace_back(resource->key());
            m_list->add_widget(ui::W::txt(resource->alias));
            update_from_resources();
            trigger_ui_change();
        }

    });
    connect(m_remove, &QPushButton::clicked, this, [&]{
        const int id = m_list->current_selected_widget_id();
        if(id != -1){
            m_list->delete_at(id);
            m_resourcesKeys.erase(std::begin(m_resourcesKeys) + id);
            update_from_resources();
            trigger_ui_change();
        }
    });
}

ExResourcesListW *ExResourcesListW::init_widget(Resource::Type resourceType, QString title, bool enabled){

    w->setEnabled(enabled);
    m_resourceType = {resourceType};
    m_title->setText(title);

    m_icon->setText("");
    m_icon->setIcon(QIcon(from_view(Resource::get_icon_path(m_resourceType.value()))));
    m_icon->setIconSize(QSize(30,30));
    m_icon->setMinimumWidth(35);

    return this;
}


void ExResourcesListW::update_from_arg(const Arg &arg){

    ExItemW::update_from_arg(arg);

    w->blockSignals(true);

    if(arg.generator.has_value()){
        if(auto type = Resource::get_type(arg.generator->info->toStdString()); type.has_value()){
            m_resourceType = {type.value()};
            m_icon->setIcon(QIcon(from_view(Resource::get_icon_path(m_resourceType.value()))));
            m_title->setText(arg.generator->info.value());
        }
    }

    m_resourcesKeys.clear();
    m_list->delete_all();

    if(!m_resourceType.has_value()){
        qDebug() << "ExResourcesListW error: no type";
        return;
    }

    if(arg.value().size() > 0){

        auto split = arg.split_value();
        for(const auto &keyStr : split){

            int key = keyStr.toInt();
            if(auto resource = ExperimentManager::get()->current()->resM.get_resource(m_resourceType.value(), ResourceKey{key}); resource != nullptr){
                m_resourcesKeys.emplace_back(keyStr.toInt());
                m_list->add_widget(ui::W::txt(resource->alias));
            }
        }
    }

    update_from_resources();

    w->blockSignals(false);
}

Arg ExResourcesListW::convert_to_arg() const{

    Arg arg = ExBaseW::convert_to_arg();

    QStringList keysStr;
    keysStr.reserve(static_cast<int>(m_resourcesKeys.size()));
    for(const auto &key : m_resourcesKeys){
        keysStr << QString::number(key);
    }

    arg.init_from(keysStr, " ");
    if(hasGenerator){
        if(m_resourceType.has_value()){
            arg.generator->info = std::string(Resource::get_name(m_resourceType.value())).c_str();
        }
    }

    return arg;
}

void ExResourcesListW::update_from_resources(){

    m_resourcesAliases->blockSignals(true);


    QString currentText = m_resourcesAliases->currentText();
    m_resourcesAliases->clear();

    QStringList names;
    std::vector<size_t> widgetsToRemove;
    std::set<QString> inside;

    if(auto resources = ExperimentManager::get()->current()->resM.get_resources(m_resourceType.value()); resources.size() > 0){

        // remove resources keys not existing anymore
        std::vector<int> elemsToRemove;
        for(size_t ii = 0; ii < m_resourcesKeys.size(); ++ii){

            bool found = false;
            for(const auto &resource : resources){
                if(resource->key() == m_resourcesKeys[ii]){
                    found = true;
                    break;
                }
            }

            if(!found){
                elemsToRemove.push_back(static_cast<int>(ii));
            }
        }

        if(elemsToRemove.size() > 0){
            for(int ii = static_cast<int>(elemsToRemove.size()) -1; ii >= 0; --ii){
                m_list->delete_at(elemsToRemove[ii]);
                m_resourcesKeys.erase(std::begin(m_resourcesKeys) + elemsToRemove[ii]);
            }
        }

        // check for new resources
        for(const auto &resource : resources){

            bool found = false;
            for(size_t ii = 0; ii < m_resourcesKeys.size(); ++ii){

                if(resource->key() == m_resourcesKeys[ii]){
                    dynamic_cast<QLabel*>(m_list->widget_at(to_int(ii)))->setText(resource->alias);
                    found = true;
                    break;
                }
            }

            if(!found){
                names << resource->alias;
            }
        }
    }

    m_resourcesAliases->addItems(names);
    m_resourcesAliases->setCurrentText(currentText);

    m_resourcesAliases->blockSignals(false);
}

#include "moc_ex_resources_list_w.cpp"

