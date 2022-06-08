
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
using UnityEngine.Rendering.PostProcessing;

namespace Ex {

    public class PostProcessComponent : ExComponent {


        #region ex_functions
        protected override bool initialize() {

            return true;
        }

        public override void update_from_current_config() {
            var ppvp = ExVR.Display().postProcessingVolume.profile;
            var ao = ppvp.GetSetting<AmbientOcclusion>();
            ao.active                  = currentC.get<bool>("ao_enable");
            ao.intensity.value         = currentC.get<bool>("ao_intensity_enable") ? currentC.get<float>("ao_intensity") : 0f;
            ao.thicknessModifier.value = currentC.get<bool>("ao_thickness_enable") ? currentC.get<float>("ao_thickness") : 1f;
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