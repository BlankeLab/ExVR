
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


    public class VolumetricVideoCameraDataInfo {
        
        public int maxNbVertices = 0;
        public int sizeAudioBuffer = 0;
        public int nbFrames = 0;
        public Matrix4x4 model = Matrix4x4.identity;
        public float duration = 0f;
    }

    public class VolumetricVideoResource : CppExResourceFile {

        public int nbCameras = 0;
        public float duration = 0f;
        public int maxNbVerticesAllCameras = 0;
        public List<VolumetricVideoCameraDataInfo> cameraDataInfo = null;


        public VolumetricVideoResource(int key, string alias, string path) : base(key, alias, path) {
        }

        public override bool initialize() {

            // init DLL
            cppDll = new DLLVolumetricVideoResource();
            cppDll.parent = this;

            // set path to load
            cppDll.set(Parameters.Container.Dynamic, "path_file", path);

            if (!cppDll.initialize()) {
                log_error(string.Format("Cannot initialize volumetric video resource dll from path {0}.", path));
                clean();
                return false;
            }

            // retrieve infos
            var vvrdll = (DLLVolumetricVideoResource)cppDll;
            nbCameras = vvrdll.nb_cameras();
            List<int> nbFramesPerCamera = new List<int>(nbCameras);
            List<Matrix4x4> modelPerCamera = new List<Matrix4x4>(nbCameras);
            List<float> durationPerCamera = new List<float>(nbCameras);

            for (int ii = 0; ii < nbCameras; ++ii) {

                nbFramesPerCamera.Add(vvrdll.nb_frames(ii));
                modelPerCamera.Add(vvrdll.model(ii));

                durationPerCamera.Add(vvrdll.duration_ms(ii));
                if (durationPerCamera[ii] > duration) {
                    duration = durationPerCamera[ii];
                }
            }

            // check number of frames
            for (int ii = 1; ii < nbFramesPerCamera.Count; ++ii) {
                if (nbFramesPerCamera[0] != nbFramesPerCamera[ii]) {
                    log_error(string.Format("Invalid number of frames from camera {1}, {2} instead of {3}",
                        ii, nbFramesPerCamera[ii], nbFramesPerCamera[0]));
                    return false;
                }
            }

            var nbFrames = nbFramesPerCamera[0];
            cameraDataInfo = new List<VolumetricVideoCameraDataInfo>(nbCameras);
            
            for (int ii = 0; ii < nbCameras; ++ii) {

                // count valid vertices 
                int maxNb = 0;
                for (int jj = 0; jj < nbFrames; ++jj) {
                    var currentNb = vvrdll.valid_vertices_count(ii, jj);
                    if (maxNb < currentNb) {
                        maxNb = currentNb;
                    }
                }
                if (maxNbVerticesAllCameras < maxNb) {
                    maxNbVerticesAllCameras = maxNb;
                }

                // generate camera data info
                var data             = new VolumetricVideoCameraDataInfo();
                data.maxNbVertices   = maxNb;                
                data.model           = modelPerCamera[ii];
                data.nbFrames        = nbFramesPerCamera[ii];
                data.duration        = durationPerCamera[ii];
                cameraDataInfo.Add(data);
            }

            return true;
        }

        public override void clean() {
            if (cppDll != null) {
                cppDll.clean();
                cppDll = null;
            }
        }

        public int nb_valid_vertices(int idCamera, int idFrame) {
            return ((DLLVolumetricVideoResource)cppDll).valid_vertices_count(idCamera, idFrame);
        }

        public int id_frame_from_time(int idCamera, float timeMs) {
            return ((DLLVolumetricVideoResource)cppDll).id_frame_from_time(idCamera, timeMs);
        }
    }
}