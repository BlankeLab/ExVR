
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

#include "check_str_ndm.hpp"

using namespace tool::ex;

void CheckStrEmbeddedW::initialize(){

    // init widget
    w->init_widget(QSL("default"));

    // set widget connections
    connect(w.get(), &ExLineEditW::ui_change_signal, this, [=]{
        emit update_internal_data_signal({0}, {std::make_shared<StringData>(w->w->text())});
        emit compute_data_signal();
    });

    // add widget to ui
    add_row_in_dialog(QSL("If Str ==: "), w->w.get());
}

void CheckStrNodeDataModel::compute(){

    auto strData = std::dynamic_pointer_cast<StringData>(interData[0]);
    QString text = QSL("== ") % strData->value();
    set_embedded_widget_text(text);

    if(check_infinity_loop(false)){
        return;
    }

    auto inputs = get_inputs();
    if(!inputs[0]){
        set_invalid_state(QSL("Missing entree #1."));
        invalidate_data();
        return;
    }

    // runtime inputs
    if(is_runtime(inputs[0])){
        set_valid_state();
        propagate_default_runtime({generate_default_runtime_any_data()});
        return;
    }

    // cast
    auto data1 = dcast<StringAnyData>(inputs[0]);
    if(!data1){
        set_error_state(QSL("Invalid cast."));
        return;
    }

    // propagate
    if(data1->value().str == strData->value()){
        propagate_data(
            text,
            {
                std::make_shared<AnyData>(data1->value().data)
            } );
    }else{
        invalidate_data();
    }
}

void CheckStrNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
    for(size_t ii = 0; ii < io.outNb; ++ii){
        outPortsInfo[ii].caption = QSL("out (") % get_name(io.outTypes[ii]) % QSL(")");
    }
}



#include "moc_check_str_ndm.cpp"
