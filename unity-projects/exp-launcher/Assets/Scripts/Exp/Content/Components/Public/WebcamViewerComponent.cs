
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

// unity
using UnityEngine;
using UnityEngine.UI;

namespace Ex{

    public class WebcamViewerComponent : CanvasWorldSpaceComponent{

        private GameObject m_panelGO = null;
        private RawImage m_image = null;
        private WebCamTexture m_webcamTexture = null;

        int m_originalWidth = 0;
        int m_originalHeight = 0;

        protected override bool initialize() {

            // init gameObjects
            m_panelGO = new GameObject("Video panel");
            m_panelGO.transform.SetParent(transform);
            m_panelGO.layer = Layers.Default;
            m_panelGO.SetActive(false);

            m_panelGO.AddComponent<RectTransform>();
            m_panelGO.AddComponent<CanvasRenderer>();
            m_image = m_panelGO.AddComponent<RawImage>();

            var rTr = m_panelGO.GetComponent<RectTransform>();
            rTr.pivot = new Vector2(0.5f, 0.5f);
            rTr.localPosition = new Vector3(0f, 0f, 0f);
            rTr.localEulerAngles = new Vector3(0f, 0f, 0f);
            rTr.sizeDelta = new Vector2(200, 200);
            rTr.localScale = new Vector3(0.01f, 0.01f, 0.01f);
            rTr.anchorMin = new Vector2(0f, 0f);
            rTr.anchorMax = new Vector2(0f, 0f);

            WebCamDevice[] devices = WebCamTexture.devices;
            if(devices.Length == 0) {
                log_error("No device available.");
                return false;
            }

            int id = initC.get<int>("device_id");
            if(id >= devices.Length) {
                log_error("Invalid id device.");
                return false;
            }

            m_webcamTexture = new WebCamTexture(devices[id].name);
            m_webcamTexture.requestedFPS = initC.get<int>("requested_fps");

            m_image.material.mainTexture = m_webcamTexture; // TODO: delay, comment it
            m_webcamTexture.Play();

            m_originalWidth = m_webcamTexture.width;
            m_originalHeight = m_webcamTexture.height;
            m_webcamTexture.Pause();

            return true;
        }

        protected override void start_routine() {
            resize_container();
            m_webcamTexture.Play();
        }

        protected override void set_update_state(bool doUpdate) {
            if (doUpdate) {
                m_webcamTexture.Play();
            } else {
                m_webcamTexture.Pause();
            }            
        }

        protected override void set_visibility(bool visibility) {
            m_panelGO.SetActive(visibility);
        }

        public override void pause() {
            m_webcamTexture.Pause();
        }

        public override void play() {
            m_webcamTexture.Play();
        }

        protected override void post_update() {
            resize_container();
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            resize_container();
        }


        public void resize_container() {

            m_panelGO.transform.position = Vector3.zero;
            m_panelGO.transform.rotation = Quaternion.identity;

            m_webcamTexture.requestedWidth = currentC.get<int>("width");
            m_webcamTexture.requestedHeight = currentC.get<int>("height");

            var rTr = m_panelGO.GetComponent<RectTransform>();
            rTr.pivot = new Vector2(0.5f, 0.5f);

            if (currentC.get<bool>("use_eye_camera")) {

                // move to head
                Transform camTr = ExVR.Display().cameras().get_eye_camera_transform();
                m_panelGO.transform.position = camTr.position + camTr.forward * currentC.get<float>("distance");
                m_panelGO.transform.rotation = camTr.rotation;
                m_panelGO.transform.eulerAngles += currentC.get_vector3("rotation");
                rTr.pivot = currentC.get_vector2("pivot");

            } else {
                rTr.localPosition = currentC.get_vector3("position");
                rTr.localEulerAngles = currentC.get_vector3("rotation");
            }

            rTr.sizeDelta = ((currentC.get<bool>("use_original_size")) ?
                        new Vector2(m_originalWidth, m_originalHeight) :
                        new Vector2(currentC.get<int>("width"), currentC.get<int>("height"))
            );

            var sf = currentC.get<float>("scale_factor") * 0.01f;
            rTr.localScale = new Vector3(
                sf, sf, sf
            );
        }

        protected override void stop_routine() {
            m_webcamTexture.Pause();
        }

        protected override void stop_experiment() {
            m_webcamTexture.Stop();
        }
    }
}