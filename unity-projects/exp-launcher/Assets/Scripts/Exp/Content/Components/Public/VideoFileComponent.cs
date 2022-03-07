
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

// unity
using UnityEngine;

namespace Ex{

    public class VideoFileComponent : ExComponent{

        private UnityEngine.Video.VideoPlayer m_player = null;
        private Texture2D m_currentFrame = null;

        protected override bool initialize() {

            add_signal("new frame");
            add_slot("pause", (nullArg) => { pause(); });
            add_slot("play", (nullArg) => { play(); });            

            string videoPath = ExVR.Resources().get_video_file_data(initC.get_resource_alias("video")).path;            
            if (videoPath.Length == 0) {
                return false;
            }

            // init player
            m_player = gameObject.AddComponent<UnityEngine.Video.VideoPlayer>();
            m_player.playOnAwake = false;
            m_player.sendFrameReadyEvents = true;
            m_player.source = UnityEngine.Video.VideoSource.Url;
            m_player.url = videoPath;

            m_player.sendFrameReadyEvents = true;
            m_player.frame = 0;
            m_player.frameReady += on_new_frame;

            return true;
        }

        protected override void start_routine() { 
            m_player.frame = 0;
            m_player.Play();
        }

        public override void play() {
            m_player.Play();
        }

        public override void pause() {
            m_player.Pause();
        }

        protected override void stop_routine() {
            m_player.Stop();
        }
        
        void on_new_frame(UnityEngine.Video.VideoPlayer player, long frameIdx) {

            RenderTexture renderTexture = player.texture as RenderTexture;
            if(m_currentFrame == null) {
                m_currentFrame = new Texture2D(renderTexture.width, renderTexture.height);
            } else {
                if(m_currentFrame.width != renderTexture.width || m_currentFrame.height != renderTexture.height) {
                    m_currentFrame.Resize(renderTexture.width, renderTexture.height);
                }
            }

            RenderTexture.active = renderTexture;
            m_currentFrame.ReadPixels(new Rect(0, 0, renderTexture.width, renderTexture.height), 0, 0);
            m_currentFrame.Apply();
            RenderTexture.active = null;

            invoke_signal("new frame", new ImageContainer(m_currentFrame, true));
        }

        protected override void clean() {
            if(m_currentFrame != null) {
                Destroy(m_currentFrame);
            }
        }
    }
}