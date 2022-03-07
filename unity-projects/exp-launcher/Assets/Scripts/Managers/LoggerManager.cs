
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
using System.Runtime.CompilerServices;

// unity
using UnityEngine;

namespace Ex {

    public class Trace{
        public string message = null;
    }

    public class ComponentTrace : Trace{ 
        public ComponentTrace(ExComponent component, string message) {
            this.component = component;
            this.message = message;
        }

        public ComponentTrace(string message) {
            this.message = message;
        }

        public ExComponent component = null;
    }

    public class ComponentsManagerTrace : Trace{
        public ComponentsManagerTrace(ExComponent component, ExComponent.Function function, bool start) {
            this.component  = component;
            this.function   = function;
            this.start      = start;
        }

        public bool start = false;
        public ExComponent component = null;
        public ExComponent.Function function = ExComponent.Function.undefined;
    }

    public class RoutinesManagerTrace : Trace{
        public RoutinesManagerTrace(Routine routine, string message, bool start) {
            this.routine = routine;
            this.message = message;
            this.start   = start;            
        }

        public bool start = false;
        public Routine routine = null;
    }

    public class LoggerManager : MonoBehaviour{

        public Log log = null;
        public ExperimentLogger exp = null;

        public void initialize() {
            log = new Log();
            exp = new ExperimentLogger();
        }
    }
    
    public class Log{

        private static readonly string loggerMessageFormat          = "[MESSAGE] {0}";
        private static readonly string loggerWarningFormat          = "[WARNING] {0}";
        private static readonly string loggerErrorFormat            = "[ERROR] {0}";
        private static readonly string loggerMessageExtraInfoFormat = "[MESSAGE] {0} from [{1}] at line ({2}) in file [{3}]";
        private static readonly string loggerWarningExtraInfoFormat = "[WARNING] {0} from [{1}] at line ({2}) in file [{3}]";
        private static readonly string loggerErrorExtraInfoFormat   = "[ERROR] {0} from [{1}] at line ({2}) in file [{3}]";

        private static readonly string guiMessageFormat             = "<font color=#bdbdbd>{0}</font>";
        private static readonly string guiWarningFormat             = "<font color=#f39e03>{0}</font>";
        private static readonly string guiErrorFormat               = "<font color=#f40404>{0}</font>";
        private static readonly string guiMessageExtraInfoFormat    = "<font color=#bdbdbd>{0}</font><font color=#808080> from <b>{1}</b> at line <b>{2}</b> in file <i>{3}</i></font>";
        private static readonly string guiWarningExtraInfoFormat    = "<font color=#f39e03>{0}</font><font color=#808080> from <b>{1}</b> at line <b>{2}</b> in file <i>{3}</i></font>";
        private static readonly string guiErrorExtraInfoFormat      = "<font color=#f40404>{0}</font><font color=#808080> from <b>{1}</b> at line <b>{2}</b> in file <i>{3}</i></font>";
#if UNITY_EDITOR
        private static readonly string editorMessageFormat          = "<color=green>{0}</color>";
        private static readonly string editorWarningFormat          = "<color=orange>{0}</color>";
        private static readonly string editorErrorFormat            = "<color=red>{0}</color>";
        private static readonly string editorMessageExtraInfoFormat = "<color=green>{0}</color> from <b>{1}</b> at line <b>{2}</b> in file <i>{3}</i>";
        private static readonly string editorWarningExtraInfoFormat = "<color=orange>{0}</color> from <b>{1}</b> at line <b>{2}</b> in file <i>{3}</i>";
        private static readonly string editorErrorExtraInfoFormat   = "<color=red>{0}</color> from <b>{1}</b> at line <b>{2}</b> in file <i>{3}</i>";
#endif

        public void no_logger_message(object m, bool addExtraInfo = false,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0) {
            message(m, addExtraInfo, memberName, sourceFilePath, sourceLineNumber, false);
        }
        public void no_logger_warning(object w, bool addExtraInfo = true,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0) {
            warning(w, addExtraInfo, memberName, sourceFilePath, sourceLineNumber, false);
        }
        public void no_logger_error(object e, bool addExtraInfo = true,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0) {
            error(e, addExtraInfo, memberName, sourceFilePath, sourceLineNumber, false);
        }

        public void message(object message, bool addExtraInfo = false,
            [CallerMemberName]  string memberName = "",
            [CallerFilePath]    string sourceFilePath = "",
            [CallerLineNumber]  int sourceLineNumber = 0, bool logger = true) {

            var messageStr = Converter.to_string(message);
            if (addExtraInfo) {
                var lineStr = Converter.to_string(sourceLineNumber);
#if UNITY_EDITOR

                UnityEngine.Debug.Log(string.Format(editorMessageExtraInfoFormat, messageStr, memberName, lineStr, sourceFilePath));
#endif
                ExVR.Events().log.Message.Invoke(string.Format(guiMessageExtraInfoFormat, messageStr, memberName, lineStr, sourceFilePath));
                if (logger) {
                    ExVR.Events().log.LoggerMessage.Invoke(string.Format(loggerMessageExtraInfoFormat, messageStr, memberName, lineStr, sourceFilePath));
                }
            } else {
#if UNITY_EDITOR

                UnityEngine.Debug.Log(string.Format(editorMessageFormat, messageStr));
#endif
                ExVR.Events().log.Message.Invoke(string.Format(guiMessageFormat, messageStr));
                if (logger) {
                    ExVR.Events().log.LoggerMessage.Invoke(string.Format(loggerMessageFormat, messageStr));
                }
            }
        }

        public void warning(object warning, bool addExtraInfo = true,
            [CallerMemberName]  string memberName = "",
            [CallerFilePath]    string sourceFilePath = "",
            [CallerLineNumber]  int sourceLineNumber = 0, bool logger = true) {

            var warningStr = Converter.to_string(warning);
            if (addExtraInfo) {
                var lineStr = Converter.to_string(sourceLineNumber);
#if UNITY_EDITOR
                UnityEngine.Debug.Log(string.Format(editorWarningExtraInfoFormat, warningStr, memberName, lineStr, sourceFilePath));
#endif
                ExVR.Events().log.Warning.Invoke(string.Format(guiWarningExtraInfoFormat, warningStr, memberName, lineStr, sourceFilePath));
                if (logger) {
                    ExVR.Events().log.LoggerWarning.Invoke(string.Format(loggerWarningExtraInfoFormat, warningStr, memberName, lineStr, sourceFilePath));
                }
            } else {
#if UNITY_EDITOR

                UnityEngine.Debug.Log(string.Format(editorWarningFormat, warningStr));
#endif
                ExVR.Events().log.Warning.Invoke(string.Format(guiWarningFormat, warningStr));
                if (logger) {
                    ExVR.Events().log.LoggerWarning.Invoke(string.Format(loggerWarningFormat, warningStr));
                }
            }
        }
        

        public void error(object error, bool addExtraInfo = true,
            [CallerMemberName]  string memberName = "",
            [CallerFilePath]    string sourceFilePath = "",
            [CallerLineNumber]  int sourceLineNumber = 0, bool logger = true) {

            var errorStr = Converter.to_string(error);
            if (addExtraInfo) {
                var lineStr = Converter.to_string(sourceLineNumber);
#if UNITY_EDITOR
                UnityEngine.Debug.LogError(string.Format(editorErrorExtraInfoFormat, errorStr, memberName, lineStr, sourceFilePath));
#endif
                ExVR.Events().log.Error.Invoke(string.Format(guiErrorExtraInfoFormat, errorStr, memberName, lineStr, sourceFilePath));
                if (logger) {
                    ExVR.Events().log.LoggerError.Invoke(string.Format(loggerErrorExtraInfoFormat, errorStr, memberName, lineStr, sourceFilePath));
                }
            } else {
#if UNITY_EDITOR

                UnityEngine.Debug.LogError(string.Format(editorErrorFormat, errorStr));
#endif
                ExVR.Events().log.Error.Invoke(string.Format(guiErrorFormat, errorStr));
                if (logger) {
                    ExVR.Events().log.LoggerError.Invoke(string.Format(loggerErrorFormat, errorStr));
                }
            }
        }
    }

    public class ExperimentLogger{

        [SerializeField]
        private string filePath = "";
        private string prevFilePath = "";
        
        private System.Collections.Concurrent.ConcurrentQueue<string> flow = null;
        // stack of the components functions call
        private System.Collections.Concurrent.ConcurrentStack<Trace> stackTrace = new System.Collections.Concurrent.ConcurrentStack<Trace>();

        private static readonly string BuilderStr = "[BUILDER] {0}";
        private static readonly string GuiStr = "[GUI] {0}";
        private static readonly string ExpStr = "[EXP] {0}";
        private static readonly string CmdStr = "[COMMAND] {0}";

        private static readonly string StartCondCompStr = "\t[Condition] (Begin) Component function {0}";
        private static readonly string EndCondCompStr = "\t[Condition] (End) Component function {0}";
        private static readonly string StartCondConnStr = "\t[Condition] (Begin) Connector function {0}";
        private static readonly string EndCondConnStr = "\t[Condition] (End) Connector function {0}";

        private static readonly string StartCompMaStr = "\t[Components_manager] (Begin) {0}";
        private static readonly string EndCompMaStr = "\t[Components_manager] (End) {0}";
        private static readonly string EnableRoutineManStr = "\t[Routines_manager] (Enable routine: {0})(Cond: {1})";
        private static readonly string RoutineManStr = "\t[Routines_manager] {0}";
        private static readonly string EnableIsiManStr = "\t[ISI_manager] (Enable ISI: {0})(Cond: {1})";
        private static readonly string StartCompStr = "\t\t\t[Component] {0} (Conf: {1}) (Func: {2}{3})";
        private static readonly string RoutineStr = "\t\t[Routine] {0} (Condition: {1})(Message: {2})";
        private static readonly string NullStr = "-";

        private static readonly string AddT0     = "{0} (tTot: {1})";
        private static readonly string AddT1     = "{0} (tExp: {1}s)";
        private static readonly string AddT2     = "{0} (tElem: {1}s)";
        private static readonly string AddT0T1   = "{0} (tTot: {1}) (tExp: {2}s)";
        private static readonly string AddT0T2   = "{0} (tTot: {1}) (tElem: {2}s)";
        private static readonly string AddT1T2   = "{0} (tExp: {1}s) (tElem: {2}s)";
        private static readonly string AddT0T1T2 = "{0} (tTot: {1}) (tExp: {2}s) (tElem: {3}s)";

        private static readonly string DateNowF = "HH:mm:ss.fff";

        private Dictionary<ExComponent.Function, string> componentsFunctionsStr = new Dictionary<ExComponent.Function, string>();
        private Dictionary<ExConnector.Function, string> connectorsFunctionsStr = new Dictionary<ExConnector.Function, string>();

        
        private Trace[] swapTrace = null;
        public ExperimentLogger() {

            foreach(ExComponent.Function f in Enum.GetValues(typeof(ExComponent.Function))) {
                componentsFunctionsStr[f] = f.ToString();
            }

            foreach (ExConnector.Function f in Enum.GetValues(typeof(ExConnector.Function))) {
                connectorsFunctionsStr[f] = f.ToString();
            }

            filePath     = string.Format("{0}/exp_log.txt", ExVR.Paths().designerLogDir);
            prevFilePath = string.Format("{0}/exp_log_previous.txt", ExVR.Paths().designerLogDir);

            // copy previous log file
            if (File.Exists(filePath)) {
                try {
                    File.Copy(filePath, prevFilePath, true);
                } catch (Exception exception) {
                    ExVR.Log().no_logger_error(string.Format("Cannot copy logger file to prev logger path {0}, get error {1}", prevFilePath, exception.Message));
                }
            }

            flow = new System.Collections.Concurrent.ConcurrentQueue<string>();
            try {
                File.WriteAllText(filePath, string.Concat("[EXVR-EXP] Started at ", DateTime.Now.ToString("MM/dd/yyyy HH:mm:ss"), "\n"));
            } catch (Exception exception) {
                ExVR.Log().no_logger_error(string.Format("Cannot create logger file {0}, get error {1}", filePath, exception.Message));
            }
            ExVR.Log().no_logger_message(string.Format("Log file {0} generated.", filePath));

            ExVR.Events().log.LoggerMessage.AddListener((message) => {
                add(message, false, false, false);
            });
            ExVR.Events().log.LoggerWarning.AddListener((warning) => {
                add(warning, false, true, true);
            });
            ExVR.Events().log.LoggerError.AddListener((error) => {
                add(error, true, true, true);
            });
            ExVR.Events().stacktrace.ComponentTrace.AddListener((component, message) => {
                push_to_strackTrace(new ComponentTrace(component, message));
            });

            swapTrace = new Trace[100];
        }

        public void isi_manager(ISIInfo info) {
            add(string.Format(EnableIsiManStr, info.name(), info.duration_str()), false, true, false);
        }

        public void routine_manager(string message) {
            add(string.Format(RoutineManStr, message), false, true, true);
        }

        public void routine_manager(RoutineInfo info) {
            add(string.Format(EnableRoutineManStr, info.name(), info.condition().name), false, true, false);
        }


        public void component_manager(ExComponent.Function function, bool start) {
            if (start) {
                add(string.Format(StartCompMaStr, componentsFunctionsStr[function]), false, true, true);
            } else {
                add(string.Format(EndCompMaStr, componentsFunctionsStr[function]), false, true, true);
            }            
        }

        public void condition(ExConnector.Function function, bool start) {
            if (start) {
                add(string.Format(StartCondConnStr, connectorsFunctionsStr[function]), false, true, true);
            } else {
                add(string.Format(EndCondConnStr, connectorsFunctionsStr[function]), false, true, true);
            }
        }

        public void condition(ExComponent.Function function, bool start) {
            if (start) {
                add(string.Format(StartCondCompStr, componentsFunctionsStr[function]), false, true, true);
            } else {
                add(string.Format(EndCondCompStr, componentsFunctionsStr[function]), false, true, true);
            }
        }

        public void log_and_add_to_stacktrace(ExComponent c, ExComponent.Function function, bool start, bool log = false, bool timeExp = false, bool timeElem = false) {

            if (log) {
                component(c, function, "", "", timeExp, timeElem);
            }
            push_to_strackTrace(new ComponentsManagerTrace(c, function, start));
        }

        public void reset_strackTrace() {
            stackTrace = new System.Collections.Concurrent.ConcurrentStack<Trace>();
        }

        public void push_to_strackTrace(Trace trace) {

            stackTrace.Push(trace);            
            int count = stackTrace.Count;
            if(count > 10000) {                
                stackTrace.TryPopRange(swapTrace, 0, 100);
                stackTrace = new System.Collections.Concurrent.ConcurrentStack<Trace>();
                stackTrace.PushRange(swapTrace);
            }            
        }

        public void display_strackTrace(int max = 100) {

            int maxDisplayed = Math.Min(stackTrace.Count, max);
            add(string.Format("### [Components stack trace, display last {0} traces out of {1}] ###", maxDisplayed, stackTrace.Count), false, false, false);

            for (int ii = 0; ii < maxDisplayed; ++ii) {

                Trace trace;
                stackTrace.TryPop(out trace);

                if (trace is ComponentTrace componentTrace) {

                    if (componentTrace.component != null) {
                        add(string.Format("\t\t\t-> from component [{0}] with stack id ({1}) with message [{2}]",
                            componentTrace.component.name,
                            ii,
                            componentTrace.message
                        ), false, false, false);
                    } else {
                        add(string.Format("\t\t\t-> from an undefined component with stack id ({0}) with message [{1}]",
                            ii,
                            componentTrace.message
                        ), false, false, false);
                    }

                } else if (trace is ComponentsManagerTrace componentsManagerTrace) {

                    if (trace.message != null) {
                        add(string.Format("\t\t-> function [{0}][{1}] of component [{2}] with stack id ({3}) with message [4]",
                            componentsManagerTrace.function,
                            componentsManagerTrace.start ? "start" : "end",
                            componentsManagerTrace.component.name,
                            ii,
                            componentsManagerTrace.message
                        ), false, false, false);
                    } else {
                        add(string.Format("\t\t-> function [{0}][{1}] of component [{2}] with stack id ({3})",
                            componentsManagerTrace.function,
                            componentsManagerTrace.start ? "start" : "end",
                            componentsManagerTrace.component.name,
                            ii
                        ), false, false, false);
                    }
                } else if (trace is RoutinesManagerTrace routinesManagerTrace) {
                    add(string.Format("\t-> routine [{0}][{1}] with stack id ({2}) with message [{3}]",
                        routinesManagerTrace.routine.name,
                        routinesManagerTrace.start ? "start" : "end",                        
                        ii,
                        routinesManagerTrace.message
                    ), false, false, false);
                }
            }

            add("### [End stack trace] ###", false, false, false);
        }

        public void routine(string routineName, string condition, string message) {
            add(string.Format(RoutineStr, routineName, condition, message), false, true, false);
        }

        public void component(ExComponent component, ExComponent.Function function, string config = "", string extra = "", bool timeExp = false, bool timeCurrentElement = false) {
            add(string.Format(StartCompStr, component.name, (config.Length > 0) ? config : NullStr, function.ToString(), extra),
                false, timeExp, timeCurrentElement
            );
        }

        public void command(string text) {
            add(string.Format(CmdStr, text), true, true, true);
        }

        public void gui(string text) {
            add(string.Format(GuiStr, text), true, true, true);
        }

        public void exp(string text, bool time, bool timeExp, bool timeCurrentElement) {            
            add(string.Format(ExpStr, text), time, timeExp, timeCurrentElement);
        }

        public void builder(string text) {
            add(string.Format(BuilderStr, text), false, false, false);
        }

        private void add(string state, bool time, bool timeExp, bool timeCurrentElement) {
            
            if (time) {

                if (timeExp && timeCurrentElement) {
                    flow.Enqueue(string.Format(AddT0T1T2,
                        state,
                        DateTime.Now.ToString(DateNowF),
                        ExVR.Time().ellapsed_exp_s().ToString(),
                        ExVR.Time().ellapsed_element_s().ToString()
                    ));
                } else if (timeExp) {
                    flow.Enqueue(string.Format(AddT0T1,
                        state,
                        DateTime.Now.ToString(DateNowF),
                        ExVR.Time().ellapsed_exp_s().ToString()
                    ));
                } else if (timeCurrentElement) {
                    flow.Enqueue(string.Format(AddT0T2,
                        state,
                        DateTime.Now.ToString(DateNowF),
                        ExVR.Time().ellapsed_element_s().ToString()
                    ));
                } else {
                    flow.Enqueue(string.Format(AddT0,
                        state,
                        DateTime.Now.ToString(DateNowF)
                    ));
                }

            } else {
                if (timeExp && timeCurrentElement) {
                    flow.Enqueue(string.Format(AddT1T2,
                        state,
                        ExVR.Time().ellapsed_exp_s().ToString(),
                        ExVR.Time().ellapsed_element_s().ToString()
                    ));
                } else if (timeExp) {
                    flow.Enqueue(string.Format(AddT1,
                        state,
                        ExVR.Time().ellapsed_exp_s().ToString()
                    ));
                } else if (timeCurrentElement) {
                    flow.Enqueue(string.Format(AddT2,
                        state,
                        ExVR.Time().ellapsed_element_s().ToString()
                    ));
                } else {
                    flow.Enqueue(state);
                }
            }
        }

        public void write() {

            try {
                File.AppendAllLines(filePath, flow);
            } catch (Exception exception) {
                ExVR.Log().no_logger_error(string.Format("Cannot write on logger file {0}, get error {1}", filePath, exception.Message));
            }
            flow = new System.Collections.Concurrent.ConcurrentQueue<string>();
        }

    }
}


