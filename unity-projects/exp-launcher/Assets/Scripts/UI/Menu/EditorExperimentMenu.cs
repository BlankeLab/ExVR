
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
using System.Diagnostics;

// unity
using UnityEngine;

#if UNITY_EDITOR

// unity
using UnityEditor;

namespace Ex{

    class SendMessage {
        [DllImport("user32.dll")]
        public static extern bool PostMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);


        public static void send_space_key() {
            const uint WM_KEYDOWN = 0x100;
            const uint WM_KEYUP = 0x0101;
            const uint KeySpace = 0x20;

            int repeatCount = 1;// 0-15 Specifies the repeat count. The value is the number of times the keystroke is repeated as a result of the user 
                                // holding down the key. The repeat count is always one for a WM_KEYUP message.
            int scanCode = 16;  // 16-23 Specifies the scan code. The value depends on the original equipment manufacturer (OEM).
            int extended = 0;   // 24 Specifies whether the key is an extended key, such as the right-hand ALT and CTRL keys that appear on an
                                // enhanced 101- or 102-key keyboard. The value is 1 if it is an extended key; otherwise, it is 0.
            int contextCode = 0; // Specifies the context code. The value is always 0 for a WM_KEYUP message.
            int previousState = 1; // Specifies the previous key state. The value is always 1 for a WM_KEYUP message.;
            int transitionState = 1; // 31 Specifies the transition state. The value is always 1 for a WM_KEYUP message. Return Value

            uint lParam = (uint)repeatCount;
            lParam += (uint)(scanCode * 0x10000);
            lParam += (uint)((extended) * 0x1000000);
            lParam += (uint)((contextCode * 2) * 0x10000000);
            lParam += (uint)((previousState * 4) * 0x10000000);
            lParam += (uint)((transitionState * 8) * 0x10000000);


            var p = Process.GetCurrentProcess();
            IntPtr edit = p.MainWindowHandle;
            UnityEngine.Debug.LogError("send key " + p.ProcessName);
            bool res = PostMessage(edit, WM_KEYDOWN, (IntPtr)(KeySpace), (IntPtr)lParam);
            if (res) {
                UnityEngine.Debug.LogError("OK");
            }
            res = PostMessage(edit, WM_KEYUP, (IntPtr)(KeySpace), IntPtr.Zero);
            if (res) {
                UnityEngine.Debug.LogError("OK");
            }

            //IntPtr hWnd;
            //string processName = "putty";

            //Process[] processList = Process.GetProcesses();

            //foreach (Process P in processList) {


            //    //PostMessage(edit, WM_KEYUP, (IntPtr)(KeySpace), IntPtr.Zero);
            //    //PostMessage(edit, WM_KEYUP, (IntPtr)(Keys.Control), IntPtr.Zero);

            //    //if (P.ProcessName.Equals(processName)) {
            //    //    IntPtr edit = P.MainWindowHandle;
            //    //    PostMessage(edit, WM_KEYDOWN, (IntPtr)(Keys.Control), IntPtr.Zero);
            //    //    PostMessage(edit, WM_KEYDOWN, (IntPtr)(Keys.A), IntPtr.Zero);
            //    //    PostMessage(edit, WM_KEYUP,   (IntPtr)(Keys.Control), IntPtr.Zero);
            //    //}
            //}
        }

    }


    public class EditorExperimentMenu {

        [MenuItem("Experiment/Load")]
        static void load_experiment() {

            string instanceFilePath;
            if (System.IO.File.Exists(ExVR.Paths().currentInstanceFile)) {
                instanceFilePath = ExVR.Paths().currentInstanceFile;
            } else {
                ExVR.Log().warning("No last loaded instance file found, will try to load default instance file instead.");
                instanceFilePath = ExVR.Paths().designerDefaultInstanceFile;
            }

            ExVR.Network().send_command_from_exp_launcher(NetworkManager.Command.Load, string.Format("{0}|{1}", ExVR.Paths().designerTempExpFile, instanceFilePath));
        }
        [MenuItem("Experiment/Start")]
        static void start_experiment() {
            ExVR.Network().send_command_from_exp_launcher(NetworkManager.Command.Play);
        }

        [MenuItem("Experiment/Stop")]
        static void stop_experiment() {
            ExVR.Network().send_command_from_exp_launcher(NetworkManager.Command.Stop);
        }

        [MenuItem("Experiment/Pause")]
        static void pause_experiment() {
            ExVR.Network().send_command_from_exp_launcher(NetworkManager.Command.Pause);
        }

        [MenuItem("Experiment/Next")]
        static void next() {
            ExVR.Network().send_command_from_exp_launcher(NetworkManager.Command.Next);
        }

        [MenuItem("Experiment/Previous")]
        static void previous() {
            ExVR.Network().send_command_from_exp_launcher(NetworkManager.Command.Previous);
        }

        [MenuItem("Experiment/Quit")]
        static void quit() {
            ExVR.Network().send_command_from_exp_launcher(NetworkManager.Command.Quit);

        }

        [MenuItem("Experiment/Look calibration at 0")]
        static void look_calibration_at_0() {
            var target = new Vector3(0, 0, 0);
            var cd = CameraUtility.calibration_forward();
            var cp = (target - CameraUtility.calibration_position()).normalized;
            CameraUtility.rotate_camera_rig_around(CameraUtility.calibration_position(), Vector3.up, Vector3.SignedAngle(cd, cp, Vector3.up));
        }

        [MenuItem("Experiment/Simulate key press")]
        static void simulate_key_press() {
            SendMessage.send_space_key();
        }
    }
}

#endif
