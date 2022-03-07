
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

namespace Ex{

    public class WebcamComponent : ExComponent{

        private WebCamTexture m_webcamTexture = null;
        private int updateCounter = 0;


        protected override bool initialize() {

            add_signal("new frame");
            add_slot("pause", (nullArg) => { pause(); });
            add_slot("play", (nullArg) => { play(); });
            

            WebCamDevice[] devices = WebCamTexture.devices;
            if (devices.Length == 0) {
                log_error("No device available.");
                return false;
            }

            int id = initC.get<int>("device_id");
            if (id >= devices.Length) {
                log_error("Invalid id device.");
                return false;
            }
            m_webcamTexture = new WebCamTexture(devices[id].name);
            m_webcamTexture.requestedWidth  = initC.get<int>("requested_width");
            m_webcamTexture.requestedHeight = initC.get<int>("requested_height");
            m_webcamTexture.requestedFPS    = initC.get<int>("requested_fps");        
            m_webcamTexture.Play();

            return true;
        }

        protected override void start_experiment() {
            m_webcamTexture.Play();
        }

        protected override void pre_start_routine() {
            updateCounter = 0;
        }

        protected override void update() {

            if (!m_webcamTexture.isPlaying) {
                return;
            }

            var currentTime = time().ellapsed_element_ms();
            double deltaTime = 1000.0 / initC.get<int>("requested_fps");
            if (currentTime / deltaTime > updateCounter) {
                ++updateCounter;
                invoke_signal("new frame", new ImageContainer(m_webcamTexture));
            }
        }

        public override void pause() {
            m_webcamTexture.Pause();
        }

        public override void play() {
            m_webcamTexture.Play();
        }

        protected override void stop_experiment() {
            m_webcamTexture.Stop();
        }
    }
}