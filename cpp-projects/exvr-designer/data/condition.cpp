
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

// qt-utility
#include "qt_logger.hpp"

using namespace tool;
using namespace tool::ex;

Condition::Condition(QString n, ConditionKey id, SecondsTS duration, double uiScale, double uiSize)
    : key(IdKey::Type::Condition, id.v), name(n), duration(duration), scale(uiScale), uiFactorSize(uiSize){
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
            }
            actions.emplace_back(std::move(action));
        }
    }

    if(copyConnections){

        // copy all connectors nodes
        connectors.clear();
        std::unordered_map<int,int> keysMapping;
        for(auto &connectorToCopy : conditionToCopy->connectors){
            connectors.emplace_back(Connector::copy_with_new_element_id(*connectorToCopy));
            keysMapping[connectorToCopy->key()] = connectors[connectors.size()-1]->key();
        }

        // apply action component node used state
        for(auto &actionToCopy : conditionToCopy->actions){
            if(auto action = get_action_from_component_key(ComponentKey{actionToCopy->component->key()}, false); action != nullptr){
                action->nodeUsed = actionToCopy->nodeUsed;
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

    auto actionFound = std::find_if(actions.begin(), actions.end(), [actionKey](const ActionUP & action){
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

    auto actionFound = std::find_if(actions.begin(), actions.end(), [actionKey](const ActionUP &action){
        return action->key() == actionKey.v;
    });
    if(actionFound != actions.end()){
        return actionFound->get();
    }

    if(displayError){
        QtLogger::error(QSL("Action with key ") % QString::number(actionKey.v) % QSL(" not found"));
    }

    return nullptr;
}

Action *Condition::get_action_from_component_key(ComponentKey componentKey, bool displayError) const{

    auto actionFound = std::find_if(actions.begin(), actions.end(), [componentKey](const ActionUP &action){
        return action->component->key() == componentKey.v;
    });
    if(actionFound != actions.end()){
        return actionFound->get();
    }

    if(displayError){
        QtLogger::error(QSL("Component with key ") % QString::number(componentKey.v) % QSL(" not found"));
    }

    return nullptr;
}

Connection *Condition::get_connection_from_key(ConnectionKey connectionKey, bool displayError) const{

    auto connectionFound = std::find_if(connections.begin(), connections.end(), [connectionKey](const ConnectionUP &connection){
        return connection->key() == connectionKey.v;
    });
    if(connectionFound != connections.end()){
        return connectionFound->get();
    }

    if(displayError){
        QtLogger::error(QSL("Connector with key ") % QString::number(connectionKey.v) % QSL(" not found"));
    }

    return nullptr;
}

Connector *Condition::get_connector_from_key(ConnectorKey connectorKey, bool displayError) const{

    auto connectorFound = std::find_if(connectors.begin(), connectors.end(), [connectorKey](const ConnectorUP &connector){
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

    if(idTab.v < to_signed(actions.size())){
        return actions[to_unsigned(idTab.v)].get();
    }

    if(displayError){
        QtLogger::error(QSL("Action with id ") % QString::number(idTab.v) % QSL(" not found"));
    }

    return nullptr;
}

Connection *Condition::get_connection_from_id(RowId  idTab, bool displayError) const{

    if(idTab.v < to_signed(connections.size())){
        return connections[to_unsigned(idTab.v)].get();
    }

    if(displayError){
        QtLogger::error(QSL("Connection with id ") % QString::number(idTab.v) % QSL(" not found"));
    }

    return nullptr;
}

Connector *Condition::get_connector_from_id(RowId  idTab, bool displayError) const{

    if(idTab.v < to_signed(connectors.size())){
        return connectors[to_unsigned(idTab.v)].get();
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
    std::sort(connections.begin(), connections.end());
    connections.erase(std::unique(connections.begin(), connections.end()), connections.end());

    size_t countAfter = connections.size();
    if(countBefore > countAfter){
        QtLogger::warning(
            QSL("Remove ") % QString::number(countBefore - countAfter) % QSL(" duplicated connections from condition ") %
            name % QSL(" with id ") % QString::number(key()) % QSL(".")
        );
    }

    // connectors
    countBefore = connectors.size();
    std::sort( connectors.begin(), connectors.end() );
    connectors.erase( std::unique( connectors.begin(), connectors.end() ), connectors.end() );
    countAfter = connectors.size();
    if(countBefore > countAfter){
        QtLogger::warning(
            QSL("Remove ") % QString::number(countBefore - countAfter) % QSL(" duplicated connectors from condition ") %
            name % QSL(" with id ") % QString::number(key()) % QSL(".")
        );
    }

    // actions
    countBefore = actions.size();
    std::sort( actions.begin(), actions.end() );
    actions.erase( std::unique( actions.begin(), actions.end() ), actions.end() );
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

bool Condition::contains_set_key(int setKeyToCheck) const{

    auto keyFound = std::find_if(std::begin(setsKeys), std::end(setsKeys), [setKeyToCheck](int setKey){
        return setKey == setKeyToCheck;
    });
    return keyFound != std::end(setsKeys);
}

bool Condition::contains_same_set_keys(const std_v1<int> setKeysToCheck) const{

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
        connectors.emplace_back(std::move(duplicatedConnector));
    }
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
            auto connectionToDelete = std::find_if(connections.begin(), connections.end(), [action](ConnectionUP &connection){

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

        auto actionToDelete = std::find_if(actions.begin(), actions.end(), [actionKey](ActionUP &action){
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
        auto connectionToDelete = std::find_if(connections.begin(), connections.end(), [](ConnectionUP &connection){
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

void Condition::remove_connection(ConnectionKey connectionKey){

    auto connectionToDelete = std::find_if(connections.begin(), connections.end(), [connectionKey](ConnectionUP &connection){
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
        auto connectionToDelete = std::find_if(connections.begin(), connections.end(), [connectorKey](ConnectionUP &connection){

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
    auto connectorToDelete = std::find_if(connectors.begin(), connectors.end(), [connectorKey](ConnectorUP &connector){
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
        auto connectionToDelete = std::find_if(connections.begin(), connections.end(), [componentKey](ConnectionUP &connection){

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
        std::swap(actions[to_unsigned(idAction)], actions[to_unsigned(idAction-1)]);
    }
}

void Condition::move_action_down(ActionKey actionKey){

    if(int idAction = get_action_id_from_key(actionKey); idAction > -1 && idAction < to_signed(actions.size()-1)){
        std::swap(actions[to_unsigned(idAction)], actions[to_unsigned(idAction+1)]);
    }
}


std_v1<Action *> Condition::actions_with_nodes() const{

    auto count = std::count_if(
                actions.begin(),
                actions.end(), [](const ActionUP &action){
        return action->nodeUsed;
    });

    std_v1<Action*> actionsWithNodes;
    actionsWithNodes.reserve(static_cast<size_t>(count));
    for(const auto &action : actions){
        if(action->nodeUsed){
            actionsWithNodes.emplace_back(action.get());
        }
    }
    return actionsWithNodes;
}


ConditionUP Condition::duplicate(const Condition &conditionToCopy){

    ConditionUP condition = std::make_unique<Condition>(
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
