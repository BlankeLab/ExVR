
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

    public class UdpReaderComponent : ExComponent{

        private UdpReceiver m_udpReceiver = null;
        private static readonly string m_messageReceivedSignalStr = "message";

        protected override bool initialize() {

            // signals
            add_signal(m_messageReceivedSignalStr);

            bool ipv6 = false; // initC.get<bool>("ipv6");
            var ipAddresses = NetworkInfo.get_ip_addresses(initC.get<string>("reading_address"), ipv6);
            if(ipAddresses.Count == 0) {
                log_error("Cannot find any ip address from: " + initC.get<string>("reading_address"));
                return false;
            }
  
            m_udpReceiver = new UdpReceiver();
            return m_udpReceiver.initialize(initC.get<int>("reading_port"), ipAddresses[0]);
        }

        protected override void pre_start_routine() {
            m_udpReceiver.set_reading_state(true);
        }    

        protected override void update() {

            var messages = m_udpReceiver.read_all_messages();
            if(messages != null) {
                foreach(var message in messages) {
                    invoke_signal(m_messageReceivedSignalStr, new TimeAny(message.Item1, message.Item2));
                }
            }
        }

        protected override void stop_routine() {
            // stop reading messages
            m_udpReceiver.set_reading_state(false);
        }

        public override void play() {
            // read messages
            m_udpReceiver.set_reading_state(true);
        }

        public override void pause() {
            // stop reading messages
            m_udpReceiver.set_reading_state(false);
        }

        protected override void clean() {
            m_udpReceiver.clean();
        }
    }
}