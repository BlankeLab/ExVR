
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

#include "curve_x_ndm.hpp"

using namespace tool::ex;


void CurveXEmbeddedW::initialize(){

    // init widget
    w->init_widget(curveTitle);
    set_title(curveTitle);
    set_text_value(curveTitle);

    // set widget connections
    connect(w.get(), &ExCurveXW::ui_change_signal, this, &CurveXEmbeddedW::compute_data_signal);
    connect(w.get(), &ExCurveXW::name_changed_signal, this, [&](QString name){
        curveTitle = name;
        set_title(curveTitle);
        set_text_value(curveTitle);
    });

    // add widget to ui
    add_row_in_dialog("", w->w.get());    
}

bool CurveXEmbeddedW::update_with_info(QStringView value){
    w->w->curve.add_values(str::Convertor::to_double_list(value.toString()));
    return false;
}

void CurveXNodeDataModel::compute(){

    if(check_infinity_loop()){
        return;
    }

    set_embedded_widget_text(embedded_w()->curveTitle);
}

void CurveXNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
}




#include "moc_curve_x_ndm.cpp"
