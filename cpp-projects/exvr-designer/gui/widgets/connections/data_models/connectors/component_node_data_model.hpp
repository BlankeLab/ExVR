
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

#pragma once

// nodes
#include "nodes/NodeData.hpp"
#include "nodes/NodeDataModel.hpp"
#include "nodes/Connection.hpp"
#include "nodes/FlowScene.hpp"
#include "nodes/FlowView.hpp"
#include "nodes/DataModelRegistry.hpp"
#include "nodes/ConnectionStyle.hpp"

// local
#include "gui/widgets/connections/data_models/base_node_data_model.hpp"
#include "data/component.hpp"
#include "data/action.hpp"


using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::PortType;
using QtNodes::PortIndex;

namespace tool::ex{

class ComponentNodeDataModel : public BaseNodeDataModel {

Q_OBJECT

public:

    ComponentNodeDataModel(int key, Component::Type componentType);

    // data
    std::shared_ptr<NodeData> outData(PortIndex port) override;
    void setInData(std::shared_ptr<NodeData>, PortIndex) override;

    // name
    void update_name(QString componentName);

    // widget
    QWidget * embeddedWidget() override { return nullptr; }

public:

    // inputData
    std_v1<std::pair<Component::Info, std::weak_ptr<NodeData>>> slotsF;
    // outputData
    std_v1<std::pair<Component::Info, std::shared_ptr<ComponentOutData>>> signalsF;

};
}
