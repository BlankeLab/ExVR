
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
using System.Diagnostics;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public enum Movement {
        MoveToTarget,MoveBack
    }

    public enum SourceInitMode {
        InitSourceOnce, UpdateSourceEachFrame
    }

    public class TargetSettings {

        public Movement movement;

        // time
        public bool teleport;
        public bool useTime;
        public float duration;        

        // interpolation
        public AnimationCurve speedCurve = null;
        public bool useSphericalInterpolation;

        // camera
        public bool neutral;

        // source
        public SourceInitMode mode;
        public Vector3 initPosSource;
        public Quaternion initRotSource;

        // target
        // # axes
        public bool usePitch;
        public bool useYaw;
        public bool useRoll;
        // # pos / rot
        public Vector3 posTarget;
        public Vector3 eulerTarget;
        public Quaternion rotTarget;
        public Transform trTarget;
        // # offets
        public Vector3 targetOffsetposition = Vector3.zero;
        public Vector3 targetOffsetEulerRotation = Vector3.zero;
    }

    public class CameraTargetComponent : ExComponent{

        // states
        private bool continueMoving = false;
        private bool movementStarted = false;        
        private bool doLoop = false;
        private float movementFactor = 0f;
        private float totalPercent   = 0f;
        private float totalFactor    = 0f;
        private float previousFactor = 0f;
        private Stopwatch timer = new Stopwatch();

        // settings        
        private TargetSettings settings  = new TargetSettings();

        // trajectories
        private Trajectory trajectory = null;
        private Trajectory previousTrajectory = null;
        private Queue<Trajectory> savedTrajectories = new Queue<Trajectory>();

        public void set_factor(float factor) {
            float value = ((float)factor + currentC.get<float>("factor_offset")) * currentC.get<float>("factor_factor");
            movementFactor = Mathf.Clamp01(value);
        }

        public void set_target_offset_position(Vector3 postionOffset) {
            settings.targetOffsetposition = postionOffset;
        }

        public void set_target_offset_rotation(Vector3 eulerRotationOffset) {
            settings.targetOffsetEulerRotation = eulerRotationOffset;
        }

        protected override bool initialize() {
            add_slot("set factor", (factor) => {
                set_factor((float)factor);
            });
            add_slot("set target offset pos", (offsetPos) => {
                set_target_offset_position((Vector3)offsetPos);
            });
            add_slot("set target offset rot", (offsetEulerRot) => {
                set_target_offset_rotation((Vector3)offsetEulerRot);
            });
            return true;
        }

        public override void update_from_current_config() {

            settings.duration = currentC.get<float>("duration");
            settings.usePitch = currentC.get<bool>("pitch");
            settings.useYaw = currentC.get<bool>("yaw");
            settings.useRoll = currentC.get<bool>("roll");
            settings.useSphericalInterpolation = currentC.get<bool>("spherical_linear_interpolation");
            settings.useTime = currentC.get<bool>("use_time");
            settings.neutral = currentC.get<bool>("use_neutral");
            settings.teleport = currentC.get<bool>("teleport");
            settings.speedCurve = currentC.get_curve("speed_curve");
            settings.targetOffsetposition = Vector3.zero;
            settings.targetOffsetEulerRotation = Vector3.zero;
            settings.mode = currentC.get<bool>("init_source_once") ? SourceInitMode.InitSourceOnce : SourceInitMode.UpdateSourceEachFrame;

            if (currentC.get<bool>("move_to_target")) {

                settings.movement = Movement.MoveToTarget;

                trajectory = new Trajectory();
                if (!currentC.get<bool>("do_not_save_traj")) {
                    savedTrajectories.Enqueue(trajectory);
                }

            } else if (currentC.get<bool>("move_back")) {

                previousTrajectory = null;

                // use the last trajectory to move back
                if (savedTrajectories.Count != 0) {

                    try {
                        previousTrajectory = savedTrajectories.Dequeue();
                    } catch (System.InvalidOperationException) {
                        previousTrajectory = null;
                    }

                    if (previousTrajectory == null) {
                        log_warning("Cannot move back, no previous trajectory available.");
                    }

                } else {
                    log_warning("No previous trajectories to follow in the queue.");
                }

                settings.movement = Movement.MoveBack;
            }

            string targetComponent = currentC.get<string>("target_component");
            if (targetComponent.Length == 0) { // no component target to follow, move to defined position/rotation
                if (currentC.get<bool>("absolute")) {
                    settings.posTarget   = currentC.get_vector3("target_pos");
                    settings.eulerTarget = currentC.get_vector3("target_rot");
                } else {

                    var posOffset = currentC.get_vector3("target_pos");
                    var rotOffset = currentC.get_vector3("target_rot");

                    settings.posTarget =
                        CameraUtility.eye_camera_position() +
                        CameraUtility.eye_camera_forward() * posOffset.z +
                        CameraUtility.eye_camera_up() * posOffset.y +
                        CameraUtility.eye_camera_right() * posOffset.x;
                    settings.eulerTarget =
                        (CameraUtility.calibration_rotation() *
                        Quaternion.AngleAxis(rotOffset.x, CameraUtility.eye_camera_right()) *
                        Quaternion.AngleAxis(rotOffset.y, CameraUtility.eye_camera_up()) *
                        Quaternion.AngleAxis(rotOffset.z, CameraUtility.eye_camera_forward())).eulerAngles;
                }
                settings.trTarget = null;
            } else {
                var go = components().get_from_name(targetComponent);
                if (go != null) {
                    settings.trTarget = go.transform;                    
                } else {
                    log_error(string.Format("Component not found: {0}", targetComponent));
                }
            }
        }

        protected override void pre_start_routine() {
            movementStarted = false;            
            movementFactor  = 0f;
        }

        protected override void stop_routine() {
            doLoop = false;
            timer.Stop();
        }

        protected override void set_update_state(bool doUpdate) {
            continueMoving = doUpdate;
        }

        protected override void update() {

            if (!movementStarted) {
                totalPercent = 0f;
                totalFactor = 0f;
                previousFactor = 0f;
                movementStarted = true;

                // reset working values
                if (settings.useTime) {
                    doLoop = settings.duration > 0f;
                } else {
                    doLoop = movementFactor < 1f;
                }

                if(settings.mode == SourceInitMode.InitSourceOnce) {
                    timer.Restart();
                    settings.initPosSource = settings.neutral ? CameraUtility.calibration_position() : CameraUtility.eye_camera_position();
                    settings.initRotSource = settings.neutral ? CameraUtility.calibration_rotation() : CameraUtility.eye_camera_rotation();                    
                }
            }

            if (movementStarted) {

                if (doLoop) {
                    if (settings.movement == Movement.MoveToTarget) {
                        update_move_to_target();
                    }else if(settings.movement == Movement.MoveBack) {
                        update_move_back();
                    }
                }
            }
        }

        private void update_move_to_target() {

            if (!continueMoving) {
                return;
            }


            float value;
            Vector3 pos;
            Quaternion rot;

            // update current value
            if (settings.useTime) {

                if (settings.mode == SourceInitMode.InitSourceOnce) {

                    float percent = (float)timer.ElapsedMilliseconds / (settings.duration * 1000f);
                    if(percent > 1f) {
                        doLoop = false;
                        value = 1f;
                    } else {
                        value = value = settings.speedCurve.Evaluate(percent);
                    }

                } else {
                    float percent = Time.deltaTime / settings.duration;
                    totalPercent += percent;
                    if (totalPercent >= 1f) {
                        doLoop = false;
                        value = 1f;
                    } else {
                        value = percent / (1f - settings.speedCurve.Evaluate(totalPercent));
                    }
                }
            } else {

                if (settings.mode == SourceInitMode.InitSourceOnce) {
                    
                    if (movementFactor >= 1f) {
                        doLoop = false;
                        value = 1f;
                    } else  if( movementFactor >= 0f){                        
                        value = Mathf.Clamp01(settings.speedCurve.Evaluate(movementFactor));
                    } else {
                        log_error("Movement factor invalid: " + movementFactor);
                        value = 0f;
                    }

                } else {

                    float deltaFactor = Mathf.Clamp01(settings.speedCurve.Evaluate(movementFactor) - previousFactor);
                    totalFactor += deltaFactor;
                    previousFactor = movementFactor;

                    if (movementFactor >= 1f) {
                        doLoop = false;
                        value = 1f;
                    } else {
                        value = deltaFactor / (1f - settings.speedCurve.Evaluate(totalFactor));
                    }
                }
            }

            if (settings.teleport) {
                value = 1f;
            }

            // get current                
            var cPos = (settings.mode == SourceInitMode.InitSourceOnce) ? 
                settings.initPosSource : 
                (settings.neutral ? CameraUtility.calibration_position() : CameraUtility.eye_camera_position());

            var cRot = (settings.mode == SourceInitMode.InitSourceOnce) ?
                settings.initRotSource : 
                (settings.neutral ? CameraUtility.calibration_rotation() : CameraUtility.eye_camera_rotation());

            // compute current target
            if (settings.trTarget == null) { // use pos and rot as target

                pos = Interpolate.vector(cPos, settings.posTarget + settings.targetOffsetposition, value, settings.useSphericalInterpolation);

                // remove inused axies from rotation
                if (!settings.usePitch || !settings.useYaw || !settings.useRoll) {

                    var cEuler = cRot.eulerAngles;
                    rot = Interpolate.rotation(cRot,
                        Quaternion.Euler(new Vector3(
                            settings.usePitch  ? settings.eulerTarget.x : cEuler.x,
                            settings.useYaw    ? settings.eulerTarget.y : cEuler.y,
                            settings.useRoll   ? settings.eulerTarget.z : cEuler.z)),
                        value, settings.useSphericalInterpolation);

                } else {
                    rot = Interpolate.rotation(cRot, settings.rotTarget * Quaternion.Euler(settings.targetOffsetEulerRotation), value, settings.useSphericalInterpolation);
                }

            } else { // using transform as target

                pos = Interpolate.vector(cPos, settings.trTarget.position + settings.targetOffsetposition, value, settings.useSphericalInterpolation);

                // remove inused axies from rotation
                if (!settings.usePitch || !settings.useYaw || !settings.useRoll) {
                    var trEuler = settings.trTarget.eulerAngles;
                    var cEuler = cRot.eulerAngles;
                    rot = Interpolate.rotation(cRot,
                        Quaternion.Euler(new Vector3(
                            settings.usePitch ? trEuler.x : cEuler.x,
                            settings.useYaw ? trEuler.y : cEuler.y,
                            settings.useRoll ? trEuler.z : cEuler.z)),
                        value, settings.useSphericalInterpolation);
                } else {
                    rot = Interpolate.rotation(
                        cRot, settings.trTarget.rotation * Quaternion.Euler(settings.targetOffsetEulerRotation), 
                        value, settings.useSphericalInterpolation);
                }
            }

            // update camera
            if (settings.neutral) {
                CameraUtility.set_calibration_transform(pos, rot);
            } else {
                CameraUtility.set_eye_camera_transform(pos, rot);
            }

            // save point to trajectory
            trajectory.add_trajectory_point(pos, rot);
        }

        private void update_move_back() {

            if (!continueMoving) {
                return;
            }


            float value = 0f;
 
            // update current value
            if (settings.useTime) {
                float percent = Time.deltaTime / settings.duration;
                totalPercent += percent;

                if (totalPercent >= 1f) {
                    doLoop = false;
                    value = 1f;
                } else {
                    value = settings.speedCurve.Evaluate(totalPercent);
                }

            } else {

                float deltaFactor = Mathf.Clamp01(settings.speedCurve.Evaluate(movementFactor) - previousFactor);
                totalFactor += deltaFactor;
                previousFactor = movementFactor;

                if (movementFactor >= 1f) {
                    doLoop = false;
                    value = 1f;
                } else {
                    value = settings.speedCurve.Evaluate(totalFactor);
                }
            }

            if (settings.teleport) {
                value = 1f;
            }

            var pos = previousTrajectory.get_position(1f - value);
            var rot = previousTrajectory.get_rotation(1f - value);

            if (settings.neutral) {
                CameraUtility.set_calibration_transform(pos, rot);
            } else {
                CameraUtility.set_eye_camera_transform(pos, rot);
            }
        }

    }
}
