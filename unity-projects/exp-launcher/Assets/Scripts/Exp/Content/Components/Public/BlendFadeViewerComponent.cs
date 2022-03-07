
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
    public class BlendFadeViewerComponent : CanvasWorldSpaceComponent{

        private GameObject m_overlayGo = null;
        private BlendFadeOverlay m_overlay = null;

        protected override bool initialize() {

            m_overlayGo = ExVR.GlobalResources().instantiate_prebab("Components/BlendFadeViewer", transform, "Blend fade overlay");
            if(m_overlayGo == null) {
                return false;
            }
            m_overlayGo.transform.localScale = new Vector3(0.5f, 0.5f, 0.5f);
            m_overlay = m_overlayGo.GetComponent<BlendFadeOverlay>();
            m_overlay.initialize();

            return true;
        }

        protected override void start_routine() {

            m_overlay.start_color_change(
                currentC.get<bool>("only_blend"),
                currentC.get_color("start_color"), 
                currentC.get_color("end_color"), 
                currentC.get<float>("duration_blend"),
                currentC.get<float>("duration_fade")
            );
        }

        protected override void set_visibility(bool visibility) {
            m_overlayGo.SetActive(visibility);
        }

        protected override void post_update() {
            m_overlay.update();
            m_overlayGo.transform.position = CameraUtility.eye_camera_position();
        }
    }
}

