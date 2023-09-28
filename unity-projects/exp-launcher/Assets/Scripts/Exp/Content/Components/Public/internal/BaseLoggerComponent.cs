
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
using System.Collections.Generic;

namespace Ex {

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
            m_fileLogger.set_file_path(fullPath);

            UnityEngine.Debug.Log("m_fileLogger " + m_fileLogger.ToString());
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
