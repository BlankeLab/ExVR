
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

#include "connector_node_data_model.hpp"

// base
#include "utility/benchmark.hpp"

using namespace tool::ex;

ConnectorNodeDataModel::ConnectorNodeDataModel(Connector::Type t, BaseNodeContainerW *w) :
    BaseNodeDataModel(Type::Connector), m_type(t), m_widget(w) {
}

void ConnectorNodeDataModel::init_ports_caption(){

    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = get_name(io.inTypes[ii]);
    }
    for(size_t ii = 0; ii < io.outNb; ++ii){
        outPortsInfo[ii].caption = get_name(io.outTypes[ii]);
    }
}

void ConnectorNodeDataModel::init_ports_types(){

    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].type = generate_node_data_type(io.inTypes[ii]);
    }
    for(size_t ii = 0; ii < io.outNb; ++ii){
        outPortsInfo[ii].type = generate_node_data_type(io.outTypes[ii]);
    }
}

void ConnectorNodeDataModel::init_ports_caption_visibility(){

    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].captionVisibility = Connector::get_in_port_visibility(m_type, ii);
    }
    for(size_t ii = 0; ii < io.outNb; ++ii){
        outPortsInfo[ii].captionVisibility = Connector::get_out_port_visibility(m_type, ii);
    }
}

void ConnectorNodeDataModel::initialize(QtNodes::NodeStyle style, ConnectorKey connectorKey){

    key = connectorKey.v;

    // name / caption
    m_name = from_view(Connector::get_name(m_type));
    m_caption = from_view(Connector::get_caption(m_type));
    m_captionVisibility = Connector::get_caption_visibility(m_type);

    // set nb ports
    const auto io = Connector::get_io(m_type);
    nbPorts[static_cast<size_t>(PortType::In)]   = io.inNb;
    nbPorts[static_cast<size_t>(PortType::Out)]  = io.outNb;
    nbPorts[static_cast<size_t>(PortType::None)] = 0;

    // ports type/caption
    inPortsInfo.resize(io.inNb);
    outPortsInfo.resize(io.outNb);

    init_ports_caption();
    init_ports_types();
    init_ports_caption_visibility();

    // resize data
    inputData.resize(nb_Ports(PortType::In));
    interData.resize(Connector::get_inter_number(m_type));
    outputData.resize(nb_Ports(PortType::Out));

    // set style
    set_node_style(style);

    // init embedded widget
    connect(m_widget, &BaseNodeContainerW::update_internal_data_signal, this, [&](std::vector<size_t> indexes, std::vector<std::shared_ptr<NodeData>> nodes){
        for(size_t ii = 0; ii < indexes.size(); ++ii){
            interData[indexes[ii]] = nodes[ii];
        }
    });
    connect(m_widget, &BaseNodeContainerW::compute_data_signal, this,   &ConnectorNodeDataModel::compute);
    connect(m_widget, &BaseNodeContainerW::compute_data_signal, this,  [&]{
        emit connector_modified_signal(ConnectorKey{key}, name(), convert_to_arg());
    });

    connect(m_widget, &BaseNodeContainerW::ask_node_to_update,  this,   &ConnectorNodeDataModel::force_node_update_signal);

    // trigger
//    m_widget->trigger_signals();

    // interactivity
    if(Connector::get_interactivity(m_type) == Connector::Interactivity::Interactive){
        m_widget->enable();
    }else{
        m_widget->disable();
    }
}


bool ConnectorNodeDataModel::check_infinity_loop(bool unknowValue){

    if(!timerOn){
        timerOn = true;
        QTimer::singleShot(100, this, [&]{
            timerOn = false;
            nbCalls = 0;
        });
    }

    ++nbCalls;
    if(nbCalls < 50){
        return false;
    }

    // cyclic relation
    set_invalid_state(QSL("Cyclic relation detected"));
    if(unknowValue){
        unknow_value_text();
    }

    for(size_t ii = 0; ii < outputData.size(); ++ii){
        outputData[ii] = nullptr;
        emit dataInvalidated(static_cast<QtNodes::PortIndex>(ii));
    }

    return true;

}

bool ConnectorNodeDataModel::has_inputs(const std::vector<std::shared_ptr<QtNodes::NodeData> > &inputs) const{

    for(const auto &input : inputs){
        if(input){ // at least one input
            return true;
        }
    }
    return false;
}

bool ConnectorNodeDataModel::check_if_no_inputs(const std::vector<std::shared_ptr<QtNodes::NodeData> > &inputs, std::optional<QString> text){

    if(has_inputs(inputs)){
        return false;
    }

    // not inputs
    set_valid_state();
    if(text.has_value()){
        set_embedded_widget_text(text.value());
    }else{
        unknow_value_text();
    }

    invalidate_data();

    return true;
}

bool ConnectorNodeDataModel::check_if_missing_inputs(const std::vector<std::shared_ptr<QtNodes::NodeData>> &inputs){

    size_t countMissing = 0;
    for(const auto &input : inputs){
        if(!input){
            ++countMissing;
        }
    }

    if(countMissing == 0){
        return false; // all inputs
    }

    // at least one input is missing
    if(countMissing == 1){
        set_invalid_state(QSL("Missing 1 entree."));
    }else{
        set_invalid_state(QSL("Missing ") % QString::number(countMissing) % QSL(" entrees."));
    }
    unknow_value_text();
    invalidate_data();

    return true;
}

bool ConnectorNodeDataModel::check_if_runtime_inputs(const std::vector<std::shared_ptr<QtNodes::NodeData> > &inputs){

    bool runtimeDetected = false;
    for(const auto &input : inputs){
        if(input){
            if(is_runtime(input)){
                runtimeDetected = true;
                break;
            }
        }
    }

    if(!runtimeDetected){
        return false; // no runtime
    }

    // at least one input is runtime
    set_valid_state();
    runtime_value_text();

    return true;
}

void ConnectorNodeDataModel::propagate_default_runtime(std::vector<std::shared_ptr<BaseNodeData>> outputs, std::vector<size_t> indexes){

    if((indexes.size() != outputs.size()) && (indexes.size() != 0)){
        error_value_text();
        set_error_state(QSL("Invalid indexes for outputs [") % QString::number(indexes.size()) % QSL("] [") % QString::number(outputs.size()) % QSL("] "));
        return;
    }

    if(indexes.size() == 0){

        if(outputs.size() > outputData.size()){
            error_value_text();
            set_error_state(QSL("Output id too large [") % QString::number(outputs.size()) % QSL("] [") % QString::number(outputData.size()) % QSL("] "));
            return;
        }

        for(size_t ii = 0; ii < outputs.size(); ++ii){
            outputs[ii]->set_runtime();
            outputData[ii] = outputs[ii];
            emit dataUpdated(static_cast<QtNodes::PortIndex>(ii));
        }

    }else{

        for(size_t ii = 0; ii < indexes.size(); ++ii){
            if(indexes[ii] >= outputData.size()){
                error_value_text();
                set_error_state(QSL("Output id too large [") % QString::number(indexes[ii]) % QSL("] [") % QString::number(outputData.size()) % QSL("] "));
                return;
            }
        }

        for(size_t ii = 0; ii < indexes.size(); ++ii){
            outputs[ii]->set_runtime();
            outputData[indexes[ii]] = outputs[ii];
            emit dataUpdated(static_cast<QtNodes::PortIndex>(indexes[ii]));
        }
    }
}

void ConnectorNodeDataModel::propagate_default_runtime(std::vector<std::shared_ptr<BaseNodeData> > outputs, size_t outputId){

    if(outputId >= outputData.size()){
        error_value_text();
        set_error_state(QSL("Output id too large."));
        return;
    }

    for(size_t ii = 0; ii < outputs.size(); ++ii){
        outputs[ii]->set_runtime();
        outputData[outputId] = outputs[ii];
        emit dataUpdated(static_cast<QtNodes::PortIndex>(outputId));
    }
}

void ConnectorNodeDataModel::propagate_data(QString valueStr, std::vector<std::shared_ptr<BaseNodeData>> outputs, std::vector<size_t> indexes){

    if((indexes.size() != outputs.size()) && (indexes.size() != 0)){
        error_value_text();
        set_error_state(QSL("Invalid indexes for outputs [") % QString::number(indexes.size()) % QSL("] [") % QString::number(outputs.size()) % QSL("] "));
        return;
    }

    set_valid_state();
    set_embedded_widget_text(valueStr);

    if(indexes.size() == 0){

        if(outputs.size() > outputData.size()){
            error_value_text();
            set_error_state(QSL("Output id too large [") % QString::number(outputs.size()) % QSL("] [") % QString::number(outputData.size()) % QSL("] "));
            return;
        }

        for(size_t ii = 0; ii < outputs.size(); ++ii){            
            outputData[ii] = outputs[ii];
            emit dataUpdated(static_cast<QtNodes::PortIndex>(ii));
        }

    }else{

        for(size_t ii = 0; ii < indexes.size(); ++ii){
            if(indexes[ii] >= outputData.size()){
                error_value_text();
                set_error_state(QSL("Output id too large [") % QString::number(indexes[ii]) % QSL("] [") % QString::number(outputData.size()) % QSL("] "));
                return;
            }
        }

        for(size_t ii = 0; ii < indexes.size(); ++ii){
            outputData[indexes[ii]] = outputs[ii];
            emit dataUpdated(static_cast<QtNodes::PortIndex>(indexes[ii]));
        }
    }
}

void ConnectorNodeDataModel::propagate_data(QString valueStr, std::vector<std::shared_ptr<BaseNodeData> > outputs, size_t outputId){

    set_valid_state();
    set_embedded_widget_text(valueStr);


    if(outputId >= outputData.size()){
        error_value_text();
        set_error_state(QSL("Output id too large."));
        return;
    }

    for(size_t ii = 0; ii < outputs.size(); ++ii){
        outputData[outputId] = outputs[ii];
        emit dataUpdated(static_cast<QtNodes::PortIndex>(outputId));
    }
}

void ConnectorNodeDataModel::invalidate_data(){
    for(size_t ii = 0; ii < outputData.size(); ++ii){
        outputData[ii] = nullptr;
        emit dataInvalidated(static_cast<QtNodes::PortIndex>(ii));
    }
}

void ConnectorNodeDataModel::set_invalid_cast(){
    invalidate_data();
    error_value_text();
    set_error_state(QSL("Invalid cast."));
}


void ConnectorNodeDataModel::update_from_connector(const Connector &connector){
    m_widget->update_from_arg(connector.arg);
    emit embeddedWidgetSizeUpdated();
}

Arg ConnectorNodeDataModel::convert_to_arg() const{
    return m_widget->convert_to_arg();
}

void ConnectorNodeDataModel::set_embedded_widget_text(const QString &text){
    m_widget->set_text_value(text);
    emit embeddedWidgetSizeUpdated();
}

void ConnectorNodeDataModel::runtime_value_text(){
    m_widget->set_text_value(QSL("? (runtime)"));
    emit embeddedWidgetSizeUpdated();
}

void ConnectorNodeDataModel::unknow_value_text(){
    m_widget->set_text_value(QSL("?"));
    emit embeddedWidgetSizeUpdated();
}

void ConnectorNodeDataModel::error_value_text(){
    m_widget->set_text_value(QSL("? (error)"));
    emit embeddedWidgetSizeUpdated();
}

void ConnectorNodeDataModel::lock_embedded_widget(){
    m_widget->disable();
}

void ConnectorNodeDataModel::unlock_embedded_widget(){
    m_widget->enable();
}

void ConnectorNodeDataModel::update_with_info(QStringView id, QStringView value){
    if(id[0] == 'v'){
        if(m_widget->update_with_info(value)){
            emit embeddedWidgetSizeUpdated();
        }
    }
}



std::shared_ptr<QtNodes::NodeData> ConnectorNodeDataModel::outData(QtNodes::PortIndex index){
    return outputData[to_size_t(index)];
}

void ConnectorNodeDataModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex index){
    inputData[to_size_t(index)] = nodeData;
    compute();
}
