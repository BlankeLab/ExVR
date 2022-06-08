
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

// node#include
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

//QtNodes::FlowScene

class ExFlowScene : public QtNodes::FlowScene{
    Q_OBJECT
protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void drawForeground(QPainter *painter, const QRectF &rect) override;
    void drawItems(QPainter *painter, int numItems, QGraphicsItem *items[], const QStyleOptionGraphicsItem options[], QWidget *widget);
};


struct ConnectionsContextMenu : QObject{

Q_OBJECT

public:
    ConnectionsContextMenu(ElementKey routineKey, ConditionKey conditionKey);

    void exec(
        QPoint pos,
        BaseNodeDataModel *nodeModelUnderMouse,
        QtNodes::Connection *connectionUnderMouse,
        const std_v1<ComponentKey> &componentsKey,
        const std_v1<ConnectorKey> &connectorsKey,
        const std_v1<ConnectionKey> &connectionsKey
    );

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
    QAction *displayNodeInfoA = nullptr;

    QAction *unselectSelectionA = nullptr;

    BaseNodeDataModel *currentHoveredConnectorModel = nullptr;
    QtNodes::Connection *currentHoveredConnection = nullptr;

    ElementKey m_routineKey;
    ConditionKey m_conditionKey;

signals:

    void delete_node_signal(int key, bool isComponentNode);
    void delete_connection_signal(QUuid id);
    void duplicate_connector_node_signal(ConnectorKey connectorKey);
    void unselect_signal();
};



class ConnectionsW : public QWidget{
    Q_OBJECT

public :

    ConnectionsW(ElementKey routineKey, ConditionKey conditionKey);

    ~ConnectionsW();

    void close_all_connectors_windows();

    void update_from_condition(Condition *condition);

    ConnectorNodeDataModel *connector(ConnectorKey connectorKey);


    constexpr ElementKey routine_key() const noexcept{return m_routineKey;}
    constexpr ConditionKey condition_key() const noexcept{return m_conditionKey;}


private slots:


    void node_moved(QtNodes::Node& n, const QPointF& newLocation);
    void create_new_connection(const QtNodes::Connection &c); // insert connection generated by the GUI

    void get_current_selection(std_v1<ComponentKey> &componentsKey, std_v1<ConnectorKey> &connectorsKey, std_v1<ConnectionKey> &connectionsKey);
    void delete_connection(const QtNodes::Connection &c);
    void delete_selection();

    // ui
    void force_graphic_update();

private:

    void reset();

    ElementKey m_routineKey;
    ConditionKey m_conditionKey;

    QAction *generate_connector_action(std::pair<QString,QString> info);

    // from condition
    void remove_elements_from_condition(Condition *condition);
    void add_new_elements_from_condition(Condition *condition);
    void update_elements_from_condition(Condition *condition);
    void update_components_context_menu();

    void add_connection(Connection *connection); // generate connection and insert it
    void add_nodes_and_connections_to_scene(std_v1<Action*> componentsNodesToAdd, std_v1<Connector*> connectorsNodesToAdd, std_v1<Connection*> conectionsToAdd);

    QPoint m_mousePosition;
    QPointF m_viewMappedPosition;
    ConnectionsContextMenu m_contextMenu;

    std::map<int, std::pair<QtNodes::Node*, ComponentNodeDataModel*>> m_componentsNodes;
    std::map<int, std::pair<QtNodes::Node*, ConnectorNodeDataModel*>> m_connectorsNodes;
    std::map<QUuid, std::pair<const QtNodes::Connection*, ConnectionKey>> m_connections;

    // component id
    std::unordered_map<int, std::tuple<Component::Type, QString>>  m_availableComponents;

    std::unique_ptr<ExFlowScene> m_scene = nullptr;
    ExFlowView* m_view = nullptr;

};
}
