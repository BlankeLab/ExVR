// system
using System;
using System.Reflection;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class TemplateComponent : BaseCompiledCSharpComponent{

        // ### internal settings, do not remove
        public static MonoBehaviour add_component_to_gameobject(GameObject host) {
            return auto_add(host, MethodBase.GetCurrentMethod().DeclaringType);
        }
        
        // ### Ex component flow functions, uncomment to use
        // # main functions
        public bool initialize() {return true;}
        public void start_experiment() {}
        // public void start_routine() {}
        // public void update() {}
        // public void stop_routine() {}
        // public void stop_experiment(){}
        // public void play(){}
        // public void pause(){}
        // public void set_update_state(bool doUpdate) { }        
        // public void set_visibility(bool visible) { }
        
        // # for advanced users 
        // public void clean(){}
        // public void pre_update() {}
        // public void post_update() {}
        // public void update_parameter_from_gui(XML.Arg arg);
        // public void update_from_current_config(){}
        
        // # slots
        // public void slot1(object value){}
        // public void slot2(object value){}
        // public void slot3(object value){}
        // public void slot4(object value){}        
		// public void slot5(IdAny idValue){
		// 	int id = idValue.id;
		// 	object value = idValue.value;
		// }  
    }
}


/**
namespace Ex{    

    public class BaseCompiledCSharpComponent : MonoBehaviour{

        // ExComponent class associated to the script (see ExComponent code snippet) 
        public ExComponent p = null;
        
        // init configuration for this component 
        public ComponentInitConfig init_config();
        // current configuration of the routine for this component
        public ComponentConfig current_config();
        // current routine associated to this component
        public Routine current_routine();
        // current condition of the associated routine
        public Condition current_condition();
        
        // states
        public bool is_initialized();
        public bool is_visible();
        public bool is_updating();
        public bool is_closed();

        // getters of others components
        public List<ExComponent> get_all(string category);
        public List<ExComponent> get_all(ExComponent.CType type);
        public ExComponent get(int key);
        public ExComponent get(string name);
        public List<T> get<T>() where T : ExComponent;
        public T get<T>(string name) where T : ExComponent;

        // special commands
        public void next();
        public void previous();
        public void close();
        public void stop();
        
        // timers
        public long ellapsed_time_exp_ms();
        public long ellapsed_time_routine_ms();
        
        // logging
        public void log_message(string message);
        public void log_error(string error);
        
        // invoke signals
        public void invoke_signal1(object value);
        public void invoke_signal2(object value);
        public void invoke_signal3(object value);
        public void invoke_signal4(object value);
        
    }
		
	public class ExComponent : MonoBehaviour{

		// enums
		public enum CType { UI, Image, Video, Tracking, Model, Avatar, Audio, Script, Scene, Network, Output, Input, Text, Interaction, Feedback, Camera, Settings, Cloud, Undefined };
		public enum InitPriority { Low, Medium, Hight};		       

		// members
		// # misc
        public int key;
        public CType type;
        public InitPriority initPriority;

		// # data 
		public Routine currentRoutine;
		public Condition currentCondition;
        public TimeLine currentTimeline;      
        // ## configs
        public ComponentConfig currentC; // current config
        public ComponentInitConfig initC; // init config
        public List<ComponentConfig> configs;		

        // # events
        public Events.SignalsSlotsConnections events();

		// functions		
		// # misc
		public string component_name();
		public string type_str();               
        
		// # log
        static public void log_message(string message);
        static public void log_error(string error);
		
		// # time
        static public long ellapsed_time_exp_ms();
        static public long ellapsed_time_routine_ms();
		
		// # get
        static public List<ExComponent> get_all(string category);
        static public List<ExComponent> get_all(CType type);
        static public ExComponent get(int key);
        static public ExComponent get(string name);
        static public List<T> get<T>() where T : ExComponent;
        static public T get<T>(string name) where T : ExComponent;

		// # specials commands
        public static void next(); //  Send an event for moving to the next flow element        
        public static void previous(); // Send an event for moving to the previous flow element
        public static void stop();
        public static void next_element_with_name(string elementName);
        public static void previous_element_with_name(string elementName);
        public static void next_element_with_condition(string elementName);
        public static void previous_element_with_condition(string elementName);
        public static void modify_routine_action_config(string routineName, string conditionName, string componentName, string newConfigName);
		public void close(); // Disable the component, it will be not enabled again before the next routine
		
		// # configs
        public ComponentInitConfig init_config();
        public ComponentConfig current_config();
        public ComponentConfig get_config(string configName);

		// # states
        public bool is_started();
        public bool is_initialized();
        public bool is_visible();
        public bool is_updating();
        public bool is_closed();
    }
}
*/
