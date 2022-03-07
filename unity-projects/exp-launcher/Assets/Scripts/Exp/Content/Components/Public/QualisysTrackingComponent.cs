
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
using System.Threading;
using System.Collections.Generic;

// unity
using UnityEngine;

// qualisys
using QualisysRealTime.Unity;

namespace Ex{

    public class QualisysTrackingComponent : ExComponent{

        private bool connected = false;
        private Dictionary<string, GameObject> objects = new Dictionary<string, GameObject>();

        private string hostName;
        private string ipAddress;
        private bool connectToFirst;

        #region ex_functions

        protected override bool initialize() {

            add_signal("tracked object");

            hostName       = initC.get<string>("host_name");
            ipAddress      = initC.get<string>("ip_address");
            connectToFirst = initC.get<bool>("connect_to_first");

            Thread thread = new Thread(connect);
            thread.Start();

            return true;
        }

        protected override void clean() {

            if (connected) {
                RTClient.GetInstance().Dispose();
                connected = false;
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
            foreach (var trackedObject in objects) {
                Destroy(trackedObject.Value);
            }
            objects.Clear();

            // recreate object list
            foreach (var objectName in currentC.get<string>("objects").Split('\n')) {

                if (objectName.Length == 0) {
                    continue;
                }

                objects[objectName] = GO.generate_sphere(objectName, transform, 0.025f, new Color(1, 0, 0));
                var rtObject = objects[objectName].AddComponent<QualisysTrackedBody>();
                rtObject.name = objectName;
            }

            set_visibility(is_visible());
        }

        protected override void pre_update() {

            foreach (var trackedObject in objects) {
                var rtObject = trackedObject.Value.GetComponent<QualisysTrackedBody>();
                if (rtObject.tracked) {

                    TransformValue trV = new TransformValue();
                    trV.position = rtObject.transform.position;
                    trV.rotation = rtObject.transform.rotation;
                    invoke_signal("tracked object", new StringAny(rtObject.name, trV));

                    trackedObject.Value.GetComponent<MeshRenderer>().material.color = Color.green;
                } else {
                    trackedObject.Value.GetComponent<MeshRenderer>().material.color = Color.red;
                }
            }
        }

        protected override void set_visibility(bool visible) {
            foreach (var trackedObject in objects) {
                trackedObject.Value.GetComponent<MeshRenderer>().enabled = visible;
            }
        }

        #endregion
        #region private_functions
        private void connect() {

            connected = false;
            var servers = RTClient.GetInstance().GetServers();
            if (servers.Count == 0) {
                log_error("No Qualisys server found.", false);
                return;
            }

            foreach (var server in servers) {

                if (!connectToFirst) {

                    if (hostName.Length > 0) {
                        if (server.HostName != hostName) {
                            continue;
                        }
                    }
                    if (ipAddress.Length > 0) {
                        if (server.IpAddress != ipAddress) {
                            continue;
                        }
                    }
                }

                if (RTClient.GetInstance().Connect(server, server.Port, true, true)) {
                    log_message(
                        String.Format("Connected to Qualisys server {0}:{1}:{2} with {3} cameras.",
                        server.HostName, server.IpAddress, server.Port, server.CameraCount
                        ), false);
                    connected = true;
                    return;
                }
            }

            log_error("Cannot connect to to any Qualisys server.", false);
        }


        public bool is_connected() {
            return connected;
        }

        public Vector3 get_object_position(string objectName) {

            if (objects.ContainsKey(objectName)) {
                return objects[objectName].transform.position;
            }

            log_error("Object with name " + objectName + " not available. ");
            return Vector3.zero;
        }

        public Quaternion get_object_rotation(string objectName) {

            if (objects.ContainsKey(objectName)) {
                return objects[objectName].transform.rotation;
            }

            log_error("Object with name " + objectName + " not available. ");
            return Quaternion.identity;
        }

        #endregion
        #region public_functions
        #endregion        
    }
}
