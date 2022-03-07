
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
using System.Text.RegularExpressions;
using System.Collections;
using System.Collections.Generic;
using System.IO.Ports;
using System.IO;

// unity
using UnityEngine;

namespace Ex{

    public class SerialPortWriterComponent : ExComponent{

        private SerialPort m_port = null;
        private string m_message = "";
        private List<byte> m_messageBytes = null;
      

        #region ex_functions
        protected override bool initialize() {

            add_slot("send byte pulse", (value) => {
                send_byte_pulse((int)value);
            });
            add_slot("write byte", (value) => {
                write_byte((byte)value);
            });
            add_slot("write message", (value) => {
                write_str((string)value);
            });
            add_slot("write line message", (value) => {
                write_line_str((string)value);
            });

            m_port = new SerialPort(initC.get<string>("port_to_write"), initC.get<int>("baud_rate"), Parity.None, 8, StopBits.None);
            m_port.Handshake = Handshake.None;
            try {
                m_port.Open();
            } catch (UnauthorizedAccessException e) {
                log_warning("Serial port open UnauthorizedAccessException::error: " + e.Message);
            } catch (IOException e) {
                log_warning("Serial port open IOException::error: " + e.Message);
            }

            if (!m_port.IsOpen) {
                log_warning(string.Format("Serial port {0} cannot be opened, no data will be sended.", initC.get<string>("port_to_write")));
            }
            return true;
        }


        protected override void start_routine() {

            if (currentC.get<bool>("send_new_routine")){
                write();
            }
        }

        protected override void set_update_state(bool doUpdate) {
            if (doUpdate && currentC.get<bool>("send_new_update_block")) {
                write();
            }else if (!doUpdate && currentC.get<bool>("send_end_update_block")){
                write();
            }            
        }

        protected override void update() {
            if (currentC.get<bool>("send_every_frame")) {
                write();
            }
        }

        public override void update_from_current_config() {

            m_message = currentC.get<string>("message");
            m_messageBytes = new List<byte>();

            if (currentC.get<bool>("bits_mode")) {
                string m = m_message.Replace('\t', ' ');
                m = m.Replace('\n', ' ');
                var splits = m.Split(' ');
                foreach (string split in splits) {

                    string r = Regex.Replace(split, "[^0-1]", "");
                    if (r.Length != 8) {
                        continue;
                    }

                    List<bool> newByte = new List<bool>(8);
                    foreach (char c in r) {
                        if (c != '0' && c != '1') {
                            break;
                        }
                        newByte.Add(c == '0' ? false : true);
                    }
                    if (newByte.Count == 8) {
                        // valid byte
                        m_messageBytes.Add(convert_bool_array_to_byte(newByte.ToArray()));
                    }
                }
            } else if (currentC.get<bool>("int_mode")) {
                string m = m_message.Replace('\t', ' ');
                m = m.Replace('\n', ' ');
                var splits = m.Split(' ');
                foreach (string split in splits) {
                    int b = Int32.Parse(split);
                    if (b < 0) {
                        b = 0;
                    } else if (b > 255) {
                        b = 255;
                    }
                    m_messageBytes.Add((byte)b);
                }
            }
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        protected override void clean() {
            if (m_port.IsOpen) {
                m_port.Close();
            }
        }

        #endregion
        #region private_functions

        private static byte convert_bool_array_to_byte(bool[] source) {

            byte result = 0;
            // This assumes the array never contains more than 8 elements!
            int index = 8 - source.Length;

            // Loop through the array
            foreach (bool b in source) {
                // if the element is 'true' set the bit at that position
                if (b) {
                    result |= (byte)(1 << (7 - index));
                }
                index++;
            }

            return result;
        }


        private void write() {

            if (currentC.get<bool>("bits_mode")) {
                write_bytes(m_messageBytes.ToArray());
            } else if (currentC.get<bool>("int_mode")){
                write_bytes(m_messageBytes.ToArray());
            } else {
                write_line_str(m_message);
            }
        }

        private void write_bytes(byte[] buffer, int offset, int count) {

            if (!m_port.IsOpen) {
                log_warning("Cannot write, port not opened.");
                return;
            }

            try {
                m_port.Write(buffer, offset, count);
            } catch (ArgumentNullException e) {
                log_error(string.Format("Write bytes ArgumentNullException::error: {0}", e.Message));
            } catch (InvalidOperationException e) {
                log_error(string.Format("Write bytes InvalidOperationException::error: {0}", e.Message));
            } catch (ArgumentOutOfRangeException e) {
                log_error(string.Format("Write bytes ArgumentOutOfRangeException::error: {0}", e.Message));
            } catch (TimeoutException e) {
                log_error(string.Format("Write bytes TimeoutException::error: {0}", e.Message));
            }
        }

        private void write_str(string text, bool line) {

            if (!m_port.IsOpen) {
                log_warning("Cannot write, port not opened.");
                return;
            }

            try {
                if (line) {
                    m_port.WriteLine(text);
                } else {
                    m_port.Write(text);
                }
            } catch (ArgumentNullException e) {
                log_error(string.Format("Write string ArgumentNullException::error: {0}", e.Message));
            } catch (InvalidOperationException e) {
                log_error(string.Format("Write string InvalidOperationException::error: {0}", e.Message));
            } catch (TimeoutException e) {
                log_error(string.Format("Write string TimeoutException::error: {0}", e.Message));
            }
        }

        private IEnumerator send_pulse(byte value) {
            write_byte(value);
            yield return new WaitForSeconds((float)currentC.get<double>("pulse_time"));
            write_byte(0);
        }

        #endregion
        #region public_functions

        public void send_byte_pulse(int value) {
            if (value < 0) {
                value = 0;
            }
            if (value > 255) {
                value = 255;
            }
            send_byte_pulse((byte)value);
        }

        public void send_byte_pulse(byte value) {
            ExVR.Coroutines().start(send_pulse(value));
        }

        public void write_byte(byte value) {
            write_bytes(new byte[] {value}, 0, 1);
        }

        public void write_bytes(byte[] values) {
            write_bytes(values, 0, values.Length);
        }

        public void write_str(string text) {
            write_str(text, false);
        }

        public void write_line_str(string text) {
            write_str(text, true);
        }

        #endregion
    }
}

// maybe see http://www.sparxeng.com/blog/software/must-use-net-system-io-ports-serialport implementation