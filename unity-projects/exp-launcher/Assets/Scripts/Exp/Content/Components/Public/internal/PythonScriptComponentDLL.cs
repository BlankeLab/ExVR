

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
using System.Runtime.InteropServices;


namespace Ex.DLL{

    public class PythonScriptComponentDLL : DLLExComponent{


        #region memory_management

        /// <summary>
        /// PythonComponentDLL default constructor
        /// </summary>
        public PythonScriptComponentDLL() : base() {
        }

        /// <summary>
        /// Allocate DLL memory
        /// </summary>
        protected override void create_DLL_class() {
            _handle = new HandleRef(this, create_python_script_component());
        }

        /// <summary>
        /// Clean DLL memory
        /// </summary>
        protected override void delete_DLL_class() {
            delete_python_script_component(_handle);
        }


        #endregion memory_management    
        #region DllImport

        //// memory management
        [DllImport("exvr-export", EntryPoint = "create_python_script_component", CallingConvention = CallingConvention.Cdecl)]
        static private extern IntPtr create_python_script_component();

        [DllImport("exvr-export", EntryPoint = "delete_python_script_component", CallingConvention = CallingConvention.Cdecl)]
        static private extern void delete_python_script_component(HandleRef pyScript);

        #endregion DllImport        
    }
}
