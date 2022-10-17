
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

namespace Ex {


    public class CommonLoggingComponents {
        public List<ExComponent> components  = null; // components to log
        public FileLogger frameFileLogger    = null; // file logger of current state at each frame
        public FileLogger triggersFileLogger = null; // file logger for triggers events
    }


    public class GlobalLoggerComponent : ExComponent {

        private FileLogger expFrameFileLogger    = null;
        private FileLogger expTriggersFileLogger = null;
        private string resourceDirectoryPath;
        private int m_nbExpFrameColumns = 0;

        private Dictionary<Category, CommonLoggingComponents> loggingPerCategory = new Dictionary<Category, CommonLoggingComponents>() {
            [Category.Input]    = new CommonLoggingComponents(),
            [Category.Network]  = new CommonLoggingComponents(),
            [Category.Tracking] = new CommonLoggingComponents(),
            [Category.UI]       = new CommonLoggingComponents(),
        };

        #region ex_functions

        protected override bool initialize() {

            resourceDirectoryPath = initC.get_resource_path(ResourcesManager.ResourceType.Directory, "directory");
            if (resourceDirectoryPath.Length == 0) {
                resourceDirectoryPath = ExVR.Paths().designerLogDir;
                log_warning(string.Format("No directory resource defined. Log path of designer will be used instead [{0}]", resourceDirectoryPath));                
            }

            return true;
        }

        protected override void start_experiment() {

            // generate directory
            string topSubDirPath = string.Format("{0}/{1}{2}{3}",
                resourceDirectoryPath,
                initC.get<string>("base_sub_directory_name"),
                initC.get<bool>("add_current_instance_to_sub_directory_name") ? 
                    string.Format("_{0}", ExVR.Experiment().instanceName) : "",
                initC.get<bool>("add_date_to_sub_directory_name") ? 
                    string.Format("_{0}", DateTime.Now.ToString(initC.get<string>("date_time_format"))) : ""
            );

            int count = 1;
            string globalDirectoryPath = topSubDirPath;            
            while (System.IO.Directory.Exists(topSubDirPath)) {
                globalDirectoryPath = string.Format("{0}_{1}", topSubDirPath, count++);
            }

            var folder = System.IO.Directory.CreateDirectory(globalDirectoryPath);
            if (!folder.Exists) {
                log_error(string.Format("Cannot create top directory [{0}].", globalDirectoryPath));
                return;
            }

            // init loggers
            // # exp
            // ## frame
            string expFrameLoggerFilePath = string.Format("{0}/log_frame_exp.{1}",
                globalDirectoryPath,
                initC.get<string>("file_extension"));
            if (!FileLogger.create_file(expFrameLoggerFilePath, "", false, false)) {
                return;
            }
            expFrameFileLogger = new FileLogger();
            expFrameFileLogger.start_logging();
            if (!expFrameFileLogger.open_file(expFrameLoggerFilePath)) {
                return;
            } else {
                List<string> values = new List<string>();                
                if (initC.get<bool>("ellapsed_exp_time")) {
                    values.Add("ellapsed_exp_time");
                }
                if (initC.get<bool>("frame_id")) {
                    values.Add("frame_id");
                }
                if (initC.get<bool>("start_frame_time")) {
                    values.Add("start_frame_time");
                }
                if (initC.get<bool>("ellapsed_element_time")) {
                    values.Add("ellapsed_element_time");
                }
                if (initC.get<bool>("element_order")) {
                    values.Add("element_order");
                }
                if (initC.get<bool>("routine_name")) {
                    values.Add("routine_name");
                }
                if (initC.get<bool>("element_iteration")) {
                    values.Add("element_iteration");
                }
                if (initC.get<bool>("condition_name")) {
                    values.Add("condition_name");
                }
                if (initC.get<bool>("condition_nb_calls")) {
                    values.Add("condition_nb_calls");
                }
                if (initC.get<bool>("actions_count")) {
                    values.Add("actions_count");
                }
                if (initC.get<bool>("connectors_count")) {
                    values.Add("connectors_count");
                }
                if (initC.get<bool>("connections_count")) {
                    values.Add("connections_count");
                }
                if (initC.get<bool>("condition_duration")) {
                    values.Add("condition_duration");
                }
                if (initC.get<bool>("eyes_camera")) {
                    values.Add("eyes_camera_pos");
                    values.Add("eyes_camera_rot");
                }
                if (initC.get<bool>("camera_calibration")) {
                    values.Add("camera_calibration_pos");
                    values.Add("camera_calibration_rot");
                }
                if (initC.get<bool>("camera_rig")) {
                    values.Add("camera_rig_pos");
                    values.Add("camera_rig_rot");
                }
                if (initC.get<bool>("framerate")) {
                    values.Add("framerate");
                }
                m_nbExpFrameColumns = values.Count;
                expFrameFileLogger.write(string.Join(",", values), true);
            }
            // ## triggers
            string expTriggersLoggerFilePath = string.Format("{0}/log_triggers_exp.{1}",
                globalDirectoryPath,
                initC.get<string>("file_extension"));
            if (!FileLogger.create_file(expTriggersLoggerFilePath, "", false, false)) {
                return;
            }
            expTriggersFileLogger = new FileLogger();
            expTriggersFileLogger.start_logging();
            if (!expTriggersFileLogger.open_file(expTriggersLoggerFilePath)) {
                return;
            }

            // # components
            loggingPerCategory[Category.Input].components    = initC.get_components_list("inputs_components");
            loggingPerCategory[Category.Network].components  = initC.get_components_list("network_components");
            loggingPerCategory[Category.UI].components       = initC.get_components_list("ui_components");
            loggingPerCategory[Category.Tracking].components = initC.get_components_list("tracking_components");

            foreach(var logging in loggingPerCategory) {

                if (logging.Value.components.Count > 0) {

                    // count components to be logged
                    int countFrame = 0, countTrigger = 0;
                    foreach (var component in logging.Value.components) {
                        if (component.has_frame_logging()) {
                            countFrame++;
                        }
                        if (component.has_trigger_logging()) {
                            countTrigger++;
                        }
                    }
                    if (countFrame > 0) {

                        // init frame logger
                        string frameLoggerFilePath = string.Format("{0}/log_frame_{1}.{2}", 
                            globalDirectoryPath,
                            Enum.GetName(typeof(Category), logging.Key).ToLower(),
                            initC.get<string>("file_extension"));
                        if (!FileLogger.create_file(frameLoggerFilePath, "", false, false)) {
                            return;
                        }

                        logging.Value.frameFileLogger = new FileLogger();
                        logging.Value.frameFileLogger.start_logging();
                        if (!logging.Value.frameFileLogger.open_file(frameLoggerFilePath)) {
                            return;
                        }

                        // write headers
                        List<string> headers = new List<string>();
                        headers.Add("frame_id");

                        foreach (var component in logging.Value.components) {
                            var header = component.format_frame_data_for_global_logger(true);
                            if(header != null) {
                                headers.Add(header);
                            }
                        }
                        logging.Value.frameFileLogger.write(string.Join(",", headers), true);
                    }

                    if (countTrigger > 0) {

                        // init trigger logger
                        string triggerLoggerFilePath = string.Format("{0}/log_triggers_{1}.{2}",
                            globalDirectoryPath,
                            Enum.GetName(typeof(Category), logging.Key).ToLower(),
                            initC.get<string>("file_extension"));
                        if (!FileLogger.create_file(triggerLoggerFilePath, "", false, false)) {
                            return;
                        }

                        logging.Value.triggersFileLogger = new FileLogger();
                        logging.Value.triggersFileLogger.start_logging();
                        if (!logging.Value.triggersFileLogger.open_file(triggerLoggerFilePath)) {
                            return;
                        }
                        logging.Value.triggersFileLogger.write(
                            "frame_id,routine_name,condition_name,ellapsed_exp_time,ellapsed_element_time,trigger_info", true);
                    }
                }
            }
        }

        protected override void end_of_frame() {

            var routine   = ExVR.Routines().current();
            var condition = routine.current_condition();

            if (expFrameFileLogger != null) {

                var elementInfo    = ExVR.Scheduler().current_element_info();
                var cameras        = ExVR.Display().cameras();
                var eyeTr          = cameras.get_eye_camera_transform();
                var calibTr        = cameras.get_calibration_transform();
                var rigTr          = cameras.get_camera_rig_transform();

                List<string> values = new List<string>(m_nbExpFrameColumns);
                if (initC.get<bool>("ellapsed_exp_time")) {
                    values.Add(Converter.to_string(time().ellapsed_exp_ms()));
                }
                if (initC.get<bool>("frame_id")) {
                    values.Add(Converter.to_string(time().frame_id()));
                }
                if (initC.get<bool>("start_frame_time")) {
                    values.Add(Converter.to_string(time().frame_start_since_experiment_ms()));
                }
                if (initC.get<bool>("ellapsed_element_time")) {
                    values.Add(Converter.to_string(time().ellapsed_element_ms()));
                }
                if (initC.get<bool>("element_order")) {
                    values.Add(Converter.to_string(elementInfo.order()));
                }
                if (initC.get<bool>("routine_name")) {
                    values.Add(routine.name);
                }
                if (initC.get<bool>("element_iteration")) {
                    values.Add(Converter.to_string(routine.element_iteration()));
                }
                if (initC.get<bool>("condition_name")) {
                    values.Add(condition.name);
                }
                if (initC.get<bool>("condition_nb_calls")) {
                    values.Add(Converter.to_string(condition.calls_nb()));
                }
                if (initC.get<bool>("actions_count")) {
                    values.Add(Converter.to_string(condition.actions.Count));
                }
                if (initC.get<bool>("connectors_count")) {
                    values.Add(Converter.to_string(condition.connectors.Count));
                }
                if (initC.get<bool>("connections_count")) {
                    values.Add(Converter.to_string(condition.connections.Count));
                }
                if (initC.get<bool>("condition_duration")) {
                    values.Add(Converter.to_string(condition.duration()));
                }
                if (initC.get<bool>("eyes_camera")) {
                    values.Add(Converter.to_string(eyeTr.localPosition, Converter.g7, " "));
                    values.Add(Converter.to_string(eyeTr.localEulerAngles, Converter.g7, " "));
                }
                if (initC.get<bool>("camera_calibration")) {
                    values.Add(Converter.to_string(calibTr.localPosition, Converter.g7, " "));
                    values.Add(Converter.to_string(calibTr.localEulerAngles, Converter.g7, " "));
                }
                if (initC.get<bool>("camera_rig")) {
                    values.Add(Converter.to_string(rigTr.localPosition, Converter.g7, " "));
                    values.Add(Converter.to_string(rigTr.localEulerAngles, Converter.g7, " "));
                }
                if (initC.get<bool>("framerate")) {
                    values.Add(Converter.to_string(FPSCounter.framerate));
                }

                if (values.Count > 0) {
                    expFrameFileLogger.write(string.Join(",", values), true);
                }
            }

            foreach (var logging in loggingPerCategory) {

                if (logging.Value.frameFileLogger != null) {

                    // retrieve data
                    List<string> values = new List<string>();
                    values.Add(Converter.to_string(time().frame_id()));
                    foreach(var component in logging.Value.components) {
                        var log = component.format_frame_data_for_global_logger(false);
                        if(log != null) {        
                            values.Add(log);
                        }
                    }

                    if(values != null) {
                        logging.Value.frameFileLogger.write(string.Join(",", values), true);
                    }
                }

                if (logging.Value.triggersFileLogger != null) {

                    var idFrameStr = Converter.to_string(time().frame_id());

                    // retrieve data
                    List<Tuple<double, double, string>> allTriggersLines = null;
                    foreach (var component in logging.Value.components) {
                        var triggersLines = component.format_trigger_data_for_global_logger();
                        if (triggersLines == null) {
                            continue;
                        }

                        if(allTriggersLines == null) {
                            allTriggersLines = triggersLines;
                        } else {
                            foreach (var triggerLine in triggersLines) {
                                allTriggersLines.Add(triggerLine);
                            }
                        }
                    }

                    if(allTriggersLines != null) {

                        // sort by exp time
                        allTriggersLines.Sort(delegate (Tuple<double, double, string> t1, Tuple<double, double, string> t2) {
                            if (t1.Item1 < t2.Item1) {
                                return 1;
                            } else if (t1.Item1 > t2.Item1) {
                                return -1;
                            }
                            return 0;
                        });

                        // format lines
                        System.Text.StringBuilder sb = new System.Text.StringBuilder();
                        foreach(var triggerLine in allTriggersLines) {
                            sb.AppendFormat("{0},{1},{2},{3},{4},{5}\n",
                                idFrameStr,
                                routine.name,
                                condition.name,
                                Converter.to_string(triggerLine.Item1),
                                Converter.to_string(triggerLine.Item2),
                                triggerLine.Item3
                            );
                        }

                        // write data
                        logging.Value.triggersFileLogger.write(sb.ToString(), false);
                    }

                }
            }
        }

        protected override void stop_experiment() {

            if(expFrameFileLogger != null) {
                expFrameFileLogger.stop_logging();
            }
            if (expFrameFileLogger != null) {
                expTriggersFileLogger.stop_logging();
            }

            foreach (var logging in loggingPerCategory) {
                if(logging.Value.frameFileLogger != null) {
                    logging.Value.frameFileLogger.stop_logging();
                }
                if (logging.Value.triggersFileLogger != null) {
                    logging.Value.triggersFileLogger.stop_logging();
                }
            }
        }

        #endregion
    }
}
