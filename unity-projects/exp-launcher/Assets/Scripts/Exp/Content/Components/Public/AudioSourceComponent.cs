
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

// unity
using UnityEngine;

namespace Ex{

    public class AudioSourceComponent : ExComponent{

        // signals
        private static readonly string sampleValueChannelStr = "sample value channel";

        private Mesh m_audioMesh = null;        
        private AudioClip m_audioClip = null;
        private AudioData m_audioData = null;

        private bool m_playNewBlock  = false;
        private bool m_pauseNewBlock = false;
        private bool m_stopNewBlock  = false;

        private bool m_playEndBlock  = false;
        private bool m_pauseEndBlock = false;
        private bool m_stopEndBlock  = false;

        public GameObject audioSourceGO = null;
        public AudioSource audioSource = null;
        public SteamAudio.SteamAudioAmbisonicSource ambiSource = null;

        #region ex_functions
        protected override bool initialize() {

            // slots
            add_slot("play", (nullArg) => {
                start_sound();
            });
            add_slot("pause", (nullArg) => {
                pause();
            });
            add_slot("set time", (time) => {
                set_time((float)time);
            });
            add_slot("set volume", (volume) => {
                set_volume((float)volume);
            });
            // signals
            add_signal("sample value channel");

            audioSourceGO = ExVR.GlobalResources().instantiate_prebab("Components/AudioSource", transform, "Audio source");
            if (audioSourceGO == null) {
                log_error("Failed to load audio source GameObject");
                return false;
            }
            audioSourceGO.SetActive(true);
            m_audioMesh = audioSourceGO.GetComponent<MeshFilter>().mesh;
            audioSourceGO.GetComponent<MeshFilter>().mesh = null;            

            audioSource             = audioSourceGO.GetComponent<AudioSource>();
            ambiSource              = audioSourceGO.GetComponent<SteamAudio.SteamAudioAmbisonicSource>();
            audioSource.clip        = null;
            audioSource.playOnAwake = false;            
            audioSource.loop        = false;
            audioSource.spatialize  = false;

            var audioFileData    = initC.get_resource_audio_data("sound");
            var assetBundleAlias = initC.get_resource_alias("asset_bundle");

            if (audioFileData == null && assetBundleAlias.Length == 0) {
                log_error("No resource file defined as a source.");
                return false;
            }

            if (audioFileData != null) {

                m_audioClip = audioFileData.clip;

            }else if(assetBundleAlias.Length != 0) {

                // instantiate asset bundle
                var assetBundleGO = ExVR.Resources().instantiate_asset_bundle(assetBundleAlias, "", transform);
                if (assetBundleGO == null) {
                    log_error(string.Format("Cannot instantiate audio asset bundle with alias [{0}]", assetBundleAlias));
                    return false;
                }
                //assetBundleGO.transform.SetParent(audioSourceGO.transform);

                // retrieve audio sources
                var subComponents = assetBundleGO.transform.GetComponents(typeof(Behaviour));
                var audioSources = new List<AudioSource>();
                foreach (Behaviour component in subComponents) {
                    if(component.GetType() == typeof(AudioSource)) {
                        audioSources.Add((AudioSource)component);
                    }         
                }

                if(audioSources.Count == 0) {
                    log_error("No audio source found in audio asset bundle.");
                    return false;
                }

                // set first found audio source
                m_audioClip = audioSources[0].clip;
                Destroy(assetBundleGO);
            }

            if (m_audioClip == null) {
                log_error("No audio file resource defined as a source.");
                return false;
            }

            // send sound characteristices
            List<string> infos = new List<string>();
            infos.Add(m_audioClip.name);
            infos.Add(Converter.to_string(m_audioClip.channels));
            infos.Add(Converter.to_string(m_audioClip.frequency));
            infos.Add(Converter.to_string(m_audioClip.length));
            infos.Add(Converter.to_string(m_audioClip.samples));
            infos.Add(Converter.to_string(m_audioClip.ambisonic));           

            send_infos_to_gui_init_config("input_sound_info", String.Join("?", infos.ToArray()));

            if (initC.get<bool>("generate_new_sound")) {
                if (!generate_new_sound_from_input()) {
                    return false;
                }
            }

            audioSource.clip = m_audioClip;
            m_audioData = new AudioData(audioSource.clip);

            //float[] samples = new float[audioSource.clip.samples * audioSource.clip.channels];
            //audioSource.clip.GetData(samples, 0);

            //float minA = 1000f;
            //float maxA = -1000f;
            //float averageA = 0f;
            //for (int jj = 0; jj < samples.Length; ++jj) {
            //    var v = samples[jj];
            //    averageA += v;
            //    if (v < minA) {
            //        minA = v;
            //    }
            //    if (v > maxA) {
            //        maxA = v;
            //    }
            //}
            //averageA /= samples.Length;
            //log_message(string.Format("aa audio min {0} max {1} average {2}", minA, maxA, averageA));

            return true;
        }

        public override void update_from_current_config() {

            audioSource.mute = currentC.get<bool>("mute");
            set_volume(currentC.get<float>("volume"));

            //audioSource.spatialBlend = 0;
            bool loop = currentC.get<bool>("loop");
            if (loop != audioSource.loop) {
                audioSource.loop = loop;
            }

            bool spatialize = currentC.get<bool>("spatialized");
            if (spatialize != audioSource.spatialize) {
                audioSource.spatialize = spatialize;
            }

            audioSource.pitch = currentC.get<float>("pitch");
            audioSource.panStereo = currentC.get<float>("stereo");
            audioSource.spatialBlend = currentC.get<float>("spatial_blend");


            if (!currentC.get<bool>("transform_do_not_apply")) {
                currentC.update_transform("transform", transform);
            }
            
            audioSource.dopplerLevel = currentC.get<float>("doppler_level");
            audioSource.spread = currentC.get<int>("spread");

            var rollof = (AudioRolloffMode)currentC.get<int>("rollof_mode");
            if (rollof != audioSource.rolloffMode) {
                audioSource.rolloffMode = rollof;
            }

            var minDistance = currentC.get<float>("min_distance");
            if (minDistance != audioSource.minDistance) {
                audioSource.minDistance = minDistance;
            }
            var maxDistance = currentC.get<float>("max_distance");
            if (maxDistance != audioSource.maxDistance) {
                audioSource.maxDistance = maxDistance;
            }

            // new/end blocks
            m_playNewBlock  = currentC.get<bool>("play_new_block");
            m_pauseNewBlock = currentC.get<bool>("pause_new_block");
            m_stopNewBlock  = currentC.get<bool>("stop_new_block");

            m_playEndBlock  = currentC.get<bool>("play_end_block");
            m_pauseEndBlock = currentC.get<bool>("pause_end_block");
            m_stopEndBlock  = currentC.get<bool>("stop_end_block");

            set_visibility(is_visible());
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }


        protected override void set_visibility(bool visibility) {
            audioSourceGO.GetComponent<MeshFilter>().mesh = (visibility && currentC.get<bool>("display")) ? m_audioMesh : null;
        }

        protected override void set_update_state(bool doUpdate) {


            if (doUpdate) {

                if (!audioSource.isPlaying && m_playNewBlock) {
                    start_sound();
                } else if (audioSource.isPlaying && m_stopNewBlock) {
                    stop_sound();
                } else if(audioSource.isPlaying && m_pauseNewBlock) {
                    pause();
                } 

            } else {

                if (!audioSource.isPlaying && m_playEndBlock) {
                    start_sound();
                } else if (audioSource.isPlaying && m_stopEndBlock) {
                    stop_sound();
                } else if (audioSource.isPlaying && m_pauseEndBlock) {
                    pause();
                }
            }
        }

        protected override void update() {

            for (int ii = 0; ii < audioSource.clip.channels; ++ii) {                      
                invoke_signal(sampleValueChannelStr, new IdAny(ii + 1, m_audioData.channel_value(audioSource.timeSamples, ii)));
            }
        }


        #endregion

        #region private_functions

        private bool generate_new_sound_from_input() {

            int newNbChannels = initC.get<int>("new_sound_channel") + 1;

            var newClip = AudioClip.Create(string.Format("{}_modified", m_audioClip.name), m_audioClip.samples, newNbChannels, m_audioClip.frequency, false);

            float[] currentSamples = new float[m_audioClip.samples * m_audioClip.channels];
            m_audioClip.GetData(currentSamples, 0);

            float[] newSamples = new float[m_audioClip.samples * newNbChannels];
            for (int ii = 0; ii < newSamples.Length; ++ii) {
                newSamples[ii] = 0f;
            }

            for (int originChannel = 0; originChannel < newNbChannels; ++originChannel) {

                var copyTo = initC.get<string>(string.Format("channel_{}_copy_destination", originChannel));
                var destinationChannelsStr = copyTo.Split(' ');

                foreach (var destinationChannelStr in destinationChannelsStr) {

                    if (destinationChannelStr.Length == 0) {
                        break;
                    }

                    var destinationChannel = Converter.to_int(destinationChannelStr);
                    if (destinationChannel < 1 || destinationChannel > 8) {
                        log_error("Bad channel value: " + destinationChannel);
                        return false;
                    }
                    destinationChannel--;

                    for (int sampleId = 0; sampleId < m_audioClip.samples; ++sampleId) {
                        newSamples[sampleId * newNbChannels + destinationChannel] = currentSamples[sampleId * m_audioClip.channels + originChannel];
                    }
                }
            }

            newClip.SetData(newSamples, 0);
            m_audioClip = newClip;

            return true;
        }

        #endregion

        #region public_functions

        public void start_sound() {
            audioSource.Play();
            audioSourceGO.GetComponent<MeshRenderer>().material.color = audioSource.mute ? Color.red : Color.green;
        }
        public void stop_sound() {            
            audioSource.Stop();
            audioSourceGO.GetComponent<MeshRenderer>().material.color = Color.red;
        }

        public void mute(bool state) {
            audioSource.mute = state;
            audioSourceGO.GetComponent<MeshRenderer>().material.color = audioSource.mute ? Color.red : Color.green;
            //if (state) {
            //    audioSourceGO.GetComponent<MeshRenderer>().material.color = Color.yellow;
            //}

        }

        public override void play() {
            audioSource.UnPause();
            audioSourceGO.GetComponent<MeshRenderer>().material.color = audioSource.mute ? Color.red : Color.green;
        }

        public override void pause() {
            audioSource.Pause();
            audioSourceGO.GetComponent<MeshRenderer>().material.color = Color.red;
        }

        public void set_time(float time) {
            if(time < audioSource.clip.length) {
                audioSource.time = time;
            }            
        }

        public void set_volume(float volume) {
            audioSource.volume = volume;
        }

        #endregion
    }
}