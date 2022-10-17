
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
using System.Runtime.InteropServices;

// unity
using UnityEngine;
using UnityEngine.Profiling;

namespace Ex{

    public static class K2{

        public const int depthWidth = 512;
        public const int depthHeight = 424;
        public const int depthCount = depthWidth * depthHeight;

        public const int nbBodies = 6;
        public const int nbJoints = 25;

        public enum BodyJointType{

            spine_base = 0,
            spine_mid,
            neck,
            head,
            shoulder_left,
            elbow_left,
            wrist_left,
            hand_left,
            shoulder_right,
            elbow_right,
            wrist_right,
            hand_right,
            hip_left,
            knee_left,
            ankle_left,
            foot_left,
            hip_right,
            knee_right,
            ankle_right,
            foot_right,
            spine_shoulder,
            hand_tip_left,
            thumb_left,
            hand_tip_right,
            thumb_right,
        }
        public enum TrackingState{

            not_tracked = 0,
            inferred,
            tracked
        };

    }

    public class K2JointInfo{
        public Vector3 position;
        public Quaternion rotation;
        public K2.TrackingState state;
    }

    public class K2BodyData{
        public bool tracked = false;
        public bool restricted = false;
        public long id = 0;
        public Dictionary<K2.BodyJointType, K2JointInfo> joints = new Dictionary<K2.BodyJointType, K2JointInfo>();
    }

    public class K2BodiesData{
        public Dictionary<long, K2BodyData> bodies = new Dictionary<long, K2BodyData>();
    }

    public class K2BodyGO{
        public K2BodyGO(GameObject topParent, Mesh sharedJointMesh) {

            parent = topParent;
            joints = new Dictionary<K2.BodyJointType, GameObject>(); ;

            for (int ii = 0; ii < K2.nbJoints; ++ii) {

                var type = (K2.BodyJointType)ii;

                // rotations avaiable only for theses joints:
                // Neck
                // SpineShoulder
                // SpineBase
                // ShoulderLeft / ShoulderRight
                // ElbowLeft / ElbowRight
                // WristLeft / WristRight
                // HipLeft / HipRight
                // KneeLeft / KneeRight

                if (type == K2.BodyJointType.neck ||
                    type == K2.BodyJointType.spine_shoulder ||
                    type == K2.BodyJointType.spine_base ||
                    type == K2.BodyJointType.shoulder_left ||
                    type == K2.BodyJointType.shoulder_right ||
                    type == K2.BodyJointType.elbow_left ||
                    type == K2.BodyJointType.elbow_right ||
                    type == K2.BodyJointType.wrist_left ||
                    type == K2.BodyJointType.wrist_right ||
                    type == K2.BodyJointType.hip_left ||
                    type == K2.BodyJointType.hip_right ||
                    type == K2.BodyJointType.knee_left ||
                    type == K2.BodyJointType.knee_right){

                    joints[type] = GO.generate_landmark("joint_" + type.ToString(), parent.transform, new Color(1, 0, 0));
                } else {
                    var jointGO = GO.generate_empty_scene_object("joint_" + type.ToString(), parent.transform, false);
                    GO.init_local_scaling(jointGO, new Vector3(0.01f, 0.01f, 0.01f));

                    var mr = jointGO.AddComponent<MeshRenderer>();
                    mr.material = ExVR.GlobalResources().instantiate_default_mat();

                    var mf = jointGO.AddComponent<MeshFilter>();
                    mf.sharedMesh = sharedJointMesh; ;

                    joints[type] = jointGO;
                }

            }
        }

        public GameObject parent = null;
        public Dictionary<K2.BodyJointType, GameObject> joints = null;
    }

    public class K2BodiesGO{

        public K2BodiesGO(GameObject topParent, Mesh sharedJointMesh) {
            parent = topParent;

            bodies = new List<K2BodyGO>(K2.nbBodies);
            bodiesParent = new List<GameObject>(K2.nbBodies);
            for (int ii = 0; ii < K2.nbBodies; ++ii) {
                GameObject bodyParent = GO.generate_empty_scene_object("body_" + ii, parent.transform, false);
                bodies.Add(new K2BodyGO(bodyParent, sharedJointMesh));
                bodiesParent.Add(bodyParent);
            }
        }

        public GameObject parent = null;
        public List<GameObject> bodiesParent = null;
        public List<K2BodyGO> bodies = null;
    }

    public class K2CameraData{

        // common data from others cameras
        public List<int> commonIndices = null;

        // infos
        public int idCamera = 0;
        public bool doUpdate = true;
        public bool doDisplay = true;
        public int idFrame = -1;
        public int diffTime = 0;
        public int prevIdFrame = -1;
        public int sameFrameCounter = 0;
        public int sizePts = 0;
        public int sizeTris = 0;

        // GC handles
        public GCHandle gcColors;
        public GCHandle gcVertices;
        public GCHandle gcIdTris;

        public GCHandle gcBodiesInfo;

        public GCHandle gcJointsId;
        public GCHandle gcJointsState;
        public GCHandle gcJointsPos;
        public GCHandle gcJointsRot;

        // clouds
        public Vector3[] vertices = null;
        public Color[] colors = null;
        public int[] idTris = null;

        // bodies
        public int[] bodiesInfo = null;

        public long[] bodiesId = null;
        public bool[] bodiesTracked = null;
        public bool[] bodiesRestricted = null;
        public int[] jointsId = null;
        public int[] jointsState = null;
        public Vector3[] jointsPos = null;
        public Vector3[] jointsRot = null;
        public Matrix4x4 calibMatrix;

        public K2CameraData(int id, Matrix4x4 calibM, List<int> commonId) {

            idCamera = id;
            calibMatrix = calibM;
            commonIndices = commonId;

            // colors
            colors = new Color[K2.depthCount];
            gcColors = GCHandle.Alloc(colors, GCHandleType.Pinned);

            // vertices
            vertices = new Vector3[K2.depthCount];
            gcVertices = GCHandle.Alloc(vertices, GCHandleType.Pinned);

            // triangles
            idTris = new int[3 * 2 * K2.depthCount];
            gcIdTris = GCHandle.Alloc(idTris, GCHandleType.Pinned);

            // bodies
            bodiesInfo = new int[K2.nbBodies * 4];
            gcBodiesInfo = GCHandle.Alloc(bodiesInfo, GCHandleType.Pinned);
            // joints
            jointsId = new int[K2.nbBodies * K2.nbJoints];
            gcJointsId = GCHandle.Alloc(jointsId, GCHandleType.Pinned);
            jointsState = new int[K2.nbBodies * K2.nbJoints];
            gcJointsState = GCHandle.Alloc(jointsState, GCHandleType.Pinned);
            jointsPos = new Vector3[K2.nbBodies * K2.nbJoints];
            gcJointsPos = GCHandle.Alloc(jointsPos, GCHandleType.Pinned);
            jointsRot = new Vector3[K2.nbBodies * K2.nbJoints];
            gcJointsRot = GCHandle.Alloc(jointsRot, GCHandleType.Pinned);

            bodiesId = new long[K2.nbBodies];
            bodiesTracked = new bool[K2.nbBodies];
            bodiesRestricted = new bool[K2.nbBodies];
        }

        ~K2CameraData() {
            clean();
        }

        public K2BodyData generate_body_data(int bodyId, K2BodyGO bodyGo) {

            K2BodyData data = new K2BodyData();
            data.tracked = bodiesTracked[bodyId];
            data.restricted = bodiesRestricted[bodyId];
            data.id = data.tracked ? bodiesId[bodyId] : -1;

            for (int joint = 0; joint < K2.nbJoints; ++joint) {

                int globalId = bodyId * K2.nbJoints + joint;                
                K2.BodyJointType jointType = (K2.BodyJointType)jointsId[globalId];

                K2JointInfo jointInfo = new K2JointInfo();
                jointInfo.state = (K2.TrackingState)jointsState[globalId];
                jointInfo.position = bodyGo.joints[jointType].transform.position;
                jointInfo.rotation = bodyGo.joints[jointType].transform.rotation;

                data.joints[jointType] = jointInfo;
            }

            return data;
        }

        public K2BodiesData generate_bodies_data(K2BodiesGO bodiesGo) {
            K2BodiesData data = new K2BodiesData();

            for (int idBody = 0; idBody < K2.nbBodies; ++idBody) {
                var bodyData = generate_body_data(idBody, bodiesGo.bodies[idBody]);
                data.bodies[bodyData.id] = bodyData;
            }

            return data;
        }

        public bool update_bodies(K2BodiesGO bodiesGO) {

            if (!doUpdate) {
                return false;
            }

            int globalJoint = 0; // 0 -> Kinect2.nbBodies*Kinect2.nbJoints
            for (int idBody = 0; idBody < bodiesGO.bodies.Count; ++idBody) {

                var body = bodiesGO.bodies[idBody];
                //int bodyId = bodiesId[body]; // TODO: ...

                bool tracked = bodiesTracked[idBody];
                body.parent.SetActive(tracked);

                for (int idJoint = 0; idJoint < body.joints.Count; ++idJoint) {

                    var joint = body.joints[(K2.BodyJointType)idJoint];
                    var pos = jointsPos[globalJoint];
                    var rot = jointsRot[globalJoint];
                    joint.transform.localPosition = new Vector3(pos.x, pos.y, pos.z);
                    joint.transform.localEulerAngles = rot;
                    joint.SetActive(tracked && (jointsState[globalJoint] != 0));
                    ++globalJoint;
                }
            }

            return true;
        }


        public void update_mesh_with_cloud(GameObject cloudGO) {


            //var v2 = new Vector3[100];
            //var c2 = new Color[100];
            //int id = 0;
            //for(int ii = 0; ii < 10; ++ii) {
            //    for (int jj = 0; jj < 10; ++jj) {
            //        v2[id] = new Vector3(ii * 0.01f, jj * 0.01f, 0);
            //        c2[id++] = new Color(1, 0, 0, 1);
            //    }
            //}

            MeshFilter mf = cloudGO.GetComponent<MeshFilter>();
            if (sizePts > 0 && doUpdate) {

                mf.mesh.Clear();
                mf.mesh.indexFormat = UnityEngine.Rendering.IndexFormat.UInt32;

                Profiler.BeginSample("[ExVR][KinectData] update_mesh_with_cloud 0");
                mf.mesh.SetVertices(vertices, 0, sizePts);
                mf.mesh.SetColors(colors, 0, sizePts);
                //mf.mesh.SetVertices(v2, 0, 100);
                //mf.mesh.SetColors(c2, 0, 100);
                Profiler.EndSample();

                Profiler.BeginSample("[ExVR][KinectData] update_mesh_with_cloud 1");
                mf.mesh.SetIndices(commonIndices, 0, sizePts, MeshTopology.Points, 0, false);
                //mf.mesh.SetIndices(commonIndices, 0, 100, MeshTopology.Points, 0, false);
                Profiler.EndSample();
            }
        }


        public void update_mesh_with_mesh(GameObject meshGO) {

            MeshFilter mf = meshGO.GetComponent<MeshFilter>();

            if (sizePts > 0 && sizeTris > 0 && doUpdate) {

                mf.mesh.Clear();
                Profiler.BeginSample("[ExVR][KinectData] update_mesh_with_mesh 0");
                mf.mesh.SetVertices(vertices, 0, sizePts);
                mf.mesh.SetColors(colors, 0, sizePts);
                Profiler.EndSample();

                Profiler.BeginSample("[ExVR][KinectData] update_mesh_with_mesh 1");
                //mf.mesh.SetTriangles(idTris, 0, sizeTris * 3, 0);
                mf.mesh.SetIndices(idTris, 0, sizeTris * 3, MeshTopology.Triangles, 0, false);
                Profiler.EndSample();
            }
        }

        public void clean() {

            if (!m_cleaned) {
                gcColors.Free();
                gcVertices.Free();
                gcIdTris.Free();
                gcJointsId.Free();
                gcJointsState.Free();
                gcJointsPos.Free();
                gcJointsRot.Free();
                gcBodiesInfo.Free();
                m_cleaned = true;
            }
        }

        private bool m_cleaned = false;
    }
}
