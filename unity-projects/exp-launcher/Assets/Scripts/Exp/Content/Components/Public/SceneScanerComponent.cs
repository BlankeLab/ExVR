

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



    public class SceneScanerComponent : ExComponent {

        static public Mesh jointMesh = null;

        private K2ManagerComponent m_kinectM = null;


        private GameObject m_parentCloudsGO = null;
        private List<GameObject> m_cloudsCustomTrGO = null; // custom transform
        private List<GameObject> m_cloudsConfigTrGO = null; // # config transfrom
        private List<GameObject> m_cloudsCalibTrGO = null; // ## calibration transform

        private GameObject m_parentMeshesGO = null;
        private List<GameObject> m_meshesCustomTrGO = null; // custom transfrom
        private List<GameObject> m_meshesConfigTrGO = null; // # config transfrom
        private List<GameObject> m_meshesCalibTrGO = null;  // ## calibration transform

        private GameObject m_parentBodiesGO = null;
        private List<GameObject> m_bodiesCustomTrGO = null; // # config transfrom
        private List<GameObject> m_bodiesConfigTrGO = null; // # config transfrom
        private List<K2BodiesGO> m_bodiesCalibTrGO = null; // ## calibration transform

        private List<GameObject> m_OBBsGO = null;

        private List<OBBFInfo> m_OBBsInfo = null;

        #region ex_functions

        protected override bool initialize() {


            m_OBBsGO = new List<GameObject>(10);
            m_OBBsInfo = new List<OBBFInfo>(10);
            for (int ii = 0; ii < 10; ++ii) {
                var obbGO = GO.generate_cube("filtering obb", transform, 1f, null, -1, ExVR.GlobalResources().instantiate_default_transparent_mat());
                obbGO.SetActive(false);
                m_OBBsGO.Add(obbGO);
                m_OBBsInfo.Add(new OBBFInfo());
            }


            // init parents GO
            m_parentCloudsGO = GO.generate_empty_scene_object("grabbers_cloud", transform, true);
            GO.init_local_scaling(m_parentCloudsGO, new Vector3(1, 1, 1));
            m_parentMeshesGO = GO.generate_empty_scene_object("grabbers_mesh", transform, true);
            GO.init_local_scaling(m_parentMeshesGO, new Vector3(1, 1, 1));
            m_parentBodiesGO = GO.generate_empty_scene_object("grabbers_bodies", transform, true);
            GO.init_local_scaling(m_parentBodiesGO, new Vector3(1, 1, 1));

            // # clouds
            m_cloudsCustomTrGO = new List<GameObject>();
            m_cloudsConfigTrGO = new List<GameObject>();
            m_cloudsCalibTrGO = new List<GameObject>();

            // # meshes                        
            m_meshesCustomTrGO = new List<GameObject>();
            m_meshesConfigTrGO = new List<GameObject>();
            m_meshesCalibTrGO = new List<GameObject>();

            // # bodies
            jointMesh = Ex.PrimitivesMesh.CubeBuilder.generate(1f);
            m_bodiesCustomTrGO = new List<GameObject>();
            m_bodiesConfigTrGO = new List<GameObject>();
            m_bodiesCalibTrGO = new List<K2BodiesGO>();

            // get kinect manager
            m_kinectM = initC.get_component<K2ManagerComponent>("kinect_manager");
            if (m_kinectM == null) {
                log_error("No kinect manager component set. Scene will not be displayed.");
                return true;
            }

            // add clouds                        
            for (int ii = 0; ii < m_kinectM.cameras_nb(); ++ii) {

                GameObject cloudCustomGO = GO.generate_empty_scene_object("custom_tr_cloud_" + ii, m_parentCloudsGO.transform, true);
                GO.init_local_scaling(cloudCustomGO, new Vector3(1, 1, 1));
                GameObject cloudConfigGO = GO.generate_empty_scene_object("config_tr_cloud_" + ii, cloudCustomGO.transform, true);
                GO.init_local_scaling(cloudConfigGO, new Vector3(1, 1, 1));
                GameObject cloudCalibGO = GO.generate_empty_scene_object("calib_tr_cloud_" + ii, cloudConfigGO.transform, true);
                GO.init_local_scaling(cloudCalibGO, new Vector3(1, 1, 1));

                m_cloudsCustomTrGO.Add(cloudCustomGO);
                m_cloudsConfigTrGO.Add(cloudConfigGO);
                m_cloudsCalibTrGO.Add(cloudCalibGO);

                var pc = cloudCalibGO.AddComponent<PointCloud>();
                pc.set_rendering(PointCloud.RenderingType.ParabloidGeo);

                Mesh mesh = cloudCalibGO.GetComponent<MeshFilter>().mesh;
                mesh.MarkDynamic();
                mesh.indexFormat = UnityEngine.Rendering.IndexFormat.UInt32;
            }

            // add meshes            
            for (int ii = 0; ii < m_kinectM.cameras_nb(); ++ii) {

                GameObject meshCustomGO = GO.generate_empty_scene_object("custom_tr_mesh_" + ii, m_parentMeshesGO.transform, true);
                GO.init_local_scaling(meshCustomGO, new Vector3(1, 1, 1));
                GameObject meshConfigGO = GO.generate_empty_scene_object("config_tr_mesh_" + ii, meshCustomGO.transform, true);
                GO.init_local_scaling(meshConfigGO, new Vector3(1, 1, 1));
                GameObject meshCalibGO = ExVR.GlobalResources().instantiate_prebab("Components/SceneScaner/Mesh", meshConfigGO.transform, "calib_tr_mesh_" + ii);
                GO.init_local_scaling(meshCalibGO, new Vector3(1, 1, 1));
                meshCalibGO.SetActive(true);

                m_meshesCustomTrGO.Add(meshCustomGO);
                m_meshesConfigTrGO.Add(meshConfigGO);
                m_meshesCalibTrGO.Add(meshCalibGO);

                Mesh mesh = meshCalibGO.GetComponent<MeshFilter>().mesh;
                mesh.MarkDynamic();
                mesh.indexFormat = UnityEngine.Rendering.IndexFormat.UInt32;
            }

            // add bodies
            for (int ii = 0; ii < m_kinectM.cameras_nb(); ++ii) {

                GameObject bodyCustomGO = GO.generate_empty_scene_object("custom_tr_body_" + ii, m_parentBodiesGO.transform, true);
                GO.init_local_scaling(bodyCustomGO, new Vector3(1, 1, 1));
                GameObject bodyConfigGO = GO.generate_empty_scene_object("config_tr_body_" + ii, bodyCustomGO.transform, true);
                GO.init_local_scaling(bodyConfigGO, new Vector3(1, 1, 1));
                K2BodiesGO bodyCalibGO = new K2BodiesGO(GO.generate_empty_scene_object("calib_tr_body_" + ii, bodyConfigGO.transform, true), jointMesh);

                m_bodiesCustomTrGO.Add(bodyCustomGO);
                m_bodiesConfigTrGO.Add(bodyConfigGO);
                m_bodiesCalibTrGO.Add(bodyCalibGO);
            }

            // apply calibration matrices
            for (int ii = 0; ii < m_kinectM.cameras_nb(); ++ii) {
                Matrix4x4 calibM = m_kinectM.calibration_matrix(ii);
                m_cloudsCalibTrGO[ii].transform.localRotation = calibM.rotation;//etRotation();
                m_cloudsCalibTrGO[ii].transform.localPosition = calibM.GetPosition();
                m_meshesCalibTrGO[ii].transform.localRotation = calibM.rotation;//etRotation();
                m_meshesCalibTrGO[ii].transform.localPosition = calibM.GetPosition();
                m_bodiesCalibTrGO[ii].parent.transform.localRotation = calibM.rotation;
                m_bodiesCalibTrGO[ii].parent.transform.localPosition = calibM.GetPosition();
            }

            return true;
        }

        protected override void set_visibility(bool visibility) {

            m_parentCloudsGO.SetActive(visibility && currentC.get<bool>("display_clouds"));
            m_parentMeshesGO.SetActive(visibility && currentC.get<bool>("display_clouds"));
            m_parentBodiesGO.SetActive(visibility && currentC.get<bool>("display_colliders"));

            for (int ii = 0; ii < m_OBBsGO.Count; ++ii) {
                m_OBBsGO[ii].SetActive(visibility && currentC.get<bool>("display_filtering_obb") && m_OBBsInfo[ii].display);
            }
        }

        protected override void start_experiment() {
            foreach (var go in m_cloudsCustomTrGO) {
                go.transform.localPosition = Vector3.zero;
                go.transform.localRotation = Quaternion.identity;
                go.transform.localScale = Vector3.one;
            }
            foreach (var go in m_meshesCustomTrGO) {
                go.transform.localPosition = Vector3.zero;
                go.transform.localRotation = Quaternion.identity;
                go.transform.localScale = Vector3.one;
            }
            foreach (var go in m_bodiesCustomTrGO) {
                go.transform.localPosition = Vector3.zero;
                go.transform.localRotation = Quaternion.identity;
                go.transform.localScale = Vector3.one;
            }
        }

        public override void update_from_current_config() {

            if (!currentC.get<bool>("global_transform_do_not_apply")) {
                foreach (var go in m_cloudsConfigTrGO) {
                    currentC.update_transform("global_transform", go.transform, true);
                }
                foreach (var go in m_meshesConfigTrGO) {
                    currentC.update_transform("global_transform", go.transform, true);
                }
                foreach (var go in m_bodiesConfigTrGO) {
                    currentC.update_transform("global_transform", go.transform, true);
                }
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


            // apply size points
            if (m_kinectM.mode() == 0) {
                float sizePoints = currentC.get<float>("size_points");
                bool removeOutside = currentC.get<bool>("filter_points_outside_obb");
                bool cones = currentC.get<bool>("cones");
                bool circles = currentC.get<bool>("circles");
                Color tintColor = currentC.get_color("tint");
                var details = (PointCloud.ParabloidGeoDetails)currentC.get<int>("details");
                var rendering = (PointCloud.RenderingType)currentC.get<int>("rendering");

                for (int ii = 0; ii < m_kinectM.cameras_nb(); ++ii) {

                    var cloud = m_cloudsCalibTrGO[ii];
                    var pc = cloud.GetComponent<PointCloud>();
                    pc.set_pt_size(sizePoints);
                    pc.set_rendering(rendering);
                    pc.set_obb_filtering_state(removeOutside);
                    pc.set_circles_state(circles);
                    pc.set_paraboloid_frag_cones_state(cones);
                    pc.set_paraboloid_geo_details(details);
                    pc.set_tint(tintColor);
                    pc.set_filtering_obb_infos(m_OBBsInfo);
                }
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

            // udpate gameobjects                              
            if (m_kinectM.mode() == 0) {
                Profiler.BeginSample("[ExVR][SceneScaner] update_mesh_with_cloud");
                for (int ii = 0; ii < m_kinectM.cameras_nb(); ++ii) {

                    var data = m_kinectM.camera_data(ii);
                    m_cloudsCalibTrGO[ii].SetActive(data.doDisplay);

                    if (data.doUpdate && data.doDisplay) {
                        data.update_mesh_with_cloud(m_cloudsCalibTrGO[ii]);
                        data.update_bodies(m_bodiesCalibTrGO[ii]);
                    }
                }
                Profiler.EndSample();
            } else {
                Profiler.BeginSample("[ExVR][SceneScaner] update_mesh_with_mesh");
                for (int ii = 0; ii < m_kinectM.cameras_nb(); ++ii) {

                    var data = m_kinectM.camera_data(ii);
                    m_meshesCalibTrGO[ii].SetActive(data.doDisplay);

                    if (data.doUpdate && data.doDisplay) {
                        data.update_mesh_with_mesh(m_meshesCalibTrGO[ii]);
                        data.update_bodies(m_bodiesCalibTrGO[ii]);
                    }
                }
                Profiler.EndSample();
            }
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

        public bool is_cloud_updated(int id) {
            if (id >= m_cloudsCalibTrGO.Count) {
                return false;
            }
            return m_kinectM.camera_data(id).doUpdate;
        }

        public int cameras_nb() {
            if (m_kinectM != null) {
                return m_kinectM.cameras_nb();
            }
            return 0;
        }

        public GameObject cloud(int id) {
            if (id >= m_cloudsCalibTrGO.Count) {
                return null;
            }
            return m_cloudsCalibTrGO[id];
        }

        public GameObject mesh(int id) {
            if (id >= m_meshesCalibTrGO.Count) {
                return null;
            }
            return m_meshesCalibTrGO[id];
        }

        public K2BodiesGO body(int id) {
            if (id >= m_bodiesConfigTrGO.Count) {
                return null;
            }
            return m_bodiesCalibTrGO[id];
        }

        #endregion
    }
}
