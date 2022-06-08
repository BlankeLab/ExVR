
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

namespace Ex {

    public class LoggerExperimentComponent : BaseLoggerComponent {

        protected bool m_addDateToFileName;
        protected string m_dateTimeFormat;  
        private string m_separator;
        private string m_headerLine;
        private bool m_eachFrame;
        private bool m_addTimeExp;
        private bool m_addTimeRoutine;
        private bool m_addRoutine;
        private bool m_addRoutineIter;
        private bool m_addCondition;
        private bool m_addConditionIter;
        private bool m_addFrameId;
        private int m_countColumns = 0;
        private static readonly string lineC = "{0}{1}";

        #region ex_functions

        protected override bool initialize() {

            if (!read_common_init_parameters()) {
                return false;
            }

            m_addDateToFileName = initC.get<bool>("add_date_to_file_name");
            m_dateTimeFormat    = initC.get<string>("date_time_format");
            m_separator         = initC.get<string>("separator");
            m_eachFrame         = initC.get<bool>("each_frame");

            if (m_addTimeExp       = initC.get<bool>("time_exp"))       { m_countColumns++; }
            if (m_addTimeRoutine   = initC.get<bool>("time_routine"))   { m_countColumns++; }
            if (m_addRoutine       = initC.get<bool>("routine"))        { m_countColumns++; }
            if (m_addRoutineIter   = initC.get<bool>("routine_iter"))   { m_countColumns++; }
            if (m_addCondition     = initC.get<bool>("condition"))      { m_countColumns++; }
            if (m_addConditionIter = initC.get<bool>("condition_iter")) { m_countColumns++; }
            if (m_addFrameId       = initC.get<bool>("frame_id"))       { m_countColumns++; }

            return true;
        }

        protected override void start_experiment() {

            base.start_experiment();
            if (initC.get<bool>("add_header_line")) {
                write_header_line();
            }
        }        

        protected override void start_routine() {           

            if (!initC.get<bool>("each_frame")) {
                write_line();
            }
        }
        protected override void update() {
            if (initC.get<bool>("each_frame")) {
                write_line();
            }
        }

        protected override void stop_experiment() {
            base.stop_experiment();
        }

        #endregion

        #region private_functions

        protected override string generate_file_name() {
            string dateStr = string.Format("_{0}", DateTime.Now.ToString(m_dateTimeFormat));
            return string.Format("{0}{1}{2}.{3}", m_baseFileName, m_addInstanceToFileName ?
                string.Concat("_", ExVR.Experiment().instanceName) : "", m_addDateToFileName ? dateStr : "", m_fileExtension);
        }

        private void write_header_line() {

            System.Text.StringBuilder builder = new System.Text.StringBuilder();
            int count = 0;
            if (m_addTimeExp) {
                if (count < m_countColumns - 1) {
                    builder.AppendFormat(lineC, "[time_exp(ms)]", m_separator);
                } else {
                    builder.Append("[time_exp(ms)]");
                }
                ++count;
            }
            if (m_addTimeRoutine) {
                if (count < m_countColumns - 1) {
                    builder.AppendFormat(lineC, "[time_routine(ms)]", m_separator);
                } else {
                    builder.Append("[time_routine(ms)]");
                }
                ++count;
            }
            if (m_addRoutine) {
                if (count < m_countColumns - 1) {
                    builder.AppendFormat(lineC, "[routine]", m_separator);
                } else {
                    builder.Append("[routine]");
                }
                ++count;
            }
            if (m_addRoutineIter) {
                if (count < m_countColumns - 1) {
                    builder.AppendFormat(lineC, "[routine_iter]", m_separator);
                } else {
                    builder.Append("[routine_iter]");
                }
                ++count;
            }
            if (m_addCondition) {
                if (count < m_countColumns - 1) {
                    builder.AppendFormat(lineC, "[condition]", m_separator);
                } else {
                    builder.Append("[condition]");
                }
                ++count;
            }
            if (m_addConditionIter) {
                if (count < m_countColumns - 1) {
                    builder.AppendFormat(lineC, "[condition_iter]", m_separator);
                } else {
                    builder.Append("[condition_iter]");
                }
                ++count;
            }
            if (m_addFrameId) {
                if (count < m_countColumns - 1) {
                    builder.AppendFormat(lineC, "[frame_id]", m_separator);
                } else {
                    builder.Append("[frame_id]");
                }
                ++count;
            }

            write(builder.ToString(), true);
        }

        private void write_line() {

            System.Text.StringBuilder builder = new System.Text.StringBuilder();
            int count = 0;
            if (m_addTimeExp) {
                if (count < m_countColumns - 1) {
                    builder.AppendFormat(lineC, Converter.to_string(ExVR.Time().ellapsed_exp_ms()), m_separator);
                } else {
                    builder.Append(Converter.to_string(ExVR.Time().ellapsed_exp_ms()));
                }
                ++count;
            }
            if (m_addTimeRoutine) {
                if (count < m_countColumns - 1) {
                    builder.AppendFormat(lineC, Converter.to_string(ExVR.Time().ellapsed_element_ms()), m_separator);
                } else {
                    builder.Append(Converter.to_string(ExVR.Time().ellapsed_element_ms()));
                }
                ++count;
            }
            if (m_addRoutine) {
                if (count < m_countColumns - 1) {
                    builder.AppendFormat(lineC, currentRoutine.name, m_separator);
                } else {
                    builder.Append(currentRoutine.name);
                }
                ++count;
            }
            if (m_addRoutineIter) {
                if (count < m_countColumns - 1) {
                    builder.AppendFormat(lineC, Converter.to_string(currentRoutine.element_iteration()), m_separator);
                } else {
                    builder.Append(Converter.to_string(currentRoutine.element_iteration()));
                }
                ++count;
            }
            if (m_addCondition) {
                if (count < m_countColumns - 1) {
                    builder.AppendFormat(lineC, currentCondition.name, m_separator);
                } else {
                    builder.Append(currentCondition.name);
                }
                ++count;
            }
            if (m_addConditionIter) {
                if (count < m_countColumns - 1) {
                    builder.AppendFormat(lineC, Converter.to_string(currentRoutine.condition_iteration()), m_separator);
                } else {
                    builder.Append(Converter.to_string(currentRoutine.condition_iteration()));
                }
                ++count;
            }
            if (m_addFrameId) {
                if (count < m_countColumns - 1) {
                    builder.AppendFormat(lineC, Converter.to_string(ExVR.Time().frame_id()), m_separator);
                } else {
                    builder.Append(Converter.to_string(ExVR.Time().frame_id()));
                }
                ++count;
            }

            write(builder.ToString(), true);
        }

        #endregion
    }
}
