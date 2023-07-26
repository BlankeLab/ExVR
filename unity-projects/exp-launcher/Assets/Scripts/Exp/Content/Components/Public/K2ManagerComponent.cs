/*******************************************************************************
** exvr-exp                                                                   **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

// system
using System.Collections.Generic;
using System.Threading.Tasks;

// unity
using UnityEngine;
using UnityEngine.Profiling;

namespace Ex {

    public class K2ManagerComponent : CppExComponent {

        private List<K2CameraData> m_camerasData = new List<K2CameraData>();
        private List<int> commonIndices = new List<int>();
        private int m_updateCounter = 0;
        private int m_nbCameras = 0;
        private int m_mode = 0;
        public bool networkConfigFileLoaded = false;
        public bool calibrationConfigFileLoaded = false;
        public bool cameraConfigFileLoaded = false;
        public HashSet<int> idToUse = null;

        // debug
        private static readonly string debugM0 = "Start parallel update from cameras";
        private static readonly string debugM1 = "update_cloud_kinect_manager_component";
        private static readonly string debugM2 = "update_bodies_kinect_manager_component";
        private static readonly string debugM3 = "update_mesh_kinect_manager_component";
        private static readonly string debugM4 = "End parallel update from cameras";

        #region ex_functions
        protected override bool initialize() {

            // debug
            if (initC.get<bool>("debug_bypass")) {
                log_warning("Debug bypass enabled, no cloud will be generated.");
            }

            // get resources files
            string networkA = initC.get_resource_alias("config");
            string calibA   = initC.get_resource_alias("calib");
            string cameraA  = initC.get_resource_alias("camera");
  
            if(networkA.Length == 0) {
                log_error("No network config file defined.");
                send_infos_to_gui_init_config("config_infos", "0%0%0%0");
                return false;
            }
            string pathNetwork = ExVR.Resources().get_text_file_data(networkA).path;

            string pathCalib = "";
            if (calibA.Length > 0) {
                var pathCalibData = ExVR.Resources().get_text_file_data(calibA);
                if(pathCalibData != null) {
                    pathCalib = pathCalibData.path;
                }
            }
            string pathCamera = "";
            if (cameraA.Length > 0) {
                var cameraPathData = ExVR.Resources().get_text_file_data(cameraA);
                if (cameraPathData != null) {
                    pathCamera = cameraPathData.path;
                }
            }

            // init DLL
            cppDll = new DLLK2ManagerComponent();
            cppDll.parent = this;
            cppDll.set(Parameters.Container.Dynamic, "path_network", pathNetwork);
            cppDll.set(Parameters.Container.Dynamic, "path_calib",  pathCalib);
            cppDll.set(Parameters.Container.Dynamic, "path_camera", pathCamera);
            cppDll.set(Parameters.Container.Dynamic, "fps", 30);
            //cppDll.set(Parameters.Container.Dynamic, "disable_grabber_display", initC.get<bool>("disable_grabber_display"));

            bool success = cppDll.initialize();
            m_mode                      = initC.get<int>("mode");
            m_nbCameras                 = cppDll.get<int>(Parameters.Container.Dynamic, "grabbers_count");
            networkConfigFileLoaded     = cppDll.get<int>(Parameters.Container.Dynamic, "network_config_file_loaded")== 1;
            calibrationConfigFileLoaded = cppDll.get<int>(Parameters.Container.Dynamic, "calibration_config_file_loaded") == 1;
            cameraConfigFileLoaded      = cppDll.get<int>(Parameters.Container.Dynamic, "camera_config_file_loaded") == 1;

            send_infos_to_gui_init_config("config_infos", string.Format("{0}%{1}%{2}%{3}",
                m_nbCameras,
                networkConfigFileLoaded ? "1" : "0",
                calibrationConfigFileLoaded ? "1" : "0",
                cameraConfigFileLoaded ? "1" : "0"));

            if (!success) {
                clean();
                log_error("Cannot initialize kinect manager dll.");
                return false;
            }


            if (m_nbCameras <= 0) {
                return false;
            }

            // init common data
            commonIndices = new List<int>(K2.depthCount * 3);
            for (int ii = 0; ii < K2.depthCount * 3; ++ii) {
                commonIndices.Add(ii);
            }

            // init cameras data            
            for (int ii = 0; ii < m_nbCameras; ++ii) {
                
                // set calibration matrix
                float[] t = cppDll.get_array<float>(Parameters.Container.Dynamic, "tr_" + ii);
                var calibMatrix = new Matrix4x4(
                    new Vector4(t[0], t[1], t[2], 0),   // c0
                    new Vector4(t[4], t[5], t[6], 0),   // c1
                    new Vector4(t[8], t[9], t[10], 0),  // c2
                    new Vector4(t[12], t[13], t[14], 1) // c3
                );

                // add kinect camera data
                m_camerasData.Add(new K2CameraData(ii, calibMatrix, commonIndices));
            }

            idToUse = new HashSet<int>();
            foreach(var idStr in Ex.Text.split(initC.get<string>("cameras_to_use"), ";", removeEmptyLines : true)) {
                int id = Converter.to<int>(idStr);
                if (idToUse.Contains(id)) {
                    log_warning(string.Format("Grabber id [{0}] already defined in the list. ", id));
                } else {
                    if(id < m_nbCameras) {
                        idToUse.Add(id);
                    } else {
                        log_warning(string.Format("Cannot use id [{0}], there is only [{1}] grabbers available. ", id, m_nbCameras));
                    }                    
                }
            }

            if(idToUse.Count == 0) {
                log_warning("No grabber id defined.");
            }


            return true;
        }

        protected override void clean() {

            foreach (var cameraData in m_camerasData) {
                cameraData.clean();
            }
            base.clean();
        }


        protected override void start_routine() {

            m_updateCounter = 0;
            base.start_routine();
        }

        protected override void pre_update() {

            // retrieve current mode
            m_mode = initC.get<int>("mode");

            // update cameras?
            if (!currentC.get<bool>("update_cameras")) {
                return;
            }

            // check last frame time
            int fps = currentC.get<int>("fps");

            var currTime = time().ellapsed_element_ms();
            double deltaTime = 1000.0 / fps;
            if (currTime / deltaTime > m_updateCounter) {
                ++m_updateCounter;
                k2_dll().ask_for_frame();
            }

            // get data
            Profiler.BeginSample("[ExVR][KinectManagerComponent] update_data_from_cameras");
            add_message_to_stack_trace("Update data from cameras");
            update_data_from_cameras();
            add_message_to_stack_trace("End update data from cameras");
            Profiler.EndSample();
        }

        #endregion

        #region private_functions

        private DLLK2ManagerComponent k2_dll() {
            return (DLLK2ManagerComponent)cppDll;
        }

        private void update_data_from_cameras() {

            int maxDiffTime = 100;
            if (currentC.has("max_diff_time")) {
                maxDiffTime = currentC.get<int>("max_diff_time");
            }

            // get current frames
            int idCamera = 0;
            foreach (var cameraData in m_camerasData) {

                string frameIdStr    = string.Format("id_frame_{0}", cameraData.idCamera);
                string timeIdStr     = string.Format("id_time_{0}", cameraData.idCamera);
                string timeIdffIdStr = string.Format("id_diff_time_{0}", cameraData.idCamera);

                if (!idToUse.Contains(idCamera++)) {
                    cameraData.doUpdate = false;
                    cameraData.doDisplay = false;
                    continue;
                }

                if (!cppDll.contains(Parameters.Container.Dynamic, frameIdStr) || !cppDll.contains(Parameters.Container.Dynamic, timeIdStr)) {
                    cameraData.doUpdate = false;
                    continue;
                }

                int idFrame = cppDll.get<int>(Parameters.Container.Dynamic, frameIdStr);
                int time    = cppDll.get<int>(Parameters.Container.Dynamic, timeIdStr);

                // check time
                if (cppDll.contains(Parameters.Container.Dynamic, timeIdffIdStr)) {
                    cameraData.diffTime = cppDll.get<int>(Parameters.Container.Dynamic, timeIdffIdStr);
                    if (cameraData.diffTime > maxDiffTime) {
                        // frame too old
                        cameraData.doUpdate = false;
                        cameraData.doDisplay = false;
                        continue;
                    } else {
                        cameraData.doDisplay = true;
                    }
                }

                // check id frame
                if (idFrame != cameraData.idFrame) {
                    cameraData.idFrame = idFrame;
                    cameraData.doUpdate = true;
                } else {
                    cameraData.doUpdate = false;
                }

                // add framerate info?
                // ...
            }

            ExVR.ExpLog().push_to_strackTrace(new ComponentTrace(debugM0));

            // do update
            if (m_mode == 0) {
                Parallel.For(0, m_nbCameras, idCam => {

                    var camD = m_camerasData[idCam];

                    if (camD.doUpdate) {

                        // cloud
                        ExVR.ExpLog().push_to_strackTrace(new ComponentTrace(debugM1));
                        k2_dll().update_cloud(idCam, camD.gcVertices.AddrOfPinnedObject(), camD.gcColors.AddrOfPinnedObject());
                        camD.sizePts = cppDll.get<int>(Parameters.Container.Dynamic, "nb_pts_" + idCam);
                        camD.sizeTris = cppDll.get<int>(Parameters.Container.Dynamic, "nb_tris_" + idCam);


                        ////send_infos_to_gui_current_config("frame_id")

                        // bodies
                        ExVR.ExpLog().push_to_strackTrace(new ComponentTrace(debugM2));
                        k2_dll().update_bodies(
                            idCam,
                            camD.gcBodiesInfo.AddrOfPinnedObject(),
                            camD.gcJointsId.AddrOfPinnedObject(),
                            camD.gcJointsState.AddrOfPinnedObject(),
                            camD.gcJointsPos.AddrOfPinnedObject(),
                            camD.gcJointsRot.AddrOfPinnedObject()
                        );

                        for (int idBody = 0; idBody < K2.nbBodies; ++idBody) {

                            if (camD.bodiesInfo[3 * idBody + 0] == -1 || camD.bodiesInfo[3 * idBody + 1] == -1) {
                                camD.bodiesId[idBody] = -1;
                            } else {
                                uint u1 = (uint)camD.bodiesInfo[3 * idBody + 0];
                                uint u2 = (uint)camD.bodiesInfo[3 * idBody + 1];
                                camD.bodiesId[idBody] = ((long)u1 << 32) | u2;
                            }

                            camD.bodiesTracked[idBody] = camD.bodiesInfo[3 * idBody + 2] == 1 ? true : false;
                            camD.bodiesRestricted[idBody] = camD.bodiesInfo[3 * idBody + 3] == 1 ? true : false;
                        }
                    }
                });
            } else {
                Parallel.For(0, m_nbCameras, idCam => {

                    var camD = m_camerasData[idCam];

                    if (camD.doUpdate) {

                        // mesh
                        ExVR.ExpLog().push_to_strackTrace(new ComponentTrace(debugM3));
                        k2_dll().update_mesh(
                            idCam, 
                            camD.gcVertices.AddrOfPinnedObject(), 
                            camD.gcColors.AddrOfPinnedObject(), 
                            camD.gcIdTris.AddrOfPinnedObject()
                        );
                        camD.sizePts  = cppDll.get<int>(Parameters.Container.Dynamic, "nb_pts_" + idCam);
                        camD.sizeTris = cppDll.get<int>(Parameters.Container.Dynamic, "nb_tris_" + idCam);

                        // bodies
                        ExVR.ExpLog().push_to_strackTrace(new ComponentTrace(debugM2));
                        k2_dll().update_bodies(
                            idCam,
                            camD.gcBodiesInfo.AddrOfPinnedObject(),
                            camD.gcJointsId.AddrOfPinnedObject(),
                            camD.gcJointsState.AddrOfPinnedObject(),
                            camD.gcJointsPos.AddrOfPinnedObject(),
                            camD.gcJointsRot.AddrOfPinnedObject()
                        );

                        for (int idBody = 0; idBody < K2.nbBodies; ++idBody) {

                            if (camD.bodiesInfo[3 * idBody + 0] == -1 || camD.bodiesInfo[3 * idBody + 1] == -1) {
                                camD.bodiesId[idBody] = -1;
                            } else {
                                uint u1 = (uint)camD.bodiesInfo[3 * idBody + 0];
                                uint u2 = (uint)camD.bodiesInfo[3 * idBody + 1];
                                camD.bodiesId[idBody] = ((long)u1 << 32) | u2;
                            }

                            camD.bodiesTracked[idBody] = camD.bodiesInfo[3 * idBody + 2] == 1 ? true : false;
                            camD.bodiesRestricted[idBody] = camD.bodiesInfo[3 * idBody + 3] == 1 ? true : false;
                        }

                        //for (int idBody = 0; idBody < Kinect2.nbBodies; ++idBody) {
                        //    camD.bodiesId[idBody] = camD.bodiesInfo[3 * idBody + 0];
                        //    camD.bodiesTracked[idBody] = camD.bodiesInfo[3 * idBody + 0] == 1 ? true : false;
                        //    camD.bodiesRestricted[idBody] = camD.bodiesInfo[3 * idBody + 0] == 1 ? true : false;
                        //    //camD.bodiesTracked[idBody] = get_parameter_int_ex_component(_handle, 2, "bodies_" + idCam + "_tracked_" + idBody) == 1;
                        //    //camD.bodiesId[idBody] = get_parameter_int_ex_component(_handle, 2, "bodies_" + idCam + "_id_" + idBody);
                        //}
                    }
                });
            }

            // send infos
            for(int ii = 0; ii < m_nbCameras; ++ii) {
                var camD = m_camerasData[ii];
                if (camD.doUpdate) {
                    send_infos_to_gui_current_config("frame_info", string.Format("{0}%{1}%{2}%{3}%{4}",
                        ii, camD.idFrame, camD.diffTime, camD.sizePts, camD.sizeTris));
                }
            }
            

            ExVR.ExpLog().push_to_strackTrace(new ComponentTrace(debugM4));
        }

        #endregion

        #region public_functions

        public int cameras_nb() {
            return m_nbCameras;
        }

        public int mode() {
            return m_mode;
        }

        public K2CameraData camera_data(int idCamera) {
            if (cppDll != null) {
                return m_camerasData[idCamera];
            }
            return null;
        }

        public Matrix4x4 calibration_matrix(int idCamera) {
            if (cppDll != null) {
                return m_camerasData[idCamera].calibMatrix;
            }
            return Matrix4x4.identity;          
        }

        #endregion
    }
}
