
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
using System.IO;

// unity
using UnityEngine;

namespace Ex{

    public class AudioResource : ExResourceFile{

        public AudioType type;
        public AudioClip clip = null;
        private static AudioClip m_lastAudioClipLoaded = null;

        public AudioResource(int key, string alias, string path) : base(key, alias, path) {

            var enumerator = load_audio_clip(path); // load file in couroutine
            while (enumerator.MoveNext()) { // wait for the end
            }

            if (m_lastAudioClipLoaded == null) {
                log_error("Failed to load audio clip with path: " + path);
            } else {
                clip = m_lastAudioClipLoaded;
            }
        }

        private System.Collections.IEnumerator load_audio_clip(string pathAudioFile) {

            // find audio type
            var ext = Path.GetExtension(pathAudioFile);
            AudioType type = AudioType.UNKNOWN;
            if (ext == ".wav") {
                type = AudioType.WAV;
            } else if (ext == ".mp3") {
                type = AudioType.MPEG;
            } else if (ext == ".ogg") {
                type = AudioType.OGGVORBIS;
            } else if (ext == ".aif") {
                type = AudioType.AIFF;
            }

            // send media request
            UnityEngine.Networking.UnityWebRequest request = UnityEngine.Networking.UnityWebRequestMultimedia.GetAudioClip(pathAudioFile, type);
            yield return request.SendWebRequest();

            while (!request.isDone) {
                yield return true;
            }

            if (request.isNetworkError || request.isHttpError) { // check errors
                log_error("Error happened during multimedia request.");
                m_lastAudioClipLoaded = null;
            } else { // get content                
                m_lastAudioClipLoaded = UnityEngine.Networking.DownloadHandlerAudioClip.GetContent(request);
            }
        }
    }

}