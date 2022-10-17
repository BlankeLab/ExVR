
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
using System.Runtime.InteropServices;

namespace Ex {

    public class DLLExperiment : DLLCppImport {

        private static readonly string bracesStr = "{0}{1}";
        private static readonly string signalStr = "signal";

        enum SenderT : int {
            GUI = 0, Component = 1, Resource = 2
        };

        // log
        // # global
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate void LogMessageCB(string message);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate void LogWarningCB(string warningMessage);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate void LogErrorCB(string errorMessage);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate void StrackTraceCB(string stackTraceMessage);
        // # per type
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate void LogMessageIdCB(string message, int sType, int sKey);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate void LogWarningIdCB(string warningMessage, int sType, int sKey);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate void LogErrorIdCB(string errorMessage, int sType, int sKey);

        // component
        // # get
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate int GetCB(string componentName);
        // # states
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate int IsVisibleCB(int key);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate int IsUpdatingCB(int key);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate int IsClosedCB(int key);
        // # signals
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate void SignalBoolCB(int key, int index, int value);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate void SignalIntCB(int key, int index, int value);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate void SignalFloatCB(int key, int index, float value);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate void SignalDoubleCB(int key, int index, double value);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        delegate void SignalStringCB(int key, int index, string value);
        // times
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate long EllapsedTimeExpMsCB();
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate long EllapsedTimeRoutineMsCB();
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]

        // commands
        private delegate void NextCB();
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate void PreviousCB();
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate void CloseCB(int key);

        private static LogMessageCB logMessageCB = null;
        private static LogWarningCB logWarningCB = null;
        private static LogErrorCB logErrorCB = null;
        private static LogMessageIdCB logMessageIdCB = null;
        private static LogWarningIdCB logWarningIdCB = null;
        private static LogErrorIdCB logErrorIdCB = null;
        private static StrackTraceCB stackTraceCB = null;
        private static EllapsedTimeExpMsCB ellapsedTimeExpMsCB = null;
        private static EllapsedTimeRoutineMsCB ellapsedTimeRoutineMsCB = null;
        private static GetCB getCB = null;
        private static CloseCB closeCB = null;
        private static NextCB nextCB = null;
        private static PreviousCB previousCB = null;
        private static IsVisibleCB isVisibleCB = null;
        private static IsUpdatingCB isUpdatingCB = null;
        private static IsClosedCB isClosedCB = null;
        private static SignalBoolCB signalBoolCB = null;
        private static SignalIntCB signalIntCB = null;
        private static SignalFloatCB signalFloatCB = null;
        private static SignalDoubleCB signalDoubleCB = null;
        private static SignalStringCB signalStringCB = null;


        // dll management
        protected override void create_DLL_class() {
            _handle = new HandleRef(this, create_ex_experiment());
            init_logger();
            init_callbacks();
        }
        protected override void delete_DLL_class() { delete_ex_experiment(_handle); }

        private void init_callbacks() {

            stackTraceCB = (trace) => {
                ExVR.ExpLog().push_to_strackTrace(new ComponentTrace(trace));
            };

            logMessageCB = (message) => {
                ExVR.Log().message(message, false, "", "", 0, true, false); 
            };
            logMessageIdCB = (message, sType, sKey) => {
                switch ((SenderT)sType) {
                    case SenderT.Component:
                        if (ExVR.Components().componentPerKey.ContainsKey(sKey)) {
                            ExVR.Components().componentPerKey[sKey].log_message(
                                message : message, 
                                verbose : true,
                                logger  : true, 
                                append  : false
                            );
                        } else {
                            ExVR.Log().error(
                                error        : string.Format("[DLL] Receive message from invalid component key {0} : {1}", sKey, message),
                                addExtraInfo : true,
                                logger       : true,
                                append       : false
                            );
                        }
                        break;
                }
            };

            logWarningCB = (message) => { 
                ExVR.Log().warning(message, false, "", "", 0, true, false);
            };
            logWarningIdCB = (message, sType, sKey) => {

                switch ((SenderT)sType) {
                    case SenderT.Component:
                        if (ExVR.Components().componentPerKey.ContainsKey(sKey)) {
                            ExVR.Components().componentPerKey[sKey].log_warning(
                                warning : message,
                                verbose : true,
                                logger  : true,
                                append  : false
                            );
                        } else {
                            ExVR.Log().error(string.Format("[DLL] Receive warning from invalid component key {0} : {1}", sKey, message), true, "", "", 0, true, false);
                        }
                        break;
                }
            };

            logErrorCB = (message) => {
                ExVR.Log().error(message, false, "", "", 0, true, false);
            };
            logErrorIdCB = (message, sType, sKey) => {

                switch ((SenderT)sType) {
                    case SenderT.Component:
                        if (ExVR.Components().componentPerKey.ContainsKey(sKey)) {
                            ExVR.Components().componentPerKey[sKey].log_error(
                                error   : message,
                                verbose : true,
                                logger  : true,
                                append  : false
                            );
                        } else {
                            ExVR.Log().error(string.Format("[DLL] Receive error from invalid component key {0} : {1}", sKey, message));
                        }
                        break;
                }
            };

            ellapsedTimeExpMsCB = () => {
                return (int)ExVR.Time().ellapsed_exp_ms();
            };
            ellapsedTimeRoutineMsCB = () => {
                return (int)ExVR.Time().ellapsed_element_ms();
            };
            getCB = (string componentName) => {
                return ExVR.Components().get_from_name(componentName).key;
            };

            isVisibleCB = (key) => {
                var c = ExVR.Components().get_from_key(key);
                if (c != null) {
                    return c.is_visible() ? 1 : 0;
                }
                return 0;
            };
            isUpdatingCB = (key) => {
                var c = ExVR.Components().get_from_key(key);
                if (c != null) {
                    return c.is_updating() ? 1 : 0;
                }
                return 0;
            };
            isClosedCB = (key) => {
                var c = ExVR.Components().get_from_key(key);
                if (c != null) {
                    return c.is_closed() ? 1 : 0;
                }
                return 0;
            };

            nextCB = () => {
                ExVR.Events().command.next();
            };
            previousCB = () => {
                ExVR.Events().command.previous();
            };
            closeCB = (key) => {
                var c = ExVR.Components().get_from_key(key);
                if (c != null) {
                    c.close();
                }
            };

            signalBoolCB = (key, index, value) => {
                var c = ExVR.Components().get_from_key(key);
                if (c != null) {
                    c.invoke_signal(string.Format(bracesStr, signalStr, Converter.to_int(index + 1)), (value == 1));
                }
            };
            signalIntCB = (key, index, value) => {
                var c = ExVR.Components().get_from_key(key);
                if (c != null) {
                    c.invoke_signal(string.Format(bracesStr, signalStr, Converter.to_int(index + 1)), value);
                }
            };
            signalFloatCB = (key, index, value) => {
                var c = ExVR.Components().get_from_key(key);
                if (c != null) {
                    c.invoke_signal(string.Format(bracesStr, signalStr, Converter.to_int(index + 1)), value);
                }
            };
            signalDoubleCB = (key, index, value) => {
                var c = ExVR.Components().get_from_key(key);
                if (c != null) {
                    c.invoke_signal(string.Format(bracesStr, signalStr, Converter.to_int(index + 1)), value);
                }
            };
            signalStringCB = (key, index, value) => {
                var c = ExVR.Components().get_from_key(key);
                if (c != null) {
                    c.invoke_signal(string.Format(bracesStr, signalStr, Converter.to_int(index + 1)), value);
                }
            };

            init_callbacks_ex_experiment(
                _handle,
                logMessageCB, logWarningCB, logErrorCB,
                logMessageIdCB, logWarningIdCB, logErrorIdCB,
                stackTraceCB,
                ellapsedTimeExpMsCB, ellapsedTimeRoutineMsCB,
                getCB,
                isVisibleCB, isUpdatingCB, isClosedCB,
                nextCB, previousCB, closeCB,
                signalBoolCB, signalIntCB, signalFloatCB, signalDoubleCB, signalStringCB
            );

            //test_call_backs_ex_experiment(_handle);
            //var l = new HandleRef(this, get_logger_ptr_ex_experiment(_handle));
            //test_log_ex_experiment(l);
        }

        private bool init_logger(string pathDir = "", string loggerFileName = "") {
            if(pathDir.Length == 0 || loggerFileName.Length == 0) {
                init_logger_no_file_ex_experiment(_handle);
            } else {
                return init_logger_ex_experiment(_handle, pathDir, loggerFileName) == 1;
            }
            return true;
        }

        // DLL functions 

        [DllImport("exvr-export", EntryPoint = "create_ex_experiment", CallingConvention = CallingConvention.Cdecl)]
        static private extern IntPtr create_ex_experiment();

        [DllImport("exvr-export", EntryPoint = "delete_ex_experiment", CallingConvention = CallingConvention.Cdecl)]
        static public extern void delete_ex_experiment(HandleRef exExperiment);


        [DllImport("exvr-export", EntryPoint = "init_logger_ex_experiment", CallingConvention = CallingConvention.Cdecl)]
        static private extern int init_logger_ex_experiment(HandleRef exExperiment, string pathDir, string loggerFileName);

        [DllImport("exvr-export", EntryPoint = "init_logger_no_file_ex_experiment", CallingConvention = CallingConvention.Cdecl)]
        static private extern void init_logger_no_file_ex_experiment(HandleRef exExperiment);

        [DllImport("exvr-export", EntryPoint = "init_callbacks_ex_experiment", CallingConvention = CallingConvention.Cdecl)]
        static private extern void init_callbacks_ex_experiment(
            HandleRef exExperiment,
            [MarshalAs(UnmanagedType.FunctionPtr)] LogMessageCB logMessageCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] LogWarningCB logWarningCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] LogErrorCB logErrorCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] LogMessageIdCB logMessageIdCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] LogWarningIdCB logWarningIdCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] LogErrorIdCB logErrorIdCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] StrackTraceCB strackTraceCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] EllapsedTimeExpMsCB ellapsedTimeExpMsCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] EllapsedTimeRoutineMsCB ellapsedTimeRoutineMsCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] GetCB getCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] IsVisibleCB isVisibleCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] IsUpdatingCB isUpdatingCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] IsClosedCB isClosedCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] NextCB nextCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] PreviousCB previousCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] CloseCB closeCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] SignalBoolCB signalBoolCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] SignalIntCB signalIntCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] SignalFloatCB signalFloatCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] SignalDoubleCB signalDoubleCB,
            [MarshalAs(UnmanagedType.FunctionPtr)] SignalStringCB signalStringCB
        );

        [DllImport("exvr-export", EntryPoint = "test_call_backs_ex_experiment", CallingConvention = CallingConvention.Cdecl)]
        static public extern void test_call_backs_ex_experiment(HandleRef exExperiment);

        [DllImport("exvr-export", EntryPoint = "test_log_ex_experiment", CallingConvention = CallingConvention.Cdecl)]
        static public extern void test_log_ex_experiment(HandleRef logger);

        [DllImport("exvr-export", EntryPoint = "get_logger_ptr_ex_experiment", CallingConvention = CallingConvention.Cdecl)]
        static public extern IntPtr get_logger_ptr_ex_experiment(HandleRef exExperiment);
    }
}