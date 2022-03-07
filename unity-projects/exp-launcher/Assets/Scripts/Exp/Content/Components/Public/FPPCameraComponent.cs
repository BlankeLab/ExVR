
/*******************************************************************************
** exvr-exp                                                                   **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

// system
using System.Diagnostics;

// unity
using UnityEngine;

namespace Ex {

    public class FPPCameraComponent : ExComponent {
        private Stopwatch m_snapTimer = new Stopwatch();

        private static readonly string camerasInfos = "cameras_info";
        private static readonly string infosFormat = "{0} {1} {2} {3}";

        private static readonly string useNeutralP = "use_neutral";

        #region ex_functions
        protected override bool initialize() {
            return true;
        }

        protected override void start_experiment() {
            if (!initC.get<bool>("init_transform_do_not_apply")) {
                var tr = initC.get_transform("init_transform");
                if (initC.get<bool>(useNeutralP)) {
                    CameraUtility.set_calibration_transform(tr.position, tr.rotation);
                } else {
                    CameraUtility.set_eye_camera_transform(tr.position, tr.rotation);
                }
            }
        }

        public override void update_from_current_config() {
            if (!currentC.get<bool>("transform_do_not_apply")) {
                var tr = currentC.get_transform("transform");
                if (currentC.get<bool>(useNeutralP)) {
                    CameraUtility.set_calibration_transform(tr.position, tr.rotation);
                } else {
                    CameraUtility.set_eye_camera_transform(tr.position, tr.rotation);
                }
            }
        }
        protected override void post_start_routine() {
            m_snapTimer.Start();
        }

        protected override void update() {

            // infos
            send_infos_to_ui();

            // signals
            //invoke_signal(eyeCamSignal, TransformValue.from_transform(ExVR.Display().cameras().get_eye_camera_transform()));
            //invoke_signal(neutralCamSignal, TransformValue.from_transform(ExVR.Display().cameras().get_calibration_transform()));
        }

        protected override void stop_routine() {
            m_snapTimer.Stop();
        }

        #endregion

        #region private_functions
        private void send_infos_to_ui() {

            // get transforms
            var neutral = ExVR.Display().cameras().get_calibration_transform();
            var eye = ExVR.Display().cameras().get_eye_camera_transform();

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

        // eye camera
        public Vector3 eye_position() {
            return CameraUtility.eye_camera_position();
        }

        public Vector3 eye_direction() {
            return CameraUtility.eye_camera_forward();
        }

        public Vector3 eye_euler_angles() {
            return CameraUtility.eye_camera_rotation().eulerAngles;
        }
        public Quaternion eye_rotation() {
            return CameraUtility.eye_camera_rotation();
        }

        public void move_camera_forward_xz(float value) {
            var d = eye_direction();
            CameraUtility.translate_camera_rig(new Vector3(d.x, 0, d.z * value));
        }

        public void rotate_camera_around_y(float value) {
            CameraUtility.rotate_camera_rig_around(eye_position(), Vector3.up, value);
        }
        public void rotate_camera_around_y_towards(Vector3 target) {
            var cd = eye_direction();
            var cp = (target - CameraUtility.eye_camera_position()).normalized;
            cp.y = cd.y;
            cp.Normalize();
            rotate_camera_around_y(Vector3.SignedAngle(cd, cp, Vector3.up));
        }

        // calibration
        public Vector3 calibration_position() {
            return CameraUtility.calibration_position();
        }

        public Vector3 calibration_direction() {
            return CameraUtility.calibration_forward();
        }

        public Vector3 calibration_euler_angles() {
            return CameraUtility.calibration_rotation().eulerAngles;
        }
        public Quaternion calibration_rotation() {
            return CameraUtility.calibration_rotation();
        }

        public void translate_calibration(Vector3 value) {
            CameraUtility.translate_camera_rig(value);
        }

        public void move_calibration_to(Vector3 position) {
            CameraUtility.translate_camera_rig(position - calibration_position());
        }
        public void rotate_calibration_around_y(float value) {
            CameraUtility.rotate_camera_rig_around(calibration_position(), Vector3.up, value);
        }
        public void snap_rotate_calibration_around_y(float value, int delayMS) {
            if (m_snapTimer.ElapsedMilliseconds > delayMS) {
                rotate_calibration_around_y(value);
                m_snapTimer.Restart();
            }
        }

        public void rotate_calibration_around_y_towards(Vector3 target) {
            var cd = CameraUtility.calibration_forward();
            var cp = (target - CameraUtility.calibration_position()).normalized;
            cp.y = cd.y;
            cp.Normalize();
            rotate_calibration_around_y(Vector3.SignedAngle(cd, cp, Vector3.up));
        }


        #endregion
    }
}