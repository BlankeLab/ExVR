
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


#include "flow_routine_ndm.hpp"

using namespace tool::ex;

void StartRoutineNodeDataModel::compute(){
    set_valid_state();
    propagate_default_runtime({std::make_shared<StringData>(""),std::make_shared<StringData>(""), std::make_shared<IntData>(0), std::make_shared<IntData>(0)});
}

void StartRoutineNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    outPortsInfo[0].caption = QSL("routine (") % get_name(io.outTypes[0]) % QSL(")");
    outPortsInfo[1].caption = QSL("condition (") % get_name(io.outTypes[1]) % QSL(")");
    outPortsInfo[2].caption = QSL("routine iter (") % get_name(io.outTypes[2]) % QSL(")");
    outPortsInfo[3].caption = QSL("condition iter (") % get_name(io.outTypes[3]) % QSL(")");
    outPortsInfo[4].caption = QSL("exp time ms (") % get_name(io.outTypes[4]) % QSL(")");
}

void StopRoutineNodeDataModel::compute(){
    set_valid_state();
    propagate_default_runtime({std::make_shared<StringData>(""),std::make_shared<StringData>("")});
}

void StopRoutineNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    outPortsInfo[0].caption = QSL("routine (") % get_name(io.outTypes[0]) % QSL(")");
    outPortsInfo[1].caption = QSL("condition (") % get_name(io.outTypes[1]) % QSL(")");
}

void PreUpdateRoutineNodeDataModel::compute(){
    set_valid_state();
    propagate_default_runtime({std::make_shared<RealData>(0)});
}

void PreUpdateRoutineNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    outPortsInfo[0].caption = QSL("routine time ms (") % get_name(io.outTypes[0]) % QSL(")");
}

void UpdateRoutineNodeDataModel::compute(){
    set_valid_state();
    propagate_default_runtime({std::make_shared<RealData>(0)});
}

void UpdateRoutineNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    outPortsInfo[0].caption = QSL("routine time ms (") % get_name(io.outTypes[0]) % QSL(")");
}

void PostUpdateRoutineNodeDataModel::compute(){
    set_valid_state();
    propagate_default_runtime({std::make_shared<RealData>(0)});
}

void PostUpdateRoutineNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    outPortsInfo[0].caption = QSL("routine time ms (") % get_name(io.outTypes[0]) % QSL(")");
}

void RoutineConditionEmbeddedW::initialize(){
    // init widget
    w->init_widget("default name");

    // set widget connections
    connect(w.get(), &ExLineEditW::ui_change_signal, this, [=]{
        emit update_internal_data_signal({0}, {std::make_shared<StringData>(w->w->text())});
        emit compute_data_signal();
    });

    // add widget to ui
    add_row_in_dialog(QSL("Routine name: "), w->w.get());
}

void RoutineConditionNodeDataModel::compute(){

    if(check_infinity_loop()){
        return;
    }

    auto inputs = get_inputs();
    if(!inputs[0]){
        set_invalid_state(QSL("Missing entree #1."));
    }else{
        set_valid_state();
    }

    set_embedded_widget_text(dcast<StringData>(interData[0])->value());
    propagate_default_runtime({std::make_shared<StringData>()});
}

void RoutineConditionNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    inPortsInfo[0].caption  = QSL("id (") % get_name(io.inTypes[0]) % QSL(")");
    outPortsInfo[0].caption = QSL("condition (") % get_name(io.outTypes[0]) % QSL(")");
}

#include "moc_flow_routine_ndm.cpp"

