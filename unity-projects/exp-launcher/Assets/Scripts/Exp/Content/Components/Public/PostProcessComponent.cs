
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