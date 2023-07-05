
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


#include "condition.hpp"

// base
#include "utility/unordered_map.hpp"

// qt-utility
#include "qt_logger.hpp"

using namespace tool;
using namespace tool::ex;

Condition::Condition(QString n, ConditionKey id, SecondsTS duration, double uiScale, double uiSize)
    : name(n), duration(duration), scale(uiScale), uiFactorSize(uiSize), m_key(IdKey::Type::Condition, id.v){
}

void Condition::apply_condition(const Condition *conditionToCopy, bool copyActions, bool copyConnections){

    if(copyActions){

        duration     = conditionToCopy->duration;
        scale        = conditionToCopy->scale;
        uiFactorSize = conditionToCopy->uiFactorSize;
        selected     = conditionToCopy->selected;

        actions.clear();
        for(auto &actionToCopy : conditionToCopy->actions){
            auto action = Action::copy_with_new_element_id(*actionToCopy);
            if(!copyConnections){
                action->nodeUsed = false;
                action->nodePosition = QPoint(0,0);
            }
            actions.emplace_back(std::move(action));
        }
    }

    if(copyConnections){

        // copy all connectors nodes
        connectors.clear();        
        umap<int,int> keysMapping;
        for(auto &connectorToCopy : conditionToCopy->connectors){
            connectors.emplace_back(Connector::copy_with_new_element_id(*connectorToCopy));
            keysMapping[connectorToCopy->key()] = connectors[connectors.size()-1]->key();
        }

        // apply action component node used state
        for(auto &actionToCopy : conditionToCopy->actions){
            if(auto action = get_action_from_component_key(ComponentKey{actionToCopy->component->key()}, false); action != nullptr){
                action->nodeUsed     = actionToCopy->nodeUsed;
                action->nodePosition = actionToCopy->nodePosition;
            }
        }

        connections.clear();
        for(auto &connection : conditionToCopy->connections){

            if(connection->startType == Connection::Type::Component){
                if(auto component = get_component_from_key(ComponentKey{connection->startKey}, false); component == nullptr){
                    QtLogger::message(QSL("From condition ") % conditionToCopy->name % QSL(", starting connection node component with key ") %
                                    QString::number(connection->startKey) % QSL(" not available in condition ") % name % QSL(", cannot copy it. "));
                    continue;
                }
            }
            if(connection->endType == Connection::Type::Component){
                if(auto component = get_component_from_key(ComponentKey{connection->endKey}, false); component == nullptr){
                    QtLogger::message(QSL("From condition ") % conditionToCopy->name % QSL(", ending connection node component with key ")
                                    % QString::number(connection->endKey) % QSL(" not available in condition ") % name % QSL(", cannot copy it. "));
                    continue;
                }
            }

            connections.emplace_back(Connection::copy_with_new_element_id(*connection, keysMapping));
        }
    }
}

int Condition::get_action_id_from_key(ActionKey actionKey, bool displayError) const{

    auto actionFound = std::find_if(actions.begin(), actions.end(), [actionKey](const std::unique_ptr<Action> & action){
        return action->key() == actionKey.v;
    });
    if(actionFound != actions.end()){
        return static_cast<int>(std::distance(actions.begin(), actionFound));
    }

    if(displayError){
        QtLogger::error(QSL("Action with key ") % QString::number(actionKey.v) % QSL(" not found"));
    }

    return -1;
}

Action *Condition::get_action_from_key(ActionKey actionKey, bool displayError) const{

    auto actionFound = std::find_if(actions.begin(), actions.end(), [actionKey](const std::unique_ptr<Action> &action){
        return action->key() == actionKey.v;
    });
    if(actionFound != actions.end()){
        return actionFound->get();
    }

    if(displayError){
        QtLogger::error(QSL("Cannot find Action with key [") % QString::number(actionKey.v) %
                        QSL("] from condition [") % name % QSL("] wtih key [") % QString::number(key()) % QSL("]"));
    }

    return nullptr;
}

Action *Condition::get_action_from_component_key(ComponentKey componentKey, bool displayError) const{

    auto actionFound = std::find_if(actions.begin(), actions.end(), [componentKey](const std::unique_ptr<Action> &action){
        return action->component->key() == componentKey.v;
    });
    if(actionFound != actions.end()){
        return actionFound->get();
    }


    if(displayError){
        QtLogger::error(QSL("Cannot find Action with component key [") % QString::number(componentKey.v) %
                        QSL("] from condition [") % name % QSL("] wtih key [") % QString::number(key()) % QSL("]"));
    }

    return nullptr;
}

Connection *Condition::get_connection_from_key(ConnectionKey connectionKey, bool displayError) const{

    auto connectionFound = std::find_if(connections.begin(), connections.end(), [connectionKey](const std::unique_ptr<Connection> &connection){
        return connection->key() == connectionKey.v;
    });
    if(connectionFound != connections.end()){
        return connectionFound->get();
    }

    if(displayError){
        QtLogger::error(QSL("Connection with key ") % QString::number(connectionKey.v) % QSL(" not found"));
    }

    return nullptr;
}

Connector *Condition::get_connector_from_key(ConnectorKey connectorKey, bool displayError) const{

    auto connectorFound = std::find_if(connectors.begin(), connectors.end(), [connectorKey](const std::unique_ptr<Connector> &connector){
        return connector->key() == connectorKey.v;
    });
    if(connectorFound != connectors.end()){
        return connectorFound->get();
    }

    if(displayError){
        QtLogger::error(QSL("Connector with key ") % QString::number(connectorKey.v) % QSL(" not found"));
    }

    return nullptr;
}

Component *Condition::get_component_from_key(ComponentKey componentKey, bool displayError) const{
    if(auto action = get_action_from_component_key(componentKey, displayError); action != nullptr){
        return action->component;
    }
    return nullptr;
}

Action *Condition::get_action_from_id(RowId  idTab, bool displayError) const{

    if(idTab.v < to_int(actions.size())){
        return actions[to_size_t(idTab.v)].get();
    }

    if(displayError){
        QtLogger::error(QSL("Action with id ") % QString::number(idTab.v) % QSL(" not found"));
    }

    return nullptr;
}

Connection *Condition::get_connection_from_id(RowId  idTab, bool displayError) const{

    if(idTab.v < to_int(connections.size())){
        return connections[to_size_t(idTab.v)].get();
    }

    if(displayError){
        QtLogger::error(QSL("Connection with id ") % QString::number(idTab.v) % QSL(" not found"));
    }

    return nullptr;
}

Connector *Condition::get_connector_from_id(RowId  idTab, bool displayError) const{

    if(idTab.v < to_int(connectors.size())){
        return connectors[to_size_t(idTab.v)].get();
    }

    if(displayError){
        QtLogger::error(QSL("Connector with id ") % QString::number(idTab.v) % QSL(" not found"));
    }

    return nullptr;
}

void Condition::update_max_length(SecondsTS length){

    duration = length;
    for(auto &action : actions){
        action->update_intervals_with_max_length(length);
    }
}

void Condition::check_integrity(){

    // connections
    size_t countBefore = connections.size();
    {
        std::unordered_set<int> seen;
        auto newEnd = std::remove_if(connections.begin(), connections.end(), [&seen](const std::unique_ptr<Connection> &connection){
            if (seen.find(connection->key()) != std::end(seen)){
                return true;
            }
            seen.insert(connection->key());
            return false;
        });
        connections.erase(newEnd, connections.end());
    }
    size_t countAfter = connections.size();

    if(countBefore > countAfter){
        QtLogger::warning(
            QSL("Remove ") % QString::number(countBefore - countAfter) % QSL(" duplicated connections from condition ") %
            name % QSL(" with id ") % QString::number(key()) % QSL(".")
        );
    }

    // connectors
    countBefore = connectors.size();
    {
        std::unordered_set<int> seen;
        auto newEnd = std::remove_if(connectors.begin(), connectors.end(), [&seen](const std::unique_ptr<Connector> &connector){
            if (seen.find(connector->key()) != std::end(seen)){
                return true;
            }
            seen.insert(connector->key());
            return false;
        });
        connectors.erase(newEnd, connectors.end());
    }
    countAfter = connectors.size();
    if(countBefore > countAfter){
        QtLogger::warning(
            QSL("Remove ") % QString::number(countBefore - countAfter) % QSL(" duplicated connectors from condition ") %
            name % QSL(" with id ") % QString::number(key()) % QSL(".")
        );
    }

    // actions
    countBefore = actions.size();
    {
        std::unordered_set<int> seen;
        auto newEnd = std::remove_if(actions.begin(), actions.end(), [&seen](const std::unique_ptr<Action> &action){
            if (seen.find(action->key()) != std::end(seen)){
                return true;
            }
            seen.insert(action->key());
            return false;
        });
        actions.erase(newEnd, actions.end());
    }
    countAfter = actions.size();
    if(countBefore > countAfter){
        QtLogger::warning(
            QSL("Remove ") % QString::number(countBefore - countAfter) % QSL(" duplicated actions from condition ") %
            name % QSL(" with id ") % QString::number(key()) % QSL(".")
        );
    }

    for(auto &action : actions){
        action->check_integrity();
    }
}

bool Condition::contains_set_key(SetKey setKeyToCheck) const{

    auto keyFound = std::find_if(std::begin(setsKeys), std::end(setsKeys), [setKeyToCheck](SetKey setKey){
        return setKey.v == setKeyToCheck.v;
    });
    return keyFound != std::end(setsKeys);
}

bool Condition::contains_same_set_keys(const std::vector<SetKey> setKeysToCheck) const{

    if(setKeysToCheck.size() != setsKeys.size()){
        return false;
    }

    for(const auto &setKeyToCheck : setKeysToCheck){
        if(!contains_set_key(setKeyToCheck)){
            return false;
        }
    }
    return true;
}

void Condition::move_connector(ConnectorKey connectorKey, QPointF pos){

    if(auto connector = get_connector_from_key(connectorKey); connector != nullptr){
        connector->pos = pos;
    }
}

void Condition::duplicate_connector(ConnectorKey connectorKey){

    if(auto connector = get_connector_from_key(connectorKey); connector != nullptr){
        auto duplicatedConnector = Connector::copy_with_new_element_id(*connector);
        duplicatedConnector->pos += QPointF(50,50);
        connectors.push_back(std::move(duplicatedConnector));
    }
}

auto Condition::duplicate_connector(QPointF position, Connector *connector) -> void{
    auto duplicatedConnector = Connector::copy_with_new_element_id(*connector);
    duplicatedConnector->pos += position;
    connectors.push_back(std::move(duplicatedConnector));
}

void Condition::modify_connector(ConnectorKey connectorKey, QString name, Arg arg){

    if(auto connector = get_connector_from_key(connectorKey); connector != nullptr){
        connector->name = name;
        connector->arg = arg;
    }
}


void Condition::move_component(ComponentKey componentKey, QPointF pos){
    if(auto action = get_action_from_component_key(componentKey); action != nullptr){
        action->nodePosition = pos;
    }
}

void Condition::remove_action(ActionKey actionKey){

    // remove connections associated to action
    if(auto action = get_action_from_key(actionKey); action != nullptr){

        while(auto searchConnections = true){
            auto connectionToDelete = std::find_if(connections.begin(), connections.end(), [action](std::unique_ptr<Connection> &connection){

                if((connection->startType == Connection::Type::Component) && (connection->startKey == action->component->key())){
                    return true;
                }

                if((connection->endType == Connection::Type::Component) && (connection->endKey == action->component->key())){
                    return true;
                }

                return false;
            });

            if(connectionToDelete != connections.end()){
                connections.erase(connectionToDelete);
                continue;
            }
            break;
        }

        auto actionToDelete = std::find_if(actions.begin(), actions.end(), [actionKey](std::unique_ptr<Action> &action){
            return action->key() == actionKey.v;
        });
        if(actionToDelete != actions.end()){
            actions.erase(actionToDelete);
            return;
        }
    }

    QtLogger::error(QSL("Cannot remove action with key ") % QString::number(actionKey.v));
}

void Condition::remove_all_actions(){

    while(auto searchConnections = true){
        auto connectionToDelete = std::find_if(connections.begin(), connections.end(), [](std::unique_ptr<Connection> &connection){
            return ((connection->startType == Connection::Type::Component) ||
                    (connection->endType == Connection::Type::Component));
        });

        if(connectionToDelete != connections.end()){
            connections.erase(connectionToDelete);
            continue;
        }
        break;
    }
    actions.clear();
}

auto Condition::paste_clipboard() -> void{
//    struct NodesClipBoard{
//        static inline bool enabled = false;
//        static inline std::vector<ComponentKey> components = {};
//        static inline std::vector<ConnectorKey> connectors = {};
//        static inline std::vector<ConnectionKey> connections = {};
//    };
}

void Condition::remove_connection(ConnectionKey connectionKey){

    auto connectionToDelete = std::find_if(connections.begin(), connections.end(), [connectionKey](std::unique_ptr<Connection> &connection){
        return connection->key() == connectionKey.v;
    });
    if(connectionToDelete != connections.end()){
        connections.erase(connectionToDelete);
        return;
    }
}

void Condition::remove_connector(ConnectorKey connectorKey){

    // remove connections associated to connector
    while(auto searchConnections = true){
        auto connectionToDelete = std::find_if(connections.begin(), connections.end(), [connectorKey](std::unique_ptr<Connection> &connection){

            if((connection->endType == Connection::Type::Connector) && (connection->endKey == connectorKey.v)){
                return true;
            }
            if((connection->startType == Connection::Type::Connector) && (connection->startKey == connectorKey.v)){
                return true;
            }
            return false;
        });

        if(connectionToDelete != connections.end()){
            connections.erase(connectionToDelete);
            continue;
        }
        break;
    }

    // remove connectors
    auto connectorToDelete = std::find_if(connectors.begin(), connectors.end(), [connectorKey](std::unique_ptr<Connector> &connector){
        return connector->key() == connectorKey.v;
    });
    if(connectorToDelete != connectors.end()){
        connectors.erase(connectorToDelete);
        return;
    }
}


void Condition::remove_component_node(ComponentKey componentKey){

    // remove connections associated to component
    while(auto searchConnections = true){
        auto connectionToDelete = std::find_if(connections.begin(), connections.end(), [componentKey](std::unique_ptr<Connection> &connection){

            if((connection->endType == Connection::Type::Component) && (connection->endKey == componentKey.v)){
                return true;
            }
            if((connection->startType == Connection::Type::Component) && (connection->startKey == componentKey.v)){
                return true;
            }
            return false;
        });

        if(connectionToDelete != connections.end()){
            connections.erase(connectionToDelete);
            continue;
        }
        break;
    }

    if(auto action = get_action_from_component_key(componentKey); action != nullptr){
        action->nodeUsed = false;
    }
}

void Condition::move_action_up(ActionKey actionKey){
    if(int idAction = get_action_id_from_key(actionKey); idAction > 0){
        std::swap(actions[to_size_t(idAction)], actions[to_size_t(idAction-1)]);
    }
}

void Condition::move_action_down(ActionKey actionKey){

    if(int idAction = get_action_id_from_key(actionKey); idAction > -1 && idAction < to_int(actions.size()-1)){
        std::swap(actions[to_size_t(idAction)], actions[to_size_t(idAction+1)]);
    }
}


std::vector<Action *> Condition::actions_with_nodes() const{

    auto count = std::count_if(
                actions.begin(),
                actions.end(), [](const std::unique_ptr<Action> &action){
        return action->nodeUsed;
    });

    std::vector<Action*> actionsWithNodes;
    actionsWithNodes.reserve(static_cast<size_t>(count));
    for(const auto &action : actions){
        if(action->nodeUsed){
            actionsWithNodes.emplace_back(action.get());
        }
    }
    return actionsWithNodes;
}


std::unique_ptr<Condition> Condition::duplicate(const Condition &conditionToCopy){

    std::unique_ptr<Condition> condition = std::make_unique<Condition>(
        conditionToCopy.name,
        ConditionKey{-1},
        conditionToCopy.duration,
        conditionToCopy.scale,
        conditionToCopy.uiFactorSize
    );

    condition->setsKeys = conditionToCopy.setsKeys;
    condition->apply_condition(&conditionToCopy, true, true);

    return condition;
}
