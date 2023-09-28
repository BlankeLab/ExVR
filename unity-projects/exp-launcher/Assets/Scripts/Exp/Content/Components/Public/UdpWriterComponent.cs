
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
using System.Diagnostics;

namespace Ex{

    public class UdpWriterComponent : ExComponent{

        private UdpSender m_udpSender = null;
        static private readonly string m_messageSentSignalStr = "message sent";

        protected override bool initialize() {

            // signals
            add_slot("send message", (message) => {
                UnityEngine.Debug.Log("send " + (string)message);
                m_udpSender.send_message((string)message);
            });
            add_signal(m_messageSentSignalStr);

            bool ipv6 = false; // initC.get<bool>("ipv6");
            var ipAddresses = NetworkInfo.get_ip_addresses(initC.get<string>("writing_address"),ipv6);
            if (ipAddresses.Count == 0) {
                log_error("Cannot find any ip address from: " + initC.get<string>("writing_address"));
                return false;
            }

            m_udpSender = new UdpSender();
            return m_udpSender.initialize(initC.get<int>("writing_port"), ipAddresses[0]);
        }

        protected override void clean() {
            m_udpSender.clean();
        }

        protected override void update() {


            TimeAny trigger;
            List<TimeAny> triggers = null;
            while (m_udpSender.triggersSent.TryDequeue(out trigger)) {
                if (triggers == null) {
                    triggers = new List<TimeAny>();
                }
                triggers.Add(trigger);                
            }

            if (is_updating()) {
                if (triggers != null) {
                    UnityEngine.Debug.Log("trigg");
                    foreach (var trigger2 in triggers) {
                        
                        invoke_signal(m_messageSentSignalStr, trigger2);
                    }
                }
            }
        }

        public void send_message(string message) {
            if (is_updating()) {                
                m_udpSender.send_message(message);
            }
        }

        public void send_bytes(byte[] bytes) {
            if (is_updating()) {
                m_udpSender.send_bytes(bytes);
            }
        }
    }
}