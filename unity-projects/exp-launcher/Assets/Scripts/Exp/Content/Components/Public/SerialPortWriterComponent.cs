
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
using System.Text.RegularExpressions;
using System.Collections;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Threading;

// unity
using UnityEngine;
using UnityEngine.Profiling;

namespace Ex{

    public class SerialPortWriterJob : ThreadedJob {

        // serial port
        private bool m_portOpened = false;
        private SerialPort m_port = null;

        // concurrency
        private ConcurrentQueue<Tuple<bool, object>> m_messages = new ConcurrentQueue<Tuple<bool, object>>();
        public ConcurrentQueue<Tuple<double, double, string>> triggersSent = new ConcurrentQueue<Tuple<double, double, string>>();

        public volatile bool doLoop = false;
        static private volatile int m_counter = 0;        

        public bool initialize(string portToWrite, int baudRate) {

            m_portOpened = false;
            try {
                m_port = new SerialPort(portToWrite, baudRate, Parity.None, 8, StopBits.None);
                m_port.Handshake = Handshake.None;
                m_port.Open();
            } catch (UnauthorizedAccessException e) {
                ExVR.Log().warning(string.Format("Serial port open UnauthorizedAccessException::error: [{0}]", e.Message));
            } catch (IOException e) {
                ExVR.Log().warning(string.Format("Serial port open IOException::error: [{0}]", e.Message));
            } finally {
                m_portOpened = m_port.IsOpen;
            }

            if (m_portOpened) {
                doLoop = true;
                start();
            } else {
                m_port = null;
            }

            return m_portOpened;
        }

        public void send_bytes(byte[] bytes) {
            if (m_portOpened) {
                m_messages.Enqueue(new Tuple<bool, object>(true, bytes));
            }
        }

        public void send_texte(string text) {
            if (m_portOpened) {
                m_messages.Enqueue(new Tuple<bool, object>(false, text));
            }
        }

        public void close() {
            if (m_port != null) {
                if (m_port.IsOpen) {
                    m_port.Close();
                }
            }
        }        

        protected override void thread_function() {

            int id = m_counter++;
            Thread.CurrentThread.Name = string.Concat("SerialPortWriterJob ", id);
            Profiler.BeginThreadProfiling("SerialPortWriterJob", Thread.CurrentThread.Name);

            while (doLoop) {

                Tuple<bool, object> messageToSend;                
                while (m_messages.TryDequeue(out messageToSend)) {

                    if (messageToSend.Item1) { // send bytes
                        
                        var buffer = (byte[])messageToSend.Item2;
                        try {                  
                            m_port.Write(buffer, 0, buffer.Length);
                            var expTime = ExVR.Time().ellapsed_exp_ms();
                            var routineTime = ExVR.Time().ellapsed_element_ms();      
                            triggersSent.Enqueue(new Tuple<double, double, string>(expTime, routineTime,                                 
                                string.Format("send_bytes [{0}]", BitConverter.ToString(buffer))));                            
                        } catch (ArgumentNullException e) {
                            ExVR.Log().error(string.Format("Write bytes ArgumentNullException::error: [{0}]", e.Message));
                        } catch (InvalidOperationException e) {
                            ExVR.Log().error(string.Format("Write bytes InvalidOperationException::error: [{0}]", e.Message));
                        } catch (ArgumentOutOfRangeException e) {
                            ExVR.Log().error(string.Format("Write bytes ArgumentOutOfRangeException::error: [{0}]", e.Message));
                        } catch (TimeoutException e) {
                            ExVR.Log().error(string.Format("Write bytes TimeoutException::error: [{0}]", e.Message));
                        }

                    } else { // send text

                        var message = (string)messageToSend.Item2;
                        try {
                            m_port.Write(message);
                            var expTime = ExVR.Time().ellapsed_exp_ms();
                            var routineTime = ExVR.Time().ellapsed_element_ms();
                            triggersSent.Enqueue(new Tuple<double, double, string>(expTime, routineTime, string.Format("send_text [{0}]", message)));
                        } catch (ArgumentNullException e) {
                            ExVR.Log().error(string.Format("Write string ArgumentNullException::error: [{0}]", e.Message));
                        } catch (InvalidOperationException e) {
                            ExVR.Log().error(string.Format("Write string InvalidOperationException::error: [{0}]", e.Message));
                        } catch (TimeoutException e) {
                            ExVR.Log().error(string.Format("Write string TimeoutException::error: [{0}]", e.Message));
                        }
                    }        
                }
                Thread.Sleep(1);
            }

            Profiler.EndThreadProfiling();
        }
    }

    public class SerialPortWriterComponent : ExComponent{
        
        private string m_message = "";
        private List<byte> m_messageBytes = null;
        private SerialPortWriterJob m_serialWriterT = null;
        static private readonly string m_messageSentSignalStr = "message sent";

        private List<Tuple<double, double, string>> m_triggerEvents = null;

        #region ex_functions
        protected override bool initialize() {

            add_slot("send byte pulse", (value) => {
                send_byte_pulse((int)value);
            });
            add_slot("write byte", (value) => {                
                write_byte(Converter.to_byte(value));
            });
            add_slot("write message", (value) => {
                write_str((string)value);
            });
            add_slot("write line message", (value) => {
                write_line_str((string)value);
            });
            add_signal(m_messageSentSignalStr);

            m_serialWriterT = new SerialPortWriterJob();
            if (!m_serialWriterT.initialize(initC.get<string>("port_to_write"), initC.get<int>("baud_rate"))) {
                m_serialWriterT = null;
                log_warning(string.Format("Serial port [{0}] cannot be opened, no data will be sended.", initC.get<string>("port_to_write")));
            }

            return true;
        }


        protected override void start_routine() {

            m_triggerEvents = null;
            if (currentC.get<bool>("send_new_routine")){
                write();
            }
        }

        protected override void set_update_state(bool doUpdate) {


            if (doUpdate && currentC.get<bool>("send_new_update_block")) {
                write();
            }else if (!doUpdate && currentC.get<bool>("send_end_update_block")) {
                write();
            }            
        }

        protected override void update() {

            if (m_serialWriterT == null) {
                return;
            }

            if (currentC.get<bool>("send_every_frame")) {
                write();
            }

            Tuple<double,double,string> triggerSent;
            while(m_serialWriterT.triggersSent.TryDequeue(out triggerSent)) {
                if(m_triggerEvents == null) {
                    m_triggerEvents = new List<Tuple<double, double, string>>();
                }
                m_triggerEvents.Add(triggerSent);
            }

            if (m_triggerEvents != null) {
                foreach (var time in m_triggerEvents) {
                    invoke_signal(m_messageSentSignalStr, new TimeAny(time.Item1, time.Item2, time.Item3));
                }
            }            
        }

        public override void update_from_current_config() {

            m_message = currentC.get<string>("message");
            m_messageBytes = new List<byte>();

            if (currentC.get<bool>("bits_mode") || currentC.get<bool>("bits_pulse_mode")) {
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
            } else if (currentC.get<bool>("int_mode") || currentC.get<bool>("int_pulse_mode")) {
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

        public override List<Tuple<double, double, string>> format_trigger_data_for_global_logger() {
            var tEvents = m_triggerEvents;
            m_triggerEvents = null;
            return tEvents;
        }

        protected override void clean() {
            
            if(m_serialWriterT != null) {
                m_serialWriterT.doLoop = false;
                m_serialWriterT.join(100);
                m_serialWriterT.close();
                m_serialWriterT = null;
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
            } else if (currentC.get<bool>("int_mode")) {
                write_bytes(m_messageBytes.ToArray());
            } else if (currentC.get<bool>("string_mode")) {
                write_line_str(m_message);
            } else if (currentC.get<bool>("bits_pulse_mode")) {
                send_bytes_pulse(m_messageBytes.ToArray());
            } else if (currentC.get<bool>("int_pulse_mode")) {
                send_bytes_pulse(m_messageBytes.ToArray());
            }
        }

        private IEnumerator send_pulse(byte value) {
            write_byte(value);
            yield return new WaitForSeconds((float)currentC.get<double>("pulse_time"));
            write_byte(0);
        }
        private IEnumerator send_pulse(byte[] value) {
            write_bytes(value);
            yield return new WaitForSeconds((float)currentC.get<double>("pulse_time"));
            write_byte(0);
        }

        #endregion
        #region public_functions

        public void send_byte_pulse(int value) {

            if (!currentC) {
                return;
            }

            if (value < 0) {
                value = 0;
            }
            if (value > 255) {
                value = 255;
            }
            ExVR.Coroutines().start(send_pulse((byte)value));
        }

        public void send_byte_pulse(byte value) {
            if (!currentC) {
                return;
            }
            ExVR.Coroutines().start(send_pulse(value));
        }

        public void send_bytes_pulse(byte[] value) {
            if (!currentC) {
                return;
            }
            ExVR.Coroutines().start(send_pulse(value));
        }

        public void write_byte(byte value) {

            if (!currentC) {
                return;
            }

            if (m_serialWriterT != null) {
                m_serialWriterT.send_bytes(new byte[] { value });
            }
        }

        public void write_bytes(byte[] values) {

            if (!currentC) {
                return;
            }

            if (m_serialWriterT != null) {
                m_serialWriterT.send_bytes(values);
            }
        }

        public void write_str(string text) {

            if (!currentC) {
                return;
            }

            if (m_serialWriterT != null) {
                m_serialWriterT.send_texte(text);
            }
        }

        public void write_line_str(string text) {

            if (!currentC) {
                return;
            }

            if (m_serialWriterT != null) {
                m_serialWriterT.send_texte(string.Concat(text, '\n'));
            }
        }

        #endregion
    }
}

// maybe see http://www.sparxeng.com/blog/software/must-use-net-system-io-ports-serialport implementation