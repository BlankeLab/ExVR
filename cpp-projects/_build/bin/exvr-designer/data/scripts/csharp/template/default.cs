// system
using System;
using System.Reflection;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class DefaultNameComponent : BaseCompiledCSharpComponent{

        // Ex component flow functions, uncomment to use
        // # once per loading
        public override bool initialize() { return true; }
        //public override void clean() { }

        // # once per experiment
        public override void start_experiment() { }
        //public override void post_start_experiment() { }
        //public override void pre_stop_experiment() { }
        //public override void stop_experiment() { }

        // once per routine
        //public override void set_current_config(string configName) { }
        //public override void update_from_current_config() { }
        //public override void pre_start_routine() { }
        public override void start_routine() { }
        //public override void post_start_routine() { }
        //public override void stop_routine() { }

        // several time per routine
        //public override void set_update_state(bool doUpdate) { }
        //public override void set_visibility(bool visible) { }
        //public override void play() { }
        //public override void pause() { }

        // every frame or more        
        //public override void pre_update() { }
        public override void update() { }
        //public override void post_update() { }
        //public override void on_gui() { }
        //public override void end_of_frame() { }

        // from gui
        //public override void action_from_gui(bool initConfig, string action) { }
        //public override void update_parameter_from_gui(string updatedArgName) { }

        // # slots
        //public override void slot1(object value) { }
        //public override void slot2(object value) { }
        //public override void slot3(object value) { }
        //public override void slot4(object value) { }
        //public override void slot5(IdAny idValue) {
        // int id = idValue.id;
        // object value = idValue.value;
        //}
    }
}


/**
namespace Ex{    

    public class BaseCompiledCSharpComponent : MonoBehaviour{

        // ExComponent class associated to the script (see ExComponent code snippet) 
        public ExComponent p = null;
        
        // randomization
        public int instance_id();
        public Interval current_element_interval();
        public int current_element_order();
        public int current_element_iteration();
        public List<RoutineInfo> get_current_routine_instance_infos();
        public List<Condition> get_current_routine_instance_conditions();
        public List<string> get_current_routine_instance_conditions_names();
        public List<string> get_routine_instance_conditions_names(string routineName);

        // routines 
        public Routine current_routine(); //  current routine associated to this component
        public Routine get_routine(string routineName);

        // conditions
        public Condition current_condition(); // current condition of the associated routine

        // configs
        public ComponentInitConfig init_config(); // init configuration for this component 
        public ComponentConfig current_config(); // current configuration of the routine for this component
        public void update_current_config(int configKey);
        public void update_current_config(string configName);
        public void update_current_config(ComponentConfig config);

        // connections
        public Events.Connections connections();

        // states
        public bool is_visible();
        public bool is_updating();
        public bool is_closed();

        // commands
        public void next();
        public void next_with_name(string componentName);
        public void schedule_next(int milliseconds);
        public void previous();
        public void previous_with_name(string componentName);
        public void close();
        public void stop();

        // times
        public long frame_id();
        public double ellapsed_time_exp_ms();
        public double ellapsed_time_routine_ms();
        public double ellapsed_time_frame_ms();

        // logs
        public void log_message(string message, bool verbose = false, bool addExtraInfo = false);
        public void log_warning(string warning, bool verbose = true, bool addExtraInfo = true);
        public void log_error(string error, bool verbose = true, bool addExtraInfo = true);

        // signals
        public void invoke_signal1(object value);
        public void invoke_signal2(object value);
        public void invoke_signal3(object value);
        public void invoke_signal4(object value);

        // components
        // # get
        public ExComponent get(int key);
        public ExComponent get(string name);
        public T get<T>(string name) where T : ExComponent;
        public T get_first<T>() where T : ExComponent;
        public void set_first<T>(ref T component) where T : ExComponent;
        public ExComponent get_component(int key);
        public ExComponent get_component(string name);
        public T get_component<T>(string name) where T;
        // # get all
        public List<ExComponent> get_all(ExComponent.Category type);
        public List<T> get_all<T>() where T : ExComponent;
        public List<ExComponent> get_all_from_category(string categoryStr);
        public List<ExComponent> get_all_from_type(string typeStr);
        // # alias
        public List<ExComponent> get_all_components(ExComponent.Category type);
        public List<T> get_all_components<T>() where T : ExComponent;
        public List<ExComponent> get_all_components_from_category(string categoryStr);
        public List<ExComponent> get_all_components_from_type(string typeStr);

        // get from current condition
        public List<ExComponent> get_all_from_current_condition();
        public List<T> get_all_from_current_condition<T>() where T : ExComponent;
        public List<ExComponent> get_all_components_from_current_condition();
        public List<T> get_all_components_from_current_condition<T>() where T : ExComponent;

        // get other scripts
        public T get_csharp_script<T>(string name) where T : BaseCompiledCSharpComponent;        

        #region ex_functions

        // once per loading
        public virtual bool initialize() { return true; }
        public virtual void clean() { }

        // once per experiment
        public virtual void start_experiment() { }
        public virtual void post_start_experiment() { }        
        public virtual void pre_stop_experiment() { }
        public virtual void stop_experiment() { }

        // once per routine
        public virtual void set_current_config(string configName) { }
        public virtual void update_from_current_config() { }
        public virtual void pre_start_routine() { }
        public virtual void start_routine() {}
        public virtual void post_start_routine() { }
        public virtual void stop_routine() { }

        // several time per routine
        public virtual void set_update_state(bool doUpdate) { }
        public virtual void set_visibility(bool visible) { }
        public virtual void play() { }
        public virtual void pause() { }

        // every frame or more        
        public virtual void pre_update() { }
        public virtual void update() { }
        public virtual void post_update() { }
        public virtual void on_gui() { }
        public virtual void end_of_frame() { }        

        // from gui
        public virtual void action_from_gui(bool initConfig, string action) { }
        public virtual void update_parameter_from_gui(string updatedArgName) { }

        // # slots
        public virtual void slot1(object value) { }
        public virtual void slot2(object value) { }
        public virtual void slot3(object value) { }
        public virtual void slot4(object value) { }
        public virtual void slot5(IdAny idValue) { }

        // # transform related
        public virtual void set_position(Vector3 position) {
            transform.position = position;
        }
        public virtual void set_rotation(Vector3 rotation) {
            transform.eulerAngles = rotation;
        }
        public virtual void set_rotation(Quaternion rotation) {
            transform.rotation = rotation;
        }
        public virtual void set_scale(Vector3 scale) {
            transform.localScale = scale;
        }

        public virtual Vector3 position() {
            return transform.position;
        }
        public virtual Quaternion rotation() {
            return transform.rotation;
        }
        public virtual Vector3 euler_angles() {
            return transform.eulerAngles;
        }

        public virtual Vector3 scale() {
            return transform.localScale;
        }

        #endregion

    }
		
	public class ExComponent : MonoBehaviour{

		// enums
        public enum Category {
            AcquisitionDevice,Audio,Avatar,Camera,Environment,Flow,InputDevice,Interaction,Model,
            Network,Output,Resource,Script,Scene,Settings,Text,UserInterface,Video,Viewer,Undefined
        };
        public enum Priority { Low, Medium, Hight};
        public enum Reserved {Public, Closed};

		// members
        public int key = -1; // id component
        public string keyStr;
        public string typeStr;
        public Category category = Category.Undefined;
        public Priority priority = Priority.Medium;

        public Routine currentRoutine = null;
        public Condition currentCondition = null;
        public TimeLine currentTimeline = null;
        protected Events.Connections m_connections = null;

        protected bool m_visibility = false; // is visible ?
        protected bool m_updating = false;   // call update function ?
        protected bool m_alwaysCallUpdate = false; // call update even if not in timeline
        protected bool m_global = false;           // call update even if not in condition
        private bool m_started = false;      // has associated routine started ?
        private bool m_closed = false;       // is closed ? (cannot be enabled anymore until next routine)
        private bool m_initialized = false;  // has initialization failed ? 
        protected bool m_catchExceptions = false;
        protected bool m_frameLogging = false;
        protected bool m_triggerLogging = false;

        // access        
        public Components components();
        public Log log();
        public TimeManager time();
        public EventsManager events();
        public Events.Connections connections();
        public Events.Command command();
        public bool is_function_defined(Function function);

        // connections
        protected Events.Signal add_signal(string name);
        protected Events.Slot add_slot(string name, System.Action<object> action);
        public bool is_signal_connected(string name);
        public void invoke_signal(string name, object arg = null);

        // configs
        public ComponentInitConfig initC = null;
        public ComponentConfig currentC = null; // current config
        public List<ComponentConfig> configs = null;
        public Dictionary<int, ComponentConfig> configsPerKey = null;
        public Dictionary<string, ComponentConfig> configsPerName = null;

        // states
        protected bool is_initialized();
        public bool is_started();
        public bool is_visible();
        public bool is_updating();
        public bool is_global();
        public bool has_frame_logging();
        public bool has_trigger_logging();
        public bool always_call_update();
        public bool is_closed();
        public void set_closed_flag(bool closed);
        public void close();
        public string component_name();
        public string verbose_name();

        // logs
        public void log_message(object message, bool verbose = false, bool extra = false);
        public void log_warning(object warning, bool verbose = true, bool extra = false);
        public void log_error(object error, bool verbose = true, bool extra = true);

        // configs
        public ComponentInitConfig init_config();
        public bool has_current_config();
        public ComponentConfig current_config();
        public ComponentConfig get_config(string configName, bool displayError = true);
        public ComponentConfig get_config(int configKey, bool displayError = true);
        public void update_current_config(string configName);
        public void update_current_config(int configKey);
        public void update_current_config(ComponentConfig config);

        // routine
        public bool has_current_routine();
        public Routine current_routine();

        // condition
        public bool has_current_condition();
        public Condition current_condition();

        // once per loading
        protected virtual bool initialize() {return true; }
        protected virtual void clean() { }

        // once per experiment
        protected virtual void start_experiment() { }
        protected virtual void post_start_experiment() { }
        protected virtual void pre_stop_experiment() { }
        protected virtual void stop_experiment() { }

        // once per routine
        protected virtual void set_current_config(string configName) {}
        public virtual void update_from_current_config() { }
        protected virtual void pre_start_routine() {}
        protected virtual void start_routine() {}        
        protected virtual void post_start_routine() {}
        protected virtual void stop_routine() {}

        // several times per routine
        protected virtual void set_visibility(bool visible) { }
        protected virtual void set_update_state(bool doUpdate) { }
        public virtual void play() { }
        public virtual void pause() { }


        // every frame or more        
        protected virtual void pre_update() {}
        protected virtual void update() {}
        protected virtual void post_update() {}
        public virtual void global_update() { }
        protected virtual void on_gui() { }
        protected virtual void end_of_frame() { }

        // events
        protected virtual void first_event(bool isCalledBeforeUpdate) {} // can be called before or after pre_update/update/post_update
        protected virtual void keyboard_event(Event kEvent) { } // called before pre_update/update/post_update
        protected virtual void mouse_event(Event mEvent) { } // called before pre_update/update/post_update
        protected virtual void last_event() { } // called after pre_update/udpate/post_update

        // from gui
        protected virtual void update_parameter_from_gui(string updatedArgName) {}
        protected virtual void action_from_gui(bool initConfig, string action) {}

        // logging
        public virtual string format_frame_data_for_global_logger(bool header) { return null; }
        public virtual List<Tuple<double, double, string>> format_trigger_data_for_global_logger() { 
            // exp time / routine time / data
            return null; 
        }

        // transform related function
        public virtual void set_position(Vector3 position) {
            transform.position = position;
        }
        public virtual void set_rotation(Vector3 rotation) {
            transform.eulerAngles = rotation;
        }
        public virtual void set_rotation(Quaternion rotation) {
            transform.rotation = rotation;
        }
        public virtual void set_scale(Vector3 scale) {
            transform.localScale = scale;
        }

        public virtual Vector3 forward() {
            return transform.forward;
        }

        public virtual Vector3 position() {
            return transform.position;
        }
        public virtual Quaternion rotation() {
            return transform.rotation;
        }

        public virtual Vector3 euler_angles() {
            return transform.eulerAngles;
        }

        public virtual Vector3 scale() {
            return transform.localScale;
        }

        public virtual void reset_init_transform() {
            if (!initC.get<bool>("init_transform_do_not_apply")) {
                initC.update_transform("init_transform", transform, true);
            }
        }
        public virtual void reset_config_transform() {
            if (!currentC.get<bool>("transform_do_not_apply")) {
                currentC.update_transform("transform", transform, true);
            }
        }
    }
}
*/
