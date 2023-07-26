

/************************************************************************************
** K4DisplayStandAloneSample                                                       **
**                                                                                 **
** MIT License                                                                     **
**                                                                                 **
** Copyright (c) 2023 Florian Lance                                                **
**                                                                                 **
** Permission is hereby granted, free of charge, to any person obtaining a copy    **
** of this software and associated documentation files (the "Software"), to deal   **
** in the Software without restriction, including without limitation the rights    **
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       **
** copies of the Software, and to permit persons to whom the Software is           **
** furnished to do so, subject to the following conditions:                        **
**                                                                                 **
** The above copyright notice and this permission notice shall be included in all  **
** copies or substantial portions of the Software.                                 **
**                                                                                 **
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      **
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        **
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     **
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          **
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   **
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   **
** SOFTWARE.                                                                       **
*************************************************************************************/


// system
using System;
using System.Runtime.InteropServices;

// unity
using UnityEngine;

namespace SA {

    public class DLLExperimentSA : DLLCppImportSA {

        static public DLLExperimentSA global = null;

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
        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        private delegate void PauseEditorCB();
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
        private static PauseEditorCB pauseEditorCB = null;
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

            stackTraceCB = (trace) => { };
            pauseEditorCB = () => { };
            logMessageCB = (message) => {
                Debug.Log(message);
            };
            logMessageIdCB = (message, sType, sKey) => { };
            logWarningCB = (message) => {
                Debug.LogWarning(message);
            };
            logWarningIdCB = (message, sType, sKey) => { };
            logErrorCB = (message) => {
                Debug.LogError(message);
            };

            logErrorIdCB = (message, sType, sKey) => { };

            ellapsedTimeExpMsCB = () => {
                //return (int)ExVR.Time().ellapsed_exp_ms();
                return 0;
            };
            ellapsedTimeRoutineMsCB = () => {
                //return (int)ExVR.Time().ellapsed_element_ms();
                return 0;
            };
            getCB = (string componentName) => {
                return 0;
            };
            isVisibleCB = (key) => {
                return 1;
            };
            isUpdatingCB = (key) => {
                return 1;
            };
            isClosedCB = (key) => {
                return 0;
            };
            nextCB = () => { };
            previousCB = () => { };
            closeCB = (key) => { };
            signalBoolCB = (key, index, value) => { };
            signalIntCB = (key, index, value) => { };
            signalFloatCB = (key, index, value) => { };
            signalDoubleCB = (key, index, value) => { };
            signalStringCB = (key, index, value) => { };

            init_callbacks_ex_experiment(
                _handle,
                logMessageCB, logWarningCB, logErrorCB,
                logMessageIdCB, logWarningIdCB, logErrorIdCB,
                stackTraceCB, pauseEditorCB,
                ellapsedTimeExpMsCB, ellapsedTimeRoutineMsCB,
                getCB,
                isVisibleCB, isUpdatingCB, isClosedCB,
                nextCB, previousCB, closeCB,
                signalBoolCB, signalIntCB, signalFloatCB, signalDoubleCB, signalStringCB
            );
        }

        private bool init_logger(string pathDir = "", string loggerFileName = "") {
            if (pathDir.Length == 0 || loggerFileName.Length == 0) {
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
            [MarshalAs(UnmanagedType.FunctionPtr)] PauseEditorCB pauseEditorCB,
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