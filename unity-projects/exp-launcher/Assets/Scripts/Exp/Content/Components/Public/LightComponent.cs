
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

    public class LightComponent : ExComponent {

        private UnityEngine.Light m_light = null;
        private MeshRenderer mr = null;
        protected string currentCookieTextureAlias = "";

        #region ex_functions
        protected override bool initialize() {

            // add slots
            add_slot("visibility", (visibility) => { set_visibility((bool)visibility); });
            add_slot("position", (position) => { transform.localPosition = (Vector3)position; });
            add_slot("rotation", (rotation) => { transform.localEulerAngles = (Vector3)rotation; });
            add_slot("transform", (value) => {
                var transformV = (TransformValue)value;
                transform.localPosition = transformV.position;
                transform.localRotation = transformV.rotation;
                transform.localScale = transformV.scale;
            });
            add_slot("intensity", (intensity) => {set_intensity((float)intensity);});
            add_slot("color", (color) => { set_color((Color)color); });

            // light
            m_light = gameObject.AddComponent<Light>();
            m_light.cullingMask = m_light.cullingMask & ~(1 << Layers.Debug);
            //m_light.lightmapBakeType = LightmapBakeType.Realtime;

            // debug
            mr = gameObject.AddComponent<MeshRenderer>();
            var m = ExVR.GlobalResources().instantiate_default_transparent_mat();
            m.color = new Color(1f, 0.95f, 0f, 0.3f);
            mr.material = m;
            mr.shadowCastingMode = UnityEngine.Rendering.ShadowCastingMode.Off;
            mr.receiveShadows = false;
            mr.lightProbeUsage = UnityEngine.Rendering.LightProbeUsage.Off;
            mr.enabled = false;
            var mf = gameObject.AddComponent<MeshFilter>();
            mf.mesh = Ex.PrimitivesMesh.SphereBuilder.generate(
                0.2f
            );

            return true;
        }
        protected override void start_experiment() {
            if (!initC.get<bool>("init_transform_do_not_apply")) {
                initC.update_transform("init_transform", transform, true);
            }
        }

        protected override void start_routine() {
            load_cookie_texture_from_resource(currentC.get_resource_alias("cookie_t"));
        }

        public override void update_from_current_config() {

            if (!currentC.get<bool>("transform_do_not_apply")) {
                currentC.update_transform("transform", transform, true);
            }

            switch (currentC.get<int>("type")){
                case 0:
                    m_light.type = LightType.Point;
                    break;
                case 1:
                    m_light.type = LightType.Spot;
                    break;
                case 2:
                    m_light.type = LightType.Directional;
                    break;
            }
            set_intensity(currentC.get<float>("intensity"));
            set_color(currentC.get_color("color"));
            m_light.range           = currentC.get<float>("range");
            m_light.spotAngle       = currentC.get<float>("spot_angle");
            //m_light.innerSpotAngle  = currentC.get<float>("inner_spot_angle");
            m_light.cookieSize      = currentC.get<float>("cookie_size");
            m_light.bounceIntensity = currentC.get<float>("bounce_intensity");

            switch (currentC.get<int>("shadows")){
                case 0:
                    m_light.shadows = LightShadows.None;
                    break;
                case 1:
                    m_light.shadows = LightShadows.Hard;
                    break;
                case 2:
                    m_light.shadows = LightShadows.Soft;
                    break;
            }
            m_light.shadowStrength      = currentC.get<float>("shadow_strength");
            m_light.shadowBias          = currentC.get<float>("shadow_bias");
            m_light.shadowNormalBias    = currentC.get<float>("shadow_normal_bias");
            m_light.shadowNearPlane     = currentC.get<float>("shadow_near_plane");

            set_visibility(is_visible());
        }

        protected override void update_parameter_from_gui(string updatedArgName) {

            if (updatedArgName == "cookie_t") {
                load_cookie_texture_from_resource(currentC.get_resource_alias(updatedArgName));
            }

            update_from_current_config();
        }

        protected override void set_visibility(bool visibility) {
            mr.enabled      = visibility && currentC.get<bool>("display_source") && (m_light.type != LightType.Directional);
            m_light.enabled = visibility;
        }

        #endregion


        #region public_functions

        public void set_intensity(float intensity) {
            m_light.intensity = intensity;
        }

        public void set_color(Color color) {
            m_light.color = color;
        }

        public void load_cookie_texture_from_resource(string textureAlias) {

            if (currentCookieTextureAlias == textureAlias) {
                return;
            }

            if (textureAlias.Length != 0) {
                set_cookie_texture(ExVR.Resources().get_image_file_data(textureAlias).texture);
            } else {
                set_cookie_texture(null);
            }

            currentCookieTextureAlias = textureAlias;
        }


        public void set_cookie_texture(Texture2D texture) {
            m_light.cookie = texture;
        }


        #endregion

    }
}