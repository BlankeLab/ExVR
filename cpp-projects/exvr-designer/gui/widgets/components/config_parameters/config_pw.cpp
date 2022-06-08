
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

#include "config_pw.hpp"

// local
#include "experiment/global_signals.hpp"
#include "resources/resources_manager.hpp"
#include "gui/ex_widgets/ex_resource_w.hpp"
#include "gui/ex_widgets/ex_component_w.hpp"

using namespace tool::ex;

ConfigParametersW::ConfigParametersW() {

    setContentsMargins(0,0,0,0);
    setLayout(m_layout = new QVBoxLayout());
    QScrollArea *area = new QScrollArea();
    area->setWidgetResizable(true);
    QWidget *w = new QWidget();
    w->setObjectName("ConfigW");
    w->setStyleSheet("QWidget#ConfigW { background-color: white }");

    auto scrollL = ui::L::VB();
    w->setLayout(scrollL);
    area->setWidget(w);
    layout()->addWidget(area);
    m_layout = scrollL;
}

void ConfigParametersW::set_infos(ComponentKey componentKey, ConfigKey configKey, bool initConfig){
    this->componentKey  = componentKey;
    this->configKey     = configKey;
    this->initConfig    = initConfig;
}

void ConfigParametersW::no_end_stretch(){
    endStretch = false;
}

void ConfigParametersW::end_stretch(){
    if(endStretch){
        qobject_cast<QVBoxLayout*>(m_layout)->addStretch();
    }
}

void ConfigParametersW::set_stretch(int id, int value){
    m_layout->setStretch(id, value);
}

void ConfigParametersW::add_widget(QWidget *w){
    m_layout->addWidget(w);
}

void ConfigParametersW::add_horizontal_widgets(std::vector<QWidget *> widgets, LMargins margins, QFrame::Shape shape, QFrame::Shadow shadow){
    add_widget(ui::F::gen(ui::L::HB(),widgets, LStretch{false}, margins, shape, shadow));
}

void ConfigParametersW::add_stretched_horizontal_widgets(std::vector<QWidget*> widgets, LMargins margins, QFrame::Shape shape, QFrame::Shadow shadow){
    add_widget(ui::F::gen(ui::L::HB(),widgets, LStretch{true}, margins, shape, shadow));
}

void ConfigParametersW::add_vertical_widgets(std::vector<QWidget *> widgets, LMargins margins, QFrame::Shape shape, QFrame::Shadow shadow){
    add_widget(ui::F::gen(ui::L::VB(),widgets, LStretch{false}, margins, shape, shadow));
}

void ConfigParametersW::add_stretched_vertical_widgets(std::vector<QWidget *> widgets, LMargins margins, QFrame::Shape shape, QFrame::Shadow shadow){
    add_widget(ui::F::gen(ui::L::VB(),widgets, LStretch{true}, margins, shape, shadow));
}

void ConfigParametersW::add_sub_part_widget(ConfigParametersSubPart &subPart){
    add_widget(subPart.frame);
}

void ConfigParametersW::map_sub_part(ConfigParametersSubPart *subPart){
    for(auto &w : subPart->inputUiElements){
        add_input_ui(w.second);
    }
}

void ConfigParametersW::init_from_args(std::map<QStringView,Arg> &args){

    // input ui
    for(const auto &inputUiElem : m_inputUiElements){

        QStringView itemName = inputUiElem.first;
        ExBaseW *exW         = inputUiElem.second;        

        // event filter
        exW->installEventFilter(this);

        if(args.count(itemName) != 0){
            // if argument exists, update ui from it
            exW->update_from_arg(args.at(itemName));
        }else{
            // else updates argument from ui
            emit GSignals::get()->new_arg_signal(componentKey, configKey, exW->convert_to_arg(), initConfig);
        }

        // init connections
        connect(exW, &ExBaseW::ui_change_signal, this, [=](QStringView uiName){
            static_cast<void>(uiName);
            emit GSignals::get()->arg_updated_signal(componentKey, configKey, exW->convert_to_arg(), initConfig);
        });

        // set tooltip
        exW->init_default_tooltip(exW->itemName);
    }

    // actions ui
    for(const auto &actionUiElem : m_actionUiElements){

        ExBaseW *exW     = actionUiElem.second;

        // event filter
        exW->installEventFilter(this);

        // init connections
        connect(exW, &ExBaseW::action_signal, this, [&](QStringView uiName){
            emit GSignals::get()->action_signal(componentKey, configKey, uiName, initConfig);
        });
    }

    // arg only
    for(const auto &inputNonUiArg : m_inputNonUiArguments){
        QStringView name = inputNonUiArg.first;
        if(args.count(name) == 0){
            emit GSignals::get()->new_arg_signal(componentKey, configKey, inputNonUiArg.second, initConfig);
        }
    }

    // update generators
    for(const auto &generatorUiElem : m_generatorsUiElements){

        ExParametersGeneratorWidgetW* generator = generatorUiElem.second;
        //QStringView itemName = generatorUiElem.first;

        connect(generator, &ExParametersGeneratorWidgetW::add_ui_signal, this, [this](Arg arg){
            emit GSignals::get()->new_arg_signal(componentKey, configKey, std::move(arg), initConfig);
        });
        connect(generator, &ExParametersGeneratorWidgetW::swap_ui_signal, this, [this](QStringView arg1Name, QStringView arg2Name){
            emit GSignals::get()->swap_arg_signal(componentKey, configKey, arg1Name, arg2Name, initConfig);
        });
        connect(generator, &ExParametersGeneratorWidgetW::remove_ui_signal, this, [this](QStringView argName){
            emit GSignals::get()->arg_removed_signal(componentKey, configKey, argName, initConfig);
        });
        connect(generator, &ExParametersGeneratorWidgetW::ui_change_signal, this, [this](QStringView argName){
            if(m_inputUiElements.count(argName) != 0){
                emit GSignals::get()->arg_updated_signal(componentKey, configKey, m_inputUiElements[argName]->convert_to_arg(), initConfig);
            }else{
                QtLogger::warning(QSL("ExParametersGeneratorWidgetW::ui_change_signal invalid name ") % argName);
            }
        });

        for(const auto &arg : args){
            // TODO do not work if more than one generator
            if(arg.second.generator.has_value()){
                generator->update_from_arg(arg.second);
            }
        }
    }
}

void ConfigParametersW::update_from_resources(){
    for(auto &ui : m_inputUiElements){
        ui.second->update_from_resources();
    }
}

void ConfigParametersW::update_from_components(){
    for(auto &ui : m_inputUiElements){
        ui.second->update_from_components();
    }
}

void ConfigParametersW::reset_args(){

    init_and_register_widgets();
    for(auto &ui : m_inputUiElements){
        emit GSignals::get()->arg_updated_signal(componentKey, configKey, ui.second->convert_to_arg(), initConfig);
    }
}

void ConfigParametersW::add_input_ui(ExBaseW *w){

    QStringView uiName = w->itemName;
    if(m_inputUiElements.count(uiName) == 0){
        m_inputUiElements[uiName] = w;
    }else{
        QtLogger::warning(QSL("ConfigParametersW input ui with name ") % uiName % QSL(" already exists."));
    }
}

void ConfigParametersW::add_inputs_ui(std::vector<ExBaseW *> widgets){
    for(const auto &w : widgets){
        add_input_ui(w);
    }
}

void ConfigParametersW::add_action_ui(ExBaseW *w){
    QStringView uiName = w->itemName;
    if(m_actionUiElements.count(uiName) == 0){
        m_actionUiElements[uiName] = w;
    }else{
        QtLogger::warning(QSL("ConfigParametersW action ui with name ") % uiName % QSL(" already exists."));
    }
}

void ConfigParametersW::add_non_ui_arg(Arg arg){

    QStringView nonUiName = arg.name;
    if(m_inputNonUiArguments.count(nonUiName) == 0){
        m_inputNonUiArguments[nonUiName] = std::move(arg);
    }else{
        QtLogger::warning(QSL("ConfigParametersW non ui arg with name ") % nonUiName % QSL(" already exists."));
    }
}

void ConfigParametersW::add_generator_ui(ExBaseW *g){
    QStringView uiName = g->itemName;
    if(m_generatorsUiElements.count(uiName) == 0){
        m_generatorsUiElements[uiName] = dynamic_cast<ExParametersGeneratorWidgetW*>(g);
    }else{
        QtLogger::warning(QSL("ConfigParametersW generator ui with name ") % g->itemName % QSL(" already exists."));
    }
}

