
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
using System.Collections.Generic;
using System.Collections.Concurrent;

// unity
using UnityEngine;

namespace Ex {

    public class AudioRecorder : MonoBehaviour {

        public ConcurrentQueue<List<float>> dataReceived = new ConcurrentQueue<List<float>>();
        public ConcurrentQueue<int> channelsReceived = new ConcurrentQueue<int>();
        //AudioClip clip;

        public bool running = false;

        private void Start() {
            //Debug.LogError("START");

            //Debug.LogError("AudioSettings.dspTime: " + AudioSettings.dspTime);
            //Debug.LogError("AudioSettings.driverCapabilities: " + AudioSettings.driverCapabilities);
            //Debug.LogError("AudioSettings.outputSampleRate: " + AudioSettings.outputSampleRate);
            //Debug.LogError("AudioSettings.speakerMode: " + AudioSettings.speakerMode);
            //Debug.LogError("AudioSettings.GetSpatializerPluginName: " + AudioSettings.GetSpatializerPluginName());

            //int bl, nb;
            //AudioSettings.GetDSPBufferSize(out bl, out nb);
            //Debug.LogError("AudioSettings.GetDSPBufferSize: " + bl + " " + nb);
            //var config = AudioSettings.GetConfiguration();
            //Debug.LogError("speakerMode: " + config.speakerMode);
            //Debug.LogError("dspBufferSize: " + config.dspBufferSize);
            //Debug.LogError("consampleRatefig: " + config.sampleRate);
            //Debug.LogError("numRealVoices: " + config.numRealVoices);
            //Debug.LogError("numVirtualVoices: " + config.numVirtualVoices);



            running = true;
            enabled = false;

        }   

        private void OnAudioFilterRead(float[] data, int channels) {
            if (!running) {
                return;
            }
            dataReceived.Enqueue(new List<float>(data));
            channelsReceived.Enqueue(channels);
            //Debug.LogError("AudioRecorder -> " + dataReceived.Count + " " + channels);
        }

        private void OnApplicationQuit() {

            //AudioClip clip;


            enabled = true;
        }

    }
}

