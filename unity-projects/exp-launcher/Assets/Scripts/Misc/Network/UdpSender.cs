
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
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Collections.Concurrent;
using System.Threading;

// unity
using UnityEngine.Profiling;

namespace Ex {

    public class UdpSender : ThreadedJob {

        public bool m_initialized = false;

        // socket
        public int m_writingPort;
        private IPAddress m_writingAddress = null;
        private IPEndPoint m_endPoint = null;
        private UdpClient m_sender = null;

        // concurrency
        private ConcurrentQueue<byte[]> m_messages = new ConcurrentQueue<byte[]>();
        public ConcurrentQueue<TimeAny> triggersSent = new ConcurrentQueue<TimeAny>();
        private volatile bool m_doLoop = false;
        static private volatile int m_counter = 0;

        public int writing_port() {
            return m_writingPort;
        }

        public IPAddress writing_address() {
            return m_writingAddress;                
        }

        public bool initialize(int port, IPAddress ipAddress, bool ipv6 = false) {

            m_initialized    = false;
            m_writingPort    = port;            
            m_writingAddress = ipAddress;

            try {
                m_endPoint = new IPEndPoint(ipAddress, port);
                m_sender = new UdpClient(ipv6 ? AddressFamily.InterNetworkV6 : AddressFamily.InterNetwork);
                m_sender.Client.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.SendBuffer, 10000);
            } catch(SocketException e) {
                UnityEngine.Debug.LogError(string.Format("Cannot initialize UDP sender with adresse [{0}] and port [{1}], error message [{1}]",
                    ipAddress.ToString(),
                    m_writingPort.ToString(),
                    e.Message)
                );
            } catch(Exception e) {
                UnityEngine.Debug.LogError(string.Format("Cannot initialize UDP sender with adresse [{0}] and port [{1}], error message [{1}]",
                    ipAddress.ToString(),
                    m_writingPort.ToString(),
                    e.Message)
                );
            } finally {
                m_initialized = true;
            }

            if (m_initialized) {
                // start thread
                m_doLoop = true;
                start();
            }

            return m_initialized;
        }

        public void clean() {

            // stop thread 
            m_doLoop = false;
            if (!join(100)) {
                UnityEngine.Debug.LogError(string.Format("Stop UDP writing thread timeout."));
            }

            if (m_sender != null) {

                try {
                    m_sender.Close();
                    m_sender.Dispose();
                } catch (SocketException e) {
                    UnityEngine.Debug.LogError(string.Format("Clean sender socket error: [{0}]", e.Message));
                } catch (Exception e) {
                    UnityEngine.Debug.LogError(string.Format("Clean sender error: [{0}]", e.Message));
                }

                m_sender = null;
            }

            m_endPoint      = null;
            m_initialized   = false;
            m_messages      = new ConcurrentQueue<byte[]>();
        }


        protected override void thread_function() {

            if (!m_initialized) {
                return;
            }

            int id = m_counter++;
            Thread.CurrentThread.Name = string.Concat("UdpSender ", id);
            Profiler.BeginThreadProfiling("UdpSender", Thread.CurrentThread.Name);

            while (m_doLoop) {

                byte[] bytesToSend;
                while(m_messages.TryDequeue(out bytesToSend)) {

                    var expTime     = ExVR.Time().ellapsed_exp_ms();
                    var routineTime = ExVR.Time().ellapsed_element_ms();
                    int nbBytesSent = 0;
                    try {
                        nbBytesSent = m_sender.Send(bytesToSend, bytesToSend.Length, m_endPoint);
                    } catch (SocketException e) {
                        UnityEngine.Debug.LogError(string.Format("Send socket error: [{0}] for message of size [{1}]", e.Message, bytesToSend.Length));
                    } catch(Exception e) {
                        UnityEngine.Debug.LogError(string.Format("Send error: [{0}] for message of size [{1}]", e.Message, bytesToSend.Length));
                    }
                    if (triggersSent.Count < 1000) {
                        triggersSent.Enqueue(new TimeAny(expTime, routineTime, nbBytesSent));
                    }
                }
                Thread.Sleep(1);
            }

            Profiler.EndThreadProfiling();
        }

        public void send_message(string message) {
            send_bytes(Encoding.UTF8.GetBytes(message));
        }

        public void send_bytes(byte[] bytesToSend) {
            if (m_initialized) {
                m_messages.Enqueue(bytesToSend);                
            }
        }
    }
}