
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
using System.Runtime.InteropServices;

namespace Ex {

    public class Parameters {
        public enum Container { InitConfig = 0, CurrentConfig = 1, Dynamic = 2, Global = 3 };
    }

    public abstract class DLLExElement : DLLCppImport {

        //set_exp_ex_component(_handle, ExVR.Experiment().cppDll.getHandle());

        public bool contains(Parameters.Container pc, string name) {
            return contains_ex_element(_handle, (int)pc, name) == 1;
        }

        public T get<T>(Parameters.Container pc, string name) {
            int idC = (int)pc;
            switch (Type.GetTypeCode(typeof(T))) {
                case TypeCode.Boolean:
                    return (T)(object)(get_parameter_int_ex_element(_handle, idC, name) == 1);
                case TypeCode.Int16:
                    return (T)(object)(get_parameter_int_ex_element(_handle, idC, name));
                case TypeCode.Int32:
                    return (T)(object)(get_parameter_int_ex_element(_handle, idC, name));
                case TypeCode.Int64:
                    return (T)(object)(get_parameter_int_ex_element(_handle, idC, name));
                case TypeCode.Single:
                    return (T)(object)(get_parameter_float_ex_element(_handle, idC, name));
                case TypeCode.Double:
                    return (T)(object)(get_parameter_double_ex_element(_handle, idC, name));
                case TypeCode.String:
                    return (T)(object)(get_parameter_string_ex_element(_handle, idC, name));
            }
            return default(T);
        }

        public T[] get_array<T>(Parameters.Container pc, string name) {

            int idC = (int)pc;
            int size = get_size_parameter_array_ex_element(_handle, idC, name);
            if (size == 0) {
                return null;
            }

            object array = null;
            switch (Type.GetTypeCode(typeof(T))) {
                case TypeCode.Boolean:
                    ExVR.Log().error("Type not managed for arrays.");
                    break;
                case TypeCode.Int16:
                    array = new int[size];
                    get_parameter_array_int_ex_element(_handle, idC, name, (int[])array);
                    break;
                case TypeCode.Int32:
                    array = new int[size];
                    get_parameter_array_int_ex_element(_handle, idC, name, (int[])array);
                    break;
                case TypeCode.Int64:
                    array = new int[size];
                    get_parameter_array_int_ex_element(_handle, idC, name, (int[])array);
                    break;
                case TypeCode.Single:
                    array = new float[size];
                    get_parameter_array_float_ex_element(_handle, idC, name, (float[])array);
                    break;
                case TypeCode.Double:
                    array = new double[size];
                    get_parameter_array_double_ex_element(_handle, idC, name, (double[])array);
                    break;
                case TypeCode.String:
                    ExVR.Log().error("Type not managed for arrays.");
                    break;
            }
            return (T[])array;
        }

        public void set(Parameters.Container pc, string name, bool value) {
            set_parameter_int_ex_element(_handle, (int)pc, name, value ? 1 : 0);
        }
        public void set(Parameters.Container pc, string name, short value) {
            set_parameter_int_ex_element(_handle, (int)pc, name, Converter.to_int(value));
        }
        public void set(Parameters.Container pc, string name, int value) {
            set_parameter_int_ex_element(_handle, (int)pc, name, value);
        }
        public void set(Parameters.Container pc, string name, long value) {
            set_parameter_int_ex_element(_handle, (int)pc, name, Converter.to_int(value));
        }
        public void set(Parameters.Container pc, string name, float value) {
            set_parameter_float_ex_element(_handle, (int)pc, name, value);
        }
        public void set(Parameters.Container pc, string name, double value) {
            set_parameter_double_ex_element(_handle, (int)pc, name, value);
        }
        public void set(Parameters.Container pc, string name, string value) {
            set_parameter_string_ex_element(_handle, (int)pc, name, value);
        }

        public void set_array(Parameters.Container pc, string name, List<int> value, int length) {
            set_parameter_array_int_ex_element(_handle, (int)pc, name, value.ToArray(), length);
        }
        public void set_array(Parameters.Container pc, string name, List<float> value, int length) {
            set_parameter_array_float_ex_element(_handle, (int)pc, name, value.ToArray(), length);
        }
        public void set_array(Parameters.Container pc, string name, List<double> value, int length) {
            set_parameter_array_double_ex_element(_handle, (int)pc, name, value.ToArray(), length);
        }

        public void set(Parameters.Container pc, Argument arg) {

            if(arg.xml.UiGeneratorType == "Resource") {
                return;
            }

            var tCode = Type.GetTypeCode(arg.type);
            if (arg.xml.Dim == 0) {

                switch (tCode) {
                    case TypeCode.Boolean:
                        set(pc, arg.xml.Name, (bool)arg.value);
                        return;
                    case TypeCode.Int16:
                        set(pc, arg.xml.Name, (short)arg.value);
                        return;
                    case TypeCode.Int32:
                        set(pc, arg.xml.Name, (int)arg.value);
                        return;
                    case TypeCode.Int64:
                        set(pc, arg.xml.Name, (long)arg.value);
                        return;
                    case TypeCode.Single:
                        set(pc, arg.xml.Name, (float)arg.value);
                        return;
                    case TypeCode.Double:
                        set(pc, arg.xml.Name, (double)arg.value);
                        return;
                    case TypeCode.String:
                        set(pc, arg.xml.Name, (string)arg.value);
                        return;
                    default:
                        ExVR.Log().warning(string.Format("Type [{0}] not managed for arg [{1}] of dim 0.", tCode.ToString(), arg.xml.Name));
                        return;
                }

            } else if (arg.xml.Dim == 1) {
                string[] sizes = arg.xml.Sizes.Split(' ');
                int length = Converter.to_int(sizes[0]);

                switch (tCode) {
                    case TypeCode.Int32:
                        set_array(pc, arg.xml.Name, Converter.to_int_list(arg.value), length);
                        return;
                    case TypeCode.Single:
                       
                        set_array(pc, arg.xml.Name, Converter.to_float_list(arg.value), length);
                        return;
                    case TypeCode.Double:
                        set_array(pc, arg.xml.Name, Converter.to_double_list(arg.value), length);
                        return;

                    default:
                        ExVR.Log().warning(string.Format("Type [{0}] not managed for arg [{1}] of dim 1.", tCode.ToString(), arg.xml.Name));
                        return;
                }
            } else if (arg.xml.Dim == 2) {
                string[] sizes = arg.xml.Sizes.Split(' ');
                int rows = Converter.to_int(sizes[0]);
                int cols = Converter.to_int(sizes[1]);

                ExVR.Log().warning("Dimension 2 not managed.");
                //set_array(exComponent, container, xmlArg.Name, value, tCode, rows, cols);
            }
        }

        [DllImport("exvr-export", EntryPoint = "set_exp_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern void set_exp_ex_element(HandleRef exElement, HandleRef exExperiment);

        [DllImport("exvr-export", EntryPoint = "contains_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern int contains_ex_element(HandleRef exElement, int containerId, string name);

        [DllImport("exvr-export", EntryPoint = "set_parameter_bool_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern void set_parameter_bool_ex_element(HandleRef exElement, int containerId, string name, int value);

        [DllImport("exvr-export", EntryPoint = "set_parameter_int_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern void set_parameter_int_ex_element(HandleRef exElement, int containerId, string name, int value);

        [DllImport("exvr-export", EntryPoint = "set_parameter_float_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern void set_parameter_float_ex_element(HandleRef exElement, int containerId, string name, float value);

        [DllImport("exvr-export", EntryPoint = "set_parameter_double_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern void set_parameter_double_ex_element(HandleRef exElement, int containerId, string name, double value);

        [DllImport("exvr-export", EntryPoint = "set_parameter_string_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern void set_parameter_string_ex_element(HandleRef exElement, int containerId, string name, string value);


        [DllImport("exvr-export", EntryPoint = "get_parameter_int_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern int get_parameter_int_ex_element(HandleRef exElement, int containerId, string name);

        [DllImport("exvr-export", EntryPoint = "get_parameter_float_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern float get_parameter_float_ex_element(HandleRef exElement, int containerId, string name);

        [DllImport("exvr-export", EntryPoint = "get_parameter_double_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern double get_parameter_double_ex_element(HandleRef exElement, int containerId, string name);

        [DllImport("exvr-export", EntryPoint = "get_parameter_string_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern string get_parameter_string_ex_element(HandleRef exElement, int containerId, string name);


        [DllImport("exvr-export", EntryPoint = "get_size_parameter_array_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern int get_size_parameter_array_ex_element(HandleRef exElement, int containerId, string name);

        [DllImport("exvr-export", EntryPoint = "get_parameter_array_int_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern void get_parameter_array_int_ex_element(HandleRef exElement, int containerId, string name, int[] array);

        [DllImport("exvr-export", EntryPoint = "get_parameter_array_float_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern void get_parameter_array_float_ex_element(HandleRef exElement, int containerId, string name, float[] array);

        [DllImport("exvr-export", EntryPoint = "get_parameter_array_double_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern void get_parameter_array_double_ex_element(HandleRef exElement, int containerId, string name, double[] array);


        [DllImport("exvr-export", EntryPoint = "set_parameter_array_int_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern void set_parameter_array_int_ex_element(HandleRef exElement, int containerId, string name, int[] values, int size);

        [DllImport("exvr-export", EntryPoint = "set_parameter_array_float_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern void set_parameter_array_float_ex_element(HandleRef exElement, int containerId, string name, float[] values, int size);

        [DllImport("exvr-export", EntryPoint = "set_parameter_array_double_ex_element", CallingConvention = CallingConvention.Cdecl)]
        static public extern void set_parameter_array_double_ex_element(HandleRef exElement, int containerId, string name, double[] values, int size);



    }
}