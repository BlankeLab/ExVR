
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

// unity
using UnityEngine;

namespace Ex {

    public class FlagPoleComponent : ExComponent {

        private SkinnedMeshRenderer smr = null;
        private Cloth flagCloth = null;

        private GameObject flagGO = null;
        private GameObject poleGO = null;
        private GameObject baseGO = null;
        private GameObject ballGO = null;

        private ClothSkinningCoefficient[] coeffs = null;
        private List<int> poleCoeffs = new List<int>();
        private Dictionary<int, float> topCoeffs = new Dictionary<int, float>();

        private Material flagPoleMaterial = null;

        #region ex_functions
        protected override bool initialize() {

            add_slot("hoist", (factor) => {
                var v = Mathf.Clamp((float)factor, 0f, 1f);
                udpate_flag_cloth_max_distance(0.5f - v*0.45f);
                update_flag_height(v);            
            });

            float width  = initC.get<float>("flag_width");
            float height = initC.get<float>("flag_height");
            float depth  = 0.001f;

            int nbVerticesH = initC.get<int>("flag_nb_vertices_horiz");
            int nbVerticesV = initC.get<int>("flag_nb_vertices_verti");

            // pole
            {
                flagPoleMaterial = ExVR.GlobalResources().instantiate_mat("Components/FlagPole/Pole");

                poleGO = new GameObject("Pole");
                poleGO.transform.SetParent(transform);                
                poleGO.AddComponent<MeshFilter>().mesh = PrimitivesMesh.CylinderBuilder.generate(10, 3, 0.025f, initC.get<float>("pole_height"));
                poleGO.AddComponent<MeshRenderer>().sharedMaterial = flagPoleMaterial;

                baseGO = new GameObject("Base");
                baseGO.transform.SetParent(transform);
                baseGO.AddComponent<MeshFilter>().mesh = PrimitivesMesh.CylinderBuilder.generate(30, 3, 0.25f, 0.015f);
                baseGO.AddComponent<MeshRenderer>().sharedMaterial = flagPoleMaterial;

                ballGO = new GameObject("ball");
                ballGO.transform.SetParent(transform);
                ballGO.AddComponent<MeshFilter>().mesh = PrimitivesMesh.SphereBuilder.generate(0.06f);
                ballGO.AddComponent<MeshRenderer>().sharedMaterial = flagPoleMaterial;
                ballGO.transform.position += new Vector3(0, initC.get<float>("pole_height") + 0.06f, 0);
            }

            // flag
            {
                flagGO = new GameObject("Flag");
                flagGO.transform.SetParent(transform);
                
                var mesh = PrimitivesMesh.GridBuilder.generate(nbVerticesH, nbVerticesV, width, height, depth);
                flagGO.AddComponent<MeshFilter>().mesh = mesh;
                var collider = flagGO.AddComponent<SphereCollider>();
                collider.radius = Mathf.Min(width, height);
                smr          = flagGO.AddComponent<SkinnedMeshRenderer>();
                smr.material = ExVR.GlobalResources().instantiate_mat("Components/FlagPole/Grid");                
                smr.sharedMesh = mesh;

                flagCloth = flagGO.AddComponent<Cloth>();
                flagCloth.randomAcceleration = new Vector3(0.1f, 0.1f, 0.1f);
                coeffs    = flagCloth.coefficients;

                float maxY = -1f;
                float minX = 10f;
                float maxX = -10f;
                for (int ii = 0; ii < coeffs.Length; ++ii) {

                    if (minX > flagCloth.vertices[ii].x) {
                        minX = flagCloth.vertices[ii].x;
                    }

                    if (maxX < flagCloth.vertices[ii].x) {
                        maxX = flagCloth.vertices[ii].x;
                    }

                    if (maxY < flagCloth.vertices[ii].y) {
                        maxY = flagCloth.vertices[ii].y;
                    }
                    if (UnityEngine.Mathf.Approximately(flagCloth.vertices[ii].x, 0f)) {
                        poleCoeffs.Add(ii);
                    }
                }

                for (int ii = 0; ii < coeffs.Length; ++ii) {
                    if (UnityEngine.Mathf.Approximately(flagCloth.vertices[ii].y, maxY)) {
                        topCoeffs[ii] = (flagCloth.vertices[ii].x - minX) / maxX;
                    }
                }

                flagCloth.capsuleColliders = new CapsuleCollider[] {
                    poleGO.AddComponent<CapsuleCollider>()
                };

                udpate_flag_cloth_max_distance(0f);
            }

            poleGO.SetActive(false);
            flagGO.SetActive(false);
            baseGO.SetActive(false);
            ballGO.SetActive(false);

            return true;
        }

        protected override void start_experiment() {
            reset_init_transform();
        }

        public override void update_from_current_config() {
            reset_config_transform();
            udpate_flag_cloth_max_distance(currentC.get<float>("cloth_max_dist"));
            update_flag_height(currentC.get<float>("height"));
        }
        protected override void pre_start_routine() {
            update_image();
        }

        protected override void update() {
            var collider = flagGO.GetComponent<SphereCollider>();

            var mean = Vector3.zero;
            foreach(var vertex in flagCloth.vertices) {
                mean += vertex;
            }
            mean /= flagCloth.vertices.Length;
            collider.center = mean;            
        }


        protected override void set_visibility(bool visibility) {
            flagGO.SetActive(visibility);
            poleGO.SetActive(visibility);
            baseGO.SetActive(visibility);
            ballGO.SetActive(visibility);
        }

        protected override void update_parameter_from_gui(string updatedArgName) {

            if (updatedArgName == "flag_image") {
                update_image();
            }
            update_from_current_config();
        }

        #endregion
        #region private_functions

        private void update_image() {
            var image = currentC.get_resource_image("flag_image", false);
            if (image != null) {
                smr.material.mainTexture = image;
            }
        }

        #endregion
        #region public_functions
        public void udpate_flag_cloth_max_distance(float maxDistance) {

            for (int ii = 0; ii < coeffs.Length; ++ii) {
                coeffs[ii].maxDistance = maxDistance;
            }

            foreach (var coeff in topCoeffs) {
                coeffs[coeff.Key].maxDistance = coeff.Value * maxDistance;
            }

            foreach (var id in poleCoeffs) {
                coeffs[id].maxDistance = 0f;
            }
            flagCloth.coefficients = coeffs;
        }

        public void update_flag_height(float factor) {
            flagGO.transform.localPosition = new Vector3(0.025f, factor * (initC.get<float>("pole_height") - initC.get<float>("flag_height")), 0);
        }

        public Texture2D current_image() {
            return (Texture2D)smr.material.mainTexture;
        }

        public void load_image_from_resource(string imageAlias) {
            if (imageAlias.Length != 0) {
                smr.material.mainTexture = ExVR.Resources().get_image_file_data(imageAlias).texture;
            }
        }

        public void set_pole_color(Color color) {
            flagPoleMaterial.color = color;
        }

        public SphereCollider flag_cloth_collider() {
            return flagGO.GetComponent<SphereCollider>();
        }

        #endregion
    }
}