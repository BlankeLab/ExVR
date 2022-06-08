
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

#include "real_ndm.hpp"

// base
#include "utility/benchmark.hpp"

using namespace tool::ex;

void RealEmbeddedW::initialize(){

    // init widget
    w->init_widget(min, value, max, step, 3);

    // set widget connections
    connect(w.get(), &ExDoubleSpinBoxW::ui_change_signal, this, [=]{
        emit update_internal_data_signal({0}, {std::make_shared<RealData>(w->w->value())});
        emit compute_data_signal();
    });

    // add widget to ui
    add_row_in_dialog(QSL("Value: "), w->w.get());

}

void RealNodeDataModel::compute(){

    if(check_infinity_loop()){
        return;
    }

    auto inputs = get_inputs();

    // runtime inputs
    if(check_if_runtime_inputs(inputs)){
        propagate_default_runtime({std::make_shared<RealData>()});
        lock_embedded_widget();
        return;
    }

    // if no input data
    if(!inputs[0]){
        unlock_embedded_widget();
        inputs[0] = interData[0];
    }else{
        lock_embedded_widget();
    }

    // cast
    auto data1 = dcast<RealData>(inputs[0]);
    if(!data1){
        set_invalid_cast();
        return;
    }

    // propagate
    double value = data1->value();
    propagate_data(
        str::Convertor::to_str(value),{
            std::make_shared<RealData>(std::move(value))
        }
    );
}

void RealNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
    for(size_t ii = 0; ii < io.outNb; ++ii){
        outPortsInfo[ii].caption = QSL("out (") % get_name(io.outTypes[ii]) % QSL(")");
    }
}

void RandomRealNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    inPortsInfo[0].caption = QSL("min (") % get_name(io.inTypes[0]) % QSL(")");
    inPortsInfo[1].caption = QSL("max (") % get_name(io.inTypes[1]) % QSL(")");
    inPortsInfo[2].caption = QSL("generate (") % get_name(io.inTypes[2]) % QSL(")");
    outPortsInfo[0].caption = QSL("out (") % get_name(io.outTypes[0]) % QSL(")");
}


void RandomRealNodeDataModel::compute(){

    if(check_infinity_loop()){
        return;
    }

    auto inputs = get_inputs();
    if(!inputs[0] && !inputs[1]){
        set_invalid_state(QSL("Missing entrees #1,#2"));
    }else if(!inputs[0]){
        set_invalid_state(QSL("Missing entrees #1"));
    }else if(!inputs[1]){
        set_invalid_state(QSL("Missing entrees #2"));
    }else{
        set_valid_state();
    }

    propagate_default_runtime({std::make_shared<RealData>()});
}


#include "moc_real_ndm.cpp"


