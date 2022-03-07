
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

namespace Ex{

    public class RealsToVector2Connector : ExConnector{

        object input0 = null;
        object input1 = null;

        protected override bool initialize() {

            add_signals(1);
            add_slot(0, (arg) => {base_slot1(arg);});
            add_slot(1, (arg) => {base_slot2(arg);});

            return true;
        }

        protected override void slot1(object arg) {
            input0 = arg;
            compute();
        }

        protected override void slot2(object arg) {
            input1 = arg;
            compute();
        }

        private void compute() {

            double v1 = 0, v2 = 0;
            if (input0 != null) {
                v1 = (double)input0;
            }
            if (input1 != null) {
                v2 = (double)input1;
            }

            if (input0 != null || input1 != null) {
                var vec = new Vector2((float)v1, (float)v2);
                invoke_signal(0, vec);
                send_connector_infos_to_gui(Converter.to_string(vec));
            }
        }
    }
}