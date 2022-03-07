
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


namespace Ex.DLL {


    public abstract class ExResourceDll : CppDllImport {

        // parent component
        public CppResource parent = null;

        // dll management
        abstract protected override void create_DLL_class();
        protected override void delete_DLL_class() { delete_ex_resource(_handle); }

        // once per loading
        public virtual bool initialize() {
            init_callbacks();
            return initialize_ex_resource(_handle) == 1 ? true : false;
        }

        public virtual void clean() {
            clean_ex_resource(_handle);
            Dispose();
        }



        public void init_callbacks() {

            stackTraceCB = (trace) => { ExVR.ExpLog().push_to_strackTrace(new ComponentTrace(trace)); };
            logCB = (message) => { parent.log_message(message, false, false); };
            logWarningCB = (warning) => { parent.log_warning(warning, true, false); };
            logErrorCB = (error) => { parent.log_error(error, true, false); };
          
            init_callbacks_ex_resource(
                _handle,
                stackTraceCB,
                logCB,
                logWarningCB,
                logErrorCB
            );
        }

        public virtual void add_parameter(string name, object value, TypeCode type) {
            update_parameter(name, value, type);
        }

        public bool contains(string name) {
            return contains_ex_resource(_handle, name) == 1;
        }

        public object get_parameter<T>(string name) {

            switch (Type.GetTypeCode(typeof(T))) {
                case TypeCode.Boolean:
                    return get_parameter_int_ex_resource(_handle, name) == 1;
                case TypeCode.Int16:
                    return get_parameter_int_ex_resource(_handle, name);
                case TypeCode.Int32:
                    return get_parameter_int_ex_resource(_handle, name);
                case TypeCode.Int64:
                    return get_parameter_int_ex_resource(_handle, name);
                case TypeCode.Single:
                    return get_parameter_float_ex_resource(_handle, name);
                case TypeCode.Double:
                    return get_parameter_double_ex_resource(_handle, name);
                case TypeCode.String:
                    return get_parameter_string_ex_resource(_handle, name);
            }
            return default(T);
        }

        public object get_parameter_1d(string name, TypeCode t) {

            int size = get_size_parameter_array_ex_resource(_handle, name);
            if (size == 0) {
                return null;
            }
            switch (t) {
                case TypeCode.Boolean:
                    ExVR.Log().error(string.Format("Type not managed for arrays: {0} {1}", t.ToString(), name));
                    break;
                case TypeCode.Int16:
                    var array1 = new int[size];
                    get_parameter_array_int_ex_resource(_handle, name, array1);
                    return array1;
                case TypeCode.Int32:
                    var array2 = new int[size];
                    get_parameter_array_int_ex_resource(_handle, name, array2);
                    return array2;
                case TypeCode.Int64:
                    var array3 = new int[size];
                    get_parameter_array_int_ex_resource(_handle, name, array3);
                    return array3;
                case TypeCode.Single:
                    var array4 = new float[size];
                    get_parameter_array_float_ex_resource(_handle, name, array4);
                    return array4;
                case TypeCode.Double:
                    var array5 = new double[size];
                    get_parameter_array_double_ex_resource(_handle, name, array5);
                    return array5;
                case TypeCode.String:
                    ExVR.Log().error(string.Format("Type not managed for arrays: {0} {1}", t.ToString(), name));
                    break;
            }

            return null;
        }

        public void update_parameter(string name, object value, TypeCode t) {

            switch (t) {
                case TypeCode.Boolean:
                    update_parameter_bool_ex_resource(_handle, name, ((bool)value) ? 1 : 0);
                    break;
                case TypeCode.Int16:
                    update_parameter_int_ex_resource(_handle, name, (int)value);
                    break;
                case TypeCode.Int32:
                    update_parameter_int_ex_resource(_handle, name, (int)value);
                    break;
                case TypeCode.Int64:
                    update_parameter_int_ex_resource(_handle, name, (int)value);
                    break;
                case TypeCode.Single:
                    update_parameter_float_ex_resource(_handle, name, (float)value);
                    break;
                case TypeCode.Double:
                    update_parameter_double_ex_resource(_handle, name, (double)value);
                    break;
                case TypeCode.String:
                    update_parameter_string_ex_resource(_handle, name, (string)value);
                    break;
            }
        }

        public void update_parameter_1d(string name, List<object> value, int length, TypeCode t) {

            switch (t) {
                case TypeCode.Boolean:
                    ExVR.Log().error(string.Format("Type not managed for arrays: {0} {1}", t.ToString(), name));
                    return;
                case TypeCode.Int16:
                    update_parameter_array_int_ex_resource(_handle, name, value.Cast<int>().ToArray(), length);
                    break;
                case TypeCode.Int32:
                    update_parameter_array_int_ex_resource(_handle, name, value.Cast<int>().ToArray(), length);
                    break;
                case TypeCode.Int64:
                    update_parameter_array_int_ex_resource(_handle, name, value.Cast<int>().ToArray(), length);
                    break;
                case TypeCode.Single:
                    update_parameter_array_float_ex_resource(_handle, name, value.Cast<float>().ToArray(), length);
                    break;
                case TypeCode.Double:
                    update_parameter_array_double_ex_resource(_handle, name, value.Cast<double>().ToArray(), length);
                    break;
                case TypeCode.String:
                    ExVR.Log().error(string.Format("Type not managed for arrays: {0} {1}", t.ToString(), name));
                    break;
            }
        }

        private void update_component_from_arg(Argument arg) {

            var tCode = Type.GetTypeCode(arg.type);
            if (arg.xml.Dim == 0) {
                update_parameter(arg.xml.Name, arg.value, tCode);
            } else if (arg.xml.Dim == 1) {
                string[] sizes = arg.xml.Sizes.Split(' ');
                int length = (int)TypeDescriptor.GetConverter(typeof(int)).ConvertFrom(sizes[0]);
                update_parameter_1d(arg.xml.Name, (List<object>)arg.value, length, tCode);
            } else if (arg.xml.Dim == 2) {
                //string[] sizes = xmlArg.Sizes.Split(' ');
                //int rows = (int)TypeDescriptor.GetConverter(typeof(int)).ConvertFrom(sizes[0]);
                //int cols = (int)TypeDescriptor.GetConverter(typeof(int)).ConvertFrom(sizes[1]);
                //update_parameter_2d(exComponent, container, xmlArg.Name, value, tCode, rows, cols);
            }
        }


        [DllImport("exvr-export", EntryPoint = "delete_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern void delete_ex_resource(HandleRef exResource);

        [DllImport("exvr-export", EntryPoint = "initialize_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern int initialize_ex_resource(HandleRef exResource);

        [DllImport("exvr-export", EntryPoint = "clean_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern void clean_ex_resource(HandleRef exResource);

        [DllImport("exvr-export", EntryPoint = "init_callbacks_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern void init_callbacks_ex_resource(
            HandleRef exResource,
            [MarshalAs(UnmanagedType.FunctionPtr)] StrackTraceCB strackTraceCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] LogCB logCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] LogWarningCB logWarningCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] LogErrorCB logErrorCB
        );

        [DllImport("exvr-export", EntryPoint = "contains_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern int contains_ex_resource(HandleRef exResource, string name);

        [DllImport("exvr-export", EntryPoint = "update_parameter_bool_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_bool_ex_resource(HandleRef exResource, string name, int value);

        [DllImport("exvr-export", EntryPoint = "update_parameter_int_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_int_ex_resource(HandleRef exResource, string name, int value);

        [DllImport("exvr-export", EntryPoint = "update_parameter_float_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_float_ex_resource(HandleRef exResource, string name, float value);

        [DllImport("exvr-export", EntryPoint = "update_parameter_double_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_double_ex_resource(HandleRef exResource, string name, double value);

        [DllImport("exvr-export", EntryPoint = "update_parameter_string_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_string_ex_resource(HandleRef exResource, string name, string value);


        [DllImport("exvr-export", EntryPoint = "get_parameter_int_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern int get_parameter_int_ex_resource(HandleRef exResource, string name);

        [DllImport("exvr-export", EntryPoint = "get_parameter_float_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern float get_parameter_float_ex_resource(HandleRef exResource, string name);

        [DllImport("exvr-export", EntryPoint = "get_parameter_double_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern double get_parameter_double_ex_resource(HandleRef exResource, string name);

        [DllImport("exvr-export", EntryPoint = "get_parameter_string_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern string get_parameter_string_ex_resource(HandleRef exResource, string name);


        [DllImport("exvr-export", EntryPoint = "get_size_parameter_array_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern int get_size_parameter_array_ex_resource(HandleRef exResource, string name);

        [DllImport("exvr-export", EntryPoint = "get_parameter_array_int_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern void get_parameter_array_int_ex_resource(HandleRef exResource, string name, int[] array);

        [DllImport("exvr-export", EntryPoint = "get_parameter_array_float_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern void get_parameter_array_float_ex_resource(HandleRef exResource, string name, float[] array);

        [DllImport("exvr-export", EntryPoint = "get_parameter_array_double_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern void get_parameter_array_double_ex_resource(HandleRef exResource, string name, double[] array);


        [DllImport("exvr-export", EntryPoint = "update_parameter_array_int_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_array_int_ex_resource(HandleRef exResource, string name, int[] values, int size);

        [DllImport("exvr-export", EntryPoint = "update_parameter_array_float_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_array_float_ex_resource(HandleRef exResource, string name, float[] values, int size);

        [DllImport("exvr-export", EntryPoint = "update_parameter_array_double_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern void update_parameter_array_double_ex_resource(HandleRef exResource, string name, double[] values, int size);
    }
}
