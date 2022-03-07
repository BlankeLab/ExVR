
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
using System.IO;
using System.Text;
using System.Collections;
using System.Collections.Generic;

// unity
using UnityEngine;


namespace Ex{

    class PythonStreamOutput : MemoryStream{
        public override void Write(byte[] buffer, int offset, int count) {

            string txt = Encoding.UTF8.GetString(buffer, offset, count);
            if (txt.Length > 2) {
                //Global.Log().message("From python: " + txt);
            }
        }
    }

    class PythonStreamErrorOutput : MemoryStream{
        public override void Write(byte[] buffer, int offset, int count) {

            string txt = Encoding.UTF8.GetString(buffer, offset, count);
            if (txt.Length > 2) {
                //Global.Log().error("From python: " + txt);
            }
        }
    }

    public class PythonManager : MonoBehaviour{

        public enum PyUnity : int
        {
            Void = 0, Color = 1, Vector2 = 2, Vector3 = 3, Vector4 = 4, Rect = 5, Ray = 6, Quaternion = 7, Plane = 8, Texture2D = 9,
            Array1D = 10, Array2D = 11, List1D = 12, String = 13, Integer = 14, Single = 15, Boolean = 16
        };

        private Dictionary<Type, Func<object, object>> pyTo;
        private Dictionary<Type, Func<object, object[]>> pyFrom;

        public void marshall_from(Type type, ref object value) {
            if (pyTo.ContainsKey(type)) {
                value = pyTo[type](value);
            }
        }

        public object[] marshall_to(Type type, object value) {

            if (pyFrom.ContainsKey(type)) {
                return pyFrom[type](value);
            }

            if (pyFrom.ContainsKey(type.GetGenericTypeDefinition())) {
                return pyFrom[type](value);
            }

            return new object[2] { 0, null };
        }


        public void initialize() {

            // from python to unity
            pyTo = new Dictionary<Type, Func<object, object>>();
            // # vector 2
            pyTo.Add(typeof(Vector2), value => {
                float[] array = (float[])value;
                return new Vector2(array[0], array[1]);
            });
            // # vector 3
            pyTo.Add(typeof(Vector3), value => {
                float[] array = (float[])value;
                return new Vector3(array[0], array[1], array[2]);
            });
            // # vector 4
            pyTo.Add(typeof(Vector4), value => {
                float[] array = (float[])value;
                return new Vector4(array[0], array[1], array[2], array[3]);
            });
            // # quaternion
            pyTo.Add(typeof(Quaternion), value => {
                float[] array = (float[])value;
                return new Quaternion(array[0], array[1], array[2], array[3]);
            });
            // # color
            pyTo.Add(typeof(Color), value => {
                float[] array = (float[])value;
                return new Color(array[1], array[1], array[2], array[3]);
            });
            // # rect
            pyTo.Add(typeof(Rect), value => {
                float[] array = (float[])value;
                return new Rect(array[0], array[1], array[2], array[3]);
            });
            // # ray2d
            pyTo.Add(typeof(Ray2D), value => {
                float[] array = (float[])value;
                return new Ray2D(new Vector2(array[0], array[1]), new Vector2(array[2], array[3]));
            });
            // # plane
            pyTo.Add(typeof(Plane), value => {
                float[] array = (float[])value;
                return new Plane(new Vector3(array[0], array[1], array[2]), new Vector3(array[3], array[4], array[5]));
            });
            // # texture2D
            pyTo.Add(typeof(Texture2D), value => {
                float[,] array2D = (float[,])value;
                Texture2D tex = new Texture2D(array2D.GetLength(0) / 3, array2D.GetLength(1));
                for (int ii = 0; ii < array2D.GetLength(0) / 3; ++ii) {
                    for (int jj = 0; jj < array2D.GetLength(1); ++jj) {
                        tex.SetPixel(ii, jj, new Color(array2D[3 * ii, jj], array2D[3 * ii + 1, jj], array2D[3 * ii + 2, jj]));
                    }
                }
                tex.Apply();
                return tex;
            });

            // from unity to python
            pyFrom = new Dictionary<Type, Func<object, object[]>>();
            // # vector 2 
            pyFrom.Add(typeof(Vector2), value => {
                Vector2 v = (Vector2)value;
                return new object[2] { (int)PyUnity.Vector2, new float[2] { v.x, v.y } };
            });
            // # vector 3
            pyFrom.Add(typeof(Vector3), value => {
                Vector3 v = (Vector3)value;
                return new object[2] { (int)PyUnity.Vector3, new float[3] { v.x, v.y, v.z } };
            });
            // # vector 4
            pyFrom.Add(typeof(Vector4), value => {
                Vector4 v = (Vector4)value;
                return new object[2] { (int)PyUnity.Vector4, new float[4] { v.x, v.y, v.z, v.w } };
            });
            // # color
            pyFrom.Add(typeof(Color), value => {
                Color c = (Color)value;
                return new object[2] { (int)PyUnity.Color, new float[4] { c.r, c.g, c.b, c.a } };
            });
            // # rect
            pyFrom.Add(typeof(Rect), value => {
                Rect r = (Rect)value;
                return new object[2] { (int)PyUnity.Rect, new float[4] { r.x, r.y, r.width, r.height } };
            });
            // # list
            pyFrom.Add(typeof(List<>), value => {

                IList collection = (IList)value;
                if (collection.Count > 0) {
                    Array arr = Array.CreateInstance(collection[0].GetType(), collection.Count);
                    for (int jj = 0; jj < collection.Count; ++jj) {
                        arr.SetValue(collection[jj], jj);
                    }
                    return new object[2] { (int)PyUnity.List1D, arr };
                }
                return null;
            });
            // # string
            pyFrom.Add(typeof(string), value => {
                return new object[2] { (int)PyUnity.String, value };
            });
            // # integer
            pyFrom.Add(typeof(int), value => {
                return new object[2] { (int)PyUnity.Integer, (int)value };
            });
            // # single
            pyFrom.Add(typeof(float), value => {
                return new object[2] { (int)PyUnity.Single, (float)value };
            });
            // # boolean
            pyFrom.Add(typeof(bool), value => {
                return new object[2] { (int)PyUnity.Boolean, (int)(((bool)value) ? 1 : 0) };
            });


        }
    }
}


//pDll = NativeMethods.LoadLibrary(ExVR.Paths().unityMainDir + "/python_export.dll");

//IntPtr pAddressOfFunctionToCall = NativeMethods.GetProcAddress(pDll, "initialize_python_interpreter");
//initInterpreter = (initialize_python_interpreter)Marshal.GetDelegateForFunctionPointer(
//    pAddressOfFunctionToCall, typeof(initialize_python_interpreter));

//pAddressOfFunctionToCall = NativeMethods.GetProcAddress(pDll, "clean_python_interpreter");
//cleanInterpreter = (clean_python_interpreter)Marshal.GetDelegateForFunctionPointer(
//    pAddressOfFunctionToCall, typeof(clean_python_interpreter));
//initInterpreter();

//[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
//private delegate void initialize_python_interpreter();

//[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
//private delegate void clean_python_interpreter();

//static class NativeMethods{

//    [DllImport("kernel32.dll")]
//    public static extern IntPtr LoadLibrary(string dllToLoad);

//    [DllImport("kernel32.dll")]
//    public static extern IntPtr GetProcAddress(IntPtr hModule, string procedureName);

//    [DllImport("kernel32.dll")]
//    public static extern bool FreeLibrary(IntPtr hModule);
//}

//private initialize_python_interpreter initInterpreter;
//private clean_python_interpreter cleanInterpreter;

//private IntPtr pDll;
