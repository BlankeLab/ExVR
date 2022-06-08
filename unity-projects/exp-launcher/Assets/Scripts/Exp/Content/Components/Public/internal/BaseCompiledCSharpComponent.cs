
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
using System.Collections.Generic;
using System.Runtime.CompilerServices;

// unity
using UnityEngine;

namespace Ex {

    public class BaseCompiledCSharpComponent : MonoBehaviour {

        // internal
        static private readonly string signal1Str = "signal1";
        static private readonly string signal2Str = "signal2";
        static private readonly string signal3Str = "signal3";
        static private readonly string signal4Str = "signal4";

        // associated component
        public ExComponent p = null;

        // randomization
        public int instance_id() { return ExVR.Instance().idInstance;}
        public Interval current_element_interval() { return ExVR.Scheduler().current_element_info().interval(); }
        public int current_element_order() { return ExVR.Scheduler().current_element_info().order(); }
        public int current_element_iteration() { return ExVR.Scheduler().current_element_info().element_iteration(); }
        public List<RoutineInfo> get_current_routine_instance_infos() { return ExVR.Instance().get_routine_infos_order(current_routine());}
        public List<Condition> get_current_routine_instance_conditions() { return ExVR.Instance().get_routine_conditions_order(current_routine());}
        public List<string> get_current_routine_instance_conditions_names() { return ExVR.Instance().get_routine_conditions_names_order(current_routine());}
        public List<string> get_routine_instance_conditions_names(string routineName) {
            var routine = get_routine(routineName);
            if (routine != null) {
                return ExVR.Instance().get_routine_conditions_names_order(routine);
            }
            log_error(string.Format("Routine with name {0} doesn't exist.", routineName));
            return new List<string>();            
        }
        
        // routines 
        public Routine current_routine() { return p.currentRoutine; }
        public Routine get_routine(string routineName) {return ExVR.Routines().get(routineName);}

        // conditions
        public Condition current_condition() { return p.currentCondition; }

        // configs
        public ComponentConfig current_config() { return p.current_config(); }
        public ComponentInitConfig init_config() { return p.init_config(); }

        // connections
        public Events.Connections connections() {return p.connections();}       

        // states
        public bool is_visible() { return p.is_visible(); }
        public bool is_updating() { return p.is_updating(); }
        public bool is_closed() { return p.is_closed(); }

        // commands
        public void next() { p.command().next(); }
        public void next_with_name(string componentName) { p.command().next_element_with_name(componentName); }
        public void schedule_next(int milliseconds) { p.command().schedule_next(milliseconds); }
        public void previous() { p.command().previous(); }
        public void close() { p.components().close(p); }
        public void stop() { p.command().force_stop_experiment(); }

        // times
        public long frame_id() { return p.time().frame_id(); }
        public double ellapsed_time_exp_ms() { return p.time().ellapsed_exp_ms();}
        public double ellapsed_time_routine_ms() { return p.time().ellapsed_element_ms();}
        public double ellapsed_time_frame_ms() {return p.time().ellapsed_frame_ms();}

        // logs
        public void log_message(string message, bool verbose = false, bool addExtraInfo = false,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0) {
            p.log_message(message, verbose, addExtraInfo, memberName, sourceFilePath, sourceLineNumber);
        }
        public void log_warning(string warning, bool verbose = true, bool addExtraInfo = true,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0) {
            p.log_warning(warning, verbose, addExtraInfo, memberName, sourceFilePath, sourceLineNumber);
        }
        public void log_error(string error, bool verbose = true, bool addExtraInfo = true,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0) {
            p.log_error(error, verbose, addExtraInfo, memberName, sourceFilePath, sourceLineNumber);
        }

        // signals
        public void invoke_signal1(object value) {p.invoke_signal(signal1Str, value);}
        public void invoke_signal2(object value) {p.invoke_signal(signal2Str, value);}
        public void invoke_signal3(object value) {p.invoke_signal(signal3Str, value);}
        public void invoke_signal4(object value) {p.invoke_signal(signal4Str, value);}

        // get
        public ExComponent get(int key) {return ExVR.Components().get_from_key(key);}
        public ExComponent get(string name) {return ExVR.Components().get_from_name(name);}
        public T get<T>(string name) where T : ExComponent {return ExVR.Components().get_from_name<T>(name);}

        public T get_first<T>() where T : ExComponent { return ExVR.Components().get_first<T>(); }
        public void set_first<T>(ref T component) where T : ExComponent {ExVR.Components().set_first<T>(ref component);}

        // # alias
        public ExComponent get_component(int key) {return get(key);}
        public ExComponent get_component(string name) {return get(name);}
        public T get_component<T>(string name) where T : ExComponent {return get<T>(name);}

        // get all
        public List<ExComponent> get_all(ExComponent.Category type) {return p.components().get_all_from_category(type);}
        public List<T> get_all<T>() where T : ExComponent {return p.components().get_all_from_type<T>();}
        public List<ExComponent> get_all_from_category(string categoryStr) {return p.components().get_all_from_category(categoryStr);}
        public List<ExComponent> get_all_from_type(string typeStr) {return p.components().get_all_from_type(Type.GetType(typeStr));}

        // # alias
        public List<ExComponent> get_all_components(ExComponent.Category type) {return get_all(type);}
        public List<T> get_all_components<T>() where T : ExComponent {return get_all<T>();}
        public List<ExComponent> get_all_components_from_category(string categoryStr) {return get_all_from_category(categoryStr);}
        public List<ExComponent> get_all_components_from_type(string typeStr) {return get_all_from_type(typeStr);}

        // get from current condition
        public List<ExComponent> get_all_from_current_condition() {return p.currentCondition.get_all_components();}
        public List<T> get_all_from_current_condition<T>() where T : ExComponent {return p.currentCondition.get_all_components<T>();}

        // # alias
        public List<ExComponent> get_all_components_from_current_condition() {return get_all_from_current_condition();}
        public List<T> get_all_components_from_current_condition<T>() where T : ExComponent {return get_all_from_current_condition<T>();}

        // get other scripts
        public T get_csharp_script<T>(string name) where T : BaseCompiledCSharpComponent { return p.components().get_csharp_script<T>(name); }

        #region ex_functions

        // once per loading
        public virtual bool initialize() { return true; }
        public virtual void clean() { }

        // once per experiment
        public virtual void start_experiment() { }
        public virtual void stop_experiment() { }

        // once per routine
        public virtual void set_current_config(string configName) { }
        public virtual void update_from_current_config() { }
        public virtual void pre_start_routine() { }
        public virtual void start_routine() {}
        public virtual void post_start_routine() { }
        public virtual void stop_routine() { }


        // several time per routine
        public virtual void set_update_state(bool doUpdate) { }
        public virtual void set_visibility(bool visible) { }
        public virtual void play() { }
        public virtual void pause() { }

        // every frame or more
        public virtual void on_gui() { }
        public virtual void pre_update() { }
        public virtual void update() { }
        public virtual void post_update() { }

        // from gui
        public virtual void action_from_gui(bool initConfig, string action) { }
        public virtual void update_parameter_from_gui(string updatedArgName) { }

        // # slots
        public virtual void slot1(object value) { }
        public virtual void slot2(object value) { }
        public virtual void slot3(object value) { }
        public virtual void slot4(object value) { }
        public virtual void slot5(IdAny idValue) { }

        // # transform related
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

        #endregion
    }
}