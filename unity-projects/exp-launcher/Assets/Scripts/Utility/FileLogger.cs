
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
using System.Collections.Generic;
using System.IO;
using System.Threading;

// unity
using UnityEngine.Profiling;

namespace Ex {

    public class WritingFileThread : ThreadedJob {
        
        volatile public bool doLoop = false;
        string filePath = "";

        private System.Collections.Concurrent.ConcurrentQueue<List<string>> m_textesList = new System.Collections.Concurrent.ConcurrentQueue<List<string>>();
        private System.Threading.ReaderWriterLock m_locker = new System.Threading.ReaderWriterLock();

        public bool is_writing() {
            return !m_textesList.IsEmpty;
        }

        public bool set_file_path(string filePath) {
            try {
                m_locker.AcquireReaderLock(1000);
                this.filePath = filePath;
                m_locker.ReleaseReaderLock();
            } catch (ApplicationException e) {
                ExVR.Log().error(string.Format("Cannot get locker for setting file path [{0}], error: [{1}].", filePath, e.Message));
                return false;
            }
            return true;
        }

        public void add(string text) {
            m_textesList.Enqueue(new List<string>(1) { text });
        }

        public void add(List<string> textes) {
            m_textesList.Enqueue(textes);
        }

        private List<string> dequeue_textes() {

            if(m_textesList.Count == 0) {
                return null;
            }

            List<string> allTextes = null;
            List<string> textes;
            while (m_textesList.TryDequeue(out textes)) {
                if (allTextes == null) {
                    allTextes = new List<string>();
                }
                allTextes.Add(Text.join(textes));
            }
            return allTextes;
        }

        protected override void thread_function() {

            Profiler.BeginThreadProfiling("WritingFileThread", System.Threading.Thread.CurrentThread.Name);

            string n = System.Threading.Thread.CurrentThread.Name + "_thread_function";

            while (doLoop) {

                Profiler.BeginSample(n);

                List<string> allTextes = dequeue_textes();
                if (allTextes != null) {
                    write_to_file(allTextes);
                }
                System.Threading.Thread.Sleep(2);

                Profiler.EndSample();
            }

            List<string> lastAllTextes = dequeue_textes();
            if (lastAllTextes != null) {
                write_to_file(lastAllTextes);
            }

            Profiler.EndThreadProfiling();
        }

        private void write_to_file(List<string> textes) {
            
            int length = textes.Count;
            var textToWrite = Text.join(textes);
            try {                
                m_locker.AcquireWriterLock(1000);
                try {
                    if (filePath.Length > 0) {
                        File.AppendAllText(filePath, textToWrite);
                    }
                } catch (Exception exception) {
                    ExVR.Log().error(string.Format("Cannot write text [{0}] on logger file [{1}], get error [{2}]", textToWrite, filePath, exception.Message));
                } finally {
                    m_locker.ReleaseWriterLock();
                }
            } catch (ApplicationException e) {
                ExVR.Log().error(string.Format("Cannot get locker for writing to file, error: {0}. {1} lost.", e.Message, length));
                return;
            }
        }
    }

    public class FileLogger {

        private string m_fileFullPath;
        private WritingFileThread m_writingJob = null;
        private bool m_canWrite = false;
        private volatile static int counter = 0;

        ~FileLogger() {
            stop_logging();
        }

        public void start_logging(string threadName = "") {
            m_writingJob = new WritingFileThread();
            m_writingJob.doLoop = true;

            int id = counter++;
            if (threadName.Length == 0) {
                threadName = string.Format("WritingFileThread_[{0}]", id);
            } else {
                threadName = string.Format("{0}_[{1}]", threadName, id);
            }            
            m_writingJob.start(System.Threading.ThreadPriority.Normal, threadName);
        }

        public bool set_file_path(string filePath) {

            if (m_writingJob == null) {
                ExVR.Log().error("Writing thread not started.");
                return false;
            }

            m_fileFullPath = filePath;
            if (!m_writingJob.set_file_path(m_fileFullPath)) {
                ExVR.Log().error(string.Format("Cannot set logger file path [{0}].", m_fileFullPath));
                m_canWrite = false;
                return false;
            } else {
                m_canWrite = true;
                return true;
            }
        }

        public void stop_logging() {

            if (m_writingJob == null) {
                return;
            }

            m_canWrite = false;
            m_writingJob.doLoop = false;
            if (!m_writingJob.join(100)) {
                ExVR.Log().error(string.Format("Stop writing thread timeout."));
            }
            m_writingJob = null;
        }
        public static bool create_file(string fileFullPath, string initLine, bool dontWriteIfExists, bool addToEndIfExists) {

            // check file existence
            bool exists = File.Exists(fileFullPath);
            if (exists && dontWriteIfExists) {
                ExVR.Log().error(string.Format("File {0} already exists, change writing settings or delete file.", fileFullPath));
                return false;
            }

            // create empty file if necessary
            if (!exists) { // doesn't exist
                try {
                    File.Create(fileFullPath).Dispose();
                } catch (Exception ex) {
                    ExVR.Log().error(string.Format("Cannot create file {0}, error {1}", fileFullPath, ex.Message));
                    return false;
                }

                if (initLine.Length > 0) {
                    try {
                        File.AppendAllText(fileFullPath, initLine, System.Text.Encoding.UTF8);
                    } catch (Exception ex) {
                        ExVR.Log().error(string.Format("Cannot add to text to file {0}, error {1}", fileFullPath, ex.Message));
                        return false;
                    }
                }

            } else if (addToEndIfExists) {  // exists, append

                if (initLine.Length > 0) {
                    try {
                        File.AppendAllText(fileFullPath, initLine, System.Text.Encoding.UTF8);
                    } catch (Exception ex) {
                        ExVR.Log().error(string.Format("Cannot add to text to file {0}, error {1}", fileFullPath, ex.Message));
                        return false;
                    }
                }

            } else { // exists, don't append
                try {
                    File.WriteAllText(fileFullPath, initLine, System.Text.Encoding.UTF8);
                } catch (Exception ex) {
                    ExVR.Log().error(string.Format("Cannot reset file {0}, error {1}", fileFullPath, ex.Message));
                    return false;
                }
            }

            return true;
        }

        public void write(List<string> values) {

            if (!m_canWrite || values == null) {
                return;
            }

            if (values.Count > 0) {
                m_writingJob.add(Text.join(values));
            }
        }

        public void write_lines(List<string> values) {

            if (!m_canWrite || values == null) {
                return;
            }

            if (values.Count > 0) {
                m_writingJob.add(Text.join_with_new_line(values));
            }
        }

        public void write(object value, bool line) {

            if (!m_canWrite || value == null) {
                return;
            }

            var text = Converter.to_string(value);
            if (text.Length > 0) {
                if (line) {
                    m_writingJob.add(string.Format("{0}\n", text));
                } else {
                    m_writingJob.add(text);
                }
            }
        }

        public bool is_writing() {
            return m_writingJob.is_writing();
        }

        public string file_path() {
            return m_fileFullPath;
        }
    }
}