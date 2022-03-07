
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

#include "component_node_data_model.hpp"

#include "widgets/connections/data_models/data/nodes_data.hpp"

using namespace tool::ex;

ComponentNodeDataModel::ComponentNodeDataModel(int key, Component::Type componentType): BaseNodeDataModel(Type::Component){

    this->key = key;

    for(const auto &cSignal : Component::get_signals(componentType)){
        signalsF.emplace_back(std::make_pair(
            Component::Info{
                std::get<1>(cSignal),
                std::get<2>(cSignal),
                std::get<3>(cSignal),
            },
            std::make_shared<ComponentOutData>())
        );
    }

    for(const auto &cSlot : Component::get_slots(componentType)){
        std::weak_ptr<NodeData> wPtr;
        slotsF.emplace_back(std::make_pair(
            Component::Info{
                std::get<1>(cSlot),
                std::get<2>(cSlot),
                std::get<3>(cSlot),
            },
            wPtr)
        );
    }


    m_captionVisibility = true;

    nbPorts[static_cast<int>(PortType::In)]   = static_cast<unsigned int>(slotsF.size());
    nbPorts[static_cast<int>(PortType::Out)]  = static_cast<unsigned int>(signalsF.size());
    nbPorts[static_cast<int>(PortType::None)] = 0;

    inPortsInfo.resize(slotsF.size());
    for(size_t ii = 0; ii < slotsF.size(); ++ii){
        inPortsInfo[ii].caption = from_view(slotsF[ii].first.functionName)
            % QSL(" (") % from_view(ConnectionNode::get_name(slotsF[ii].first.argType)) % QSL(")");

        inPortsInfo[ii].type = generate_node_data_type(slotsF[ii].first.argType);
        inPortsInfo[ii].captionVisibility = true;
    }

    outPortsInfo.resize(signalsF.size());
    for(size_t ii = 0; ii < signalsF.size(); ++ii){
        outPortsInfo[ii].caption = from_view(signalsF[ii].first.functionName)
            % QSL(" (") % from_view(ConnectionNode::get_name(signalsF[ii].first.argType)) % QSL(")");

        outPortsInfo[ii].type = generate_node_data_type(signalsF[ii].first.argType);
        outPortsInfo[ii].captionVisibility = true;
    }
}

void ComponentNodeDataModel::update_name(QString componentName){
    m_name      = componentName % QSL("_component");
    m_caption   = componentName;
}

void ComponentNodeDataModel::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, PortIndex port){

    const auto id = static_cast<size_t>(port);
    if(id < slotsF.size()){
        slotsF[id].second = nodeData;
    }
}

std::shared_ptr<QtNodes::NodeData> ComponentNodeDataModel::outData(QtNodes::PortIndex port){

    const auto id = static_cast<size_t>(port);
    if(id < signalsF.size()){
        return signalsF[id].second;
    }
    return nullptr;
}

