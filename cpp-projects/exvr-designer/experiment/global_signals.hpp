
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

    static auto get() -> GSignals *;

signals:

    // ui
    auto show_resources_manager_dialog_signal(Resource::Type type = Resource::Type::SizeEnum) -> void;
    auto show_documentation_signal() -> void;
    auto show_about_signal() -> void;
    auto show_settings_dialog_signal() -> void;
    auto show_benchmark_dialog_signal() -> void;
    auto toggle_mode_signal() -> void;
    auto toggle_follow_condition_mode_signal() -> void;
    auto fix_colors_signal() -> void;

    // state
    auto exp_launcher_state_updated_signal(tool::ex::ExpLauncherState state, QStringView infos) -> void;
    auto exp_state_updated_signal(tool::ex::ExpState state, QStringView infos) -> void;

    // info
    auto component_info_update_signal(tool::ex::ComponentKey componentKey, tool::ex::ConfigKey configKey, QStringView id, QStringView value) -> void;
    auto connector_info_update_signal(tool::ex::ElementKey elementKey, tool::ex::ConditionKey conditionKey, tool::ex::ConnectorKey connectorKey, QStringView uiName, QStringView value) -> void;

    // instances
    auto generate_instances_signal(QString directoryPath, unsigned int seed, bool manual, int nbInstances, int startId, QString baseName, QStringList manualNames) -> void;

    // # components
    auto show_component_node_documentation_signal(tool::ex::ComponentKey componentKey) -> void;
    auto display_component_help_window_signal(tool::ex::Component::Type type, bool resetWindow = true) -> void;
    auto component_name_changed_signal(tool::ex::ComponentKey componentKey, QString name) -> void;
    auto sort_components_by_category_signal() -> void;
    auto sort_components_by_type_signal() -> void;
    auto sort_components_by_name_signal() -> void;
    auto update_component_position_signal(tool::ex::ComponentKey componentKey, tool::ex::RowId position) -> void;
    auto add_component_signal(tool::ex::Component::Type type, tool::ex::RowId position) -> void;
    auto show_component_custom_menu_signal(QPoint globalPos, tool::ex::ComponentKey key) -> void;
    auto toggle_selection_component_signal(tool::ex::ComponentKey key) -> void;
    auto toggle_component_parameters_signal(tool::ex::ComponentKey key) -> void;
    auto remove_component_signal(tool::ex::ComponentKey key) -> void;
    auto duplicate_component_signal(tool::ex::ComponentKey componentKey) -> void;
//    auto enter_component_signal(tool::ex::ComponentKey key) -> void;
//    auto leave_component_signal(tool::ex::ComponentKey key) -> void;
    auto show_component_informations_signal(tool::ex::ComponentKey componentKey) -> void;
    auto copy_component_signal(tool::ex::Component *component, std::vector<tool::ex::ConfigKey> configKeys, tool::ex::RowId to) -> void;

    // ## config
    auto select_action_config_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey, tool::ex::RowId configTabId) -> void;
    auto insert_config_signal(tool::ex::ComponentKey componentKey, tool::ex::RowId id, QString name) -> void;
    auto copy_config_signal(tool::ex::ComponentKey componentKey, tool::ex::RowId id, QString name) -> void;
    auto remove_config_signal(tool::ex::ComponentKey componentKey, tool::ex::RowId id) -> void;
    auto select_config_signal(tool::ex::ComponentKey componentKey, tool::ex::RowId id) -> void;
    auto move_config_signal(tool::ex::ComponentKey componentKey, tool::ex::RowId from, tool::ex::RowId to) -> void;
    auto rename_config_signal(tool::ex::ComponentKey componentKey, tool::ex::RowId id, QString name) -> void;
    // ### args
    auto new_arg_signal(tool::ex::ComponentKey componentKey, tool::ex::ConfigKey configKey, tool::ex::Arg arg, bool initConfig) -> void;
    auto arg_updated_signal(tool::ex::ComponentKey componentKey, tool::ex::ConfigKey configKey, tool::ex::Arg arg, bool initConfig) -> void;
    auto arg_removed_signal(tool::ex::ComponentKey componentKey, tool::ex::ConfigKey configKey, QStringView argName, bool initConfig) -> void;
    auto swap_arg_signal(tool::ex::ComponentKey componentKey, tool::ex::ConfigKey configKey, QStringView argName1, QStringView argName2, bool initConfig) -> void;
    auto action_signal(tool::ex::ComponentKey componentKey, tool::ex::ConfigKey configKey, QStringView actionName, bool initConfig) -> void;

    // elements
    auto update_element_name_signal(tool::ex::ElementKey key, QString name) -> void;
    auto update_element_informations_signal(tool::ex::ElementKey routineKey, QString informations) -> void;
    auto duplicate_element_signal(tool::ex::ElementKey elementKey) -> void;
    auto select_element_signal(tool::ex::ElementKey elementKey, bool updateSignal) -> void;
    auto select_element_id_signal(RowId id, bool updateSignal) -> void;
    auto select_element_id_no_nodes_signal(RowId id, bool updateSignal) -> void;
    auto unselect_element_signal(bool updateSignal) -> void;
    auto remove_selected_element_signal() -> void;
    auto add_element_signal(tool::ex::FlowElement::Type type, size_t index) -> void;
    auto remove_element_signal(tool::ex::ElementKey elementKey) -> void;
    auto remove_everything_before_signal(tool::ex::ElementKey elementKey) -> void;
    auto remove_everything_after_signal(tool::ex::ElementKey elementKey) -> void;
    auto move_element_signal(tool::ex::ElementKey elementKey) -> void;
    auto set_duration_for_all_routine_conditions_signal(tool::ex::ElementKey routineKey) -> void;
    auto move_element_left_signal(size_t id) -> void;
    auto move_element_right_signal(size_t id) -> void;
    auto clean_current_routine_condition_signal(tool::ex::ElementKey routineKey) -> void;
    auto clean_all_routine_conditions_signal(tool::ex::ElementKey routineKey) -> void;
    // # loops
    auto modify_loop_nb_reps_signal(tool::ex::ElementKey loopKey, int nbReps) -> void;
    auto modify_loop_n_signal(tool::ex::ElementKey loopKey, int nbReps) -> void;
    auto modify_loop_no_following_value_signal(tool::ex::ElementKey loopKey, bool state) -> void;
    auto modify_loop_type_signal(tool::ex::ElementKey loopKey, tool::ex::Loop::Mode mode) -> void;
    auto modify_loop_set_name_signal(tool::ex::ElementKey loopKey, QString set, tool::ex::RowId idSet) -> void;
    auto modify_loop_set_occurrencies_nb_signal(tool::ex::ElementKey loopKey, int occurenciesNb, tool::ex::RowId idSet) -> void;
    auto add_loop_sets_signal(tool::ex::ElementKey loopKey, QString set, tool::ex::RowId idSet) -> void;
    auto remove_set_signal(tool::ex::ElementKey loopKey, tool::ex::RowId idSet) -> void;
    auto move_loop_set_up_signal(tool::ex::ElementKey loopKey, tool::ex::RowId idSet) -> void;
    auto move_loop_set_down_signal(tool::ex::ElementKey loopKey, tool::ex::RowId idSet) -> void;
    auto load_loop_sets_file_signal(tool::ex::ElementKey loopKey, QString filePath) -> void;
    // # isi
    auto set_isi_randomize_signal(tool::ex::ElementKey isiKey, bool checked) -> void;
    auto add_isi_interval_signal(tool::ex::ElementKey isiKey, double interval, tool::ex::RowId idInterval) -> void;
    auto modify_isi_interval_signal(tool::ex::ElementKey isiKey, double interval, tool::ex::RowId idInterval) -> void;
    auto remove_isi_interval_signal(tool::ex::ElementKey isiKey, tool::ex::RowId idInterval) -> void;
    auto move_isi_interval_up_signal(tool::ex::ElementKey isiKey, tool::ex::RowId idInterval) -> void;
    auto move_isi_interval_down_signal(tool::ex::ElementKey isiKey, tool::ex::RowId idInterval) -> void;
    // # routine
    auto set_routine_as_randomizer_signal(tool::ex::ElementKey routineKey, bool isARandomizer) -> void;
    auto routine_selected_signal(tool::ex::ElementKey routineKey, bool updateSignal) -> void;
    auto select_routine_condition_signal(tool::ex::ElementKey routineKey, tool::ex::RowId conditionTabId) -> void;
    auto move_routine_condition_down_signal(tool::ex::ElementKey routineKey, tool::ex::RowId conditionTabId) -> void;
    auto move_routine_condition_up_signal(tool::ex::ElementKey routineKey, tool::ex::RowId conditionTabId) -> void;
    // ## condition
    auto routine_condition_selected_signal(tool::ex::ElementKey routineKey, tool::ex::RowId  conditionTabId) -> void;
    auto copy_condition_to_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey) -> void;
    // ### actions
    auto delete_actions_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey) -> void;
    auto fill_actions_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey) -> void;
    auto clean_actions_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey) -> void;
    auto add_action_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey, tool::ex::ComponentKey componentKey, std::optional<tool::ex::ConfigKey> ConfigKey, bool fillUpdate, bool fillVisibility) -> void;
    auto remove_action_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey) -> void;
    auto fill_action_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey, bool update, bool visibility) -> void;
    auto clean_action_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey, bool update, bool visibility) -> void;
    auto move_action_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey, tool::ex::RowId id) -> void;
    auto move_action_up_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey) -> void;
    auto move_action_down_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey) -> void;
    auto insert_action_to_selected_routine_signal(tool::ex::ComponentKey componentKey) -> void;
    auto insert_action_to_all_selected_routine_conditions_signal(tool::ex::ComponentKey componentKey) -> void;
    auto insert_action_to_all_routines_conditions_signal(tool::ex::ComponentKey componentKey, std::optional<tool::ex::ConfigKey> ConfigKey, bool fillUpdate, bool fillVisibility) -> void;
    auto insert_action_to_signal(tool::ex::ComponentKey componentKey, std::vector<std::tuple<tool::ex::ElementKey,tool::ex::ConditionKey,tool::ex::ConfigKey,bool, bool>> details) -> void;
    auto insert_action_with_details_signal(tool::ex::ComponentKey componentKey) -> void;
    auto modify_action_with_details_signal(tool::ex::ComponentKey componentKey) -> void;
    auto remove_action_from_all_selected_routine_conditions_signal(tool::ex::ComponentKey componentKey) -> void;
    auto remove_action_from_all_routines_conditions_signal(tool::ex::ComponentKey componentKey) -> void;
    // #### timeline
    auto update_timeline_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, double duration, double scale, double uiSize) -> void;
    // ##### interval
    auto add_interval_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey, bool updateTimeline, Interval interval) -> void;
    auto remove_interval_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ActionKey actionKey, bool updateTimeline, Interval interval) -> void;
    // ### connections / connectors / nodes
    auto delete_connections_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey) -> void;
    auto nodes_connection_created_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::Connection *connection) -> void;
    auto connector_input_connection_validity_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ConnectorKey connectorKey, bool state) -> void;
    auto show_connector_node_documentation_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey, tool::ex::ConnectorKey connectorKey) -> void;
    auto connector_node_created_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::Connector *connector) -> void;
    auto connector_node_modified_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ConnectorKey connectorKey, QString name, tool::ex::Arg arg) -> void;
    auto connector_node_moved_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ConnectorKey connectorKey, QPointF pos) -> void;
    auto duplicate_connector_node_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ConnectorKey connectorKey) -> void;
    auto component_node_created_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ComponentKey componentKey, QPointF pos) -> void;
    auto component_node_moved_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, tool::ex::ComponentKey componentKey, QPointF pos) -> void;
    auto unselect_nodes_and_connections_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey, bool doUpdate) -> void;
    auto duplicate_selected_nodes_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey) -> void;
    auto delete_selected_nodes_signal(tool::ex::ElementKey routineKey,tool::ex::ConditionKey conditionKey) -> void;
    auto delete_nodes_and_connections_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey,
        std::vector<tool::ex::ConnectorKey> connectorsKey, std::vector<tool::ex::ComponentKey> componentsKey, std::vector<tool::ex::ConnectionKey> connectionsKey, bool doUpdate) -> void;
    auto select_nodes_and_connections_signal(tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey,
        std::vector<tool::ex::ConnectorKey> connectorsKey, std::vector<tool::ex::ComponentKey> componentsKey, std::vector<tool::ex::ConnectionKey> connectionsKey, bool doUpdate) -> void;
    auto paste_nodes_clip_board_signal(QPointF mousePosition, tool::ex::ElementKey routineKey, tool::ex::ConditionKey conditionKey) -> void;
    // resource
    auto copy_resource_signal(tool::ex::Resource *resource) -> void;

    auto ask_for_udapte_signal(int) -> void;


    // debug & test
    auto delete_unused_components_debug_signal() -> void;
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
