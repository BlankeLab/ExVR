
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
using System;
using System.Runtime.InteropServices;

// unity
using UnityEngine;

namespace Ex {

    public class DLLK4VolumetricVideoResource : DLLExResource {

        public int nb_cameras() {
            return get_cameras_nb_k4_volumetric_video_ex_resource(_handle);
        }

        public int nb_frames(int idCamera) {
            return get_nb_frames_k4_volumetric_video_ex_resource(_handle, idCamera);
        }

        public float duration_ms(int idCamera) {
            return get_duration_ms_k4_volumetric_video_ex_resource(_handle, idCamera);
        }

        public Matrix4x4 model(int idCamera) {

            float[] t = new float[16];
            get_camera_transform_k4_volumetric_video_ex_resource(_handle, idCamera, t);
            return new Matrix4x4(new Vector4(t[0], t[1], t[2], 0),     // c0
                                 new Vector4(t[4], t[5], t[6], 0),     // c1
                                 new Vector4(t[8], t[9], t[10], 0),    // c2
                                 new Vector4(t[12], t[13], t[14], 1)); // c3
        }

        public int id_frame_from_time(int idCamera, float timeMs) {
            return get_id_frame_from_time_ms_k4_volumetric_video_ex_resource(_handle, idCamera, timeMs);
        }

        public int valid_vertices_count(int idCamera, int idFrame) {
            return get_valid_vertices_count_k4_volumetric_video_ex_resource(_handle, idCamera, idFrame);
        }

        #region memory_management

        public DLLK4VolumetricVideoResource() : base() {
        }

        protected override void create_DLL_class() {
            _handle = new HandleRef(this, create_k4_volumetric_video_ex_resource());
        }

        #endregion memory_management  
        #region DllImport

        [DllImport("lnco-exvr-export", EntryPoint = "create_k4_volumetric_video_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern IntPtr create_k4_volumetric_video_ex_resource();

        [DllImport("lnco-exvr-export", EntryPoint = "get_cameras_nb_k4_volumetric_video_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int get_cameras_nb_k4_volumetric_video_ex_resource(HandleRef volumetricVideoResource);

        [DllImport("lnco-exvr-export", EntryPoint = "get_nb_frames_k4_volumetric_video_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int get_nb_frames_k4_volumetric_video_ex_resource(HandleRef volumetricVideoResource, int idCamera);

        [DllImport("lnco-exvr-export", EntryPoint = "get_duration_ms_k4_volumetric_video_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern float get_duration_ms_k4_volumetric_video_ex_resource(HandleRef volumetricVideoResource, int idCamera);

        [DllImport("lnco-exvr-export", EntryPoint = "get_camera_transform_k4_volumetric_video_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern void get_camera_transform_k4_volumetric_video_ex_resource(HandleRef volumetricVideoResource, int idCamera, float[] model);

        [DllImport("lnco-exvr-export", EntryPoint = "get_id_frame_from_time_ms_k4_volumetric_video_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int get_id_frame_from_time_ms_k4_volumetric_video_ex_resource(HandleRef volumetricVideoResource, int idCamera, float timeMs);

        [DllImport("lnco-exvr-export", EntryPoint = "get_valid_vertices_count_k4_volumetric_video_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static private extern int get_valid_vertices_count_k4_volumetric_video_ex_resource(HandleRef volumetricVideoResource, int idCamera, int idFrame);

        #endregion DllImport  
    }
}




    //struct ExampleVertex {
    //    public Vector3 pos;
    //    public Color32 col;
    //}

    ////public class VolumetricVideoCameraData {
    ////    public VolumetricVideoCameraData(int maxNbVertices, int sizeAudioBuffer) {
    ////        this.maxNbVertices   = maxNbVertices;
    ////        this.sizeAudioBuffer = sizeAudioBuffer;
    ////        vertices = new Vector3[maxNbVertices];
    ////        colors = new Color[maxNbVertices];
    ////        audio = new float[sizeAudioBuffer];
    ////    }

    ////    public int lastFrameId = -1;

    ////    public int maxNbVertices=0;
    ////    public int sizeAudioBuffer = 0;
    ////    public int nbFrames = 0;
    ////    public Matrix4x4 model = Matrix4x4.identity;
    ////    public float duration = 0f;

    ////    public Vector3[] vertices = null;
    ////    public Color[] colors = null;
    ////    public float[] audio = null;


    ////    public AudioClip audioClip = null;
    ////}

    //public class VolumetricVideoResource : ExResourceFile {

    //    public DLL.VolumetricVideoResource video = null;

    //    //public int nbCameras = 0;
    //    //public float duration = 0f;
    //    //public List<VolumetricVideoCameraData> cameraData = null;
    //    //public List<int> commonIndices = null;

    //    public VolumetricVideoResource(int key, string alias, string path) : base(key, alias, path) { }

    //    //public override bool read_data() {
    //    //    video = new DLL.VolumetricVideoResource(this);
    //    //    if (!video.load(path)) {
    //    //        log_error(string.Format("Cannot load volumetric video from path {0}.", path));
    //    //        return false;
    //    //    }
    //    //    return true;
    //    //}

    //    public override bool initialize() {

    //        //// retrieve infos
    //        //nbCameras = video.nb_cameras();
    //        //List<int> nbFramesPerCamera = new List<int>(nbCameras);
    //        //List<Matrix4x4> modelPerCamera = new List<Matrix4x4>(nbCameras);
    //        //List<float> durationPerCamera = new List<float>(nbCameras);

    //        //for (int ii = 0; ii < nbCameras; ++ii) {
    //        //    nbFramesPerCamera.Add(video.nb_frames(ii));
    //        //    modelPerCamera.Add(Matrix4x4.identity);
    //        //    //modelPerCamera.Add(video.model(ii));
    //        //    Debug.Log("ii  " + ii + " " + modelPerCamera[ii]);
    //        //    durationPerCamera.Add(video.duration_ms(ii));
    //        //    if (durationPerCamera[ii] > duration) {
    //        //        duration = durationPerCamera[ii];
    //        //    }
    //        //}

    //        //// check number of frames
    //        //for (int ii = 1; ii < nbFramesPerCamera.Count; ++ii) {
    //        //    if (nbFramesPerCamera[0] != nbFramesPerCamera[ii]) {
    //        //        log_error(string.Format("Invalid number of frames from camera {1}, {2} instead of {3}",
    //        //            ii, nbFramesPerCamera[ii], nbFramesPerCamera[0]));
    //        //        return false;
    //        //    }
    //        //}

    //        //var nbFrames = nbFramesPerCamera[0];
    //        //cameraData = new List<VolumetricVideoCameraData>(nbCameras);
    //        //int maxNbVerticesAllCameras = 0;
    //        //for (int ii = 0; ii < nbCameras; ++ii) {

    //        //    // count valid vertices 
    //        //    int maxNb = 0;
    //        //    for (int jj = 0; jj < nbFrames; ++jj) {
    //        //        var currentNb = video.valid_vertices_count(ii, jj);
    //        //        if (maxNb < currentNb) {
    //        //            maxNb = currentNb;
    //        //        }
    //        //    }
    //        //    if(maxNbVerticesAllCameras < maxNb) {
    //        //        maxNbVerticesAllCameras = maxNb;
    //        //    }

    //        //    // generate camera data
    //        //    int sizeSamples = video.process_audio(ii);
    //        //    cameraData.Add(new VolumetricVideoCameraData(maxNb, sizeSamples));
    //        //    cameraData[ii].model = modelPerCamera[ii];
    //        //    cameraData[ii].nbFrames = nbFramesPerCamera[ii];
    //        //    cameraData[ii].duration = durationPerCamera[ii];

    //        //    video.copy_audio_samples(ii, cameraData[ii].audio);
    //        //    cameraData[ii].audioClip = AudioClip.Create("camera_" + ii, sizeSamples/7, 7, 48000, false);
    //        //    cameraData[ii].audioClip.SetData(cameraData[ii].audio, 0);
    //        //}

    //        //// generate indices
    //        //commonIndices = new List<int>(maxNbVerticesAllCameras);
    //        //for(int ii = 0; ii < maxNbVerticesAllCameras; ++ii) {
    //        //    commonIndices.Add(ii);
    //        //}

    //        return true;
    //    }

    //    public int update_time(float timeMs) {

    //        //int currentFrameId = video.id_frame_from_time(0, timeMs);
    //        //if(currentFrameId != cameraData[0].lastFrameId) {
    //        //    int nbVertices = video.uncompress_frame(0, currentFrameId);
    //        //    video.copy_uncompressed_data(0,
    //        //        cameraData[0].vertices,
    //        //        cameraData[0].colors
    //        //    );
    //        //    cameraData[0].lastFrameId = currentFrameId;
    //        //    return nbVertices;
    //        //}
    //        return -1;
    //    }


    //    public override void clean() {
    //        video.Dispose();
    //    }
