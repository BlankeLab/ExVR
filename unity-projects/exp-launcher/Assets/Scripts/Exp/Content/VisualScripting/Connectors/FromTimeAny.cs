
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

    public class FromTimeAnyConnector : ExConnector {

        object input0 = null;

        protected override bool initialize() {

            add_signals(2);
            add_slot(0, (input) => {       
                base_slot1(input); }
            );

            return true;
        }

        protected override void slot1(object arg) {
            input0 = arg;
            compute();
        }

        private void compute() {

            var timeAny = (TimeAny)input0;
            invoke_signal(0, timeAny.time);
            if (timeAny.value != null) {
                invoke_signal(1, timeAny.value);
                send_connector_infos_to_gui(string.Format("Type:{0}", Converter.get_type_name(timeAny.value.GetType())));                
            } else {
                send_connector_infos_to_gui("Type: NULL");
            }            
        }
    }
}