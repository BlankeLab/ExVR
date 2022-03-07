
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
using System.Runtime.InteropServices;

// unity
using UnityEngine;

// SRanipal
using ViveSR.anipal.Eye;

namespace Ex {

    public class ViveProEyeTrackingComponent : ExComponent {

        public Vector3 gazeDirectionCombined = Vector3.zero;
        public float leftGazeOpeness = 0f, rightGazeOpeness = 0f;
        public float leftPupilDiameter = 0f, rightPupilDiameter = 0f;
        public Vector2 leftPupilPos = Vector2.zero, rightPupilPos = Vector2.zero;

        private static EyeData eyeData = new EyeData();

        static private readonly string gazeDirectionSignal = "gaze direction";
        static private readonly string eyeOpenessSignal    = "eye openess";
        static private readonly string pupilPositionSignal = "pupil position";
        static private readonly string pupilDiameterSignal = "pupil diameter";

        #region ex_functions

        protected override bool initialize() {

            add_signal(gazeDirectionSignal);
            add_signal(eyeOpenessSignal);
            add_signal(pupilPositionSignal);
            add_signal(pupilDiameterSignal);

            gameObject.AddComponent<SRanipal_Eye_Framework>();
            SRanipal_Eye_Framework.Instance.EnableEye             = true;
            SRanipal_Eye_Framework.Instance.EnableEyeDataCallback = true;            
            
            return
                (SRanipal_Eye_Framework.Status != SRanipal_Eye_Framework.FrameworkStatus.WORKING) &&
                (SRanipal_Eye_Framework.Status != SRanipal_Eye_Framework.FrameworkStatus.NOT_SUPPORT);
        }

        protected override void start_experiment() {
            SRanipal_Eye.WrapperRegisterEyeDataCallback(Marshal.GetFunctionPointerForDelegate((SRanipal_Eye.CallbackBasic)eye_call_back));
        }

        protected override void stop_experiment() {
            SRanipal_Eye.WrapperUnRegisterEyeDataCallback(Marshal.GetFunctionPointerForDelegate((SRanipal_Eye.CallbackBasic)eye_call_back));
        }

        protected override void post_update() {


            // openness
            if (SRanipal_Eye.GetEyeOpenness(EyeIndex.LEFT, out leftGazeOpeness, eyeData)) {
                invoke_signal(eyeOpenessSignal, new IdAny(0, leftGazeOpeness));
            }            
            if (SRanipal_Eye.GetEyeOpenness(EyeIndex.RIGHT, out rightGazeOpeness, eyeData)) {
                invoke_signal(eyeOpenessSignal, new IdAny(1, rightGazeOpeness));
            }

            // pupil             
            if (SRanipal_Eye.GetPupilPosition(EyeIndex.LEFT, out leftPupilPos, eyeData)) {
                invoke_signal(pupilPositionSignal, new IdAny(0, leftPupilPos));
            }
            if (SRanipal_Eye.GetPupilPosition(EyeIndex.RIGHT, out rightPupilPos, eyeData)) {
                invoke_signal(pupilPositionSignal, new IdAny(1, rightPupilPos));
            }

            // diameter
            if (get_pupil_diameter(EyeIndex.LEFT, out leftPupilDiameter, eyeData)) {
                invoke_signal(pupilDiameterSignal, new IdAny(0, leftPupilDiameter));
            }
            if (get_pupil_diameter(EyeIndex.RIGHT, out rightPupilDiameter, eyeData)) {
                invoke_signal(pupilDiameterSignal, new IdAny(1, rightPupilDiameter));
            }

            // gaze
            Vector3 gazeOriginCombinedLocal = Vector3.zero, gazeDirectionCombinedLocal = Vector3.zero;
            if (!SRanipal_Eye.GetGazeRay(GazeIndex.COMBINE, out gazeOriginCombinedLocal, out gazeDirectionCombinedLocal, eyeData)) {
                if (!SRanipal_Eye.GetGazeRay(GazeIndex.LEFT, out gazeOriginCombinedLocal, out gazeDirectionCombinedLocal, eyeData)) {
                    if (!SRanipal_Eye.GetGazeRay(GazeIndex.RIGHT, out gazeOriginCombinedLocal, out gazeDirectionCombinedLocal, eyeData)) {
                        return;
                    }
                }
            }

            if (gazeOriginCombinedLocal.magnitude > 0 && gazeDirectionCombinedLocal.magnitude > 0) {
                var cam = ExVR.Display().cameras().bothEyesCamera;
                gazeDirectionCombined = cam.transform.TransformDirection(gazeDirectionCombinedLocal);
                invoke_signal(gazeDirectionSignal, gazeDirectionCombined);
            }
        }


        #endregion
        #region private_functions

        private static void eye_call_back(ref EyeData eye_data) {
            eyeData = eye_data;
        }

        public static bool get_pupil_diameter(EyeIndex eyeIndex, out float diameter, EyeData eyeData) {

            bool valid = false;
            if (SRanipal_Eye_Framework.Status == SRanipal_Eye_Framework.FrameworkStatus.WORKING) {
                SingleEyeData data = (eyeIndex == EyeIndex.LEFT) ? eyeData.verbose_data.left : eyeData.verbose_data.right;
                valid = data.GetValidity(SingleEyeDataValidity.SINGLE_EYE_DATA_PUPIL_DIAMETER_VALIDITY);                
                diameter = valid ? diameter = data.pupil_diameter_mm : 0f;

            } else {
                // If not support eye tracking, set default in middle.
                diameter = 0f;
                valid = true;
            }
            return valid;
        }


        #endregion
        #region public_functions
        #endregion
    }
}
