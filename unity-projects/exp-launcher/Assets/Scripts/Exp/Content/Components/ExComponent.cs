
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
            play,
            pause,
            stop_routine,
            stop_experiment,
            clean,
            slot1,
            slot2,
            slot3,
            slot4,
            slot5,
            on_gui,
            undefined,
            not_defined_in_component
        }

        public enum Category {
            Audio,
            Avatar,
            Camera,
            Environment,
            Flow,
            Input,
            Interaction,                        
            Model,
            Network,
            Output,
            Resource,
            Script,
            Scene,
            Settings,
            Text,
            UI,
            Tracking,
            Video,
            Cloud,
            Viewer,
            Undefined
        };

        public enum Pritority { Low, Medium, Hight};
        public enum Reserved {Public, Closed, LNCO};

        public int key = -1; // id component
        public string keyStr;
        public string typeStr;
        public Category category = Category.Undefined;
        public Pritority priority = Pritority.Medium;
        public Function currentFunction = Function.undefined;
        public Dictionary<Function, bool> functionsDefined = null;
        
        public Routine currentRoutine = null;
        public Condition currentCondition = null;
        public TimeLine currentTimeline = null;                
        protected Events.Connections m_connections = null;

        protected bool m_visibility = false; // is visible ?
        protected bool m_updating = false;   // call update function ?
        private bool m_started = false;      // has associated routine started ?
        private bool m_closed = false;       // is closed ? (cannot be enabled anymore until next routine)
        private bool m_initialized = false;  // has initialization failed ? 
        protected bool catchExceptions = false;

        // access        
        public Components components() { return ExVR.Components(); }
        public Log log() { return ExVR.Log(); }
        public TimeManager time() { return ExVR.Time(); }
        public EventsManager events() { return ExVR.Events(); }
        public Events.Connections connections() { return m_connections; }
        public Events.Command command() { return events().command; }

        public bool is_function_defined(Function function) {return functionsDefined[function];}

        // connections
        protected void add_signal(string name){m_connections.add_signal(name);}
        protected void add_slot(string name, System.Action<object> action) {m_connections.add_slot(name, action);}
        public void invoke_signal(string name, object arg = null) {m_connections.invoke_signal(name, arg);}

        // configs
        public ComponentInitConfig initC = null;
        public ComponentConfig currentC = null; // current config
        public List<ComponentConfig> configs = null;
        public Dictionary<int, ComponentConfig> configsPerKey = null;
        public Dictionary<string, ComponentConfig> configsPerName = null;

        // states
        public bool is_started() {return m_started;}
        public bool is_visible() {return m_visibility;}
        public bool is_updating() {return m_updating;}
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

            //var stack = new System.Diagnostics.StackTrace(e, true);
            //System.Diagnostics.StackFrame frame = stack.GetFrame(0);

            //string className = "Unknow";
            //string functionName = "Unknow";

            //int lineNb = 0;
            //int columnNb = 0;
            //string fileName = null;
            //if (frame != null) {

            //    var method = frame.GetMethod();
            //    if (method != null) {
            //        className = method.ReflectedType.Name;
            //        functionName = method.ToString();
            //    }

            //    lineNb   = frame.GetFileLineNumber();
            //    columnNb = frame.GetFileColumnNumber();
            //    fileName = frame.GetFileName();
            //}

            //var builder = new StringBuilder();
            //builder.Append("[EX_COMPONENT ERROR]");
            //if(fileName != null) {
            //    builder.AppendFormat("  [LOCATION] from class [{0}] in function [{1}] in file [{2}] at line ({3}) and column ({4})\n",
            //        className, functionName, fileName, lineNb.ToString(), columnNb.ToString());
            //} else {
            //    builder.AppendFormat("  [LOCATION] from class [{0}] in function [{1}]\n",
            //        className, functionName);
            //}
            //builder.AppendFormat("[EX_COMPONENT EXCEPTION] {0} ", e.Message);
            //log_error(builder.ToString(), true);
            log_error(string.Format("[EX_COMPONENT EXCEPTION] {0} ", e.Message), true);
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
            [CallerLineNumber] int sourceLineNumber = 0) {
            if (verbose) {
                log().message(string.Concat(Converter.to_string(message), verbose_name()), extra, memberName, sourceFilePath, sourceLineNumber);
            } else {
                log().message(message, extra, memberName, sourceFilePath, sourceLineNumber);
            }            
        }

        public void log_warning(object warning, bool verbose = true, bool extra = false,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0) {
            if (verbose) {
                log().warning(string.Concat(Converter.to_string(warning), verbose_name()), extra, memberName, sourceFilePath, sourceLineNumber);
            } else {
                log().warning(warning, extra, memberName, sourceFilePath, sourceLineNumber);
            }
        }

        public void log_error(object error, bool verbose = true, bool extra = true,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0) {
            if (verbose) {
                log().error(string.Concat(Converter.to_string(error), verbose_name()), extra, memberName, sourceFilePath, sourceLineNumber);
            } else {
                log().error(error, extra, memberName, sourceFilePath, sourceLineNumber);
            }
        }

        // configs
        public ComponentInitConfig init_config() { return initC; }

        public ComponentConfig current_config() {
            if (currentC == null) {
                log_error("No current config set, check if component has been added to this condition.");
            }
            return currentC;
        }
        public ComponentConfig get_config(string configName) {
            if (configsPerName.ContainsKey(configName)) {
                return configsPerName[configName];
            }
            return null;
        }

        public ComponentConfig get_config(int configKey) {
            if (configsPerKey.ContainsKey(configKey)) {
                return configsPerKey[configKey];
            }
            return null;
        }


        // ######### UNTESTED
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
            if(action == null) {
                return;
            }

            var config = action.component().configsPerName[configToUse];
            if(config != null){
                // update config of the action
                action.set_config(config);
            }             
        }

        // setup component, parent, layer, configurations...
        public bool setup_component_object(XML.Component xmlComponent) {

            // set members
            gameObject.name = xmlComponent.Name;
            key             = xmlComponent.Key;
            keyStr = Converter.to_string(xmlComponent.Key);
            catchExceptions = ExVR.GuiSettings().catchComponentsExceptions;

            typeStr = string.Format("Ex.{0}Component", xmlComponent.Type);
            if (Components.Names2Info.ContainsKey(typeStr)) {
                var infos = Components.Names2Info[typeStr];

                bool valid = false;
                if (infos.reserved == Reserved.Public) {
                    valid = true;
                }else if (infos.reserved == Reserved.Closed) {
#if CLOSED_COMPONENTS
                    valid = true;
#endif
                }else if (infos.reserved == Reserved.LNCO) {
#if LNCO_COMPONENTS
                    valid = true;
#endif
                }
                if (!valid) {
                    log_error("Component is restricted and not available for this ExVR build.");
                    return false;
                }
                
                category = infos.category;
                priority = infos.priority;
            } else {
                category = Category.Scene;
                log_error("Component doesn't belong to a category.");
            }
            tag = string.Format("{0}Component", category.ToString());

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

            // look for defined functions in child class
            Type derivedType = this.GetType();
            functionsDefined = new Dictionary<Function, bool>();
            // # private
            var flagPrivate = System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.NonPublic;
            var flagPublic = System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.Public;

            functionsDefined[Function.initialize] = (derivedType.GetMethod("initialize", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.clean] = (derivedType.GetMethod("clean", flagPrivate).DeclaringType == derivedType);

            functionsDefined[Function.start_experiment] = (derivedType.GetMethod("start_experiment", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.stop_experiment] = (derivedType.GetMethod("stop_experiment", flagPrivate).DeclaringType == derivedType);

            functionsDefined[Function.set_current_config] = (derivedType.GetMethod("set_current_config", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.update_from_current_config] = (derivedType.GetMethod("update_from_current_config", flagPublic).DeclaringType == derivedType);
            functionsDefined[Function.pre_start_routine] = (derivedType.GetMethod("pre_start_routine", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.start_routine] = (derivedType.GetMethod("start_routine", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.post_start_routine] = (derivedType.GetMethod("post_start_routine", flagPrivate).DeclaringType == derivedType);

            functionsDefined[Function.on_gui] = (derivedType.GetMethod("on_gui", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.pre_update] = (derivedType.GetMethod("pre_update", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.update] = (derivedType.GetMethod("update", flagPrivate).DeclaringType == derivedType);
            functionsDefined[Function.post_update] = (derivedType.GetMethod("post_update", flagPrivate).DeclaringType == derivedType);
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
            //try {
                if (!initialize()) {
                    log_error("Initialization failed.");
                    return m_initialized;
                }
            //} catch (Exception e) {
            //    display_exception(e);
            //    return m_initialized;
            //}

            return m_initialized = true;
        }

        public void base_clean() {

            if (!m_initialized) {
                return;
            }

            currentFunction = Function.clean;
            if (catchExceptions) {
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

            if (catchExceptions) {
                try {
                    start_experiment();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                start_experiment();
            }      
        }

        // This function is called only once at the end of an experiment 
        public void base_stop_experiment() {
    
            currentFunction = Function.stop_experiment;
            if (catchExceptions) {
                try {
                    stop_experiment();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                stop_experiment();
            }

            // disable the closing state
            m_closed = false;
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
            if (catchExceptions) {
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
            if (catchExceptions) {
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
            if (catchExceptions) {
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
            if (catchExceptions) {
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
            if (catchExceptions) {
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
            if (catchExceptions) {
                try {
                    stop_routine();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                stop_routine();
            }
        }

        // Called several times per frame if component inside timeline
        public void base_on_gui() {

            currentFunction = Function.on_gui;
            if (catchExceptions) {
                try {
                    on_gui();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                on_gui();
            }
        }

        // Called at every frame if component inside timeline, will call child pre_update
        public void base_pre_update() {

            currentFunction = Function.pre_update;
            if (catchExceptions) {
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
            if (catchExceptions) {
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
            if (catchExceptions) {
                try {
                    post_update();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                post_update();
            }
        }

        // Play command from the GUI, will resumed if on pause, will call child play
        public void base_play() {

            currentFunction = Function.play;
            if (catchExceptions) {
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
            if (catchExceptions) {
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

            if (catchExceptions) {
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
            if (catchExceptions) {
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
            if (catchExceptions) {
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

                if (catchExceptions) {
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

                        if (catchExceptions) {
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
        protected virtual void on_gui() {}
        protected virtual void pre_update() {}
        protected virtual void update() {}
        protected virtual void post_update() {}

        // from gui
        protected virtual void update_parameter_from_gui(string updatedArgName) {}
        protected virtual void action_from_gui(bool initConfig, string action) {}



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