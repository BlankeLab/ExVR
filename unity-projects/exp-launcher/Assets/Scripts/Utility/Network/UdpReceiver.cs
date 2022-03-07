
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
using System.Threading;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Collections;
using System.Collections.Generic;

namespace Ex {

    public class UdpReceiver : ThreadedJob {

        public bool readingEnabled = false;
        public bool initialized = false;

        public int readingPort;
        public IPAddress readingAddress = null;
        private IPEndPoint clientIpEndPoint = null;

        private Socket clientSocket = null;
        private const int bufSize = 8 * 1024;
        private byte[] buffer = new byte[bufSize];

        public ReaderWriterLock rwl = new ReaderWriterLock();
        public Queue queue = new Queue();

        private int m_readSizeBuffer = -1;
        public void reset_buffer_size_to_read(int readSizeBuffer) {
            m_readSizeBuffer = readSizeBuffer;
            buffer = new byte[readSizeBuffer];
        }

        public bool initialize(int port, IPAddress ipAddress, int timeoutMs = 10) {

            readingPort = port;
            readingAddress = ipAddress;
            initialized = false;

            clientIpEndPoint = new IPEndPoint(ipAddress, readingPort);
            clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            clientSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
            clientSocket.Bind(clientIpEndPoint);
            clientSocket.ReceiveTimeout = timeoutMs;

            return initialized = true;
        }

        public void set_reading_state(bool readingState) {

            if (!initialized) {
                return;
            }

            try {
                rwl.AcquireWriterLock(1000);
                if (readingEnabled) {
                    if (readingState) {
                        // do nothing
                    } else {
                        stop(); // stop thread
                        readingEnabled = false;
                    }
                } else {
                    if (!readingState) {
                        // do nothing
                    } else {
                        readingEnabled = true;
                        start(); // start thread
                    }
                }
            } catch (ApplicationException e) {
                // timeout
                UnityEngine.Debug.LogError(string.Format("Set reading state locker error: {0}", e.Message));
            } finally {
                rwl.ReleaseWriterLock();
            }
        }

        public string read_message() {

            string message = "";
            if (clientSocket == null) {
                return message;
            }

            try {
                rwl.AcquireWriterLock(5);
                if (queue.Count > 0) {
                    message = (string)queue.Dequeue();
                }
            } catch (ApplicationException) {
                // timeout
            } finally {
                rwl.ReleaseWriterLock();
            }

            return message;
        }

        public List<string> read_all_messages() {

            if (clientSocket == null) {
                return new List<string>();
            }

            List<string> messages = null;
            try {
                rwl.AcquireWriterLock(5);

                var queueA = queue.ToArray();
                messages = new List<string>(queueA.Length);
                foreach (var message in queueA) {
                    messages.Add((string)message);
                }
                queue.Clear();

            } catch (ApplicationException) {
                // timeout
            } finally {
                rwl.ReleaseWriterLock();
            }

            if (messages == null) {
                return new List<string>();
            }

            return messages;
        }

        public void clean() {

            readingEnabled = false;
            stop();

            try { 
                rwl.AcquireWriterLock(300);

                if (clientSocket != null) {
                    clientSocket.Close();
                    clientSocket.Dispose();
                    clientSocket = null;
                    clientIpEndPoint = null;
                    initialized = false;
                    queue.Clear();
                }

            } catch (ApplicationException e) {
                // timeout
                UnityEngine.Debug.LogError(string.Format("Clean locker error: {0}", e.Message));
            } finally {
                rwl.ReleaseWriterLock();
            }
        }

        protected override void OnFinished() { }

        protected override void ThreadFunction() {

            byte[] endByte = Encoding.ASCII.GetBytes(new char[] { '\0' });
            while (readingEnabled) {

                // receive a message 
                bool messageReceived = false;
                int count = 0;
                try {                    
                    if (m_readSizeBuffer == -1) {
                        count = clientSocket.Receive(buffer);
                    } else {                        
                        count = clientSocket.Receive(buffer, 0, m_readSizeBuffer, SocketFlags.None);
                    }
                    messageReceived = true;
                } catch (SocketException e) {
                    if (e.SocketErrorCode != SocketError.TimedOut) {
                        UnityEngine.Debug.LogError(string.Format("Receive socket error: {0}", e.Message));
                    } else {
                        // timeout
                    }
                } catch (ArgumentNullException e) {
                    UnityEngine.Debug.LogError(string.Format("Receive argument null error: {0}", e.Message));
                } catch (ArgumentOutOfRangeException e) {
                    UnityEngine.Debug.LogError(string.Format("Receive argument out of range error: {0}", e.Message));
                } catch (ObjectDisposedException e) {
                    UnityEngine.Debug.LogError(string.Format("Receive object disposed error: {0}", e.Message));
                } catch (System.Security.SecurityException e) {
                    UnityEngine.Debug.LogError(string.Format("Receive security error: {0}", e.Message));
                } catch(Exception e) {
                    // TODO: replace abort thread
                    //UnityEngine.Debug.LogError(string.Format("Receive error: {0}", e.Message));
                }

                if (messageReceived) {

                    int endId = Array.IndexOf(buffer, endByte[0]);
                    if (endId < count) {
                        count = endId;
                    }

                    if (readingEnabled) {
                        try {
                            rwl.AcquireWriterLock(10);
                            queue.Enqueue(Encoding.UTF8.GetString(buffer, 0, count));
                        } catch (DecoderFallbackException e) {
                            UnityEngine.Debug.LogError(string.Format("GetString decoder error: {0}", e.Message));
                        } catch (ArgumentException e) {
                            UnityEngine.Debug.LogError(string.Format("GetString argument error: {0}", e.Message));
                        } catch (ApplicationException e) {
                            UnityEngine.Debug.LogError(string.Format("Writer lock error: {0}", e.Message));
                        } finally {
                            rwl.ReleaseWriterLock();
                        }
                    }
                } else {
                    Thread.Sleep(5);
                }
            }
        }
    }
}