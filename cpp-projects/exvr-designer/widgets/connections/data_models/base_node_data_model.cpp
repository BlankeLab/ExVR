
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

#include "base_node_data_model.hpp"

// base
#include "utility/benchmark.hpp"

using namespace tool::ex;



QtNodes::NodeValidationState BaseNodeDataModel::validationState() const{
    return modelValidationState;
}

QString BaseNodeDataModel::validationMessage() const{
    return modelValidationMessage;
}

const QString &BaseNodeDataModel::portCaption(QtNodes::PortType t, QtNodes::PortIndex i) const{

    const auto id = static_cast<size_t>(i);
    if(t == QtNodes::PortType::In){
        return inPortsInfo[id].caption;
    }else if(t == QtNodes::PortType::Out){
        return outPortsInfo[id].caption;
    }
    return defaultPortCaption;
}

const QtNodes::NodeDataType &BaseNodeDataModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const {

    const auto id = static_cast<size_t>(portIndex);
    if(portType == QtNodes::PortType::In){
        return inPortsInfo[id].type;
    }else if(portType == QtNodes::PortType::Out){
        return outPortsInfo[id].type;
    }    
    return NodeData::defaultType;
}

bool BaseNodeDataModel::portCaptionVisible(QtNodes::PortType portType, QtNodes::PortIndex index) const{

    if(portType == QtNodes::PortType::In){
        return inPortsInfo[index].captionVisibility;
    }else if(portType == QtNodes::PortType::Out){
        return outPortsInfo[index].captionVisibility;
    }
    return  false;
}


void BaseNodeDataModel::set_valid_state(){
    modelValidationState   = QtNodes::NodeValidationState::Valid;
    emit input_connection_validity_signal(true);
}

void BaseNodeDataModel::set_invalid_state(QString warning){
    modelValidationState   = QtNodes::NodeValidationState::Warning;
    modelValidationMessage = warning;
    emit input_connection_validity_signal(false);
}

void BaseNodeDataModel::set_error_state(QString error){
    modelValidationState   = QtNodes::NodeValidationState::Error;
    modelValidationMessage = error;
    emit input_connection_validity_signal(false);
}
