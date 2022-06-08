
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

// unity
using UnityEngine;

namespace Ex {

    public class DLLScanerVideoResource : CppDllImport {

        private float[] jointsPositions = new float[Kinect2.nbJoints * 3];
        private float[] jointsEulerRotations = new float[Kinect2.nbJoints * 3];
        private int[] jointsStates = new int[Kinect2.nbJoints];

        public DLLScanerVideoResource() : base() {
        }

        public bool load(string path) {
            return load_scaner_video_resource(_handle, path) == 1;
        }

        public int nb_cameras() {
            return get_camera_nb_scaner_video_resource(_handle);
        }

        public int camera_cloud_size(int idCamera) {
            return get_camera_cloud_size_video_resource(_handle, idCamera);
        }

        public int duration_ms() {
            return get_duration_ms_scaner_video_resource(_handle);
        }

        public int nb_bodies(int idCamera) {
            return get_nb_bodies_scaner_video_resource(_handle, idCamera);
        }

        public bool is_body_tracked(int idCamera, int idBody) {
            return is_body_tracked_scaner_video_resource(_handle, idCamera, idBody) == 1;
        }
        public bool is_body_restricted(int idCamera, int idBody) {
            return is_body_restricted_scaner_video_resource(_handle, idCamera, idBody) == 1;
        }

        public bool is_hand_detected(int idCamera, int idBody, bool leftHand) {
            return is_body_hand_detected_scaner_video_resource(_handle, idCamera, idBody, leftHand ? 1 : 0) == 1;
        }

        public bool is_hand_confident(int idCamera, int idBody, bool leftHand) {
            return is_body_hand_confident_scaner_video_resource(_handle, idCamera, idBody, leftHand ? 1 : 0) == 1;
        }

        public void update_joints(int idCamera, int idBody, Dictionary<Kinect2.BodyJointType, JointInfo> bodyJoints) {

            body_joints_positions_scaner_video_resource(_handle, idCamera, idBody, jointsPositions);
            body_joints_rotations_scaner_video_resource(_handle, idCamera, idBody, jointsEulerRotations);
            body_joints_states_scaner_video_resource(_handle, idCamera, idBody, jointsStates);

            for (int ii = 0; ii < Kinect2.nbJoints; ++ii) {
                var type = (Kinect2.BodyJointType)ii;
                bodyJoints[type].position = new Vector3(jointsPositions[ii * 3], jointsPositions[ii * 3 + 1], jointsPositions[ii * 3 + 2]);
                bodyJoints[type].rotation = Quaternion.Euler(jointsEulerRotations[ii * 3], jointsEulerRotations[ii * 3 + 1], jointsEulerRotations[ii * 3 + 2]);
                bodyJoints[type].state = (Kinect2.TrackingState)jointsStates[ii];
            }
        }


        public Matrix4x4 camera_model(int idCamera) {

            float[] t = new float[16];
            get_cloud_model_transform_scaner_video_resource(_handle, idCamera, t);
            return new Matrix4x4(new Vector4(t[0], t[1], t[2], 0),  // c0
                                    new Vector4(t[4], t[5], t[6], 0),  // c1
                                    new Vector4(t[8], t[9], t[10], 0),  // c2
                                    new Vector4(t[12], t[13], t[14], 1)); // c3

        }

        public int update_cloud_data(int idCamera, int timeMs, int maxDiffTimeMs, bool loop, IntPtr vertices, IntPtr colors) {
            return update_cloud_data_scaner_video_resource(_handle, idCamera, timeMs, maxDiffTimeMs, loop ? 1 : 0, vertices, colors);
        }


        #region memory_management

        protected override void create_DLL_class() {
            _handle = new HandleRef(this, create_scaner_video_resource());
        }

        protected override void delete_DLL_class() {
            delete_scaner_video_resource(_handle);
        }


        #endregion memory_management    
        #region DllImport

        // memory management
        [DllImport("lnco-exvr-export", EntryPoint = "create_scaner_video_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern IntPtr create_scaner_video_resource();

        [DllImport("lnco-exvr-export", EntryPoint = "delete_scaner_video_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern void delete_scaner_video_resource(HandleRef scanerVideoResource);

        [DllImport("lnco-exvr-export", EntryPoint = "load_scaner_video_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int load_scaner_video_resource(HandleRef scanerVideoResource, string pathVideoFile);

        [DllImport("lnco-exvr-export", EntryPoint = "get_duration_ms_scaner_video_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int get_duration_ms_scaner_video_resource(HandleRef scanerVideoResource);

        [DllImport("lnco-exvr-export", EntryPoint = "get_camera_nb_scaner_video_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int get_camera_nb_scaner_video_resource(HandleRef scanerVideoResource);

        [DllImport("lnco-exvr-export", EntryPoint = "get_camera_cloud_size_video_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int get_camera_cloud_size_video_resource(HandleRef scanerVideoResource, int idC);

        [DllImport("lnco-exvr-export", EntryPoint = "get_cloud_model_transform_scaner_video_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern void get_cloud_model_transform_scaner_video_resource(HandleRef scanerVideoResource, int idC, float[] model);

        [DllImport("lnco-exvr-export", EntryPoint = "update_cloud_data_scaner_video_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int update_cloud_data_scaner_video_resource(HandleRef scanerVideoResource, int idC, int timeMs, int maxDiffTimeMs, int loop, IntPtr vertices, IntPtr colors);

        [DllImport("lnco-exvr-export", EntryPoint = "get_nb_bodies_scaner_video_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int get_nb_bodies_scaner_video_resource(HandleRef scanerVideoResource, int idC);

        [DllImport("lnco-exvr-export", EntryPoint = "is_body_tracked_scaner_video_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int is_body_tracked_scaner_video_resource(HandleRef scanerVideoResource, int idC, int idBody);

        [DllImport("lnco-exvr-export", EntryPoint = "is_body_restricted_scaner_video_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int is_body_restricted_scaner_video_resource(HandleRef scanerVideoResource, int idC, int idBody);

        [DllImport("lnco-exvr-export", EntryPoint = "is_body_hand_detected_scaner_video_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int is_body_hand_detected_scaner_video_resource(HandleRef scanerVideoResource, int idC, int idBody, int leftHand);

        [DllImport("lnco-exvr-export", EntryPoint = "is_body_hand_confident_scaner_video_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int is_body_hand_confident_scaner_video_resource(HandleRef scanerVideoResource, int idC, int idBody, int leftHand);

        [DllImport("lnco-exvr-export", EntryPoint = "body_joints_positions_scaner_video_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern void body_joints_positions_scaner_video_resource(HandleRef scanerVideoResource, int idC, int idBody, float[] positions);

        [DllImport("lnco-exvr-export", EntryPoint = "body_joints_rotations_scaner_video_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern void body_joints_rotations_scaner_video_resource(HandleRef scanerVideoResource, int idC, int idBody, float[] rotations);

        [DllImport("lnco-exvr-export", EntryPoint = "body_joints_states_scaner_video_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern void body_joints_states_scaner_video_resource(HandleRef scanerVideoResource, int idC, int idBody, int[] states);

        #endregion DllImport        

    }
}