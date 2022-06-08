
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
using System.Diagnostics;

namespace Ex.Events{

    public class Signal{
        
        private bool m_connected = false;
        private string m_name;
       
        public event EventHandler<object> ev = delegate { };
        public void invoke(object arg = null) {
            ev?.Invoke(this, arg);
        }

        public void reset() {
            ev = delegate { };
            m_connected = false;
        }

        public Signal(string name) {
            m_name = name;
        }

        public string name() {
            return m_name;
        }

        public bool is_connected() {
            return m_connected;
        }

        public void remove_listeners() {
            reset();
        }

        public void set_connected() {
            m_connected = true;
        }

        //private bool check_infinity_loop() {

        //    if (m_infiniteLoop) {
        //        return true;
        //    }

        //    if (!m_swInfinityLoop.IsRunning) {
        //        m_swInfinityLoop.Start();
        //    }

        //    if (m_swInfinityLoop.Elapsed.TotalMilliseconds < 1000.0 / 90.0) {
        //        m_invokeCount++;

        //        if (m_invokeCount > 2500) {
        //            m_infiniteLoop = true;
        //            m_swInfinityLoop.Stop();
        //            log_error("Signal/Slot infinity loop detected, connection canceled.");
        //            return true;
        //        }
        //    } else {
        //        m_swInfinityLoop.Restart();
        //        m_invokeCount = 0;
        //    }

        //    return false;
        //}
    }
}