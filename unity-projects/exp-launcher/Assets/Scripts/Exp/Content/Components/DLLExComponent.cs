
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
using System.Collections.Generic;
using System.Runtime.InteropServices;


namespace Ex{



    public abstract class DLLExComponent : DLLExElement {

        // parent component
        public ExComponent parent = null;

        // dll management
        abstract protected override void create_DLL_class();
        protected override void delete_DLL_class() { delete_ex_component(_handle);}

        // once per loading
        public virtual bool initialize() {

            set_exp_ex_element(_handle, ExVR.Experiment().cppDll.getHandle());
            set(Parameters.Container.Global, "component_key", parent.key);

            foreach (KeyValuePair<string, Argument> arg in parent.initC.args) {
                set(Parameters.Container.InitConfig, arg.Value);
            }
            return initialize_ex_component(_handle) == 1 ? true : false;
        }

        public virtual void clean() { 
            clean_ex_component(_handle);
            Dispose();
        }

        // once per experiment
        public virtual void start_experiment() {start_experiment_ex_component(_handle);}
        public virtual void stop_experiment() {stop_experiment_ex_component(_handle);}


        // once per routine
        public virtual void set_current_config(string configName) {set_current_config_ex_component(_handle, configName);}
        public virtual void update_from_current_config() {

            foreach (KeyValuePair<string, Argument> arg in parent.currentC.args) {
                set(Parameters.Container.CurrentConfig, arg.Value);
            }

            update_from_current_config_ex_component(_handle);
        } 
        public virtual void pre_start_routine() {pre_start_routine_ex_component(_handle);}
        public virtual void start_routine() {start_routine_ex_component(_handle);}
        public virtual void post_start_routine() {post_start_routine_ex_component(_handle);}
        public virtual void stop_routine() {stop_routine_ex_component(_handle);}

        // every frame or more
        public virtual void on_gui() { on_gui_ex_component(_handle); }
        public virtual void pre_update() { pre_update_ex_component(_handle); }
        public virtual void update() {update_ex_component(_handle);}
        public virtual void post_update() { post_update_ex_component(_handle); }

        // several times per routine
        public virtual void set_visibility(bool visible) { set_visibility_ex_component(_handle, visible ? 1 : 0); }
        public virtual void set_update_state(bool doUpdate) { set_update_state_ex_component(_handle, doUpdate ? 1 : 0); }
        public virtual void play() { play_ex_component(_handle); }
        public virtual void pause() { pause_ex_component(_handle); }

        // gui
        public virtual void update_parameter_from_gui(string updatedArgName) {
            set( Parameters.Container.CurrentConfig, parent.currentC.args[updatedArgName]);
            update_parameter_from_gui_ex_component(_handle, updatedArgName);
        }
        public virtual void action_from_gui(bool initConfig, string action) {
            action_from_gui_ex_component(_handle, initConfig ? 1 : 0, action);
        }

        //public virtual void call_slot(int index, object value) {

        //    int idC = (int)Parameters.Container.Dynamic;

        //    string idSlot = string.Concat("slot", index);
        //    if (value is bool) {
        //        update_parameter_bool_ex_component(_handle, idC, idSlot, ((bool)value) ? 1 : 0);
        //    } else if (value is int) {
        //        update_parameter_int_ex_component(_handle, idC, idSlot, (int)value);
        //    } else if (value is float) {
        //        update_parameter_float_ex_component(_handle, idC, idSlot, (float)value);
        //    } else if (value is double) {
        //        update_parameter_double_ex_component(_handle, idC, idSlot, (double)value);
        //    } else if (value is string) {
        //        update_parameter_string_ex_component(_handle, idC, idSlot, (string)value);
        //    } else if (value is List<double>) {
        //        update_parameter_array_double_ex_component(_handle, idC, idSlot, ((List<double>)value).ToArray(), ((List<double>)value).Count);
        //    } else {
        //        ExVR.Log().error("Type not managed by component.");
        //        return;
        //    }
        //    call_slot_ex_component(_handle, index);
        //}

        [DllImport("exvr-export", EntryPoint = "delete_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void delete_ex_component(HandleRef exComponent);

        [DllImport("exvr-export", EntryPoint = "initialize_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern int initialize_ex_component(HandleRef exComponent);

        [DllImport("exvr-export", EntryPoint = "clean_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void clean_ex_component(HandleRef exComponent);

        [DllImport("exvr-export", EntryPoint = "start_experiment_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void start_experiment_ex_component(HandleRef exComponent);

        [DllImport("exvr-export", EntryPoint = "stop_experiment_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void stop_experiment_ex_component(HandleRef exComponent);


        [DllImport("exvr-export", EntryPoint = "set_current_config_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void set_current_config_ex_component(HandleRef exComponent, string configName);
        [DllImport("exvr-export", EntryPoint = "update_from_current_config_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_from_current_config_ex_component(HandleRef exComponent);

        [DllImport("exvr-export", EntryPoint = "pre_start_routine_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void pre_start_routine_ex_component(HandleRef exComponent);

        [DllImport("exvr-export", EntryPoint = "start_routine_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void start_routine_ex_component(HandleRef exComponent);

        [DllImport("exvr-export", EntryPoint = "post_start_routine_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void post_start_routine_ex_component(HandleRef exComponent);

        [DllImport("exvr-export", EntryPoint = "stop_routine_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void stop_routine_ex_component(HandleRef exComponent);


        [DllImport("exvr-export", EntryPoint = "on_gui_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void on_gui_ex_component(HandleRef exComponent);
        [DllImport("exvr-export", EntryPoint = "pre_update_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void pre_update_ex_component(HandleRef exComponent);

        [DllImport("exvr-export", EntryPoint = "update_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_ex_component(HandleRef exComponent);
        [DllImport("exvr-export", EntryPoint = "post_update_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void post_update_ex_component(HandleRef exComponent);


        [DllImport("exvr-export", EntryPoint = "update_parameter_from_gui_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_from_gui_ex_component(HandleRef exComponent, string updatedArgName);

        [DllImport("exvr-export", EntryPoint = "action_from_gui_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void action_from_gui_ex_component(HandleRef exComponent, int initConfig, string action);


        [DllImport("exvr-export", EntryPoint = "set_visibility_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void set_visibility_ex_component(HandleRef exComponent, int visible);

        [DllImport("exvr-export", EntryPoint = "set_update_state_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void set_update_state_ex_component(HandleRef exComponent, int doUpdate);

        [DllImport("exvr-export", EntryPoint = "play_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void play_ex_component(HandleRef exComponent);

        [DllImport("exvr-export", EntryPoint = "pause_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void pause_ex_component(HandleRef exComponent);

        [DllImport("exvr-export", EntryPoint = "call_slot_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void call_slot_ex_component(HandleRef exComponent, int index);     
    }
}
