// system
using System;
using System.Reflection;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class TestComponent : BaseCompiledCSharpComponent{

        // ### internal settings, do not remove
        public static MonoBehaviour add_component_to_gameobject(GameObject host) {
            return auto_add(host, MethodBase.GetCurrentMethod().DeclaringType);
        }

        // ### Ex component flow functions, uncomment to use
        // # main functions
        public bool initialize() { return true; }
        public void start_experiment() { }
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

        // slots
        public void slot1(object value) {
            var v = (double)value;            
            if(v > 0) {
                v = 1;
            } else {
                v = 0;
            }
            invoke_signal1(v);
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
