
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
using UnityEngine.UI;

namespace Ex{

    public class ImageViewerComponent : CanvasWorldSpaceComponent{

        private GameObject m_imageGO = null;
        private RectTransform m_rectTr = null;
        private Texture2D m_currentImage = null;
        private Texture2D m_generatedInputImage = null;

        private int m_currentImageWidth = 0;
        private int m_currentImageHeight = 0;

        #region ex_functions
        protected override bool initialize() {

            // init slots
            add_slot("set image", (imageContainer) => {
                load_image_from_container((ImageContainer)imageContainer);
            });

            // init gameObjects
            m_imageGO = ExVR.GlobalResources().instantiate_prebab("Common/Image", transform);
            m_imageGO.name = "ImageRect";
            m_rectTr = m_imageGO.GetComponent<RectTransform>();

            // init default 
            set_image(ExVR.Resources().get_image_file_data("default_texture").texture);

            return m_imageGO != null;
        }

        protected override void clean() {
            if(m_generatedInputImage != null) {
                Destroy(m_generatedInputImage);
                m_generatedInputImage = null;
            }
        }

        protected override void start_routine() {
            if (!currentC.get<bool>("do_not_load")) {
                load_image_from_resource(currentC.get_resource_alias("image"));
            }
            resize_image();
        }

        protected override void set_visibility(bool visibility) {
            m_imageGO.SetActive(visibility);
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            if(updatedArgName == "image") {
                load_image_from_resource(currentC.get_resource_alias(updatedArgName));
            }
            resize_image();
        }

        protected override void post_update() {
            resize_image();
        }

        #endregion

        #region public_functions

        public void set_pivot(Vector2 pivot) {
            m_rectTr.pivot = pivot;
        }

        public void set_size(Vector2 size) {
            m_rectTr.sizeDelta = size;
        }

        public void resize_image() {

            m_imageGO.transform.position = Vector3.zero;
            m_imageGO.transform.rotation = Quaternion.identity;

            m_rectTr.pivot = new Vector2(0.5f, 0.5f);

            if (currentC.get<bool>("use_eye_camera")) {

                // move to head
                Transform camTr = ExVR.Display().cameras().get_eye_camera_transform();
                m_imageGO.transform.position = camTr.position + camTr.forward * currentC.get<float>("distance");
                m_imageGO.transform.rotation = camTr.rotation;
                m_imageGO.transform.eulerAngles += currentC.get_vector3("rotation");
                m_rectTr.pivot = currentC.get_vector2("pivot");

            } else {
                m_rectTr.localPosition = currentC.get_vector3("position");
                m_rectTr.localEulerAngles = currentC.get_vector3("rotation");                
            }

            m_rectTr.sizeDelta = ((currentC.get<bool>("use_original_size")) ?
                        new Vector2(m_currentImageWidth, m_currentImageHeight) :
                        new Vector2(currentC.get<int>("width"), currentC.get<int>("height"))
            );

            var sf = currentC.get<float>("scale_factor") * 0.01f;
            m_rectTr.localScale = new Vector3(
                sf, sf, sf
            );
        }

        public void load_image_from_resource(string imageAlias) {

            if (imageAlias.Length != 0) {
                set_image(ExVR.Resources().get_image_file_data(imageAlias).texture);
            }
        }

        public void load_image_from_container(ImageContainer imageContainer) {
            if (imageContainer.texture != null) {
                set_image(imageContainer.texture);
            } else {
                m_generatedInputImage = imageContainer.update_texture2D(m_generatedInputImage);
                set_image(m_generatedInputImage);
            }
        }

        public void set_image(Texture2D texture) {

            m_currentImage = texture;

            m_currentImageWidth     = m_currentImage.width;
            m_currentImageHeight    = m_currentImage.height;
            m_imageGO.GetComponent<UnityEngine.UI.Image>().sprite = Sprite.Create(
                m_currentImage, 
                new Rect(0.0f, 0.0f, m_currentImageWidth, m_currentImageHeight), 
                new Vector2(0f, 0f), 
                100.0f, 
                0, 
                SpriteMeshType.FullRect
            );
        }

        #endregion
    }
}