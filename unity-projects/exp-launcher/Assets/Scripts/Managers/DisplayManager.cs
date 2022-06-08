
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
using System.Collections;

// unity
using UnityEngine;
using UnityEngine.Rendering.PostProcessing;

// valve
using Valve.VR;

namespace Ex{

    public class DisplayManager : MonoBehaviour{

        public enum DisplayMode : int{
            OpenVR = 0, Flat, FlatStereo
        }

        // cameras
        public CamerasManager camerasManager = null;


        // steam vr
        public SteamVR_PlayArea playArea = null;
        public SteamVR_TrackedObject leftController = null;
        public SteamVR_TrackedObject rightController = null;

        public GameObject steamVR = null;
        public SteamVR_Behaviour behaviour = null;
        public SteamVR_Render render = null;

        // post process
        public PostProcessVolume postProcessingVolume = null;

        // debug
        public GameObject debugVrHmd = null;

        public CamerasManager cameras() {
            return camerasManager;
        }

        public void initialize() {

            camerasManager = GetComponent<CamerasManager>();

            postProcessingVolume = transform.Find("PostProcessVolume").GetComponent<PostProcessVolume>();

            switch (ExVR.GuiSettings().displayMode) {
                case DisplayMode.OpenVR:
                    set_openvr_parameters();
                    break;
                case DisplayMode.FlatStereo:
                    set_flat_stereo_parameters();
                    return;
                case DisplayMode.Flat:
                    set_flat_parameters();
                    return;
            }

            // quality
            QualitySettings.antiAliasing = 4;
        }
 
        IEnumerator load_device(string device, bool enableVR) {

            UnityEngine.XR.XRSettings.LoadDeviceByName(device);
            yield return new WaitForEndOfFrame();

            UnityEngine.XR.XRSettings.enabled = enableVR;
            yield return new WaitForEndOfFrame();
        }


        public void change_display_mode(DisplayMode mode) {

            if (UnityEngine.XR.XRSettings.loadedDeviceName == "None" && (mode == DisplayMode.Flat || mode == DisplayMode.FlatStereo)) {
                return;
            }

            if (UnityEngine.XR.XRSettings.loadedDeviceName == "OpenVR" && mode == DisplayMode.OpenVR) {
                return;
            }

            switch (mode) {
                case DisplayMode.OpenVR:
                    set_openvr();
                    break;
                case DisplayMode.FlatStereo:
                    set_flat_stereo();                    
                    return;
                case DisplayMode.Flat:
                    set_flat();
                    return;
            }
        }

        private void create_steam_vr() {

            if (steamVR == null) {
                steamVR = new GameObject("[SteamVR]");
                steamVR.transform.parent = GetComponent<CamerasManager>().get_camera_rig_transform();
                steamVR.SetActive(false);
                render = steamVR.AddComponent<SteamVR_Render>();
                behaviour = steamVR.AddComponent<SteamVR_Behaviour>();
                behaviour.initializeSteamVROnAwake = false;
                steamVR.SetActive(true);
            }            
        }

        private void destroy_steam_vr() {
            if (steamVR != null) {
                Destroy(steamVR);
                steamVR = null;
            }
        }

        private void set_openvr() {            
            create_steam_vr();
            behaviour.InitializeSteamVR(true);
            StartCoroutine(load_device("OpenVR", true));
            set_openvr_parameters();
        }

        private void set_flat_stereo() {
            destroy_steam_vr();
            StartCoroutine(load_device("None", false));
            set_flat_stereo_parameters();
        }

        private void set_flat() {
            destroy_steam_vr();
            StartCoroutine(load_device("None", false));
            set_flat_parameters();
        }

        private void set_openvr_parameters() {
            Application.targetFrameRate = 90;
            var cm = GetComponent<CamerasManager>();
            cm.bothEyesCamera.enabled = true;
            cm.leftEyeCamera.enabled = true;
            cm.rightEyeCamera.enabled = true;
            cm.leftEyeStereoCamera.enabled = false;
            cm.rightEyeStereoCamera.enabled = false;
        }

        private void set_flat_stereo_parameters() {
            Application.targetFrameRate = 60;
            var cm = GetComponent<CamerasManager>();
            cm.bothEyesCamera.enabled = false;
            cm.leftEyeCamera.enabled = false;
            cm.rightEyeCamera.enabled = false;
            cm.leftEyeStereoCamera.enabled = true;
            cm.rightEyeStereoCamera.enabled = true;

            cm.leftEyeStereoCamera.fieldOfView  = ExVR.GuiSettings().stereoFov;
            cm.rightEyeStereoCamera.fieldOfView = ExVR.GuiSettings().stereoFov;
        }

        private void set_flat_parameters() {
            Application.targetFrameRate = 60;
            var cm = GetComponent<CamerasManager>();
            cm.bothEyesCamera.enabled = true;
            cm.leftEyeCamera.enabled = false;
            cm.rightEyeCamera.enabled = false;
            cm.leftEyeStereoCamera.enabled = false;
            cm.rightEyeStereoCamera.enabled = false;
        }





        public void apply_eye_camera_only_layer_to_hierarchy(GameObject go) {
            foreach (Transform tr in go.GetComponentsInChildren<Transform>(true)) {
                tr.gameObject.layer = Layers.EyeCameraOnly;
            }
        }

        // reset VR parameters before starting experiment
        public void reset_experiment_settings() {

            // reset debug vr hmd layer
            foreach (Transform tr in debugVrHmd.GetComponentsInChildren<Transform>(true)) {
                tr.gameObject.layer = Layers.Debug;
            }

            camerasManager.reset();
        }

        public void Update() {

            if(debugVrHmd != null) {
                var tr = camerasManager.get_eye_camera_transform();
                debugVrHmd.transform.position = tr.position;
                debugVrHmd.transform.rotation = tr.rotation;
            }
        }
    }
}



///// <summary>
///// Fixes MSAA not being applied in VR with URP. Taken from; https://forum.unity.com/threads/quest-lwrp-urp-msaa-not-working.786026/
///// </summary>
///// <returns></returns>
//IEnumerator MSAAFix() {

//    UnityEngine.XR.XRSettings.eyeTextureResolutionScale = 0.5f; // Any value, just to trigger the refresh
//    yield return new WaitForEndOfFrame(); // Needed to apply the changes
//    UnityEngine.XR.XRSettings.eyeTextureResolutionScale = 1f; // Use your target resolution here
//    Debug.LogError("after: " + UnityEngine.XR.XRSettings.eyeTextureDesc.msaaSamples);
//    yield return null;
//}