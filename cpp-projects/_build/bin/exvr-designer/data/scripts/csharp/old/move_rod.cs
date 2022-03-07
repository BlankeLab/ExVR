
// system
using System;
using System.Reflection;
using System.Collections.Generic;

// unity
using UnityEngine;

// Every component is a GameObject and uses MonoBehaviour, so you have access to:
// # gameObject
// # transform
// # and any other component by using AddComponent<T> and GetComponent<T>

// ################## Utility functions available:

// ######### Expy.Component available functions and members (by using p)
// #### State
// Name of the component
// ## string componentName =component_name();
// Component visibility (timeline)
// ## bool visibility = is_visible();
// Component is updating (timeline)
// ## bool isUdpating = is_updating();
// Is component closed? (a closed component cannot be started again during the rest of the timeline routine)
// ## bool closed = return is_closed();
// #### Configurations
// Get the initialization configuration
// ##Expy.ComponentInitConfig initConfig = init_config();
// Get the config associated to the current condition
// ## Expy.ComponentConfig currentConfig = current_config();
// #### Messages 
// Will send a log error to the UI
// ## log_error(string(error));
// Will send a log message to the UI
// ## log(string(message));
// #### Flow
// Send an event for moving to the next flow element
// ## next();
// Send an event for moving to the previous flow element
// ## previous();
// Disable the component, it will be not enabled again before the next routine
// ## close();
// Retrieve component by type and name (ComponentType : Overlay, Object3D, Device, Audio, Script, Scene, Network, Input, Undefined)
// ## var otherCmponent = get_component(Expy.Component.ComponentType(type), string(componentName));

// ######### For getting arguments from configurations (Expy.ComponentInitConfig and Expy.ComponentConfig)
// For simples types (bool,int,string,float...)
// ## public T get<T>(string argName); // 
// For list
// ## public List<T> get_list<T>(string argName);
// Unity types
// ## public Color get_color(string argName);
// ## public Vector2 get_vector(string argName);
// Unity transform
// ## public List<Vector3> get_transform(string argName);
// ## Examples:
// ## string path = current_config().get<string>("path_file");
// ## List<float> values = init_config().get_list<float>("some_values");


// ################## Add and define in the class the functions you need (they will be called automatically by the manager, do not use one inside another)

// ######### Flow functions

// Initialize the component, called only once at the beginning of the experiment
// ## public bool initialize(){}
// ## Initialize by using init_config()
// ## Example: 
// ## string path = init_config().get<string>("some_file_path");
// Start the whole experiment, called once after the beggining
// ## public void start_experiment(){}
// Stop the whole experiment, called once at the end
// ## public void stop_experiment(){}
// Play from GUI button
// ## public void play(){}
// Pause from GUI button
// ## public void pause(){}
// Start the component, called every time at the timeline start of an associated routine
// ## public void start_routine(){}    
// Stop the component, called every time at the timeline end of an associated routine
// ## public void stop_routine(){}    
// Clean the component, called when component is destroyed (when program is left, or experiment is reloaded)
// ## public void clean(){}
// Manage the visibility of your GameObjects ("visibility" is true if current time is inside visibility timeline)
// ## public void set_visibility(bool visibility){}
// Called at each mofification of any UI elements of the component (only for current configuration)
// ## public void update_parameter_from_gui(Expy.XML.Arg arg) {
// ##   // example
// ##   if (arg.Name == "path"){
// ##      string path = current_config().get<string>(arg.Name);
// ##   }
// ## }

// ######### Events call by others components
// keyboard events from "Input/Keyboard" component
// ## public void keyboard_event(Dictionary<string, Expy.ButtonState> keys){}
// joypad events from "Input/Joypad" component
// ## public void joypad_key_event(Dictionary<string, Expy.ButtonState> keys){}
// ## public void joypad_axis_event(Dictionary<string, float> axes){}

namespace Expy{

    public class MoveRod : BaseCompiledCSharpComponent{

        // ##### DO NOT REMOVE
        public static MonoBehaviour add_component_to_gameobject(GameObject host) {
            return auto_add(host, MethodBase.GetCurrentMethod().DeclaringType);
        }
        // ###################

        // ### EXPERIMENT FLOW FUNCTIONS (called automatically by the experiment, do not call them manually)

        Component rod = null;

        public bool initialize() {
            log("initialize MoveRod");
            return true;
        }

        public void start_experiment() {
            rod = Component.get("3D rod image");
        }

        public void start_routine() {
            // ...
        }

        public void update() {
            // ...
        }
        
        public void stop_routine() {
            // ... 
        }
        
        public void stop_experiment() {
            // ... 
        }

        public void joypad_axis_event(Dictionary<string, float> axes) {

            foreach (var axe in axes) {
                if (axe.Key == "DPadX_P1") {
                    if (axe.Value != 0f) {
                        if (axe.Value < 0f) {
                            //log("dpad left" + axe.Value);
                            var currentRot = rod.transform.rotation.eulerAngles;
                            currentRot.z -= 1;
                            rod.transform.rotation = Quaternion.Euler(currentRot);

                        } else {
                            var currentRot = rod.transform.rotation.eulerAngles;
                            currentRot.z += 1;
                            rod.transform.rotation = Quaternion.Euler(currentRot);
                        }
                        return;
                    }
                }
            }
        }
    }
}