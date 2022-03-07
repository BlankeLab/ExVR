
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

// qt-utility
#include "qt_logger.hpp"
#include "qt_str.hpp"

// local
#include "element.hpp"
#include "condition.hpp"

namespace tool::ex {

struct Routine;
using RoutineUP = std::unique_ptr<Routine>;

struct Routine : public Element {

    Routine(QString n, int id, QString infos = "");

    static RoutineUP copy_with_new_element_id(const Routine &routineToCopy, const QString &newName);

    void set_as_randomizer(bool randomizer);

    void select_condition(ConditionKey conditionKey);
    void move_condition_down(RowId id);
    void move_condition_up(RowId id);

    void create_nodes_connection(ConditionKey conditionKey, Connection *connection);
    void delete_nodes_connection(ConditionKey conditionKey, ConnectionKey connectionKey);

    void create_connector_node(ConditionKey conditionKey, Connector *connector);
    void move_connector_node(ConditionKey conditionKey, ConnectorKey connectorKey, QPointF pos);
    void duplicate_connector_node(ConditionKey conditionKey, ConnectorKey connectorKey);
    void modify_connector(ConditionKey conditionKey, ConnectorKey connectorKey, QString connectorName, Arg arg);
    void set_connector_input_connection_validity(ConditionKey conditionKey, ConnectorKey connectorKey, bool state);

    void fill_actions_from_condition(ConditionKey conditionKey);
    void clean_actions_from_condition(ConditionKey conditionKey);
    void delete_actions_from_condition(ConditionKey conditionKey);

    void check_integrity() override;


    Condition *get_condition(ConditionKey conditionKey) const;

    QString to_string() const;

    bool isARandomizer = false;
    std_v1<ConditionUP> conditions;

//    std_v1<ConditionUP> ghostsConditions; // condition that could be restored
};

static bool operator<(const RoutineUP &l, const RoutineUP &r){
    if(l->key() == r->key()){
        return false;
    }
    return true;
}

static bool operator==(const RoutineUP &l, const RoutineUP &r){
    return !(l < r) && !(r < l);
}

}
