// system
using System;
using System.Reflection;
using System.Collections.Generic;

// unity
using UnityEngine;
using System.Collections.Specialized;

namespace Ex{

    public class MovePlatformComponent : BaseCompiledCSharpComponent{

        float leftJoyX = 0f;
        float leftJoyY = 0f;

        float rightJoyX = 0f;
        float rightJoyY = 0f;

        SoncebozSGComponent platform = null;

        // ### Ex component flow functions, uncomment to use
        // # main functions
        public override bool initialize() {
            platform = get<SoncebozSGComponent>("Sonceboz SG");
            return true;
        }
        // public override void start_experiment() {}
        // public override void start_routine() {}
        // public override void update() {}
        // public override void stop_routine() {}
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
        //public override void slot1(object value) { }
        // public override void slot2(object value){}
        // public override void slot3(object value){}
        // public override void slot4(object value){}        

        public override void slot5(IdAny idValue) {

            float trXMin = -95f;
            float trYMin = -95f;
            float trZMin = -115.0f;

            float trXMax = 95f;
            float trYMax = 95f;
            float trZMax = 300f;

            float rtXMin = -16.0f;
            float rtYMin = -17.0f;
            float rtZMin = -16.0f;

            float rtXMax = 25.0f;
            float rtYMax = 17.0f;
            float rtZMax = 16.0f;

            float rangeFactor = 0.95f;

            float v = (float)((double)(idValue.value)); 
            if (idValue.id == 0) { // left joystick X
                leftJoyX = v;
            } else if (idValue.id == 1) { // left joystick Y
                leftJoyY = v;
            } else if (idValue.id == 2) { // right joystick X
                rightJoyX = v;
            } else if (idValue.id == 3) { // right joystick Y
                rightJoyY = v;
            }
  
            var trXRange = trXMax - trXMin;
            float trX = leftJoyX;
            // -1 1 -> +1
            trX += 1;
            // 0 2 -> *0.5
            trX *= 0.5f;
            // 0 1 -> * range
            trX *= trXRange;
            // 0 range -> +trXMin
            trX += trXMin;
            // -trMin trMax -> *rangeFactor
            trX *= rangeFactor;

            var trYRange = trYMax - trYMin;
            float trY = leftJoyY;
            // -1 1 -> +1
            trY += 1;
            // 0 2 -> *0.5
            trY *= 0.5f;
            // 0 1 -> * range
            trY *= trYRange;
            // 0 range -> +trXMin
            trY += trYMin;
            // -trMin trMax -> *rangeFactor
            trY *= rangeFactor;


            var trZRange = trZMax - trZMin;
            float trZ = rightJoyY;
            // -1 1 -> +1
            trZ += 1;
            // 0 2 -> *0.5
            trZ *= 0.5f;
            // 0 1 -> * range
            trZ *= trZRange;
            // 0 range -> +trXMin
            trZ += trZMin;
            // -trMin trMax -> *rangeFactor
            trZ *= rangeFactor;

            //log_message("tr: " + trX);
            platform.move_linear_offset(new Vector3((float)((int)trX), (float)((int)trY), (float)((int)trZ)), Vector3.zero);
        }
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
