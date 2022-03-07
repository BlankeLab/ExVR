
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

#include "time_ndm.hpp"

using namespace tool::ex;

void TimeEmbeddedW::initialize(){

    // init widget
    w->init_widget();

    // set widget connections
    connect(w.get(), &ExTimeW::ui_change_signal, this, [=]{
        emit update_internal_data_signal({0}, {std::make_shared<IntData>(0)});
        emit compute_data_signal();
    });

    // add widget to ui
    add_row_in_dialog(QSL("Time settings: "), w->w.get(), false);
}


bool TimeEmbeddedW::set_text_value(QStringView value){

    //bool askForResize = m_valueText.text().size() != value.size();
    m_valueText.blockSignals(true);
    m_valueText.setText(value.toString() % QSL("ms"));
    m_valueText.blockSignals(false);
    //return askForResize;
    return false;
}


void TimeNodeDataModel::compute(){

    if(embedded_w()->w->modeT.w->currentIndex() == 0){
        m_caption = embedded_w()->w->typeT.w->currentText() % QSL(" (trigger)");
    }else{
        m_caption = embedded_w()->w->typeT.w->currentText() % QSL(" (") % QString::number(embedded_w()->w->frequency.w->value()) % QSL(" fps)");
    }

    set_valid_state();
    propagate_default_runtime({std::make_shared<RealData>(0)});
}


void TimeNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    inPortsInfo[0].caption = QSL("ask time (") % get_name(io.inTypes[0]) % QSL(")");
    outPortsInfo[0].caption = QSL("ms (") % get_name(io.outTypes[0]) % QSL(")");
}


#include "moc_time_ndm.cpp"
