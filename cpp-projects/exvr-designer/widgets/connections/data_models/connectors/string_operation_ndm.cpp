
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

#include "string_operation_ndm.hpp"

using namespace tool::ex;

void StringOperationEmbeddedW::initialize(){

    // init widget
    w->init_widget({QSL("Add"), QSL("Remove"), QSL("Contains"), QSL("Do not contains"),
                    QSL("Equals"), QSL("Do not equals"), QSL("Starts with"), QSL("Ends with"), QSL("Splits with")});

    // set widget connections
    connect(w.get(), &ExComboBoxTextW::ui_change_signal, this, [=]{
        emit compute_data_signal();
    });

    // add widget to ui
    add_row_in_dialog(QSL("Operator: "), w->w.get());
}

void StringOperationNodeDataModel::compute(){

    m_caption = embedded_w()->w->w->currentText();

    auto cId = embedded_w()->w->w->currentIndex();
    if(cId < 2){
        outPortsInfo[0].caption = QSL("out (") % from_view(ConnectionNode::get_name(ConnectionNode::Type::string_t)) % QSL(")");
        outPortsInfo[0].type = StringData().type();
    }else if(cId < 8){
        outPortsInfo[0].caption = QSL("out (") % from_view(ConnectionNode::get_name(ConnectionNode::Type::boolean_t)) % QSL(")");
        outPortsInfo[0].type = BoolData().type();
    }else{
        outPortsInfo[0].caption = QSL("out (") % from_view(ConnectionNode::get_name(ConnectionNode::Type::string_list_t)) % QSL(")");
        outPortsInfo[0].type = StringListData().type();
    }

    if(check_infinity_loop()){
        return;
    }

    auto inputs = get_inputs();
    const int index = embedded_w()->w->w->currentIndex();

    if(check_if_missing_inputs(inputs)){
        return;
    }

    // runtime inputs
    if(check_if_runtime_inputs(inputs)){
        set_valid_state();
        if(index < 2){
            propagate_default_runtime({std::make_shared<StringData>()});
        }else if(index < 8){
            propagate_default_runtime({std::make_shared<BoolData>()});
        }else{
            propagate_default_runtime({std::make_shared<StringListData>()});
        }
        return;
    }


    auto data1 = dcast<StringData>(inputs[0]);
    auto data2 = dcast<StringData>(inputs[1]);
    if(!data1 || !data2){
        set_invalid_cast();
        return;
    }

    auto v1 = data1->value();
    auto v2 = data2->value();

    std::variant<QString,bool,QStringList> output;
    QString txt;
    if(index == 0){ // Add
        output = v1 + v2;
        txt = std::get<QString>(output);
        if(txt.size() > 15){
            txt = txt.left(15) + "...";
        }
    }else if (index == 1){ // remove
        output = v1.remove(v2);
        txt = std::get<QString>(output);
        if(txt.size() > 15){
            txt = txt.left(15) + "...";
        }
    }else if (index == 2){ // contains
        output = v1.contains(v2);
        txt = str::Convertor::to_str(std::get<bool>(output));
    }else if (index == 3){ // do not contains
        output = !v1.contains(v2);
        txt = str::Convertor::to_str(std::get<bool>(output));
    }else if (index == 4){ // equals
        output = v1 == v2;
        txt = str::Convertor::to_str(std::get<bool>(output));
    }else if (index == 5){ // do not equals
        output = v1 != v2;
        txt = str::Convertor::to_str(std::get<bool>(output));
    }else if (index == 6){ // starts with
        output = v1.startsWith(v2);
        txt = str::Convertor::to_str(std::get<bool>(output));
    }else if (index == 7){ // ends with
        output = v1.endsWith(v2);
        txt = str::Convertor::to_str(std::get<bool>(output));
    }else if (index == 8){ // splits with
        output = v1.split(v2);
        txt = QSL("Size: ") % str::Convertor::to_str(std::get<QStringList>(output).size());
    }

    if(auto pval = std::get_if<QString>(&output)){
        propagate_data(txt,{std::make_shared<StringData>(*pval)} );
    }else if(auto pval = std::get_if<bool>(&output)){
        propagate_data(txt,{std::make_shared<BoolData>(*pval)} );
    }else if(auto pval = std::get_if<QStringList>(&output)){
        propagate_data(txt,{std::make_shared<StringListData>(*pval)} );
    }
}

void StringOperationNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    inPortsInfo[0].caption = QSL("A (") % get_name(io.inTypes[0]) % QSL(")");
    inPortsInfo[1].caption = QSL("B (") % get_name(io.inTypes[1]) % QSL(")");
    outPortsInfo[0].caption = QSL("out (") % from_view(ConnectionNode::get_name(ConnectionNode::Type::string_t)) % QSL(")");
}


#include "moc_string_operation_ndm.cpp"
