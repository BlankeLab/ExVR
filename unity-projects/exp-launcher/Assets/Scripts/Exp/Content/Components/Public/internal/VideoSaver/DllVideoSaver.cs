
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


namespace Ex.DLL{

    public class VideoSaver : DLLExComponent{

        byte[] m_byteData = null;

        //ExComponent m_component = null;
        public void add_frame(ImageContainer image) {

            //Profiler.BeginSample("[ExVR][VideoSaver] add_frame 1");
            if (m_byteData == null) {
                m_byteData = new byte[image.width * image.height * 3];
            }
            if(m_byteData.Length != image.width * image.height * 3) {
                m_byteData = new byte[image.width * image.height * 3];
            }

            int id = 0;
            for (int ii = 0; ii < image.width; ++ii) {
                int currH = ii * image.height;
                for (int jj = 0; jj < image.height; ++jj) {
                    int currId = 3 * id;
                    var pixel = image.pixels[currH + jj];
                    m_byteData[currId + 0] = pixel.b;
                    m_byteData[currId + 1] = pixel.g;
                    m_byteData[currId + 2] = pixel.r;
                    ++id;
                }
            }
            //Profiler.EndSample();

            //Profiler.BeginSample("[ExVR][VideoSaver] add_frame 2");
            add_frame_video_saver_component(_handle, image.width, image.height, m_byteData, "rgba", 0);
            //Profiler.EndSample();
        }

        #region memory_management

        /// <summary>
        /// DllVideoSaver default constructor
        /// </summary>
        public VideoSaver() : base() {
        }

        /// <summary>
        /// Allocate DLL memory
        /// </summary>
        protected override void create_DLL_class() {
            _handle = new HandleRef(this, create_video_saver_component());
        }

        /// <summary>
        /// Clean DLL memory
        /// </summary>
        protected override void delete_DLL_class() {
            delete_video_saver_component(_handle);
        }

        #endregion memory_management    
        #region DllImport

        //// memory management
        [DllImport("exvr-export", EntryPoint = "create_video_saver_component", CallingConvention = CallingConvention.Cdecl)]
        static private extern IntPtr create_video_saver_component();

        [DllImport("exvr-export", EntryPoint = "delete_video_saver_component", CallingConvention = CallingConvention.Cdecl)]
        static private extern void delete_video_saver_component(HandleRef handle);

        [DllImport("exvr-export", EntryPoint = "add_frame_video_saver_component", CallingConvention = CallingConvention.Cdecl)]
        static private extern int add_frame_video_saver_component(HandleRef handle, int width, int height, byte[] frameData, string format, int dataType);

        #endregion DllImport        
    }
}
