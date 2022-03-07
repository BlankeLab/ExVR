
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
// ## bool initiliazedd = is_initialized();
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
// ## public void keyboard_event(Dictionary<string, Expy.ButtonState> keys){}
// joypad events from "Input/Joypad" component
// ## public void joypad_key_event(Dictionary<string, Expy.ButtonState> keys){}
// ## public void joypad_axis_event(Dictionary<string, float> axes){}
// network events 
// ## public void network_message_received_event(string componentName, string message){}

namespace Expy{

    public class VisualStimuliComponent : BaseCompiledCSharpComponent{

        float routineDuration = 15f;
        float currentTime = 0.0f;

        // ball
        FlashingDotComponent ball = null;
        bool isBallMoving = true;
        float ballDirection = 1.0f;
        float currentBallPosition = 0.0f;
        float ballStartingPoint = -0.270f; // hmd
        float ballEndingPoint = -0.06f; // hmd
        float lengthBallMovement;
        float ballMovingTime = 0.0f;

        // vibration
        SerialPortWriterComponent tactile = null;
        float vibrationDuration = 0.8f;
        float systemDelayVt = 0.1f;
        float vibrationInterval = 1.6f;
        float vibrationDelay = 0f;
        // # messages to send
        Byte[] externalStim1 = new Byte[] { (Byte)255, (Byte)09, (Byte)19, (Byte)29, (Byte)39, (Byte)49, (Byte)59, (Byte)60, (Byte)70, (Byte)80, (Byte)90, (Byte)100, (Byte)110, (Byte)0, (Byte)80, (Byte)3, (Byte)16, (Byte)170 };
        Byte[] externalStim2 = new Byte[] { (Byte)255, (Byte)59, (Byte)49, (Byte)39, (Byte)29, (Byte)19, (Byte)09, (Byte)60, (Byte)70, (Byte)80, (Byte)90, (Byte)100, (Byte)110, (Byte)0, (Byte)80, (Byte)3, (Byte)16, (Byte)170 };
        Byte[] externalStop = new Byte[] { (Byte)255, (Byte)50, (Byte)40, (Byte)30, (Byte)20, (Byte)10, (Byte)00, (Byte)60, (Byte)70, (Byte)80, (Byte)90, (Byte)100, (Byte)110, (Byte)0, (Byte)0, (Byte)0, (Byte)0, (Byte)170 };
        Byte[] externalClick = new Byte[] { (Byte)255, (Byte)09, (Byte)19, (Byte)29, (Byte)39, (Byte)49, (Byte)59, (Byte)69, (Byte)79, (Byte)89, (Byte)99, (Byte)109, (Byte)119, (Byte)0, (Byte)0, (Byte)7, (Byte)104, (Byte)170 };


        // ##### DO NOT REMOVE
        public static MonoBehaviour add_component_to_gameobject(GameObject host) {
            return auto_add(host, MethodBase.GetCurrentMethod().DeclaringType);
        }
        // ###################       
        

        public bool initialize() {
            return true;
        }

        public void start_experiment() {
        }

        public void start_routine() {
            
            // get components
            ball = (FlashingDotComponent)Static.Components.get("Flashing dot");            
            tactile = (SerialPortWriterComponent)Static.Components.get("Tactile");

            // init tactile
            tactile.write_bytes(externalStim1, 0, externalStim1.Length);

            // reset time
            currentTime = 0;
            routineDuration = (float)p.currentCondition.max_time();
            Static.Log.message("routineDuration " + routineDuration);

            // reset 
            currentBallPosition = ballStartingPoint;
            ballDirection = 1.0f;
            isBallMoving = true;
            ballMovingTime = 0.0f;
            lengthBallMovement = ballEndingPoint - ballStartingPoint;

            // set delay according from condition
            var currCondition = p.currentCondition.name();
            if (currCondition.Contains("asynch")) {
                vibrationDelay = 0.8f;
                Static.Log.error("asynch");
            } else {
                vibrationDelay = 0f;
                Static.Log.error("synch");
            }
        }

        public void invert_ball_state() {
            ball.c.set<bool>("on", !ball.c.get<bool>("on"));
        }

        public void update() {

            // update ball position
            //var ballTr = ball.transform;
            //ballTr.localPosition = new Vector3(currentBallPosition, 0f, 0f);

            var vrPos = Static.VR.eyeCamera.transform.localPosition;

            // update current time
            currentTime = currentTime + Time.deltaTime;

            if (currentTime < systemDelayVt + vibrationDelay){
                // do nothing
            }else if (currentTime < vibrationDuration+ systemDelayVt + vibrationDelay && ballDirection ==1f && isBallMoving ==true){

                if (ballMovingTime < vibrationDuration){
                    ballMovingTime += Time.deltaTime;
                    invert_ball_state();
                }
                currentBallPosition =ballStartingPoint + lengthBallMovement * ballMovingTime / vibrationDuration;
            }
            else if (currentTime < vibrationDuration + systemDelayVt + vibrationDelay && ballDirection == -1f && isBallMoving == true){

                if (ballMovingTime < vibrationDuration){
                    ballMovingTime += Time.deltaTime;
                    invert_ball_state();
                }
                currentBallPosition = ballEndingPoint - lengthBallMovement*ballMovingTime / vibrationDuration;
            }
            else if (currentTime >= vibrationDuration + systemDelayVt + vibrationDelay && isBallMoving ==true){

                isBallMoving = false;
                ball.c.set<bool>("on", false);
            }
            else if(currentTime >= vibrationDuration + systemDelayVt + vibrationInterval && isBallMoving ==false){

                currentTime = 0.0f;
                isBallMoving = true;
                ballMovingTime = 0.0f;
                if (ballDirection == -1f){
                    ballDirection = 1f;
                    tactile.write_bytes(externalStim1, 0, externalStim1.Length);
                }else{
                    ballDirection = -1f;
                    tactile.write_bytes(externalStim2, 0, externalStim2.Length);
                }
            }

            var currCondition = p.currentCondition.name();
            if (currCondition.Contains("asynch")) {
                vibrationDelay = vibrationDelay = 0.8f - vibrationDuration / routineDuration * ExComponent.ellapsed_time_routine_ms() / 1000;                
            } else {
                vibrationDelay = vibrationDuration / routineDuration * ExComponent.ellapsed_time_routine_ms() / 1000;
            }

            ball.update_from_current_config();
            ball.transform.localPosition = vrPos - new Vector3(currentBallPosition-0.3f, 0f, 0f);
        }

        public void stop_routine() {

            if (p.currentRoutine.name == "Up+VT"){
                tactile.write_bytes(externalClick, 0, externalClick.Length);
            }else{
                tactile.write_bytes(externalStop, 0, externalStop.Length);
            }
        }

    }
}