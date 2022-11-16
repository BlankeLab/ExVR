
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

        private PointCloud pc = null;
        private GameObject cloudGO = null;
        private List<GameObject> m_OBBsGO = null;
        private List<OBBFInfo> m_OBBsInfo = null;
        protected override bool initialize() {

            // slots
            add_slot("visibility", (visibility) => { set_visibility((bool)visibility); });
            add_slot("position", (position) => { transform.localPosition = (Vector3)position; });
            add_slot("rotation", (rotation) => { transform.localEulerAngles = (Vector3)rotation; });
            add_slot("scale", (scale) => { transform.localScale = (Vector3)scale; });
            add_slot("transform", (value) => {
                var transformV = (TransformValue)value;
                transform.localPosition = transformV.position;
                transform.localRotation = transformV.rotation;
            });

            string alias = initC.get_resource_alias("cloud");
            if(alias.Length == 0) {
                log_error("No cloud alias defined.");
                return false;
            }

            


            m_OBBsGO = new List<GameObject>(10);
            m_OBBsInfo = new List<OBBFInfo>(10);
            for (int ii = 0; ii < 10; ++ii) {
                var obbGO = GO.generate_cube("filtering obb", transform, 1f, null, -1, ExVR.GlobalResources().instantiate_default_transparent_mat());
                obbGO.SetActive(false);
                m_OBBsGO.Add(obbGO);
                m_OBBsInfo.Add(new OBBFInfo());
            }

            cloudGO = GO.generate_empty_object("cloud", transform, true);
            pc = cloudGO.AddComponent<PointCloud>();
            var cloudData = ExVR.Resources().get_cloud_file_data(alias);
            
            return pc.set_points(cloudData.vertices, cloudData.colors, cloudData.vertices.Count);
        }

        protected override void start_experiment() {
            if (!initC.get<bool>("init_transform_do_not_apply")) {
                initC.update_transform("init_transform", cloudGO.transform, true);
            }
        }

        public override void update_from_current_config() {

            if (!currentC.get<bool>("transform_do_not_apply")) {
                currentC.update_transform("transform", cloudGO.transform, true);
            }

            var list = currentC.get_list<string>("filtering_obb_tab");
            for (int ii = 0; ii < m_OBBsGO.Count; ++ii) {
                if (ii < list.Count) {
                    var args = Ex.Text.split(list[ii], "[#OBBFW#]");
                    if (args.Length == 4) {
                        m_OBBsInfo[ii].enabled = Converter.to<bool>(args[0]);
                        m_OBBsInfo[ii].display = Converter.to<bool>(args[1]);
                        m_OBBsInfo[ii].color = Converter.to<Color>(args[2]);
                        m_OBBsInfo[ii].transform = Converter.to_transform_value(args[3]);
                    } else {
                        log_error("Invalid filtering obb arg.");
                    }
                } else {
                    m_OBBsInfo[ii].enabled = false;
                    m_OBBsInfo[ii].display = false;
                    m_OBBsInfo[ii].transform = new TransformValue();
                    m_OBBsInfo[ii].color = new Color(1, 0, 0, 0.2f);
                }

                Apply.to_transform(m_OBBsInfo[ii].transform, m_OBBsGO[ii].transform, false);
                m_OBBsGO[ii].GetComponent<MeshRenderer>().material.SetColor("_Color", m_OBBsInfo[ii].color);
            }

            pc.set_pt_size(currentC.get<float>("size_points"));
            pc.set_rendering((PointCloud.RenderingType)currentC.get<int>("rendering"));
            pc.set_obb_filtering_state(currentC.get<bool>("filter_points_outside_obb"));
            pc.set_circles_state(currentC.get<bool>("circles"));
            pc.set_paraboloid_frag_cones_state(currentC.get<bool>("cones"));
            pc.set_paraboloid_geo_details((PointCloud.ParabloidGeoDetails)currentC.get<int>("details"));
            pc.set_tint(currentC.get_color("tint"));
            pc.set_filtering_obb_infos(m_OBBsInfo);

            set_visibility(is_visible());
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        protected override void set_visibility(bool visibility) {

            cloudGO.GetComponent<MeshRenderer>().enabled = visibility;

            for (int ii = 0; ii < m_OBBsGO.Count; ++ii) {
                m_OBBsGO[ii].SetActive(visibility && currentC.get<bool>("display_filtering_obb") && m_OBBsInfo[ii].display);
            }
        }

    }
}