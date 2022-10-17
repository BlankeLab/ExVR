
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

    public class ImageContainer{

        public int width;
        public int height;
        public Color32[] pixels = null;
        public Texture2D texture = null;

        public ImageContainer(WebCamTexture webcamTexture) {
            pixels = webcamTexture.GetPixels32();
            width = webcamTexture.width;
            height = webcamTexture.height;
        }

        public ImageContainer(Texture2D texture, bool copyData) {

            width = texture.width;
            height = texture.height;

            if (copyData) {
                pixels = texture.GetPixels32();
            } else {
                this.texture = texture;
            }
        }

        public Texture2D update_texture2D(Texture2D texture) {

            if (texture == null) {
                texture = new Texture2D(width, height);
                texture.filterMode = FilterMode.Trilinear;
                texture.wrapMode = TextureWrapMode.Clamp;
                texture.SetPixels32(pixels);
                texture.Apply();
            } else {
                if (texture.width != width || texture.height != height) {
                    texture.Resize(width, height);
                }
                texture.SetPixels32(pixels);
                texture.Apply();
            }
            return texture;
        }
    }

    public class ImageResource : ExResourceFile{

        public byte[] bytes = null;
        public Texture2D texture = null;

        public ImageResource(int key, string alias, string path) : base(key, alias, path) {
            texture = new Texture2D(2, 2);//, TextureFormat.RGB24, false);

            // set textures parameters
            //texture.filterMode = FilterMode.Trilinear;
            texture.wrapMode = TextureWrapMode.Clamp;
            texture.filterMode = FilterMode.Point;

        }
        public override bool read_data() {

            if (path.Length == 0) {
                bytes = null;
                return false;
            }

            try {
                bytes = File.ReadAllBytes(path);
            } catch (System.Exception ex) {
                log_error(string.Format("Cannot read image file {0}, error: {1}", path, ex.Message));
                bytes = null;
                return false;
            }
            return true;
        }

        public override bool initialize() {
            
            bool loaded = false;
            if (bytes != null) {
                if (bytes.Length > 0) {
                    loaded = texture.LoadImage(bytes);  //..this will auto-resize the texture dimensions.
                } else {
                    log_error(string.Format("Can't create texture from image file {0}. Replaced with default image instead.", path));
                }
            }

            if (!loaded) {
                texture.Resize(500, 500);
                Color[] colors = new Color[500 * 500];
                Color black = new Color(1, 0, 0, 1);
                for (int ii = 0; ii < colors.Length; ++ii) {
                    colors[ii] = black;
                }
                texture.SetPixels(colors);
                texture.Apply();
            }

            return true;
        }


        public override void clean() {
            if (texture != null) {
                ExVR.Memory().delete_texture(texture);
            }
        }
    }
}