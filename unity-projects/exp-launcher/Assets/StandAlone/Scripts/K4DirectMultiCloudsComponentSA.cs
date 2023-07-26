


/************************************************************************************
** K4DisplayStandAloneSample                                                       **
**                                                                                 **
** MIT License                                                                     **
**                                                                                 **
** Copyright (c) 2023 Florian Lance                                                **
**                                                                                 **
** Permission is hereby granted, free of charge, to any person obtaining a copy    **
** of this software and associated documentation files (the "Software"), to deal   **
** in the Software without restriction, including without limitation the rights    **
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       **
** copies of the Software, and to permit persons to whom the Software is           **
** furnished to do so, subject to the following conditions:                        **
**                                                                                 **
** The above copyright notice and this permission notice shall be included in all  **
** copies or substantial portions of the Software.                                 **
**                                                                                 **
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      **
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        **
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     **
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          **
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   **
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   **
** SOFTWARE.                                                                       **
*************************************************************************************/

// system
using Ex;
using System.Collections.Generic;

// unity
using UnityEngine;


namespace SA {


    public static class ApplySA {

        public static void to_transform(TransformValueSA trV, Transform transform, bool local, bool applyPosition = true, bool applyRotation = true, bool applyScale = true) {

            if (applyScale) {
                transform.localScale = trV.scale;
            }

            if (local) {
                if (applyPosition) {
                    transform.localPosition = trV.position;
                }
                if (applyRotation) {
                    transform.localRotation = trV.rotation;
                }
            } else {
                if (applyPosition) {
                    transform.position = trV.position;
                }
                if (applyRotation) {
                    transform.rotation = trV.rotation;
                }
            }
        }
    }

     public static class GOSA {

        static public GameObject generate_scene_object(string name, System.Type[] components, Transform parent = null, bool active = false, int layer = -1) {

            if (layer < 0) {
                layer = UnityEngine.LayerMask.NameToLayer("Default");
            }

            var go = new GameObject(name, components);
            if (parent != null) {
                go.transform.SetParent(parent);
            }
            init_local_transform(go, Vector3.zero, Vector3.zero, Vector3.one);

            go.SetActive(active);
            go.layer = layer;

            return go;
        }

        static private GameObject generate_geometry_object(Material material, string name, Transform parent, Color? color = null, int layer = -1) {

            var coTypes = new System.Type[] { typeof(MeshRenderer), typeof(MeshFilter) };
            var geoObject = generate_scene_object(name, coTypes, parent, true, layer);


            material.color = (color == null) ? Color.white : color.Value;
            geoObject.GetComponent<MeshRenderer>().material = material;

            return geoObject;
        }

        static public GameObject generate_cube(Material material, string name = "cube", Transform parent = null, float size = 0.05f, Color? color = null, int layer = -1) {
            var cube = generate_geometry_object(material, name, parent, color, layer);
            cube.GetComponent<MeshFilter>().mesh = PrimitivesMesh.CubeBuilderSA.generate(size);
            return cube;
        }

        static public GameObject init_local_scaling(GameObject go, Vector3 scale) {
            go.transform.localScale = scale;
            return go;
        }

        static public GameObject init_local_transform(GameObject go, Vector3 position, Vector3 eulerRotation, Vector3 scale) {
            go.transform.localPosition = position;
            go.transform.localEulerAngles = eulerRotation;
            go.transform.localScale = scale;
            return go;
        }

        static public GameObject generate_empty_object(string name, Transform parent = null, bool active = false) {

            var go = new GameObject(name);
            init_local_transform(go, new Vector3(0, 0, 0), new Vector3(0, 0, 0), new Vector3(1, 1, 1));

            if (parent != null) {
                go.transform.SetParent(parent);
            }
            go.SetActive(active);

            return go;
        }
        static public GameObject generate_empty_scene_object(string name, Transform parent = null, bool active = false, int layer = -1) {

            if (layer < 0) {
                layer = UnityEngine.LayerMask.NameToLayer("Default");
            }

            var go = generate_empty_object(name, parent, active);
            go.layer = layer;
            return go;
        }
    }


    public class K4DirectMultiCloudsComponentSA : MonoBehaviour {

        [Header("Components")]
        public K4ManagerComponentSA m_kinectM = null;

        [Header("Materials")]
        public Material m_oobbMaterial = null;
        public Material m_pointCloudMaterial = null;

        [Header("Clouds rendering settings")]
        public bool displayClouds = true;
        public float sizePoints   = 0.0030f;
        public bool removeOutside = false;
        public bool cones         = true;
        public bool circles       = true;
        public Color tintColor    = new Color(1f, 1f, 1f, 0.0f);
        public PointCloudSA.ParabloidGeoDetails details = PointCloudSA.ParabloidGeoDetails.Hight;
        public PointCloudSA.RenderingType rendering = PointCloudSA.RenderingType.ParabloidGeo;

        // OOBB rendering
        [Header("OOBB settings")]
        public bool displayFilteringObb = false;
        public List<OBBFInfoSA> m_OBBsInfo = null;       

        [Header("States")]
        public GameObject m_parentCloudsGO = null;
        public List<GameObject> m_cloudsCustomTrGO = null; // custom transform
        public List<GameObject> m_cloudsConfigTrGO = null; // # config transfrom
        public List<GameObject> m_cloudsCalibTrGO = null;  // ## calibration transform
        public GameObject m_OBBsParent = null;
        public List<GameObject> m_OBBsGO = null;        
        public List<bool> m_cloudUpdated = null;

        #region mono_behaviour

        private void Start() {

            if (m_kinectM == null) {
                Debug.LogError("No kinect manager component set. Scene will not be displayed.");
                return;
            }

            // init parents GO
            m_parentCloudsGO = GOSA.generate_empty_scene_object("grabbers_cloud", transform, true);
            GOSA.init_local_scaling(m_parentCloudsGO, new Vector3(-1, 1, 1)); // invert x scale 

            //// # clouds
            m_cloudsCustomTrGO = new List<GameObject>();
            m_cloudsConfigTrGO = new List<GameObject>();
            m_cloudsCalibTrGO = new List<GameObject>();
            m_cloudUpdated = new List<bool>();

            // obb
            m_OBBsGO = new List<GameObject>(10);
            m_OBBsInfo = new List<OBBFInfoSA>(10);
            m_OBBsParent = GO.generate_empty_scene_object("OBBs", m_parentCloudsGO.transform, true);
            m_OBBsParent.transform.localPosition    = Vector3.zero;
            m_OBBsParent.transform.localEulerAngles = Vector3.zero;
            m_OBBsParent.transform.localScale       = Vector3.one;

            for (int ii = 0; ii < 10; ++ii) {
                var obbGO = GOSA.generate_cube(Instantiate(m_oobbMaterial), "filtering obb", m_OBBsParent.transform, 1f, null, -1);
                obbGO.SetActive(false);
                m_OBBsGO.Add(obbGO);
                m_OBBsInfo.Add(new OBBFInfoSA());
            }

            // add clouds                        
            PointCloudSA.pointCloudMat = m_pointCloudMaterial;
            for (int ii = 0; ii < m_kinectM.connections_nb(); ++ii) {

                GameObject cloudCustomGO = GOSA.generate_empty_scene_object("custom_tr_cloud_" + ii, m_parentCloudsGO.transform, true);
                GOSA.init_local_scaling(cloudCustomGO, new Vector3(1, 1, 1));
                GameObject cloudConfigGO = GOSA.generate_empty_scene_object("config_tr_cloud_" + ii, cloudCustomGO.transform, true);
                GOSA.init_local_scaling(cloudConfigGO, new Vector3(1, 1, 1));
                GameObject cloudCalibGO = GOSA.generate_empty_scene_object("calib_tr_cloud_" + ii, cloudConfigGO.transform, true);
                GOSA.init_local_scaling(cloudCalibGO, new Vector3(1, 1, 1));

                m_cloudsCustomTrGO.Add(cloudCustomGO);
                m_cloudsConfigTrGO.Add(cloudConfigGO);
                m_cloudsCalibTrGO.Add(cloudCalibGO);
                m_cloudUpdated.Add(false);

                var pc = cloudCalibGO.AddComponent<PointCloudSA>();
                pc.set_as_dynamic();
                pc.set_rendering(PointCloudSA.RenderingType.ParabloidGeo);
            }

            // apply model matrices
            for (int ii = 0; ii < m_kinectM.connections_nb(); ++ii) {
                Matrix4x4 calibM = m_kinectM.model_matrix(ii);
                m_cloudsCalibTrGO[ii].transform.localRotation = calibM.rotation;
                m_cloudsCalibTrGO[ii].transform.localPosition = calibM.GetPosition();
            }

            update_from_settings();

        }



        public void set_visibility(bool visibility) {

            m_parentCloudsGO.SetActive(visibility && displayClouds);

            for (int ii = 0; ii < m_OBBsGO.Count; ++ii) {
                m_OBBsGO[ii].SetActive(visibility && displayFilteringObb && m_OBBsInfo[ii].display);
            }
        }

        public void update_from_settings() {

            for (int ii = 0; ii < m_OBBsGO.Count; ++ii) {
                ApplySA.to_transform(m_OBBsInfo[ii].transform, m_OBBsGO[ii].transform, false);
                m_OBBsGO[ii].GetComponent<MeshRenderer>().material.SetColor("_Color", m_OBBsInfo[ii].color);
            }


            // point cloud shading           
            for (int ii = 0; ii < m_kinectM.connections_nb(); ++ii) {

                var cloud = m_cloudsCalibTrGO[ii];
                var pc = cloud.GetComponent<PointCloudSA>();
                pc.set_pt_size(sizePoints);
                pc.set_rendering(rendering);
                pc.set_circles_state(circles);
                pc.set_paraboloid_frag_cones_state(cones);
                pc.set_paraboloid_geo_details(details);
                pc.set_tint(tintColor);
                pc.set_obb_filtering_state(removeOutside);
                pc.set_filtering_obb_infos(m_OBBsInfo);
            }

            set_visibility(true);
        }


        void Update() {

            if (m_kinectM == null) {
                return;
            }

            update_from_settings();

            for (int ii = 0; ii < m_kinectM.connections_nb(); ++ii) {
                m_cloudUpdated[ii] = m_kinectM.update_cloud(ii, m_cloudsCalibTrGO[ii].GetComponent<PointCloudSA>());
            }
        }

        #endregion

        #region public_functions

        public void set_cloud_custom_transform(int id, TransformValueSA tv, bool global) {

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

        public void set_cloud_config_transform(int id, TransformValueSA tv, bool global) {

            if (id >= m_cloudsConfigTrGO.Count) {
                return;
            }

            if (!global) {
                m_cloudsConfigTrGO[id].transform.localPosition  = tv.position;
                m_cloudsConfigTrGO[id].transform.localRotation  = tv.rotation;
                m_cloudsConfigTrGO[id].transform.localScale     = tv.scale;
            } else {
                m_cloudsConfigTrGO[id].transform.position       = tv.position;
                m_cloudsConfigTrGO[id].transform.rotation       = tv.rotation;
                m_cloudsConfigTrGO[id].transform.localScale     = tv.scale;
            }
        }

        public void set_cloudcalib_transform(int id, TransformValueSA tv, bool global) {

            if (id >= m_cloudsCalibTrGO.Count) {
                return;
            }

            if (!global) {
                m_cloudsCalibTrGO[id].transform.localPosition   = tv.position;
                m_cloudsCalibTrGO[id].transform.localRotation   = tv.rotation;
                m_cloudsCalibTrGO[id].transform.localScale      = tv.scale;
            } else {
                m_cloudsCalibTrGO[id].transform.position        = tv.position;
                m_cloudsCalibTrGO[id].transform.rotation        = tv.rotation;
                m_cloudsCalibTrGO[id].transform.localScale      = tv.scale;
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
