
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

// unity
using UnityEngine;

namespace Ex{


    public class CamerasManager : MonoBehaviour {

        // camera rig
        public Transform cameraRig = null;
        // cameras
        public Camera bothEyesCamera = null;
        public Camera leftEyeCamera = null;
        public Camera rightEyeCamera = null;
        public Camera leftEyeStereoCamera = null;
        public Camera rightEyeStereoCamera = null;
        // transforms
        public Transform calibration = null;
        public Transform eyeCameraRelative = null;

        private Vector3 startExperimentCalibrationPosition = Vector3.zero;
        private Quaternion startExperimentCalibrationRotation = Quaternion.identity;

        public void reset() {


            //Valve.VR.OpenVR.System.ResetSeatedZeroPose();
            //Valve.VR.OpenVR.Compositor.SetTrackingSpace(Valve.VR.ETrackingUniverseOrigin.TrackingUniverseRawAndUncalibrated);
            //Valve.VR.ETrackingUniverseOrigin.TrackingUniverseSeated);

            // reset camera rig
            set_camera_rig_transform(Vector3.zero, Quaternion.identity);

            var camerasTr = get_camera_rig_transform().Find("Cameras");
            camerasTr.rotation = Quaternion.identity;
            camerasTr.position = Vector3.zero;

            // reset calibration
            reset_calibration_from_eye_camera(
                ExVR.GuiSettings().useCameraXAxixAsNeutral, 
                ExVR.GuiSettings().useCameraYAxixAsNeutral, 
                ExVR.GuiSettings().useCameraZAxixAsNeutral
            );

            // store initial calibration
            startExperimentCalibrationPosition = calibration.localPosition;
            startExperimentCalibrationRotation = calibration.localRotation;

            // update relative
            update_eye_camera_relative();
        }

        // transform
        public Transform get_calibration_transform() {
            return calibration;
        }

        public Transform get_eye_camera_transform() {            
            return bothEyesCamera.transform;
        }

        public Transform get_eye_camera_relative_transform() {
            return eyeCameraRelative;
        }

        public Transform get_camera_rig_transform() {
            return cameraRig;
        }

        // camera
        public Camera get_eye_camera() {
            return bothEyesCamera;
        }

        // camera rig
        public void apply_vector_to_camera_rig_position(Vector3 vector) {
            cameraRig.position += vector;
        }

        public void set_camera_rig_position(Vector3 position) {
            cameraRig.position = position;
        }
        public void set_camera_rig_rotation(Quaternion rotation) {
            cameraRig.rotation = rotation;
        }

        public void set_camera_rig_transform(Vector3 worldPosition, Quaternion worldRotation) {
            var rig = get_camera_rig_transform();
            rig.rotation = worldRotation;
            rig.position = worldPosition;
        }

        // calibration
        public void restore_start_experiment_calibration() {
            set_camera_rig_transform(Vector3.zero, Quaternion.identity);
            calibration.localPosition = startExperimentCalibrationPosition;
            calibration.localRotation = startExperimentCalibrationRotation;
        }

        public void reset_calibration_from_eye_camera(bool useX, bool useY, bool useZ) {

            calibration.localPosition = bothEyesCamera.transform.localPosition;
            var angles = bothEyesCamera.transform.localEulerAngles;
            calibration.localEulerAngles = new Vector3(
                useX ? angles.x : 0f,
                useY ? angles.y : 0f,
                useZ ? angles.z : 0f
            );
        }

        public void set_calibration(Vector3 position, Quaternion rotation){
            calibration.position = position;
            calibration.rotation = rotation;
        }

        // move/rotate/set target
        public void move_target_horizontally_by_modifying_camera_rig(Transform target, float amount) {
            apply_vector_to_camera_rig_position(target.right.normalized * amount);
        }
        public void move_target_vertically_by_modifying_camera_rig(Transform target, float amount) {
            apply_vector_to_camera_rig_position(target.up.normalized * amount);
        }
        public void move_target_forward_by_modifying_camera_rig(Transform target, float amount) {
            apply_vector_to_camera_rig_position(target.forward * amount);
        }
        private void move_target_backward_by_modifying_camera_rig(Transform target, float amount) {
            apply_vector_to_camera_rig_position(-target.forward * amount);
        }
        private void rotate_target_by_modifying_camera_rig(Transform target, Quaternion rotation) {

            // !!!  use only with targets from camera rig hierarchy !!!
            var targetWorldPosition = target.position;
            set_camera_rig_rotation((target.rotation * rotation) * Quaternion.Inverse(target.localRotation));
            apply_vector_to_camera_rig_position(targetWorldPosition - target.position);
        }
        private void set_target_rotation_by_modifying_camera_rig(Transform target, Quaternion worldRotation) {

            // !!!  use only with targets from camera rig hierarchy !!!
            var targetWorldPosition = target.position;
            set_camera_rig_rotation(worldRotation* Quaternion.Inverse(target.localRotation));
            apply_vector_to_camera_rig_position(targetWorldPosition - target.position);
        }

        // move/rotate/set calibration
        public void move_calibration_horizontally_by_modifying_camera_rig(float amount) {
            move_target_horizontally_by_modifying_camera_rig(get_calibration_transform(), amount);
        }
        public void move_calibration_vertically_by_modifying_camera_rig(float amount) {
            move_target_vertically_by_modifying_camera_rig(get_eye_camera_transform(), amount);
        }
        public void move_calibration_forward_by_modifying_camera_rig(float amount) {
            move_target_forward_by_modifying_camera_rig(get_calibration_transform(), amount);
        }
        public void move_eye_camera_horizontally_by_modifying_camera_rig(float amount) {
            move_target_horizontally_by_modifying_camera_rig(get_eye_camera_transform(), amount);
        }
        public void move_calibration_backward_by_modifying_camera_rig(float amount) {
            move_target_backward_by_modifying_camera_rig(get_calibration_transform(), amount);
        }
        public void translate_calibration_referential_by_modifying_camera_rig(Vector3 vector) {
            move_calibration_forward_by_modifying_camera_rig(vector.x);
            move_calibration_vertically_by_modifying_camera_rig(vector.y);
            move_calibration_horizontally_by_modifying_camera_rig(vector.z);
        }
        public void rotate_calibration_by_modifying_camera_rig(Quaternion rotation) {
            rotate_target_by_modifying_camera_rig(get_calibration_transform(), rotation);
        }
        public void set_calibration_position_by_modifying_camera_rig(Vector3 worldPosition) {
            apply_vector_to_camera_rig_position(worldPosition - get_calibration_transform().position);
        }
        public void set_calibration_rotation_by_modifying_camera_rig(Quaternion worldRotation) {
            set_target_rotation_by_modifying_camera_rig(get_calibration_transform(), worldRotation);
        }
        public void set_calibration_transform_by_modifying_camera_rig(Vector3 worldPosition, Quaternion worldRotation) {

            var target = get_calibration_transform();
            set_camera_rig_rotation(worldRotation* Quaternion.Inverse(target.localRotation));
            apply_vector_to_camera_rig_position(worldPosition - target.position);
        }

        // move/rotate/set eye camera
        public void move_eye_camera_vertically_by_modifying_camera_rig(float amount) {
            move_target_vertically_by_modifying_camera_rig(get_calibration_transform(), amount);
        }
        public void move_eye_camera_forward_by_modifying_camera_rig(float amount) {
            move_target_forward_by_modifying_camera_rig(get_eye_camera_transform(), amount);
        }
        public void move_eye_camera_backward_by_modifying_camera_rig(float amount) {
            move_target_backward_by_modifying_camera_rig(get_eye_camera_transform(), amount);
        }
        public void translate_eye_camera_referential_by_modifying_camera_rig(Vector3 vector) {
            move_eye_camera_forward_by_modifying_camera_rig(vector.x);
            move_eye_camera_vertically_by_modifying_camera_rig(vector.y);
            move_eye_camera_horizontally_by_modifying_camera_rig(vector.z);
        }
        public void rotate_eye_camera_by_modifying_camera_rig(Quaternion rotation) {
            rotate_target_by_modifying_camera_rig(get_eye_camera_transform(), rotation);
        }
        public void set_eye_camera_position_by_modifying_camera_rig(Vector3 worldPosition) {
            apply_vector_to_camera_rig_position(worldPosition - get_eye_camera_transform().position);
        }
        public void set_eye_camera_rotation_by_modifying_camera_rig(Quaternion worldRotation) {
            set_target_rotation_by_modifying_camera_rig(get_eye_camera_transform(), worldRotation);
        }
        public void set_eye_camera_transform_by_modifying_camera_rig(Vector3 worldPosition, Quaternion worldRotation) {
            var target = get_eye_camera_transform();
            set_camera_rig_rotation(worldRotation* Quaternion.Inverse(target.localRotation));
            apply_vector_to_camera_rig_position(worldPosition - target.position);
        }

        // screen
        public bool mouse_on_screen() {
            var view = bothEyesCamera.ScreenToViewportPoint(UnityEngine.Input.mousePosition);
            return view.x < 0 || view.x > 1 || view.y < 0 || view.y > 1;
        }

        private void update_eye_camera_relative() {
            if (eyeCameraRelative != null) {
                var tr = get_eye_camera_transform();
                eyeCameraRelative.localPosition    = tr.localPosition - calibration.localPosition;
                eyeCameraRelative.localEulerAngles = tr.localEulerAngles - calibration.localEulerAngles;
            }
        }
        public void Update() {
            update_eye_camera_relative();
        }
    }
}