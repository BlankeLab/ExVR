
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

    public class VideoResourceComponent : ExComponent{

        private Texture2D m_currentFrame = null;
        private VideoResource m_video = null;

        #region ex_functions
        protected override bool initialize() {

            add_signal("new frame");
            add_slot("pause", (nullArg) => { pause(); });
            add_slot("play", (nullArg) => { play(); });

            return true;
        }

        protected override void start_routine() {

            m_video = ExVR.Resources().get_video_file_data(currentC.get_resource_alias("video"));
            if (!m_video) {
                log_error("Not video resource set.");
                return;
            }
            
            m_video.player.isLooping = true;
            m_video.player.frameReady += on_new_frame;

            if (currentC.get<bool>("play_at_new_routine")) {
                m_video.start();
            }

            if (currentC.get<bool>("play_audio")) {
                m_video.player.audioOutputMode = UnityEngine.Video.VideoAudioOutputMode.Direct;
            } else {
                m_video.player.audioOutputMode = UnityEngine.Video.VideoAudioOutputMode.None;
            }            
        }

        public override void play() {
            if (m_video != null) {
                m_video.play();
            }
        }

        public override void pause() {
            if (m_video != null) {
                m_video.pause();
            }
        }

        protected override void stop_routine() {
            if (m_video != null) {
                m_video.stop();
                m_video.player.frameReady -= on_new_frame;
            }
        }

        protected override void clean() {
            if (m_currentFrame != null) {
                Destroy(m_currentFrame);
            }
        }

        #endregion

        #region private_functions

        private void on_new_frame(UnityEngine.Video.VideoPlayer player, long frameIdx) {

            if (!currentC.get<bool>("play_images")) {
                return;
            }

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

        #endregion
    }
}