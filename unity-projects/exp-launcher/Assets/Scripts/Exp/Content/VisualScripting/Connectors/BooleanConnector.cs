
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

namespace Ex {

    public class BooleanConnector : ExConnector{

        private object inputValue = null;
        private bool uiValue = false;

        protected override bool initialize(){

            add_signals(1);
            add_slot(0, (arg) => { base_slot1(arg); });
            add_slot(1, (nullArg) =>    { base_slot2(null); });

            uiValue = m_config.get<bool>(valueStr);

            return true;
        }

        protected override void slot1(object arg) {
            inputValue = arg;
            if (inputValue != null) {
                send_output((bool)inputValue);
            } else {
                send_output(uiValue);
            }
        }

        protected override void slot2(object nullArg) {

            if (inputValue != null) {
                inputValue = !(bool)inputValue;                
            } else {
                inputValue = !uiValue;
            }
            send_output((bool)inputValue);
        }

        protected override void update_from_gui() {
            uiValue = m_config.get<bool>(valueStr);
            pre_start_routine();
        }

        protected override void pre_start_routine() {
            if (inputGO.Count == 0) {
                send_output(uiValue);
            }
        }

        void send_output(bool value) {
            send_connector_infos_to_gui(Converter.to_string(value));
            invoke_signal(0, value);
        }

        protected override void stop_routine() {
            inputValue = null;
        }
    }
}