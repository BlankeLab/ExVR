
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
using System.Threading.Tasks;
using System.Collections.Generic;

// unity
using UnityEngine;
using Unity.Collections;
using UnityEngine.Profiling;
using UnityEngine.Rendering;

namespace Ex {


    public class VolumetricVideoCameraData {

        public int lastFrameId = -1;
        public VolumetricVideoCameraDataInfo info = null;
        public float[] audio = null;
        public AudioClip audioClip = null;
        public NativeDLLVertices vertices = null;
    }

    public class ProcessVolumetricVideoComponent {


        class VolumetricVideoFrameInfos {
            public VolumetricVideoFrameInfos(
                DLLK4VolumetricVideoComponent cppDll, VolumetricVideoCameraData data, 
                int cameraId, int frameId, int nbValidVertices, PointCloud pc, NativeIndices commonIndices) {
                this.cppDll        = cppDll;
                this.data          = data;
                this.cameraId      = cameraId;
                this.frameId       = frameId;
                this.nbVertices    = nbValidVertices;
                this.pc = pc;
                this.commonIndices = commonIndices;
            }

            public DLLK4VolumetricVideoComponent cppDll = null;
            public VolumetricVideoCameraData data = null;
            public int cameraId = 0;
            public int frameId = 0;
            public PointCloud pc = null;
            public NativeIndices commonIndices = null;            
            public int nbVertices = 0;
        }

        static bool processDone = false;
        static List<VolumetricVideoFrameInfos> framesToProcess = new List<VolumetricVideoFrameInfos>();

        public static void reset() {
            if (processDone) {
                processDone = false;
            }
        }

        public static void add_frame(
            DLLK4VolumetricVideoComponent cppDll, VolumetricVideoCameraData data, 
            int cameraId, int frameId, int nbValidVertices, PointCloud pc, NativeIndices commonIndices) {
            framesToProcess.Add(new VolumetricVideoFrameInfos(cppDll, data, cameraId, frameId, nbValidVertices, pc, commonIndices));
        }

        public static void process_all() {

            if (processDone) {
                return;
            }

            //for (int ii = 0; ii < framesToProcess.Count; ++ii) {
                Parallel.For(0, framesToProcess.Count, ii => {

                    var dtp = framesToProcess[ii];
                    Profiler.BeginSample("[ExVR][ProcessVolumetricVideoComponent] uncompress_frame");
                    if (!dtp.cppDll.uncompress_frame(
                        dtp.cameraId,
                        dtp.frameId,
                        ref dtp.data.vertices.native
                    )) {
                        Debug.LogError("uncompress error");
                        return;
                    }
                    Profiler.EndSample();
                    dtp.data.lastFrameId = dtp.frameId;
                });
            //}

            var layout = new[]{
                new VertexAttributeDescriptor(VertexAttribute.Position, VertexAttributeFormat.Float32, 3),
                new VertexAttributeDescriptor(VertexAttribute.Color, VertexAttributeFormat.UNorm8, 4)
            };

            for (int ii = 0; ii < framesToProcess.Count; ++ii) {
                var ftp = framesToProcess[ii];
                if (ftp.nbVertices >= 0) {
                    ftp.pc.set_points(ftp.data.vertices, ftp.commonIndices, ftp.nbVertices);
                    Debug.Log("ii " + ii + " " + ftp.nbVertices);
                }
            }

            processDone = true;
            framesToProcess.Clear();
        }
    }

    public class VolumetricVideoComponent : CppExComponent {

        private VolumetricVideoResource volumetricVideo = null;
        private List<VolumetricVideoCameraData> dataPerCamera = null;
        private NativeIndices indices = null;

        public AudioSource audioSource = null;
        public List<PointCloud> clouds = null;

        private List<GameObject> m_OBBsGO = null;
        private List<OBBFInfo> m_OBBsInfo = null;

        #region ex_functions
        protected override bool initialize() {

            // retrieve resources
            volumetricVideo = initC.get_volumetric_video_data("volumetric_video");
            if (volumetricVideo == null) {
                return false;
            }

            // init DLL
            cppDll = new DLLK4VolumetricVideoComponent(volumetricVideo.get_dll_handle());
            cppDll.parent = this;
            cppDll.initialize();            

            // obb
            m_OBBsGO = new List<GameObject>(10);
            m_OBBsInfo = new List<OBBFInfo>(10);
            for (int ii = 0; ii < 10; ++ii) {
                var obbGO = GO.generate_cube("filtering obb", transform, 1f, null, -1, ExVR.GlobalResources().instantiate_default_transparent_mat());
                obbGO.SetActive(false);
                m_OBBsGO.Add(obbGO);
                m_OBBsInfo.Add(new OBBFInfo());
            }

            // audio
            audioSource = gameObject.AddComponent<AudioSource>();
            audioSource.clip = null;
            audioSource.playOnAwake = false;
            audioSource.loop = false;
            audioSource.enabled = false;
            audioSource.spatialize = false;
            audioSource.minDistance = 0f;
            audioSource.maxDistance = 100000f;
            audioSource.panStereo = 0.5f;
            audioSource.volume = 1f;
            audioSource.spatialBlend = 0.5f;

            var vvcDll    = (DLLK4VolumetricVideoComponent)cppDll;
            clouds        = new List<PointCloud>(volumetricVideo.nbCameras);
            dataPerCamera = new List<VolumetricVideoCameraData>(volumetricVideo.nbCameras);
           
            for (int ii = 0; ii < volumetricVideo.nbCameras; ++ii) {

                GameObject cloudGO = GO.generate_empty_scene_object("camera_" + ii, transform, true);
                //GO.init_local_scaling(cloudGO, new Vector3(0.1f, 0.1f, 0.1f));

                // set point cloud
                var pc = cloudGO.AddComponent<PointCloud>();
                pc.set_as_dynamic();
                clouds.Add(pc);

                VolumetricVideoCameraData data = new VolumetricVideoCameraData();
                data.info = volumetricVideo.cameraDataInfo[ii];

                // set audio
                int sizeSamples = vvcDll.process_audio(ii);
                if (sizeSamples > 0) {
                    data.audio = new float[sizeSamples];
                    vvcDll.copy_audio_samples(ii, data.audio);
                    data.audioClip = AudioClip.Create("camera_" + ii, sizeSamples / 7, 7, 48000, false);
                    data.audioClip.SetData(data.audio, 0);
                } else {
                    log_message(string.Format("No audio frames detected for camera [{0}].", ii));
                    data.audioClip = null;
                }

                // set vertices
                data.vertices = new NativeDLLVertices(data.info.maxNbVertices);

                dataPerCamera.Add(data);
            }

            // apply calibration matrices
            for (int ii = 0; ii < volumetricVideo.nbCameras; ++ii) {
                clouds[ii].transform.localRotation = volumetricVideo.cameraDataInfo[ii].model.GetRotation();
                clouds[ii].transform.localPosition = volumetricVideo.cameraDataInfo[ii].model.GetPosition();
            }

            // generate indices
            indices = new NativeIndices(volumetricVideo.maxNbVerticesAllCameras);

            string camerasInfo = "Cameras:\n";
            for (int ii = 0; ii < volumetricVideo.nbCameras; ++ii) {
                var cdi = volumetricVideo.cameraDataInfo[ii];
                int nbAudioFrames = dataPerCamera[ii].audio != null ? dataPerCamera[ii].audio.Length : 0;
                camerasInfo += string.Format("Camera n°{0}\tNb frames: {1}\n\tNb max vertices: {2}\n\tDuration: {3}\nNb audio frames: {4}\nModel:\n{5}\n",
                    ii, cdi.nbFrames, cdi.maxNbVertices, cdi.duration, nbAudioFrames, cdi.model);
            }

            send_infos_to_gui_init_config("infos", string.Format(
                "Duration: {0}ms\nNb cameras: {1}\nMax vertices number: {2}\n{3}",
                volumetricVideo.duration, volumetricVideo.nbCameras, volumetricVideo.maxNbVerticesAllCameras, camerasInfo));

            return true;
        }

        protected override void start_experiment() {
            reset_init_transform();
        }

        protected override void start_routine() {

            if (currentC.get<int>("audio_id") < dataPerCamera.Count) {
                audioSource.clip = dataPerCamera[currentC.get<int>("audio_id")].audioClip;
            } else {
                audioSource.clip = dataPerCamera[0].audioClip;
            }

            if (audioSource.clip != null) {
                audioSource.Play();
            }
        }

        protected override void set_visibility(bool visibility) {

            foreach (var cloudGO in clouds) {
                cloudGO.gameObject.SetActive(visibility && currentC.get<bool>("display_clouds"));
            }

            for (int ii = 0; ii < m_OBBsGO.Count; ++ii) {
                m_OBBsGO[ii].SetActive(visibility && currentC.get<bool>("display_filtering_obb") && m_OBBsInfo[ii].display);
            }
        }

        public override void update_from_current_config() {

            if (!currentC.get<bool>("global_transform_do_not_apply")) {
                currentC.update_transform("global_transform", transform, true);
            }

            // obb
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

            // audio
            audioSource.loop    = currentC.get<bool>("loop");
            audioSource.enabled = currentC.get<bool>("enable_audio");
            audioSource.volume  = currentC.get<float>("volume");
            // offset time?

            // point cloud shading
            float sizePoints    = currentC.get<float>("size_points");
            bool removeOutside = currentC.get<bool>("filter_points_outside_obb");
            bool cones          = currentC.get<bool>("cones");
            bool circles        = currentC.get<bool>("circles");
            Color tintColor     = currentC.get_color("tint");
            var details         = (PointCloud.ParabloidGeoDetails)currentC.get<int>("details");
            var rendering       = (PointCloud.RenderingType)currentC.get<int>("rendering");

            for (int ii = 0; ii < volumetricVideo.nbCameras; ++ii) {
                var pc = clouds[ii];
                pc.set_pt_size(sizePoints);
                pc.set_rendering(rendering);                
                pc.set_circles_state(circles);
                pc.set_paraboloid_frag_cones_state(cones);
                pc.set_paraboloid_geo_details(details);
                pc.set_tint(tintColor);
                pc.set_obb_filtering_state(removeOutside);
                pc.set_filtering_obb_infos(m_OBBsInfo);
            }

            set_visibility(is_visible());
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        protected override void pre_update() {
            ProcessVolumetricVideoComponent.reset();
        }

        protected override void update() {

            var videoD = volumetricVideo.duration;
            float start = currentC.get<float>("start_time") * 1000f;
            float end = currentC.get<float>("end_time") * 1000f;
            if (start > end) {
                start = end;
            }
            if (start > videoD) {
                start = videoD;
            }
            if (end > videoD) {
                end = videoD;
            }

            var duration = end - start;
            var currTime = time().ellapsed_element_ms() + start;
            if (currTime > duration) {
                if (!currentC.get<bool>("loop")) {
                    return;
                }
                float percentage = (float)(currTime - start) / duration;
                currTime = start + (percentage - System.Math.Truncate(percentage)) * duration;
            }

            for (int ii = 0; ii < volumetricVideo.nbCameras; ++ii) {
                update_time(ii, (float)currTime);
            }
        }

        protected override void post_update() {
            ProcessVolumetricVideoComponent.process_all();
        }

        protected override void stop_routine() {
            audioSource.Stop();
        }

        public override void play() {
            audioSource.UnPause();
        }

        public override void pause() {
            audioSource.Pause();
        }

        protected override void clean() {

            if (indices != null) {
                indices.clean();
            }
            if (dataPerCamera != null) {
                foreach (var camData in dataPerCamera) {
                    camData.vertices.clean();
                }
            }
        }

        #endregion
        #region private_functions

        private void update_time(int cameraId, float timeMs) {
            
            var vvcDll = (DLLK4VolumetricVideoComponent)cppDll;
            var data = dataPerCamera[cameraId];

            int currentFrameId  = volumetricVideo.id_frame_from_time(cameraId, timeMs);
            int nbValidVertices = volumetricVideo.nb_valid_vertices(cameraId, currentFrameId);
            if (currentFrameId != data.lastFrameId) {

                ProcessVolumetricVideoComponent.add_frame(
                    vvcDll,
                    data, 
                    cameraId, currentFrameId, nbValidVertices,
                    clouds[cameraId],
                    indices
                ); 
            }
        }


        #endregion
    }
}

