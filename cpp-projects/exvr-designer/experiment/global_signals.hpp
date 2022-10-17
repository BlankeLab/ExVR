
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
#include <memory>

// Qt
#include <QObject>

// local
#include "data/interval.hpp"
#include "data/connector.hpp"
#include "data/connection.hpp"
#include "data/flow_elements/loop.hpp"
#include "resources/resource.hpp"

namespace tool::ex{

class GSignals : public QObject{
    Q_OBJECT
public:

    static void init();
    static GSignals *get();

signals:

    // ui
    void show_resources_manager_dialog_signal(Resource::Type type = Resource::Type::SizeEnum);
    void show_documentation_signal();
    void show_about_signal();
    void show_settings_dialog_signal();
    void show_benchmark_dialog_signal();
    void toggle_mode_signal();
    void toggle_follow_condition_mode_signal();

    // state
    void exp_launcher_state_updated_signal(tool::ex::ExpLauncherState state, QStringView infos);
    void exp_state_updated_signal(tool::ex::ExpState state, QStringView infos);

    // info
    void component_info_update_signal(tool::ex::ComponentKey componentKey, tool::ex::ConfigKey configKey, QStringView id, QStringView value);
    void connector_info_update_signal(tool::ex::ElementKey elementKey, tool::ex::ConditionKey conditionKey, tool::ex::ConnectorKey connectorKey, QStringView uiName, QStringView value);

    // instances
    void generate_instances_signal(QString directoryPath, unsigned int seed, bool manual, int nbInstances, int startId, QString baseName, QStringList manualNames);

    // # components
    void show_component_node_documentation_signal(tool::ex::ComponentKey componentKey);
    void display_component_help_window_signal(tool::ex::Component::Type type, bool resetWindow = true);
    void component_name_changed_signal(tool::ex::ComponentKey componentKey, QString name);
    void sort_components_by_category_signal();
    void sort_components_by_type_signal();
    void sort_components_by_name_signal();
    void update_component_position_signal(tool::ex::ComponentKey componentKey, tool::ex::RowId position);
    void add_component_signal(tool::ex::Component::Type type, tool::ex::RowId position);
    void show_component_custom_menu_signal(QPoint globalPos, tool::ex::ComponentKey key);
    void toggle_selection_component_signal(tool::ex::ComponentKey key);
    void toggle_component_parameters_signal(tool::ex::ComponentKey key);
    void remove_component_signal(tool::ex::ComponentKey key);
    void duplicate_component_signal(tool::ex::ComponentKey componentKey);
//    void enter_component_signal(tool::ex::ComponentKey key);
//    void leave_component_signal(tool::ex::ComponentKey key);
    void show_component_informations_signal(tool::ex::ComponentKey componentKey);
    void copy_component_signal(tool::ex::Component *component, std::vector<tool::ex::ConfigKey> configKeys, tool::ex::RowId to);

    // ## config
    void select_action_config_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey, tool::ex::RowId configTabId);
    void insert_config_signal(tool::ex::ComponentKey componentKey, tool::ex::RowId id, QString name);
    void copy_config_signal(tool::ex::ComponentKey componentKey, tool::ex::RowId id, QString name);
    void remove_config_signal(tool::ex::ComponentKey componentKey, tool::ex::RowId id);
    void select_config_signal(tool::ex::ComponentKey componentKey, tool::ex::RowId id);
    void move_config_signal(tool::ex::ComponentKey componentKey, tool::ex::RowId from, tool::ex::RowId to);
    void rename_config_signal(tool::ex::ComponentKey componentKey, tool::ex::RowId id, QString name);
    // ### args
    void new_arg_signal(tool::ex::ComponentKey componentKey, tool::ex::ConfigKey configKey, tool::ex::Arg arg, bool initConfig);
    void arg_updated_signal(tool::ex::ComponentKey componentKey, tool::ex::ConfigKey configKey, tool::ex::Arg arg, bool initConfig);
    void arg_removed_signal(tool::ex::ComponentKey componentKey, tool::ex::ConfigKey configKey, QStringView argName, bool initConfig);
    void swap_arg_signal(tool::ex::ComponentKey componentKey, tool::ex::ConfigKey configKey, QStringView argName1, QStringView argName2, bool initConfig);
    void action_signal(tool::ex::ComponentKey componentKey, tool::ex::ConfigKey configKey, QStringView actionName, bool initConfig);

    // elements
    void update_element_name_signal(tool::ex::ElementKey key, QString name);
    void update_element_informations_signal(tool::ex::ElementKey routineKey, QString informations);
    void duplicate_element_signal(tool::ex::ElementKey elementKey);
    void select_element_signal(tool::ex::ElementKey elementKey, bool updateSignal);
    void select_element_id_signal(RowId id, bool updateSignal);
    void select_element_id_no_nodes_signal(RowId id, bool updateSignal);
    void unselect_element_signal(bool updateSignal);
    void remove_selected_element_signal();
    void add_element_signal(tool::ex::FlowElement::Type type, size_t index);
    void remove_element_signal(tool::ex::ElementKey elementKey);
    void remove_everything_before_signal(tool::ex::ElementKey elementKey);
    void remove_everything_after_signal(tool::ex::ElementKey elementKey);
    void move_element_signal(tool::ex::ElementKey elementKey);
    void set_duration_for_all_routine_conditions_signal(tool::ex::ElementKey routineKey);
    void move_element_left_signal(size_t id);
    void move_element_right_signal(size_t id);
    void clean_current_routine_condition_signal(tool::ex::ElementKey routineKey);
    void clean_all_routine_conditions_signal(tool::ex::ElementKey routineKey);
    // # loops
    void modify_loop_nb_reps_signal(tool::ex::ElementKey loopKey, int nbReps);
    void modify_loop_n_signal(tool::ex::ElementKey loopKey, int nbReps);
    void modify_loop_no_following_value_signal(tool::ex::ElementKey loopKey, bool state);
    void modify_loop_type_signal(tool::ex::ElementKey loopKey, tool::ex::Loop::Mode mode);
    void modify_loop_set_name_signal(tool::ex::ElementKey loopKey, QString set, tool::ex::RowId idSet);
    void modify_loop_set_occurrencies_nb_signal(tool::ex::ElementKey loopKey, int occurenciesNb, tool::ex::RowId idSet);
    void add_loop_sets_signal(tool::ex::ElementKey loopKey, QString set, tool::ex::RowId idSet);
    void remove_set_signal(tool::ex::ElementKey loopKey, tool::ex::RowId idSet);
    void move_loop_set_up_signal(tool::ex::ElementKey loopKey, tool::ex::RowId idSet);
    void move_loop_set_down_signal(tool::ex::ElementKey loopKey, tool::ex::RowId idSet);
    void load_loop_sets_file_signal(tool::ex::ElementKey loopKey, QString filePath);
    // # isi
    void set_isi_randomize_signal(tool::ex::ElementKey isiKey, bool checked);
    void add_isi_interval_signal(tool::ex::ElementKey isiKey, double interval, tool::ex::RowId idInterval);
    void modify_isi_interval_signal(tool::ex::ElementKey isiKey, double interval, tool::ex::RowId idInterval);
    void remove_isi_interval_signal(tool::ex::ElementKey isiKey, tool::ex::RowId idInterval);
    void move_isi_interval_up_signal(tool::ex::ElementKey isiKey, tool::ex::RowId idInterval);
    void move_isi_interval_down_signal(tool::ex::ElementKey isiKey, tool::ex::RowId idInterval);
    // # routine
    void set_routine_as_randomizer_signal(tool::ex::ElementKey routineKey, bool isARandomizer);
    void routine_selected_signal(tool::ex::ElementKey routineKey, bool updateSignal);
    void select_routine_condition_signal(tool::ex::ElementKey routineKey, tool::ex::RowId conditionTabId);
    void move_routine_condition_down_signal(tool::ex::ElementKey routineKey, tool::ex::RowId conditionTabId);
    void move_routine_condition_up_signal(tool::ex::ElementKey routineKey, tool::ex::RowId conditionTabId);
    // ## condition
    void routine_condition_selected_signal(tool::ex::ElementKey routineKey, tool::ex::RowId  conditionTabId);
    void copy_condition_to_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey);
    // ### actions
    void delete_actions_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey);
    void fill_actions_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey);
    void clean_actions_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey);
    void add_action_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey, tool::ex::ComponentKey componentKey, std::optional<tool::ex::ConfigKey> ConfigKey, bool fillUpdate, bool fillVisibility);
    void remove_action_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey);
    void fill_action_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey, bool update, bool visibility);
    void clean_action_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey, bool update, bool visibility);
    void move_action_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey, tool::ex::RowId id);
    void move_action_up_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey);
    void move_action_down_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey);
    void insert_action_to_selected_routine_signal(tool::ex::ComponentKey componentKey);
    void insert_action_to_all_selected_routine_conditions_signal(tool::ex::ComponentKey componentKey);
    void insert_action_to_all_routines_conditions_signal(tool::ex::ComponentKey componentKey, std::optional<tool::ex::ConfigKey> ConfigKey, bool fillUpdate, bool fillVisibility);
    void insert_action_to_signal(tool::ex::ComponentKey componentKey, std::vector<std::tuple<tool::ex::ElementKey,tool::ex::ConditionKey,tool::ex::ConfigKey,bool, bool>> details);
    void insert_action_with_details_signal(tool::ex::ComponentKey componentKey);
    void modify_action_with_details_signal(tool::ex::ComponentKey componentKey);
    void remove_action_from_all_selected_routine_conditions_signal(tool::ex::ComponentKey componentKey);
    void remove_action_from_all_routines_conditions_signal(tool::ex::ComponentKey componentKey);
    // #### timeline
    void update_timeline_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, double duration, double scale, double uiSize);
    // ##### interval
    void add_interval_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey, bool updateTimeline, Interval interval);
    void remove_interval_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey, bool updateTimeline, Interval interval);
    // ### connections / connectors / nodes
    void delete_connections_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey);
    void nodes_connection_created_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::Connection *connection);
    void connector_input_connection_validity_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ConnectorKey connectorKey, bool state);
    void show_connector_node_documentation_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey, tool::ex::ConnectorKey connectorKey);
    void connector_node_created_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::Connector *connector);
    void connector_node_modified_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ConnectorKey connectorKey, QString name, tool::ex::Arg arg);
    void connector_node_moved_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ConnectorKey connectorKey, QPointF pos);
    void duplicate_connector_node_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ConnectorKey connectorKey);
    void component_node_created_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ComponentKey componentKey, QPointF pos);
    void component_node_moved_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ComponentKey componentKey, QPointF pos);
    void unselect_nodes_and_connections_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, bool doUpdate);
    void delete_selected_nodes_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey);
    void delete_nodes_and_connections_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey,
        std_v1<tool::ex::ConnectorKey> connectorsKey, std_v1<tool::ex::ComponentKey> componentsKey, std_v1<tool::ex::ConnectionKey> connectionsKey, bool doUpdate);
    void select_nodes_and_connections_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey,
        std_v1<tool::ex::ConnectorKey> connectorsKey, std_v1<tool::ex::ComponentKey> componentsKey, std_v1<tool::ex::ConnectionKey> connectionsKey, bool doUpdate);

    // resource
    void copy_resource_signal(tool::ex::Resource *resource);


    void ask_for_udapte_signal(int);


    // debug & test
    void delete_unused_components_debug_signal();

private:
    static inline std::unique_ptr<GSignals> m_signals = nullptr;
};

class BlockSignalsGuard{
public:
    BlockSignalsGuard(){
        currentState = GSignals::get()->signalsBlocked();
        GSignals::get()->blockSignals(true);
    }

    ~BlockSignalsGuard(){
        GSignals::get()->blockSignals(currentState);
    }
private:
    bool currentState;
};
}
