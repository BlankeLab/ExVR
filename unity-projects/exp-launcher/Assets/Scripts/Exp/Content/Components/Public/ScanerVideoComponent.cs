/*******************************************************************************
** exvr-exp                                                                   **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

// system
using System;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Collections.Generic;

// unity
using UnityEngine;
using UnityEngine.Profiling;

namespace Ex{

    public class ScanerVideoComponent : ExComponent{

        static public Mesh jointMesh = null;
        public int nbCameras;
        public int durationMs;
        public GameObject parentClouds = null;
        public GameObject parentBodies = null;
        public List<int> bodiesNbPerCam = new List<int>();
        public List<Dictionary<K2.BodyJointType, K2JointInfo>> jointsPerCam = new List<Dictionary<K2.BodyJointType, K2JointInfo>>();
        public List<GameObject> cloudsGO = null;
        public List<K2BodiesGO> bodiesGO = null;
        // matrices
        public List<Matrix4x4> calibMatrices = new List<Matrix4x4>();
        // sizes
        public List<bool> camerasValidity = new List<bool>();
        public List<int> sizesPts = new List<int>();


        private ScanerVideoResource m_videoFileData = null;
        //private long m_lastUpdateTime = 0;
        private long m_updateCounter = 0;
        
        // GC handles
        private List<GCHandle> m_gcColors = new List<GCHandle>();
        private List<GCHandle> m_gcVertices = new List<GCHandle>();

        // data
        private List<Vector3[]> m_vertices = new List<Vector3[]>();
        private List<Color[]> m_colors = new List<Color[]>();
        private List<int> m_commonIndices = new List<int>();

        #region ex_functions
        protected override bool initialize() {

            string aliasScanerVideo = initC.get_resource_alias("scaner_video");
            if (aliasScanerVideo.Length == 0) {
                ExVR.Log().error("No scaner video alias defined.");
                return false;
            }
            m_videoFileData = ExVR.Resources().get_scaner_video_file_data(aliasScanerVideo);

            nbCameras  = m_videoFileData.dllScanerVideoResource.nb_cameras();
            durationMs = m_videoFileData.dllScanerVideoResource.duration_ms();

            m_commonIndices = new List<int>(K2.depthCount);
            for (int ii = 0; ii < K2.depthCount; ++ii) {
                m_commonIndices.Add(ii);
            }

            for (int ii = 0; ii < nbCameras; ++ii) {
                calibMatrices.Add(m_videoFileData.dllScanerVideoResource.camera_model(ii));
                m_colors.Add(new Color[K2.depthCount]);
                m_gcColors.Add(GCHandle.Alloc(m_colors[ii], GCHandleType.Pinned));
                m_vertices.Add(new Vector3[K2.depthCount]);
                m_gcVertices.Add(GCHandle.Alloc(m_vertices[ii], GCHandleType.Pinned));
                sizesPts.Add(0);
            }

            // init clouds            
            parentClouds = GO.generate_empty_scene_object("grabbers_cloud", transform, true);
            GO.init_local_scaling(parentClouds, new Vector3(-1, 1, 1));
            cloudsGO = new List<GameObject>(nbCameras);
            for (int ii = 0; ii < nbCameras; ++ii) {

                GameObject cloudGO = GO.generate_empty_scene_object("cloud_" + ii, parentClouds.transform, true);
                GO.init_local_scaling(cloudGO, new Vector3(1, 1, 1));

                cloudGO.AddComponent<PointCloud>();
                Mesh mesh = cloudGO.GetComponent<MeshFilter>().mesh;
                mesh.MarkDynamic();
                mesh.indexFormat = UnityEngine.Rendering.IndexFormat.UInt32;
                cloudsGO.Add(cloudGO);

                camerasValidity.Add(m_videoFileData.dllScanerVideoResource.camera_cloud_size(ii) > 0);
            }

            // init bodies
            var jointMesh = Ex.PrimitivesMesh.CubeBuilder.generate(1f);
            parentBodies = GO.generate_empty_scene_object("grabbers_bodies", transform, true);
            
            bodiesGO = new List<K2BodiesGO>(nbCameras);
            for (int ii = 0; ii < nbCameras; ++ii) {

                bodiesNbPerCam.Add(m_videoFileData.dllScanerVideoResource.nb_bodies(ii));

                bodiesGO.Add(new K2BodiesGO(GO.generate_empty_scene_object("cam_" + ii, parentBodies.transform, true), jointMesh));

                var jointsInfo = new Dictionary<K2.BodyJointType, K2JointInfo>();
                for (int jj = 0; jj < K2.nbJoints; ++jj) {
                    var type = (K2.BodyJointType)jj;
                    jointsInfo[type] = new K2JointInfo();  
                }
                jointsPerCam.Add(jointsInfo);   
            }


            // apply calibration matrices
            for (int ii = 0; ii < nbCameras; ++ii) {
                cloudsGO[ii].transform.localRotation = calibMatrices[ii].GetRotation();
                cloudsGO[ii].transform.localPosition = calibMatrices[ii].GetPosition();

                bodiesGO[ii].parent.transform.localRotation = calibMatrices[ii].GetRotation();
                bodiesGO[ii].parent.transform.localPosition = calibMatrices[ii].GetPosition();                
            }


            string infos = "" + nbCameras + "%" + durationMs;
            log_message(infos);
            send_infos_to_gui_init_config("scaner_video_infos", infos);

            return m_videoFileData != null;
        }

        protected override void clean() {
            
            foreach (GCHandle handle in m_gcColors) {
                handle.Free();
            }
            foreach (GCHandle handle in m_gcVertices) {
                handle.Free();
            }

            base.clean();
        }

        
        public override void update_from_current_config() {

            if (!currentC.get<bool>("global_transform_do_not_apply")) {
                currentC.update_transform("global_transform", transform, true);
            }

            set_visibility(is_visible());

            float sizePoints = currentC.get<float>("size_points");
            for (int ii = 0; ii < nbCameras; ++ii) {
                cloudsGO[ii].GetComponent<MeshRenderer>().material.SetFloat("_PointSize", sizePoints);
            }
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }


        protected override void start_routine() {            
            m_updateCounter = 0;
        }

        protected override void update() {

            // check last frame time
            int fps = currentC.get<int>("fps");
            var currTime = time().ellapsed_element_ms();

            if (currTime / (1000.0 / fps) > m_updateCounter) {

                ++m_updateCounter;

                Profiler.BeginSample("[ExVR][ScanerVideo] update_cloud_data");

                bool loop = currentC.get<bool>("loop");
                int maxDiffTimeMs = currentC.get<int>("max_diff_time_ms");
                Parallel.For(0, nbCameras, ii => {
                    if (camerasValidity[ii]) {
                        sizesPts[ii] = m_videoFileData.dllScanerVideoResource.update_cloud_data(ii, (int)currTime, maxDiffTimeMs, loop, m_gcVertices[ii].AddrOfPinnedObject(), m_gcColors[ii].AddrOfPinnedObject());
                    }
                });

                Profiler.EndSample();

                Profiler.BeginSample("[ExVR][ScanerVideo] update_mesh");
                for (int ii = 0; ii < nbCameras; ++ii) {

                    if (!camerasValidity[ii]) {
                        continue;
                    }

                    Profiler.BeginSample("[ExVR][ScanerVideo] update_cloud");
                    update_mesh_with_cloud(cloudsGO[ii], ii);
                    Profiler.EndSample();

                    Profiler.BeginSample("[ExVR][ScanerVideo] update_joints");
                    for (int jj = 0; jj < bodiesNbPerCam[ii]; ++jj) {

                        var isTracked = m_videoFileData.dllScanerVideoResource.is_body_tracked(ii, jj + 1);
                        bodiesGO[ii].bodies[jj].parent.SetActive(isTracked);


                        m_videoFileData.dllScanerVideoResource.update_joints(ii, jj + 1, jointsPerCam[ii]);

                        foreach (var joint in jointsPerCam[ii]) {
                            var jointGO = bodiesGO[ii].bodies[jj].joints[joint.Key];
                            jointGO.SetActive(joint.Value.state == K2.TrackingState.tracked);
                            jointGO.transform.localPosition  = joint.Value.position;
                            jointGO.transform.localRotation  = joint.Value.rotation;
                        }
                    }
                    Profiler.EndSample();
                }
                Profiler.EndSample();
            }
        }
 

        protected override void set_visibility(bool visibility) {
            parentClouds.SetActive(visibility && currentC.get<bool>("display_clouds"));
            parentBodies.SetActive(visibility && currentC.get<bool>("display_colliders"));
        }

        #endregion
        #region private_functions
        private void update_mesh_with_cloud(GameObject cloudGO, int idCloud) {

            MeshFilter mf = cloudGO.GetComponent<MeshFilter>();
            mf.mesh.Clear();

            if (sizesPts[idCloud] > 0) {

                Profiler.BeginSample("[ExVR][ScanerVideo] update_mesh_with_cloud 0");
                mf.mesh.SetVertices(m_vertices[idCloud], 0, sizesPts[idCloud]);
                mf.mesh.SetColors(m_colors[idCloud], 0, sizesPts[idCloud]);
                Profiler.EndSample();

                Profiler.BeginSample("[ExVR][ScanerVideo] update_mesh_with_cloud 1");
                mf.mesh.SetIndices(m_commonIndices, 0, sizesPts[idCloud], MeshTopology.Points, 0, false);   
                Profiler.EndSample();
            }
   
        }

        #endregion

    }
}