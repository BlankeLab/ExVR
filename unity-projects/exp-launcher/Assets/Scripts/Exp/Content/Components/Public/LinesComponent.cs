
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
using System.Collections.Generic;
using Unity.XRTools.Rendering;

// unity
using UnityEngine;

namespace Ex{

    public class LinesComponent : ExComponent{

        List<XRLineRenderer> lines = null;

        protected override bool initialize() {

            lines = new List<XRLineRenderer>();

            //maxLine = ExVR.GlobalResources().get_material("Lines/MinLine");
            //if(maxLine == null) {
            //    log_error("No material.");
            //}
            // slots
            //events().add_slot("set_visibility", (visibility) => { set_visibility((bool)visibility); });
            //events().add_slot("set_position", (position) => { transform.localPosition = (Vector3)position; });
            //events().add_slot("set_rotation", (rotation) => { transform.localEulerAngles = (Vector3)rotation; });
            //events().add_slot("set_scale", (scale) => { transform.localScale = (Vector3)scale; });
            // signals
            //events().add_signal("visibility_changed", 1);

            return true;
        }

        protected override void stop_experiment() {
            remove_lines();
        }

        public override void update_from_current_config() {
            if (!currentC.get<bool>("transform_do_not_apply")) {
                currentC.update_transform("transform", transform, true);
            }
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        protected override void set_visibility(bool visibility) {
            foreach(var line in lines) {
                line.gameObject.SetActive(visibility);
            }
        }

        public int add_line(float width, Color color, Vector3[] positions) {

            var go = ExVR.GlobalResources().instantiate_prebab("Components/MaxLine", transform, "line_" + lines.Count);
            go.transform.position = Vector3.zero;
            go.transform.rotation = Quaternion.identity;
            go.SetActive(is_visible());

            var lineR = go.GetComponent<XRLineRenderer>();
            lineR.SetTotalWidth(width);
            lineR.SetVertexCount(positions.Length);
            lineR.SetPositions(positions);
            lineR.SetTotalColor(color);

            lines.Add(lineR);
            return lines.Count;
        }

        public void set_line_points(int id, Vector3[] positions) {
            if (id < lines.Count) {
                lines[id].SetVertexCount(positions.Length);
                lines[id].SetPositions(positions);
            }
        }

        public void set_line_width(int id, float width) {
            if (id < lines.Count) {
                lines[id].SetTotalWidth(width);
            }
        }

        public void set_line_color(int id, Color color) {
            if (id < lines.Count) {
                lines[id].SetTotalColor(color);
            }
        }

        public void remove_line(int id) {
            if(id < lines.Count) {
                if (lines[id] != null) {
                    Destroy(lines[id].gameObject);
                }
                lines.RemoveAt(id);
            }
        }

        public void remove_lines() {
            foreach (var line in lines) {
                Destroy(line.gameObject);
            }
            lines.Clear();
        }
    }
}