
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
using Ex.XML;

// unity
using UnityEngine;

namespace Ex{

    public class CameraTrajectoryComponent : ExComponent{

        int lastTarget = -1;
        double currentT = 0f;
        Mesh cameraMesh = null;
        Vector3 originPosition = new Vector3();
        Quaternion originRotation = new Quaternion();

        List<CameraTarget> targets = new List<CameraTarget>();
        List<double> totalTimes = new List<double>();
        List<GameObject> camerasGO = new List<GameObject>();
        double totalTime = 0.0;

        private List<CameraTarget> get_camera_targets(string argName) {

            int id = 0;
            var values = currentC.get_list<string>(argName);
            List<CameraTarget> positions = new List<CameraTarget>(values.Count);            
            foreach(string value in values) {
                string[] splitArg = value.Split(new string[] { "[#0#]" }, StringSplitOptions.None);
                CameraTarget cp = new CameraTarget();
                cp.time                 = Converter.to_double(splitArg[0]);
                cp.display              = Converter.to_bool(splitArg[1]);
                cp.sphericInterpolation = Converter.to_bool(splitArg[2]);
                cp.translation          = Converter.to_vector3(splitArg[3]);
                cp.rotation             = Converter.to_vector3(splitArg[4]);
                cp.color                = Converter.to_color(splitArg[5]);
                cp.speed                = Converter.to_animation_curve(splitArg[6]);
                cp.id                   = id++;
                positions.Add(cp);                
            }

            if (currentC.get<bool>("compute_times")) {
                if (positions.Count > 0) {
                    // compute new times
                    var t = currentC.get<float>("global_time");
                    if (positions.Count == 1) {
                        positions[0].time = t;
                        return positions;
                    } 

                    List<Vector3> pos = new List<Vector3>();
                    //pos.Add(Global.VR().eyeCamera.transform.position);
                    pos.Add(new Vector3(0,0,0));
                    List<float> dist = new List<float>();
                    double totalDist = 0;
                    for (int ii = 0; ii < positions.Count; ++ii) {
                        pos.Add(positions[ii].translation);
                        var m = (pos[ii+1] - pos[ii]).magnitude;
                        //Debug.Log("dist :" + ii + " " + m + " " + pos[ii + 1] + " " + pos[ii]);
                        dist.Add(m);
                        totalDist += m;
                    }

                    for (int ii = 0; ii < positions.Count; ++ii) {
                        positions[ii].time = t * dist[ii] / totalDist;
                        //Debug.Log("times : " + ii + " " + positions[ii].time + " " + dist[ii] + " " + totalDist);
                    }
                }
            }

            return positions;
        }

        Quaternion interpolate(Quaternion q1, Quaternion q2, float t, bool sphericalInterpolation = true) {

            if (sphericalInterpolation) {
                return Quaternion.Slerp(q1, q2, t);
            }
            return Quaternion.Lerp(q1, q2, t);
        }

        Vector3 interpolate(Vector3 v1, Vector3 v2, float t, bool sphericalInterpolation = true) {

            if (sphericalInterpolation) {
                return Vector3.Slerp(v1, v2, t);
            }
            return Vector3.Lerp(v1, v2, t);
        }

        CameraTarget current_target(double timeSeconds) {
            for(int ii = 0; ii < totalTimes.Count; ++ii) {
                if(timeSeconds < totalTimes[ii]) {
                    return targets[ii];
                }
            }
            return null;
        }

        void add_camera_go(string name) {

            var go    = GO.generate_empty_scene_object(name,  transform,      true, Layers.Default);
            var subGo = GO.generate_empty_scene_object("sub", go.transform,   true, Layers.Debug);
            GO.init_local_euler_rotation(subGo, new Vector3(0, 180, 0));
            var mr = subGo.AddComponent<MeshRenderer>();
            var mf = subGo.AddComponent<MeshFilter>();
            mf.sharedMesh = cameraMesh;
            camerasGO.Add(go);
        }

        void update_camera_go(GameObject go, CameraTarget ct) {

            bool offset = currentC.get<bool>("add_camera_position");
            go.transform.localPosition = ct.translation;
            go.transform.localRotation = Quaternion.Euler(ct.rotation);

            if (offset) {
                go.transform.localPosition += ExVR.Display().cameras().get_eye_camera_transform().localPosition;
                go.transform.localRotation *= ExVR.Display().cameras().get_eye_camera_transform().localRotation;
            }

            go.SetActive(ct.display);
            go.transform.Find("sub").gameObject.GetComponent<MeshRenderer>().material.color = ct.color;
        }

        void delete_cameras_go() {
            foreach (GameObject go in camerasGO) {
                Destroy(go);
            }
            camerasGO.Clear();
        }

        protected override void start_routine() {

            lastTarget = -1;
            currentT = 0f;
            totalTime = 0;
            targets = get_camera_targets("camera_trajectory");
            totalTimes = new List<double>();
            for(int ii = 0; ii < targets.Count; ++ii) { 
                totalTime += targets[ii].time;
                totalTimes.Add(totalTime);
                add_camera_go("Target " + ii);
                update_camera_go(camerasGO[ii], targets[ii]);
            }            
        }

        protected override bool initialize() {
            cameraMesh = Instantiate(UnityEngine.Resources.Load<Mesh>("Meshes/VR/generic_hmd"));
            return true;
        }

        protected override void update() {

            var timeS = time().ellapsed_element_ms() * 0.001;
            var target = current_target(timeS);

            if(target == null) { // no target remaining
                return;
            }

            // check if new target
            if (target.id != lastTarget) {
                // new target
                lastTarget = target.id;                
                originPosition = ExVR.Display().cameras().get_camera_rig_transform().position;
                originRotation = ExVR.Display().cameras().get_camera_rig_transform().rotation;
                currentT = timeS;
            }

            // update targets position
            bool offset = currentC.get<bool>("add_camera_position");
            GameObject camGO = null;
            for (int ii = 0; ii < targets.Count; ++ii) {
                update_camera_go(camerasGO[ii], targets[ii]);
                if (targets[ii].id == target.id) {                    
                    camGO = camerasGO[ii];
                }                
            }
            if(camGO == null) {
                return;
            }

            // get current time
            double tF = (timeS - currentT )/ target.time;
            float speedT = target.speed.Evaluate((float)tF);

            Quaternion eyeToOriginRot = Quaternion.Inverse(ExVR.Display().cameras().get_eye_camera_transform().localRotation);
            var eyeLocalPosition = ExVR.Display().cameras().get_eye_camera_transform().localPosition;

            // compute current target
            var targetPosition = camGO.transform.position;
            var targetRotation = camGO.transform.rotation;// Quaternion.Euler(new Vector3(target.rotation.x, target.rotation.y, target.rotation.z));

            // compute origin position/rotation target interpolation
            ExVR.Display().cameras().get_camera_rig_transform().rotation = interpolate(originRotation, targetRotation * eyeToOriginRot, speedT, target.sphericInterpolation);
            ExVR.Display().cameras().get_camera_rig_transform().position = interpolate(originPosition, targetPosition - (ExVR.Display().cameras().get_camera_rig_transform().rotation * eyeLocalPosition), speedT, target.sphericInterpolation);
        }

        protected override void stop_routine() {
            delete_cameras_go();
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            
            if(updatedArgName == "camera_trajectory") {

                // reload targets
                targets = get_camera_targets(updatedArgName);

                // reset time
                totalTime = 0;

                // if change in targets number, rebuild time array
                if (targets.Count != targets.Count) {
                    totalTimes = new List<double>();
                    foreach (var target in targets) {
                        totalTime += target.time;
                        totalTimes.Add(totalTime);
                    }
                } else { // update new times
                    foreach (var target in targets) {
                        totalTime += target.time;
                    }
                }

                // if change in targets number, rebuild all cameras gameobjects   
                if(targets.Count != camerasGO.Count) {
                    delete_cameras_go();
                    for(int ii = 0; ii < targets.Count; ++ii) {
                        add_camera_go("Target " + ii);
                    }
                }

                // update cameras gameobjects
                for (int ii = 0; ii < targets.Count; ++ii) {
                    update_camera_go(camerasGO[ii], targets[ii]);
                }
            }
        }

        protected override void set_visibility(bool visible) {
         
            foreach(var go in camerasGO) {
                go.SetActive(visible);
            }
        }
    }
}