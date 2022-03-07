
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
using System.IO.Ports;
using System.IO;

namespace Ex {

    public class SerialPortReaderComponent : ExComponent {

        private SerialPort m_port = null;


        #region ex_functions
        protected override bool initialize() {

            // signals
            add_signal("integer message");
            add_signal("string message");

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
                return false;
            }

            m_port.DataReceived += new SerialDataReceivedEventHandler(data_received);

            return true;
        }

        protected override void clean() {
            m_port.Close();
        }

        #endregion
        #region private_functions

        private void data_received(object sender, SerialDataReceivedEventArgs e) {

            int nbBytes = m_port.BytesToRead;
            byte[] data = new byte[nbBytes];
            m_port.Read(data, 0, data.Length);

            if (is_updating()) {
                if (currentC.get<bool>("int_mode")) {
                    invoke_signal("integer message", BitConverter.ToInt32(data, 0));
                } else if (currentC.get<bool>("string_mode")) {
                    invoke_signal("string message", BitConverter.ToString(data, 0));
                }
            }
        }

        #endregion
    }
}
