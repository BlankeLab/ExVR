
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

    public class GuiIpc : MonoBehaviour{

        private static readonly string componentInfosFormat     = "[$I1]{0}|{1}|{2}|{3}[/I1$]";
        private static readonly string connectorInfosFormat     = "[$I2]{0}|{1}|{2}|{3}|{4}[/I2$]";

        private static readonly string logFormat                = "[$L]{0}[/L$]";
        private static readonly string warningFormat            = "[$W]{0}[/W$]";
        private static readonly string errorFormat              = "[$E]{0}[/E$]";

        private static readonly string experimentStateGuiFormat = "[$EX]{0}[/EX$]";

        private static readonly string expLauncherStateGuiFormat= "[$EL]{0}[/EL$]";



        public string lastComponentInfo = "";
        public string lastConnectorInfo = "";

        private UdpCommunication m_udpCommunication = null;


        public void send_log_to_GUI(string log) {
            m_udpCommunication.send_message(string.Format(logFormat, log));
        }

        public void send_warning_to_GUI(string log) {
            m_udpCommunication.send_message(string.Format(warningFormat, log));
        }

        public void send_error_to_GUI(string error) {
            m_udpCommunication.send_message(string.Format(errorFormat, error));
        }
        
        public void send_component_infos_to_GUI(string componentKey, string configKey, string id, string value) {
            m_udpCommunication.send_message(string.Format(componentInfosFormat,
                componentKey, 
                configKey, 
                id, 
                value
            ));
        }

        public void send_connector_infos_to_GUI(string routineKey, string conditionKey, string connectorKey, string id, string value) {

            m_udpCommunication.send_message(string.Format(connectorInfosFormat,
                routineKey,
                conditionKey,
                connectorKey,
                id,
                value
            ));
        }

        public void send_experiment_state_to_GUI(string expState) {
            m_udpCommunication.send_message(string.Format(experimentStateGuiFormat, expState));
        }

        public void send_exp_launcher_state_to_GUI(string expState) {
            m_udpCommunication.send_message(string.Format(expLauncherStateGuiFormat, expState));
        }

        public void debug_fake_communication(string cmd) {
            ExVR.Events().gui.MessageFromGUI.Invoke(cmd);
        }


        public void define_udp_ports(int portToWrite) {
            m_udpCommunication = new UdpCommunication(true, true, 
                portToWrite,                            "localhost", 
                -1 /* will be choosen automatically */, "localhost" /**"127.0.0.1"*/);
            m_udpCommunication.set_reading_mode(true);
        }

        public void clean() {
            m_udpCommunication.clean();
        }

        public int reading_port() {
            return m_udpCommunication.receiver_port();
        }

        public bool is_initalized() {
            return m_udpCommunication.receiver_initialized() && m_udpCommunication.sender_initialized();
        }


        void Update() {

            if (m_udpCommunication == null) {
                return;
            }

            // get last 
            for (int ii = 0; ii < 5; ++ii) {
                string cmd = m_udpCommunication.read_message();
                if (cmd.Length == 0) {
                    return;
                }
                ExVR.Events().gui.MessageFromGUI.Invoke(cmd);
            }
        }
    }
}