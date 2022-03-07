
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

using UnityEngine;

namespace Ex{

    [RequireComponent(typeof(MeshFilter), typeof(MeshRenderer))]
    public class PointCloud : MonoBehaviour{
        public enum RenderingType {
            Quad=0, ParabloidFrag, ParabloidGeo
        };

        public enum ParabloidGeoDetails {
            None = 0, Low, Medium, Hight
        };

        Shader quadShader = null;
        Shader paraboloidFrag = null;
        Shader paraboloidGeoWorld = null;

        RenderingType currentRendering = RenderingType.ParabloidGeo;

        public void Start() {

            Mesh mesh = new Mesh();
            mesh.triangles = new int[0];
            GetComponent<MeshFilter>().mesh = mesh;

            var renderer                  = GetComponent<MeshRenderer>();
            renderer.material             = ExVR.GlobalResources().instantiate_mat("Common/PointCloud");
            renderer.shadowCastingMode    = UnityEngine.Rendering.ShadowCastingMode.Off;
            renderer.receiveShadows       = false;
            renderer.lightProbeUsage      = UnityEngine.Rendering.LightProbeUsage.Off;
            renderer.reflectionProbeUsage = UnityEngine.Rendering.ReflectionProbeUsage.Off;
            
            quadShader         = Shader.Find("Custom/Cloud/QuadGeoWorldSizeShader");
            paraboloidFrag     = Shader.Find("Custom/Cloud/ParaboloidFragWorldSizeShader");
            paraboloidGeoWorld = Shader.Find("Custom/Cloud/ParaboloidGeoWorldSizeShader");

            set_rendering(currentRendering);
        }

        public void set_rendering(RenderingType rendering) {

            if(currentRendering == rendering) {
                return;
            }

            var material = GetComponent<MeshRenderer>().material;
            switch (currentRendering = rendering) {
                case RenderingType.Quad:
                    material.shader = quadShader;
                    break;
                case RenderingType.ParabloidFrag:
                    material.shader = paraboloidFrag;
                    break;
                case RenderingType.ParabloidGeo:
                    material.shader = paraboloidGeoWorld;
                    break;
            }
        }

        public void set_circles_state(bool circle) {
            GetComponent<MeshRenderer>().material.SetInt("_Circles", circle ? 1 : 0);
        }

        public void set_paraboloid_frag_cones_state(bool cone) {
            if (currentRendering == RenderingType.ParabloidFrag) {
                GetComponent<MeshRenderer>().material.SetInt("_Cones", cone ? 1 : 0);
            }
        }

        public void set_paraboloid_geo_details(ParabloidGeoDetails details) {
            if(currentRendering == RenderingType.ParabloidGeo) {
                GetComponent<MeshRenderer>().material.SetInt("_Details", (int)details);
            }
        }

        public void set_obb_filtering_state(bool enabled) {
            GetComponent<MeshRenderer>().material.SetInt("_OBBFiltering", enabled ? 1 : 0);
        }

        public void set_tint(Color color) {
            GetComponent<MeshRenderer>().material.SetColor("_Tint", color);
        }

        public void set_pt_size(float size) {
            GetComponent<MeshRenderer>().material.SetFloat("_PointSize", size);
        }

        public void set_filtering_obb_transform(Transform obbTr) {
            var material = GetComponent<MeshRenderer>().material;
            material.SetVector("_ObbPos", obbTr.position);
            material.SetVector("_ObbSize", obbTr.localScale*0.5f);
            material.SetMatrix("_ObbOrientation", Matrix4x4.Rotate(obbTr.rotation).inverse);
        }
    }


}