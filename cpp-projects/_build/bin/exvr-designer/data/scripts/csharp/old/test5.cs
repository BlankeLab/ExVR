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
        public bool initialize() {

            ExVR.Log().message("###initialize " + init_config().get<int>("testInit"));

            //public ComponentConfig current_config() { return p.c; }
            //public ComponentInitConfig init_config() { return p.initC; }

            
            return true;
        }


        public void start_experiment() {

            ExVR.Log().message("start_experiment " + init_config().get<int>("testInit"));

        }

        public void start_routine() {

            ExVR.Log().message("    start_routine " + init_config().get<int>("testInit") + current_config().get<int>("test1"));

        }
        public void update() {
            int a = init_config().get<int>("testInit") + 5;
            int b = current_config().get<int>("test1") + 5;

            //List<Vector3> tr = new List<Vector3>(3);
            //tr.Add(new Vector3(0, 0, 0));
            //tr.Add(new Vector3(b, 0, 0));
            //tr.Add(new Vector3(1, 1, 1));
            //var avatarComponent = ExComponent.get("Human avatar");
            //avatarComponent.c.set_transform("head_transform", tr);
            //avatarComponent.update_from_current_config();
        }

        public void stop_routine() {

            ExVR.Log().message("    stop_routine " + init_config().get<int>("testInit") + current_config().get<int>("test1"));
        }

        public void stop_experiment() {

            ExVR.Log().message("stop_experiment " + init_config().get<int>("testInit") + current_config().get<int>("test1"));
        }

        public void play() {

            ExVR.Log().message("        play " + init_config().get<int>("testInit") + current_config().get<int>("test1"));
        }

        public void pause() {

            ExVR.Log().message("        pause " + init_config().get<int>("testInit") + current_config().get<int>("test1"));

        }

        public void set_update_state(bool doUpdate) {
            ExVR.Log().message("    set_update_state " + init_config().get<int>("testInit") + current_config().get<int>("test1"));
        }

        public void set_visibility(bool visible) {
            ExVR.Log().message("    set_visibility " + init_config().get<int>("testInit") + current_config().get<int>("test1"));
        }

        // # for advanced users 
        public void clean() {

            ExVR.Log().message("### clean " + init_config().get<int>("testInit"));// + current_config().get<int>("test1"));
        }

        public void pre_update() {
            int a = init_config().get<int>("testInit") + 5;
            int b = current_config().get<int>("test1") + 5;
        }

        public void post_update() {
            int a = init_config().get<int>("testInit") + 5;
            int b = current_config().get<int>("test1") + 5;
        }

        public void update_parameter_from_gui(XML.Arg arg) {
            ExVR.Log().message("    update_parameter_from_gui " + init_config().get<int>("testInit") + current_config().get<int>("test1"));
        }

        public void update_from_current_config() {
            ExVR.Log().message("    update_from_current_config " + init_config().get<int>("testInit") + current_config().get<int>("test1"));
        }

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
