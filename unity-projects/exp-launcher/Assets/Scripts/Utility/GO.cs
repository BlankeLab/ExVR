
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

    public static class GO{

        static public int get_depth_level(Transform tr) {
            int depth = 0;
            while (tr.parent != null) {
                tr = tr.parent;
                depth++;
            }
            return depth;
        }

        static public GameObject init_local_euler_rotation(GameObject go, Vector3 eulerRotation) {
            go.transform.localEulerAngles = eulerRotation;
            return go;
        }

        static public GameObject init_local_scaling(GameObject go, Vector3 scale) {
            go.transform.localScale = scale;
            return go;
        }

        static public GameObject init_local_transform(GameObject go, Vector3 position, Quaternion rotation, Vector3 scale) {
            go.transform.localPosition = position;
            go.transform.localRotation = rotation;
            go.transform.localScale = scale;
            return go;
        }

        static public GameObject init_transform(GameObject go, Vector3 position, Quaternion rotation, Vector3 scale) {
            go.transform.position = position;
            go.transform.rotation = rotation;
            go.transform.localScale = scale;
            return go;
        }

        static public GameObject init_local_transform(GameObject go, Vector3 position, Vector3 eulerRotation, Vector3 scale) {
            go.transform.localPosition = position;
            go.transform.localEulerAngles = eulerRotation;
            go.transform.localScale = scale;
            return go;
        }

        static public GameObject init_transform(GameObject go, Vector3 position, Vector3 eulerRotation, Vector3 scale) {
            go.transform.position = position;
            go.transform.eulerAngles = eulerRotation;
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

            if(layer < 0) {
                layer = Layers.Default;
            }

            var go = generate_empty_object(name, parent, active);
            go.layer = layer;
            return go;
        }

        static public GameObject generate_scene_object(string name, System.Type[] components, Transform parent = null, bool active = false, int layer = -1) {

            if (layer < 0) {
                layer = Layers.Default;
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

        static private GameObject generate_geometry_object(string name, Transform parent, Color? color = null, int layer = -1, Material material = null) {

            var coTypes = new System.Type[] { typeof(MeshRenderer), typeof(MeshFilter) };
            var geoObject = generate_scene_object(name, coTypes, parent, true, layer);

            if (material == null) {
                material = ExVR.GlobalResources().instantiate_default_mat();
            }

            material.color = (color == null) ? Color.white : color.Value;
            geoObject.GetComponent<MeshRenderer>().material = material;

            return geoObject;
        }

        static public GameObject generate_cube(string name = "cube", Transform parent = null, float size = 0.05f, Color? color = null, int layer = -1, Material material = null) {
            var cube = generate_geometry_object(name, parent, color, layer, material);
            cube.GetComponent<MeshFilter>().mesh = PrimitivesMesh.CubeBuilder.generate(size);
            return cube;
        }

        static public GameObject generate_sphere(string name = "sphere", Transform parent = null, float ray = 0.05f, Color? color = null, int layer = -1, Material material = null) {
            var sphere = generate_geometry_object(name, parent, color, layer, material);
            sphere.GetComponent<MeshFilter>().mesh = PrimitivesMesh.SphereBuilder.generate(ray);
            return sphere;
        }

        static public GameObject generate_landmark(string name = "landmark", Transform parent = null, Color? color = null, int layer = -1) {

            if (layer < 0) {
                layer = Layers.Default;
            }

            var coTypes = new System.Type[] { typeof(MeshRenderer), typeof(MeshFilter) };
            var landmark    = generate_empty_object(name, null, true);
            init_local_transform(landmark, new Vector3(0, 0, 0), new Vector3(0, 0, 0), new Vector3(1, 1, 1));
            var subLandmark = generate_empty_object(name, landmark.transform, true);
            init_local_transform(subLandmark, new Vector3(0, 0, 0), new Vector3(0, 0, 0), new Vector3(0.3f, 0.3f, 0.3f));
            subLandmark.name = "top";
            var center      = init_local_scaling(generate_scene_object("center", coTypes, subLandmark.transform, true), new Vector3(0.5f, 0.5f, 0.5f));
            var xAxis       = init_local_scaling(generate_scene_object("x", coTypes, subLandmark.transform, true), new Vector3(10f, 1f, 1f));
            var yAxis       = init_local_scaling(generate_scene_object("y", coTypes, subLandmark.transform, true), new Vector3(1f, 10f, 1f));
            var zAxis       = init_local_scaling(generate_scene_object("z", coTypes, subLandmark.transform, true), new Vector3(1f, 1f,  10f));
            
            var centerMat = ExVR.GlobalResources().instantiate_default_mat();
            centerMat.color = (color == null) ? Color.white : color.Value;
            var xMat = ExVR.GlobalResources().instantiate_default_mat();
            xMat.color = Color.red;
            var yMat = ExVR.GlobalResources().instantiate_default_mat();
            yMat.color = Color.green;
            var zMat = ExVR.GlobalResources().instantiate_default_mat();
            zMat.color = Color.blue;

            center.GetComponent<MeshRenderer>().material = centerMat;
            xAxis.GetComponent<MeshRenderer>().material = xMat;
            yAxis.GetComponent<MeshRenderer>().material = yMat;
            zAxis.GetComponent<MeshRenderer>().material = zMat;

            center.GetComponent<MeshFilter>().mesh = PrimitivesMesh.CubeBuilder.generate(0.1f);

            var cubeM = PrimitivesMesh.CubeBuilder.generate(0.025f);
            xAxis.GetComponent<MeshFilter>().mesh       = cubeM;
            yAxis.GetComponent<MeshFilter>().sharedMesh = cubeM;
            zAxis.GetComponent<MeshFilter>().sharedMesh = cubeM;

            xAxis.transform.localPosition += new Vector3(0.125f, 0f, 0f);
            yAxis.transform.localPosition += new Vector3(0f, 0.125f, 0f);
            zAxis.transform.localPosition += new Vector3(0f, 0f, 0.125f);

            //landmark.layer = layer;
            foreach (Transform child in landmark.transform) {
                child.gameObject.layer = layer;
            }

            if (parent != null) {
                landmark.transform.SetParent(parent);
            }



            return landmark;
        }

        static public GameObject copy(GameObject go, Transform parent, string newName = "", bool worldSpace = false) {
            var cGO = ExVR.GO().instantiate(go, parent, worldSpace);

            if (newName.Length > 0) {
                cGO.transform.name = newName;
            }
            return cGO;

        }
    }
}