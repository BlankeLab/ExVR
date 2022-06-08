
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

#include "boolean_ndm.hpp"

using namespace tool::ex;

void BooleanEmbeddedW::initialize() {

    // init widget
    w->init_widget("", true);

    // set widget connections
    connect(w.get(), &ExCheckBoxW::ui_change_signal, this, [=]{
        emit update_internal_data_signal({0}, {std::make_shared<BoolData>(w->w->isChecked())});
        emit compute_data_signal();
    });

    // add widget to ui
    add_row_in_dialog(QSL("Value: "), w->w.get());
}

void BooleanNodeDataModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex port){

    if(port == 0){ // value
        inputData[0] = nodeData;
    }else{ // trigger
        inputData[1] = nodeData;
    }

    compute();
}

void BooleanNodeDataModel::input_connection_created(const QtNodes::Connection &c){
    int id = c.getPortIndex(QtNodes::PortType::In);
    if(id == 0){
        lock_embedded_widget();
    }
}

void BooleanNodeDataModel::input_connection_deleted(const QtNodes::Connection &c){
    int id = c.getPortIndex(QtNodes::PortType::In);
    if(id == 0){
        unlock_embedded_widget();
    }
}

void BooleanNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);

    inPortsInfo[0].caption = QSL("in (") % get_name(io.inTypes[0]) % QSL(")");
    inPortsInfo[1].caption = QSL("toggle (") % get_name(io.inTypes[1]) % QSL(")");

    for(size_t ii = 0; ii < io.outNb; ++ii){
        outPortsInfo[ii].caption = QSL("out (") % get_name(io.outTypes[ii]) % QSL(")");
    }
}

void BooleanNodeDataModel::compute(){

    if(check_infinity_loop()){
        return;
    }

    auto inputs = get_inputs();

    // runtime inputs
    if(check_if_runtime_inputs(inputs)){
        propagate_default_runtime({std::make_shared<BoolData>()});
        return;
    }

    // if no input data
    if(!inputs[0]){
        inputs[0] = interData[0];
    }

    // cast
    auto data1 = dcast<BoolData>(inputs[0]);
    if(!data1){
        set_invalid_cast();
        return;
    }

    // propagate
    bool value = data1->value();
    propagate_data(
        str::Convertor::to_str(value),
        {
            std::make_shared<BoolData>(std::move(value))
        } );
}


#include "moc_boolean_ndm.cpp"

