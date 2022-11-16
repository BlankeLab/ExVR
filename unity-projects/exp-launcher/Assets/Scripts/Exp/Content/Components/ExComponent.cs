
/***********************************************************************************
** exvr-exp                                                                       **
** MIT License                                                                    **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                                **
** Permission is hereby granted, free of charge, to any person obtaining a copy   **
** of this software and associated documentation files (the "Software"), to deal  **
** in the Software without restriction, including without limitation the rights   **
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      **
** copies of the Software, and to permit persons to whom the Software is          **
** furnished to do so, subject to the following conditions:                       **
**                                                                                **
** The above copyright notice and this permission notice shall be included in all **
** copies or substantial portions of the Software.                                **
**                                                                                **
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     **
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       **
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    **
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         **
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  **
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  **
** SOFTWARE.                                                                      **
************************************************************************************/

// system
using System;
using System.Text;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

// unity
using UnityEngine;
using UnityEngine.UI;

namespace Ex {

    public class ExComponent : MonoBehaviour {

        public enum Function{
            initialize,
            start_experiment,
            post_start_experiment,
            set_current_config,
            pre_start_routine,
            start_routine,
            post_start_routine,
            set_update_state,
            set_visibility,
            update_parameter_from_gui,
            update_from_current_config,
            action_from_gui,
            pre_update,
            update,
            post_update,
            global_update,
            play,
            pause,
            stop_routine,
            pre_stop_experiment,
            stop_experiment,
            clean,
            slot1,
            slot2,
            slot3,
            slot4,
            slot5,
            on_gui,
            end_of_frame,
            undefined,
            not_defined_in_component
        }

        public enum Category {
            AcquisitionDevice,
            Audio,
            Avatar,
            Camera,
            Environment,
            Flow,
            InputDevice,
            Interaction,                        
            Model,
            Network,
            Output,
            Resource,
            Script,
            Scene,
            Settings,
            Text,
            UserInterface,            
            Video,
            Viewer,
            Undefined
        };

        public enum Priority { Low, Medium, Hight};
        public enum Reserved {Public, Closed, LNCO};

        public int key = -1; // id component
        public string keyStr;
        public string typeStr;
        public Category category = Category.Undefined;
        public Priority priority = Priority.Medium;

        [SerializeField]
        private Reserved m_reserved = Reserved.Public;

        public Function currentFunction = Function.undefined;
        public Dictionary<Function, bool> functionsDefined = null;       

        public Routine currentRoutine = null;
        public Condition currentCondition = null;
        public TimeLine currentTimeline = null;

        [SerializeField]
        protected Events.Connections m_connections = null;

        [SerializeField]
        protected bool m_visibility = false; // is visible ?
        [SerializeField]
        protected bool m_updating = false;   // call update function ?
        [SerializeField]
        protected bool m_alwaysCallUpdate = false; // call update even if not in timeline
        [SerializeField]
        protected bool m_global = false;           // call update even if not in condition
        [SerializeField]
        private bool m_started = false;      // has associated routine started ?
        [SerializeField]
        private bool m_closed = false;       // is closed ? (cannot be enabled anymore until next routine)
        [SerializeField]
        private bool m_initialized = false;  // has initialization failed ? 
        [SerializeField]
        protected bool m_catchExceptions = false;
        [SerializeField]
        protected bool m_frameLogging = false;
        [SerializeField]
        protected bool m_triggerLogging = false;
        private int m_layoutEventsNbCallsForCurrentFrame = 0;

        // access        
        public Components components() { return ExVR.Components(); }
        public Log log() { return ExVR.Log(); }
        public TimeManager time() { return ExVR.Time(); }
        public EventsManager events() { return ExVR.Events(); }
        public Events.Connections connections() { return m_connections; }
        public Events.Command command() { return events().command; }
        public bool is_function_defined(Function function) {return functionsDefined[function];}

        // connections
        protected Events.Signal add_signal(string name){return m_connections.add_signal(name);}
        protected Events.Slot add_slot(string name, System.Action<object> action) {return m_connections.add_slot(name, action);}
        public bool is_signal_connected(string name) {return m_connections.is_signal_connected(name);}
        public void invoke_signal(string name, object arg = null) {m_connections.invoke_signal(name, arg);}

        // configs
        public ComponentInitConfig initC = null;
        public ComponentConfig currentC = null; // current config
        public List<ComponentConfig> configs = null;
        public Dictionary<int, ComponentConfig> configsPerKey = null;
        public Dictionary<string, ComponentConfig> configsPerName = null;

        // states
        protected bool is_initialized() { return m_initialized; }
        public bool is_started() {return m_started;}
        public bool is_visible() {return m_visibility;}
        public bool is_updating() { return m_updating; }
        public bool is_global() { return m_global; }
        public bool has_frame_logging() { return m_frameLogging; }
        public bool has_trigger_logging() { return m_triggerLogging; }
        public bool always_call_update() { return m_alwaysCallUpdate; }
        public bool is_closed() {return m_closed;}
        public void set_closed_flag(bool closed) { m_closed = closed; }
        public void close() { components().close(this); }

        public string component_name() {return name;}

        public string verbose_name() {
            
            StringBuilder builder = new StringBuilder();
            builder.AppendFormat(" [From component: [{0}]({1})([{2}])", name, this.GetType().ToString(), currentFunction.ToString());
            if(currentRoutine != null) {
                builder.AppendFormat(", from routine: [{0}] with condition: [{1}]", currentRoutine.name, currentCondition.name);
            }
            builder.AppendFormat(" at time {0}ms ({1}ms)]", time().ellapsed_element_ms().ToString(), time().ellapsed_exp_ms().ToString());
            return builder.ToString();
        }

        protected void display_exception(Exception e) { 
            log_error(string.Format("[ERROR-EXCEPTION] "), true, false);
            log_error(string.Format("[MESSAGE] {0}", e.Message), false, false);
            log_warning(string.Format("\t[SOURCE] {0}", e.Source), false, false);
            if (e.TargetSite != null) {
                log_warning(string.Format("\t[TARGET] {0}", e.TargetSite.ToString()), false, false);
            } else {
                log_warning("\t[TARGET] None");
            }
            log_warning(string.Format("\t[STACK]: {0}", e.StackTrace), false, false);
        }

        protected void send_infos_to_gui_init_config(string id, string infos) {
            ExVR.Network().gui_ipc().send_component_infos_to_GUI(keyStr, initC.keyStr, id, infos);
        }
        protected void send_infos_to_gui_current_config(string id, string infos) {
            ExVR.Network().gui_ipc().send_component_infos_to_GUI(keyStr, currentC.keyStr, id, infos);
        }

        public void add_message_to_stack_trace(string message) {
            ExVR.Events().stacktrace.ComponentTrace.Invoke(this, message);
        }


        // logs
        public void log_message(object message, bool verbose = false, bool extra = false,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0, bool logger = true, bool append = true) {

            log().message(
                message         : verbose ? string.Concat(Converter.to_string(message), verbose_name()) : message,
                addExtraInfo    : extra,
                memberName      : memberName,
                sourceFilePath  : sourceFilePath,
                sourceLineNumber: sourceLineNumber,
                logger          : logger,
                append          : append
            );
        }

        public void log_warning(object warning, bool verbose = true, bool extra = false,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0, bool logger = true, bool append = false) {

            log().warning(
                warning          : verbose ? string.Concat(Converter.to_string(warning), verbose_name()) : warning,
                addExtraInfo     : extra,
                memberName       : memberName,
                sourceFilePath   : sourceFilePath,
                sourceLineNumber : sourceLineNumber,
                logger           : logger,
                append           : append
            );
        }

        public void log_error(object error, bool verbose = true, bool extra = true,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0, bool logger = true, bool append = false) {

            log().error(
                error            : verbose ? string.Concat(Converter.to_string(error), verbose_name()) : error,
                addExtraInfo     : extra, 
                memberName       : memberName,
                sourceFilePath   : sourceFilePath,
                sourceLineNumber : sourceLineNumber,
                logger           : logger,
                append           : append
            );
        }

        // configs
        public ComponentInitConfig init_config() { return initC; }

        public bool has_current_config() {
            return currentC != null;
        }

        public ComponentConfig current_config() {
            //if (!has_current_config()) {
            //    log_error("No current config set, check if component has been added to this condition.");
            //}
            return currentC;
        }
        public ComponentConfig get_config(string configName, bool displayError = true) {
            if (configsPerName.ContainsKey(configName)) {
                return configsPerName[configName];
            }
            if (displayError) {
                log_error(string.Format("No config with name [{0}] available for this component.", configName));
            }
            return null;
        }

        public ComponentConfig get_config(int configKey, bool displayError = true) {
            if (configsPerKey.ContainsKey(configKey)) {
                return configsPerKey[configKey];
            }
            if (displayError) {
                log_error(string.Format("No config with key [{0}] available for this component.", configKey));
            }
            return null;
        }

        public void update_current_config(string configName) {
            var config = get_config(configName);
            if(config != null) {
                currentC = config;
            }
        }

        public void update_current_config(int configKey) {
            var config = get_config(configKey);
            if (config != null) {
                currentC = config;
            }
        }

        public void update_current_config(ComponentConfig config) {
            currentC = config;
        }

        // routine
        public bool has_current_routine() {
            return currentRoutine != null;
        }
        public Routine current_routine() {
            if (!has_current_routine()) {
                log_error("No current routine set, check if component has been added to this condition.");
            }
            return currentRoutine;
        }

        // condition
        public bool has_current_condition() {
            return currentCondition != null;
        }
        public Condition current_condition() {
            if (!has_current_condition()) {
                log_error("No current condition set, check if component has been added to this condition.");
            }
            return currentCondition;
        }

        // setup component, parent, layer, configurations...
        public bool setup_component_object(XML.Component xmlComponent) {

            // set members
            gameObject.name     = xmlComponent.Name;
            key                 = xmlComponent.Key;
            keyStr              = Converter.to_string(xmlComponent.Key);
            m_global            = xmlComponent.Global;
            priority            = (Priority)xmlComponent.Priority;
            tag                 = string.Format("{0}Component", xmlComponent.Category);
            typeStr             = string.Format("Ex.{0}Component", xmlComponent.Type);
            category            = (Category)Enum.Parse(typeof(Category), xmlComponent.Category);
            m_reserved          = (Reserved)xmlComponent.Restricted;
            m_catchExceptions     = ExVR.GuiSettings().catchComponentsExceptions || xmlComponent.Exceptions;
            m_alwaysCallUpdate  = xmlComponent.AlwaysUpdating;
            m_frameLogging      = xmlComponent.FrameLogging;
            m_triggerLogging    = xmlComponent.TriggerLogging;

            bool valid = false;
            if (m_reserved == Reserved.Public) {
                valid = true;
            } else if (m_reserved == Reserved.Closed) {
#if CLOSED_COMPONENTS
                valid = true;
#endif
            } else if (m_reserved == Reserved.LNCO) {
#if LNCO_COMPONENTS
                valid = true;
#endif
            }
            if (!valid) {
                log_error("Component is restricted and not available for this ExVR build.");
                return false;
            }

            // set parent
            if(Components.Category2Transform.ContainsKey(category)){
                var name = Components.Category2Transform[category];
                var categoryTr = Ex.ExVR.Components().transform.Find(name);
                if (categoryTr == null) {
                    GameObject categoryGO = new GameObject(name);
                    categoryGO.transform.SetParent(Ex.ExVR.Components().transform);
                    categoryTr = categoryGO.transform;
                }
                transform.SetParent(categoryTr);

            } else {
                log_error(string.Format("GameObject type {0} is not available in components hierarchy.", category.ToString()));
            }

            var parentConfigs = new GameObject("Configs");            
            parentConfigs.transform.SetParent(transform);

            // init config
            var initConfigGo = new GameObject();
            initConfigGo.transform.SetParent(parentConfigs.transform);
            initC = initConfigGo.AddComponent<ComponentInitConfig>();
            initC.initialize(xmlComponent.InitConfig);           

            // conditions config
            configs = new List<ComponentConfig>(xmlComponent.Configs.Config.Count);
            configsPerKey = new Dictionary<int, ComponentConfig>(xmlComponent.Configs.Config.Count);
            configsPerName = new Dictionary<string, ComponentConfig>(xmlComponent.Configs.Config.Count); 
            foreach (XML.Config xmlConfig in xmlComponent.Configs.Config) {

                var configGo = new GameObject();
                configGo.transform.SetParent(parentConfigs.transform);

                var componentConfig = configGo.AddComponent<ComponentConfig>();
                componentConfig.initialize(xmlConfig);
                configs.Add(componentConfig);

                configsPerKey[componentConfig.key] = componentConfig;
                configsPerName[componentConfig.name] = componentConfig;
            }

            if(configs.Count == 0) {
                log_error(string.Format("No config found from component with name {0} and type {1}.", name, typeStr));
                return false;
            }

            // init current config with the first one
            currentC = configs[0];

            // look for defined functions in child class
            Type derivedType = this.GetType();
            functionsDefined = new Dictionary<Function, bool>();
            // # private
            var flagPrivate = System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.NonPublic;
            var flagPublic = System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public;

            functionsDefined[Function.initialize] = (derivedType.GetMethod("initialize", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.clean] = (derivedType.GetMethod("clean", flagPrivate).DeclaringType == derivedType);

            functionsDefined[Function.start_experiment] = (derivedType.GetMethod("start_experiment", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.post_start_experiment] = (derivedType.GetMethod("post_start_experiment", flagPrivate).DeclaringType == derivedType);            
            functionsDefined[Function.pre_stop_experiment] = (derivedType.GetMethod("pre_stop_experiment", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.stop_experiment] = (derivedType.GetMethod("stop_experiment", flagPrivate).DeclaringType == derivedType);

            functionsDefined[Function.set_current_config] = (derivedType.GetMethod("set_current_config", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.update_from_current_config] = (derivedType.GetMethod("update_from_current_config", flagPublic).DeclaringType == derivedType);

            functionsDefined[Function.pre_start_routine] = (derivedType.GetMethod("pre_start_routine", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.start_routine] = (derivedType.GetMethod("start_routine", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.post_start_routine] = (derivedType.GetMethod("post_start_routine", flagPrivate).DeclaringType == derivedType);
            
            functionsDefined[Function.pre_update] = (derivedType.GetMethod("pre_update", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.update] = (derivedType.GetMethod("update", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.post_update] = (derivedType.GetMethod("post_update", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.global_update] = (derivedType.GetMethod("global_update", flagPublic).DeclaringType == derivedType);
            
            functionsDefined[Function.on_gui] = (derivedType.GetMethod("on_gui", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.end_of_frame] = (derivedType.GetMethod("end_of_frame", flagPrivate).DeclaringType == derivedType);

            functionsDefined[Function.stop_routine] = (derivedType.GetMethod("stop_routine", flagPrivate).DeclaringType == derivedType);

            functionsDefined[Function.set_update_state] = (derivedType.GetMethod("set_update_state", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.set_visibility] = (derivedType.GetMethod("set_visibility", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.play] = (derivedType.GetMethod("play", flagPublic).DeclaringType == derivedType);
            functionsDefined[Function.pause] = (derivedType.GetMethod("pause", flagPublic).DeclaringType == derivedType);

            functionsDefined[Function.update_parameter_from_gui] = (derivedType.GetMethod("update_parameter_from_gui", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.action_from_gui] = (derivedType.GetMethod("action_from_gui", flagPrivate).DeclaringType == derivedType);
            
            // init events
            m_connections = new Events.Connections(name);

            return true;
        }



        // Initialize the component, called only once at the beginning of the experiment, will call child initialize
        public virtual bool base_initialize() {            

            currentFunction = Function.initialize;
            m_initialized = false;
            if (m_catchExceptions) {
                try {
                    m_initialized = initialize();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                m_initialized = initialize();  
            }

            if (!m_initialized) {
                log_error("Initialization failed.");
                base_clean(true);
            }

            return m_initialized;
        }

        public void base_clean(bool forceClean = false) {

            if (!forceClean) {
                if (!m_initialized) {
                    return;
                }
            }

            currentFunction = Function.clean;
            if (m_catchExceptions) {
                try {
                    clean();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                clean();
            }
        }


        // This function is called only once at the beggining of an experiment 
        public void base_start_experiment() {
            
            currentFunction = Function.start_experiment;
            if (m_catchExceptions) {
                try {
                    start_experiment();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                start_experiment();
            }      
        }
        public void base_post_start_experiment() {

            currentFunction = Function.post_start_experiment;
            if (m_catchExceptions) {
                try {
                    post_start_experiment();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                post_start_experiment();
            }
        }


        

        // This function is called only once at the end of an experiment 
        public void base_pre_stop_experiment() {

            currentFunction = Function.pre_stop_experiment;
            if (m_catchExceptions) {
                try {
                    pre_stop_experiment();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                pre_stop_experiment();
            }
        }
        public void base_stop_experiment() {
    
            currentFunction = Function.stop_experiment;
            if (m_catchExceptions) {
                try {
                    stop_experiment();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                stop_experiment();
            }
        }

        public void base_set_current_config(Condition condition, ComponentConfig config, TimeLine timeline) {

            // update infos
            currentCondition = condition;
            currentRoutine = currentCondition.parent_routine();
            currentTimeline = timeline;
            currentC = config;

            // update layer if necessary
            int layer = Layers.Default;
            if (currentC.has("eye_to_render")) {
                string renderEyes = currentC.get<string>("eye_to_render");
                if (renderEyes == "Left eye") {
                    layer = Layers.LeftEye;
                } else if (renderEyes == "Right eye") {
                    layer = Layers.RightEye;
                }
            }
            gameObject.layer = layer;
            foreach (Transform tr in gameObject.GetComponentInChildren<Transform>(true)) {
                tr.gameObject.layer = layer;
            }

            // call virtual function
            currentFunction = Function.set_current_config;
            if (m_catchExceptions) {
                try {
                    set_current_config(config.name);
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                set_current_config(config.name);
            }
        }

        public void base_update_from_current_config() {

            currentFunction = Function.update_from_current_config;
            if (m_catchExceptions) {
                try {
                    update_from_current_config();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                update_from_current_config();
            }
        }

        public void base_pre_start_routine() {

            m_started = true;
            currentFunction = Function.pre_start_routine;
            if (m_catchExceptions) {
                try {
                    pre_start_routine();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                pre_start_routine();
            }
        }

        public void base_start_routine() {

            currentFunction = Function.start_routine;
            if (m_catchExceptions) {
                try {
                    start_routine();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                start_routine();
            }           
        }

        public void base_post_start_routine() {

            currentFunction = Function.post_start_routine;
            if (m_catchExceptions) {
                try {
                    post_start_routine();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                post_start_routine();
            }
        }

        public void base_stop_routine() {

            m_started = false;
            currentFunction = Function.stop_routine;
            if (m_catchExceptions) {
                try {
                    stop_routine();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                stop_routine();
            }

            // disable the closing state
            set_closed_flag(false);
        }

        // Called at every frame if component inside timeline, will call child pre_update
        public void base_pre_update() {

            currentFunction = Function.pre_update;
            if (m_catchExceptions) {
                try {
                    pre_update();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                pre_update();
            }
        }

        // Called at every frame if component inside timeline, will call child update
        public void base_update() {
 
            currentFunction = Function.update;
            if (m_catchExceptions) {
                try {
                    update();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                update();
            }

        }

        // Called at every frame if component inside timeline, will call child post_update
        public void base_post_update() {

            currentFunction = Function.post_update;
            if (m_catchExceptions) {
                try {
                    post_update();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                post_update();
            }
        }


        public void base_global_update() {

            currentFunction = Function.global_update;
            if (m_catchExceptions) {
                try {
                    global_update();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                global_update();
            }
        }


        private void process_event(Event cEvent) {

            var eType = cEvent.type;

            //  detect first event
            if (eType == EventType.Layout) {
                // A layout event.
                // (This event is sent prior to anything else - this is a chance to perform any initialization. It is used by the automatic layout system
                ++m_layoutEventsNbCallsForCurrentFrame;
                if (m_layoutEventsNbCallsForCurrentFrame == 1) {
                    first_event(!ExVR.Time().is_current_frames_started());
                    return;
                }
            }

            // process others events                        
            switch (eType) {
                // mouse events
                case EventType.MouseDown:
                    // Mouse button was pressed.
                    // (This event gets sent when any mouse button is pressed. Use Event.button to determine which button was pressed down.)
                    mouse_event(cEvent);
                    return;
                case EventType.MouseUp:
                    // Mouse button was released.
                    // (This event gets sent when any mouse button is released. Use Event.button to determine which button was pressed down.)
                    mouse_event(cEvent);
                    return;
                case EventType.MouseMove:
                    // Mouse was moved (Editor views only).
                    // (The mouse was moved without any buttons being held down. Use Event.mousePosition and Event.delta to determine mouse motion.)
                    // (Note that this event is only sent in the Editor for EditorWindow windows which have EditorWindow.wantsMouseMove set to true. Mouse move events are never sent in the games.)
                    mouse_event(cEvent);
                    return;
                case EventType.MouseDrag: // editor view
                    // Mouse was dragged.
                    // (The mouse was moved with a button held down - a mouse drag. Use Event.mousePosition and Event.delta to determine mouse motion.)
                    mouse_event(cEvent);
                    return;
                case EventType.ScrollWheel:
                    // The scroll wheel was moved.
                    // (Use Event.delta to determine X & Y scrolling amount.)
                    mouse_event(cEvent);
                    return;
                // keyboard events
                case EventType.KeyDown:
                    // A keyboard key was pressed
                    // (Use Event.character to find out what has been typed. Use Event.keyCode to handle arrow, home/end or other function keys, or to find out which physical key has been pressed.
                    // This event is sent repeatedly depending on the end user's keyboard repeat settings.)
                    // (Note that key presses can come as separate events, one with valid Event.keyCode, and another with valid Event.character. In case of keyboard layouts with dead keys,
                    // multiple Event.keyCode events can generate a single Event.character event)
                    keyboard_event(cEvent);
                    return;
                case EventType.KeyUp:
                    // A keyboard key was released.
                    // (Use Event.keyCode to find which physical key was released. Note that depending on the system and keyboard layout, Event.character might not contain any character for a key release event.)
                    keyboard_event(cEvent);
                    return;
                case EventType.Repaint:
                    // A repaint event. One is sent every frame. (All other events are processed first, then the repaint event is sent.)
                    last_event();
                    return;
            }
        }

        // Called several times per frame if component inside timeline
        public void base_on_gui() {

            currentFunction = Function.on_gui;
            var cEvent = Event.current;
            if (m_catchExceptions) {
                try {
                    process_event(cEvent);
                    on_gui();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                process_event(cEvent);
                on_gui();
            }
        }

        // Called at every frame if component inside timeline, will call child enbd_of_frame
        public void base_end_of_frame() {
            currentFunction = Function.end_of_frame;
            if (m_catchExceptions) {
                try {
                    end_of_frame();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                end_of_frame();
            }

            m_layoutEventsNbCallsForCurrentFrame = 0;
        }


        // Play command from the GUI, will resumed if on pause, will call child play
        public void base_play() {

            currentFunction = Function.play;
            if (m_catchExceptions) {
                try {
                    play();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                play();
            }
        }

        // Pause command from the GUI, will pause if on play, will call child pause
        public void base_pause() {

            currentFunction = Function.pause;
            if (m_catchExceptions) {
                try {
                    pause();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                pause();
            }
        }

        public void base_set_visibility(bool visibility) {

            m_visibility = visibility;
            currentFunction = Function.set_visibility;

            if (m_catchExceptions) {
                try {
                    set_visibility(m_visibility);
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                set_visibility(m_visibility);
            }
        }

        public void base_set_update_state(bool doUpdate) {

            m_updating = doUpdate;

            currentFunction = Function.set_update_state;
            if (m_catchExceptions) {
                try {
                    set_update_state(m_updating);
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                set_update_state(m_updating);
            }
        }

        // Update the parameters from GUI
        public void base_update_parameter_from_gui(string updatedArgName) {
            
            currentFunction = Function.update_parameter_from_gui;
            if (m_catchExceptions) {
                try {
                    update_parameter_from_gui(updatedArgName);
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                update_parameter_from_gui(updatedArgName);
            }                
        }

        public void base_action_from_gui(int configKey, string action) {

            currentFunction = Function.action_from_gui;
            if (initC.key == configKey) {

                if (m_catchExceptions) {
                    try {
                        action_from_gui(true, action);
                    } catch (Exception e) {
                        display_exception(e);
                    }
                } else {
                    action_from_gui(true, action);
                }

            } else {
                if (currentC != null) {
                    if (currentC.key == configKey) {

                        if (m_catchExceptions) {
                            try {
                                action_from_gui(false, action);
                            } catch (Exception e) {
                                display_exception(e);
                            }
                        } else {
                            action_from_gui(false, action);
                        }
                    }
                }
            }
        }


        // once per loading
        protected virtual bool initialize() {return true; }
        protected virtual void clean() { }

        // once per experiment
        protected virtual void start_experiment() { }
        protected virtual void post_start_experiment() { }
        protected virtual void pre_stop_experiment() { }
        protected virtual void stop_experiment() { }

        // once per routine
        protected virtual void set_current_config(string configName) {}
        public virtual void update_from_current_config() { }
        protected virtual void pre_start_routine() {}
        protected virtual void start_routine() {}        
        protected virtual void post_start_routine() {}
        protected virtual void stop_routine() {}

        // several times per routine
        protected virtual void set_visibility(bool visible) { }
        protected virtual void set_update_state(bool doUpdate) { }
        public virtual void play() { }
        public virtual void pause() { }


        // every frame or more        
        protected virtual void pre_update() {}
        protected virtual void update() {}
        protected virtual void post_update() {}
        public virtual void global_update() { }
        protected virtual void on_gui() { }
        protected virtual void end_of_frame() { }

        // events
        protected virtual void first_event(bool isCalledBeforeUpdate) {} // can be called before or after pre_update/update/post_update
        protected virtual void keyboard_event(Event kEvent) { } // called before pre_update/update/post_update
        protected virtual void mouse_event(Event mEvent) { } // called before pre_update/update/post_update
        protected virtual void last_event() { } // called after pre_update/udpate/post_update


        // from gui
        protected virtual void update_parameter_from_gui(string updatedArgName) {}
        protected virtual void action_from_gui(bool initConfig, string action) {}


        // logging
        public virtual string format_frame_data_for_global_logger(bool header) { return null; }
        public virtual List<Tuple<double, double, string>> format_trigger_data_for_global_logger() { 
            // exp time / routine time / data
            return null; 
        }


        // transform related function
        public virtual void set_position(Vector3 position) {
            transform.position = position;
        }
        public virtual void set_rotation(Vector3 rotation) {
            transform.eulerAngles = rotation;
        }
        public virtual void set_rotation(Quaternion rotation) {
            transform.rotation = rotation;
        }
        public virtual void set_scale(Vector3 scale) {
            transform.localScale = scale;
        }

        public virtual Vector3 forward() {
            return transform.forward;
        }

        public virtual Vector3 position() {
            return transform.position;
        }
        public virtual Quaternion rotation() {
            return transform.rotation;
        }

        public virtual Vector3 euler_angles() {
            return transform.eulerAngles;
        }

        public virtual Vector3 scale() {
            return transform.localScale;
        }

        public virtual void reset_init_transform() {
            if (!initC.get<bool>("init_transform_do_not_apply")) {
                initC.update_transform("init_transform", transform, true);
            }
        }
        public virtual void reset_config_transform() {
            if (!currentC.get<bool>("transform_do_not_apply")) {
                currentC.update_transform("transform", transform, true);
            }
        }


        // compatibility
        [System.Obsolete("Use ExVR.Components().get_from_name instead.")]
        public static ExComponent get(string componentName) {
            return ExVR.Components().get_from_name(componentName);
        }
        [System.Obsolete("Do not use anymore.")]
        public static void next_element_with_name(string componentname) {
            ExVR.Events().command.next_element_with_name(componentname);
        }


        //  UNTESTED
        public static void set_component_config<T>(string routineName, string conditionName, string componentName, string configToUse) where T : ExComponent {

            // parse everything just in case there is routines/conditions/components/configs with the same names

            var routine = ExVR.Routines().get(routineName);
            if (routine == null) {
                return;
            }

            var condition = routine.get_condition_from_name(conditionName);
            if (condition == null) {
                return;
            }

            var action = condition.get_action_from_component_name(componentName);
            if (action == null) {
                return;
            }

            var config = action.component().configsPerName[configToUse];
            if (config != null) {
                // update config of the action
                action.set_config(config);
            }
        }
    }

    public class CanvasWorldSpaceComponent : ExComponent{

        public override bool base_initialize() {

            var canvas = gameObject.AddComponent<Canvas>();
            gameObject.AddComponent<CanvasScaler>();
            gameObject.AddComponent<GraphicRaycaster>();
            canvas.renderMode = RenderMode.WorldSpace;

            return base.base_initialize();
        }
    }

    //public class 
}