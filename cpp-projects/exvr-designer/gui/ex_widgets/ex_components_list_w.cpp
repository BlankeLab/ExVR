
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

#include "ex_components_list_w.hpp"

// local
#include "experiment/experiment.hpp"


using namespace tool::ex;

ExComponentsListW::ExComponentsListW(QString name) : ExItemW<QFrame>(UiType::ComponentsList, name){

    w->setFrameShadow(QFrame::Raised);
    w->setFrameShape(QFrame::Shape::NoFrame);

    auto f1 = ui::F::gen_frame(ui::L::HB(), {
        {m_icon     = new QPushButton(),1},
        {m_title    = new QLabel(), 1},
        {m_componentNames = new QComboBox(), 50},
        {m_add      = new QPushButton(QSL("Add")), 1},
        {m_remove   = new QPushButton(QSL("Rem")), 1}},
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

        if(m_componentNames->count() == 0){
            return;
        }

        const auto currTxt = m_componentNames->currentText();
        if(auto component = ExperimentManager::get()->current()->compM.get_component(currTxt); component != nullptr){
            m_componentsKeys.emplace_back(component->key());
            m_list->add_widget(ui::W::txt(component->name()));
            update_from_components();
            trigger_ui_change();
        }

    });
    connect(m_remove, &QPushButton::clicked, this, [&]{
        const int id = m_list->current_selected_widget_id();
        if(id != -1){
            m_list->delete_at(id);
            m_componentsKeys.erase(std::begin(m_componentsKeys) + id);
            update_from_components();
            trigger_ui_change();
        }
    });
}

ExComponentsListW *ExComponentsListW::init_widget(Component::Type componentType, QString title, bool enabled){

    w->setEnabled(enabled);
    m_componentType = {componentType};
    m_title->setText(title);

    m_icon->setText("");
    m_icon->setIcon(QIcon(Component::get_icon_path(m_componentType.value())));
    m_icon->setIconSize(QSize(30,30));
    m_icon->setMinimumWidth(35);

    return this;
}

ExComponentsListW *ExComponentsListW::init_widget(Component::Category componentCategory, bool canBeLogged, QString title, bool enabled){

    w->setEnabled(enabled);
    m_componentCategory = {componentCategory};
    m_canBeLogged = canBeLogged;
    m_title->setText(title);

    // TODO: add category iconds
    m_icon->setText("");
//    m_icon->setIcon(QIcon(Component::get_icon_path(m_componentType.value())));
    m_icon->setIconSize(QSize(30,30));
    m_icon->setMinimumWidth(35);

    return this;
}


void ExComponentsListW::update_from_arg(const Arg &arg){

    ExItemW::update_from_arg(arg);

    w->blockSignals(true);

    if(arg.generator.has_value()){
        if(auto type = Component::get_type_from_name(arg.generator->info->toStdString()); type.has_value()){
            m_componentType = {type.value()};            
            m_icon->setIcon(QIcon(Component::get_icon_path(m_componentType.value())));
            m_title->setText(arg.generator->info.value());
        } else if(auto category = Component::get_category(arg.generator->info->toStdString()); category.has_value()){
            m_componentCategory = {category.value()};
            //m_icon->setIcon(QIcon(Component::get_icon_path(m_componentType.value())));
            m_title->setText(arg.generator->info.value());
        }
    }

    m_componentsKeys.clear();
    m_list->delete_all();

    if(arg.value().size() > 0){

        auto split = arg.split_value();
        for(const auto &keyStr : split){

            int key = keyStr.toInt();
            if(auto component = ExperimentManager::get()->current()->compM.get_component(ComponentKey{key}); component != nullptr){
                m_componentsKeys.emplace_back(keyStr.toInt());
                m_list->add_widget(ui::W::txt(component->name()));
            }
        }
    }

    update_from_components();

    w->blockSignals(false);
}

Arg ExComponentsListW::convert_to_arg() const{

    Arg arg = ExBaseW::convert_to_arg();

    QStringList keysStr;
    keysStr.reserve(static_cast<int>(m_componentsKeys.size()));
    for(const auto &key : m_componentsKeys){
        keysStr << QString::number(key);
    }

    arg.init_from(keysStr, " ");
    if(hasGenerator){
        if(m_componentType.has_value()){
            arg.generator->info = from_view(Component::get_full_name(m_componentType.value()));
        }
        if(m_componentCategory.has_value()){
            arg.generator->info = from_view(Component::get_display_name(m_componentCategory.value()));
        }
    }

    return arg;
}

void ExComponentsListW::update_from_components(){

    m_componentNames->blockSignals(true);

    QString currentText = m_componentNames->currentText();
    m_componentNames->clear();

    QStringList names;


    std::vector<Component *> components;
    if(m_componentType.has_value()){
        components = ExperimentManager::get()->current()->compM.get_components(m_componentType.value());
    }else if(m_componentCategory.has_value()){
        components = ExperimentManager::get()->current()->compM.get_components(m_componentCategory.value(), m_canBeLogged);
    }


    // remove component keys not existing anymore
    std::vector<int> elemsToRemove;
    for(size_t ii = 0; ii < m_componentsKeys.size(); ++ii){

        bool found = false;
        for(const auto &component : components){
            if(component->key() == m_componentsKeys[ii]){
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
            m_componentsKeys.erase(std::begin(m_componentsKeys) + elemsToRemove[ii]);
        }
    }

    // check for new components
    for(const auto &component : components){

        bool found = false;
        for(size_t ii = 0; ii < m_componentsKeys.size(); ++ii){

            if(component->key() == m_componentsKeys[ii]){
                dynamic_cast<QLabel*>(m_list->widget_at(to_int(ii)))->setText(component->name());
                found = true;
                break;
            }
        }

        if(!found){
            names << component->name();
        }
    }

    m_componentNames->addItems(names);
    m_componentNames->setCurrentText(currentText);

    m_componentNames->blockSignals(false);
}

#include "moc_ex_components_list_w.cpp"

