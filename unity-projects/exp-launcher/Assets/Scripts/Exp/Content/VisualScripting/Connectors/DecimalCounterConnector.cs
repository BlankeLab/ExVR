
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

    public class DecimalCounterConnector : ExConnector{

        DecimalValue current = new DecimalValue(0);

        protected override bool initialize() {

            add_signals(1);
            add_slot(0, (arg) => { base_slot1(arg); });
            add_slot(1, (arg) => { base_slot2(arg); });
            add_slot(2, (nullArg) => { base_slot3(null); });
            return true;
        }

        protected override void slot1(object arg) {
            current.add((DecimalValue)arg);
            send_output();
        }
        protected override void slot2(object arg) {
            current.sub((DecimalValue)arg);
            send_output();
        }
        protected override void slot3(object arg) {
            current = new DecimalValue(0);
            send_output();
        }

        private void send_output() {
            send_connector_infos_to_gui(Converter.to_string(current));
            invoke_signal(0, current);
        }

        protected override void pre_start_routine() {
            current = new DecimalValue(0);
        }

        protected override void stop_routine() {
            current = null;
        }
    }
}