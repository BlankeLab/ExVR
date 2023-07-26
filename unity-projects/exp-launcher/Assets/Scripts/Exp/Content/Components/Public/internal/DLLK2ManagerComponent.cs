/*******************************************************************************
** exvr-exp                                                                   **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

// system
using System;
using System.Runtime.InteropServices;

namespace Ex {

    public class DLLK2ManagerComponent : DLLExComponent {

        public void ask_for_frame() {
            ask_for_frame_k2_manager_ex_component(_handle);
        }
        public int update_cloud(int idC, IntPtr vertices, IntPtr colors) {
            return update_cloud_k2_manager_ex_component(_handle, idC, vertices, colors);
        }
        public void update_mesh(int idC, IntPtr vertices, IntPtr colors, IntPtr idTris) {
            update_mesh_k2_manager_ex_component(_handle, idC, vertices, colors, idTris);
        }

        public void update_bodies(int idC, IntPtr bodiesInfo, IntPtr jointsType, IntPtr jointsState, IntPtr jointsPosition, IntPtr jointsRotation) {
            update_bodies_k2_manager_ex_component(_handle, idC, bodiesInfo, jointsType, jointsState, jointsPosition, jointsRotation);
        }


        #region memory_management


        public DLLK2ManagerComponent() : base() {
        }
        protected override void create_DLL_class() {
            _handle = new HandleRef(this, create_k2_manager_ex_component());
        }

        #endregion memory_management    
        #region DllImport

        // memory management
        [DllImport("exvr-export", EntryPoint = "create_k2_manager_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static private extern IntPtr create_k2_manager_ex_component();

        [DllImport("exvr-export", EntryPoint = "update_cloud_k2_manager_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static private extern int update_cloud_k2_manager_ex_component(HandleRef kinectManagerComponent, int idC, IntPtr vertices, IntPtr colors);

        [DllImport("exvr-export", EntryPoint = "update_mesh_k2_manager_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static private extern void update_mesh_k2_manager_ex_component(HandleRef kinectManagerComponent, int idC, IntPtr vertices, IntPtr colors, IntPtr idTris);

        [DllImport("exvr-export", EntryPoint = "update_bodies_k2_manager_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static private extern void update_bodies_k2_manager_ex_component(HandleRef kinectManagerComponent, int idC, IntPtr bodiesInfo, IntPtr jointsType, IntPtr jointsState, IntPtr jointsPosition, IntPtr jointsRotation);

        [DllImport("exvr-export", EntryPoint = "ask_for_frame_k2_manager_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static private extern void ask_for_frame_k2_manager_ex_component(HandleRef kinectManagerComponent);


        #endregion DllImport        
    }
}
