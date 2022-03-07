
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
using System.IO;

// unity
using UnityEngine;

namespace Ex{

    public class CloudComponent : ExComponent{

        GameObject cloudGO = null;

        protected override bool initialize() {

            // slots
            add_slot("set visibility", (visibility) => { set_visibility((bool)visibility); });
            add_slot("set position", (position) => { transform.localPosition = (Vector3)position; });
            add_slot("set rotation", (rotation) => { transform.localEulerAngles = (Vector3)rotation; });
            add_slot("set scale", (scale) => { transform.localScale = (Vector3)scale; });
            // signals
            add_signal("visibility changed");

            string alias = initC.get_resource_alias("cloud");
            if(alias.Length == 0) {
                log_error("No cloud alias defined.");
                return false;
            }

            var cloudData = ExVR.Resources().get_cloud_file_data(alias);
            string path = cloudData.path;
            string extension = System.IO.Path.GetExtension(path);

            List<Vector3> vertices = new List<Vector3>();
            List<Color> colors = new List<Color>();

            if (extension == ".ply") {
                // ...
            } else if (extension == ".asc") {

                var lines = File.ReadLines(path);
                foreach (var line in lines) {
                    var split = line.Split(' ');
                    if(split.Length > 5) {
                        vertices.Add(new Vector3(Converter.to_float(split[0]), Converter.to_float(split[1]), Converter.to_float(split[2])));
                        colors.Add(new Color(Converter.to_int(split[3])/255f, Converter.to_int(split[4]) / 255f, Converter.to_int(split[5]) / 255f));
                    }
                }
            } else {
                ExVR.Log().error("Extension not managed.");
                return false;
            }

            // generate cloud gameobject
            cloudGO = GO.generate_empty_scene_object("cloud", transform, false);
            var mRenderer = cloudGO.AddComponent<MeshRenderer>();
            mRenderer.material = ExVR.GlobalResources().instantiate_mat("pointCloud");

            var mFilter   = cloudGO.AddComponent<MeshFilter>();
            Mesh mesh = mFilter.mesh;
            mesh.MarkDynamic();
            mesh.indexFormat = UnityEngine.Rendering.IndexFormat.UInt32;

            int sizeVertices = vertices.Count;
            mesh.vertices = vertices.ToArray();
            mesh.colors = colors.ToArray();
            mesh.triangles = new int[0];

            int[] indices = new int[sizeVertices];
            for (int ii = 0; ii < sizeVertices; ++ii) {
                indices[ii] = ii;
            }
            mesh.SetIndices(indices, MeshTopology.Points, 0);

            return true;
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
            cloudGO.SetActive(visibility);
        }

    }
}