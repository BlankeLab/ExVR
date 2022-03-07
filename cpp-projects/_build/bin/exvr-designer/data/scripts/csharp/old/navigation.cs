// system
using System;
using System.Reflection;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class NavigationComponent : BaseCompiledCSharpComponent{

        // ### Ex component flow functions, uncomment to use
        // # main functions
        // public override bool initialize() {return true;}

        AssetBundleComponent arena = null;

        Transform targetParent = null;
        List<Transform> targets = null;

        List<Transform> groups = null;
        List<List<GameObject>> objects = null;

        List<int> categoriesOrders = new List<int>();
        List<int> positions = new List<int>();
        int lastGroup1Object = -1;
        int lastGroup2Object = -1;
        int lastGroup3Object = -1;
        int currentEncoding = 0;
        int currentRetrieval = 0;

        public override void start_experiment() {

            arena = get<AssetBundleComponent>("Arena");

            targetParent = arena.transform.Find("targets");
            targets = new List<Transform>();
            for (int ii = 1; ii < 10; ++ii) {
                targets.Add(targetParent.Find("t" + ii));
            }

            objects = new List<List<GameObject>>();
            objects.Add(new List<GameObject>(9));
            objects.Add(new List<GameObject>(9));
            objects.Add(new List<GameObject>(9));

            var topObjects = arena.transform.Find("objects");
            groups = new List<Transform>(3);
            groups.Add(topObjects.Find("group1"));
            groups.Add(topObjects.Find("group2"));
            groups.Add(topObjects.Find("group3"));

            for(int ii = 0; ii < groups.Count; ++ii) {
                for(int jj = 0; jj < groups[ii].GetChildCount(); ++jj) {
                    objects[ii].Add(groups[ii].GetChild(jj).gameObject);
                }
            }
        }

        public override void start_routine() {

            string routineName = current_routine().name;
            string conditionName = current_condition().name;
            var splitCondition = conditionName.Split(new string[] { "-" }, System.StringSplitOptions.None);

            if (routineName.Contains("Reset")) {
                // clean previous randomization
                lastGroup1Object = -1;
                lastGroup2Object = -1;
                lastGroup3Object = -1;
                currentEncoding = 0;
                currentRetrieval = 0;
                categoriesOrders.Clear();
                positions.Clear();
                next();
            } else if (routineName.Contains("g1-obj")) {
                lastGroup1Object = Convert.ToInt32(splitCondition[1]) - 1;
                next();
            } else if (routineName.Contains("g2-obj")) {
                lastGroup2Object = Convert.ToInt32(splitCondition[1]) - 1;
                next();
            } else if (routineName.Contains("g3-obj")) {
                lastGroup3Object = Convert.ToInt32(splitCondition[1]) - 1;
                next();
            } else if (routineName.Contains("order")) {
                for (int ii = 0; ii < 3; ++ii) {
                    categoriesOrders.Add(Convert.ToInt32(splitCondition[1][ii]) - 1);
                }
                next();
            } else if (routineName.Contains("positions")) {
                positions.Add(Convert.ToInt32(splitCondition[1]) - 1);
                next();
            } else if (routineName == "Encoding") {

            } else if (routineName == "Retrieval") {

            }
        }
        // public override void update() {}

        public override void stop_routine() {

            if (routineName == "Encoding") {
                ++currentEncoding;
            } else if (routineName == "Retrieval") {
                ++currentRetrieval;
            }
        }

        // public override void stop_experiment(){}
        // public override void play(){}
        // public override void pause(){}
        // public override void set_update_state(bool doUpdate) { }        
        // public override void set_visibility(bool visibility) { }

        // # for advanced users 
        // public override void clean(){}
        // public override void pre_update() {}
        // public override void post_update() {}
        // public override void update_parameter_from_gui(XML.Arg arg){}
        // public override void update_from_current_config(){}

        // # slots
        // public override void slot1(object value){}
        // public override void slot2(object value){}
        // public override void slot3(object value){}
        // public override void slot4(object value){}        
        // public override void slot5(IdAny idValue){
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
        public double ellapsed_time_exp_ms();
        public double ellapsed_time_routine_ms();
        
        // logging
        public void log_message(string message);
		public void log_warning(string warning);
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
        static public double ellapsed_time_exp_ms();
        static public double ellapsed_time_routine_ms();
		
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
