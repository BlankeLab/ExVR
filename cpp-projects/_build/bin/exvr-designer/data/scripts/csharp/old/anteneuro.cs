// system
using System;
using System.Reflection;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class AnteNeuroRead : BaseCompiledCSharpComponent{

        // ### internal settings, do not remove
        public static MonoBehaviour add_component_to_gameobject(GameObject host) {
            return auto_add(host, MethodBase.GetCurrentMethod().DeclaringType);
        }
        
        // ### Ex component flow functions, uncomment to use
        // # main functions
        public bool initialize() {return true;}
        public void start_experiment() {

            //invoke_signal1("init 0 215");
        }

        // public void start_routine() {}
        public void update() {

            var time = ellapsed_time_routine_ms() * 0.001;
            string text = "time remaining: + "(60 - time);
            
        }
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
        public void slot1(object value) {
            var message = (string)value;

        }
        // public void slot2(object value){}
        // public void slot3(object value){}
        // public void slot4(object value){}

        // # events (will be called if the corresponding component is available)
        // public void keyboard_event(Dictionary<KeyCode, ButtonState> keys) { }
        // public void mouse_click_event(Dictionary<KeyCode, MouseButtonState> keys) { }
        // public void joypad_key_event(Dictionary<string, ButtonState> keys) { }
        // public void joypad_axis_event(Dictionary<string, float> axes) { }
        // public void network_message_received_event(string componentName, string message) { }
    }
}
