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


        public void start_routine() {
            times = new List<double>();
        }




        public void update() {

            double totalDiff = 0;
            if(times.Count < 5) {
                return;
            }

            for (int ii = 0; ii < times.Count-1; ++ii) {
                var diff = times[ii + 1] - times[ii];
                totalDiff += diff;
            }
            double bpm = 1000.0 / (totalDiff / (times.Count - 1));

            invoke_signal1(times.Count);
            invoke_signal2(bpm);
        }


        List<double> times = null;

        // # slots
        public void slot1(object value) {

            int nbBeatsToKeep = current_config().get<int>("beats_number_to_keep");
            times.Add(ellapsed_time_routine_ms());
            if(times.Count > nbBeatsToKeep) {
                times.RemoveAt(0);
            }
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
