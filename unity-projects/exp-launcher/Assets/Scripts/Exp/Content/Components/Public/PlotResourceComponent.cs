
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

    public class PlotResourceComponent : ExComponent{

        public List<AnimationCurve> curves = null;
        private PlotResource data = null;

        protected override bool initialize() {
            add_signal("plot loaded");
            add_signal("plot loaded alias");
            add_signal("plot loaded path");
            return true;
        }

        public override void update_from_current_config() {
            load_plot();
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        private void load_plot() {

            if (currentC.get<bool>("random")) {
                var plots = currentC.get_plots_resources_list("plots_list");
                if (plots.Count > 0) {
                    data = plots[Random.Range(0, plots.Count - 1)];
                }
            } else {
                data = currentC.get_resource_plot_data("plot");
            }

            curves = new List<AnimationCurve>();            
            if (data != null) {
                foreach(var plot in data.curves) {
                    curves.Add(plot);
                }
            } else {
                // load default                
                curves.Add(new AnimationCurve());                
                // ...
            }

            if (data != null) {
                invoke_signal("plot loaded", curves[0]);
                invoke_signal("plot loaded alias", data.alias);
                invoke_signal("plot loaded path", data.path);
            }

        }
    }
}