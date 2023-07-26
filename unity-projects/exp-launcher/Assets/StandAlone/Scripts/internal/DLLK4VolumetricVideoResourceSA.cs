
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

// unity
using UnityEngine;

namespace SA {

    public class DLLK4VolumetricVideoResourceSA : DLLExResourceSA {

        public int nb_cameras() {
            return get_cameras_nb_k4_volumetric_video_ex_resource(_handle);
        }

        public int nb_frames(int idCamera) {
            return get_nb_frames_k4_volumetric_video_ex_resource(_handle, idCamera);
        }

        public float duration_ms(int idCamera) {
            return get_duration_ms_k4_volumetric_video_ex_resource(_handle, idCamera);
        }

        public Matrix4x4 model(int idCamera) {

            float[] t = new float[16];
            get_camera_transform_k4_volumetric_video_ex_resource(_handle, idCamera, t);
            return new Matrix4x4(new Vector4(t[0], t[1], t[2], 0),     // c0
                                 new Vector4(t[4], t[5], t[6], 0),     // c1
                                 new Vector4(t[8], t[9], t[10], 0),    // c2
                                 new Vector4(t[12], t[13], t[14], 1)); // c3
        }

        public int id_frame_from_time(int idCamera, float timeMs) {
            return get_id_frame_from_time_ms_k4_volumetric_video_ex_resource(_handle, idCamera, timeMs);
        }

        public int valid_vertices_count(int idCamera, int idFrame) {
            return get_valid_vertices_count_k4_volumetric_video_ex_resource(_handle, idCamera, idFrame);
        }

        public int audio_data_total_size(int idCamera) {
            return get_audio_data_total_size_k4_volumetric_video_ex_resource(_handle, idCamera);
        }


        #region memory_management

        public DLLK4VolumetricVideoResourceSA() : base() {
        }

        protected override void create_DLL_class() {
            _handle = new HandleRef(this, create_k4_volumetric_video_ex_resource());
        }

        #endregion memory_management  
        #region DllImport

        [DllImport("exvr-export", EntryPoint = "create_k4_volumetric_video_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern IntPtr create_k4_volumetric_video_ex_resource();

        [DllImport("exvr-export", EntryPoint = "get_cameras_nb_k4_volumetric_video_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int get_cameras_nb_k4_volumetric_video_ex_resource(HandleRef volumetricVideoResource);

        [DllImport("exvr-export", EntryPoint = "get_nb_frames_k4_volumetric_video_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int get_nb_frames_k4_volumetric_video_ex_resource(HandleRef volumetricVideoResource, int idCamera);

        [DllImport("exvr-export", EntryPoint = "get_duration_ms_k4_volumetric_video_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern float get_duration_ms_k4_volumetric_video_ex_resource(HandleRef volumetricVideoResource, int idCamera);

        [DllImport("exvr-export", EntryPoint = "get_camera_transform_k4_volumetric_video_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern void get_camera_transform_k4_volumetric_video_ex_resource(HandleRef volumetricVideoResource, int idCamera, float[] model);

        [DllImport("exvr-export", EntryPoint = "get_id_frame_from_time_ms_k4_volumetric_video_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int get_id_frame_from_time_ms_k4_volumetric_video_ex_resource(HandleRef volumetricVideoResource, int idCamera, float timeMs);

        [DllImport("exvr-export", EntryPoint = "get_valid_vertices_count_k4_volumetric_video_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int get_valid_vertices_count_k4_volumetric_video_ex_resource(HandleRef volumetricVideoResource, int idCamera, int idFrame);

        [DllImport("exvr-export", EntryPoint = "get_audio_data_total_size_k4_volumetric_video_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int get_audio_data_total_size_k4_volumetric_video_ex_resource(HandleRef volumetricVideoResource, int idCamera);

        #endregion DllImport  
    }
}



