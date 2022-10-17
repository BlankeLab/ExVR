
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

#include "ex_condition_w.hpp"

// local
#include "experiment/experiment.hpp"

using namespace tool::ex;

ExConditionW::ExConditionW(QString name) : ExItemW<QFrame>(UiType::Routine_condition, name){

    w->setFrameShadow(QFrame::Raised);
    w->setFrameShape(QFrame::Shape::NoFrame);

    QVBoxLayout *l = new QVBoxLayout();
    w->setLayout(l);

    l->addWidget(m_routineTitle = new QLabel("Routine:"));
    l->addWidget(m_routineNames = new QComboBox());

    l->addWidget(m_conditionTitle = new QLabel("Condition:"));
    l->addWidget(m_conditionNames = new QComboBox());

    l->addStretch();

    connect(m_routineNames, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&]{

        m_currentRoutineKey.v = -1;
        if(m_routineNames->currentIndex() > 0){
            auto id = m_routineNames->currentIndex()-1;
            if(auto routine = ExperimentManager::get()->current()->get_routine(RowId{id}); routine != nullptr){
                m_currentRoutineKey = routine->e_key();
            }
        }
        update_routines_list_widget();
        trigger_ui_change();
    });

    connect(m_conditionNames, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&]{

        m_currentConditionKey.v = -1;
        if(m_currentConditionKey.v != -1){
            if(m_conditionNames->currentIndex() > 0){
                const size_t id = to_unsigned(m_conditionNames->currentIndex()-1);
                if(auto currentRoutine = ExperimentManager::get()->current()->get_routine(m_currentRoutineKey); currentRoutine != nullptr){
                    m_currentConditionKey =  currentRoutine->conditions[id]->c_key();
                }
            }
        }
        trigger_ui_change();
    });
}

ExConditionW *ExConditionW::init_widget(bool enabled){
    w->setEnabled(enabled);
    return this;
}


void ExConditionW::update_from_arg(const Arg &arg){

    ExItemW::update_from_arg(arg);

    w->blockSignals(true);

    if(auto split = arg.split_value(); split.size() > 5){
        m_currentRoutineKey.v   = split[2].toInt();
        m_currentConditionKey.v = split[3].toInt();
    }

    update_from_components();

    w->blockSignals(false);
}

Arg ExConditionW::convert_to_arg() const{

    Arg arg = ExBaseW::convert_to_arg();
    arg.init_from({
          m_routineNames->currentText(),
          m_conditionNames->currentText(),
          QString::number(m_currentRoutineKey.v),
          QString::number(m_currentConditionKey.v)
      },
      "%%%"
                  );
    return arg;
}

void ExConditionW::update_from_components(){

    update_routines_list_widget();
    update_conditions_list_widget();

    m_routineNames->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_conditionNames->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    emit update_from_components_signal();
}

QString ExConditionW::display() const{
    return QSL("Rout: ") % m_routineNames->currentText()     % QSL("\n") %
           QSL("Cond: ") % m_conditionNames->currentText()   % QSL("\n");
}

void ExConditionW::update_routines_list_widget(){

    m_routineNames->blockSignals(true);
    auto exp = ExperimentManager::get()->current();

    // retrieve routines names list
    auto routinesNames = exp->get_routines_name();

    bool rebuildList = false;
    if(m_routineNames->count() != static_cast<int>((routinesNames.size()+1))){
        rebuildList = true;
    }

    if(!rebuildList){
        for(size_t ii = 0; ii < routinesNames.size(); ++ii){
            if(m_routineNames->itemText(static_cast<int>(ii)+1) != routinesNames[ii]){
                rebuildList = true;
                break;
            }
        }
    }

    if(rebuildList){

        // clear combobox
        m_routineNames->clear();
        m_routineNames->addItem("");

        QStringList componentsNamesList;
        for(auto routineN : routinesNames){
            componentsNamesList << routineN.toString();
        }

        if(componentsNamesList.size() > 0){
            m_routineNames->addItems(componentsNamesList);
        }
    }

    // check current index
    int id = 1;
    int newId = -1;

//    auto routines = exp->get_elements_of_type(FlowElement::Type::Routine);
//    for(auto routine : exp->get_elements_of_type(FlowElement::Type::Routine)){


//    }
//    for(auto routine : componentsM->get_components()){
//        if(component->key() == m_currentComponentKey.v){
//            newId = id;
//        }
//        ++id;
//    }

//    if(newId != -1){
//        m_componentNames->setCurrentIndex(newId);
//    }else{
//        m_currentComponentKey.v = -1;
//        if(m_componentNames->count() > 0){
//            m_componentNames->setCurrentIndex(0);
//        }
//    }

    m_routineNames->blockSignals(false);
}

void ExConditionW::update_conditions_list_widget(){

    m_conditionNames->blockSignals(true);

//    ComponentsManager *componentsM = &ExperimentManager::get()->current()->compM;

//    // check if no current component or no component
//    if(m_currentComponentKey.v == -1 || m_componentNames->count() == 1){
//        // clear combobox
//        if(m_configNames->count() != 1){
//            m_configNames->clear();
//            m_configNames->addItem("");
//        }
//        m_currentConfigKey.v = -1;
//        m_configNames->setCurrentIndex(0);
//        return;
//    }

//    // check if invalid current component
//    auto currentComponent = componentsM->get_component(m_currentComponentKey);
//    if(!currentComponent){
//        // clear combobox
//        if(m_configNames->count() != 1){
//            m_configNames->clear();
//            m_configNames->addItem("");
//        }
//        m_currentConfigKey.v = -1;
//        m_configNames->setCurrentIndex(0);
//        return;
//    }

//    // check if must rebuild config list
//    bool rebuildList = false;
//    if(m_configNames->count() != static_cast<int>((currentComponent->configs.size()+1))){
//        rebuildList = true;
//    }

//    if(!rebuildList){
//        for(size_t ii = 0; ii < currentComponent->configs.size(); ++ii){
//            if(m_configNames->itemText(static_cast<int>(ii)+1) != currentComponent->configs[ii]->name){
//                rebuildList = true;
//                break;
//            }
//        }
//    }

//    // rebuild combobox
//    if(rebuildList){

//        m_configNames->clear();

//        QStringList configNamesList;
//        configNamesList << "";
//        for(const auto &config : currentComponent->configs){
//            configNamesList << config->name;
//        }

//        if(configNamesList.size() > 0){
//            m_configNames->addItems(configNamesList);
//        }
//    }


//    // check current index
//    int id = 1;
//    int newId = -1;
//    for(const auto &config : currentComponent->configs){
//        if(config->key() == m_currentConfigKey.v){
//            newId = id;
//        }
//        ++id;
//    }

//    if(newId != -1){
//        m_configNames->setCurrentIndex(newId);
//    }else{
//        m_currentConfigKey.v = -1;
//        if(m_configNames->count() > 0){
//            m_configNames->setCurrentIndex(0);
//        }
//    }

    m_conditionNames->blockSignals(false);
}

#include "moc_ex_condition_w.cpp"
