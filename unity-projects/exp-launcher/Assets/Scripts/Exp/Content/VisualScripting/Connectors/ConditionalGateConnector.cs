﻿
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

    public class ConditionalGateConnector : ExConnector{

        private object inputStateValue = null;
        private bool uiStateValue = false;
        protected override bool initialize() {

            uiStateValue = m_config.get<bool>(valueStr);

            add_signals(1);            
            add_slot(0, (arg) =>     { base_slot1(arg); });
            add_slot(1, (arg) =>     { base_slot2(arg); });
            return true;
        }

        protected override void slot1(object arg) {
            if(inputStateValue == null) {
                if (uiStateValue) {
                    invoke_signal(0, arg);
                }
            } else {
                if ((bool)inputStateValue) {                    
                    invoke_signal(0, arg);
                }
            }
        }

        protected override void slot2(object arg) {
            inputStateValue = (bool)arg;
            send_connector_infos_to_gui(Converter.to_string(inputStateValue));
        }
    }
}