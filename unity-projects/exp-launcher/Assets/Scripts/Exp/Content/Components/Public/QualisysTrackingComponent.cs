
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
using System.Text;
using System.Collections.Generic;

// unity
using UnityEngine;

// qualisys
using QualisysRealTime.Unity;

namespace Ex{

    public class QualisysTrackingComponent : ExComponent{

        private Dictionary<string, GameObject> m_objects = new Dictionary<string, GameObject>();
        private Dictionary<string, SixDOFBody> m_bodies  = new Dictionary<string, SixDOFBody>();

        private RTClient m_rtClient = null;
        private int m_currentFrame = 0;

        #region ex_functions

        protected override bool initialize() {

            add_signal("tracked object");

            connect_to_qualisys();

            return true;
        }

        protected override void clean() {
            if (m_rtClient != null) {
                m_rtClient.Dispose();
            }
        }

        public override void update_from_current_config() {
            reset_config_transform();
            if (!currentC.get<bool>("transform_do_not_apply")) {
                currentC.update_transform("transform", transform, true);
            }
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        protected override void start_routine() {

            // clean objects
            foreach (var trackedObject in m_objects) {
                Destroy(trackedObject.Value);
            }
            m_objects.Clear();
            m_bodies.Clear();

            // recreate object list
            foreach (var objectName in currentC.get<string>("objects").Split('\n')) {

                if (objectName.Length == 0) {
                    continue;
                }
                m_objects[objectName] = GO.generate_sphere(objectName, transform, 0.025f, new Color(1, 0, 0));
                m_bodies[objectName] = null;
            }

            set_visibility(is_visible());
        }

        protected override void pre_update() {

            if(m_rtClient == null) {
                return;
            }

            // get new frame
            m_currentFrame = m_rtClient.GetFrame();

            // read bodies
            foreach(var obj in m_objects) {

                m_bodies[obj.Key] = m_rtClient.GetBody(obj.Key);

                if (is_updating()) {

                    var currentBody   = m_bodies[obj.Key];
                    var currentObject = m_objects[obj.Key];
                    bool tracked = false;
                    if (currentBody != null) {
                        if (currentBody.Position.magnitude > 0) {

                            // update go                            
                            currentObject.transform.localPosition = currentBody.Position;
                            currentObject.transform.localRotation = currentBody.Rotation;

                            // update signal tr
                            TransformValue trV = new TransformValue();
                            trV.position = currentBody.Position;
                            trV.rotation = currentBody.Rotation;
                            invoke_signal("tracked object", new StringAny(obj.Key, trV));

                            tracked = true;
                        }
                    }
                    currentObject.GetComponent<MeshRenderer>().material.color = tracked ? Color.green : Color.red;
                }
            }
        }

        protected override void set_visibility(bool visible) {
            foreach (var trackedObject in m_objects) {
                trackedObject.Value.GetComponent<MeshRenderer>().enabled = visible;
            }
        }

        public override string format_frame_data_for_global_logger(bool header) {

            if (header) {
                return "Qualisys: [body_name_n] tracked_n tx_n ty_n tz_n rx_n ry_n rz_n";
            }

            if (m_rtClient == null) {
                return "[Not_connected]";
            }

            if(m_bodies.Count == 0) {
                return "[No_body_tracked]";
            }

            List<string> bodiesStr = new List<string>();
            foreach (var body in m_bodies) {

                if(body.Value != null) {
                    if (body.Value.Position.magnitude > 0) {
                        bodiesStr.Add(string.Format("[{0}] 1 {1} {2}",
                            body.Key,
                            Converter.to_string(body.Value.Position, Converter.g7, " "),
                            Converter.to_string(body.Value.Rotation.eulerAngles, Converter.g7, " ")
                        ));
                        continue;
                    }
                }
                bodiesStr.Add(string.Format("[{0}] 0 ? ? ? ? ? ?",
                    body.Key
                ));
            }
            return string.Join(" ", bodiesStr);
        }

        #endregion
        #region private_functions
        private void connect_to_qualisys() {

            m_rtClient  = RTClient.GetInstance();
            var servers = m_rtClient.GetServers();
            if (servers.Count == 0) {
                log_error("No Qualisys server found.", false);
                return;
            }

            foreach (var server in servers) {

                if (!initC.get<bool>("connect_to_first")) {

                    var hostName = initC.get<string>("host_name");
                    if (hostName.Length > 0) {
                        if (server.HostName != hostName) {
                            continue;
                        }
                    }
                    var ipAddress = initC.get<string>("ip_address");
                    if (ipAddress.Length > 0) {
                        if (server.IpAddress != ipAddress) {
                            continue;
                        }
                    }
                }
                if(m_rtClient.Connect(server, server.Port, true, true, true, true, true, true)) { 
                
                    log_message(
                        String.Format("Connected to Qualisys server {0}:{1}:{2} with {3} cameras.",
                        server.HostName, server.IpAddress, server.Port, server.CameraCount
                        ), false);
                    
                    return;
                }
            }

            log_error("Cannot connect to to any Qualisys server.", false);
            m_rtClient = null;
        }

        public bool is_connected() {
            return m_rtClient != null;
        }

        public Vector3 get_body_position(string objectName) {

            if (m_bodies.ContainsKey(objectName)) {
                return m_bodies[objectName].Position;
            }

            log_error(string.Format("Body with name [{0}] not available. ", objectName));
            return Vector3.zero;
        }

        public Quaternion get_body_rotation(string objectName) {

            if (m_bodies.ContainsKey(objectName)) {
                return m_bodies[objectName].Rotation;
            }

            log_error(string.Format("Body with name [{0}] not available. ", objectName));
            return Quaternion.identity;
        }

        public Vector3 get_object_position(string objectName) {

            if (m_objects.ContainsKey(objectName)) {
                return m_objects[objectName].transform.position;
            }

            log_error(string.Format("Object with name [{0}] not available. ", objectName));
            return Vector3.zero;
        }

        public Quaternion get_object_rotation(string objectName) {

            if (m_objects.ContainsKey(objectName)) {
                return m_objects[objectName].transform.rotation;
            }

            log_error(string.Format("Object with name [{0}] not available. ", objectName));
            return Quaternion.identity;
        }

        #endregion
    }
}
