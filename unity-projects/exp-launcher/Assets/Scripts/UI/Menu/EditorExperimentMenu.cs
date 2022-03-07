
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

using UnityEngine;

#if UNITY_EDITOR

// unity
using UnityEditor;

namespace Ex{


    public class EditorExperimentMenu{

        [MenuItem("Experiment/Load")]
        static void load_experiment() {

            string instanceFilePath;
            if (System.IO.File.Exists(ExVR.Paths().lastLoadedInstanceFile)) {
                instanceFilePath = ExVR.Paths().lastLoadedInstanceFile;
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
            var target = new Vector3(0,0,0);
            var cd = CameraUtility.calibration_forward();
            var cp = (target - CameraUtility.calibration_position()).normalized;
            CameraUtility.rotate_camera_rig_around(CameraUtility.calibration_position(), Vector3.up, Vector3.SignedAngle(cd,cp, Vector3.up));
        }
    }
}

#endif
