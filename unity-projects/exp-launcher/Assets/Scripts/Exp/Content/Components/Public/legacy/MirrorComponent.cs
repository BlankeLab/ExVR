
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

    public class MirrorComponent : ExComponent {

        GameObject m_mirrorGO = null;

        protected override bool initialize() {
            m_mirrorGO = ExVR.GlobalResources().instantiate_prebab("Components/Mirror", transform);
            m_mirrorGO.name = "Mirror";
            return true;
        }


        public override void update_from_current_config() {

            if (!currentC.get<bool>("transform_do_not_apply")) {
                currentC.update_transform("transform", m_mirrorGO.transform, true);
            }
            m_mirrorGO.transform.Find("MirrorFrame").gameObject.SetActive(currentC.get<bool>("show_frame"));            
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        protected override void set_visibility(bool visibility) {
            m_mirrorGO.SetActive(visibility);
        }

        public void apply_mirror_only_layer(GameObject go) {

            foreach (Transform tr in go.GetComponentsInChildren<Transform>(true)) {
                tr.gameObject.layer = Layers.MirrorOnly;
            }
        }
    }
}