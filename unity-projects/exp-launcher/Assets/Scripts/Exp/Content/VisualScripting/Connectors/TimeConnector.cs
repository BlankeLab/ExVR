
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

    public class TimeConnector : ExConnector{

        public bool sinceExp = true;
        public bool sendEveryUpdate = true;
        public int frequency = 30;
        private int count = 0;

        protected override bool initialize() {
            update_from_gui();

            add_slot(0, (arg) => {
                var currentTime = sinceExp ? ExVR.Time().ellapsed_exp_ms() : ExVR.Time().ellapsed_element_ms();
                invoke_signal(0, currentTime);
                send_connector_infos_to_gui(Converter.to_string((int)currentTime));                
            });

            add_signals(1);
            return true;
        }

        protected override void update_from_gui() {

            var args = m_config.get_list<string>(valueStr);
            if (args.Count == 3) { 
                sinceExp = (args[0] == "Time since exp");
                frequency = Converter.to_int(args[1]);
                sendEveryUpdate = (args[2] == "At each frame");
                count = 0;
            } else if(args.Count == 2) { // old system
                sinceExp = (args[0] == "Time since exp");
                frequency = Converter.to_int(args[1]);
            } else {
                log_error("Invalid TimeConnector arg.", true);
                return;
            }
        }

        protected override void update() {

            if (!sendEveryUpdate) {
                return;
            }

            var currentTime = sinceExp ? ExVR.Time().ellapsed_exp_ms() : ExVR.Time().ellapsed_element_ms();
            double deltaTime = 1000.0 / frequency;

            if (currentTime / deltaTime > count) {

                invoke_signal(0, currentTime);
                send_connector_infos_to_gui(Converter.to_string((int)currentTime));                
                ++count;
            }
        }

        protected override void stop_routine() {
            count = 0;
        }
    }
}