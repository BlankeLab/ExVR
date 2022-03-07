
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

        private bool m_x32Mode = false;
        private bool m_int16Mode = false;
        private bool m_available = false;

        private float m_pulseTime = 1f;
        private string m_port = "0x378";

        protected override bool initialize() {

            m_int16Mode = initC.get<bool>("int16_mode");

            add_slot("write", (value) => {
                write((int)value);
            });
            add_slot("send pulse", (value) => {
                ExVR.Coroutines().start(send_pulse((int)value));
            });

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
   
        public IEnumerator send_pulse(int value) {
            write(value);
            yield return new WaitForSeconds(m_pulseTime);
            write(0);
        }


        public override void update_from_current_config() {
            m_pulseTime = (float)currentC.get<double>("pulse_time");
            m_port      = currentC.get<string>("port");
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }


        private void write(int value, string port) {

            if (!m_available) {
                return;
            }
       
            if (m_int16Mode) {

                dl_port_write_port_ushort_x64(Convert.ToUInt16(m_port, 16), (ushort)value);

            } else {

                dl_port_write_port_ulong_x64(Convert.ToUInt32(m_port, 16), (uint)value);
            }

        }
         public void write(int value) {
            write(value, m_port);
        }
    }
}
