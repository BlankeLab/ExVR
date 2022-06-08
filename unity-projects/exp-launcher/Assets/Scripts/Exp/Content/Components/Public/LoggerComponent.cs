
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

namespace Ex{

    public class LoggerComponent : BaseLoggerComponent {

        // parameters
        private bool m_insertNewRoutineInfo;
        protected bool m_addDateToFileName;
        protected string m_dateTimeFormat;

        protected override string generate_file_name() {
            string dateStr = string.Format("_{0}", DateTime.Now.ToString(m_dateTimeFormat));
            return string.Format("{0}{1}{2}.{3}", m_baseFileName, 
                m_addInstanceToFileName ? 
                    string.Format("_{0}", ExVR.Experiment().instanceName) : 
                    "", 
                m_addDateToFileName ? 
                    dateStr : 
                    "", 
                m_fileExtension
            );
        }

        protected override bool initialize() {

            if (!read_common_init_parameters()) {
                return false;
            }
            m_insertNewRoutineInfo = initC.get<bool>("insert_new_routine_infos");
            m_addDateToFileName    = initC.get<bool>("add_date_to_file_name");
            m_dateTimeFormat       = initC.get<string>("date_time_format");

            // slots
            add_slot("write", (any) => {
                write(any, false);
            });
            add_slot("write line", (any) => {
                write(any, true);
            });
            add_slot("write lines", (list) => {
                write_lines((List<string>)list);
            });

            return true;
        }

        protected override void start_experiment() {

            base.start_experiment();

            if (initC.get<bool>("add_header_line")) {
                write(initC.get<string>("header_line"), true);
            }
        }

        protected override void start_routine() {

            if(m_insertNewRoutineInfo) {
                string startRoutineLine = String.Format("[Time_exp(ms):{0}][Routine:{1}|Iter:{2}][Condition:{3}|Iter:{4}][Frame_id:{5}])", 
                    Converter.to_string(ExVR.Time().ellapsed_exp_ms()),
                    currentRoutine.name, currentRoutine.element_iteration(),
                    currentCondition.name, currentRoutine.condition_iteration(),
                    Converter.to_string(ExVR.Time().frame_id())
                );
                
                write(startRoutineLine, true);
            }
        }

        protected override void stop_experiment() {
            base.stop_experiment();
        }
    }
}
