
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

namespace Ex{

    public class CurveXConnector : ExConnector{

        public List<double> stackValues = new List<double>();
        public int frequency = 30;
        private int count = 0;

        protected override bool initialize() {
            add_slot(0, (arg) => { base_slot1(arg); });
            return true;
        }
        
        protected override void slot1(object arg) {
            stackValues.AddRange((List<double>)arg);
        }

        protected override void stop_routine() {
            count = 0;
        }

        protected override void update() {

            double totalTime = ExVR.Time().ellapsed_element_ms();
            double deltaTime = 1000.0 / frequency;
            if (totalTime / deltaTime > count) {
                if (stackValues.Count > 0) {
                    send_output(stackValues);
                    stackValues.Clear();
                }
                ++count;
            }
        }

        void send_output(List<double> values) {
            send_connector_infos_to_gui(Converter.to_string(values));
        }
    }
}