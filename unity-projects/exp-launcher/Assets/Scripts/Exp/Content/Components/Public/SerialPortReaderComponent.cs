
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
using System.IO;
using System.IO.Ports;
using System.Collections.Concurrent;
using System.Collections.Generic;

namespace Ex {

    public class SerialPortReaderComponent : ExComponent {

        private volatile bool m_receiveData = false;
        private SerialPort m_port = null;
        private ConcurrentQueue<Tuple<double,double, byte[]>> m_messagesReceived = null;
        private bool intMode = false;
        private bool stringMode = false;
        private static readonly string messageReadSignalStr      = "message read";

        private List<Tuple<double, double, string>> triggerEvents = null;

        #region ex_functions
        protected override bool initialize() {

            // signals
            add_signal(messageReadSignalStr);

            // init port
            m_port = new SerialPort(initC.get<string>("port_to_read"));
            m_port.Handshake = Handshake.None;
            try {
                m_port.Open();
            } catch (UnauthorizedAccessException e) {
                log_error("Serial port open UnauthorizedAccessException::error: " + e.Message);
            } catch (IOException e) {
                log_error("Serial port open IOException::error: " + e.Message);
            }

            if (!m_port.IsOpen) {
                log_error(string.Format("Serial port {0} cannot be opened", initC.get<string>("port_to_read")));
                m_port = null;
                return false;
            }

            m_messagesReceived = new ConcurrentQueue<Tuple<double, double, byte[]>>();
            m_port.DataReceived += new SerialDataReceivedEventHandler(data_received);

            return true;
        }

        protected override void start_routine() {
            triggerEvents = null;
        }

        protected override void set_update_state(bool doUpdate) {
            m_receiveData = true;
        }

        public override void update_from_current_config() {
            intMode    = currentC.get<bool>("int_mode");
            stringMode = currentC.get<bool>("string_mode");
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        protected override void update() {
            
            List<Tuple<double, double, byte[]>> messages = null;
            {
                Tuple<double, double, byte[]> message;
                while (m_messagesReceived.TryDequeue(out message)) {

                    if (triggerEvents == null) {
                        triggerEvents = new List<Tuple<double, double, string>>();
                    }
                    if (messages == null) {
                        messages = new List<Tuple<double, double, byte[]>>();
                    }
                    messages.Add(message);

                    if (intMode) {
                        triggerEvents.Add(new Tuple<double, double, string>(message.Item1, message.Item2, string.Format("read_int {0}",BitConverter.ToInt32(message.Item3, 0))));
                    } else {
                        triggerEvents.Add(new Tuple<double, double, string>(message.Item1, message.Item2, string.Format("read_text {0}", BitConverter.ToString(message.Item3, 0))));
                    }
                }
            }

            if (is_updating()) {
                if (messages != null) {
                    foreach (var message in messages) {
                        if (intMode) {
                            invoke_signal(messageReadSignalStr, new TimeAny(message.Item1, message.Item2, BitConverter.ToInt32(message.Item3, 0)));
                        } else if (stringMode) {
                            invoke_signal(messageReadSignalStr, new TimeAny(message.Item1, message.Item2, BitConverter.ToString(message.Item3, 0)));
                        }
                    }
                }
            }
        }

        protected override void clean() {
            if (m_port != null) {
                if (m_port.IsOpen) {
                    m_port.Close();
                }
            }
        }

        public override List<Tuple<double, double, string>> format_trigger_data_for_global_logger() {
            var tEvents = triggerEvents;
            triggerEvents = null;
            return tEvents;
        }

        #endregion
        #region private_functions

        private void data_received(object sender, SerialDataReceivedEventArgs e) {

            if (!m_receiveData) {
                return;
            }
            
            int nbBytes = m_port.BytesToRead;
            if (nbBytes > 0) {
                byte[] data = new byte[nbBytes];
                m_port.Read(data, 0, data.Length);
                double expTime = ExVR.Time().ellapsed_exp_ms();
                double routineTime = ExVR.Time().ellapsed_element_ms();
                m_messagesReceived.Enqueue(new Tuple<double, double, byte[]>(expTime, routineTime, data));
            }
        }

        #endregion
    }
}
