
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
using System.Linq;
using System.ComponentModel;
using System.Collections.Generic;
using System.Runtime.InteropServices;


namespace Ex.DLL{


    public class Parameters {
        public enum Container { InitConfig = 0, CurrentConfig = 1, Dynamic = 2 };
    }

    public abstract class ExComponentDLL : CppDllImport{

        // parent component
        public ExComponent parent = null;


        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate long EllapsedTimeExpMsCB();
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate long EllapsedTimeRoutineMsCB();
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate int GetCB(string componentName);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate int IsInitializedCB(int key);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate int IsVisibleCB(int key);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate int IsUpdatingCB(int key);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate int IsClosedCB(int key);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate void NextCB();
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate void PreviousCB();
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate void CloseCB(int key);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate void SignalBoolCB(int key, int index, int value);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate void SignalIntCB(int key, int index, int value);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate void SignalFloatCB(int key, int index, float value);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate void SignalDoubleCB(int key, int index, double value);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate void SignalStringCB(int key, int index, string value);

        EllapsedTimeExpMsCB ellapsedTimeExpMsCB = null;
        EllapsedTimeRoutineMsCB ellapsedTimeRoutineMsCB = null;
        GetCB getCB = null;
        IsInitializedCB isInitializedCB = null;
        IsVisibleCB isVisibleCB = null;
        IsUpdatingCB isUpdatingCB = null;
        IsClosedCB isClosedCB = null;
        NextCB nextCB = null;
        PreviousCB previousCB = null;
        CloseCB closeCB = null;
        SignalBoolCB signalBoolCB = null;
        SignalIntCB signalIntCB = null;
        SignalFloatCB signalFloatCB = null;
        SignalDoubleCB signalDoubleCB = null;
        SignalStringCB signalStringCB = null;

        public void init_callbacks() {

            stackTraceCB            = (trace) => {ExVR.ExpLog().push_to_strackTrace(new ComponentTrace(trace));};
            logCB                   = (message) => {parent.log_message(message, false, false);};
            logWarningCB            = (warning) => {parent.log_warning(warning, true, false);};
            logErrorCB              = (error) => { parent.log_error(error, true, false);};
            ellapsedTimeExpMsCB     = () => {return (int)ExVR.Time().ellapsed_exp_ms();};
            ellapsedTimeRoutineMsCB = () => {return (int)ExVR.Time().ellapsed_element_ms();};
            getCB                   = (string componentName) => {return ExVR.Components().get_from_name(componentName).key;};
            isInitializedCB         = (key) => { // TODO: remove
                return 1;
            };
            isVisibleCB             = (key) => {return parent.is_visible() ? 1 : 0;};
            isUpdatingCB            = (key) => {return parent.is_updating() ? 1 : 0;};
            isClosedCB              = (key) => {return parent.is_closed() ? 1 : 0;};
            nextCB                  = () => {ExVR.Events().command.next();};
            previousCB              = () => {ExVR.Events().command.previous();};
            closeCB                 = (key) => {parent.close();};
            signalBoolCB            = (key, index, value) => {parent.invoke_signal("signal" + (index + 1), (value == 1));};
            signalIntCB             = (key, index, value) => {parent.invoke_signal("signal" + (index + 1), value);};
            signalFloatCB           = (key, index, value) => {parent.invoke_signal("signal" + (index + 1), value);};
            signalDoubleCB          = (key, index, value) => {parent.invoke_signal("signal" + (index + 1), value);};
            signalStringCB          = (key, index, value) => {parent.invoke_signal("signal" + (index + 1), value);};

            init_callbacks_ex_component(
                _handle,
                stackTraceCB,
                logCB,
                logWarningCB,
                logErrorCB,
                ellapsedTimeExpMsCB,
                ellapsedTimeRoutineMsCB,
                getCB,
                isInitializedCB,
                isVisibleCB,
                isUpdatingCB,
                isClosedCB,
                nextCB,
                previousCB,
                closeCB,
                signalBoolCB,
                signalIntCB,
                signalFloatCB,
                signalDoubleCB,
                signalStringCB
            );
        }


        // dll management
        abstract protected override void create_DLL_class();
        protected override void delete_DLL_class() { delete_ex_component(_handle);}

        // once per loading
        public virtual bool initialize() {

            init_callbacks();

            foreach (KeyValuePair<string, Argument> arg in parent.initC.args) {
                update_component_from_arg(Parameters.Container.InitConfig, arg.Value);
            }

            update_parameter_int_ex_component(_handle, (int)Parameters.Container.Dynamic, "key", parent.key);

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
        public virtual void update_from_current_config() {update_from_current_config_ex_component(_handle);} 
        public virtual void pre_start_routine() {pre_start_routine_ex_component(_handle);}
        public virtual void start_routine() {

            foreach (KeyValuePair<string, Argument> arg in parent.currentC.args) {
                update_component_from_arg(Parameters.Container.CurrentConfig, arg.Value);
            }
            start_routine_ex_component(_handle);
        }

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
            update_component_from_arg(Parameters.Container.CurrentConfig, parent.currentC.args[updatedArgName]);
            update_parameter_from_gui_ex_component(_handle, updatedArgName);
        }
        public virtual void action_from_gui(bool initConfig, string action) {
            action_from_gui_ex_component(_handle, initConfig ? 1 : 0, action);
        }
        public virtual void add_dynamic_parameter(string name, object value, TypeCode type) {
            update_parameter(Parameters.Container.Dynamic, name, value, type);
        }

        public virtual void call_slot(int index, object value) {

            int idC = (int)Parameters.Container.Dynamic;

            string idSlot = string.Concat("slot", index);
            if (value is bool) {
                update_parameter_bool_ex_component(_handle, idC, idSlot, ((bool)value) ? 1 : 0);
            } else if (value is int) {
                update_parameter_int_ex_component(_handle, idC, idSlot, (int)value);
            } else if (value is float) {
                update_parameter_float_ex_component(_handle, idC, idSlot, (float)value);
            } else if (value is double) {
                update_parameter_double_ex_component(_handle, idC, idSlot, (double)value);
            } else if (value is string) {
                update_parameter_string_ex_component(_handle, idC, idSlot, (string)value);
            } else if(value is List<double>) {
                update_parameter_array_double_ex_component(_handle, idC, idSlot, ((List<double>)value).ToArray(), ((List<double>)value).Count);
            } else {
                ExVR.Log().error("Type not managed by component.");
                return;
            }
            call_slot_ex_component(_handle, index);
        }

        public bool contains(Parameters.Container container, string name) {
            return contains_ex_component(_handle, (int)container, name) == 1;
        }

        public object get_parameter<T>(Parameters.Container container, string name) {
            int idC = (int)container;
            switch (Type.GetTypeCode(typeof(T))) {
                case TypeCode.Boolean:
                    return get_parameter_int_ex_component(_handle, idC, name) == 1;
                case TypeCode.Int16:
                    return get_parameter_int_ex_component(_handle, idC, name);
                case TypeCode.Int32:
                    return get_parameter_int_ex_component(_handle, idC, name);
                case TypeCode.Int64:
                    return get_parameter_int_ex_component(_handle, idC, name);
                case TypeCode.Single:
                    return get_parameter_float_ex_component(_handle, idC, name);
                case TypeCode.Double:
                    return get_parameter_double_ex_component(_handle, idC, name);
                case TypeCode.String:
                    return get_parameter_string_ex_component(_handle, idC, name);
            }
            return default(T);
        }

        public object get_parameter_1d(Parameters.Container container, string name, TypeCode t) {

            int idC = (int)container;
            int size = get_size_parameter_array_ex_component(_handle, idC, name);
            if (size == 0) {
                return null;
            }
            switch (t) {
                case TypeCode.Boolean:
                    ExVR.Log().error("Type not managed for arrays.");
                    break;
                case TypeCode.Int16:
                    var array1 = new int[size];
                    get_parameter_array_int_ex_component(_handle, idC, name, array1);
                    return array1;
                case TypeCode.Int32:
                    var array2 = new int[size];
                    get_parameter_array_int_ex_component(_handle, idC, name, array2);
                    return array2;
                case TypeCode.Int64:
                    var array3 = new int[size];
                    get_parameter_array_int_ex_component(_handle, idC, name, array3);
                    return array3;
                case TypeCode.Single:
                    var array4 = new float[size];
                    get_parameter_array_float_ex_component(_handle, idC, name, array4);
                    return array4;
                case TypeCode.Double:
                    var array5 = new double[size];
                    get_parameter_array_double_ex_component(_handle, idC, name, array5);
                    return array5;
                case TypeCode.String:
                    ExVR.Log().error("Type not managed for arrays.");
                    break;
            }

            return null;
        }

        public void update_parameter(Parameters.Container container, string name, object value, TypeCode t) {

            int idC = (int)container;
            switch (t) {
                case TypeCode.Boolean:
                    update_parameter_bool_ex_component(_handle, idC, name, ((bool)value) ? 1 : 0);
                    break;
                case TypeCode.Int16:
                    update_parameter_int_ex_component(_handle, idC, name, (int)value);
                    break;
                case TypeCode.Int32:
                    update_parameter_int_ex_component(_handle, idC, name, (int)value);
                    break;
                case TypeCode.Int64:
                    update_parameter_int_ex_component(_handle, idC, name, (int)value);
                    break;
                case TypeCode.Single:
                    update_parameter_float_ex_component(_handle, idC, name, (float)value);
                    break;
                case TypeCode.Double:
                    update_parameter_double_ex_component(_handle, idC, name, (double)value);
                    break;
                case TypeCode.String:
                    update_parameter_string_ex_component(_handle, idC, name, (string)value);
                    break;
            }
        }

        public void update_parameter_1d(Parameters.Container container, string name, List<object> value, int length, TypeCode t) {

            int idC = (int)container;
            switch (t) {
                case TypeCode.Boolean:
                    // ExVR.Log().error("Type not managed for arrays: " + t.ToString() + " "+ name);
                    return;
                case TypeCode.Int16:                    
                    update_parameter_array_int_ex_component(_handle, idC, name, value.Cast<int>().ToArray(), length);
                    break;
                case TypeCode.Int32:
                    update_parameter_array_int_ex_component(_handle, idC, name, value.Cast<int>().ToArray(), length);
                    break;
                case TypeCode.Int64:
                    update_parameter_array_int_ex_component(_handle, idC, name, value.Cast<int>().ToArray(), length);
                    break;
                case TypeCode.Single:
                    update_parameter_array_float_ex_component(_handle, idC, name, value.Cast<float>().ToArray(), length);
                    break;
                case TypeCode.Double:
                    update_parameter_array_double_ex_component(_handle, idC, name, value.Cast<double>().ToArray(), length);
                    break;
                case TypeCode.String:
                    // ExVR.Log().error("Type not managed for arrays: " + t.ToString() + " "+ name);
                    break;
            }
        }

        private void update_component_from_arg(Parameters.Container container, Argument arg) {


            var tCode = Type.GetTypeCode(arg.type);
            if (arg.xml.Dim == 0) {
                update_parameter(container, arg.xml.Name, arg.value, tCode);
            } else if (arg.xml.Dim == 1) {
                string[] sizes = arg.xml.Sizes.Split(' ');
                int length = (int)TypeDescriptor.GetConverter(typeof(int)).ConvertFrom(sizes[0]);
                update_parameter_1d(container, arg.xml.Name, (List<object>)arg.value, length, tCode);
            } else if (arg.xml.Dim == 2) {
                //string[] sizes = xmlArg.Sizes.Split(' ');
                //int rows = (int)TypeDescriptor.GetConverter(typeof(int)).ConvertFrom(sizes[0]);
                //int cols = (int)TypeDescriptor.GetConverter(typeof(int)).ConvertFrom(sizes[1]);
                //update_parameter_2d(exComponent, container, xmlArg.Name, value, tCode, rows, cols);
            }
        }


        [DllImport("exvr-export", EntryPoint = "delete_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void delete_ex_component(HandleRef exComponent);


        [DllImport("exvr-export", EntryPoint = "initialize_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern int initialize_ex_component(HandleRef exComponent);

        [DllImport("exvr-export", EntryPoint = "clean_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void clean_ex_component(HandleRef exComponent);

        [DllImport("exvr-export", EntryPoint = "init_callbacks_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static private extern void init_callbacks_ex_component(
            HandleRef exComponent,
            [MarshalAs(UnmanagedType.FunctionPtr)] StrackTraceCB strackTraceCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] LogCB logCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] LogWarningCB logWarningCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] LogErrorCB logErrorCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] EllapsedTimeExpMsCB ellapsedTimeExpMsCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] EllapsedTimeRoutineMsCB ellapsedTimeRoutineMsCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] GetCB getCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] IsInitializedCB isInitializedCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] IsVisibleCB isVisibleCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] IsUpdatingCB isUpdatingCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] IsClosedCB isClosedCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] NextCB nextCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] PreviousCB previousCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] CloseCB closeCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] SignalBoolCB signalBoolCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] SignalIntCB signalIntCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] SignalFloatCB signalFloatCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] SignalDoubleCB signalDoubleCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] SignalStringCB signalStringCB
        );


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

        [DllImport("exvr-export", EntryPoint = "contains_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern int contains_ex_component(HandleRef exComponent, int containerId, string name);

        [DllImport("exvr-export", EntryPoint = "update_parameter_bool_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_bool_ex_component(HandleRef exComponent, int containerId, string name, int value);

        [DllImport("exvr-export", EntryPoint = "update_parameter_int_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_int_ex_component(HandleRef exComponent, int containerId, string name, int value);

        [DllImport("exvr-export", EntryPoint = "update_parameter_float_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_float_ex_component(HandleRef exComponent, int containerId, string name, float value);

        [DllImport("exvr-export", EntryPoint = "update_parameter_double_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_double_ex_component(HandleRef exComponent, int containerId, string name, double value);

        [DllImport("exvr-export", EntryPoint = "update_parameter_string_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_string_ex_component(HandleRef exComponent, int containerId, string name, string value);


        [DllImport("exvr-export", EntryPoint = "get_parameter_int_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern int get_parameter_int_ex_component(HandleRef exComponent, int containerId, string name);

        [DllImport("exvr-export", EntryPoint = "get_parameter_float_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern float get_parameter_float_ex_component(HandleRef exComponent, int containerId, string name);

        [DllImport("exvr-export", EntryPoint = "get_parameter_double_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern double get_parameter_double_ex_component(HandleRef exComponent, int containerId, string name);

        [DllImport("exvr-export", EntryPoint = "get_parameter_string_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern string get_parameter_string_ex_component(HandleRef exComponent, int containerId, string name);


        [DllImport("exvr-export", EntryPoint = "get_size_parameter_array_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern int get_size_parameter_array_ex_component(HandleRef exComponent, int containerId, string name);

        [DllImport("exvr-export", EntryPoint = "get_parameter_array_int_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void get_parameter_array_int_ex_component(HandleRef exComponent, int containerId, string name, int[] array);

        [DllImport("exvr-export", EntryPoint = "get_parameter_array_float_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void get_parameter_array_float_ex_component(HandleRef exComponent, int containerId, string name, float[] array);

        [DllImport("exvr-export", EntryPoint = "get_parameter_array_double_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void get_parameter_array_double_ex_component(HandleRef exComponent, int containerId, string name, double[] array);


        [DllImport("exvr-export", EntryPoint = "update_parameter_array_int_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_array_int_ex_component(HandleRef exComponent, int containerId, string name, int[] values, int size);

        [DllImport("exvr-export", EntryPoint = "update_parameter_array_float_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_array_float_ex_component(HandleRef exComponent, int containerId, string name, float[] values, int size);

        [DllImport("exvr-export", EntryPoint = "update_parameter_array_double_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_array_double_ex_component(HandleRef exComponent, int containerId, string name, double[] values, int size);


       
    }
}
