// system
using System;
using System.Reflection;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class ReadTxtFileComponent : BaseCompiledCSharpComponent{

        // ### internal settings, do not remove
        public static MonoBehaviour add_component_to_gameobject(GameObject host) {
            return auto_add(host, MethodBase.GetCurrentMethod().DeclaringType);
        }


        System.IO.StreamReader fileReader = null;

        // ### Ex component flow functions, uncomment to use
        // # main functions
        public bool initialize() {return true;}

        public void start_experiment() {

            var pathTxtFile = init_config().get<string>("path");
            fileReader = new System.IO.StreamReader(pathTxtFile);

        }

        public void start_routine() {

            if (fileReader != null) {
                fileReader.DiscardBufferedData();
                fileReader.BaseStream.Seek(0, System.IO.SeekOrigin.Begin);
                string line = fileReader.ReadLine();
                ExVR.Log().message("Start reading: " + line);
            }
        }

        public void update() {
            if (fileReader != null) {
                string line = fileReader.ReadLine();
                if (line != null) {

                    var splits = ExVR.split(line, ',');
                    if (splits.Length == 2) {
                        //ExVR.Log().message("-> " + Convert.ToInt32(splits[0]) + " " + Convert.ToInt64(splits[0]));
                        invoke_signal1(Convert.ToInt32(splits[0]));
                        invoke_signal2(Convert.ToDouble(splits[1]));
                    }
                }
            }
        }

        // public void stop_routine() {}
        // public void stop_experiment(){}
        // public void play(){}
        // public void pause(){}
        // public void set_update_state(bool doUpdate) { }        
        // public void set_visibility(bool visible) { }

        // # for advanced users 
        //public void clean() { }
        // public void pre_update() {}
        // public void post_update() {}
        // public void update_parameter_from_gui(XML.Arg arg);
        // public void update_from_current_config(){}

        // # slots
        // public void slot1(object value){}
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
