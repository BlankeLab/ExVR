
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

struct Condition  {

    Condition() = delete;
    Condition(QString n, ConditionKey id, SecondsTS duration, double uiScale, double uiSize );
    Condition(const Component &) = delete;
    Condition& operator=(const Condition&) = delete;

    static std::unique_ptr<Condition> generate_new_default(){
        return std::make_unique<Condition>(QSL("default"), ConditionKey{-1}, SecondsTS{100.}, 10., 1.);
    }
    static std::unique_ptr<Condition> generate_new(const QString &name){
        return std::make_unique<Condition>(name, ConditionKey{-1}, SecondsTS{100.}, 10., 1.);
    }
    static std::unique_ptr<Condition> generate_from_data(const QString &name, ConditionKey key, SecondsTS duration, qreal scale, qreal uiFactorSize){
        return std::make_unique<Condition>(name, key, duration, scale, uiFactorSize);
    }
    static std::unique_ptr<Condition> duplicate(const Condition &conditionToCopy);

    void apply_condition(const Condition *condition, bool copyActions, bool copyConnections);

    // sets
    bool contains_set_key(SetKey setKeyToCheck) const;
    bool contains_same_set_keys(const std::vector<SetKey> setKeysToCheck) const;

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

    constexpr int key() const noexcept{ return m_key();}
    constexpr ConditionKey c_key() const noexcept {return ConditionKey{key()};}

    QString name;
    std::vector<SetKey> setsKeys;

    SecondsTS duration{100.};
    qreal scale = 10. ;
    qreal uiFactorSize = 0.5;

    bool selected = false;

    std_v1<std::unique_ptr<Connection>> connections;
    std_v1<std::unique_ptr<Connector>> connectors;
    std_v1<std::unique_ptr<Action>> actions;

    // copy
    static inline bool currentNodesCopySet = false;
    static inline std_v1<std::unique_ptr<Connector>> connectorsToCopy = {};
    static inline std_v1<std::unique_ptr<Connection>> connectionsToCopy = {};

private:

    IdKey m_key;
};

[[maybe_unused]] static bool operator<(const std::unique_ptr<Condition> &l, const std::unique_ptr<Condition> &r){

    if(l->key() < r->key()){
        return true;
    }

    if(l->key() > r->key()){
        return false;
    }

    if(l->name < r->name){
        return true;
    }
    return false;
}

[[maybe_unused]] static bool operator==(const std::unique_ptr<Condition> &l, const std::unique_ptr<Condition> &r){
    return (l->key() == r->key()) && (l->name == r->name);
}


}
