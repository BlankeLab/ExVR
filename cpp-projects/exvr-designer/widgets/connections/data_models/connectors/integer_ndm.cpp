
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


#include "integer_ndm.hpp"

// qt-utility
#include "qt_logger.hpp"

using namespace tool::ex;

void IntegerEmbeddedW::initialize(){

    // init widget
    w->init_widget(MinV<int>{-100000000}, V<int>{10}, MaxV<int>{100000000}, StepV<int>{1});

    // set widget connections
    connect(w.get(), &ExSpinBoxW::ui_change_signal, this, [=]{
        emit update_internal_data_signal({0}, {std::make_shared<IntData>(w->w->value())});
        emit compute_data_signal();
    });

    // add widget to ui
    add_row_in_dialog(QSL("Value: "), w->w.get());
}

void IntegerNodeDataModel::compute(){

    if(check_infinity_loop()){
        return;
    }

    auto inputs = get_inputs();

    // runtime inputs
    if(check_if_runtime_inputs(inputs)){
        propagate_default_runtime({std::make_shared<IntData>()});
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
    auto data1 = dcast<IntData>(inputs[0]);
    if(!data1){
        set_invalid_cast();
        return;
    }

    // propagate
    int value = data1->value();
    propagate_data(
        str::Convertor::to_str(value),
        {
            std::make_shared<IntData>(std::move(value))
        } );
}

void IntegerNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
    for(size_t ii = 0; ii < io.outNb; ++ii){
        outPortsInfo[ii].caption = QSL("out (") % get_name(io.outTypes[ii]) % QSL(")");
    }
}

#include "moc_integer_ndm.cpp"
