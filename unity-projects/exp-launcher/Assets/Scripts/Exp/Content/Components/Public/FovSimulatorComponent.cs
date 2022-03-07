
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
using System.Diagnostics;
using System.Collections;
using System.Collections.Generic;

// unity
using UnityEngine;
//using UnityEngine.VR;
using UnityEngine.XR;

namespace Ex{

    public class FovSimulatorComponent : ExComponent{

        MeshRenderer mr = null;
        MeshFilter mf = null;

        protected override bool initialize() {

            // generate cube
            // # mesh rendering
            mr = gameObject.AddComponent<MeshRenderer>();
            mr.material = ExVR.GlobalResources().instantiate_mat("Components/FovSimulator/TransparentScreen");
            mr.enabled = false;
            
            // #mesh filtering
            mf = gameObject.AddComponent<MeshFilter>();

            float vFov = initC.get<float>("vertical_fov");//52.1f;
            float hFov = initC.get<float>("horizontal_fov");// 34f;
            float distance = initC.get<float>("distance");// 0.15f;
            
            float inf1W = 0.15f;
            float inf1H = 0.15f;
            float inf2W = 0.20f;
            float inf2H = 0.20f;

            float W = 2f * (Mathf.Tan(Mathf.Deg2Rad * vFov * 0.5f)) * distance;
            float H = 2f * (Mathf.Tan(Mathf.Deg2Rad * hFov * 0.5f)) * distance;
            float halfW = W * 0.5f;
            float halfH = H * 0.5f;

            //float depth0 =  0.0f;// .2f;
            float depth1 = -0.0f;
            float depth2 = -0.3f;

            Vector3[] vertices = new Vector3[24];

            vertices[0] = new Vector3(-inf2H, -inf2W, depth2);
            vertices[1] = new Vector3(-inf2H, 0, depth2);
            vertices[2] = new Vector3(-inf2H, +inf2W, depth2);
            vertices[3] = new Vector3(0, +inf2W, depth2);
            vertices[4] = new Vector3(+inf2H, +inf2W, depth2);
            vertices[5] = new Vector3(+inf2H, 0, depth2);
            vertices[6] = new Vector3(+inf2H, -inf2W, depth2);
            vertices[7] = new Vector3(0, -inf2W, depth2);

            vertices[8]  = new Vector3(-inf1H, -inf1W, depth1);
            vertices[9]  = new Vector3(-inf1H,  0,     depth1);
            vertices[10] = new Vector3(-inf1H, +inf1W, depth1);
            vertices[11] = new Vector3(0,      +inf1W, depth1);
            vertices[12] = new Vector3(+inf1H, +inf1W, depth1);
            vertices[13] = new Vector3(+inf1H, 0,      depth1);
            vertices[14] = new Vector3(+inf1H, -inf1W, depth1);
            vertices[15] = new Vector3(0,      -inf1W, depth1);

            vertices[16] = new Vector3(-halfH, -halfW, 0);
            vertices[17] = new Vector3(-halfH, +halfW, 0);
            vertices[18] = new Vector3(+halfH, +halfW, 0);
            vertices[19] = new Vector3(+halfH, -halfW, 0);

            //vertices[20] = new Vector3(-halfH, -halfW, depth0);
            //vertices[21] = new Vector3(-halfH, +halfW, depth0);
            //vertices[22] = new Vector3(+halfH, +halfW, depth0);
            //vertices[23] = new Vector3(+halfH, -halfW, depth0);

            int[] triangles = {
                0,1,8, 1,10,8, 1,2,10,
                2,3,10, 3,12,10, 3,4,12,
                4,5,12, 5,14,12, 5,6,14,
                6,7,14, 7,8,14, 7,0,8,
                8,9,16, 9,17,16, 9,10,17,
                10,11,17, 11,18,17, 11,12,18,
                12,13,18, 13,19,18, 13,14,19,
                14,15,19, 15,16,19, 15,8,16,
                //20,16,17, 20,17,21,
                //21,17,18, 21,18,22,
                //22,18,19, 22,19,23,
                //23,19,16, 23,16,20
            };

            Mesh mesh = new Mesh();
            mesh.vertices  = vertices;
            mesh.triangles = triangles;  
            mf.mesh = mesh;

            return true;
        }

        protected override void pre_update() {
            var eyeTr = ExVR.Display().cameras().get_eye_camera_transform();
            float distance = 0.15f;
            transform.position = eyeTr.position + eyeTr.forward * distance;
            transform.rotation = eyeTr.rotation;
        }

        public override void update_from_current_config() {
            var color = currentC.get_color("color");
            mr.material.color = color;
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        protected override void set_visibility(bool visibility) {
            GetComponent<MeshRenderer>().enabled = visibility;
        }

        protected override void clean() {
            Destroy(mr.material);
        }
    }
}