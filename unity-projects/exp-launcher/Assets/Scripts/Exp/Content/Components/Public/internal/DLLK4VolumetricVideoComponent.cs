/*******************************************************************************
** exvr-exp                                                                   **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

// system
using System;
using System.Runtime.InteropServices;

// unity
using UnityEngine;

namespace Ex {

    public class DLLK4VolumetricVideoComponent : DLLExComponent {

        public bool uncompress_frame(int idCamera, int idFrame, ref Unity.Collections.NativeArray<Vector3> vertices, ref Unity.Collections.NativeArray<Color> colors) {
            unsafe {
                return uncompress_frame_c4f_k4_volumetric_video_ex_component(
                    _handle,
                    idCamera,
                    idFrame,
                    Unity.Collections.LowLevel.Unsafe.NativeArrayUnsafeUtility.GetUnsafePtr(vertices),
                    Unity.Collections.LowLevel.Unsafe.NativeArrayUnsafeUtility.GetUnsafePtr(colors)
                ) == 1;
            }
        }

        public bool uncompress_frame(int idCamera, int idFrame, ref Unity.Collections.NativeArray<Vector3> vertices, ref Unity.Collections.NativeArray<Color32> colors) {
            unsafe {
                return uncompress_frame_c3i_k4_volumetric_video_ex_component(
                    _handle, 
                    idCamera, 
                    idFrame,
                    Unity.Collections.LowLevel.Unsafe.NativeArrayUnsafeUtility.GetUnsafePtr(vertices),
                    Unity.Collections.LowLevel.Unsafe.NativeArrayUnsafeUtility.GetUnsafePtr(colors)
                ) == 1;
            }
        }

        public bool uncompress_frame(int idCamera, int idFrame, ref Unity.Collections.NativeArray<DLLVertex> vertices) {
            unsafe {
                return uncompress_frame_vmd_k4_volumetric_video_ex_component(
                    _handle,
                    idCamera,
                    idFrame,
                    Unity.Collections.LowLevel.Unsafe.NativeArrayUnsafeUtility.GetUnsafePtr(vertices)
                ) == 1;
            }
        }

        public int process_audio(int idCamera) {
            return process_audio_k4_volumetric_video_ex_component(_handle, idCamera);
        }

        public void copy_audio_samples(int idCamera, float[] audio) {
            unsafe {
                fixed (float* audioP = audio) {
                    copy_audio_samples_k4_volumetric_video_ex_component(_handle, idCamera, audioP);
                }
            }
        }


        #region memory_management

        /// <summary>
        /// VolumetricVideoComponentDLL default constructor
        /// </summary>
        public DLLK4VolumetricVideoComponent(HandleRef volumetricVideoResource) : base() {
            _handle = new HandleRef(this, create_k4_volumetric_video_ex_component(volumetricVideoResource));
        }

        /// <summary>
        /// Allocate DLL memory
        /// </summary>
        protected override void create_DLL_class() {
        }

        #endregion memory_management    
        #region DllImport

        //memory management
        [DllImport("exvr-export", EntryPoint = "create_k4_volumetric_video_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static private extern IntPtr create_k4_volumetric_video_ex_component(HandleRef volumetricVideoComponent);


        [DllImport("exvr-export", EntryPoint = "uncompress_frame_c4f_k4_volumetric_video_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static unsafe private extern int uncompress_frame_c4f_k4_volumetric_video_ex_component(
            HandleRef vvc, int idCamera, int idFrame, void* vertices, void* colors);

        [DllImport("exvr-export", EntryPoint = "uncompress_frame_c3i_k4_volumetric_video_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static unsafe private extern int uncompress_frame_c3i_k4_volumetric_video_ex_component(
            HandleRef vvc, int idCamera, int idFrame, void* vertices, void* colors);

        [DllImport("exvr-export", EntryPoint = "uncompress_frame_vmd_k4_volumetric_video_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static unsafe private extern int uncompress_frame_vmd_k4_volumetric_video_ex_component(
            HandleRef vvc, int idCamera, int idFrame, void* vertices);

        [DllImport("exvr-export", EntryPoint = "process_audio_k4_volumetric_video_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static private extern int process_audio_k4_volumetric_video_ex_component(
            HandleRef vvc, int idCamera);

        [DllImport("exvr-export", EntryPoint = "copy_audio_samples_k4_volumetric_video_ex_component", CallingConvention = CallingConvention.Cdecl)]
        static unsafe private extern void copy_audio_samples_k4_volumetric_video_ex_component(
            HandleRef vvc, int idCamera, float* audioSamples);

        #endregion DllImport        
    }
}
