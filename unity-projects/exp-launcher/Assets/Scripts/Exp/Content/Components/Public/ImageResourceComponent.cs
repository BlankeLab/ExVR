
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

// unity
using UnityEngine;

namespace Ex{

    public class ImageResourceComponent : ExComponent {

        private bool sentWhenExpStarted = false;
        public List<ImageResource> images = null;
        public Dictionary<string, ImageResource> imagesPerAlias = null;

        #region ex_functions
        protected override bool initialize() {
            add_signal("image");
            add_signal("alias");
            add_signal("path");
            add_slot("image from alias", (value) => {
                var alias = (string)value;
                send_image(alias);
            });
            add_slot("image from id", (value) => {
                var id = (int)value;
                send_image(id);
            });

            images = initC.get_images_resources_list("images_list");
            imagesPerAlias = new Dictionary<string, ImageResource>(images.Count);
            foreach (var image in images) {

                var imageResource = ExVR.Resources().get_image_file_data(image.alias);
                if(imageResource != null) {
                    imagesPerAlias[image.alias] = imageResource;
                } else {
                    return false;
                }                
            }

            return true;
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
            send_image();
        }

        protected override void start_experiment() {
            sentWhenExpStarted = false;  
        }

        protected override void pre_start_routine() {

            if (!sentWhenExpStarted && currentC.get<bool>("start_exp")) {
                send_image();
            }
            sentWhenExpStarted = true;


            if (currentC.get<bool>("start_routine")) {
                send_image();
            }
        }

        protected override void stop_routine() {
            if (currentC.get<bool>("stop_routine")) {
                send_image();
            }
        }

        #endregion ex_functions
        #region private_functions

        private void send_image(ImageResource image) {
            if (image != null) {
                invoke_signal("image", new ImageContainer(image.texture, false));
                invoke_signal("alias", image.alias);
                invoke_signal("path", image.path);
            }
        }

        #endregion
        #region public_functions

        public void send_image() {
            if (currentC.get<bool>("use_alias")) {
                send_image(currentC.get<string>("alias"));
            } else {
                send_image(currentC.get<int>("id"));
            }
        }

        public void send_image(int id) {
            var img = image(id);
            if (img != null) {
                send_image(img);
            }
        }

        public void send_image(string alias) {
            var img = image(alias);
            if (img != null) {
                send_image(img);
            }
        }


        ImageResource image(int id) {
            if (id < images.Count) {
                return images[id];
            } 
            log_error(string.Format("No image with id {0} available in init config images resources list. ", id));
            return null;
        }

        ImageResource image(string alias) {
            if (imagesPerAlias.ContainsKey(alias)) {
                return imagesPerAlias[alias];
            } 
            log_error(string.Format("No alias {0} available in init config images resources list. ", alias));
            return null;
        }


        #endregion
    }
}