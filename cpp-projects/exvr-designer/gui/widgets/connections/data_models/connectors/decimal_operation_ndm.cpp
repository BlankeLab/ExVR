
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

#include "decimal_operation_ndm.hpp"

using namespace tool::ex;

void DecimalOperationEmbeddedW::initialize(){

    // init widget
    QStringList items = {QSL("  Add  "), QSL("Substract"), QSL("Multiply"), QSL("Divide"),
                         QSL("Modulo"), QSL("Inferior?"), QSL("Inferior or equal?"),
                         QSL("Superior?"), QSL("Superior or equal?"), QSL(" Equal? "), QSL("Not equal?")};
    w->init_widget(items);

    // set widget connections
    connect(w.get(), &ExComboBoxTextW::ui_change_signal, this, [=]{
        emit compute_data_signal();
    });

    // add widget to ui
    add_row_in_dialog(QSL("Operator: "), w->w.get());
}

void DecimalOperationNodeDataModel::compute(){

    const int index = embedded_w()->w->w->currentIndex();
    m_caption = embedded_w()->w->w->currentText();

    const auto typeOut = (embedded_w()->w->w->currentIndex() < 5) ? DecimalData().type() : BoolData().type();
    for(size_t ii = 0; ii < outPortsInfo.size(); ++ii){
        outPortsInfo[ii].type = typeOut;
    }
    outPortsInfo[0].caption = (embedded_w()->w->w->currentIndex() < 5) ?
        QSL("out (") % from_view(ConnectionNode::get_name(ConnectionNode::Type::decimal_t)) % QSL(")") :
        QSL("out (") % from_view(ConnectionNode::get_name(ConnectionNode::Type::boolean_t)) % QSL(")");


    if(check_infinity_loop()){
        return;
    }

    auto inputs = get_inputs();
    if(!inputs[0] && !inputs[1]){
        set_invalid_state(QSL("Missing 2 entrees."));
        invalidate_data();
        return;
    }

    if(!inputs[0] || !inputs[1]){
        set_invalid_state(QSL("Missing 1 entrees."));
        invalidate_data();
        return;
    }

    // runtime inputs
    if(is_runtime(inputs[0]) || is_runtime(inputs[1])){
        set_valid_state();
        runtime_value_text();
        if(embedded_w()->w->w->currentIndex() < 5){
            propagate_default_runtime({std::make_shared<DecimalData>()});
        }else{
            propagate_default_runtime({std::make_shared<BoolData>()});
        }
        return;
    }

    // cast
    auto data1 = dcast<DecimalData>(inputs[0]);
    auto data2 = dcast<DecimalData>(inputs[1]);
    if(!data1 || !data2){
        invalidate_data();
        set_error_state(QSL("Invalid cast."));
        return;
    }

    auto v1 = data1->value();
    auto v2 = data1->value();

    std::variant<Decimal, bool> out;

    if(index == 0){ // Add
        out = add(v1,v2);
    }else if(index == 1){ // Sub
        out = sub(v1,v2);
    }else if(index == 2){ // Mult
        out = mul(v1,v2);
    }else if(index == 3){ // Div
         if(almost_equal<double>(v2.to_double(), 0.)){
             invalidate_data();
            set_invalid_state(QSL("B is null"));
            return;
         }else{
            out = div(v1,v2);
         }
    }else if(index == 4){ // mod
        if(almost_equal<double>(v2.to_double(), 0.)){
            invalidate_data();
            set_invalid_state(QSL("B is null"));
            return;
        }else{
            out = mod(v1,v2);
        }
    }else if(index == 5){ // <
        out = inferior(v1,v2);
    }else if(index == 6){ // <=
        out = inferior_or_equal(v1,v2);
    }else if(index == 7){ // >
        out = superior(v1,v2);
    }else if(index == 8){ // >=
        out = superior_or_equal(v1,v2);
    }else if(index == 9){ // =
        out = equal(v1,v2);
    }else{ // !=
        out = no_equal(v1,v2);
    }

    if(auto pval = std::get_if<bool>(&out)){

        propagate_data(
            str::Convertor::to_str(*pval),
            {
                std::make_shared<BoolData>(std::move(*pval))
            } );

    }else if(auto pval = std::get_if<Decimal>(&out)){

        propagate_data(
            str::Convertor::to_str(*pval),
            {
                std::make_shared<DecimalData>(std::move(*pval))
            } );
    }
}

void DecimalOperationNodeDataModel::init_ports_caption(){

    const auto io = Connector::get_io(m_type);
    inPortsInfo[0].caption = QSL("A (") % get_name(io.inTypes[0]) % QSL(")");
    inPortsInfo[1].caption = QSL("B (") % get_name(io.inTypes[1]) % QSL(")");
    outPortsInfo[0].caption = QSL("out (") % from_view(ConnectionNode::get_name(ConnectionNode::Type::decimal_t)) % QSL(")");
}


#include "moc_decimal_operation_ndm.cpp"
