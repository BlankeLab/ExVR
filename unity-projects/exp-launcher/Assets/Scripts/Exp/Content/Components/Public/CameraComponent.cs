
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

// unity
using UnityEngine;

namespace Ex{

    public class CameraComponent : ExComponent{

        private static readonly string eyeCamSignal = "eye cam";
        private static readonly string neutralCamSignal = "neutral cam";
        private static readonly string camerasInfos = "cameras_info";
        private static readonly string infosFormat = "{0} {1} {2} {3}";

        private static readonly string mouseX = "Mouse X";
        private static readonly string mouseY = "Mouse Y";
        private static readonly string mouseScrollWheel = "Mouse ScrollWheel";

        private static readonly string debugCameraP = "debug_camera";
        private static readonly string useNeutralP = "use_neutral";

        private Vector3 initConfigPosition = Vector3.zero;
        private Vector3 initConfigRotation = Vector3.zero;

        private Vector3 currentConfigPosition = Vector3.zero;
        private Vector3 currentConfigRotation = Vector3.zero;


#region ex_functions

        protected override bool initialize() {

            add_slot("set eye cam", (tr) => {
                var eyeTr = (TransformValue)tr;
                CameraUtility.set_eye_camera_transform(eyeTr.position, eyeTr.rotation);
            });
            add_slot("set neutral cam", (tr) => {
                var startNeuralTr = (TransformValue)tr;
                CameraUtility.set_calibration_transform(startNeuralTr.position, startNeuralTr.rotation);
            });
            add_signal(eyeCamSignal);
            add_signal(neutralCamSignal);
           
            initConfigPosition = initC.get_vector3("position");
            initConfigRotation = initC.get_vector3("rotation");

            return true;
        }

        protected override void start_experiment() {
            if (initC.get<bool>("start_experiment")) {
                apply_init_config_camera();
            }
        }

        public override void update_from_current_config() {
            currentConfigPosition = currentC.get_vector3("position");
            currentConfigRotation = currentC.get_vector3("rotation");
        }

        protected override void start_routine() {

            if (currentC.get<bool>("start_routine")) {
                apply_current_config_camera();
            }
        }

        protected override void update() {

            // update debug camera
            update_debug_camera_from_mouse_inputs();

            // infos
            send_infos_to_ui();

            // signals
            invoke_signal(eyeCamSignal,     Converter.to_transform_value(ExVR.Display().cameras().get_eye_camera_transform()));
            invoke_signal(neutralCamSignal, Converter.to_transform_value(ExVR.Display().cameras().get_calibration_transform()));
        }

        protected override void update_parameter_from_gui(string updatedArgName) {

            update_from_current_config();

            if (updatedArgName == "position" || updatedArgName == "rotation") {
                apply_current_config_camera();
            }
        }

        protected override void set_update_state(bool doUpdate) {

            if (doUpdate) {
                if (currentC.get<bool>("update_on")) {
                    apply_current_config_camera();
                }
            }
        }

#endregion
#region private_functions

        private void apply_init_config_camera() {
            if (initC.get<bool>(useNeutralP)) {
                CameraUtility.set_calibration_transform(initConfigPosition, initConfigRotation);
            } else {
                CameraUtility.set_eye_camera_transform(initConfigPosition, initConfigRotation);
            }
        }

        private void apply_current_config_camera() {
            if (currentC.get<bool>(useNeutralP)) {
                CameraUtility.set_calibration_transform(currentConfigPosition, currentConfigRotation);
            } else {
                CameraUtility.set_eye_camera_transform(currentConfigPosition, currentConfigRotation);
            }
        }

        private void update_debug_camera_from_mouse_inputs() {
            
            // check if mouse inside screen and if debug camera enabled
            var fpsCam = ExVR.GO().DebugEditorCamera.GetComponent<RuntimeDebugCamera>();
            if (!currentC.get<bool>(debugCameraP)){                
                return;
            }
            if (!fpsCam.is_mouse_outside()) {
                return;
            }

            bool useNeutral = currentC.get<bool>(useNeutralP);

            // retrieve inputs
            bool leftClick      = UnityEngine.Input.GetMouseButton(0);
            bool rightClick     = UnityEngine.Input.GetMouseButton(1);
            bool middleClick    = UnityEngine.Input.GetMouseButton(2);
            float nx            = UnityEngine.Input.GetAxis(mouseX);
            float ny            = UnityEngine.Input.GetAxis(mouseY);
            float scroll        = UnityEngine.Input.GetAxis(mouseScrollWheel);

            // apply movments to camera
            float rxSpeed = 2f;
            float rySpeed = 2f;
            float rzSpeed = 2f;

            // mouse scrolling
            if (scroll > 0f) { // forward
                if (useNeutral) {
                    CameraUtility.move_calibration_forward(0.02f);
                } else {
                    CameraUtility.move_eye_camera_forward(0.02f);
                }
            } else if (scroll < 0f) { // backward
                if (useNeutral) {
                    CameraUtility.move_calibration_backward(0.02f);
                } else {
                    CameraUtility.move_eye_camera_backward(0.02f);
                }
            }

            // mouve middle click
            if (middleClick) {

                // check horizontal right click mouse drag movement
                if (nx != 0) {
                    if (useNeutral) {
                        CameraUtility.move_calibration_horizontally(rxSpeed * nx * 0.02f);
                    } else {
                        CameraUtility.move_eye_camera_horizontally(rxSpeed * nx * 0.02f);
                    }
                }
                // check vertical right click mouse drag movement
                if (ny != 0) {
                    if (useNeutral) {
                        CameraUtility.move_calibration_vertically(rySpeed * ny * 0.02f);
                    } else {
                        CameraUtility.move_eye_camera_vertically(rySpeed * ny * 0.02f);
                    }
                }

            } else if (leftClick && rightClick) { // both left click and right click
                if (useNeutral) {
                    CameraUtility.rotate_calibration(Quaternion.Euler(0, 0, rzSpeed * nx));
                } else {
                    CameraUtility.rotate_eye_camera(Quaternion.Euler(0, 0, rzSpeed * nx));
                }

            } else if (rightClick) { // only right click
                if (useNeutral) {
                    CameraUtility.rotate_calibration(Quaternion.Euler(-rxSpeed * ny, rySpeed * nx, 0));
                } else {
                    CameraUtility.rotate_eye_camera(Quaternion.Euler(-rxSpeed * ny, rySpeed * nx, 0));
                }
            } else if (leftClick) { // only left click
                // ...
            }
        }

        private void send_infos_to_ui() {

            // get transforms
            var neutral = ExVR.Display().cameras().get_calibration_transform();
            var eye     = ExVR.Display().cameras().get_eye_camera_transform();

            // gui info
            send_infos_to_gui_current_config(
                camerasInfos,
                string.Format(
                    infosFormat,
                    Converter.to_string(neutral.position),
                    Converter.to_string(neutral.eulerAngles),
                    Converter.to_string(eye.position),
                    Converter.to_string(eye.eulerAngles)
                )
            );
        }
#endregion
#region public_functions
        public Vector3 init_config_position() {
            return initConfigPosition;
        }

        public Vector3 init_config_rotation() {
            return initConfigRotation;
        }

        public Vector3 current_config_position() {
            return currentConfigPosition;
        }

        public Vector3 current_config_rotation() {
            return currentConfigRotation;
        }
#endregion
    }

}