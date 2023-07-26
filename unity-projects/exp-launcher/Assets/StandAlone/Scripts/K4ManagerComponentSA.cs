
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
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

// unity
using UnityEngine;

namespace SA {

    public static class Text {
        public static string[] split(string text, string sep, bool removeEmptyLines = true) {
            return text.Split(new string[] { sep }, removeEmptyLines ? StringSplitOptions.RemoveEmptyEntries : StringSplitOptions.None);
        }
    }

    public class K4GrabberDataSA {

        public int id = 0;
        public bool connected = false;

        public int currentFrameId = -1;
        public int nbVertices = 0;
        public bool update = false;

        public NativeDLLVerticesSA vertices = null;

        public GCHandle lastFrameStateH;
        public int[] lastFrameState = null; // success (1/0) | lastFrameId | nbVertices        
        public Matrix4x4 model = Matrix4x4.identity;
    }


    public class K4ManagerComponentSA : MonoBehaviour {

        #region parameters

        [Header("Init parameters")]         
        public string camerasToUse = "0;1;2;3";
        public string networkFilePath = "";
        public string deviceFilePath = "";
        public string filtersFilePath = "";
        public string colorsFilePath = "";
        public string modelsFilePath = "";

        [Header("Parameters")]
        public int delayMs = 0;

        #endregion
        #region states

        [Header("States")]
        public bool m_initialized = false;
        public int m_nbConnections = 0;
        public bool m_networkFileLoaded = false;
        public bool m_deviceConfigFileLoaded = false;
        public bool m_filtersFileLoaded = false;
        public bool m_colorFileLoaded = false;
        public bool m_modelFileLoaded = false;
        public HashSet<int> idToUse = null;

        public List<K4GrabberDataSA> m_grabbersData = null;
        public NativeIndicesSA m_indices = null;

        public DLLK4ManagerComponentSA m_managerDll = null;
        

        #endregion

        #region mono_behaviour

        private void Awake() {

            // init experiment DLL
            if(DLLExperimentSA.global == null) {
                DLLExperimentSA.global = new DLLExperimentSA();
            }

            // init manager DLL
            try {
                m_managerDll = new DLLK4ManagerComponentSA();
            } catch (System.Exception exception) {
                Debug.LogError(string.Format("Cannot init K4Manager DLL, error: {0}", exception.Message));
                m_managerDll = null;
                return;
            }

            if (networkFilePath.Length != 0) {
                m_managerDll.set(ParametersSA.Container.Dynamic, "path_network_settings_file", networkFilePath);
            } else {
                Debug.LogError("Set the network settings file path.");
                return;
            }

            if (deviceFilePath.Length != 0) {
                m_managerDll.set(ParametersSA.Container.Dynamic, "path_device_settings_file", deviceFilePath);
            } else {
                Debug.LogWarning("No device settings file path set.");
            }

            if (filtersFilePath.Length != 0) {
                m_managerDll.set(ParametersSA.Container.Dynamic, "path_filters_file", filtersFilePath);
            } else {
                Debug.LogWarning("No filters file path set.");
            }
            
            if (colorsFilePath.Length != 0) {
                m_managerDll.set(ParametersSA.Container.Dynamic, "path_color_settings_file", colorsFilePath);
            } else {
                Debug.LogWarning("No colors settings file path set.");
            }

            if (modelsFilePath.Length != 0) {
                m_managerDll.set(ParametersSA.Container.Dynamic, "path_model_file", modelsFilePath);
            } else {
                Debug.LogError("No models file path set.");
            }

            m_managerDll.expDll = DLLExperimentSA.global;
            bool success = m_managerDll.initialize();
            if (!success) {
                Debug.LogError("Cannot initialize k4 manager dll.");
                return;
            }

            m_nbConnections          = m_managerDll.get<int>(ParametersSA.Container.Dynamic, "nb_connections");
            m_networkFileLoaded      = m_managerDll.get<int>(ParametersSA.Container.Dynamic, "network_settings_file_loaded") == 1;
            m_deviceConfigFileLoaded = m_managerDll.get<int>(ParametersSA.Container.Dynamic, "device_settings_file_loaded") == 1;
            m_filtersFileLoaded      = m_managerDll.get<int>(ParametersSA.Container.Dynamic, "filters_file_loaded") == 1;
            m_colorFileLoaded        = m_managerDll.get<int>(ParametersSA.Container.Dynamic, "color_settings_file_loaded") == 1;
            m_modelFileLoaded        = m_managerDll.get<int>(ParametersSA.Container.Dynamic, "models_file_loaded") == 1;

            if (m_nbConnections == 0) {
                Debug.LogError("No connection defined in network file.");
                return;
            }

            // generate indices
            int maxNbVertices = 2000000;
            m_indices = new NativeIndicesSA(maxNbVertices);

            // init grabbers info
            m_grabbersData = new List<K4GrabberDataSA>(m_nbConnections);
            for (int ii = 0; ii < m_nbConnections; ++ii) {
                K4GrabberDataSA gData = new K4GrabberDataSA();
                gData.id = ii;
                gData.connected = false;
                gData.lastFrameState = new int[3];
                gData.lastFrameStateH = GCHandle.Alloc(gData.lastFrameState, GCHandleType.Pinned);

                gData.vertices = new NativeDLLVerticesSA(maxNbVertices);

                // retrieve model
                float[] t = m_managerDll.get_array<float>(ParametersSA.Container.Dynamic, "tr_" + ii);
                gData.model = new Matrix4x4(
                    new Vector4(t[0], t[1], t[2], 0),   // c0
                    new Vector4(t[4], t[5], t[6], 0),   // c1
                    new Vector4(t[8], t[9], t[10], 0),  // c2
                    new Vector4(t[12], t[13], t[14], 1) // c3
                );

                m_grabbersData.Add(gData);
            }

            idToUse = new HashSet<int>();
            foreach (var idStr in Text.split(camerasToUse, ";", removeEmptyLines: true)) {

                int id = System.Convert.ToInt32(idStr);
                if (idToUse.Contains(id)) {
                    Debug.LogWarning(string.Format("Grabber id [{0}] already defined in the list. ", id));
                } else {
                    if (id < m_nbConnections) {
                        idToUse.Add(id);
                    } else {
                        Debug.LogWarning(string.Format("Cannot use id [{0}], there is only [{1}] grabbers available. ", id, m_nbConnections));
                    }
                }
            }

            if (idToUse.Count == 0) {
                Debug.LogWarning("No grabber id defined.");
            }            

            m_initialized = true;
        }

        void Start() {

            if (m_initialized) {
                m_managerDll.start_experiment();
            } 
        }


        void Update() {

            if (!m_initialized) {
                return;
            }

            update_from_settings();

            m_managerDll.update();

            for (int ii = 0; ii < m_grabbersData.Count; ++ii) {
                bool connected = m_managerDll.get<int>(ParametersSA.Container.Dynamic, "grabber_connected_" + ii) == 1;
                if (connected != m_grabbersData[ii].connected) {
                    m_grabbersData[ii].connected = connected;
                }
            }

            Parallel.For(0, m_grabbersData.Count, ii => {

                if (!idToUse.Contains(ii)) {
                    return;
                }

                var gData = m_grabbersData[ii];
                m_managerDll.retrieve_cloud_frame_data(gData.id, gData.currentFrameId, gData.vertices, gData.lastFrameStateH.AddrOfPinnedObject());

                if (gData.lastFrameState[0] == 1) {
                    gData.update = true;
                    gData.currentFrameId = gData.lastFrameState[1];
                    gData.nbVertices = gData.lastFrameState[2];
                } else {
                    gData.update = false;
                }
            });

        }

        void OnDestroy() {

            if (m_initialized) {
                m_managerDll.stop_experiment();
            }            

            clean();
        }

        #endregion

        #region private_functions

        private void clean() {
            m_indices.clean();
            foreach (var gData in m_grabbersData) {
                gData.lastFrameStateH.Free();
                gData.vertices.clean();
            }
            m_managerDll.clean();           
        }

        #endregion

        #region public_functions

        public void update_from_settings() {
            m_managerDll.set(ParametersSA.Container.Dynamic, "delay", delayMs);
            m_managerDll.update_from_current_config();
        }

        public int connections_nb() {
            return m_nbConnections;
        }

        public Matrix4x4 model_matrix(int idGrabber) {
            return m_grabbersData[idGrabber].model;
        }

        public bool update_cloud(int idGrabber, PointCloudSA pc) {

            var gData = m_grabbersData[idGrabber];
            if (gData.nbVertices >= 0 && gData.update) {
                pc.set_points(gData.vertices, m_indices, gData.nbVertices);
                gData.update = false;
                return true;
            }
            return false;
        }


        #endregion

    }
}