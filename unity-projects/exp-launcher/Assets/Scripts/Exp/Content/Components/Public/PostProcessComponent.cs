
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
using UnityEngine.Rendering.PostProcessing;

namespace Ex {

    public class PostProcessComponent : ExComponent {


        #region ex_functions
        protected override bool initialize() {
            return true;
        }

        protected override void clean() {

            var ppvp = ExVR.Display().postProcessingVolume.profile;
            var cg = ppvp.GetSetting<ColorGrading>();
            cg.active               = true;
            cg.temperature.value = 0f;
            cg.tint.value = 0f;
            cg.postExposure.value = 0f;
            cg.colorFilter.value = Color.white;
            cg.hueShift.value = 0f;
            cg.saturation.value = 0f;
            cg.contrast.value = 0;

            var v = ppvp.GetSetting<Vignette>();
            v.active = false;
            v.color.value = Color.black;
            v.center.value = new Vector2(0.5f, 0.5f);
            v.intensity.value =  0.0f;
            v.smoothness.value = 1.0f;
            v.roundness.value =  1.0f;
            v.rounded.value =  false;

            var ao = ppvp.GetSetting<AmbientOcclusion>();
            ao.active = true;
            ao.intensity.value =  1.58f;
            ao.thicknessModifier.value = 1.96f;
            ao.color.value =  Color.black;


            var bl = ppvp.GetSetting<Bloom>();
            bl.active = false;
        }

        public override void update_from_current_config() {

            var ppvp = ExVR.Display().postProcessingVolume.profile;
            var cg = ppvp.GetSetting<ColorGrading>();
            cg.active               = currentC.get<bool>("cg_enable");
            cg.temperature.value    = currentC.get<bool>("cg_wb_temp_enable") ? currentC.get<float>("cg_wb_temp") : 0f;
            cg.tint.value           = currentC.get<bool>("cg_wb_tint_enable") ? currentC.get<float>("cg_wb_tint") : 0f;
            cg.postExposure.value   = currentC.get<bool>("cg_tone_post_exposure_enable") ? currentC.get<float>("cg_tone_post_exposure") : 0f;
            cg.colorFilter.value    = currentC.get<bool>("cg_tone_color_filter_enable") ? currentC.get_color("cg_tone_color_filter") : Color.white;
            cg.hueShift.value       = currentC.get<bool>("cg_tone_hue_shift_enable") ? currentC.get<int>("cg_tone_hue_shift") : 0;
            cg.saturation.value     = currentC.get<bool>("cg_tone_saturation_enable") ? currentC.get<int>("cg_tone_saturation") : 0;
            cg.contrast.value       = currentC.get<bool>("cg_tone_contrast_enable") ? currentC.get<int>("cg_tone_contrast") : 0;

            var v = ppvp.GetSetting<Vignette>();
            v.active                = currentC.get<bool>("v_enable");
            v.color.value           = currentC.get<bool>("v_color_enable") ? currentC.get_color("v_color") : Color.black;
            v.center.value          = currentC.get<bool>("v_center_enable") ? currentC.get_vector2("v_center") : new Vector2(0.5f,0.5f);
            v.intensity.value       = currentC.get<bool>("v_intensity_enable") ? currentC.get<float>("v_intensity") : 0.5f;
            v.smoothness.value      = currentC.get<bool>("v_smoothness_enable") ? currentC.get<float>("v_smoothness") : 0.5f;
            v.roundness.value       = currentC.get<bool>("v_roundness_enable") ? currentC.get<float>("v_roundness") : 0.5f;
            v.rounded.value         = currentC.get<bool>("v_rounded_enable") ? currentC.get<bool>("v_rounded") : false;

            var ao = ppvp.GetSetting<AmbientOcclusion>();
            ao.active                  = currentC.get<bool>("ao_enable");
            ao.intensity.value         = currentC.get<bool>("ao_intensity_enable") ? currentC.get<float>("ao_intensity") : 0f;
            ao.thicknessModifier.value = currentC.get<bool>("ao_thickness_enable") ? currentC.get<float>("ao_thickness") : 1f;
            ao.color.value             = currentC.get<bool>("ao_color_enable") ? currentC.get_color("ao_color") : Color.black;

            var bl = ppvp.GetSetting<Bloom>();
            //bl.active                  = currentC.get<bool>("bl_enable");
            //bl.intensity.value = currentC.get<bool>("bl_intensity_enable") ? currentC.get<float>("bl_intensity") : 0f; // Strength of the bloom filter
            //bl.threshold.value = currentC.get<bool>("bl_threshold_enable") ? currentC.get<float>("bl_threshold") : 0f; // Filters out pixels under this level of brightness. Value is in gamma-space.


            //[Min(0f)]
            //[Tooltip("Filters out pixels under this level of brightness. Value is in gamma-space.")]
            //public FloatParameter threshold = new FloatParameter {
            //    value = 1f
            //};

            //[Range(0f, 1f)]
            //[Tooltip("Makes transitions between under/over-threshold gradual. 0 for a hard threshold, 1 for a soft threshold).")]
            //public FloatParameter softKnee = new FloatParameter {
            //    value = 0.5f
            //};

            //[Tooltip("Clamps pixels to control the bloom amount. Value is in gamma-space.")]
            //public FloatParameter clamp = new FloatParameter {
            //    value = 65472f
            //};

            //[Range(1f, 10f)]
            //[Tooltip("Changes the extent of veiling effects. For maximum quality, use integer values. Because this value changes the internal iteration count, You should not animating it as it may introduce issues with the perceived radius.")]
            //public FloatParameter diffusion = new FloatParameter {
            //    value = 7f
            //};

            //[Range(-1f, 1f)]
            //[Tooltip("Distorts the bloom to give an anamorphic look. Negative values distort vertically, positive values distort horizontally.")]
            //public FloatParameter anamorphicRatio = new FloatParameter {
            //    value = 0f
            //};

            //[ColorUsage(false, true)]
            //[Tooltip("Global tint of the bloom filter.")]
            //public ColorParameter color = new ColorParameter {
            //    value = Color.white
            //};

            //[FormerlySerializedAs("mobileOptimized")]
            //[Tooltip("Boost performance by lowering the effect quality. This settings is meant to be used on mobile and other low-end platforms but can also provide a nice performance boost on desktops and consoles.")]
            //public BoolParameter fastMode = new BoolParameter {
            //    value = false
            //};

            //[Tooltip("The lens dirt texture used to add smudges or dust to the bloom effect.")]
            //[DisplayName("Texture")]
            //public TextureParameter dirtTexture = new TextureParameter {
            //    value = null
            //};

            //[Min(0f)]
            //[Tooltip("The intensity of the lens dirtiness.")]
            //[DisplayName("Intensity")]
            //public FloatParameter dirtIntensity = new FloatParameter {
            //    value = 0f
            //};
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        #endregion
        #region private_functions
       

        #endregion
        #region public_functions

        #endregion
    }
}