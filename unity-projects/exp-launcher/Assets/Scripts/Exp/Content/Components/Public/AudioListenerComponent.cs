
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
using System.Text;
using System.IO;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex {



    //public static class BinaryWriterExtensions {

    //    private const int HeaderSize = 44;
    //    private const int Hz = 16000; //frequency or sampling rate
    //    private const float RescaleFactor = 32767; //to convert float to Int16

    //    public static void AppendWaveData<T>(T stream, float[] buffer) where T : Stream {
    //        if (stream.Length > HeaderSize) {
    //            stream.Seek(0, SeekOrigin.End);
    //        } else {
    //            stream.SetLength(HeaderSize);
    //            stream.Position = HeaderSize;
    //        }

    //        // rescale
    //        var floats = Array.ConvertAll(buffer, x => (short)(x * RescaleFactor));

    //        // Copy to bytes
    //        var result = new byte[floats.Length * sizeof(short)];
    //        Buffer.BlockCopy(floats, 0, result, 0, result.Length);

    //        // write to stream
    //        stream.Write(result, 0, result.Length);

    //        // Update Header
    //        UpdateHeader(stream);
    //    }

    //    public static void UpdateHeader(Stream stream) {

    //        var writer = new BinaryWriter(stream);
    //        writer.Seek(0, SeekOrigin.Begin);
    //        writer.Write(Encoding.ASCII.GetBytes("RIFF")); //RIFF marker. Marks the file as a riff file. Characters are each 1 byte long. 
    //        writer.Write((int)(writer.BaseStream.Length - 8)); //file-size (equals file-size - 8). Size of the overall file - 8 bytes, in bytes (32-bit integer). Typically, you'd fill this in after creation.
    //        writer.Write(Encoding.ASCII.GetBytes("WAVE")); //File Type Header. For our purposes, it always equals "WAVE".
    //        writer.Write(Encoding.ASCII.GetBytes("fmt ")); //Mark the format section. Format chunk marker. Includes trailing null. 
    //        writer.Write(16); //Length of format data.  Always 16. 
    //        writer.Write((short)1); //Type of format (1 is PCM, other number means compression) . 2 byte integer. Wave type PCM
    //        writer.Write((short)2); //Number of Channels - 2 byte integer
    //        writer.Write(Hz); //Sample Rate - 32 byte integer. Sample Rate = Number of Samples per second, or Hertz.
    //        writer.Write(Hz * 2 * 1); // sampleRate * bytesPerSample * number of channels, here 16000*2*1.
    //        writer.Write((short)(1 * 2)); //channels * bytesPerSample, here 1 * 2  // Bytes Per Sample: 1=8 bit Mono,  2 = 8 bit Stereo or 16 bit Mono, 4 = 16 bit Stereo
    //        writer.Write((short)16); //Bits per sample (BitsPerSample * Channels) ?? should be 8???
    //        writer.Write(Encoding.ASCII.GetBytes("data")); //"data" chunk header. Marks the beginning of the data section.    
    //        writer.Write((int)(writer.BaseStream.Length - HeaderSize)); //Size of the data section. data-size (equals file-size - 44). or NumSamples * NumChannels * bytesPerSample ??        
    //    }
    //} 



    public class AudioListenerComponent : ExComponent {


        private double sampling_frequency = 48000;
        public AudioClip clip = null;

        // https://answers.unity.com/questions/1427912/converting-audiolistenergetoutputdata-to-a-wav-fil.html


        //public AudioListener audioListener = null;

        #region ex_functions
        protected override bool initialize() {






            return true;
        }


        protected override void set_update_state(bool doUpdate) {
            ExVR.Display().camerasManager.bothEyesCamera.GetComponent<AudioRecorder>().enabled = doUpdate;
        }

        protected override void stop_routine() {
            var recorder = ExVR.Display().camerasManager.bothEyesCamera.GetComponent<AudioRecorder>();

            //AudioClip clip  = AudioClip.Create("test", )

            Debug.LogError("stop routine " + recorder.dataReceived.Count);
            List<List<float>> blocks = new List<List<float>>();
            List<int> channels = new List<int>();
            int totalSize = 0;
            while (!recorder.dataReceived.IsEmpty) {
                List<float> data;
                int nbChannels;
                recorder.dataReceived.TryDequeue(out data);
                recorder.channelsReceived.TryDequeue(out nbChannels);
                blocks.Add(data);
                totalSize += data.Count;
                channels.Add(nbChannels);
                Debug.Log("-> " + data.Count + " " + nbChannels);
            }
            //AudioClip.Create

            if(channels.Count == 0) {
                return;
            }

            int nbChannelsT = channels[0];
            foreach (var nbChannels in channels) {
                if(nbChannelsT != nbChannels) {
                    return;
                }
            }
            int nbSamples = totalSize / nbChannelsT;
            
            Debug.Log("nbSamples " + nbSamples);
            Debug.Log("nbChannelsT " + nbChannelsT);
            Debug.Log("totalSize " + totalSize);
            //SavWav.Save();


            float mean = 0f;
            List<float> d = new List<float>(totalSize);
            for (int ii = 0; ii < blocks.Count; ++ii) {
                for (int jj = 0; jj < blocks[ii].Count; ++jj) {
                    d.Add(blocks[ii][jj]);
                    mean += blocks[ii][jj];
                }
            }
            float[] newSamples = d.ToArray();
            Debug.Log("data " + mean + " " + newSamples.Length);

            //int sampleId = 0;
            //for(int ii = 0; ii < blocks.Count; ++ii) {
            //    Debug.Log("block " + ii + " " + blocks.Count);
            //    for(int jj = 0; jj < blocks[ii].Count; ++jj) {

            //        int currentChannelV = jj % nbChannelsT;
            //        //Debug.Log("s " + jj + " " + blocks[ii].Count + " "+ currentChannelV);
            //        if(sampleId * nbChannelsT + currentChannelV >= newSamples.Length) {
            //            Debug.LogError("error " + (sampleId * nbChannelsT + currentChannelV) + " " + newSamples.Length);
            //            break;
            //        }
            //        newSamples[sampleId * nbChannelsT + currentChannelV] = blocks[ii][jj];
            //        ++sampleId;
            //    }
            //}



            //for (int sampleId = 0; sampleId < m_audioClip.samples; ++sampleId) {
            //    newSamples[sampleId * newNbChannels + destinationChannel] = currentSamples[sampleId * m_audioClip.channels + originChannel];
            //}

            
            clip = AudioClip.Create("test", nbSamples, nbChannelsT, AudioSettings.outputSampleRate, false);
            clip.SetData(newSamples,0);
            SavWav.Save("./testA.wav", clip);

            //Debug.LogError("droutine " + recorder.dataReceived.Count);
        }

        void save() {
            //recording = true;
            //ExVR.Display().camerasManager.bothEyesCamera.GetComponent<AudioRecorder>().enabled = false;

        }


        //void OnAudioFilterRead(float[] data, int channels) {
        //    Debug.LogError("data " + data.Length + " " + channels);
        //}

        //protected override void post_update() {

        //    Debug.LogError("p");

        //    for (int ii = 0; ii < outputData.Length; ++ii) {
        //        outputData[ii] = 0;
        //    }

        //    Debug.LogError("gob " + outputData.Length);
        //    AudioListener.GetOutputData(outputData, 0);
        //    Debug.LogError("go " + outputData.Length);

        //    for (int ii = 0; ii < outputData.Length; ++ii) {
        //        if (outputData[ii] == 0) {
        //            Debug.Log("-> " + ii);
        //            break;
        //        }
        //    }
        //}

        #endregion
    }
}