
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

#include "decimal_trigo_ndm.hpp"

using namespace tool::ex;

void DecimalTrigoEmbeddedW::initialize(){

    // init widget
    w->init_widget({QSL("Cos"), QSL("Sin"), QSL("Tan"), QSL("Acos"),
                    QSL("Asin"), QSL("Atan"), QSL("Cosh"), QSL("Sinh"), QSL("Tanh")});

    // set widget connections
    connect(w.get(), &ExComboBoxTextW::ui_change_signal, this, [=]{
        emit compute_data_signal();
    });

    // add widget to ui
    add_row_in_dialog(QSL("Operator"), w->w.get());
}

void DecimalTrigoNodeDataModel::compute(){

    m_caption = embedded_w()->w->w->currentText();

    if(check_infinity_loop()){
        return;
    }

    auto inputs = get_inputs();

    if(!inputs[0]){
        set_invalid_state(QSL("Missing 1 entree."));
        invalidate_data();
        return;
    }

    // runtime inputs
    if(is_runtime(inputs[0]) ){
        set_valid_state();
        propagate_default_runtime({std::make_shared<DecimalData>()});
        return;
    }

    // cast
    auto data1 = dcast<DecimalData>(inputs[0]);
    if(!data1){
        set_invalid_cast();
        return;
    }

    // propagate
    const int index = embedded_w()->w->w->currentIndex();
    const auto v1 = data1->value();

    Decimal dec;
    if(index == 0){ // cos
        dec = Decimal::cos(v1);
    }else if(index == 1){ // sin
        dec = Decimal::sin(v1);
    }else if(index == 2){ // tan
        dec = Decimal::tan(v1);
    }else if(index == 3){ // acos
        dec = Decimal::acos(v1);
    }else if(index == 4){ // asin
        dec = Decimal::asin(v1);
    }else if(index == 5){ // atan
        dec = Decimal::atan(v1);
    }else if(index == 6){ // cosh
        dec = Decimal::cosh(v1);
    }else if(index == 7){ // sinh
        dec = Decimal::sinh(v1);
    }else if(index == 8){ // tanh
        dec = Decimal::tanh(v1);
    }

    propagate_data(
        str::Convertor::to_str(dec),
        {
            std::make_shared<DecimalData>(std::move(dec))
        } );
}

void DecimalTrigoNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
    for(size_t ii = 0; ii < io.outNb; ++ii){
        outPortsInfo[ii].caption = QSL("out (") % get_name(io.inTypes[ii]) % QSL(")");
    }
}

#include "moc_decimal_trigo_ndm.cpp"
