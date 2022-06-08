
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

#include "ex_config_w.hpp"

// local
#include "experiment/experiment.hpp"

using namespace tool::ex;

ExConfigW::ExConfigW(QString name) : ExItemW<QFrame>(UiType::Component_config, name){

    w->setFrameShadow(QFrame::Raised);
    w->setFrameShape(QFrame::Shape::NoFrame);

    QVBoxLayout *l = new QVBoxLayout();
    w->setLayout(l);


    l->addWidget(m_routineTitle = new QLabel("Routine:"));
    l->addWidget(m_routineName = new QLineEdit());
//    l->addWidget(m_routineNames = new QComboBox());
    l->addWidget(m_conditionTitle = new QLabel("Condition:"));
    l->addWidget(m_conditionName = new QLineEdit());
//    l->addWidget(m_conditionNames = new QComboBox());
    l->addWidget(m_componentTitle = new QLabel("Component:"));
    l->addWidget(m_componentNames = new QComboBox());    
    l->addWidget(m_configTitle = new QLabel("Config:"));
    l->addWidget(m_configNames = new QComboBox());
    l->addStretch();


    //    connect(m_routineNames, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&,nameParam]{

    //        m_currentRoutineKey = -1;
    //        if(m_routineNames->currentIndex() > 0){
    //            const size_t id = to_unsigned(m_routineNames->currentIndex()-1);
    ////            ComponentsManager *componentsM = ComponentsManager::get();
    ////            m_currentComponentKey = componentsM->components[id]->key();
    //        }
    //        //        update_configs_list_widget();
    //        //        emit ui_change_signal(nameParam);
    //    });

    //    connect(m_conditionNames, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&,nameParam]{

    //        //        m_currentComponentKey = -1;
    //        //        if(m_componentNames->currentIndex() > 0){
    //        //            const size_t id = to_unsigned(m_componentNames->currentIndex()-1);
    //        //            ComponentsManager *componentsM = ComponentsManager::get();
    //        //            m_currentComponentKey = componentsM->components[id]->key();
    //        //        }
    //        //        update_configs_list_widget();
    //        //        emit ui_change_signal(nameParam);
    //    });


    connect(m_componentNames, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&]{

        m_currentComponentKey.v = -1;
        if(m_componentNames->currentIndex() > 0){
            const size_t id = to_unsigned(m_componentNames->currentIndex()-1);
            m_currentComponentKey.v = ExperimentManager::get()->current()->compM.get_components()[id]->key();
        }
        update_configs_list_widget();
        trigger_ui_change();
    });

    connect(m_configNames, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&]{

        m_currentConfigKey.v = -1;
        if(m_currentComponentKey.v != -1){
            if(m_configNames->currentIndex() > 0){
                const size_t id = to_unsigned(m_configNames->currentIndex()-1);
                if(auto currentComponent = ExperimentManager::get()->current()->compM.get_component(m_currentComponentKey); currentComponent != nullptr){
                    m_currentConfigKey.v = currentComponent->configs[id]->key();
                }
            }
        }
        trigger_ui_change();
    });

    connect(m_routineName, &QLineEdit::textEdited, this, [&]{
        m_currentRoutineName = m_routineName->text();
        trigger_ui_change();
    });
    connect(m_conditionName, &QLineEdit::textEdited, this, [&]{
        m_currentConditionName = m_conditionName->text();
        trigger_ui_change();
    });
}

ExConfigW *ExConfigW::init_widget(bool enabled){
    w->setEnabled(enabled);
    return this;
}


void ExConfigW::update_from_arg(const Arg &arg){

    ExItemW::update_from_arg(arg);

    w->blockSignals(true);

    if(auto split = arg.split_value(); split.size() > 5){
        m_currentRoutineName  = split[2];
        m_currentConditionName= split[3];
        m_currentComponentKey.v = split[4].toInt();
        m_currentConfigKey.v    = split[5].toInt();
    }

    update_from_components();

    w->blockSignals(false);
}

Arg ExConfigW::convert_to_arg() const{

    Arg arg = ExBaseW::convert_to_arg();
    arg.init_from({
            m_componentNames->currentText(),
            m_configNames->currentText(),
            m_currentRoutineName,
            m_currentConditionName,
            QString::number(m_currentComponentKey.v),
            QString::number(m_currentConfigKey.v)
        },
        "%%%"
    );
    return arg;
}

void ExConfigW::update_from_components(){

    m_routineName->blockSignals(true);
        m_routineName->setText(m_currentRoutineName);
    m_routineName->blockSignals(false);

    m_conditionName->blockSignals(true);
        m_conditionName->setText(m_currentConditionName);
    m_conditionName->blockSignals(false);

    update_components_list_widget();
    update_configs_list_widget();

    m_componentNames->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_configNames->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    emit update_from_components_signal();
}

QString ExConfigW::display() const{
//    return QSL("Rout: <b>") % m_currentRoutineName              % QSL("</b><br>") %
//           QSL("Cond: <b>") % m_currentConditionName            % QSL("</b><br>") %
//           QSL("Comp: <b>") % m_componentNames->currentText()   % QSL("</b><br>") %
//           QSL("Conf: <b>") % m_configNames->currentText()      % QSL("</b>");
    return QSL("Rout: ") % m_currentRoutineName              % QSL("\n") %
           QSL("Cond: ") % m_currentConditionName            % QSL("\n") %
           QSL("Comp: ") % m_componentNames->currentText()   % QSL("\n") %
           QSL("Conf: ") % m_configNames->currentText()      % QSL("");
}

void ExConfigW::update_components_list_widget(){

    m_componentNames->blockSignals(true);

    // retrieve components names list
    ComponentsManager *componentsM = &ExperimentManager::get()->current()->compM;

    bool rebuildList = false;
    if(m_componentNames->count() != static_cast<int>((componentsM->count()+1))){
        rebuildList = true;
    }

    if(!rebuildList){
        for(size_t ii = 0; ii < componentsM->count(); ++ii){
            if(m_componentNames->itemText(static_cast<int>(ii)+1) != componentsM->get_components()[ii]->name()){
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
        for(auto component : componentsM->get_components()){
            componentsNamesList << component->name();
        }

        if(componentsNamesList.size() > 0){
            m_componentNames->addItems(componentsNamesList);
        }
    }

    // check current index
    int id = 1;
    int newId = -1;
    for(auto component : componentsM->get_components()){
        if(component->key() == m_currentComponentKey.v){
            newId = id;
        }
        ++id;
    }

    if(newId != -1){
        m_componentNames->setCurrentIndex(newId);
    }else{
        m_currentComponentKey.v = -1;
        if(m_componentNames->count() > 0){
            m_componentNames->setCurrentIndex(0);
        }
    }

    m_componentNames->blockSignals(false);
}

void ExConfigW::update_configs_list_widget(){

    m_configNames->blockSignals(true);

    ComponentsManager *componentsM = &ExperimentManager::get()->current()->compM;

    // check if no current component or no component
    if(m_currentComponentKey.v == -1 || m_componentNames->count() == 1){
        // clear combobox
        if(m_configNames->count() != 1){
            m_configNames->clear();
            m_configNames->addItem("");
        }
        m_currentConfigKey.v = -1;
        m_configNames->setCurrentIndex(0);
        return;
    }

    // check if invalid current component
    auto currentComponent = componentsM->get_component(m_currentComponentKey);
    if(!currentComponent){
        // clear combobox
        if(m_configNames->count() != 1){
            m_configNames->clear();
            m_configNames->addItem("");
        }
        m_currentConfigKey.v = -1;
        m_configNames->setCurrentIndex(0);
        return;
    }

    // check if must rebuild config list
    bool rebuildList = false;
    if(m_configNames->count() != static_cast<int>((currentComponent->configs.size()+1))){
        rebuildList = true;
    }

    if(!rebuildList){
        for(size_t ii = 0; ii < currentComponent->configs.size(); ++ii){
            if(m_configNames->itemText(static_cast<int>(ii)+1) != currentComponent->configs[ii]->name){
                rebuildList = true;
                break;
            }
        }
    }

    // rebuild combobox
    if(rebuildList){

        m_configNames->clear();

        QStringList configNamesList;
        configNamesList << "";
        for(const auto &config : currentComponent->configs){
            configNamesList << config->name;
        }

        if(configNamesList.size() > 0){
            m_configNames->addItems(configNamesList);
        }
    }


    // check current index
    int id = 1;
    int newId = -1;
    for(const auto &config : currentComponent->configs){
        if(config->key() == m_currentConfigKey.v){
            newId = id;
        }
        ++id;
    }

    if(newId != -1){
        m_configNames->setCurrentIndex(newId);
    }else{
        m_currentConfigKey.v = -1;
        if(m_configNames->count() > 0){
            m_configNames->setCurrentIndex(0);
        }
    }

    m_configNames->blockSignals(false);
}

#include "moc_ex_config_w.cpp"
