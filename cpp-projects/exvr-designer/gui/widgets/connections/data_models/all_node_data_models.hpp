
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

// std
#include <utility>

// nodes
#include "nodes/DataModelRegistry.hpp"
#include "nodes/TypeConverter.hpp"
using QtNodes::DataModelRegistry;

// base
#include "utility/vector.hpp"

// local
#include "data/connector.hpp"
#include "data/component.hpp"
#include "connectors/connector_node_data_model.hpp"
#include "connectors/component_node_data_model.hpp"

namespace tool::ex {

class DataNodeModels{
public:

    // init styles
    static inline QString FlowViewStyleStr = "";
    static inline QString ConnectionStyleStr = "";
    static inline QString NodeStyleStr = "";

    static inline QtNodes::NodeStyle generatorStyle;
    static inline QtNodes::NodeStyle convertorStyle;
    static inline QtNodes::NodeStyle functionStyle;
    static inline QtNodes::NodeStyle componentStyle;
    static inline QtNodes::NodeStyle operatorStyle;
    static inline QtNodes::NodeStyle actionStyle;
    static inline QtNodes::NodeStyle displayStyle;
    static inline QtNodes::NodeStyle eventStyle;
    static inline QtNodes::NodeStyle flowStyle;
    static inline QtNodes::NodeStyle linkStyle;
    static inline QtNodes::NodeStyle resourceStyle;


    static void initialize();

    template<Connector::Category C>


    static std::vector<std::pair<QString,QString>> get_connectors(){

        const auto& types = Connector::types_with_category<C>();
        std::vector<std::pair<QString, QString>> connectors;
        for (const auto &type : types) {
            connectors.emplace_back(std::make_pair(from_view(Connector::get_name(type)),from_view(Connector::get_caption(type))));
        }
        return connectors;
    }

    static std::unique_ptr<ConnectorNodeDataModel> generate_connector_data_model(Connector *connector);
    static std::unique_ptr<ComponentNodeDataModel> generate_component_data_model(Component *component);
    static inline std::shared_ptr<DataModelRegistry> registry = nullptr;
};

}
