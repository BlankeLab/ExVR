
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
#include "utility/types.hpp"

// qt_utility
#include "qt_str.hpp"

// local
#include "action.hpp"
#include "connection.hpp"
#include "connector.hpp"

namespace tool::ex {

struct Condition;
using ConditionUP = std::unique_ptr<Condition>;

struct Condition  {


    Condition(QString n, ConditionKey id, SecondsTS duration, double uiScale, double uiSize );

    static ConditionUP generate_new_default(){
        return std::make_unique<Condition>(QSL("default"), ConditionKey{-1}, SecondsTS{100.}, 10., 1.);
    }
    static ConditionUP generate_new(const QString &name){
        return std::make_unique<Condition>(name, ConditionKey{-1}, SecondsTS{100.}, 10., 1.);
    }
    static ConditionUP generate_from_data(const QString &name, ConditionKey key, SecondsTS duration, qreal scale, qreal uiFactorSize){
        return std::make_unique<Condition>(name, key, duration, scale, uiFactorSize);
    }
    static ConditionUP duplicate(const Condition &conditionToCopy);

    void apply_condition(const Condition *condition, bool copyActions, bool copyConnections);

    // sets
    bool contains_set_key(int setKeyToCheck) const;
    bool contains_same_set_keys(const std_v1<int> setKeysToCheck) const;

    // actions
    int get_action_id_from_key(ActionKey actionKey, bool displayError = true) const;
    Action *get_action_from_key(ActionKey actionKey, bool displayError = true) const;
    Action *get_action_from_component_key(ComponentKey componentKey, bool displayError = true) const;
    Action *get_action_from_id(RowId  idTab, bool displayError = true) const;
    std_v1<Action*> actions_with_nodes() const;
    void move_action_up(ActionKey actionKey);
    void move_action_down(ActionKey actionKey);
    void remove_action(ActionKey actionKey);
    void remove_all_actions();

    inline QString to_string() const{return QSL("Condition(")  % name % QSL("|") % QString::number(key()) % QSL(")");}

    // connections
    Connection *get_connection_from_key(ConnectionKey connectionKey, bool displayError = true) const;
    Connection *get_connection_from_id(RowId  idTab, bool displayError = true) const;
    void remove_connection(ConnectionKey connectionKey);    

    // connectors
    Connector *get_connector_from_key(ConnectorKey connectorKey, bool displayError = true) const;
    Connector *get_connector_from_id(RowId  idTab, bool displayError = true) const;
    void move_connector(ConnectorKey connectorKey, QPointF pos);
    void duplicate_connector(ConnectorKey connectorKey);
    void remove_connector(ConnectorKey connectorKey);
    void modify_connector(ConnectorKey connectorKey, QString name, Arg arg);

    // components
    Component *get_component_from_key(ComponentKey componentKey, bool displayError = true) const;
    void move_component(ComponentKey componentKey, QPointF pos);
    void remove_component_node(ComponentKey componentKey);

    void update_max_length(SecondsTS length);

    void check_integrity();


    IdKey key;
    QString name;    
    std_v1<int> setsKeys;

    SecondsTS duration{100.};
    qreal scale = 10. ;
    qreal uiFactorSize = 0.5;

    bool selected = false;

    std_v1<ConnectionUP> connections;
    std_v1<ConnectorUP> connectors;
    std_v1<ActionUP> actions;

    // copy
    static inline bool currentNodesCopySet = false;
    static inline std_v1<ConnectorUP> connectorsToCopy = {};
    static inline std_v1<ConnectionUP> connectionsToCopy = {};
};

static bool operator<(const ConditionUP &l, const ConditionUP &r){
    if(l->key() == r->key()){
        return false;
    }
    if(l->name == r->name){
        return false;
    }
    return true;
}

static bool operator==(const ConditionUP &l, const ConditionUP &r){
    return !(l < r) && !(r < l);
}


}
