
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
using Ex.XML;
using UnityEngine;

namespace Ex{

    public class FlashingDotComponent : ExComponent{

        GameObject dot = null;

        protected override bool initialize() {

            dot = GameObject.CreatePrimitive(PrimitiveType.Sphere);
            dot.transform.SetParent(transform);
            GO.init_local_transform(dot, new Vector3(), new Vector3(), new Vector3(0.1f, 0.1f, 0.1f));
            dot.layer = Layers.Default;            
            dot.name = "Flashing dot";
            dot.SetActive(false);
            Destroy(dot.GetComponent<SphereCollider>());
            dot.GetComponent<Renderer>().material.shader = Shader.Find("Unlit/Color");

            return true;
        }


        protected override void set_visibility(bool visibility) {
            dot.SetActive(visibility);
        }

        public override void update_from_current_config() {
            if (!currentC.get<bool>("transform_do_not_apply")) {
                currentC.update_transform("transform", dot.transform);
            }
            dot.GetComponent<MeshRenderer>().material.color = currentC.get<bool>("on") ? currentC.get_color("on_color") : currentC.get_color("off_color");
        }


        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }
    }
}