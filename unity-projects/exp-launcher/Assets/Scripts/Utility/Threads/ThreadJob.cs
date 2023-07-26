
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
using System.Threading;
using System.Diagnostics;

namespace Ex{

    public class ThreadedJob{

        public volatile bool isDone = false;
        private Thread m_Thread = null;
        private string m_name = "";

        public void start(ThreadPriority priority = ThreadPriority.Normal, string name = "") {
            isDone   = false;
            m_name = name;
            m_Thread = new System.Threading.Thread(run);
            m_Thread.Priority = priority;
            m_Thread.Start();
        }

        private void run() {
            if (m_name.Length > 0) {
                Thread.CurrentThread.Name = m_name;
            }
            thread_function();
            on_finished();
            isDone = true;
        }

        public void stop() {
            if (m_Thread != null) {
                m_Thread.Abort();
                m_Thread.Join();
                m_Thread = null;
            }
        }

        public bool join(int timeoutMs) {

            if(m_Thread == null) {
                return true;
            }

            bool timeout = false;
            Stopwatch sw = new Stopwatch();
            sw.Start();
            while (!isDone) {
                if (sw.ElapsedMilliseconds > timeoutMs) {
                    m_Thread.Abort();                    
                    m_Thread.Join();                    
                    isDone  = true;
                    timeout = true;
                    break;
                }
                Thread.Sleep(1);
            }
            m_Thread.Join();
            m_Thread = null;

            return !timeout;
        }

        // to be overrided
        protected virtual void thread_function() { }
        protected virtual void on_finished() { }

    }
}