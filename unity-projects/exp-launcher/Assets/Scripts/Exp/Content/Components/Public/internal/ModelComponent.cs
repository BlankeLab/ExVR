
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

namespace Ex {

    public class ModelComponent : ExComponent {

        protected MeshRenderer mr = null;
        protected MeshFilter mf = null;
        protected Material m = null;
        protected Rigidbody r = null;
        protected Collider c = null;

        protected string currentRgbaTextureAlias = "";
        protected string currentNormalTextureAlias = "";
        protected string currentHeightTextureAlias = "";

        #region ex_functions
        protected override bool initialize() {

            // slots
            add_slot("visibility", (visibility) => { set_visibility((bool)visibility); });
            add_slot("position", (position) => { transform.localPosition = (Vector3)position; });
            add_slot("rotation", (rotation) => { transform.localEulerAngles = (Vector3)rotation; });
            add_slot("scale", (scale) => { transform.localScale = (Vector3)scale; });
            add_slot("transform", (value) => {
                var transformV = (TransformValue)value;
                transform.localPosition = transformV.position;
                transform.localRotation = transformV.rotation;
                transform.localScale = transformV.scale;
            });
            add_slot("RGBA texture", (imageContainer) => {
                set_rgba_texture(((ImageContainer)imageContainer).texture);
            });
            // generate 
            // # mesh renderer
            mr = gameObject.AddComponent<MeshRenderer>();
            if (initC.get<bool>("transparent")) {
                mr.material = (m = ExVR.GlobalResources().instantiate_default_transparent_mat());
            } else {
                mr.material = (m = ExVR.GlobalResources().instantiate_default_mat());
            }
            mr.enabled = false;

            // # mesh filter
            mf = gameObject.AddComponent<MeshFilter>();
            generate_mesh();

            // # rigidbody
            r = gameObject.AddComponent<Rigidbody>();
            r.useGravity = false;
            r.mass = 0f;
            r.drag = 0f;
            r.velocity = Vector3.zero;
            r.detectCollisions = false;
            r.angularVelocity = Vector3.zero;

            // # collider
            generate_collider();

            return true;
        }
        protected override void start_experiment() {
            if (!initC.get<bool>("init_transform_do_not_apply")) {
                initC.update_transform("init_transform", transform, true);
            }
        }

        protected override void start_routine() {
            load_rgba_texture_from_resource(currentC.get_resource_alias("rgba_t"));
            load_normal_texture_from_resource(currentC.get_resource_alias("normal_t"));
            load_height_texture_from_resource(currentC.get_resource_alias("height_t"));
        }

        public override void update_from_current_config() {

            if (!currentC.get<bool>("transform_do_not_apply")) {
                currentC.update_transform("transform", transform, true);
            }
            set_color(currentC.get_color("color"));
            set_metallic(currentC.get<float>("metallic"));
            set_smoothness(currentC.get<float>("smoothness"));

            mr.shadowCastingMode = (UnityEngine.Rendering.ShadowCastingMode)currentC.get<int>("shadow_mode");
            mr.receiveShadows = currentC.get<bool>("receive_shadows");

            r.useGravity = currentC.get<bool>("enable_gravity");
            r.mass = currentC.get<float>("mass");
            r.drag = currentC.get<float>("drag");
            r.velocity = Vector3.zero;
            r.angularVelocity = Vector3.zero;
            r.detectCollisions = currentC.get<bool>("detect_collisions");

            if (c != null) {
                c.enabled = currentC.get<bool>("enable_collider");
            }
        }

        protected override void update_parameter_from_gui(string updatedArgName) {

            update_from_current_config();

            if (updatedArgName == "rgba_t") {
                load_rgba_texture_from_resource(currentC.get_resource_alias(updatedArgName));
            } else if (updatedArgName == "normal_t") {
                load_normal_texture_from_resource(currentC.get_resource_alias(updatedArgName));
            } else if (updatedArgName == "height_t") {
                load_height_texture_from_resource(currentC.get_resource_alias(updatedArgName));
            }
        }

        protected override void set_visibility(bool visibility) {
            mr.enabled = visibility;
        }

        #endregion
        #region private_functions

        protected virtual void generate_mesh() { }

        protected virtual void generate_collider() { }

        #endregion

        #region public_functions

        public void load_rgba_texture_from_resource(string textureAlias) {

            if (currentRgbaTextureAlias == textureAlias) {
                return;
            }

            if (textureAlias.Length != 0) {
                set_rgba_texture(ExVR.Resources().get_image_file_data(textureAlias).texture);
            } else {
                set_rgba_texture(null);
            }

            currentRgbaTextureAlias = textureAlias;
        }

        public void load_normal_texture_from_resource(string textureAlias) {

            if (currentNormalTextureAlias == textureAlias) {
                return;
            }

            if (textureAlias.Length != 0) {
                set_normal_texture(ExVR.Resources().get_image_file_data(textureAlias).texture);
            } else {
                set_normal_texture(null);
            }

            currentNormalTextureAlias = textureAlias;
        }

        public void load_height_texture_from_resource(string textureAlias) {

            if (currentHeightTextureAlias == textureAlias) {
                return;
            }

            if (textureAlias.Length != 0) {
                set_height_texture(ExVR.Resources().get_image_file_data(textureAlias).texture);
            } else {
                set_height_texture(null);
            }

            currentHeightTextureAlias = textureAlias;
        }

        public void set_rgba_texture(Texture2D texture) {
            m.SetTexture("_MainTex", texture);
        }

        public void set_normal_texture(Texture2D texture) {
            m.SetTexture("_BumpMap", texture);
        }

        public void set_height_texture(Texture2D texture) {
            m.SetTexture("_ParallaxMap", texture);
        }

        public void set_color(Color color) {
            m.SetColor("_Color", color);
        }

        public void set_metallic(float metallic) {
            m.SetFloat("_Metallic", metallic);
        }

        public void set_smoothness(float smoothness) {
            m.SetFloat("_Glossiness", smoothness);
        }

        #endregion

    }
}