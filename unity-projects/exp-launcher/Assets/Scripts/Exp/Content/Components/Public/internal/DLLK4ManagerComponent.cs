/*******************************************************************************
** exvr-exp                                                                   **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

// system
using System;
using System.Runtime.InteropServices;


namespace Ex {

    public class DLLK4ManagerComponent : DLLExComponent {

        public void retrieve_cloud_frame_data(int idCamera, int idCurrentFrame, NativeDLLVertices vertices, IntPtr lastFrameState) {
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

        public DLLK4ManagerComponent() : base() {
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
