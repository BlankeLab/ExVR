
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

// base
#include "utility/unordered_map.hpp"

// node
#include "nodes/DataModelRegistry.hpp"
#include "nodes/NodeDataModel.hpp"
#include "nodes/NodeData.hpp"
#include "nodes/Node.hpp"
#include "nodes/TypeConverter.hpp"

using QtNodes::DataModelRegistry;
using QtNodes::NodeData;
using QtNodes::NodeGraphicsObject;
using QtNodes::NodeDataType;
using QtNodes::PortType;
using QtNodes::PortIndex;
using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDataModel;
using QtNodes::NodeValidationState;
using QtNodes::TypeConverter;
using QtNodes::TypeConverterId;

// local
// # data
#include "data/condition.hpp"
// # widgets
#include "ex_flow_view_widget.hpp"

// # data models
#include "data_models/connectors/component_node_data_model.hpp"
#include "data_models/connectors/connector_node_data_model.hpp"

Q_DECLARE_METATYPE(tool::ex::Connection*)
Q_DECLARE_METATYPE(tool::ex::Connector*)

namespace tool::ex {


class ExFlowScene : public QtNodes::FlowScene{
    Q_OBJECT
protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void drawForeground(QPainter *painter, const QRectF &rect) override;
    void drawItems(QPainter *painter, int numItems, QGraphicsItem *items[], const QStyleOptionGraphicsItem options[], QWidget *widget) override;
};


struct ConnectionsContextMenu : QObject{

Q_OBJECT

public:
    ConnectionsContextMenu(ElementKey routineKey, ConditionKey conditionKey);

    auto execute(
        QPointF globalPos,
        QPointF mappedPos,
        QtNodes::Node *nodeUnderMouse,
        QtNodes::Connection *connectionUnderMouse,
        const std::vector<ComponentKey> &componentsKey,
        const std::vector<ConnectorKey> &connectorsKey,
        const std::vector<ConnectionKey> &connectionsKey
    ) -> void;

    // top menues
    QMenu addNode;
    // # add node sub menues
    QMenu generatorM;
    QMenu operatorM;
    QMenu functionM;
    QMenu convertorM;
    QMenu actionM;
    QMenu eventM;
    QMenu linkM;
    QMenu flowM;
    QMenu displayM;
    QMenu resourcesM;
    QMenu legacyM;
    QMenu componentM;

private:

    QAction *removeNodeA = nullptr;
    QAction *removeAllSelectedNodesA = nullptr;
    QAction *removeConnectionA = nullptr;
    QAction *duplicateConnectorNodeA = nullptr;
    QAction *duplicateAllSelectedNodesA = nullptr;
    QAction *displayNodeInfoA = nullptr;
    QAction *copySelectionA = nullptr;
    QAction *pasteA = nullptr;

    QAction *unselectSelectionA = nullptr;

    QtNodes::Node *currentHoveredConnectorNode = nullptr;
    QtNodes::Connection *currentHoveredConnection = nullptr;

    ElementKey m_routineKey;
    ConditionKey m_conditionKey;

    QPointF m_currentMousePosition;
    // current selection    
    std::vector<ComponentKey> currentComponentsNodesSelected;
    std::vector<ConnectorKey> currentConnectorsNodesSelected;
    std::vector<ConnectionKey> currentConnectionSelected;

signals:

    auto delete_node_signal(int key, bool isComponentNode) -> void;
    auto delete_connection_signal(QUuid id) -> void;
    auto duplicate_connector_node_signal(tool::ex::ConnectorKey connectorKey) -> void;
    auto unselect_signal() -> void;

    auto open_menu_signal() -> void;
    auto close_menu_signal() -> void;
};



class ConnectionsW : public QWidget{
    Q_OBJECT

public :

    ConnectionsW(ElementKey routineKey, ConditionKey conditionKey);
    ~ConnectionsW();

    // get
    constexpr auto routine_key() const noexcept -> ElementKey{return m_routineKey;}
    constexpr auto condition_key() const noexcept -> ConditionKey{return m_conditionKey;}
    auto connector(ConnectorKey connectorKey) -> ConnectorNodeDataModel*;

    // update
    auto update_from_condition(Condition *condition) -> void;
    // ui
    auto close_all_connectors_windows() -> void;

public slots:

    auto lock_graphics_elements() -> void;
    auto unlock_graphics_elements() -> void;

private slots:

    // mouse hovering
    auto node_hovered(QtNodes::Node& n, QPoint screenPos)-> void;
    auto node_hover_left(QtNodes::Node& n) -> void;
    auto connection_hovered(QtNodes::Connection& c) -> void;
    auto connection_hover_left(QtNodes::Connection& c) -> void;
    // modify
    auto node_moved(QtNodes::Node& n, const QPointF& newLocation) -> void;

    // create
    auto create_new_connection(const QtNodes::Connection &c) -> void; // insert connection generated by the GUI
    // get
    auto get_current_selection(std::vector<ComponentKey> &componentsKey, std::vector<ConnectorKey> &connectorsKey, std::vector<ConnectionKey> &connectionsKey) -> void;
    // delete
    auto node_deleted(QtNodes::Node &n) -> void;
    auto connection_deleted(const QtNodes::Connection &c) -> void;
    auto delete_selection() -> void;

    // ui
    auto force_graphic_update() -> void;


private:

    auto reset() -> void;

    ElementKey m_routineKey;
    ConditionKey m_conditionKey;

    QAction *generate_connector_action(std::pair<QString,QString> info);

    // from condition
    void remove_elements_from_condition(Condition *condition);
    void add_new_elements_from_condition(Condition *condition);
    void update_elements_from_condition(Condition *condition);
    void update_components_context_menu();

    void add_connection(Connection *connection); // generate connection and insert it
    void add_nodes_and_connections_to_scene(std::vector<Action*> componentsNodesToAdd, std::vector<Connector*> connectorsNodesToAdd, std::vector<Connection*> conectionsToAdd);

    // positions
    QPoint m_mousePosition;
    QPointF m_viewMappedPosition;
    // hovered elements
    QtNodes::Node *m_hoveredNode = nullptr;
    QtNodes::Connection *m_hoveredConnection = nullptr;
    // menu
    ConnectionsContextMenu m_contextMenu;

    umap<int, std::pair<QtNodes::Node*, ComponentNodeDataModel*>> m_componentsNodes;
    umap<int, std::pair<QtNodes::Node*, ConnectorNodeDataModel*>> m_connectorsNodes;
    umap<QUuid, std::pair<const QtNodes::Connection*, ConnectionKey>> m_connections;

    // component id
    umap<int, std::tuple<Component::Type, QString>>  m_availableComponents;

    // scene & view
    std::unique_ptr<ExFlowScene> m_scene = nullptr;
    ExFlowView* m_view = nullptr;
};
}
