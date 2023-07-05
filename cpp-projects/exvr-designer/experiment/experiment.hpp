
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
#include <optional>
#include <ranges>

// local
// # data
#include "data/flow_elements/routine.hpp"
#include "data/flow_elements/isi.hpp"
#include "data/flow_elements/loop.hpp"
#include "data/settings.hpp"
#include "data/gui.hpp"
#include "data/interval.hpp"
#include "data/argument.hpp"
#include "data/components_manager.hpp"
#include "data/states.hpp"
// # resources
#include "resources/resources_manager.hpp"
// # experiment
#include "randomizer.hpp"

namespace tex = tool::ex;

template<class T>
concept FlowElementDerived = std::is_base_of<tex::FlowElement, T>::value;

namespace tool::ex{

// ############################################################################### CONSTANTS

constexpr static int UpdateFlow        = 0b1;
constexpr static int UpdateSelection   = 0b10;
constexpr static int UpdateComponents  = 0b100;
constexpr static int UpdateRoutines    = 0b1000;
constexpr static int UpdateUI          = 0b10000;
constexpr static int UpdateResources   = 0b100000;
constexpr static int UpdateSettings    = 0b1000000;
[[maybe_unused]] constexpr static int ResetUI           = 0b10000000;
constexpr static int UpdateAll         = UpdateComponents | UpdateFlow | UpdateSelection | UpdateUI | UpdateRoutines | UpdateResources | UpdateSettings;


class Experiment;
using ExperimentUP = std::unique_ptr<Experiment>;

class Experiment : public QObject{

    Q_OBJECT

public:    

    Experiment(QString nVersion);
    ~Experiment() = default;

    // getters
    // # element
    auto nb_elements()const noexcept -> size_t;
    auto nb_elements_no_nodes()const noexcept-> size_t;

    auto get_element_position(FlowElement *element) const -> RowId;
    auto get_element_position_no_nodes(FlowElement *element) const -> RowId;

    auto get_element(RowId id) const -> FlowElement*;
    auto get_element_no_nodes(RowId id) const -> FlowElement*;
    auto get_element(ElementKey elementKey, bool showError = true) const -> FlowElement*;

    template<class T>
    auto get_element_from_type_and_id(ElementKey key) const -> T*{
        for(const auto &elem : elements){
            if(T* child = dynamic_cast<T*>(elem.get()); child != nullptr){
                if(elem->key() == key.v){
                    return child;
                }
            }
        }
        return nullptr;
    }

    auto get_elements() const -> std::vector<FlowElement*>;
    auto get_elements_of_type(FlowElement::Type type) const;

    template<FlowElementDerived T>
    auto get_elements_from_type() const -> std::vector<T*>{

        std::vector<T*> children;
        for(const auto &elem : elements){
            if(auto child = dynamic_cast<T*>(elem.get()); child != nullptr){
                children.push_back(child);
            }
        }
        return children;
    }

    // ## routine
    auto get_routine(tex::ElementKey routineKey) const -> Routine*;
    auto get_routine(RowId id) const -> Routine*;
    auto get_routines_name() const -> std::vector<QStringView>;
    // ### condition
    auto get_condition(tex::ConditionKey conditionKey) const -> Condition*;
    auto get_condition(tex::ElementKey routineKey, tex::ConditionKey conditionKey) const -> Condition*;
    // #### action
    Action *get_action(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ActionKey actionKey) const;

    // ## isi
    Isi *get_isi(ElementKey isiKey) const;
    // ## loop
    Loop *get_loop(ElementKey loopKey) const;
    std::vector<Loop*> get_loops() const;
    // # components
    Component *get_component(ComponentKey componentKey) const;
    // ## config
    Config *get_config(ComponentKey componentKey, ConfigKey configKey) const;

    inline Settings *settings() noexcept{return &m_settings;}
    inline GUI *gui() noexcept{return &m_gui;}

    // check
    void check_elements();
    void check_legacy_conditions();
    void check_integrity();

    // update flags
    inline void set_update_all_flag() noexcept{updateFlag = UpdateAll;}
    inline void add_to_update_flag(int flag) noexcept{updateFlag |= flag;}
    inline void reset_update_flag() noexcept{updateFlag = 0;}
    inline int update_flag() const noexcept {return updateFlag;}

public slots:

    // update
    void compute_loops_levels();
    void update_conditions();
    // # randomization
    void update_randomization_seed(unsigned int seed);

    // files
    void new_experiment();
    void clean_experiment();
    void set_instance_name(QString instanceName);

    // settings
    void reset_settings();
    void update_settings(tex::Settings settings);

    // resources
    void add_resources(Resource::Type type, QStringList filesPath);
    void update_resource_path(QString currentPath, QString newPath);
    void update_resource_alias(QString currentAlias, QString newAlias);
    void select_resource(Resource::Type type, size_t index);
    void remove_resource(Resource::Type type, size_t index);
    void clean_resources(Resource::Type type);
    void update_reload_resource_code(int reloadCode);
    void copy_resource(tex::Resource *resource);

    // ui
    void toggle_actions_connections_separation();
    void toggle_design_mode();
    void toggle_follow_condition_mode();
    // # infos
    void update_connector_dialog_with_info(tex::ElementKey elementKey, tex::ConditionKey conditionKey, tex::ConnectorKey connectorKey, QStringView id, QStringView value);
    void update_component_dialog_with_info(tex::ComponentKey componentKey, tex::ConfigKey configKey, QStringView id, QStringView value);
    // # states
    void update_exp_launcher_state(tex::ExpLauncherState state, QStringView infos);
    void update_exp_state(tex::ExpState state, QStringView infos);

    // components
    void add_new_component(tex::Component::Type type, tex::RowId id);
    void copy_component(tex::Component *component, std::vector<tex::ConfigKey> configKeys, tex::RowId id);
    void duplicate_component(tex::ComponentKey componentKey);
    void remove_component(tex::ComponentKey componentKey);
    void update_component_position(tex::ComponentKey componentKey, tex::RowId id);
    void update_component_name(tex::ComponentKey componentKey, QString name);
    void sort_components_by_category();
    void sort_components_by_type();
    void sort_components_by_name();
    void delete_unused_components();
    // # components configs
    void select_config_in_component(tex::ComponentKey componentKey, tex::RowId id);
    void insert_config_in_component(tex::ComponentKey componentKey, tex::RowId id, QString configName);
    void copy_config_from_component(tex::ComponentKey componentKey, tex::RowId id, QString configName);
    void remove_config_from_component(tex::ComponentKey componentKey, tex::RowId id);
    void move_config_in_component(tex::ComponentKey componentKey, tex::RowId from, tex::RowId to);
    void rename_config_in_component(tex::ComponentKey componentKey, tex::RowId id, QString configName);
    // ## arguments
    void new_arg(tex::ComponentKey componentKey, tex::ConfigKey configKey, tex::Arg arg, bool initConfig);
    void arg_updated(tex::ComponentKey componentKey, tex::ConfigKey configKey, tex::Arg arg, bool initConfig);
    void arg_removed(tex::ComponentKey componentKey, tex::ConfigKey configKey, QStringView argName, bool initConfig);
    void swap_arg(tex::ComponentKey componentKey, tex::ConfigKey configKey, QStringView argName1, QStringView argName2, bool initConfig);

    // elements
    void unselect_all_elements(bool updateSignal = true) noexcept;
    void select_element(tex::ElementKey elementKey, bool updateSignal = true);
    void select_element_id(tex::RowId elementId, bool updateSignal = true);
    void select_element_id_no_nodes(tex::RowId elementId, bool updateSignal = true);
    void select_element_from_ptr(tex::FlowElement *element, bool updateSignal = true);
    void add_element(tex::FlowElement::Type type, size_t index);
    void remove_element(tex::FlowElement *elemToDelete, bool updateConditions = true);
    void remove_element_of_key(tex::ElementKey elementKey);
    void remove_everything_before_element_of_key(tex::ElementKey elementKey);
    void remove_everything_after_element_of_key(tex::ElementKey elementKey);
    void duplicate_element(tex::ElementKey elementKey);
    void clean_current_routine_condition(tex::ElementKey routineKey);
    void clean_all_routine_conditions(tex::ElementKey routineKey);
    void set_duration_for_all_routine_conditions(tex::ElementKey routineKey);
    void remove_selected_element();
    void move_left(size_t id);
    void move_right(size_t id);
    void move_element(tex::ElementKey elementKey);
    void update_element_name(tex::ElementKey elementKey, QString elemName);
    void update_element_informations(tex::ElementKey elementKey, QString informations);

    // # loop
    void add_loop_sets(tex::ElementKey loopKey, QString sets, tex::RowId id);
    void modify_loop_set_name(tex::ElementKey loopKey, QString setName, tex::RowId id);
    void modify_loop_set_occurrencies_nb(tex::ElementKey loopKey, int setOccuranciesNb, tex::RowId id);
    void modify_loop_type(tex::ElementKey loopKey, Loop::Mode type);
    void modify_loop_nb_reps(tex::ElementKey loopKey, int nbReps);
    void modify_loop_N(tex::ElementKey loopKey, int N);
    void modify_loop_no_following_value(tex::ElementKey loopKey, bool state);
    void remove_set(tex::ElementKey loopKey, tex::RowId id);
    void sort_loop_sets_lexico(tex::ElementKey loopKey);
    void sort_loop_sets_num(tex::ElementKey loopKey);
    void move_loop_set_up(tex::ElementKey loopKey, tex::RowId id);
    void move_loop_set_down(tex::ElementKey loopKey, tex::RowId id);
    void load_loop_sets_file(tex::ElementKey loopKey, QString path);

    // # isi
    void add_isi_interval(tex::ElementKey isiKey, qreal value, tex::RowId id);
    void remove_isi_interval(tex::ElementKey isiKey, tex::RowId id);
    void set_isi_randomize(tex::ElementKey isiKey, bool randomize);
    void modify_isi_interval(tex::ElementKey isiKey, qreal value, tex::RowId id);
    void move_isi_interval_up(tex::ElementKey isiKey, tex::RowId id);
    void move_isi_interval_down(tex::ElementKey isiKey, tex::RowId id);

    // # routine
    void select_routine_condition(tex::ElementKey routineKey, tex::RowId  conditionTabId);
    void move_routine_condition_down(tex::ElementKey routineKey, tex::RowId id);
    void move_routine_condition_up(tex::ElementKey routineKey, tex::RowId id);
    void set_routine_as_randomizer(tex::ElementKey routineKey, bool isARandomizer);
    // ## condition
    void update_condition_timeline(tex::ElementKey routineKey, tex::ConditionKey conditionKey, double duration, double scale, double uiFactorSize);
    void copy_to_conditions(tex::ElementKey routineKey, tex::ConditionKey conditionKey, std::vector<std::pair<tex::ElementKey,tex::ConditionKey>> conditionsToBeEcrased, bool copyActions, bool copyConnections);
    void delete_actions_from_condition(tex::ElementKey routineKey, tex::ConditionKey conditionKey);
    void fill_actions_from_condition(tex::ElementKey routineKey, tex::ConditionKey conditionKey);
    void clean_actions_from_condition(tex::ElementKey routineKey, tex::ConditionKey conditionKey);
    // ### action
    void add_action(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ComponentKey componentKey,
        std::optional<tex::ConfigKey> ConfigKey, bool fillUpdateTimeline, bool fillVisibilityTimeline);
    void add_action_to_all_conditions(tex::ElementKey routineKey, tex::ComponentKey componentKey,
        std::optional<tex::ConfigKey> ConfigKey, bool fillUpdateTimeline, bool fillVisibilityTimeline);
    void add_action_to_all_routines_conditions(tex::ComponentKey componentKey,
        std::optional<tex::ConfigKey> ConfigKey, bool fillUpdateTimeline, bool fillVisibilityTimeline);
    void insert_action_to(tex::ComponentKey componentKey,  std::vector<std::tuple<tex::ElementKey,tex::ConditionKey,tex::ConfigKey,bool, bool>> details);
    void modify_action(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ComponentKey componentKey,
        bool changeConfig, bool changeUpdateTimeline, bool changeVisibilityTimeline,
        tex::ConfigKey ConfigKey, bool fillUpdateTimeline, bool fillVisibilityTimeline);
    void modify_action_to_all_conditions(tex::ElementKey routineKey, tex::ComponentKey componentKey,
        bool changeConfig, bool changeUpdateTimeline, bool changeVisibilityTimeline,
        tex::ConfigKey ConfigKey, bool fillUpdateTimeline, bool fillVisibilityTimeline);
    void modify_action_to_all_routines_conditions(tex::ComponentKey componentKey,
        bool changeConfig, bool changeUpdateTimeline, bool changeVisibilityTimeline,
        tex::ConfigKey ConfigKey, bool fillUpdateTimeline, bool fillVisibilityTimeline);

    void remove_action_from_all_selected_routine_conditions(tex::ElementKey routineKey, tex::ComponentKey componentKey);
    void remove_action_from_all_routines_conditions(tex::ComponentKey componentKey);
    void remove_action_from_condition(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ActionKey actionKey, bool update = true);
    void fill_action(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ActionKey actionKey, bool update, bool visibility);
    void clean_action(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ActionKey actionKey, bool update, bool visibility);
    void move_action_up(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ActionKey actionKey);
    void move_action_down(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ActionKey actionKey);
    // #### timeline
    void add_timeline_interval(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ActionKey actionKey, bool updateTimeline, tex::Interval interval);
    void remove_timeline_interval(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ActionKey actionKey, bool updateTimeline, tex::Interval interval);
    // #### config
    void select_action_config(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ActionKey actionKey, tex::RowId configTabId);
    // ### connection
    void create_connection(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::Connection *connection);
    void delete_connections(tex::ElementKey routineKey, tex::ConditionKey conditionKey);
    // ### component node
    void create_component_node(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ComponentKey componentKey, QPointF pos);
    void move_component_node(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ComponentKey componentKey, QPointF pos);
    // ### connector node
    void create_connector_node(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::Connector *connector);
    void modify_connector_node(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ConnectorKey connectorKey, QString name, tex::Arg arg);
    void move_connector_node(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ConnectorKey connectorKey, QPointF pos);
    void duplicate_connector_node(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ConnectorKey connectorKey);
    void set_connector_input_connection_validity(tex::ElementKey routineKey, tex::ConditionKey conditionKey, tex::ConnectorKey connectorKey, bool state);
    // ### connections / connectors / components
    void delete_selected_nodes(tex::ElementKey routineKey, tex::ConditionKey conditionKey);
    auto duplicate_selected_nodes(tex::ElementKey routineKey, tex::ConditionKey conditionKey) -> void;
    void unselect_nodes_and_connections(tex::ElementKey routineKey, tex::ConditionKey conditionKey, bool doUpdate);
    void delete_nodes_and_connections(tex::ElementKey routineKey, tex::ConditionKey conditionKey,
        std::vector<ConnectorKey> connectorsKey, std::vector<ComponentKey> componentsKey, std::vector<ConnectionKey> connectionsKey, bool doUpdate);
    void select_nodes_and_connections(tex::ElementKey routineKey, tex::ConditionKey conditionKey,
        std::vector<ConnectorKey> connectorsKey, std::vector<ComponentKey> componentsKey, std::vector<ConnectionKey> connectionsKey, bool doUpdate);
    auto paste_nodes_clip_board(QPointF mousePosition, tex::ElementKey routineKey, tex::ConditionKey conditionKey) ->void;

    // # debug
    void display_exp_infos();
    void fix_colors();

private :    

    // clean
    void remove_elements_not_in_flow();

    auto get_element_iterator(ElementKey elementKey) const -> std::vector<std::unique_ptr<FlowElement>>::const_iterator;
    auto get_element_iterator(FlowElement *element) const -> std::vector<std::unique_ptr<FlowElement>>::const_iterator;


public :

    States states;
    Randomizer randomizer;

    // elements
    std::vector<std::unique_ptr<FlowElement>> elements;
    std::vector<std::unique_ptr<Loop>> loops; // not in the flow
    FlowElement *selectedElement = nullptr;
    Routine *lastRoutineSelected = nullptr;
    Isi *lastIsiSelected = nullptr;

    // managers
    ComponentsManager compM;
    ResourcesManager resM;

    // infos
    using UiKey   = QStringView;
    using UiValue = QStringView;
    template<class T1,class T2>
    using umap = umap<T1,T2>;
    umap<int, umap<int, umap<int, umap<UiKey, UiValue>>>> connectorsInfo;
    umap<int, umap<int, umap<UiKey, UiValue>>>  componentsInfo;

private :

    // update
    int updateFlag = 0;

    GUI m_gui;
    Settings m_settings;

};

class ExperimentManager{
public:
    static void init();

    static ExperimentManager *get();

    void init_current(const QString &numVersion);
    void init_imported(const QString &numVersion);

    Experiment *current();
    Experiment *imported();

    void clean_current();
    void clean_imported();
    void clean();

private:

    std::unique_ptr<Experiment> m_experiment = nullptr;
    std::unique_ptr<Experiment> m_importedExperiment = nullptr;
};
}

