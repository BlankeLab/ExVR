
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

#include "binary_operation_ndm.hpp"

using namespace tool::ex;

void BinaryOperationEmbeddedW::initialize(){

    // init widget
    w->init_widget({QSL("AND"), QSL("OR"), QSL("XOR"), QSL("NOT")});

    // set widget connections
    connect(w.get(), &ExComboBoxTextW::ui_change_signal, this, [=]{
        emit compute_data_signal();
    });

    // add widget to ui
    add_row_in_dialog(QSL("Operator: "), w->w.get());
}

void BinaryOperationNodeDataModel::compute(){

    m_caption = embedded_w()->w->w->currentText();

    const auto io = Connector::get_io(m_type);
    inPortsInfo[1].caption =  (embedded_w()->w->w->currentIndex() != 3) ?
            QSL("B (") % get_name(io.inTypes[1]) % QSL(")") : QSL("-");

    if(check_infinity_loop()){
        return;
    }

    auto inputs = get_inputs();
    const int index = embedded_w()->w->w->currentIndex();

    // missing inputs
    if(index != 3){
        if(check_if_missing_inputs(inputs)){
            return;
        }
    }else{
        if(!inputs[0]){
            set_invalid_state(QSL("Missing entree #1."));
            invalidate_data();
            return;
        }
    }

    // runtime inputs
    if(check_if_runtime_inputs(inputs)){
        set_valid_state();
        propagate_default_runtime({std::make_shared<BoolData>()});
        return;
    }

    // cast
    bool v1, v2;
    auto data1 = dcast<BoolData>(inputs[0]);
    auto data2 = dcast<BoolData>(inputs[1]);
    if(index != 3){
        if(!data1 || !data2){
            set_invalid_cast();
            return;
        }
        v1 = data1->value();
        v2 = data2->value();
    }else{
        if(!data1){
            set_invalid_cast();
            return;
        }
        v1 = data1->value();
    }

    // propagate
    bool state = false;
    if(index == 0){ // AND
        state = v1 && v2;
    }else if(index == 1){ // OR
        state = v1 || v2;
    }else if(index == 2){ // XOR
        state = v1 != v2;
    }else if(index == 3){ // NOT
        state = !v1;
    }

    propagate_data(
        str::Convertor::to_str(state),
        {
            std::make_shared<BoolData>(state)
                } );
}


void BinaryOperationNodeDataModel::init_ports_caption(){

    const auto io = Connector::get_io(m_type);

    inPortsInfo[0].caption = QSL("A (") % get_name(io.inTypes[0]) % QSL(")");
    inPortsInfo[1].caption = QSL("B (") % get_name(io.inTypes[1]) % QSL(")");

    for(size_t ii = 0; ii < io.outNb; ++ii){
        outPortsInfo[ii].caption = QSL("out (") % get_name(io.outTypes[ii]) % QSL(")");
    }
}




#include "moc_binary_operation_ndm.cpp"
