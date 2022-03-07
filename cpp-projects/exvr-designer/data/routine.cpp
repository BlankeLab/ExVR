
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

#include "routine.hpp"

using namespace tool::ex;

Routine::Routine(QString n, int id, QString infos) : Element(Type::Routine, n, id, infos){
}

RoutineUP Routine::copy_with_new_element_id(const Routine &routineToCopy, const QString &newName){

    RoutineUP routine = std::make_unique<Routine>(newName, -1, routineToCopy.informations);

    routine->conditions.reserve(routineToCopy.conditions.size());
    for(const auto &conditionToCopy : routineToCopy.conditions){
        routine->conditions.emplace_back(Condition::duplicate(*conditionToCopy.get()));
    }

    return routine;
}

void Routine::set_as_randomizer(bool randomizer){

    isARandomizer = randomizer;
    for(auto &condition : conditions){
        condition->actions.clear();
        condition->connections.clear();
        condition->connectors.clear();
    }
//    ghostsConditions.clear();
}

void Routine::select_condition(ConditionKey conditionKey){
    for(const auto &condition : conditions){
        condition->selected = (condition->key() == conditionKey.v);
    }
}

void Routine::move_condition_down(RowId id){
    if(id.v + 1 < to_signed(conditions.size())){
        std::swap(conditions[id.v], conditions[id.v+1]);
    }
}

void Routine::move_condition_up(RowId id){
    if(id.v > 0){
        std::swap(conditions[id.v], conditions[id.v-1]);
    }
}

void Routine::create_nodes_connection(ConditionKey conditionKey, Connection *connection){
    if(auto condition = get_condition(conditionKey); condition != nullptr){
        condition->connections.emplace_back(std::unique_ptr<Connection>(connection));
    }
}

void Routine::delete_nodes_connection(ConditionKey conditionKey, ConnectionKey connectionKey){
    if(auto condition = get_condition(conditionKey); condition != nullptr){
        condition->remove_connection(connectionKey);
    }
}

void Routine::create_connector_node(ConditionKey conditionKey, Connector *connector){
    if(auto condition = get_condition(conditionKey); condition != nullptr){
        condition->connectors.emplace_back(std::unique_ptr<Connector>(connector));
    }
}

void Routine::move_connector_node(ConditionKey conditionKey, ConnectorKey connectorKey, QPointF pos){
    if(auto condition = get_condition(conditionKey); condition != nullptr){
        condition->move_connector(connectorKey, pos);
    }
}

void Routine::duplicate_connector_node(ConditionKey conditionKey, ConnectorKey connectorKey){
    if(auto condition = get_condition(conditionKey); condition != nullptr){
        condition->duplicate_connector(connectorKey);
    }
}

void Routine::modify_connector(ConditionKey conditionKey, ConnectorKey connectorKey, QString connectorName, Arg arg){
    if(auto condition = get_condition(conditionKey); condition != nullptr){
        condition->modify_connector(connectorKey, connectorName, arg);
    }
}

void Routine::set_connector_input_connection_validity(ConditionKey conditionKey, ConnectorKey connectorKey, bool state){
    if(auto condition = get_condition(conditionKey); condition != nullptr){
        if(auto connector = condition->get_connector_from_key(connectorKey); connector != nullptr){
            connector->inputValidity = state;
        }
    }
}

void Routine::fill_actions_from_condition(ConditionKey conditionKey){
    if(auto condition = get_condition(conditionKey); condition != nullptr){
        for(auto &action : condition->actions){
            action->timelineUpdate->fill(condition->duration);
            action->timelineVisibility->fill(condition->duration);
        }
    }
}

void Routine::clean_actions_from_condition(ConditionKey conditionKey){
    if(auto condition = get_condition(conditionKey); condition != nullptr){
        for(auto &action : condition->actions){
            action->timelineUpdate->clean();
            action->timelineVisibility->clean();
        }
    }
}

void Routine::delete_actions_from_condition(ConditionKey conditionKey){
    if(auto condition = get_condition(conditionKey); condition != nullptr){
        condition->remove_all_actions();
    }
}

void Routine::check_integrity(){

    size_t countBefore = conditions.size();

    std::sort(conditions.begin(), conditions.end());
    conditions.erase(std::unique(conditions.begin(), conditions.end()), conditions.end());

    size_t countAfter = conditions.size();
    if(countBefore > countAfter){
        QtLogger::warning(
            QSL("Remove ") % QString::number(countBefore - countAfter) % QSL(" duplicated conditions from routine ") %
            name() % QSL(" with id ") % QString::number(key()) % QSL(".")
        );
    }

    for(auto &condition : conditions){
        condition->check_integrity();
    }
}



Condition *Routine::get_condition(ConditionKey conditionKey) const{
    for(const auto &condition : conditions){
        if(condition->key() == conditionKey.v){
            return condition.get();
        }
    }

    QtLogger::error(QSL("Condition with key ") % QString::number(conditionKey.v) % QSL(" not found."));
    return nullptr;
}

QString Routine::to_string() const{return QSL("Routine(") % name() % QSL("|") % QString::number(key()) % QSL(")");}
