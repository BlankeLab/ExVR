
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

#include "connections_widget.hpp"

// qt-utility
#include "qt_logger.hpp"

// local
#include "data_models/all_node_data_models.hpp"
#include "data_models/data/nodes_data_converters.hpp"
#include "utility/benchmark.hpp"
#include "experiment/global_signals.hpp"



using namespace tool;
using namespace tool::ex;


ConnectionsW::ConnectionsW(ElementKey routineKey, ConditionKey conditionKey) : m_routineKey(routineKey), m_conditionKey(conditionKey), m_contextMenu(routineKey, conditionKey){

    auto l = new QHBoxLayout();
    l->setContentsMargins(0,0,0,0);
    setLayout(l);

    // init scene
    m_scene = std::make_unique<ExFlowScene>();
    m_scene->setSceneRect(QRectF(0,0,2000,1000));

    m_scene->setRegistry(DataNodeModels::registry);

    // init view
    m_view  = new ExFlowView(m_scene.get());
    layout()->addWidget(m_view);
//    m_view->deleteSelectionAction()->blockSignals(true);

    // connections
    // # scene
    connect(m_scene.get(), &ExFlowScene::connectionCreated,     this, &ConnectionsW::create_new_connection);
    connect(m_scene.get(), &ExFlowScene::connectionDeleted,     this, &ConnectionsW::connection_deleted);
    connect(m_scene.get(), &ExFlowScene::nodeDeleted,           this, &ConnectionsW::node_deleted);
    connect(m_scene.get(), &ExFlowScene::nodeMoved,             this, &ConnectionsW::node_moved);
    connect(m_scene.get(), &ExFlowScene::nodeHovered,           this, &ConnectionsW::node_hovered);
    connect(m_scene.get(), &ExFlowScene::nodeHoverLeft,         this, &ConnectionsW::node_hover_left);
    connect(m_scene.get(), &ExFlowScene::connectionHovered,     this, &ConnectionsW::connection_hovered);
    connect(m_scene.get(), &ExFlowScene::connectionHoverLeft,   this, &ConnectionsW::connection_hover_left);

    // # view   
    connect(m_view, &ExFlowView::mouse_wheel_event_signal, this, [&](QWheelEvent *event){
        if(m_hoveredNode){
            dynamic_cast<BaseNodeDataModel*>(m_hoveredNode->nodeDataModel())->wheel_event(event);
        }
    });
    connect(m_view, &ExFlowView::mouse_pressed_event_signal, this, [&](){
        std::vector<ComponentKey> componentsKey;
        std::vector<ConnectorKey> connectorsKey;
        std::vector<ConnectionKey> connectionsKey;
        get_current_selection(componentsKey, connectorsKey, connectionsKey);
        emit GSignals::get()->select_nodes_and_connections_signal(routine_key(), condition_key(), std::move(connectorsKey), std::move(componentsKey),  std::move(connectionsKey), false);
    });
    connect(m_view, &ExFlowView::mouse_release_event_signal, this, [&](){
        std::vector<ComponentKey> componentsKey;
        std::vector<ConnectorKey> connectorsKey;
        std::vector<ConnectionKey> connectionsKey;
        get_current_selection(componentsKey, connectorsKey, connectionsKey);
        emit GSignals::get()->select_nodes_and_connections_signal(routine_key(), condition_key(), std::move(connectorsKey), std::move(componentsKey),  std::move(connectionsKey), false);
    });
    connect(m_view, &ExFlowView::open_context_menu_signal, this, [&](QPoint globalPosition, QPointF mappedPos){

        m_mousePosition      = globalPosition;
        m_viewMappedPosition = mappedPos;

        std::vector<ComponentKey> componentsKey;
        std::vector<ConnectorKey> connectorsKey;
        std::vector<ConnectionKey> connectionsKey;
        get_current_selection(componentsKey, connectorsKey, connectionsKey);

        auto currentSelection = m_scene->selectedNodes();
        m_contextMenu.execute(globalPosition, mappedPos, m_hoveredNode, m_hoveredConnection, componentsKey, connectorsKey, connectionsKey);
        for(auto &node : currentSelection){
            node->nodeGraphicsObject().setSelected(true);
        }
    });    

    connect(m_view, &ExFlowView::delete_signal, this, &ConnectionsW::delete_selection);

    // generate context menu view nodes actions
    for(const auto &info : DataNodeModels::get_connectors<Connector::Category::Generator>()){
        m_contextMenu.generatorM.addAction(generate_connector_action(info));
    }
    for(const auto &info : DataNodeModels::get_connectors<Connector::Category::Operator>()){
        m_contextMenu.operatorM.addAction(generate_connector_action(info));
    }
    for(const auto &info : DataNodeModels::get_connectors<Connector::Category::Function>()){
        m_contextMenu.functionM.addAction(generate_connector_action(info));
    }
    for(const auto &info : DataNodeModels::get_connectors<Connector::Category::Convertor>()){
        m_contextMenu.convertorM.addAction(generate_connector_action(info));
    }
    for(const auto &info : DataNodeModels::get_connectors<Connector::Category::Action>()){
        m_contextMenu.actionM.addAction(generate_connector_action(info));
    }
    for(const auto &info : DataNodeModels::get_connectors<Connector::Category::Event>()){
        m_contextMenu.eventM.addAction(generate_connector_action(info));
    }
    for(const auto &info : DataNodeModels::get_connectors<Connector::Category::Link>()){
        m_contextMenu.linkM.addAction(generate_connector_action(info));
    }
    for(const auto &info : DataNodeModels::get_connectors<Connector::Category::Display>()){
        m_contextMenu.displayM.addAction(generate_connector_action(info));
    }
    for(const auto &info : DataNodeModels::get_connectors<Connector::Category::Flow>()){
        m_contextMenu.flowM.addAction(generate_connector_action(info));
    }
    for(const auto &info : DataNodeModels::get_connectors<Connector::Category::Resource>()){
        m_contextMenu.resourcesM.addAction(generate_connector_action(info));
    }
    for(const auto &info : DataNodeModels::get_connectors<Connector::Category::Legacy>()){
        m_contextMenu.legacyM.addAction(generate_connector_action(info));
    }

    // context menu node
    connect(&m_contextMenu, &ConnectionsContextMenu::open_menu_signal, this, &ConnectionsW::lock_graphics_elements);
    connect(&m_contextMenu, &ConnectionsContextMenu::close_menu_signal, this, &ConnectionsW::unlock_graphics_elements);

    connect(&m_contextMenu, &ConnectionsContextMenu::delete_node_signal, this, [&](int nodeKey, bool isComponentNode){

        if(isComponentNode){
            emit GSignals::get()->delete_nodes_and_connections_signal(routine_key(), condition_key(), {}, {ComponentKey{nodeKey}}, {}, true);
        }else{
            emit GSignals::get()->delete_nodes_and_connections_signal(routine_key(), condition_key(), {ConnectorKey{nodeKey}}, {}, {}, true);
        }
    });
    connect(&m_contextMenu, &ConnectionsContextMenu::delete_connection_signal, this, [&](QUuid id){
        for(const auto &connection : m_scene->connections()){
            if(connection.second->id() == id){
                emit GSignals::get()->delete_nodes_and_connections_signal(routine_key(), condition_key(), {}, {}, {m_connections[connection.first].second}, true);
                return;
            }
        }
    });
    connect(&m_contextMenu, &ConnectionsContextMenu::duplicate_connector_node_signal, this, [&](ConnectorKey connectorKey){
        emit GSignals::get()->duplicate_connector_node_signal(routine_key(), condition_key(), connectorKey);
    });

    connect(&m_contextMenu, &ConnectionsContextMenu::unselect_signal, this, [&](){
        emit GSignals::get()->unselect_nodes_and_connections_signal(routine_key(), condition_key(), true);
    });
}

ConnectionsW::~ConnectionsW(){
    reset();
    m_scene.release();
}

auto ConnectionsW::close_all_connectors_windows() -> void{
    for(auto &connector : m_connectorsNodes){
        qobject_cast<BaseNodeContainerW*>(connector.second.second->embeddedWidget())->close();
    }
}

auto ConnectionsW::lock_graphics_elements() -> void{

    for(auto &node : m_scene->nodes()){
        node.second->nodeGraphicsObject().lock(true);
    }
    for(auto &connection : m_scene->connections()){
        connection.second->getConnectionGraphicsObject().lock(true);
    }
}

auto ConnectionsW::unlock_graphics_elements() -> void{

    for(auto &node : m_scene->nodes()){
        node.second->nodeGraphicsObject().lock(false);
    }
    for(auto &connection : m_scene->connections()){
        connection.second->getConnectionGraphicsObject().lock(false);
    }
}

auto ConnectionsW::get_current_selection(std::vector<ComponentKey> &componentsKey, std::vector<ConnectorKey> &connectorsKey, std::vector<ConnectionKey> &connectionsKey) -> void{

    componentsKey.clear();
    connectorsKey.clear();
    connectionsKey.clear();

    if(m_scene->selectedNodes().size() > 0){ // nodes are selected

        for(const auto &node : m_scene->selectedNodes()){
            auto baseDataModel = dynamic_cast<BaseNodeDataModel*>(node->nodeDataModel());
            if(baseDataModel->type == BaseNodeDataModel::Type::Component){
                componentsKey.emplace_back(ComponentKey{baseDataModel->key});
            }else{
                connectorsKey.emplace_back(ConnectorKey{baseDataModel->key});
            }

            // remove connections associated to node
            // # input
            auto inConnections = node->nodeState().getEntries(PortType::In);
            for(const auto &connectionsSet : inConnections){
                for(const auto &connection : connectionsSet){
                    connectionsKey.emplace_back(m_connections[connection.first].second);
                }
            }
            // # output
            auto outConnections = node->nodeState().getEntries(PortType::Out);
            for(const auto &connectionsSet : outConnections){
                for(const auto &connection : connectionsSet){
                    connectionsKey.emplace_back(m_connections[connection.first].second);
                }
            }
        }

    }else{ // a connection is selected

        for(const auto &connection : m_scene->connections()){
            if(connection.second->getConnectionGraphicsObject().isSelected()){
                connectionsKey.emplace_back(m_connections[connection.first].second);
            }
        }
    }
}


auto ConnectionsW::node_deleted(QtNodes::Node &n) -> void{

    if(m_hoveredNode){
        if(n.id() == m_hoveredNode->id()){
            m_hoveredNode = nullptr;
        }
    }
}

auto ConnectionsW::delete_selection() -> void{

    std::vector<ComponentKey> componentsKey;
    std::vector<ConnectorKey> connectorsKey;
    std::vector<ConnectionKey> connectionsKey;
    get_current_selection(componentsKey, connectorsKey, connectionsKey);

    if(componentsKey.size() > 0 || connectorsKey.size() > 0 || connectionsKey.size() > 0){
        emit GSignals::get()->delete_nodes_and_connections_signal(routine_key(), condition_key(), std::move(connectorsKey), std::move(componentsKey), std::move(connectionsKey), true);
    }
}

auto ConnectionsW::node_moved(QtNodes::Node &n, const QPointF &newLocation) -> void{

    auto dataModel = dynamic_cast<BaseNodeDataModel*>(n.nodeDataModel());
    if(dataModel->type == BaseNodeDataModel::Type::Component){
        emit GSignals::get()->component_node_moved_signal(routine_key(), condition_key(), ComponentKey{dataModel->key}, newLocation);
    }else{
        emit GSignals::get()->connector_node_moved_signal(routine_key(), condition_key(), ConnectorKey{dataModel->key}, newLocation);
    }
}

auto ConnectionsW::node_hovered(QtNodes::Node &n, QPoint) -> void{
    m_hoveredNode = &n;
}

auto ConnectionsW::node_hover_left(QtNodes::Node &n) -> void {
    m_hoveredNode = nullptr;
}

auto ConnectionsW::connection_hovered(QtNodes::Connection &c) -> void {
    m_hoveredConnection = &c;
}

auto ConnectionsW::connection_hover_left(QtNodes::Connection &) -> void {
    m_hoveredConnection = nullptr;
}

void ConnectionsW::add_connection(Connection *connection){

    QtNodes::Node *startNode = nullptr;
    if(connection->startType == Connection::Type::Connector){
        if(!m_connectorsNodes.count(connection->startKey)){
            QtLogger::error(QString("Cannot create connection %1 , there is no connector node with key %2").arg(connection->key(),connection->startKey));
            return;
        }
        startNode = m_connectorsNodes[connection->startKey].first;
    }else{
        if(!m_componentsNodes.count(connection->startKey)){
            QtLogger::error(QString("Cannot create connection %1 , there is no component node with key %2").arg(connection->key(),connection->startKey));
            return;
        }
        startNode = m_componentsNodes[connection->startKey].first;
    }

    QtNodes::Node *endNode = nullptr;
    if(connection->endType == Connection::Type::Connector){
        if(!m_connectorsNodes.count(connection->endKey)){
            QtLogger::error(QString("Cannot create connection %1 , there is no connector node with key %2").arg(connection->key(),connection->endKey));
            return;
        }
        endNode = m_connectorsNodes[connection->endKey].first;
    }else{
        if(!m_componentsNodes.count(connection->endKey)){
            QtLogger::error(QString("Cannot create connection %1 , there is no component node with key %2").arg(connection->key(),connection->endKey));
            return;
        }
        endNode = m_componentsNodes[connection->endKey].first;
    }

    const NodeDataType &outType = startNode->nodeDataModel()->port_data_type(PortType::Out,connection->startIndex);
    const NodeDataType &intType = endNode->nodeDataModel()->port_data_type(PortType::In,connection->endIndex);

    auto connectionW = m_scene->createConnection(*endNode, connection->endIndex, *startNode, connection->startIndex, DataNodeModels::registry->getTypeConverter(outType, intType));
    m_connections[connectionW->id()] = std::make_pair(connectionW.get(), ConnectionKey{connection->key()});
}

void ConnectionsW::add_nodes_and_connections_to_scene(std::vector<Action *> componentsNodesToAdd, std::vector<Connector *> connectorsNodesToAdd, std::vector<Connection *>){

    std::vector<std::tuple<bool, std::unique_ptr<NodeDataModel>,QPointF>> nodesDataModelType;

    // generate components data model
    for(auto componentNodesToAdd: componentsNodesToAdd){
        auto componentDataModel = DataNodeModels::generate_component_data_model(componentNodesToAdd->component);
        nodesDataModelType.emplace_back(std::make_tuple(
            true,
            std::move(componentDataModel),
            componentNodesToAdd->nodePosition
        ));
    }

    auto scenePtr = m_scene.get();

    // generate connectors data model
    for(auto connectorToAdd : connectorsNodesToAdd){
        if(auto connectorDataModel = DataNodeModels::generate_connector_data_model(connectorToAdd); connectorDataModel != nullptr){

            const auto connectorKey     = ConnectorKey{connectorDataModel->key};
            const auto connectorType    = connectorDataModel->type;
            auto connectorDataModelPtr  = connectorDataModel.get();

            // create connections
            connect(connectorDataModelPtr, &ConnectorNodeDataModel::force_node_update_signal, this, [&, connectorKey, connectorType]{
                if(connectorType == BaseNodeDataModel::Type::Connector){
                    for(auto &c : m_connectorsNodes){
                        if(c.first == connectorKey.v){
                            c.second.first->nodeGraphicsObject().update();
                            return;
                        }
                    }
                }else{
                    for(auto &c : m_componentsNodes){
                        if(c.first == connectorKey.v){
                            c.second.first->nodeGraphicsObject().update();
                            return;
                        }
                    }
                }
            });
            connect(connectorDataModelPtr, &ConnectorNodeDataModel::input_connection_validity_signal, this, [=](bool state){
                emit GSignals::get()->connector_input_connection_validity_signal(routine_key(), condition_key(), connectorKey, state);
            });
            connect(connectorDataModelPtr, &ConnectorNodeDataModel::connector_modified_signal, this, [=](ConnectorKey connectorKey, QString name, Arg arg){
                scenePtr->update();
                emit GSignals::get()->connector_node_modified_signal(routine_key(), condition_key(), connectorKey, name, arg);
            });
            // lock ui when popup is opened
            connect(connectorDataModelPtr->node_container(), &BaseNodeContainerW::popup_opened_signal, this, [&](){
                m_view->lock_mouse();
            });
            connect(connectorDataModelPtr->node_container(), &BaseNodeContainerW::popup_closed_signal, this, [&](){
                QTimer::singleShot(200, m_view, &ExFlowView::unlock_mouse);
//                m_hoveredNode       = nullptr;
//                m_hoveredConnection = nullptr;
            });

            // trigger embedded widget signal for calling compute and updating experiment
            connectorDataModel->node_container()->trigger_signals();

            // add model
            nodesDataModelType.emplace_back(std::make_tuple(
                false,
                std::move(connectorDataModel),
                connectorToAdd->pos)
            );
        }        
    }


    // create nodes
    for(auto &nodeDataModelType : nodesDataModelType){

        const bool isComponentNode = std::get<0>(nodeDataModelType);
        auto dataModelPtr   = std::get<1>(nodeDataModelType).get();

        QtNodes::Node &node = m_scene->createNode(std::move(std::get<1>(nodeDataModelType)));
        auto nodePtr  = &node;

        // geometry
        node.nodeGeometry().set_spacing(20);

        // position
        node.nodeGraphicsObject().setPos(std::get<2>(nodeDataModelType));

        if(isComponentNode){ // component node
            auto componentDataModel = dynamic_cast<ComponentNodeDataModel*>(dataModelPtr);

            // store node and its model
            m_componentsNodes[componentDataModel->key] = std::make_pair(nodePtr, componentDataModel);

        }else{
            auto connectorDataModel = dynamic_cast<ConnectorNodeDataModel*>(dataModelPtr);

            // store node and its model
            m_connectorsNodes[connectorDataModel->key] = std::make_pair(nodePtr, connectorDataModel);
        }
    }
}

auto ConnectionsW::create_new_connection(const QtNodes::Connection &c) -> void{

    auto connection = new Connection(ConnectionKey{-1});

    // signal
    connection->startIndex      = c.getPortIndex(PortType::Out);
    auto signalDataModel        = dynamic_cast<BaseNodeDataModel*>(c.getNode(PortType::Out)->nodeDataModel());
    connection->startKey        = signalDataModel->key;
    if(signalDataModel->type == BaseNodeDataModel::Type::Component){
        connection->startType   = Connection::Type::Component;
        connection->signal      = from_view(dynamic_cast<ComponentNodeDataModel*>(signalDataModel)->signalsF[static_cast<size_t>(connection->startIndex)].first.functionName);
    }else{
        connection->startType   = Connection::Type::Connector;
        connection->signal      = QSL("output value ") % QString::number(connection->startIndex);
    }

    // slot
    connection->endIndex        = c.getPortIndex(PortType::In);
    auto slotDataModel          = dynamic_cast<BaseNodeDataModel*>(c.getNode(PortType::In)->nodeDataModel());
    connection->endKey          = slotDataModel->key;
    if(slotDataModel->type == BaseNodeDataModel::Type::Component){
        connection->endType     = Connection::Type::Component;
        connection->slot        = from_view(dynamic_cast<ComponentNodeDataModel*>(slotDataModel)->slotsF[static_cast<size_t>(connection->endIndex)].first.functionName);
    }else{
        connection->endType     = Connection::Type::Connector;
        connection->slot        = QSL("input value ") % QString::number(connection->endIndex);
    }

    connection->startDataType   = c.dataType(PortType::Out).id;
    connection->endDataType     = c.dataType(PortType::In).id;

    m_connections[c.id()] = std::make_pair(&c, ConnectionKey{connection->key()});

    emit GSignals::get()->nodes_connection_created_signal(routine_key(), condition_key(), connection);
}

auto ConnectionsW::connection_deleted(const QtNodes::Connection &c) -> void{

    if(m_hoveredConnection){
        if(m_hoveredConnection->id() == c.id()){
            m_hoveredConnection = nullptr;
        }
    }
    // connection will be removed by internal nodes library, so we have to remove the qid from m_connections now and avoid calling the exp update
    m_connections.erase(c.id());
    emit GSignals::get()->delete_nodes_and_connections_signal(routine_key(), condition_key(), {}, {}, {m_connections[c.id()].second}, false);
}

auto ConnectionsW::force_graphic_update() -> void{

    for(auto &c : m_componentsNodes){
        c.second.first->nodeGraphicsObject().update();

        const int nbIn  = static_cast<int>(c.second.first->nodeDataModel()->nb_Ports(PortType::In));
        const int nbOut = static_cast<int>(c.second.first->nodeDataModel()->nb_Ports(PortType::Out));

        for(int ii = 0; ii < nbIn; ++ii){
            for(auto &connection : c.second.first->nodeState().connections(PortType::In, ii)){
                connection.second->getConnectionGraphicsObject().update();
            }
        }
        for(int ii = 0; ii < nbOut; ++ii){
            for(auto &connection : c.second.first->nodeState().connections(PortType::Out, ii)){
                connection.second->getConnectionGraphicsObject().update();
            }
        }
    }

    for(auto &c : m_connectorsNodes){
        c.second.first->nodeGraphicsObject().update();

        const int nbIn  = static_cast<int>(c.second.first->nodeDataModel()->nb_Ports(PortType::In));
        const int nbOut = static_cast<int>(c.second.first->nodeDataModel()->nb_Ports(PortType::Out));

        for(int ii = 0; ii < nbIn; ++ii){
            for(auto &connection : c.second.first->nodeState().connections(PortType::In, ii)){
                connection.second->getConnectionGraphicsObject().update();
            }
        }
        for(int ii = 0; ii < nbOut; ++ii){
            for(auto &connection : c.second.first->nodeState().connections(PortType::Out, ii)){
                connection.second->getConnectionGraphicsObject().update();
            }
        }
    }

    m_scene->update();
}

QAction *ConnectionsW::generate_connector_action(std::pair<QString, QString> info){
    QAction *action = new QAction();
    action->setText(info.second);
    connect(action, &QAction::triggered, this, [=]{
        auto type = Connector::get_type_from_name(info.first.toStdString());
        auto connector =  new Connector(ConnectorKey{-1}, type.value(), info.first, m_viewMappedPosition);
        emit GSignals::get()->connector_node_created_signal(routine_key(), condition_key(), connector);
    });
    return action;
}

auto ConnectionsW::update_from_condition(Condition *condition) -> void{

    // remove
    bool display = false;
    Bench::start("ConnectionsW update_from_condition 1"sv, display);
        remove_elements_from_condition(condition);
    Bench::stop();

    // add
    Bench::start("ConnectionsW update_from_condition 2"sv, display);
        add_new_elements_from_condition(condition);
    Bench::stop();

    // update
    Bench::start("ConnectionsW update_from_condition 3"sv, display);
        update_elements_from_condition(condition);
    Bench::stop();

    // redraw everything
    Bench::start("ConnectionsW update_from_condition 4"sv, display);
        force_graphic_update();
    Bench::stop();
}

auto ConnectionsW::connector(ConnectorKey connectorKey) -> ConnectorNodeDataModel*{
    if(m_connectorsNodes.count(connectorKey.v) != 0){
        return m_connectorsNodes[connectorKey.v].second;
    }
    return nullptr;
}

auto ConnectionsW::reset() -> void{

    BlockSignalsGuard guard;

    close_all_connectors_windows();

    // delete all connections
    std::set<QUuid> connectionsToDelete;
    for(const auto &connection : m_connections){
        connectionsToDelete.emplace(connection.first);
    }
    m_connections.clear();
    for(const auto& id : connectionsToDelete){
        if(m_scene->connections().count(id) != 0){
            m_scene->deleteConnection(*m_scene->connections().at(id));
        }
    }

    // delete all components nodes
    for(auto &componentNode : m_componentsNodes){
        m_scene->removeNode(*componentNode.second.first);
    }
    m_componentsNodes.clear();

    // delete all connectors nodes
    for(auto &connectorNode : m_connectorsNodes){
        m_scene->removeNode(*connectorNode.second.first);
    }
    m_connectorsNodes.clear();   
}

void ConnectionsW::add_new_elements_from_condition(Condition *condition){

    std::vector<Action*> commonentsNodesToAdd;
    std::vector<Connector*> connectorsToAdd;
    std::vector<Connection*> connectionsToAdd;

    // components nodes
    for(auto &action : condition->actions_with_nodes()){
        if(m_componentsNodes.count(action->component->key()) == 0){ // if new node
            commonentsNodesToAdd.emplace_back(action);
        }
    }

    // connector nodes
    for(auto &connector : condition->connectors){
        if(m_connectorsNodes.count(connector->key()) == 0){ // if new node            
            connectorsToAdd.emplace_back(connector.get());
        }
    }
    add_nodes_and_connections_to_scene(commonentsNodesToAdd, connectorsToAdd, connectionsToAdd);

    // connections
    for(const auto &connection : condition->connections){

        bool found = false;
        for(const auto &connectionW : m_connections){

            if(connectionW.second.second.v == connection->key()){
                found = true;
                break;
            }
        }

        if(!found){
            add_connection(connection.get());
        }
    }
}

void ConnectionsW::update_elements_from_condition(Condition *condition){

    // update selection with QGraphicsItem maybe.. TODO

    // components nodes
    m_availableComponents.clear();
    for(const auto &action : condition->actions){

        bool found = false;
        for(auto &componentNode : m_componentsNodes){
            if(action->component->key() == componentNode.first){

                componentNode.second.second->update_name(action->component->name());

                // update position
                m_scene->setNodePosition(*componentNode.second.first, action->nodePosition);

                // update selection
                componentNode.second.first->nodeGraphicsObject().setSelected(action->nodeSelected);

                found = true;
                break;
            }
        }
        if(!found){
            m_availableComponents[action->component->key()] = std::make_tuple(action->component->type, action->component->name());
        }
    }

    update_components_context_menu();

    // connectors nodes
    for(const auto &connector : condition->connectors){

        for(auto &connectorNode : m_connectorsNodes){

            if(connectorNode.first == connector->key()){

                // update position
                auto pos = m_scene->getNodePosition(*connectorNode.second.first);
                if(!almost_equal(pos.x(),connector->pos.x()) || !almost_equal(pos.y(),connector->pos.y())){
                    m_scene->setNodePosition(*connectorNode.second.first, connector->pos);
                }

                // update arg
                connectorNode.second.second->update_from_connector(*connector);

                // update selection
                connectorNode.second.first->nodeGraphicsObject().setSelected(connector->selected);

                break;
            }
        }

    }
}


void ConnectionsW::remove_elements_from_condition(Condition *condition){


    // connections
    // # find connections to be removed
    std::vector<QUuid> qidToRemove;
    for(const auto& p : m_connections){

        bool found = false;
        for(const auto &connection : condition->connections){

            if(p.second.second.v == connection->key()){
                found = true; // exists
                break;
            }
        }
        if(!found){
            qidToRemove.emplace_back(p.first);
        }
    }

    // # remove connections
    for(const auto &qid : qidToRemove){

        // delete connection from scene
        if(m_scene->connections().count(qid) != 0){
            m_scene->deleteConnection(*m_scene->connections().at(qid));
        }

        // remove from map
        if(m_connections.count(qid) != 0){
            m_connections.erase(qid);
        }
    }

    // components nodes
    // # find nodes to be removed
    std::vector<int> toRemove;
    for(const auto& p : m_componentsNodes){

        bool found = false;
        for(const auto &action : condition->actions){

            if(p.first == action->component->key() && action->nodeUsed){ // remove also if node not displayed
                found = true; // exists
                break;
            }
        }
        if(!found){
            toRemove.emplace_back(p.first);
        }
    }

    // # remove nodes
    for(const auto &id : toRemove){

        // delete node from scene
        if(m_scene->nodes().count(m_componentsNodes[id].first->id()) != 0){
            m_scene->removeNode(*m_componentsNodes[id].first);
        }

        // remove from map
        if(m_componentsNodes.count(id) != 0){
            m_componentsNodes.erase(id);
        }
    }

    // connector nodes
    // # find nodes to be removed
    toRemove.clear();
    for(const auto& p : m_connectorsNodes){                

        bool found = false;
        for(const auto &connector : condition->connectors){

            if(p.first == connector->key()){

                found = true; // exists
                break;
            }
        }
        if(!found){
            toRemove.emplace_back(p.first);
        }
    }

    // # remove nodes
    for(const auto &id : toRemove){

        // delete node from scene
        if(m_scene->nodes().count(m_connectorsNodes[id].first->id()) != 0){
            m_scene->removeNode(*m_connectorsNodes[id].first);
        }

        // remove from map
        if(m_connectorsNodes.count(id) != 0){
            m_connectorsNodes.erase(id);
        }
    }
}


void ConnectionsW::update_components_context_menu(){

    m_contextMenu.componentM.clear();
    for(const auto &compoInfo : m_availableComponents){

        QAction *action = new QAction();
        action->setText(std::get<1>(compoInfo.second));
        const auto componentKey = ComponentKey{compoInfo.first};
        connect(action, &QAction::triggered, this, [&,componentKey]{
            emit GSignals::get()->component_node_created_signal(routine_key(), condition_key(), componentKey, m_viewMappedPosition);
            force_graphic_update();
        });
        m_contextMenu.componentM.addAction(action);
    }
}

ConnectionsContextMenu::ConnectionsContextMenu(ElementKey routineKey, ConditionKey conditionKey) : m_routineKey(routineKey), m_conditionKey(conditionKey){

    addNode.setTitle("Add node");
    generatorM.setTitle("Generator");
    operatorM.setTitle("Operator");
    functionM.setTitle("Function");
    convertorM.setTitle("Convertor");
    actionM.setTitle("Action");
    eventM.setTitle("Event");
    linkM.setTitle("Link");
    flowM.setTitle("Flow");
    displayM.setTitle("Display");
    resourcesM.setTitle("Resources");
    legacyM.setTitle("Legacy");
    componentM.setTitle("Component");

    addNode.addMenu(&generatorM);
    addNode.addMenu(&operatorM);
    addNode.addMenu(&functionM);
    addNode.addMenu(&convertorM);
    addNode.addMenu(&linkM);
    addNode.addMenu(&eventM);
    addNode.addMenu(&displayM);
    addNode.addMenu(&resourcesM);
    addNode.addSeparator();
    addNode.addMenu(&flowM);
    addNode.addMenu(&actionM);
    addNode.addSeparator();
    addNode.addMenu(&componentM);
    addNode.addSeparator();
    addNode.addMenu(&legacyM);

    // generate context menu node action
    removeNodeA = new QAction("Remove node");
    connect(removeNodeA, &QAction::triggered, this, [=](){
        if(auto model = (currentHoveredConnectorNode != nullptr) ? dynamic_cast<BaseNodeDataModel*>(currentHoveredConnectorNode->nodeDataModel()) : nullptr){
            emit delete_node_signal(model->key, model->type == BaseNodeDataModel::Type::Component);
        }
    });

    duplicateAllSelectedNodesA = new QAction("Duplicate selection");
    connect(duplicateAllSelectedNodesA, &QAction::triggered, this, [=](){
        emit GSignals::get()->duplicate_selected_nodes_signal(m_routineKey, m_conditionKey);
    });

    removeAllSelectedNodesA = new QAction("Remove selection");
    connect(removeAllSelectedNodesA, &QAction::triggered, this, [=](){
        emit GSignals::get()->delete_selected_nodes_signal(m_routineKey, m_conditionKey);
    });

    removeConnectionA = new QAction("Remove connection");
    connect(removeConnectionA, &QAction::triggered, this, [=](){
        emit delete_connection_signal(currentHoveredConnection->id());
    });

    copySelectionA = new QAction("Copy selection");
    connect(copySelectionA, &QAction::triggered, this, [=](){
        NodesClipBoard::fromRoutine  = m_routineKey;
        NodesClipBoard::fromCondition= m_conditionKey;
        NodesClipBoard::components   = currentComponentsNodesSelected;
        NodesClipBoard::connectors   = currentConnectorsNodesSelected;
        NodesClipBoard::connections  = currentConnectionSelected;
        NodesClipBoard::enabled      = currentComponentsNodesSelected.size() > 0 || currentConnectorsNodesSelected.size() > 0;
    });

    pasteA = new QAction("Paste");
    connect(pasteA, &QAction::triggered, this, [=](){
        emit GSignals::get()->paste_nodes_clip_board_signal(m_currentMousePosition, m_routineKey, m_conditionKey);
    });

    displayNodeInfoA = new QAction("Show info");
    connect(displayNodeInfoA, &QAction::triggered, this, [=](){
        if(auto model = currentHoveredConnectorNode != nullptr ? dynamic_cast<BaseNodeDataModel*>(currentHoveredConnectorNode->nodeDataModel()) : nullptr){
            if(model->type == BaseNodeDataModel::Type::Component){
                emit GSignals::get()->show_component_node_documentation_signal(ComponentKey{model->key});
            }else{
                emit GSignals::get()->show_connector_node_documentation_signal(m_routineKey, m_conditionKey, ConnectorKey{model->key});
            }
        }
    });

    duplicateConnectorNodeA = new QAction("Duplicate node");
    connect(duplicateConnectorNodeA, &QAction::triggered, this, [=](){
        if(auto model = currentHoveredConnectorNode != nullptr ? dynamic_cast<BaseNodeDataModel*>(currentHoveredConnectorNode->nodeDataModel()) : nullptr){
            if(model->type != BaseNodeDataModel::Type::Component){
                emit duplicate_connector_node_signal(ConnectorKey{model->key});
            }
        }
    });

    unselectSelectionA = new QAction("Unselect");
    connect(unselectSelectionA, &QAction::triggered, this, [=](){
        emit unselect_signal();
    });
}

auto ConnectionsContextMenu::execute(
    QPointF globalPos,
    QPointF mappedPos,
    QtNodes::Node *nodeUnderMouse,
    QtNodes::Connection *connectionUnderMouse,
    const std::vector<ComponentKey> &componentsKey,
    const std::vector<ConnectorKey> &connectorsKey,
    const std::vector<ConnectionKey> &connectionsKey) -> void{

    m_currentMousePosition = globalPos;

    // copy current selection
    currentComponentsNodesSelected = componentsKey;
    currentConnectorsNodesSelected = connectorsKey;
    currentConnectionSelected      = connectionsKey;

    const bool selectionEmpty            = componentsKey.size() == 0 && connectorsKey.size() == 0 && connectionsKey.size() == 0;
    const bool noOnlyConnectionsSelected = componentsKey.size() > 0 || connectorsKey.size() > 0;

    // store hovered elements
    currentHoveredConnectorNode = nodeUnderMouse;
    currentHoveredConnection    = connectionUnderMouse;

    // hover actions
    bool canDuplicateHoveredNode    = false;
    bool canRemoveHoveredNode       = false;
    bool canRemoveHoveredConnection = false;
    bool canDisplayInfoAboutHoveredNode = false;
    // selection actions
    bool canCopySelection           = false;
    bool canRemoveSelection         = false;
    bool canUnselectSelection       = false;
    // add actions
    bool canAddNewNodes             = false;
    // others actions
    bool canPastCopySelection       = NodesClipBoard::enabled;
//    bool canMoveCameraToClosestNode = false;

    auto hoveredModel = currentHoveredConnectorNode ? dynamic_cast<BaseNodeDataModel*>(currentHoveredConnectorNode->nodeDataModel()) : nullptr;

    if(selectionEmpty){
        // no selection
        if(!nodeUnderMouse && !connectionUnderMouse){
            // no hovering on anything
            canAddNewNodes =true;
        }else if(hoveredModel){
            // hovering on node
            canPastCopySelection = false;            
            canDuplicateHoveredNode = hoveredModel->type != BaseNodeDataModel::Type::Component;
            canRemoveHoveredNode    = true;
            canDisplayInfoAboutHoveredNode = true;
        }else if(connectionUnderMouse){
            // hovering on connection
            canRemoveHoveredConnection = true;
        }
    }else if(!selectionEmpty){
        // selection
        canCopySelection = noOnlyConnectionsSelected;
        canRemoveSelection   = true;
        canUnselectSelection = true;        
        if(!nodeUnderMouse && !connectionUnderMouse){
            // no hovering on anything
            canAddNewNodes = true;
        }else if(hoveredModel){
            // hovering on node
            canPastCopySelection = false;
            canDuplicateHoveredNode = hoveredModel->type != BaseNodeDataModel::Type::Component;
            canRemoveHoveredNode    = true;
            canDisplayInfoAboutHoveredNode = true;
        }else if(connectionUnderMouse){
            // hovering on connection
            canRemoveHoveredConnection = true;
        }
    }

    // disable copy/paste TODO
    canCopySelection = false;
    canPastCopySelection = false;

    // create menu
    QMenu topMenu;
    bool displayHoveredMenu   = canDuplicateHoveredNode || canRemoveHoveredNode || canRemoveHoveredConnection || canDisplayInfoAboutHoveredNode;
    bool displaySelectionMenu = canCopySelection || canRemoveSelection || canUnselectSelection;
    bool displayAddNodesMenu  = canAddNewNodes;
    bool displayOtherMenu     = canPastCopySelection;

    // hover menu
    if(displayHoveredMenu){

        if(canDisplayInfoAboutHoveredNode){
            topMenu.addAction(displayNodeInfoA);
        }

        if(canDuplicateHoveredNode){
            topMenu.addAction(duplicateConnectorNodeA);
        }

        if(canRemoveHoveredNode){
            topMenu.addAction(removeNodeA);            
        }

        if(canRemoveHoveredConnection){
            topMenu.addAction(removeConnectionA);
        }        
    }

    // selection menu
    if(displaySelectionMenu){

        if(displayHoveredMenu){
            topMenu.addSeparator();
        }
        if(canUnselectSelection){
            topMenu.addAction(unselectSelectionA);
        }
        if(canRemoveSelection){
            topMenu.addAction(removeAllSelectedNodesA);
        }

        if(canCopySelection){
            topMenu.addAction(copySelectionA);
        }
    }

    // add menu
    if(displayAddNodesMenu){

        if(displayHoveredMenu || displaySelectionMenu){
            topMenu.addSeparator();
        }

        if(canAddNewNodes){
            topMenu.addMenu(&addNode);
        }
    }

    // always other menu
    if(displayOtherMenu){

        if(displayHoveredMenu || displaySelectionMenu || displayAddNodesMenu){
            topMenu.addSeparator();
        }

        if(canPastCopySelection){
            topMenu.addAction(pasteA);
        }
    }

    emit open_menu_signal();
    topMenu.exec(globalPos.toPoint());
    emit close_menu_signal();
}

void ExFlowScene::drawBackground(QPainter *painter, const QRectF &rect){
    Bench::start("ExFlowScene drawBackground"sv);
        QtNodes::FlowScene::drawBackground(painter, rect);
    Bench::stop();
}

void ExFlowScene::drawForeground(QPainter *painter, const QRectF &rect){
    Bench::start("ExFlowScene drawForeground"sv);
        QtNodes::FlowScene::drawForeground(painter, rect);
    Bench::stop();
}

void ExFlowScene::drawItems(QPainter *painter, int numItems, QGraphicsItem *items[], const QStyleOptionGraphicsItem options[], QWidget *widget){
    Bench::start("ExFlowScene drawItems"sv);
        QtNodes::FlowScene::drawItems(painter, numItems, items, options, widget);
    Bench::stop();
}
