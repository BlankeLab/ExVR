
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

// unity
using UnityEngine.Profiling;

namespace Ex {

    public class WritingFileThread : ThreadedJob {

        private volatile static int counter = 0;
        volatile public bool doLoop = false;

        private StreamWriter m_steamWriter = null;
        private System.Collections.Concurrent.ConcurrentQueue<Tuple<List<string>, bool>> m_linesList = new System.Collections.Concurrent.ConcurrentQueue<Tuple<List<string>, bool>>();
        private System.Threading.ReaderWriterLock m_locker = new System.Threading.ReaderWriterLock();

        public bool is_writing() {
            return !m_linesList.IsEmpty;
        }

        public bool open_file(string filePath) {

            bool success = false;
            try {
                m_locker.AcquireWriterLock(1000);
                try {

                    if (m_steamWriter != null) {
                        m_steamWriter.Close();
                    }
                    m_steamWriter = new StreamWriter(filePath, true, System.Text.Encoding.UTF8);
                    success = m_steamWriter.BaseStream != null;

                } finally {
                    m_locker.ReleaseWriterLock();
                }
            } catch (ApplicationException e) {
                ExVR.Log().error(string.Format("Cannot get locker for opening file with path [{0}], error: [{1}].", filePath, e.Message));
                return false;
            }

            return success;
        }

        public void add_text(string text) {
            m_linesList.Enqueue(new Tuple<List<string>, bool>(new List<string>(1) { text }, false));
        }

        public void add_line(string line) {
            m_linesList.Enqueue(new Tuple<List<string>, bool>(new List<string>(1) { line }, true));
        }

        public void add_lines(List<string> lines) {
            m_linesList.Enqueue(new Tuple<List<string>, bool>(lines, true));
        }
        protected override void thread_function() {

            int id = counter++;
            System.Threading.Thread.CurrentThread.Name = string.Concat("WritingFileThread ", id);
            Profiler.BeginThreadProfiling("WritingFileThread", System.Threading.Thread.CurrentThread.Name);

            while (doLoop) {

                Tuple<List<string>, bool> lines;
                if(m_linesList.TryDequeue(out lines)) {
                    write_to_file(lines);                    
                }
                System.Threading.Thread.Sleep(1);                
            }

            Tuple<List<string>, bool> lastLines;
            while (m_linesList.TryDequeue(out lastLines)) {                
                write_to_file(lastLines);
            }

            if (m_steamWriter != null) {
                m_steamWriter.Close();
            }
            --counter;

            Profiler.EndThreadProfiling();
        }

        private void write_to_file(Tuple<List<string>, bool> lines) {            

            int length = lines.Item1.Count;
            try {                
                m_locker.AcquireWriterLock(1000);
                try {
                    foreach (var line in lines.Item1) {
                        if (lines.Item2) {                            
                            m_steamWriter.WriteLine(line);
                        } else {
                            m_steamWriter.Write(line);
                        }
                    }
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

        public void start_logging() { 
            m_writingJob = new WritingFileThread();
            m_writingJob.doLoop = true;
            m_writingJob.start();
        }

        public bool open_file(string filePath) {

            if (m_writingJob == null) {
                ExVR.Log().error("Writing thread not started.");
                return false;
            }

            m_fileFullPath = filePath;
            if (!m_writingJob.open_file(m_fileFullPath)) {
                ExVR.Log().error(string.Format("Cannot open stream writer with path [{0}].", m_fileFullPath));
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

        public void write_lines(List<string> values) {

            if (!m_canWrite || values == null) {
                return;
            }

            if (values.Count > 0) {
                m_writingJob.add_lines(values);
            }
        }

        public void write(object value, bool line = true) {

            if (!m_canWrite || value == null) {
                return;
            }

            if (line) {
                m_writingJob.add_line(Converter.to_string(value));
            } else {
                m_writingJob.add_text(Converter.to_string(value));
            }
        }

        public bool is_writing() {
            return m_writingJob.is_writing();
        }

        public string file_path() {
            return m_fileFullPath;
        }
    }

    public class BaseLoggerComponent : ExComponent {

        // parameters
        protected bool m_addInstanceToFileName;
        protected string m_directoryPath;
        protected string m_baseFileName;
        protected string m_fileExtension;

        protected FileLogger m_fileLogger = null;

        #region ex_functions

        protected override void start_experiment() {

            m_fileLogger = new FileLogger();

            string fileName = generate_file_name();
            if (fileName.Length == 0) {
                log_error("File is empty.");
                return;
            }

            string fullPath = string.Format("{0}/{1}", m_directoryPath, fileName);
            if (!FileLogger.create_file(
                    fullPath,
                    "",
                    initC.get<bool>("dont_write_if_file_exists"),
                    initC.get<bool>("add_to_end_if_file_exists"))){
                return;
            }

            m_fileLogger.start_logging();
            m_fileLogger.open_file(fullPath);
        }

        protected override void stop_experiment() {
            m_fileLogger.stop_logging();
        }

        #endregion

        #region private_functions

        protected bool read_common_init_parameters() {

            m_directoryPath = initC.get_resource_path(ResourcesManager.ResourceType.Directory, "directory");
            if (m_directoryPath.Length == 0) {
                log_error("No directory resource defined.");
                return false;
            }

            m_addInstanceToFileName = initC.get<bool>("add_current_instance_to_file_name");
            m_baseFileName          = initC.get<string>("base_file_name");
            m_fileExtension         = initC.get<string>("file_extension");

            return true;
        }

        protected virtual string generate_file_name() {
            return "";
        }

        #endregion

        #region public_functions

        public bool is_writing() {
            return m_fileLogger.is_writing();
        }

        public string parent_directory_path() {
            return m_directoryPath;
        }

        public string file_path() {
            return m_fileLogger.file_path();
        }

        public string file_extension() {
            return m_fileExtension;
        }

        public void write_lines(List<string> values) {
            m_fileLogger.write_lines(values);
        }

        public void write(object value, bool line = true) {
            m_fileLogger.write(value, line);
        }

        #endregion
    }
}
