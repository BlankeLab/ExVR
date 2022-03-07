
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

using System;
using System.Collections;

// unity
using UnityEngine;
using UnityEngine.UI;

namespace Ex
{

    public class VideoFileCameraViewerComponent : CanvasWorldSpaceComponent{

        private GameObject m_panelGO = null;
        private RawImage m_image = null;
        private RenderTexture m_texture = null;
        private UnityEngine.Video.VideoPlayer m_player = null;

        int m_originalWidth = 0;
        int m_originalHeight = 0;
        bool m_initSize = false;

        protected override bool initialize() {

            // init gameObjects
            m_panelGO = new GameObject("Video panel");            
            m_panelGO.transform.SetParent(transform);
            m_panelGO.layer = Layers.Default;
            m_panelGO.SetActive(true);


            m_panelGO.AddComponent<RectTransform>();
            m_panelGO.AddComponent<CanvasRenderer>();
            m_image = m_panelGO.AddComponent<RawImage>();
            m_player = m_panelGO.AddComponent<UnityEngine.Video.VideoPlayer>();

            var rTr = m_panelGO.GetComponent<RectTransform>();
            rTr.pivot = new Vector2(0.5f, 0.5f);
            rTr.localPosition = new Vector3(0f, 0f, 0f);
            rTr.localEulerAngles = new Vector3(0f, 0f, 0f);
            rTr.sizeDelta = new Vector2(200, 200);
            rTr.localScale = new Vector3(0.01f, 0.01f, 0.01f);
            rTr.anchorMin = new Vector2(0f, 0f);
            rTr.anchorMax = new Vector2(0f, 0f);
            

            m_player.source = UnityEngine.Video.VideoSource.Url;
            m_player.playOnAwake = true;
            m_player.isLooping = true;

            string videoPath = ExVR.Resources().get_video_file_data(initC.get_resource_alias("video")).path;
            if (videoPath.Length == 0) {
                return false;
            }

            m_player.url = videoPath;

            m_panelGO.SetActive(false);

            return true;
        }

        protected override void start_routine() {
            m_player.frame = 0;
            resize_image();

            bool currentVisibility = m_panelGO.activeSelf;
            m_panelGO.SetActive(true);
            m_player.Play();
            m_panelGO.SetActive(currentVisibility);
        }

        public override void play() {
            m_player.Play();
        }

        public override void pause() {
            m_player.Pause();
        }

        protected override void stop_routine() {
            m_player.Stop();
        }

        protected override void set_visibility(bool visibility) {
            m_panelGO.SetActive(visibility);
        }

        protected override void update() {

            //if (!m_player.isLooping) {
            //    //m_player.loopPointReached
            //}


            if (m_player.texture != null && !m_initSize) {

                m_originalWidth = (int)m_player.width;
                m_originalHeight = (int)m_player.height;

                m_texture = new RenderTexture(m_originalWidth, m_originalHeight, 0);
                m_player.targetTexture = m_texture;
                m_image.texture = m_texture;

                m_initSize = true;
                resize_image();
            }

            Transform camTr = ExVR.Display().cameras().get_eye_camera_transform();
            m_panelGO.transform.position = camTr.position + camTr.forward * currentC.get<float>("distance");
            m_panelGO.transform.rotation = camTr.rotation;
            m_panelGO.transform.eulerAngles += currentC.get_vector3("rotation");
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            resize_image();
        }

        public void resize_image() {

            if (m_texture == null) {
                return;
            }

            var rTr = m_panelGO.GetComponent<RectTransform>();
            if (m_initSize) {
                rTr.sizeDelta = ((currentC.get<bool>("use_original_size")) ?
                                new Vector2(m_originalWidth, m_originalHeight) : new Vector2(currentC.get<int>("width"), currentC.get<int>("height")))
                                * currentC.get<float>("scale_factor");
            } else {
                rTr.sizeDelta = new Vector2(currentC.get<int>("width"), currentC.get<int>("height") * currentC.get<float>("scale_factor"));
            }

            rTr.pivot = new Vector2(currentC.get<float>("pivot_x"), currentC.get<float>("pivot_y"));
        }
    }
}