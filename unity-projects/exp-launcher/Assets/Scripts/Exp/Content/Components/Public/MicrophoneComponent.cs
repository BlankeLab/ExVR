
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
using System.Threading;
using System.IO;

// unity
using UnityEngine;

namespace Ex{


    public class MicrophoneComponent : ExComponent{

        private string device = "";

        AudioSource source;
        private List<AudioClip> clips = new List<AudioClip>();
        

        void send_devices_info() {

            string devincesInfo = "";
            foreach (string device in Microphone.devices) {
                log_message("Microphone detected: " + device);
                devincesInfo += device + "%";
            }
            devincesInfo.PadLeft(1);
            send_infos_to_gui_init_config("microphones_info", devincesInfo);
        }


        protected override bool initialize() {

            source = gameObject.AddComponent<AudioSource>();

            var devices = Microphone.devices;            
            if (devices.Length == 0) {
                log_error("No recording device available.");
                return false;
            }
            send_devices_info();




            int idDeviceToSelect = 0;
            device = Microphone.devices[idDeviceToSelect];

            //int minFreq = 0, maxFreq = 0;
            //Microphone.GetDeviceCaps(device, out minFreq, out maxFreq);
            //int position = Microphone.GetPosition(device);
            //bool isRecording = Microphone.IsRecording(device);


            //Microphone.Start(device, false, lengthSec)



            //Microphone.End();

            
            //AudioSource aud = gameObject.AddComponent<AudioSource>();
            //aud.clip = Microphone.Start(null, true, 1, 4400);
            //aud.loop = true;
            //while (!(Microphone.GetPosition(null) > 0)) { }
            //aud.Play();
            //aud.clip.SetData();

            //aud = gameObject.AddComponent<AudioSource>();

            //foreach (string device in Microphone.devices) {
            //    Debug.Log("Name: " + device);
            //}

            //aud.clip = Microphone.Start(Microphone.devices[1], false, 10, 44100);
            ////AudioClip.Create()
            ////aud.clip = Microphone.Start(Microphone.devices[1], false, 1000, 88000);
            //aud.Play();


            return true;
        }

        protected override void start_experiment() {
            send_devices_info();
        }

        protected override void stop_experiment() {
            Debug.Log("clips " + clips.Count);

            string path = "G:/test_";

            //int id = 0;
            //foreach (var clip in clips) {


            //    //float min = 100000f;
            //    //float max = -100000f;
            //    //float[] currentSamples = new float[clip.samples * clip.channels];
            //    //clip.GetData(currentSamples, 0);
     
            //    //for(int ii = 0; ii < currentSamples.Length; ++ii) {
            //    //    if(currentSamples[ii] < min) {
            //    //        min = currentSamples[ii];
            //    //    }

            //    //    if(currentSamples[ii] > max) {
            //    //        max = currentSamples[ii];
            //    //    }

            //    //}
            //    //Debug.Log("min " + min + " max " + max);
            //    Debug.Log("CLIP " + device + " " + clip.length + " " + clip.samples + " " + clip.name + " " + clip.channels + " " + clip.frequency + " " + clip.ambisonic);

                
            //    ++id;
            //}
            SavWav.Save(path , source.clip, false);

        }

        protected override void stop_routine() {
            Microphone.End(device);
        }

        protected override void update() {
            //Debug.Log("device " + device + " -> " + Microphone.GetPosition(device) + " " + Microphone.IsRecording(device));
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
        }

        public void start_recording_new_clip() {
            Debug.Log("######################### start_recording_new_clip");

            var interval = currentTimeline.get_upate_time_interval(time().ellapsed_element_ms() * 0.001);
            if (interval != null) {
                int minFreq = 0, maxFreq = 0;
                int length = (int)(interval.tEndS - interval.tStartS);
                if(length > 3600) {
                    log_error("Can't save more than one hour at one using microphone");
                    length = 3600;
                }
                Microphone.GetDeviceCaps(device, out minFreq, out maxFreq);
                Debug.Log(minFreq + " " + maxFreq + " "+ length);
                source.clip = Microphone.Start(device, true, length, 44100); 
            }
        }

        public void stop_current_recording() {
            Debug.Log("######################### stop_current_recording");
            Microphone.End(device);
            //if (Microphone.IsRecording(device)) {


                

            //    float min = 100000f;
            //    float max = -100000f;
            //    float[] currentSamples = new float[currentClip.samples * currentClip.channels];
            //    currentClip.GetData(currentSamples, 0);

            //    for (int ii = 0; ii < currentSamples.Length; ++ii) {
            //        if (currentSamples[ii] < min) {
            //            min = currentSamples[ii];
            //        }

            //        if (currentSamples[ii] > max) {
            //            max = currentSamples[ii];
            //        }

            //    }
            //    Debug.Log("min " + min + " max " + max);
            //}
        }

        protected override void set_update_state(bool doUpdate) {
            if (doUpdate) {
                start_recording_new_clip();
            } else {
                stop_current_recording();
            }
        }

        public override void play() {
            start_recording_new_clip();
        }

        public override void pause() {
            stop_current_recording();
        }
    }
}