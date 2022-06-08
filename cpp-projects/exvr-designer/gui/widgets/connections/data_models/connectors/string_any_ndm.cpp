
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


#include "string_any_ndm.hpp"

using namespace tool::ex;

void StringAnyEmbeddedW::initialize(){

    // init widget
    w->init_widget(QSL("default"));

    // set widget connections
    connect(w.get(), &ExLineEditW::ui_change_signal, this, [=]{
        emit update_internal_data_signal({0}, {std::make_shared<StringData>(w->w->text())});
        emit compute_data_signal();
    });

    // add widget to ui
    add_row_in_dialog(QSL("Str: "), w->w.get());
}

void StringAnyNodeDataModel::compute(){

    auto interV = std::dynamic_pointer_cast<StringData>(interData[0]);
    const QString str = interV->value();
    const QString txt = interV->value_as_text();
    set_embedded_widget_text(txt);

    if(check_infinity_loop()){
        return;
    }

    auto inputs = get_inputs();
    // no inputs
    if(!has_inputs(inputs)){
        set_embedded_widget_text(txt);
        set_invalid_state(QSL("Missing 1 entree."));
        return;
    }

    // runtime inputs
    if(check_if_runtime_inputs(inputs)){
        propagate_default_runtime({
            std::make_shared<StringAnyData>(StringAny{str, nullptr})}
        );
        set_embedded_widget_text(txt);
        return;
    }

    // cast
    auto data = dcast<AnyData>(inputs[0]);
    if(!data){
        set_invalid_cast();
        return;
    }

    // propagate
    propagate_data(
        txt,
        {
            std::make_shared<StringAnyData>(StringAny{str,data->value()})
        });
}

void StringAnyNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
    for(size_t ii = 0; ii < io.outNb; ++ii){
        outPortsInfo[ii].caption = QSL("out (") % get_name(io.outTypes[ii]) % QSL(")");
    }
}


#include "moc_string_any_ndm.cpp"
