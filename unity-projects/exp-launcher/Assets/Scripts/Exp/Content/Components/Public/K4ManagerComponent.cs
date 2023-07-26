/*******************************************************************************
** exvr-exp                                                                   **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

// system
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

// unity
using UnityEngine;
using UnityEngine.Profiling;
using Unity.Collections;
using UnityEngine.Rendering;
using JetBrains.Annotations;

namespace Ex {

    public class K4GrabberData {

        public int id = 0;
        public bool connected = false;

        public int currentFrameId = -1;
        public int nbVertices = 0;
        public bool update = false;

        public NativeDLLVertices vertices = null;

        public GCHandle lastFrameStateH;
        public int[] lastFrameState = null; // success (1/0) | lastFrameId | nbVertices        
        public Matrix4x4 model = Matrix4x4.identity;
    }


    public class K4ManagerComponent : CppExComponent {

        private int m_nbConnections             = 0;
        private bool m_networkFileLoaded        = false;
        private bool m_deviceConfigFileLoaded   = false;
        private bool m_filtersFileLoaded        = false;
        private bool m_colorFileLoaded          = false;
        private bool m_modelFileLoaded          = false;
        private List<K4GrabberData> m_grabbersData = null;
        private NativeIndices indices = null;
        public HashSet<int> idToUse = null;

        #region ex_functions
        protected override bool initialize() {

            // debug
            if (initC.get<bool>("debug_bypass")) {
                log_warning("Debug bypass enabled, no cloud will be generated.");
            }

            add_slot("set delay (ms)", (value) => {
                int delay = Mathf.Clamp((int)value, 0, 5000);
                cppDll.set(Parameters.Container.Dynamic, "delay", delay);
                cppDll.update_from_current_config();
            });

            var networkFile = ExVR.Resources().get_text_file_data(initC.get_resource_alias("network_settings"), false);
            if(networkFile == null) {
                log_error("No network config file defined.");
                return false;
            }

            // init DLL
            try {
                cppDll = new DLLK4ManagerComponent();
            } catch (System.Exception exception) {
                log_error(string.Format("Cannot init K4Manager DLL, error: {0}", exception.Message));
                cppDll = null;
                return false;
            }

            cppDll.parent = this;
            cppDll.set(Parameters.Container.Dynamic, "path_network_settings_file", networkFile.path);

            var deviceFile = ExVR.Resources().get_text_file_data(initC.get_resource_alias("device_settings"), false);
            if (deviceFile != null) {
                cppDll.set(Parameters.Container.Dynamic, "path_device_settings_file", deviceFile.path);
            }
            var filtersFile = ExVR.Resources().get_text_file_data(initC.get_resource_alias("filters"), false);
            if (filtersFile != null) {
                cppDll.set(Parameters.Container.Dynamic, "path_filters_file", filtersFile.path);
            }
            var colorFile = ExVR.Resources().get_text_file_data(initC.get_resource_alias("color_settings"), false);
            if (colorFile != null) {
                cppDll.set(Parameters.Container.Dynamic, "path_color_settings_file", colorFile.path);
            }
            var modelsFile = ExVR.Resources().get_text_file_data(initC.get_resource_alias("models"), false);
            if (modelsFile != null) {
                cppDll.set(Parameters.Container.Dynamic, "path_model_file", modelsFile.path);
            }

            bool success = cppDll.initialize();
            if (!success) {
                log_error("Cannot initialize k4 manager dll.");
                clean();
                return false;
            }

            m_nbConnections             = cppDll.get<int>(Parameters.Container.Dynamic, "nb_connections");
            m_networkFileLoaded         = cppDll.get<int>(Parameters.Container.Dynamic, "network_settings_file_loaded") == 1;
            m_deviceConfigFileLoaded    = cppDll.get<int>(Parameters.Container.Dynamic, "device_settings_file_loaded") == 1;
            m_filtersFileLoaded         = cppDll.get<int>(Parameters.Container.Dynamic, "filters_file_loaded") == 1;
            m_colorFileLoaded           = cppDll.get<int>(Parameters.Container.Dynamic, "color_settings_file_loaded") == 1;
            m_modelFileLoaded           = cppDll.get<int>(Parameters.Container.Dynamic, "models_file_loaded") == 1;

            send_infos_to_gui_init_config("config_infos", string.Format("{0}%{1}%{2}%{3}%{4}%{5}",
                m_nbConnections,
                m_networkFileLoaded ? "1" : "0",
                m_deviceConfigFileLoaded ? "1" : "0",
                m_filtersFileLoaded ? "1" : "0",
                m_colorFileLoaded ? "1" : "0",
                m_modelFileLoaded ? "1" : "0"
            ));

            if (m_nbConnections == 0) {
                log_error("No connection defined in network file.");
                return false;
            }

            log_message("device loaded-> " + cppDll.get<int>(Parameters.Container.InitConfig, "device_init_file"));
            log_message("device loaded-> " + cppDll.get<int>(Parameters.Container.Dynamic, "device_settings_file_loaded"));


            // generate indices
            int maxNbVertices = 2000000;
            indices = new NativeIndices(maxNbVertices);

            // init grabbers info
            m_grabbersData = new List<K4GrabberData>(m_nbConnections);
            for (int ii = 0; ii < m_nbConnections; ++ii) {
                K4GrabberData gData = new K4GrabberData();
                gData.id = ii;
                gData.connected = false;
                gData.lastFrameState = new int[3];
                gData.lastFrameStateH = GCHandle.Alloc(gData.lastFrameState, GCHandleType.Pinned);

                gData.vertices = new NativeDLLVertices(maxNbVertices);
                
                // retrieve model
                float[] t = cppDll.get_array<float>(Parameters.Container.Dynamic, "tr_" + ii);
                gData.model = new Matrix4x4(
                    new Vector4(t[0], t[1], t[2], 0),   // c0
                    new Vector4(t[4], t[5], t[6], 0),   // c1
                    new Vector4(t[8], t[9], t[10], 0),  // c2
                    new Vector4(t[12], t[13], t[14], 1) // c3
                );

                m_grabbersData.Add(gData);
            }


            idToUse = new HashSet<int>();
            foreach (var idStr in Ex.Text.split(initC.get<string>("cameras_to_use"), ";", removeEmptyLines: true)) {
                int id = Converter.to<int>(idStr);
                if (idToUse.Contains(id)) {
                    log_warning(string.Format("Grabber id [{0}] already defined in the list. ", id));
                } else {
                    if (id < m_nbConnections) {
                        idToUse.Add(id);
                    } else {
                        log_warning(string.Format("Cannot use id [{0}], there is only [{1}] grabbers available. ", id, m_nbConnections));
                    }
                }
            }

            if (idToUse.Count == 0) {
                log_warning("No grabber id defined.");
            }

            return true;
        }

        protected override void start_experiment() {

            for (int ii = 0; ii < m_grabbersData.Count; ++ii) {
                m_grabbersData[ii].connected = false;
                cppDll.set(Parameters.Container.Dynamic, "grabber_connected_" + ii, 0);
                send_infos_to_gui_init_config(string.Format("grabber_infos_{0}_{1}", ii, m_grabbersData.Count), "0");
            }

            cppDll.start_experiment();
        }

        protected override void stop_experiment() {

            cppDll.stop_experiment();

            for (int ii = 0; ii < m_grabbersData.Count; ++ii) {
                m_grabbersData[ii].connected = false;
                cppDll.set(Parameters.Container.Dynamic, "grabber_connected_" + ii, 0);
                send_infos_to_gui_init_config(string.Format("grabber_infos_{0}_{1}", ii, m_grabbersData.Count), "0");
            }            
        }

        protected override void clean() {

            indices.clean();
            foreach (var gData in m_grabbersData) {
                gData.lastFrameStateH.Free();
                gData.vertices.clean();
            }
            cppDll.clean();
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        public override void update_from_current_config() {
            cppDll.set(Parameters.Container.Dynamic, "delay", currentC.get<int>("delay"));
            cppDll.update_from_current_config();
        }

        protected override void update() {


            if (initC.get<bool>("debug_bypass")) {
                return;
            }

            Profiler.BeginSample("[ExVR][K4ManagerComponent] DLL update");

            cppDll.update();

            for (int ii = 0; ii < m_grabbersData.Count; ++ii) {
                bool connected = cppDll.get<int>(Parameters.Container.Dynamic, "grabber_connected_" + ii) == 1;
                if (connected != m_grabbersData[ii].connected) {
                    m_grabbersData[ii].connected = connected;
                    send_infos_to_gui_init_config(string.Format("grabber_infos_{0}_{1}", ii, m_grabbersData.Count), connected ? "1" : "0");
                }
            }

            Profiler.EndSample();

            Profiler.BeginSample("[ExVR][K4ManagerComponent] retrieve_cloud_frame_data");

            Parallel.For(0, m_grabbersData.Count, ii => {

                if (!idToUse.Contains(ii)) {
                    return;
                }

                var gData = m_grabbersData[ii];
                k4_dll().retrieve_cloud_frame_data(gData.id, gData.currentFrameId, gData.vertices, gData.lastFrameStateH.AddrOfPinnedObject());

                if (gData.lastFrameState[0] == 1) {
                    gData.update            = true;
                    gData.currentFrameId    = gData.lastFrameState[1];
                    gData.nbVertices        = gData.lastFrameState[2];
                } else {
                    gData.update = false;
                }
            });

            Profiler.EndSample();

            for (int ii = 0; ii < m_grabbersData.Count; ++ii) {
                var gData = m_grabbersData[ii];
                if (gData.update) {
                    send_infos_to_gui_current_config(
                        string.Format("frame_infos_{0}_{1}", ii, m_grabbersData.Count),
                        string.Format("Size: {0} Frame id: {1}", gData.nbVertices, gData.currentFrameId)
                    );
                }
            }
        }

        #endregion

        #region private_functions

        private DLLK4ManagerComponent k4_dll() {
            return (DLLK4ManagerComponent)cppDll;
        }

        #endregion

        #region public_functions


        public int connections_nb() {
            return m_nbConnections;
        }

        public Matrix4x4 model_matrix(int idGrabber) {
            if (cppDll != null) {
                return m_grabbersData[idGrabber].model;
            }
            return Matrix4x4.identity;
        }

        public bool update_cloud(int idGrabber, PointCloud pc) {

            var gData = m_grabbersData[idGrabber];
            if (gData.nbVertices >= 0 && gData.update) {
                Profiler.BeginSample("[ExVR][K4ManagerComponent] update_cloud " + idGrabber);
                pc.set_points(gData.vertices, indices, gData.nbVertices);       
                gData.update = false;
                Profiler.EndSample();
                return true;
            }
            return false;
        }

        #endregion  
    }
}

