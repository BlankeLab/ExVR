
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

namespace Ex {

    public class BaseLoggerComponent : ExComponent {

        // parameters
        protected bool m_dontWriteIfFileExists;
        protected bool m_addToEndIfFileExists;
        protected bool m_streamToFile;        
        protected bool m_addInstanceToFileName;
        protected string m_directoryPath;
        protected string m_baseFileName;
        protected string m_fileExtension;
        protected string m_fileFullPath;

        protected List<string> m_lines = null;

        #region ex_functions

        protected override void start_experiment() {

            string fileName = generate_file_name();
            if (fileName.Length == 0) {
                log_error("File is empty.");
                return;
            }
            m_fileFullPath = string.Format("{0}/{1}", m_directoryPath, fileName);

            if (!create_file(m_fileFullPath)) {
                return;
            }

            if (initC.get<bool>("add_header_line")) {
                write(initC.get<string>("header_line"), true);
            }
        }

        protected override void stop_experiment() {

            if (!m_streamToFile) {
                write_to_file();
            }
        }

        #endregion

        #region private_functions

        protected bool read_common_init_parameters() {

            m_directoryPath = initC.get_resource_path(ResourcesManager.ResourceType.Directory, "directory");
            if (m_directoryPath.Length == 0) {
                log_error("No directory resource defined.");
                return false;
            }

            m_streamToFile = initC.get<bool>("stream_to_file");
            m_dontWriteIfFileExists = initC.get<bool>("dont_write_if_file_exists");
            m_addToEndIfFileExists = initC.get<bool>("add_to_end_if_file_exists");
            m_addInstanceToFileName = initC.get<bool>("add_current_instance_to_file_name");
            m_baseFileName = initC.get<string>("base_file_name");
            m_fileExtension = initC.get<string>("file_extension");

            m_lines = new List<string>(); // todo add estimated length for reserving memory

            return true;
        }

        protected virtual string generate_file_name() {
            return "";
        }

        protected bool create_file(string fileFullPath) {

            // check file existence
            bool exists = File.Exists(fileFullPath);
            if (exists && m_dontWriteIfFileExists) {
                log_error(string.Format("File {0} already exists, change writing settings or delete file.", fileFullPath));
                return false;
            }

            // create empty file if necessary
            if (!exists) {
                try {
                    File.Create(fileFullPath).Dispose();
                } catch (Exception ex) {
                    log_error(string.Format("Cannot create file {0}, error {1}", fileFullPath, ex.Message));
                    return false;
                }
            } else if (!m_addToEndIfFileExists) {
                try {
                    File.WriteAllText(fileFullPath, string.Empty, System.Text.Encoding.UTF8);
                } catch (Exception ex) {
                    log_error(string.Format("Cannot reset file {0}, error {1}", fileFullPath, ex.Message));
                    return false;
                }
            }

            return true;
        }


        #endregion

        #region public_functions

        public string parent_directory_path() {
            return m_directoryPath;
        }

        public string file_path() {
            return m_fileFullPath;
        }

        public string file_extension() {
            return m_fileExtension;
        }

        public void write_lines(List<string> values) {

            if (m_streamToFile) {
                File.AppendAllLines(m_fileFullPath, values, System.Text.Encoding.UTF8);
            } else {
                m_lines.AddRange(values);
            }
        }

        public void write(object value, bool line = true) {

            if (m_streamToFile) {

                if (line) {
                    File.AppendAllText(m_fileFullPath, Converter.to_string(value) + '\n', System.Text.Encoding.UTF8);
                } else {
                    File.AppendAllText(m_fileFullPath, Converter.to_string(value), System.Text.Encoding.UTF8);
                }

            } else {
                m_lines.Add(Converter.to_string(value));
            }
        }

        public void write_to_file() {

            if (m_streamToFile) {
                log_error("Do not use write_to_file function when streaming to file is enabled.");
                return;
            }

            // write everything to file
            try {
                if (m_addToEndIfFileExists) {
                    File.AppendAllLines(m_fileFullPath, m_lines, System.Text.Encoding.UTF8);
                } else {
                    File.WriteAllLines(m_fileFullPath, m_lines, System.Text.Encoding.UTF8);
                }
            } catch (Exception ex) {
                log_error(string.Format("Failed to writes lines to file {0}, error {1}", m_fileFullPath, ex.Message));
            }
            m_lines.Clear();
        }

        #endregion
    }
}
