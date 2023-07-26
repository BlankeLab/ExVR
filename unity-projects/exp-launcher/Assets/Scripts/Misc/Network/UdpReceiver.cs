
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
using System.Collections.Generic;
using System.Collections.Concurrent;
using System.Threading;

// unity
using UnityEngine.Profiling;

namespace Ex {

    public class UdpReceiver : ThreadedJob {
        
        public bool m_initialized = false;        

        // socket
        private int m_readingPort;
        private IPAddress m_readingAddress = null;
        private IPEndPoint m_clientIpEndPoint = null;
        private Socket m_clientSocket = null;

        // buffer
        private const int m_bufSize = 8 * 1024;
        private byte[] m_buffer = new byte[m_bufSize];
        private int m_readSizeBuffer = -1;

        // concurrency
        private ConcurrentQueue<Tuple<double,double, string>> m_messages = new ConcurrentQueue<Tuple<double, double, string>>();
        private volatile bool m_doLoop          = false;
        private volatile bool m_processMessages = false;
        static private volatile int m_counter = 0;

        public bool initialize(int port, IPAddress ipAddress, int timeoutMs = 10) {

            m_readingPort      = port;
            m_readingAddress   = ipAddress;
            m_initialized      = false;

            try {
                m_clientIpEndPoint = new IPEndPoint(ipAddress, m_readingPort);
                m_clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                m_clientSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
                m_clientSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReceiveTimeout, 100);
                m_clientSocket.Bind(m_clientIpEndPoint);
                m_clientSocket.ReceiveTimeout = timeoutMs;
            } catch (SocketException e) {
                UnityEngine.Debug.LogError(string.Format("Cannot initialize UDP receiver with adresse [{0}] and port [{1}], error message [{1}]",
                    ipAddress.ToString(),
                    m_readingPort.ToString(),
                    e.Message)
                );
            } catch (Exception e) {
                UnityEngine.Debug.LogError(string.Format("Cannot initialize UDP receiver with adresse [{0}] and port [{1}], error message [{1}]",
                    ipAddress.ToString(),
                    m_readingPort.ToString(),
                    e.Message)
                );
            } finally {
                m_initialized = true;
            }

            // start thread
            m_doLoop = true;
            start(); 

            return m_initialized;
        }
        public void reset_buffer_size_to_read(int readSizeBuffer) {
            m_readSizeBuffer = readSizeBuffer;
            m_buffer = new byte[readSizeBuffer];
        }
        public bool initialized() {
            return m_initialized;
        }

        public int reading_port() {
            return m_readingPort;
        }

        public IPAddress reading_address() {
            return m_readingAddress;
        }

        public Tuple<double, double, string> read_message() {

            Tuple<double, double, string> message;
            if (m_messages.TryDequeue(out message)) {
                return message;
            }
            return null;
        }

        public List<Tuple<double, double, string>> read_all_messages() {

            Profiler.BeginSample("[ExVR][UdpReceiver][read_all_messages]");
            List<Tuple<double, double, string>> messages = null;
            Tuple<double, double, string> message;
            int size = m_messages.Count;
            while (m_messages.TryDequeue(out message)) {
                if (messages == null) {
                    messages = new List<Tuple<double, double, string>>(size);
                }
                messages.Add(message);
            }
            Profiler.EndSample();
            return messages;
        }

        public void set_reading_state(bool readingState) {
            m_processMessages = readingState;
        }

        public void clean() {

            // stop thread 
            m_processMessages = false;
            m_doLoop          = false;
            if (!join(100)) {
                UnityEngine.Debug.LogError(string.Format("Stop UDP reading thread timeout."));
            }

            // clean socket
            if (m_clientSocket != null) {

                try {
                    m_clientSocket.Close();
                    m_clientSocket.Dispose();                    
                } catch (SocketException e) {
                    UnityEngine.Debug.LogError(string.Format("Clean socket error: [{0}]", e.Message));
                } catch (Exception e) {
                    UnityEngine.Debug.LogError(string.Format("Clean socket error: [{0}]", e.Message));
                }

                m_clientSocket = null;
            }

            m_clientIpEndPoint  = null;
            m_initialized       = false;
            m_messages = new ConcurrentQueue<Tuple<double, double, string>>();
        }


        protected override void thread_function() {

            if (!m_initialized) {
                return;
            }

            int id = m_counter++;
            Thread.CurrentThread.Name = string.Concat("receiver ", id);
            Profiler.BeginThreadProfiling("UdpReceiver", Thread.CurrentThread.Name);

            byte[] endByte = Encoding.ASCII.GetBytes(new char[] { '\0' });
            while (m_doLoop) {

                // receive a message                                  
                try {
                    int count = 0;
                    if (m_readSizeBuffer == -1) {
                        count = m_clientSocket.Receive(m_buffer);
                    } else {
                        count = m_clientSocket.Receive(m_buffer, 0, m_readSizeBuffer, SocketFlags.None);
                    }
                    double expTime = ExVR.Time().ellapsed_exp_ms();
                    double routineTime = ExVR.Time().ellapsed_element_ms();

                    if (m_processMessages) {
                        int endId = Array.IndexOf(m_buffer, endByte[0]);
                        if (endId < count) {
                            count = endId;
                        }

                        if (count > 0) {
                            try {
                                m_messages.Enqueue(new Tuple<double, double, string>(expTime, routineTime, Encoding.UTF8.GetString(m_buffer, 0, count)));
                            } catch (DecoderFallbackException e) {
                                UnityEngine.Debug.LogError(string.Format("GetString decoder error: {0}", e.Message));
                            } catch (ArgumentException e) {
                                UnityEngine.Debug.LogError(string.Format("GetString argument error: {0}", e.Message));
                            }
                        }
                    }

                } catch (SocketException e) {
                    if (e.SocketErrorCode != SocketError.TimedOut) {
                        UnityEngine.Debug.LogError(string.Format("Receive socket error: [{0}]", e.Message));
                    }
                } catch (ArgumentNullException e) {
                    UnityEngine.Debug.LogError(string.Format("Receive argument null error: [{0}]", e.Message));
                } catch (ArgumentOutOfRangeException e) {
                    UnityEngine.Debug.LogError(string.Format("Receive argument out of range error: [{0}]", e.Message));
                } catch (ObjectDisposedException e) {
                    UnityEngine.Debug.LogError(string.Format("Receive object disposed error: [{0}]", e.Message));
                } catch (System.Security.SecurityException e) {
                    UnityEngine.Debug.LogError(string.Format("Receive security error: [{0}]", e.Message));
                } catch (Exception e) {
                    // TODO: replace abort thread                    
                    UnityEngine.Debug.LogWarning(string.Format("Receive error: [{0}]", e.Message));
                }
            }

            Profiler.EndThreadProfiling();
        }
    }
}