
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
using System.Collections;

namespace Ex{

    public class ThreadedJob{

        private bool m_IsDone = false;
        private object m_Handle = new object();
        private System.Threading.Thread m_Thread = null;

        public bool is_done {
            get {
                bool tmp;
                lock (m_Handle) {
                    tmp = m_IsDone;
                }
                return tmp;
            }
            set {
                lock (m_Handle) {
                    m_IsDone = value;
                }
            }
        }

        public void set_priority(System.Threading.ThreadPriority p) {
            if (m_Thread != null) {
                m_Thread.Priority = p;
            }
        }

        public void start() {
            m_Thread = new System.Threading.Thread(run);
            m_Thread.Start();
        }

        public void stop() {
            if (m_Thread != null) {
                m_Thread.Abort();
                m_Thread.Join();
            }
        }

        protected virtual void ThreadFunction() { }

        protected virtual void OnFinished() { }

        public virtual bool update() {
            if (is_done) {
                OnFinished();
                return true;
            }
            return false;
        }

        IEnumerator wait_for() {
            while (!update()) {
                yield return null;
            }
        }

        private void run() {
            ThreadFunction();
            is_done = true;
        }
    }
}