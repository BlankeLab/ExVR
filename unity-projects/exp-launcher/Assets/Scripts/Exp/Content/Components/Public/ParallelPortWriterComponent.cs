
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
using System.Runtime.InteropServices;
using System.Collections;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class ParallelPortWriterComponent : ExComponent{

        [DllImport("inpoutx64.dll", EntryPoint = "IsInpOutDriverOpen")]
        private static extern UInt32 is_inpout_driver_opened_x64();

        [DllImport("inpoutx64.dll", EntryPoint = "Out32")]
        private static extern void out32_x64(short port, short Data);

        [DllImport("inpoutx64.dll", EntryPoint = "DlPortWritePortUshort")]
        private static extern void dl_port_write_port_ushort_x64(ushort port, ushort Data);

        [DllImport("inpoutx64.dll", EntryPoint = "DlPortWritePortUlong")]
        private static extern void dl_port_write_port_ulong_x64(uint port, uint Data);

        private bool m_int16Mode = false;
        private bool m_available = false;
        private static readonly string m_messageSentSignalStr = "message sent";
        private List<Tuple<double, double, string>> triggerEvents = null;

        #region ex_functions
        protected override bool initialize() {

            m_int16Mode = initC.get<bool>("int16_mode");

            add_slot("write", (value) => {
                write((int)value, currentC.get<string>("port"));
            });
            add_slot("send pulse", (value) => {
                send_pulse((int)value, (float)currentC.get<double>("pulse_time"), currentC.get<string>("port"));
            });
            add_signal(m_messageSentSignalStr);

            try {
                m_available = is_inpout_driver_opened_x64() != 0;
                if (!m_available) {
                    log_warning("Inpout driver not opened. No signal can be sent.", false);
                }
            } catch (DllNotFoundException) {
                log_warning("Inpoutx64.dll not found. No signal can be sent.", false);
            }

            return true;
        }

        protected override void start_routine() {
            triggerEvents = null;
        }

        public override List<Tuple<double, double, string>> format_trigger_data_for_global_logger() {
            var tEvents = triggerEvents;
            triggerEvents = null;
            return tEvents;              
        }

        #endregion

        #region private_functions

        private IEnumerator reset_pulse(float pulseTime, string port) {
            yield return new WaitForSeconds(pulseTime);
            dll_write(0, port);
        }
        private void dll_write(int value, string port) {

            if (m_int16Mode) {
                dl_port_write_port_ushort_x64(Convert.ToUInt16(port, 16), (ushort)value);
            } else {
                dl_port_write_port_ulong_x64(Convert.ToUInt32(port, 16), (uint)value);
            }
        }

        #endregion

        #region public_functions

        public double send_pulse(int value, float pulseTime = -1f, string port = "") {

            if (!m_available) {
                return -1;
            }

            if (pulseTime < 0) {
                pulseTime = (float)currentC.get<double>("pulse_time");
            }

            if(port.Length == 0) {
                port = currentC.get<string>("port");
            }

            double expTime = time().ellapsed_exp_ms();
            double routineTime = time().ellapsed_element_ms();

            // send value
            dll_write(value, port);
            
            // schedule a reset value
            ExVR.Coroutines().start(reset_pulse(pulseTime, port));

            // send trigger time
            invoke_signal(m_messageSentSignalStr, new TimeAny(expTime, routineTime, value));

            // add trigger
            if (triggerEvents == null) {
                triggerEvents = new List<Tuple<double, double, string>>();
            }
            triggerEvents.Add(new Tuple<double, double, string>(expTime, routineTime, string.Format("write_pulse {0}", Converter.to_string(value))));

            return expTime;
        }

        public double write(int value, string port = "") {

            if (!m_available) {
                return -1;
            }

            if (port.Length == 0) {
                port = currentC.get<string>("port");
            }

            double expTime = time().ellapsed_exp_ms();
            double routineTime = time().ellapsed_element_ms();

            // send value
            dll_write(value, port);

            // send trigger time
            invoke_signal(m_messageSentSignalStr, new TimeAny(expTime, routineTime, value));

            // add trigger
            if(triggerEvents == null) {
                triggerEvents = new List<Tuple<double, double, string>>();
            }
            triggerEvents.Add(new Tuple<double, double, string>(expTime, routineTime, string.Format("write {0}", Converter.to_string(value))));

            return expTime;
        }

        #endregion
    }
}
