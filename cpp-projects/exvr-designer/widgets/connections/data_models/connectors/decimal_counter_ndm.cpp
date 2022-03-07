
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


#include "decimal_counter_ndm.hpp"

using namespace tool::ex;


void DecimalCounterConfigNodeDataModel::compute(){

    if(check_infinity_loop()){
        return;
    }

    auto inputs = get_inputs();
    if(!inputs[0] && !inputs[1]){
        set_invalid_state(QSL("Missing 1 entree."));
        unknow_value_text();
        invalidate_data();
        return;
    }

    set_valid_state();
    runtime_value_text();
    propagate_default_runtime({std::make_shared<DecimalData>(Decimal(0))});
}

void DecimalCounterConfigNodeDataModel::init_ports_caption(){

    const auto io = Connector::get_io(m_type);
    inPortsInfo[0].caption = QSL("add (") % get_name(io.inTypes[0]) % QSL(")");
    inPortsInfo[1].caption = QSL("sub (") % get_name(io.inTypes[1]) % QSL(")");
    inPortsInfo[2].caption = QSL("reset (") % get_name(io.inTypes[2]) % QSL(")");

    outPortsInfo[0].caption = QSL("count (") % get_name(io.outTypes[0]) % QSL(")");
}


#include "moc_decimal_counter_ndm.cpp"
