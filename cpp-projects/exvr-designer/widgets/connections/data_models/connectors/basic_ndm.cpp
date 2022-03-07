
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

#include "basic_ndm.hpp"

using namespace tool::ex;

void NextNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
}

void PreviousNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
}


void StopNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
}

void PauseNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
}

void NextWithNameEmbeddedW::initialize(){

    // init widget
    w->init_widget("default routine");

    // set widget connections
    connect(w.get(), &ExLineEditW::ui_change_signal, this, &NextWithNameEmbeddedW::compute_data_signal);

    // add widget to ui
    add_row_in_dialog("Element name: ", w->w.get());
}

void NextWithNameNodeDataModel::compute(){
    set_embedded_widget_text(embedded_w()->w->w->text());
}

void NextWithNameNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
}

void PreviousWithNameEmbeddedW::initialize(){

    // init widget
    w->init_widget("default routine");

    // set widget connections
    connect(w.get(), &ExLineEditW::ui_change_signal, this, &PreviousWithNameEmbeddedW::compute_data_signal);

    // add widget to ui
    add_row_in_dialog("Element name: ", w->w.get());
}

void PreviousWithNameNodeDataModel::compute(){
    set_embedded_widget_text(embedded_w()->w->w->text());
}

void PreviousWithNameNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
}

void NextWithCondEmbeddedW::initialize(){
    // init widget
    w->init_widget("default condition");

    // set widget connections
    connect(w.get(), &ExLineEditW::ui_change_signal, this, &NextWithCondEmbeddedW::compute_data_signal);

    // add widget to ui
    add_row_in_dialog("Condition name: ", w->w.get());
}

void NextWithCondNodeDataModel::compute(){
    set_embedded_widget_text(embedded_w()->w->w->text());
}

void NextWithCondNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
}

void PreviousWithCondEmbeddedW::initialize(){

    // init widget
    w->init_widget("default condition");

    // set widget connections
    connect(w.get(), &ExLineEditW::ui_change_signal, this, &PreviousWithCondEmbeddedW::compute_data_signal);

    // add widget to ui
    add_row_in_dialog("Condition name: ", w->w.get());
}

void PreviousWithCondNodeDataModel::compute(){
    set_embedded_widget_text(embedded_w()->w->w->text());
}

void PreviousWithCondNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
}

void ForceComponentConfigW::initialize(){

    // init widget
    w->init_widget();

    // set widget connections
    connect(w.get(), &ExConfigW::ui_change_signal,       this, &ForceComponentConfigW::compute_data_signal);
    connect(w.get(), &ExConfigW::update_from_components_signal, this, &ForceComponentConfigW::compute_data_signal);

    // add widget to ui
    add_row_in_dialog("", w->w.get());
}

void ForceComponentConfigNodeDataModel::compute(){
    set_embedded_widget_text(embedded_w()->w->display());
}

void ForceComponentConfigNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    inPortsInfo[0].caption = QSL("in (") % get_name(io.inTypes[0]) % QSL(")");
}


#include "moc_basic_ndm.cpp"
