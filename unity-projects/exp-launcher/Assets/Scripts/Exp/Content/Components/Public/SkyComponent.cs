
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

namespace Ex {

    public class SkyComponent : ExComponent {

        // default cameras parameters
        private CameraSettings defaultBothEyes;
        //private CameraSettings defaultLeftEye;
        //private CameraSettings defaultRightEye;
        private CameraSettings defaultLeftEyeStereo;
        private CameraSettings defaultRightEyeStereo;
        private CameraSettings defaultDebug;

        // lights
        private Light m_sun = null;

        // skybox
        public enum SkyboxMode { Color = 0, Skybox6, SkyboxCubeMap, SkyboxPanoramic, SkyboxProcedural };
        // # materials
        private Material defaultSkyboxMat = null;        
        private Material skyboxProcedural = null;
        private Material skyboxPanoramic = null;
        private Material skyboxCubeMap = null;
        public Material skybox6Sided = null; // TODO: add access to uniforms
        // # textures
        private List<Texture2D> textures6Sided = new List<Texture2D>();
        private Texture2D texturePanoramic = null;
        private Cubemap textureCubeMap = null;

        #region ex_functions
        protected override bool initialize() {

            // init camera settings
            defaultBothEyes         = new CameraSettings(ExVR.Display().cameras().bothEyesCamera);;
            defaultLeftEyeStereo    = new CameraSettings(ExVR.Display().cameras().leftEyeStereoCamera);
            defaultRightEyeStereo   = new CameraSettings(ExVR.Display().cameras().rightEyeStereoCamera);
            defaultDebug            = new CameraSettings(ExVR.GO().DebugEditorCamera.GetComponent<Camera>());

            // retrieve sun
            m_sun = ExVR.GO().GlobalLights.transform.Find("Sun").GetComponent<Light>();

            // skyboxes
            defaultSkyboxMat    = RenderSettings.skybox;
            // # get materials
            skybox6Sided        = ExVR.GlobalResources().instantiate_mat("Components/Sky/skybox_6sided");
            skyboxCubeMap       = ExVR.GlobalResources().instantiate_mat("Components/Sky/skybox_cubemap");
            skyboxPanoramic     = ExVR.GlobalResources().instantiate_mat("Components/Sky/skybox_panoramic");
            skyboxProcedural    = ExVR.GlobalResources().instantiate_mat("Components/Sky/skybox_procedural");
            // # init textures
            if (textures6Sided.Count == 0) {
                for (int ii = 0; ii < 6; ++ii) {
                    Texture2D tex = new Texture2D(2, 2);
                    tex.filterMode = FilterMode.Trilinear;
                    tex.wrapMode = TextureWrapMode.Clamp;
                    textures6Sided.Add(tex);
                }
            }
            texturePanoramic = new Texture2D(2, 2, TextureFormat.RGB24, false);
            texturePanoramic.filterMode = FilterMode.Trilinear;
            texturePanoramic.wrapMode = TextureWrapMode.Clamp;
            //textureCubeMap = new Cubemap()
            //textureCubeMap = new Texture2D(2, 2);
            //textureCubeMap.filterMode = FilterMode.Trilinear;
            //textureCubeMap.wrapMode = TextureWrapMode.Clamp;

            return true;
        }

        protected override void stop_experiment() {

            defaultBothEyes.reset_default();
            //defaultLeftEye.reset_default();
            //defaultRightEye.reset_default();
            defaultLeftEyeStereo.reset_default();
            defaultRightEyeStereo.reset_default();

            //defaultDebug.reset_default();

            RenderSettings.skybox = defaultSkyboxMat;
            DynamicGI.UpdateEnvironment();
        }

        public override void update_from_current_config() {
            load_all_textures(currentC.get<int>("mode"));
            update_textures_parameters();
        }

        protected override void update_parameter_from_gui(string updatedArgName) {

            if (updatedArgName == "mode") {
                load_all_textures(currentC.get<int>(updatedArgName));
            } else { // reload only current modified texture
                reload_current_texture(updatedArgName);
            }

            //update_from_current_config();
            update_textures_parameters();
        }
        

        protected override void clean() {

            foreach (var text in textures6Sided) {
                if (text != null) {
                    Destroy(text);
                }
            }
            if (texturePanoramic != null) {
                Destroy(texturePanoramic);
            }
            if (textureCubeMap != null) {
                Destroy(textureCubeMap);
            }

            Destroy(skybox6Sided);
            Destroy(skyboxProcedural);
            Destroy(skyboxPanoramic);
            Destroy(skyboxCubeMap);
        }

        #endregion
        #region private_functions
        private Texture2D copy(Texture2D from, Texture2D to, bool invert) {

            // load data
            to.Resize(from.width, from.height);
            to.SetPixels32(from.GetPixels32());
            to.Apply();

            // invert texture
            if (invert) {
                int width   = to.width;
                int height  = to.height;
                Color32[] pixels = to.GetPixels32();
                Color32[] pixelsFlipped = new Color32[pixels.Length];
                for (int ii = 0; ii < height; ii++) {
                    Array.Copy(pixels, ii * width, pixelsFlipped, (height - ii - 1) * width, width);
                }
                to.SetPixels32(pixelsFlipped);
                to.Apply();
            }
            return to;
        }

        private Cubemap copy_to_cubemap(Texture2D from, Cubemap to, bool invert) {

            // TODO INVERT

            int cubedim = from.width / 6;
            if (to != null) {
                Destroy(to);
            }

            // create cubemap
            to = new Cubemap(cubedim, TextureFormat.ARGB32, false);
            to.SetPixels(from.GetPixels(0, 0, cubedim, cubedim), CubemapFace.PositiveX);
            to.SetPixels(from.GetPixels(cubedim, 0, cubedim, cubedim), CubemapFace.NegativeX);
            to.SetPixels(from.GetPixels(2 * cubedim, 0, cubedim, cubedim), CubemapFace.PositiveY);
            to.SetPixels(from.GetPixels(3 * cubedim, 0, cubedim, cubedim), CubemapFace.NegativeY);
            to.SetPixels(from.GetPixels(4 * cubedim, 0, cubedim, cubedim), CubemapFace.PositiveZ);
            to.SetPixels(from.GetPixels(5 * cubedim, 0, cubedim, cubedim), CubemapFace.NegativeZ);
            to.Apply();
            return to;
        }

        private void load_all_textures(int mode) {

            switch ((SkyboxMode)mode) {
                case SkyboxMode.Color:
                    break;
                case SkyboxMode.Skybox6:
                    skybox6Sided.SetTexture("_FrontTex", copy(currentC.get_resource_image("Z+"), textures6Sided[0], currentC.get<bool>("Z+_invert")));
                    skybox6Sided.SetTexture("_BackTex", copy(currentC.get_resource_image("Z-"), textures6Sided[1], currentC.get<bool>("Z-_invert")));
                    skybox6Sided.SetTexture("_LeftTex", copy(currentC.get_resource_image("X+"), textures6Sided[2], currentC.get<bool>("X+_invert")));
                    skybox6Sided.SetTexture("_RightTex", copy(currentC.get_resource_image("X-"), textures6Sided[3], currentC.get<bool>("X-_invert")));
                    skybox6Sided.SetTexture("_UpTex", copy(currentC.get_resource_image("Y+"), textures6Sided[4], currentC.get<bool>("Y+_invert")));
                    skybox6Sided.SetTexture("_DownTex", copy(currentC.get_resource_image("Y-"), textures6Sided[5], currentC.get<bool>("Y-_invert")));
                    break;
                case SkyboxMode.SkyboxCubeMap:
                    skyboxCubeMap.SetTexture("_Tex", copy_to_cubemap(currentC.get_resource_image("cubemap"), textureCubeMap, currentC.get<bool>("cubemap_invert")));
                    break;
                case SkyboxMode.SkyboxPanoramic:
                    skyboxPanoramic.SetTexture("_MainTex", copy(currentC.get_resource_image("panoramic"), texturePanoramic, currentC.get<bool>("panoramic_invert")));
                    break;
                case SkyboxMode.SkyboxProcedural:
                    break;
            }
        }

        private void reload_current_texture(string updatedArgName) {
            var n = updatedArgName.Substring(0, 2);
            if (n == "Z+") {
                skybox6Sided.SetTexture("_FrontTex", copy(currentC.get_resource_image(n), textures6Sided[0], currentC.get<bool>(n + "_invert")));
            } else if (n == "Z-") {
                skybox6Sided.SetTexture("_BackTex", copy(currentC.get_resource_image(n), textures6Sided[1], currentC.get<bool>(n + "_invert")));
            } else if (n == "X+") {
                skybox6Sided.SetTexture("_LeftTex", copy(currentC.get_resource_image(n), textures6Sided[2], currentC.get<bool>(n + "_invert")));
            } else if (n == "X-") {
                skybox6Sided.SetTexture("_RightTex", copy(currentC.get_resource_image(n), textures6Sided[3], currentC.get<bool>(n + "_invert")));
            } else if (n == "Y+") {
                skybox6Sided.SetTexture("_UpTex", copy(currentC.get_resource_image(n), textures6Sided[4], currentC.get<bool>(n + "_invert")));
            } else if (n == "Y-") {
                skybox6Sided.SetTexture("_DownTex", copy(currentC.get_resource_image(n), textures6Sided[5], currentC.get<bool>(n + "_invert")));
            } else if (n == "pa") {
                skyboxPanoramic.SetTexture("_MainTex", copy(currentC.get_resource_image("panoramic"), texturePanoramic, currentC.get<bool>("panoramic_invert")));
            } else if (n == "cu") {
                skyboxCubeMap.SetTexture("_Tex", copy_to_cubemap(currentC.get_resource_image("cubemap"), textureCubeMap, currentC.get<bool>("cubemap_invert")));
            }
        }

        private void update_textures_parameters() {

            // textures parameters
            Material currentMat = null;
            switch ((SkyboxMode)currentC.get<int>("mode")) {
                case SkyboxMode.Color:

                    var color = currentC.get_color("background_color");
                    defaultBothEyes.set_background_color(color);
                    //defaultLeftEye.set_background_color(color);
                    //defaultRightEye.set_background_color(color);
                    defaultLeftEyeStereo.set_background_color(color);
                    defaultRightEyeStereo.set_background_color(color);
                    //defaultDebug.set_background_color(color);
                    currentMat = null;
                    break;
                case SkyboxMode.Skybox6:
                    skybox6Sided.SetColor("_Tint", currentC.get_color("6sided-tint")); // = (0.500000, 0.500000, 0.500000, 0.500000)
                    skybox6Sided.SetFloat("_Exposure", currentC.get<float>("6sided-exposure")); // (0 - 8) = 1
                    skybox6Sided.SetFloat("_Rotation", currentC.get<float>("6sided-rotation")); // (0 -360) = 0
                    currentMat = skybox6Sided;
                    break;
                case SkyboxMode.SkyboxCubeMap:
                    skyboxCubeMap.SetColor("_Tint", currentC.get_color("cubemap-tint")); // = (0.500000, 0.500000, 0.500000, 0.500000)
                    skyboxCubeMap.SetFloat("_Exposure", currentC.get<float>("cubemap-exposure")); // (0 - 8) = 1
                    skyboxCubeMap.SetFloat("_Rotation", currentC.get<float>("cubemap-rotation")); // (0 -360) = 0
                    currentMat = skyboxCubeMap;
                    break;
                case SkyboxMode.SkyboxPanoramic:
                    skyboxPanoramic.SetColor("_Tint", currentC.get_color("panoramic-tint")); // = (0.500000, 0.500000, 0.500000, 0.500000)
                    skyboxPanoramic.SetFloat("_Exposure", currentC.get<float>("panoramic-exposure")); // (0 - 8) = 1
                    skyboxPanoramic.SetFloat("_Rotation", currentC.get<float>("panoramic-rotation")); // (0 -360) = 0
                    skyboxPanoramic.SetFloat("_Mapping", (float)currentC.get<int>("panoramic-mapping"));   // (0 || 1) (6 Frames Layout, Latitude Longitude Layout) = 1
                    skyboxPanoramic.SetFloat("_ImageType", (float)currentC.get<int>("panoramic-type"));   // (0 || 1) (360 Degrees, 0, 180 Degrees, 1) = 0
                    skyboxPanoramic.SetFloat("_MirrorOnBack", currentC.get<bool>("panoramic-mirror_back") ? 1f : 0f); // (0 || 1 ) = 0
                    skyboxPanoramic.SetFloat("_Layout", (float)currentC.get<int>("panoramic-layout")); // (0 || 1 || 2 ) (None, 0, Side by Side, 1, Over Under, 2) = 0.000000
                    currentMat = skyboxPanoramic;
                    break;
                case SkyboxMode.SkyboxProcedural:
                    skyboxProcedural.SetFloat("_SunDisk", (float)currentC.get<int>("sun")); // (None, Simple, High Quality)  = 2.0
                    skyboxProcedural.SetFloat("_SunSize", currentC.get<float>("sun-size")); // Range(0.000000,1.000000 = 0.04
                    skyboxProcedural.SetFloat("_SunSizeConvergence", currentC.get<float>("convergence")); //  Range(1.000000,10.000000) = 5.0
                    skyboxProcedural.SetFloat("_AtmosphereThickness", currentC.get<float>("atmosphere-thickness")); //  Range(0.000000,5.000000) = 1.0
                    skyboxProcedural.SetColor("_SkyTint", currentC.get_color("procedural-sky-tint")); // = (0.500000, 0.500000, 0.500000, 100000)
                    skyboxProcedural.SetColor("_GroundColor", currentC.get_color("procedural-ground-color")); // =(0.369000,0.349000,0.341000,1.000000)
                    skyboxProcedural.SetFloat("_Exposure", currentC.get<float>("procedural-exposure")); // (0 - 8) = 1.3
                    currentMat = skyboxProcedural;
                    break;
            }

            // set camera clear flags            
            if (currentMat != null) {
                RenderSettings.skybox = currentMat;
                defaultBothEyes.set_clear_flags(CameraClearFlags.Skybox);
                //defaultLeftEye.set_clear_flags(CameraClearFlags.Skybox);
                //defaultRightEye.set_clear_flags(CameraClearFlags.Skybox);
                defaultLeftEyeStereo.set_clear_flags(CameraClearFlags.Skybox);
                defaultRightEyeStereo.set_clear_flags(CameraClearFlags.Skybox);
                //defaultDebug.set_clear_flags(CameraClearFlags.Skybox);
            } else {
                RenderSettings.skybox = defaultSkyboxMat;
                defaultBothEyes.set_clear_flags(CameraClearFlags.Color);
                //defaultLeftEye.set_clear_flags(CameraClearFlags.Color);
                //defaultRightEye.set_clear_flags(CameraClearFlags.Color);
                defaultLeftEyeStereo.set_clear_flags(CameraClearFlags.Color);
                defaultRightEyeStereo.set_clear_flags(CameraClearFlags.Color);
                //defaultDebug.set_clear_flags(CameraClearFlags.Color);
            }


            // update environment
            DynamicGI.UpdateEnvironment();

            // ambient 
            RenderSettings.ambientMode = (UnityEngine.Rendering.AmbientMode)currentC.get<int>("ambient_mode");
            // # skybox
            RenderSettings.ambientIntensity = currentC.get<float>("ambient_intensity");
            // # flat
            RenderSettings.ambientLight = currentC.get_color("ambient_color");
            // # gradient
            RenderSettings.ambientGroundColor = currentC.get_color("ground_color");
            RenderSettings.ambientEquatorColor = currentC.get_color("equator_color");
            RenderSettings.ambientSkyColor = currentC.get_color("sky_color");

            // sun
            m_sun.transform.localEulerAngles = currentC.get_vector3("sun_rotation");
            m_sun.intensity = currentC.get<float>("sun_intensity");
            m_sun.bounceIntensity = currentC.get<float>("sun_bounce_intensity");
            m_sun.color = currentC.get_color("sun_color");
            m_sun.shadowStrength = currentC.get<float>("sun_shadow_strength");
            m_sun.shadowBias = currentC.get<float>("sun_shadow_bias");
            m_sun.shadowNormalBias = currentC.get<float>("sun_shadow_normal_bias");
            m_sun.shadowNearPlane = currentC.get<float>("sun_shadow_near_plane");
            m_sun.shadows = ((LightShadows)currentC.get<int>("sun_shadow_mode"));

            //log_message("" + m_sun.transform.localEulerAngles + " " + m_sun.intensity + " " + m_sun.color);

            //if (MovingTheSun.Rot >= 65) {RenderSettings.skybox = day;}
            //else if (MovingTheSun.Rot< 65 && MovingTheSun.rot >= 0) {RenderSettings.skybox = darkday;}
            //else if (MovingTheSun.Rot< 0 && MovingTheSun.rot >= -65) {RenderSettings.skybox = evening;}
            //else {RenderSettings.skybox = night;}

            // fog
            //  fog Is fog enabled?
            //  fogColor The color of the fog.
            //  fogDensity The density of the exponential fog.
            //  fogEndDistance The ending distance of linear fog.
            //  fogMode Fog mode to use.
            //  fogStartDistance The starting distance of linear fog.

            // halo
            //  haloStrength Size of the Light halos.

            // flare
            //  flareFadeSpeed The fade speed of all flares in the Scene.
            //  flareStrength The intensity of all flares in the Scene.
        }

        #endregion
        #region public_functions

        //public void set_mode(int mode) {
        //    load_all_textures(mode);
        //    update_from_current_config();
        //}

        #endregion
    }
}