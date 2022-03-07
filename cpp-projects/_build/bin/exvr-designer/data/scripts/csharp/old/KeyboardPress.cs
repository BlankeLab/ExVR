
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

// ######### Expy.ExComponent available functions and members (by using p)
// #### State
// Name of the component 
// ## string componentName = name; 
// Component initialization
// ## bool initiliazed = is_initialized();
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
// ## Static.Log.error(string(error));
// Will send a log message to the UI
// ## Static.Log.message(string(message));
// #### Flow
// Send an event for moving to the next flow element
// ## next();
// Send an event for moving to the previous flow element
// ## previous();
// Disable the component, it will be not enabled again before the next routine
// ## close();
// Retrieve component by name
// ## ExComponent otherComponent = get("component_name");
// ## SliderOverlayComponent sliderComponent  = (SliderOverlayComponent)Static.Components.get("component_name");
// Retrieve components by type (ExComponent.CType { UIOverlay, ImageOverlay, Image, VideoOverlay, Volume, Mesh, Audio, Script, Scene, Network, Output, Input)
// ## List<ExComponent> allScriptsComponents = Static.Components.get(ExComponent.CType.Script); 

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
// ## public void keyboard_event(Dictionary<KeyCode, Expy.ButtonState> keys){}
// joypad events from "Input/Joypad" component
// ## public void joypad_key_event(Dictionary<string, Expy.ButtonState> keys){}
// ## public void joypad_axis_event(Dictionary<string, float> axes){}
// network events 
// ## public void network_message_received_event(string componentName, string message){}

namespace Expy{

    public class KeyboardPress : BaseCompiledCSharpComponent{

        // ##### DO NOT REMOVE
        public static MonoBehaviour add_component_to_gameobject(GameObject host) {
            return auto_add(host, MethodBase.GetCurrentMethod().DeclaringType);
        }
        // ###################

        // ### EXPERIMENT FLOW FUNCTIONS (called automatically by the experiment, do not call them manually)

        public bool initialize() {
            Static.Log.message("initialize");
            return true;
        }

        public void start_experiment() {
            // ... 
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

        public void network_message_received_event(string componentName, string message) {
            // ...
        }

        public void keyboard_event(Dictionary<KeyCode, Expy.ButtonState> keys) {
            if (keys[KeyCode.Keypad1].firstFrameUp) {
                Static.Log.error("pressed");
                stop();
            }
        }
    }
}