

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
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Threading.Tasks;

// unity
using UnityEngine.Rendering;
using UnityEngine;
using Unity.Collections;

namespace SA {

    [System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
    public struct DLLVertexSA {
        public Vector3 pos;
        public Color32 col;
    }

    public class NativeDLLVerticesSA {

        public DLLVertexSA[] data = null;
        public NativeArray<DLLVertexSA> native;

        public NativeDLLVerticesSA(int nbVertices) {
            data = new DLLVertexSA[nbVertices];
            native = new NativeArray<DLLVertexSA>(data, Allocator.Persistent);
        }

        public void clean() {
            native.Dispose();
        }
    }

    public class NativeIndicesSA {

        public int[] data = null;
        public NativeArray<int> native;

        public NativeIndicesSA(int nbIndices) {

            data = new int[nbIndices];
            for (int ii = 0; ii < nbIndices; ++ii) {
                data[ii] = ii;
            }
            native = new NativeArray<int>(data, Allocator.Persistent);
        }

        public void clean() {
            native.Dispose();
        }
    }

    [System.Serializable]
    public class TransformValueSA {

        public TransformValueSA() { }
        public TransformValueSA(Vector3 position, Quaternion rotation, Vector3 scale) {
            this.position = position;
            this.rotation = rotation;
            this.scale = scale;
        }

        public Vector3 position = Vector3.zero;
        public Quaternion rotation = Quaternion.identity;
        public Vector3 scale = Vector3.one;
    }

    [System.Serializable]
    public class OBBFInfoSA {
        public bool enabled = false;
        public bool display = false;
        public Color color = new Color(1, 0, 0, 0.2f);
        public TransformValueSA transform = new TransformValueSA();
    }

    [RequireComponent(typeof(MeshFilter), typeof(MeshRenderer))]
    public class PointCloudSA : MonoBehaviour {

        static public Material pointCloudMat = null;
        public enum RenderingType {
            Quad = 0, ParabloidFrag, ParabloidGeo
        };

        public enum ParabloidGeoDetails {
            None = 0, Low, Medium, Hight
        };

        private Shader quadShader = null;
        private Shader paraboloidFrag = null;
        private Shader paraboloidGeoWorld = null;
        private RenderingType currentRendering = RenderingType.ParabloidGeo;

        public void Awake() {

            Mesh mesh = new Mesh();
            mesh.indexFormat = UnityEngine.Rendering.IndexFormat.UInt32;
            mesh.triangles = new int[0];
            GetComponent<MeshFilter>().mesh = mesh;

            var renderer = GetComponent<MeshRenderer>();
            renderer.material = Instantiate(pointCloudMat);
            renderer.shadowCastingMode = UnityEngine.Rendering.ShadowCastingMode.Off;
            renderer.receiveShadows = false;
            renderer.lightProbeUsage = UnityEngine.Rendering.LightProbeUsage.Off;
            renderer.reflectionProbeUsage = UnityEngine.Rendering.ReflectionProbeUsage.Off;
            renderer.rendererPriority = 3000;

            quadShader          = Shader.Find("Custom/Cloud/QuadGeoWorldSizeShaderSA");
            paraboloidFrag      = Shader.Find("Custom/Cloud/ParaboloidFragWorldSizeShaderSA");
            paraboloidGeoWorld  = Shader.Find("Custom/Cloud/ParaboloidGeoWorldSizeShaderSA");

            set_rendering(currentRendering);
        }


        public void set_as_dynamic() {
            GetComponent<MeshFilter>().mesh.MarkDynamic();
        }

        public bool set_points(NativeDLLVerticesSA vertices, NativeIndicesSA indices, int count) {

            if (count > vertices.data.Length || count > indices.data.Length) {
                Debug.LogError("PointCloud: Invalid inputs.");
                return false;
            }

            var layout = new[]{
                new VertexAttributeDescriptor(VertexAttribute.Position, VertexAttributeFormat.Float32, 3),
                new VertexAttributeDescriptor(VertexAttribute.Color, VertexAttributeFormat.UNorm8, 4)
            };

            var mesh = GetComponent<MeshFilter>().mesh;
            mesh.Clear();

            MeshUpdateFlags flags =
              MeshUpdateFlags.DontValidateIndices
            | MeshUpdateFlags.DontNotifyMeshUsers
            | MeshUpdateFlags.DontResetBoneBounds;

            mesh.SetVertexBufferParams(count, layout);
            mesh.SetVertexBufferData(vertices.native, 0, 0, count, 0, flags);
            mesh.SetIndexBufferParams(count, IndexFormat.UInt32);
            mesh.SetIndexBufferData(indices.native, 0, 0, count, flags);
            mesh.SetSubMesh(0, new SubMeshDescriptor(0, count, MeshTopology.Points), flags);
            mesh.bounds = mesh.GetSubMesh(0).bounds;

            return true;
        }

        public void set_rendering(RenderingType rendering) {

            if (currentRendering == rendering) {
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
            if (currentRendering == RenderingType.ParabloidGeo) {
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

        public void set_filtering_obb_infos(List<OBBFInfoSA> infos) {

            var material = GetComponent<MeshRenderer>().material;
            Vector4[] positions = new Vector4[infos.Count];
            Matrix4x4[] rotations = new Matrix4x4[infos.Count];
            Vector4[] sizes = new Vector4[infos.Count];

            for (int ii = 0; ii < infos.Count; ++ii) {
                positions[ii] = infos[ii].transform.position;
                rotations[ii] = Matrix4x4.Rotate(infos[ii].transform.rotation).inverse;
                sizes[ii] = infos[ii].transform.scale * 0.5f;
                sizes[ii].w = infos[ii].enabled ? 1f : 0f;
            }

            material.SetVectorArray("_ObbsPos", positions);
            material.SetVectorArray("_ObbsSize", sizes);
            material.SetMatrixArray("_ObbsOrientation", rotations);
        }
    }


}