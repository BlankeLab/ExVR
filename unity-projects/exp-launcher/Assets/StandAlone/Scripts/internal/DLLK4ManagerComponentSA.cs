

/************************************************************************************
** K4DisplayStandAloneSample                                                       **
**                                                                                 **
** MIT License                                                                     **
**                                                                                 **
** Copyright (c) 2023 Florian Lance                                                **
**                                                                                 **
** Permission is hereby granted, free of charge, to any person obtaining a copy    **
** of this software and associated documentation files (the "Software"), to deal   **
** in the Software without restriction, including without limitation the rights    **
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       **
** copies of the Software, and to permit persons to whom the Software is           **
** furnished to do so, subject to the following conditions:                        **
**                                                                                 **
** The above copyright notice and this permission notice shall be included in all  **
** copies or substantial portions of the Software.                                 **
**                                                                                 **
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      **
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        **
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     **
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          **
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   **
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   **
** SOFTWARE.                                                                       **
*************************************************************************************/


// system
using System;
using System.Runtime.InteropServices;

namespace SA {


    public class DLLK4ManagerComponentSA : DLLExComponentSA {

        public void retrieve_cloud_frame_data(int idCamera, int idCurrentFrame, NativeDLLVerticesSA vertices, IntPtr lastFrameState) {
            unsafe {
                retrieve_cloud_frame_data_k4_manager_ex_component(
                    _handle,
                    idCamera,
                    idCurrentFrame,
                    Unity.Collections.LowLevel.Unsafe.NativeArrayUnsafeUtility.GetUnsafePtr(vertices.native),
                    lastFrameState
                );
            }
        }

        #region memory_management

        public DLLK4ManagerComponentSA() : base() {
        }
        protected override void create_DLL_class() {
            _handle = new HandleRef(this, create_k4_manager_ex_component());
        }

        #endregion memory_management    
        #region DllImport

        // memory management
        [DllImport("exvr-export", EntryPoint = "create_k4_manager_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static private extern IntPtr create_k4_manager_ex_component();

        [DllImport("exvr-export", EntryPoint = "retrieve_cloud_frame_data_k4_manager_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static unsafe private extern void retrieve_cloud_frame_data_k4_manager_ex_component(
        HandleRef k4m, int idCamera, int idCurrentFrame, void* vertices, IntPtr lastFrameState);


        #endregion DllImport        
    }


}