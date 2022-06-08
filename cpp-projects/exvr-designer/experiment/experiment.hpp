
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
#include "data/flow_elements/node_flow.hpp"
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

template<class T>
concept FlowElementDerived = std::is_base_of<tool::ex::FlowElement, T>::value;

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

//    template <typename T>
//    static constexpr FlowElement::Type get_type(){
//        if constexpr(std::is_same<T, Routine>()){
//            return FlowElement::Type::Routine;
//        }else if constexpr(std::is_same<T, Isi>()){
//            return FlowElement::Type::Isi;
//        }else if constexpr(std::is_same<T, LoopNode>()){
//            return FlowElement::Type::LoopStart;
//        }
//        return FlowElement::Type::Node;
//    }

    auto get_elements_from_type(FlowElement::Type type) const{
        return elements | std::ranges::views::filter([type](const auto &element) {
            return element->type() == type;
        });
    }

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
    auto get_routine(ElementKey routineKey) const -> Routine*;
    // ### condition
    auto get_condition(ConditionKey conditionKey) const -> Condition*;
    auto get_condition(ElementKey routineKey, ConditionKey conditionKey) const -> Condition*;
    // #### action
    Action *get_action(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey) const;

    // ## isi
    Isi *get_isi(ElementKey isiKey) const;
    // ## loop
    Loop *get_loop(ElementKey loopKey) const;
    std_v1<Loop*> get_loops() const;
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
    void update_settings(Settings settings);

    // resources
    void add_resources(Resource::Type type, QStringList filesPath);
    void update_resource_path(QString currentPath, QString newPath);
    void update_resource_alias(QString currentAlias, QString newAlias);
    void select_resource(Resource::Type type, size_t index);
    void remove_resource(Resource::Type type, size_t index);
    void clean_resources(Resource::Type type);
    void update_reload_resource_code(int reloadCode);
    void copy_resource(Resource *resource);

    // ui
    void toggle_actions_connections_separation();
    void toggle_design_mode();
    void toggle_follow_condition_mode();
    // # infos
    void update_connector_dialog_with_info(ElementKey elementKey, ConditionKey conditionKey, ConnectorKey connectorKey, QStringView id, QStringView value);
    void update_component_dialog_with_info(ComponentKey componentKey, ConfigKey configKey, QStringView id, QStringView value);
    // # states
    void update_exp_launcher_state(ExpLauncherState state, QStringView infos);
    void update_exp_state(ExpState state, QStringView infos);

    // components
    void add_new_component(Component::Type type, RowId id);
    void copy_component(Component *component, std::vector<ConfigKey> configKeys, RowId id);
    void duplicate_component(ComponentKey componentKey);
    void remove_component(ComponentKey componentKey);
    void update_component_position(ComponentKey componentKey, RowId id);
    void update_component_name(ComponentKey componentKey, QString name);
    void sort_components_by_category();
    void sort_components_by_type();
    void sort_components_by_name();
    void delete_unused_components();
    // # components configs
    void select_config_in_component(ComponentKey componentKey, RowId id);
    void insert_config_in_component(ComponentKey componentKey, RowId id, QString configName);
    void copy_config_from_component(ComponentKey componentKey, RowId id, QString configName);
    void remove_config_from_component(ComponentKey componentKey, RowId id);
    void move_config_in_component(ComponentKey componentKey, RowId from, RowId to);
    void rename_config_in_component(ComponentKey componentKey, RowId id, QString configName);
    // ## arguments
    void new_arg(ComponentKey componentKey, ConfigKey configKey, Arg arg, bool initConfig);
    void arg_updated(ComponentKey componentKey, ConfigKey configKey, Arg arg, bool initConfig);
    void arg_removed(ComponentKey componentKey, ConfigKey configKey, QStringView argName, bool initConfig);
    void swap_arg(ComponentKey componentKey, ConfigKey configKey, QStringView argName1, QStringView argName2, bool initConfig);

    // elements
    void unselect_all_elements(bool updateSignal = true) noexcept;
    void select_element(ElementKey elementKey, bool updateSignal = true);
    void select_element_id(RowId elementId, bool updateSignal = true);
    void select_element_id_no_nodes(RowId elementId, bool updateSignal = true);
    void select_element_from_ptr(FlowElement *element, bool updateSignal = true);
    void add_element(FlowElement::Type type, size_t index);
    void remove_element(FlowElement *elemToDelete);    
    void remove_element_of_key(ElementKey elementKey);
    void duplicate_element(ElementKey elementKey);
    void clean_current_routine_condition(ElementKey routineKey);
    void clean_all_routine_conditions(ElementKey routineKey);
    void set_duration_for_all_routine_conditions(ElementKey routineKey);
    void remove_selected_element();
    void move_left(size_t id);
    void move_right(size_t id);
    void move_element(ElementKey elementKey);
    void update_element_name(ElementKey elementKey, QString elemName);
    void update_element_informations(ElementKey elementKey, QString informations);

    // # loop
    void select_loop_set(ElementKey loopKey, QString setName);
    void add_loop_sets(ElementKey loopKey, QString sets, RowId id);
    void modify_loop_set_name(ElementKey loopKey, QString setName, RowId id);
    void modify_loop_set_occurrencies_nb(ElementKey loopKey, int setOccuranciesNb, RowId id);
    void modify_loop_type(ElementKey loopKey, Loop::Mode type);
    void modify_loop_nb_reps(ElementKey loopKey, int nbReps);
    void modify_loop_N(ElementKey loopKey, int N);
    void modify_loop_no_following_value(ElementKey loopKey, bool state);
    void remove_set(ElementKey loopKey, RowId id);
    void sort_loop_sets_lexico(ElementKey loopKey);
    void sort_loop_sets_num(ElementKey loopKey);
    void move_loop_set_up(ElementKey loopKey, RowId id);
    void move_loop_set_down(ElementKey loopKey, RowId id);
    void load_loop_sets_file(ElementKey loopKey, QString path);
    void reload_loop_sets_file(ElementKey loopKey);

    // # isi
    void add_isi_interval(ElementKey isiKey, qreal value, RowId id);
    void remove_isi_interval(ElementKey isiKey, RowId id);
    void set_isi_randomize(ElementKey isiKey, bool randomize);
    void modify_isi_interval(ElementKey isiKey, qreal value, RowId id);
    void move_isi_interval_up(ElementKey isiKey, RowId id);
    void move_isi_interval_down(ElementKey isiKey, RowId id);

    // # routine
    void select_routine_condition(ElementKey routineKey, RowId  conditionTabId);
    void move_routine_condition_down(ElementKey routineKey, RowId id);
    void move_routine_condition_up(ElementKey routineKey, RowId id);
    void set_routine_as_randomizer(ElementKey routineKey, bool isARandomizer);    
    // ## condition
    void update_condition_timeline(ElementKey routineKey, ConditionKey conditionKey, double duration, double scale, double uiFactorSize);
    void copy_to_conditions(ElementKey routineKey, ConditionKey conditionKey, std_v1<std::pair<ElementKey,ConditionKey>> conditionsToBeEcrased, bool copyActions, bool copyConnections);
    void delete_actions_from_condition(ElementKey routineKey, ConditionKey conditionKey);
    void fill_actions_from_condition(ElementKey routineKey, ConditionKey conditionKey);
    void clean_actions_from_condition(ElementKey routineKey, ConditionKey conditionKey);
    // ### action
    void add_action(ElementKey routineKey, ConditionKey conditionKey, ComponentKey componentKey,
        std::optional<ConfigKey> ConfigKey, bool fillUpdateTimeline, bool fillVisibilityTimeline);
    void add_action_to_all_conditions(ElementKey routineKey, ComponentKey componentKey,
        std::optional<ConfigKey> ConfigKey, bool fillUpdateTimeline, bool fillVisibilityTimeline);
    void add_action_to_all_routines_conditions(ComponentKey componentKey,
        std::optional<ConfigKey> ConfigKey, bool fillUpdateTimeline, bool fillVisibilityTimeline);
    void insert_action_to(ComponentKey componentKey,  std::vector<std::tuple<ElementKey,ConditionKey,ConfigKey,bool, bool>> details);
    void modify_action(ElementKey routineKey, ConditionKey conditionKey, ComponentKey componentKey,
        bool changeConfig, bool changeUpdateTimeline, bool changeVisibilityTimeline,
        ConfigKey ConfigKey, bool fillUpdateTimeline, bool fillVisibilityTimeline);
    void modify_action_to_all_conditions(ElementKey routineKey, ComponentKey componentKey,
        bool changeConfig, bool changeUpdateTimeline, bool changeVisibilityTimeline,
        ConfigKey ConfigKey, bool fillUpdateTimeline, bool fillVisibilityTimeline);
    void modify_action_to_all_routines_conditions(ComponentKey componentKey,
        bool changeConfig, bool changeUpdateTimeline, bool changeVisibilityTimeline,
        ConfigKey ConfigKey, bool fillUpdateTimeline, bool fillVisibilityTimeline);

    void remove_action_from_all_selected_routine_conditions(ElementKey routineKey, ComponentKey componentKey);
    void remove_action_from_all_routines_conditions(ComponentKey componentKey);
    void remove_action_from_condition(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey, bool update = true);
    void fill_action(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey, bool update, bool visibility);
    void clean_action(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey, bool update, bool visibility);
    void move_action_up(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey);
    void move_action_down(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey);
    // #### timeline
    void add_timeline_interval(ElementKey routineKey,ConditionKey conditionKey, ActionKey actionKey, bool updateTimeline, Interval interval);
    void remove_timeline_interval(ElementKey routineKey,ConditionKey conditionKey, ActionKey actionKey, bool updateTimeline, Interval interval);
    // #### config
    void select_action_config(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey, RowId configTabId);
    // ### connection
    void create_connection(ElementKey routineKey,ConditionKey conditionKey, Connection *connection);
    void delete_connections(ElementKey routineKey, ConditionKey conditionKey);
    // ### component node
    void create_component_node(ElementKey routineKey,ConditionKey conditionKey, ComponentKey componentKey, QPointF pos);
    void move_component_node(ElementKey routineKey,ConditionKey conditionKey, ComponentKey componentKey, QPointF pos);
    // ### connector node
    void create_connector_node(ElementKey routineKey,ConditionKey conditionKey, Connector *connector);
    void modify_connector_node(ElementKey routineKey,ConditionKey conditionKey, ConnectorKey connectorKey, QString name, Arg arg);
    void move_connector_node(ElementKey routineKey,ConditionKey conditionKey, ConnectorKey connectorKey, QPointF pos);
    void duplicate_connector_node(ElementKey routineKey,ConditionKey conditionKey, ConnectorKey connectorKey);
    void set_connector_input_connection_validity(ElementKey routineKey, ConditionKey conditionKey, ConnectorKey connectorKey, bool state);
    // ### connections / connectors / components
    void delete_selected_nodes(ElementKey routineKey, ConditionKey conditionKey);
    void unselect_nodes_and_connections(ElementKey routineKey, ConditionKey conditionKey, bool doUpdate);
    void delete_nodes_and_connections(ElementKey routineKey, ConditionKey conditionKey,
        std_v1<ConnectorKey> connectorsKey, std_v1<ComponentKey> componentsKey, std_v1<ConnectionKey> connectionsKey, bool doUpdate);
    void select_nodes_and_connections(ElementKey routineKey, ConditionKey conditionKey,
        std_v1<ConnectorKey> connectorsKey, std_v1<ComponentKey> componentsKey, std_v1<ConnectionKey> connectionsKey, bool doUpdate);

    // # debug
    void display_exp_infos();

private :    

    // clean
    void remove_elements_not_in_flow();

    auto get_element_iterator(ElementKey elementKey) const -> std::vector<std::unique_ptr<FlowElement>>::const_iterator;
    auto get_element_iterator(FlowElement *element) const -> std::vector<std::unique_ptr<FlowElement>>::const_iterator;


public :

    States states;
    Randomizer randomizer;

    // elements
    std_v1<std::unique_ptr<FlowElement>> elements;
    std_v1<std::unique_ptr<Loop>> loops; // not in the flow
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
    using umap = std::unordered_map<T1,T2>;
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

    static void init(){
        if(m_expManager == nullptr){
            m_expManager = std::make_unique<ExperimentManager>();
        }
    }
    static ExperimentManager *get(){
        if(m_expManager != nullptr){
            return m_expManager.get();
        }
        return nullptr;
    }

    inline void init_current(const QString &numVersion){
        auto currentSource = IdKey::current_source();
        IdKey::set_source(IdKey::Source::Current);
        m_experiment = std::make_unique<Experiment>(numVersion);
        IdKey::set_source(currentSource);
    }

    inline void init_imported(const QString &numVersion){
        auto currentSource = IdKey::current_source();
        IdKey::set_source(IdKey::Source::Imported);
        m_importedExperiment = std::make_unique<Experiment>(numVersion);
        IdKey::set_source(currentSource);
    }

    inline Experiment *current(){
        return m_experiment.get();
    }
    inline Experiment *imported(){
        return m_importedExperiment.get();
    }

    inline void clean_current(){
        auto currentSource = IdKey::current_source();
        IdKey::set_source(IdKey::Source::Current);
        m_experiment = nullptr;
        IdKey::set_source(currentSource);
    }

    inline void clean_imported(){        
        auto currentSource = IdKey::current_source();
        IdKey::set_source(IdKey::Source::Imported);
        m_importedExperiment = nullptr;
        IdKey::set_source(currentSource);
    }

    inline void clean(){
        clean_imported();
        clean_current();
    }

private:

    std::unique_ptr<Experiment> m_experiment = nullptr;
    std::unique_ptr<Experiment> m_importedExperiment = nullptr;
    static inline std::unique_ptr<ExperimentManager> m_expManager = nullptr;
};
}

