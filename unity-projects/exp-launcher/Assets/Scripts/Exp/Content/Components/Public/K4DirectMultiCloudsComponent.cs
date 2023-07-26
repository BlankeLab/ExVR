

/*******************************************************************************
** exvr-exp                                                                   **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

// system
using System.Collections.Generic;

// unity
using UnityEngine;
using UnityEngine.Profiling;

namespace Ex {

    public class K4DirectMultiCloudsComponent : ExComponent {
        
        private K4ManagerComponent m_kinectM = null;

        private GameObject m_parentCloudsGO = null;
        private List<GameObject> m_cloudsCustomTrGO = null; // custom transform
        private List<GameObject> m_cloudsConfigTrGO = null; // # config transfrom
        private List<GameObject> m_cloudsCalibTrGO = null;  // ## calibration transform

        private GameObject m_OBBsParent   = null;
        private List<GameObject> m_OBBsGO = null;
        private List<OBBFInfo> m_OBBsInfo = null;

        private List<bool> m_cloudUpdated = null;

        #region ex_functions

        protected override bool initialize() {

            // get kinect manager
            m_kinectM = initC.get_component<K4ManagerComponent>("k4_manager");
            if (m_kinectM == null) {
                log_error("No kinect manager component set. Scene will not be displayed.");
                return true;
            }

            // init parents GO
            m_parentCloudsGO = GO.generate_empty_scene_object("clouds", transform, true);
            GO.init_local_scaling(m_parentCloudsGO, new Vector3(-1, 1, 1)); // invert y scale 

            //// # clouds
            m_cloudsCustomTrGO = new List<GameObject>();
            m_cloudsConfigTrGO = new List<GameObject>();
            m_cloudsCalibTrGO = new List<GameObject>();
            m_cloudUpdated = new List<bool>();

            // obb
            m_OBBsGO     = new List<GameObject>(10);
            m_OBBsInfo   = new List<OBBFInfo>(10);
            m_OBBsParent = GO.generate_empty_scene_object("OBBs", m_parentCloudsGO.transform, true);
            m_OBBsParent.transform.localPosition = Vector3.zero;
            m_OBBsParent.transform.localEulerAngles = Vector3.zero;
            m_OBBsParent.transform.localScale = Vector3.one;

            for (int ii = 0; ii < 10; ++ii) {
                var obbGO = GO.generate_cube("filtering obb", m_OBBsParent.transform, 1f, null, -1, ExVR.GlobalResources().instantiate_default_transparent_mat());
                obbGO.SetActive(false);
                m_OBBsGO.Add(obbGO);
                m_OBBsInfo.Add(new OBBFInfo());
            }

            // add clouds                        
            for (int ii = 0; ii < m_kinectM.connections_nb(); ++ii) {

                GameObject cloudCustomGO = GO.generate_empty_scene_object("custom_tr_cloud_" + ii, m_parentCloudsGO.transform, true);
                GO.init_local_scaling(cloudCustomGO, new Vector3(1, 1, 1));
                GameObject cloudConfigGO = GO.generate_empty_scene_object("config_tr_cloud_" + ii, cloudCustomGO.transform, true);
                GO.init_local_scaling(cloudConfigGO, new Vector3(1, 1, 1));
                GameObject cloudCalibGO = GO.generate_empty_scene_object("calib_tr_cloud_" + ii, cloudConfigGO.transform, true);
                GO.init_local_scaling(cloudCalibGO, new Vector3(1, 1, 1));

                m_cloudsCustomTrGO.Add(cloudCustomGO);
                m_cloudsConfigTrGO.Add(cloudConfigGO);
                m_cloudsCalibTrGO.Add(cloudCalibGO);
                m_cloudUpdated.Add(false);

                var pc = cloudCalibGO.AddComponent<PointCloud>();
                pc.set_as_dynamic();
                pc.set_rendering(PointCloud.RenderingType.ParabloidGeo);
            }

            // apply model matrices
            for (int ii = 0; ii < m_kinectM.connections_nb(); ++ii) {
                Matrix4x4 calibM = m_kinectM.model_matrix(ii);
                m_cloudsCalibTrGO[ii].transform.localRotation = calibM.rotation;
                m_cloudsCalibTrGO[ii].transform.localPosition = calibM.GetPosition();
            }

            return true;
        }

        protected override void start_experiment() {

            foreach (var go in m_cloudsCustomTrGO) {
                go.transform.localPosition  = Vector3.zero;
                go.transform.localRotation  = Quaternion.identity;
                go.transform.localScale = new Vector3(1, 1, 1);
            }
        }

        protected override void set_visibility(bool visibility) {

            m_parentCloudsGO.SetActive(visibility && currentC.get<bool>("display_clouds"));

            for (int ii = 0; ii < m_OBBsGO.Count; ++ii) {
                m_OBBsGO[ii].SetActive(visibility && currentC.get<bool>("display_filtering_obb") && m_OBBsInfo[ii].display);
            }
        }

        public override void update_from_current_config() {

            if (!currentC.get<bool>("global_transform_do_not_apply")) {
                foreach (var go in m_cloudsConfigTrGO) {
                    currentC.update_transform("global_transform", go.transform, true);
                }
                currentC.update_transform("global_transform", m_OBBsParent.transform, true);                
            }

            // obb
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

                Apply.to_transform(m_OBBsInfo[ii].transform, m_OBBsGO[ii].transform, true);
                m_OBBsGO[ii].GetComponent<MeshRenderer>().material.SetColor("_Color", m_OBBsInfo[ii].color);
            }


            // point cloud shading
            float sizePoints = currentC.get<float>("size_points");
            bool removeOutside = currentC.get<bool>("filter_points_outside_obb");
            bool cones = currentC.get<bool>("cones");
            bool circles = currentC.get<bool>("circles");
            Color tintColor = currentC.get_color("tint");
            var details = (PointCloud.ParabloidGeoDetails)currentC.get<int>("details");
            var rendering = (PointCloud.RenderingType)currentC.get<int>("rendering");

            for (int ii = 0; ii < m_kinectM.connections_nb(); ++ii) {

                var cloud = m_cloudsCalibTrGO[ii];
                var pc = cloud.GetComponent<PointCloud>();
                pc.set_pt_size(sizePoints);
                pc.set_rendering(rendering);                
                pc.set_circles_state(circles);
                pc.set_paraboloid_frag_cones_state(cones);
                pc.set_paraboloid_geo_details(details);
                pc.set_tint(tintColor);
                pc.set_obb_filtering_state(removeOutside);
                pc.set_filtering_obb_infos(m_OBBsInfo);
            }
      
            set_visibility(is_visible());
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        protected override void update() {

            if (m_kinectM == null) {
                return;
            }

            Profiler.BeginSample("[ExVR][K4DirectMultiCloudsComponent] update_mesh_from_cloud all");
            for (int ii = 0; ii < m_kinectM.connections_nb(); ++ii) {
                m_cloudUpdated[ii] = m_kinectM.update_cloud(ii, m_cloudsCalibTrGO[ii].GetComponent<PointCloud>());
            }
            Profiler.EndSample();
        }

        #endregion
        #region public_functions

        public void set_cloud_custom_transform(int id, TransformValue tv, bool global) {

            if (id >= m_cloudsCustomTrGO.Count) {
                return;
            }

            if (!global) {
                m_cloudsCustomTrGO[id].transform.localPosition = tv.position;
                m_cloudsCustomTrGO[id].transform.localRotation = tv.rotation;
                m_cloudsCustomTrGO[id].transform.localScale = tv.scale;
            } else {
                m_cloudsCustomTrGO[id].transform.position = tv.position;
                m_cloudsCustomTrGO[id].transform.rotation = tv.rotation;
                m_cloudsCustomTrGO[id].transform.localScale = tv.scale;
            }
        }

        public GameObject cloud(int id) {
            if (id >= m_cloudsCalibTrGO.Count) {
                return null;
            }
            return m_cloudsCalibTrGO[id];
        }

        public int cloud_count() {
            return m_kinectM.connections_nb();
        }

        public bool cloud_updated(int id) {
            if (id >= m_cloudUpdated.Count) {
                return false;
            }
            return m_cloudUpdated[id];
        }

        #endregion
    }
}
