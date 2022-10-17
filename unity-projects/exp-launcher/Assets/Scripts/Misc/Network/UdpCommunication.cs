
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
using System;
using System.Collections.Generic;

namespace Ex{

    public class UdpCommunication{

        private UdpReceiver m_receiver  = null;
        private UdpSender m_sender      = null;
        private List<byte[]> m_messagesToSend = new List<byte[]>(1000);

        public UdpCommunication(bool write, bool read, int wPort, string wAddress, int rPort, string rAddress) {

            var wIp = NetworkInfo.get_ip_addresses(wAddress, false);
            var rIp = NetworkInfo.get_ip_addresses(rAddress, false);

            if (wIp.Count == 0 || rIp.Count == 0) {
                UnityEngine.Debug.LogError("Cannot found ip addreses.");          
                return;
            }

            if (write) {
                m_sender = new UdpSender();
                m_sender.initialize(wPort, wIp[0]);
            }

            if (read) {

                m_receiver = new UdpReceiver();

                if (rPort == -1) { // undefined reading port
                    for (rPort = wPort + 1; rPort < wPort + 10; ++rPort) {  // try to find aumatically the good reading port

                        m_receiver.initialize(rPort, rIp[0]);
                        if (m_receiver.initialized()) {
                            break;
                        }   
                    }
                } else {
                    m_receiver.initialize(rPort, rIp[0]);
                }

                if (!m_receiver.initialized()) {
                    m_receiver = null;
                    UnityEngine.Debug.LogError("UDP receiver error with : " + rAddress + ":" + rPort);
                }
            }
        }

        public bool receiver_initialized() {
            return m_receiver != null;
        }

        public bool sender_initialized() {
            return m_sender != null;
        }

        public int receiver_port() {
            if (receiver_initialized()) {
                return m_receiver.reading_port();
            }
            return 0;
        }

        public int sender_port() {
            if (sender_initialized()) {
                return m_sender.m_writingPort;
            }            
            return 0;
        }

        public void set_reading_mode(bool readingEnabled) {
            if (receiver_initialized()) {
                m_receiver.set_reading_state(readingEnabled);
            }
        }

        public void send_message(string message) {
            if (sender_initialized()) {
                m_sender.send_message(message);
            }
        }
        public void send_message(byte[] bytes) {
            if (sender_initialized()) {
                m_sender.send_bytes(bytes);
            }
        }

        public void append_message(string message) {
            if (sender_initialized()) {
                m_messagesToSend.Add(System.Text.Encoding.UTF8.GetBytes(message));
            }
        }
        public void append_message(byte[] bytes) {
            if (sender_initialized()) {
                m_messagesToSend.Add(bytes);
            }
        }

        public int send_all_messages() {
            if (sender_initialized()) {
                int nbMessages = m_messagesToSend.Count;
                if (nbMessages > 0) {                    
                    m_sender.send_bytes(Containers.merge(m_messagesToSend));
                    m_messagesToSend = new List<byte[]>(1000);                    
                }
                return nbMessages;
            }
            return -1;
        }

        public Tuple<double, double, string> read_message() {
            if (receiver_initialized()) {
                return m_receiver.read_message();
            }
            return null;
        }

        public List<Tuple<double, double, string>> read_all_messages() {
            if (receiver_initialized()) {
                return m_receiver.read_all_messages();
            }
            return null;
        }

        public void clean() {
            if (receiver_initialized()) {
                m_receiver.clean();
                m_receiver = null;
            }
            if (sender_initialized()) {
                m_sender.clean();
                m_sender = null;
            }
        }
    }
}