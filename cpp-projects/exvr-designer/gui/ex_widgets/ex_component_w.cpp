
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

#include "ex_component_w.hpp"

// local
#include "experiment/experiment.hpp"

using namespace tool::ex;

ExComponentW::ExComponentW(QString name) : ExItemW<QFrame>(UiType::Component, name){

    w->setFrameShadow(QFrame::Raised);
    w->setFrameShape(QFrame::Shape::NoFrame);

    QHBoxLayout *l = new QHBoxLayout();
    w->setLayout(l);
    l->addWidget(m_icon = new QPushButton());
    l->addWidget(m_title = new QLabel());
    l->addWidget(m_componentNames = new QComboBox());
    l->setStretch(0,1);
    l->setStretch(1,1);
    l->setStretch(2,50);
    l->setContentsMargins(2,2,2,2);
    l->setSpacing(5);

    connect(m_componentNames, QOverload<int>::of(&QComboBox::currentIndexChanged), this,[&](int index){

        m_currentKey = {-1};

        if(index > 0){
            const size_t id = to_unsigned(index-1);

            if(auto components = ExperimentManager::get()->current()->compM.get_components(m_componentType.value()); id < components.size()){
                m_currentKey =  components[id]->c_key();
            }
        }
        trigger_ui_change();
    });
}

ExComponentW *ExComponentW::init_widget(tool::ex::Component::Type componentType, QString title, bool enabled){

    w->setEnabled(enabled);
    m_componentType = std::make_optional(componentType);
    m_title->setText(title);

    m_icon->setText("");
    m_icon->setIcon(QIcon(from_view(Component::get_icon_path(m_componentType.value()))));
    m_icon->setIconSize(QSize(30,30));
    m_icon->setMinimumWidth(35);

    return this;
}


void ExComponentW::update_from_arg(const Arg &arg){

    ExItemW::update_from_arg(arg);

    w->blockSignals(true);

    if(arg.generator.has_value()){
        if(auto type = Component::get_type_from_name(arg.generator->info.value().toStdString()); type.has_value()){
            m_componentType = type;
            m_icon->setIcon(QIcon(from_view(Component::get_icon_path(m_componentType.value()))));
            m_title->setText(arg.generator->info.value());
        }
    }

    if(auto split = arg.split_value(); split.size() > 1){
        m_currentKey.v = split[1].toInt();
    }
    update_from_components();

    w->blockSignals(false);
}

Arg ExComponentW::convert_to_arg() const{

    Arg arg = ExBaseW::convert_to_arg();
    arg.init_from({m_componentNames->currentText(),QString::number(m_currentKey.v)}, "%%%");

    if(hasGenerator){
        if(m_componentType.has_value()){
            arg.generator->info = from_view(Component::get_full_name(m_componentType.value()));
        }
    }

    return arg;
}

void ExComponentW::update_from_components(){

    m_componentNames->blockSignals(true);

    // retrieve components names list
    auto components = ExperimentManager::get()->current()->compM.get_components(m_componentType.value());

    bool rebuildList = false;
    if(m_componentNames->count() != static_cast<int>((components.size()+1))){
        rebuildList = true;
    }

    if(!rebuildList){
        for(size_t ii = 0; ii < components.size(); ++ii){
            if(m_componentNames->itemText(static_cast<int>(ii)+1) != components[ii]->name()){
                rebuildList = true;
                break;
            }
        }
    }

    if(rebuildList){

        // clear combobox
        m_componentNames->clear();
        m_componentNames->addItem("");

        QStringList componentsNamesList;
        for(const auto component : components){
            componentsNamesList << component->name();
        }

        if(componentsNamesList.size() > 0){
            m_componentNames->addItems(componentsNamesList);
        }
    }

    // check current index
    int id = 1;
    int newId = -1;
    for(const auto &component : components){
        if(component->c_key() == m_currentKey){
            newId = id;
        }
        ++id;
    }

    if(newId != -1){
        m_componentNames->setCurrentIndex(newId);
    }else{
        m_currentKey = {-1};
        if(m_componentNames->count() > 0){
            m_componentNames->setCurrentIndex(0);
        }
    }

    m_componentNames->blockSignals(false);
}

QString ExComponentW::current_component_name_displayed() const{
    return m_componentNames->currentText();
}
