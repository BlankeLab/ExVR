
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

    public class PlanarReflectionManagement : MonoBehaviour{

        public GameObject m_reflectionPlane = null;

        public Camera m_reflectionCamera = null;
        public Camera m_mainCamera = null;

        public Material m_planeMaterial = null;

        public RenderTexture m_renderTarget = null;

        [Range(0.0f, 1.0f)]
        public float m_reflectionFactor = 0.5f;        

        public void initialize(GameObject reflectionPlane, Material planeMaterial, Camera mainCamera) {

            GameObject reflectionCameraGo = new GameObject("ReflectionCamera");
            reflectionCameraGo.transform.SetParent(transform);
            m_reflectionCamera = reflectionCameraGo.AddComponent<Camera>();
            m_reflectionCamera.enabled = false;
            m_renderTarget = new RenderTexture(Screen.width, Screen.height, 24);

            m_mainCamera = mainCamera;
            m_reflectionPlane = reflectionPlane;

            m_planeMaterial = planeMaterial;

            Camera.onPostRender += render_reflection;
        }

        public void clean(){
            Camera.onPostRender -= render_reflection;
        }

        // Update is called once per frame
        void Update() {
            Shader.SetGlobalFloat("_reflectionFactor", m_reflectionFactor);
        }

        void render_reflection(Camera camera) {

            if(m_mainCamera.GetInstanceID() != camera.GetInstanceID()) {
                return;
            }

            m_reflectionCamera.CopyFrom(m_mainCamera);

            Vector3 cameraDirectionWorldSpace = m_mainCamera.transform.forward;
            Vector3 cameraUpWorldSpace = m_mainCamera.transform.up;
            Vector3 cameraPositionWorldSpace = m_mainCamera.transform.position;

            // transform the vectors to the floor's space
            Vector3 cameraDirectionPlaneSpace = m_reflectionPlane.transform.InverseTransformDirection(cameraDirectionWorldSpace);
            Vector3 cameraUpPlaneSpace = m_reflectionPlane.transform.InverseTransformDirection(cameraUpWorldSpace);
            Vector3 cameraPositionPlaneSpace = m_reflectionPlane.transform.InverseTransformPoint(cameraPositionWorldSpace);

            // mirror the vectores
            cameraDirectionPlaneSpace.y *= -1.0f;
            cameraUpPlaneSpace.y *= -1.0f;
            cameraPositionPlaneSpace.y *= -1.0f;

            // transform the vectors back to world space
            cameraDirectionWorldSpace = m_reflectionPlane.transform.TransformDirection(cameraDirectionPlaneSpace);
            cameraUpWorldSpace = m_reflectionPlane.transform.TransformDirection(cameraUpPlaneSpace);
            cameraPositionWorldSpace = m_reflectionPlane.transform.TransformPoint(cameraPositionPlaneSpace);

            // set camera position and rotation
            m_reflectionCamera.transform.position = cameraPositionWorldSpace;
            m_reflectionCamera.transform.LookAt(cameraPositionWorldSpace + cameraDirectionWorldSpace, cameraUpWorldSpace);

            // set render target for the reflection camera
            m_reflectionCamera.targetTexture = m_renderTarget;

            // render the reflection camera
            m_reflectionCamera.Render();

            // draw full screen quad
            draw_quad();
        }

        void draw_quad() {

            GL.PushMatrix();

            m_planeMaterial.SetPass(0);
            m_planeMaterial.SetTexture("_ReflectionTex", m_renderTarget);

            GL.LoadOrtho();
            GL.Begin(GL.QUADS);
            GL.TexCoord2(1.0f, 0.0f);
            GL.Vertex3(0.0f, 0.0f, 0.0f);
            GL.TexCoord2(1.0f, 1.0f);
            GL.Vertex3(0.0f, 1.0f, 0.0f);
            GL.TexCoord2(0.0f, 1.0f);
            GL.Vertex3(1.0f, 1.0f, 0.0f);
            GL.TexCoord2(0.0f, 0.0f);
            GL.Vertex3(1.0f, 0.0f, 0.0f);
            GL.End();
            GL.PopMatrix();
        }
    }
}