// system
using System;
using System.Diagnostics;
using System.Reflection;
using System.Collections;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{


    public class ObjectInfo{

        public bool hasBeenResized = false;
        public AssetBundleComponent obj = null;
    }

    public class GazeComponent : BaseCompiledCSharpComponent{

        // default values are defines here, but if an user interface has been added in the CSharpScriptComponent, it will update theses values with it (UI tag)
        // # debug
        public bool displayGazeTarget = true; // (UI) display the sphere component as the gaze target 
        // # times
        // ## gaze mode
        public int gazeFocusTimeMs           = 500;  // (UI) (Gaze mode) time to wait while looking at an object (EyeCamera forward) without stopping before it starts upsizing (milliseconds)
        public int gazeUpsizingTimeMs        = 1000; // (UI) (Gaze mode) time of the upsizing (milliseconds) 
        public int gazeBetweenTimeMs         = 2000; // (UI) (Gaze mode) time to wait between the upsizing and the downsizing (milliseconds) 
        public int gazeDownsizingTimeMs      = 1000; // (UI) (Gaze mode) time of the downsizing (milliseconds) 
        // ## automatic mode
        public int automaticWaitTimeMs       = 2000; // (UI) (Automatic mode) time to wait after routine start before a random object starts upsizing (milliseconds)
        public int automaticUpsizingTimeMs   = 1000; // (UI) (Automatic mode) time of the upsizing (milliseconds)
        public int automaticBetweenTimeMs    = 2000; // (UI) (Automatic mode) time to wait between the upsizing and the downsizing (milliseconds)
        public int automaticDownsizingTimeMs = 1000; // (UI) (Automatic mode) time of the downsizing (milliseconds)
        // # sizes 
        public Vector3 originalSize       = new Vector3(1, 1, 1);          // original scaling size of every object
        public Vector3 gazeFinalSize      = new Vector3(1.5f, 1.5f, 1.5f); // (UI) (Gaze mode) final size of objects after upsizing
        public Vector3 automaticFinalSize = new Vector3(2f, 2f, 2f);       // (UI) (Automatic mode) final size of objects after upsizing

        // states
        public bool objectIsResizing = false;   // indicates if an object is currently resizing
        public bool automaticResize  = false;   // gaze/automatic mode
        public int currentRoutineId  = 0;
        public int nbRoutinesBeforeAuto = 5;
        private bool objectHasBeenResizedDuringRoutine = false;

        // timers
        private Stopwatch m_gazeTimer   = new Stopwatch();
        private Stopwatch m_resizeTimer = new Stopwatch();

        // objects 
        private ObjectInfo lastObjectHit = null;                // (Gaze mode) last object to be hit 
        public List<AssetBundleComponent> objects = null;       // (UI) list of the resizable objects displayed in the scene
        public Dictionary<int, ObjectInfo> objectsInfos = null; // infos for each objects
        private SphereComponent gazeTarget = null;              // (UI) gaze target sphere

        // will look at the object parent hierarchy until it finds an AssetBundleComponent and return it if it's inside the components list, else return null
        ObjectInfo get_info_object_from_child(GameObject go) {
            do {
                var component = go.GetComponent<AssetBundleComponent>();
                if (component != null) {
                    if (objectsInfos.ContainsKey(component.key)){
                        return objectsInfos[component.key];
                    }
                    return null;
                }
                go = go.transform.parent.gameObject;

            } while (go != null);

            return null;
        }

        // upsize input object, wait, downsize it (according to the mode: automatic/gaze)
        IEnumerator resize_object(bool automaticMode, ObjectInfo info) {

            var finalSize       = automaticMode ? automaticFinalSize        : gazeFinalSize;
            var upsizingTime    = automaticMode ? automaticUpsizingTimeMs   : gazeUpsizingTimeMs;
            var betweenTime     = automaticMode ? automaticBetweenTimeMs    : gazeBetweenTimeMs;
            var downsizingTime  = automaticMode ? automaticDownsizingTimeMs : gazeDownsizingTimeMs;

            // resizing starts
            objectIsResizing = true;

            if (automaticMode) {
                // time to wait before starting resizing int automatic mode
                yield return new WaitForSeconds(automaticWaitTimeMs * 0.001f);
            }


            // store start resizing time + object name
            invoke_signal1(0);

 
            // start upsizing from original size to final size
            m_resizeTimer.Restart();
            while (m_resizeTimer.ElapsedMilliseconds < upsizingTime) {                
                info.obj.set_scale(Vector3.Lerp(originalSize, finalSize, 1f * m_resizeTimer.ElapsedMilliseconds / upsizingTime));
                yield return null; 
            }

            // wait
            yield return new WaitForSeconds(gazeBetweenTimeMs * 0.001f);

            // start downsizing from final size to original size
            m_resizeTimer.Restart();
            while (m_resizeTimer.ElapsedMilliseconds < downsizingTime) {
                info.obj.set_scale(Vector3.Lerp(finalSize, originalSize, 1f * m_resizeTimer.ElapsedMilliseconds / downsizingTime));
                yield return null;
            }

            // resizing ends
            info.hasBeenResized = true; // mark object, so it will not be resizable anymore
            objectIsResizing = false;
            objectHasBeenResizedDuringRoutine = true;
        }


        // TOUT SAUVEGARDER DANS LE LOGGER


        // après 5 routines, les objets non utilisés s'aggrandissent automatiquement (sans les pointer) un à un (diminution de la taille identique)
        // logger: ordre des objets agrandis et si ils sont agrandis automatiquement (save routine name, time, point time)
        // compteur de routines start_experiment()
        // si un objet s'est agrandis il ne peut plus le faire                

        // ### Ex component flow functions

        public override bool initialize() {

            // retrieve gaze target
            gazeTarget  = init_config().get_component<SphereComponent>("target");
            // retrieve objects
            objects = init_config().get_components_list<AssetBundleComponent>("objects");

            return true; 
        }

        public override void start_experiment() {

            // init info
            objectsInfos     = new Dictionary<int, ObjectInfo>();
            automaticResize  = false;
            currentRoutineId = 0;

            foreach (var asset in objects) {
                ObjectInfo info = new ObjectInfo();
                info.obj = asset;
                objectsInfos[asset.key] = info;
            }
        }
        public override void start_routine() {

            objectHasBeenResizedDuringRoutine = false;

            // retrieve ui values (if defined)
            var c = current_config();
            c.get_if_exists("display_gaze_target",      ref displayGazeTarget);
            c.get_if_exists("gaze_focus_time",          ref gazeFocusTimeMs);
            c.get_if_exists("gaze_upsize_time",         ref gazeUpsizingTimeMs);
            c.get_if_exists("gaze_between_time",        ref gazeBetweenTimeMs);
            c.get_if_exists("gaze_downsize_time",       ref gazeDownsizingTimeMs);
            c.get_if_exists("automatic_wait_time",      ref automaticWaitTimeMs);
            c.get_if_exists("automatic_upsize_time",    ref automaticUpsizingTimeMs);
            c.get_if_exists("automatic_between_time",   ref automaticBetweenTimeMs);
            c.get_if_exists("automatic_downsize_time",  ref automaticDownsizingTimeMs);
            c.get_if_exists("gaze_final_size",          ref gazeFinalSize);
            c.get_if_exists("automatic_final_size",     ref automaticFinalSize);
            c.get_if_exists("nb_routines_before_auto",  ref nbRoutinesBeforeAuto);            

            // after fifth routine, enable automic mode
            if (currentRoutineId == nbRoutinesBeforeAuto) {
                automaticResize = true;
                log_message("Automatic upsize mode enabled");
            }
        }


        public override void update() {

            // update ray
            Ray ray = new Ray(ExVR.VR().eyeCamera.transform.position, ExVR.VR().eyeCamera.transform.forward * 10000);

            // update target
            gazeTarget.transform.position = ExVR.VR().eyeCamera.transform.position + ExVR.VR().eyeCamera.transform.forward * 1.5f;

            // do nothing if an object is already resizing
            if (objectIsResizing) { 
                return;
            }

            // an object has already been resized during this routine
            if (objectHasBeenResizedDuringRoutine) {
                return;
            }

            // manual resizing by looking at objects
            if (!automaticResize) { 

                RaycastHit hit;
                if (Physics.Raycast(ray, out hit)) { // an object has been hit

                    // retrieve object info
                    var objectInfo = get_info_object_from_child(hit.collider.gameObject);
                    if (objectInfo == null) { // not object from the list found
                        return;
                    }

                    // object has already been resized
                    if (objectInfo.hasBeenResized) {
                        return;
                    }

                    if (lastObjectHit == null) { // no object has been hit during previous frame

                        lastObjectHit = objectInfo;
                        m_gazeTimer.Start();
                        log_message("new hit " + lastObjectHit.obj.name);

                    } else { // an object has been hit during previous frame

                        if (lastObjectHit.obj.key != objectInfo.obj.key) { // the previous object was different
                            lastObjectHit = objectInfo;
                            m_gazeTimer.Restart();
                            log_message("new hit " + lastObjectHit.obj.name);

                        } else { // the previous object was identic

                            if (m_gazeTimer.ElapsedMilliseconds > gazeFocusTimeMs) { // check if focus gaze time is enough
                                log_message("START UPSIZING!");
                                m_gazeTimer.Reset();

                                // start resizing coroutine
                                IEnumerator coroutine = resize_object(false, lastObjectHit);
                                StartCoroutine(coroutine);
                            }
                        }
                    }

                } else {

                    if (lastObjectHit != null) {

                        log_message("lose hit " + lastObjectHit.obj.name);
                        // stop timer
                        m_gazeTimer.Reset();
                    }

                    lastObjectHit = null;
                }
            } else { // automatic resize

                // retrieve objects not resized
                List<ObjectInfo> objectsToResize = new List<ObjectInfo>();
                foreach (var obj in objectsInfos) {
                    if (!obj.Value.hasBeenResized) {
                        objectsToResize.Add(obj.Value);
                    }
                }

                // no objects to resize remaining
                if (objectsToResize.Count == 0) {                    
                    return;
                }

                // select one object randomely
                System.Random rnd = new System.Random();
                int id = rnd.Next(0, objectsToResize.Count - 1);

                // start resizing coroutine
                IEnumerator coroutine = resize_object(true, objectsToResize[id]);
                StartCoroutine(coroutine);
            }
        }

        public override void stop_routine() {
            ++currentRoutineId;
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
        static public long ellapsed_time_exp_ms();
        static public long ellapsed_time_routine_ms();
		
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
