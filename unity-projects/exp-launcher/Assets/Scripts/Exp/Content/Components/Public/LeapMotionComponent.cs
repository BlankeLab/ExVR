
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
using System.Text;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class LeapMotionFrame {

        public bool leftArmVisible = false;
        public bool rightArmVisible = false;

        public static readonly Dictionary<Leap.Finger.FingerType, string> fingersName = new Dictionary<Leap.Finger.FingerType, string>() {
            [Leap.Finger.FingerType.TYPE_INDEX] = "index",
            [Leap.Finger.FingerType.TYPE_MIDDLE] = "middle",
            [Leap.Finger.FingerType.TYPE_RING] = "ring",
            [Leap.Finger.FingerType.TYPE_PINKY] = "pinky",
            [Leap.Finger.FingerType.TYPE_THUMB] = "thumb",
        };

        public static readonly Dictionary<Leap.Bone.BoneType, string> bonesName = new Dictionary<Leap.Bone.BoneType, string>() {
            [Leap.Bone.BoneType.TYPE_DISTAL] = "distal",
            [Leap.Bone.BoneType.TYPE_INTERMEDIATE] = "intermediate",
            [Leap.Bone.BoneType.TYPE_METACARPAL] = "metacarpal",
            [Leap.Bone.BoneType.TYPE_PROXIMAL] = "proximal"
        };


        public Dictionary<string, float> leftArmMeasures = new Dictionary<string, float>();
        public Dictionary<string, float> rightArmMeasures = new Dictionary<string, float>();

        public Dictionary<string, Vector3> leftArmPositions = new Dictionary<string, Vector3>() {
            ["palm"]            = Vector3.zero,
            ["stabilized_palm"] = Vector3.zero,
            ["wrist"]           = Vector3.zero,
            ["elbow"]           = Vector3.zero,
            ["arm_center"]      = Vector3.zero,
            ["index_tip"]       = Vector3.zero,
            ["middle_tip"]      = Vector3.zero,
            ["ring_tip"]        = Vector3.zero,
            ["pinky_tip"]       = Vector3.zero,
            ["thumb_tip"]       = Vector3.zero
        };
        public Dictionary<string, Vector3> rightArmPositions = new Dictionary<string, Vector3>() {
            ["palm"]            = Vector3.zero,
            ["stabilized_palm"] = Vector3.zero,
            ["wrist"]           = Vector3.zero,
            ["elbow"]           = Vector3.zero,
            ["arm_center"]      = Vector3.zero,
            ["index_tip"]       = Vector3.zero,
            ["middle_tip"]      = Vector3.zero,
            ["ring_tip"]        = Vector3.zero,
            ["pinky_tip"]       = Vector3.zero,
            ["thumb_tip"]       = Vector3.zero
        };

        public Dictionary<string, Vector3> leftArmDirections = new Dictionary<string, Vector3>() {
            ["hand"]    = Vector3.one,
            ["arm"]     = Vector3.one,
            ["index"]   = Vector3.one,
            ["middle"]  = Vector3.one,
            ["ring"]    = Vector3.one,
            ["pinky"]   = Vector3.one,
            ["thumb"]   = Vector3.one
        };
        public Dictionary<string, Vector3> rightArmDirections = new Dictionary<string, Vector3>() {
            ["hand"]    = Vector3.one,
            ["arm"]     = Vector3.one,
            ["index"]   = Vector3.one,
            ["middle"]  = Vector3.one,
            ["ring"]    = Vector3.one,
            ["pinky"]   = Vector3.one,
            ["thumb"]   = Vector3.one
        };

        public Dictionary<string, Vector3> leftArmNormals = new Dictionary<string, Vector3>();
        public Dictionary<string, Vector3> rightArmNormals = new Dictionary<string, Vector3>();

        public Dictionary<string, Quaternion> leftArmRotations = new Dictionary<string, Quaternion>() {
            ["hand"] = Quaternion.identity,
            ["arm"]  = Quaternion.identity
        };
        public Dictionary<string, Quaternion> rightArmRotations = new Dictionary<string, Quaternion>() {
            ["hand"] = Quaternion.identity,
            ["arm"]  = Quaternion.identity
        };

        public Vector3 arm_position(bool left, string positionName) {

            var arm = left ? leftArmPositions : rightArmPositions;
            if (arm.ContainsKey(positionName)) {
                return arm[positionName];
            }
            ExVR.Log().error(string.Format("Position {0} doesn't exist", positionName));
            return Vector3.zero;
        }

        public Vector3 arm_normal(bool left, string normalName) {

            var arm = left ? leftArmNormals : rightArmNormals;
            if (arm.ContainsKey(normalName)) {
                return arm[normalName];
            }
            ExVR.Log().error(string.Format("Normal {0} doesn't exist", normalName));
            return Vector3.one;
        }

        public float arm_measure(bool left, string measureName) {
            var armInfo = left ? leftArmMeasures : rightArmMeasures;
            if (armInfo.ContainsKey(measureName)) {
                return armInfo[measureName];
            }
            ExVR.Log().error(string.Format("Measure {0} doesn't exist", measureName));
            return 0f;
        }

        public Vector3 arm_direction(bool left, string directionName) {
            var armDirection = left ? leftArmDirections : rightArmDirections;
            if (armDirection.ContainsKey(directionName)) {
                return armDirection[directionName];
            }
            ExVR.Log().error(string.Format("Direction {0} doesn't exist", directionName));
            return Vector3.one;
        }

        public Quaternion arm_rotation(bool left, string rotationName) {
            var armRotations = left ? leftArmRotations : rightArmRotations;
            if (armRotations.ContainsKey(rotationName)) {
                return armRotations[rotationName];
            }
            ExVR.Log().error(string.Format("Rotation {0} doesn't exist", rotationName));
            return Quaternion.identity;
        }

        private Vector3 from_leap_vector(Leap.Vector vector) {
            return new Vector3(vector.x, vector.y, vector.z);
        }

        private Quaternion from_leap_quaternion(Leap.LeapQuaternion quaternion) {
            return new Quaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
        }

        public LeapMotionFrame(Leap.Frame frame) {

            if (frame == null) {
                ExVR.Log().error("Leap motion frame is invalid.");
                return;
            }

            leftArmVisible = false;
            rightArmVisible = false;

            foreach (var hand in frame.Hands) {

                // check visible hand
                if (hand.IsLeft) {
                    leftArmVisible = true;
                }
                if (hand.IsRight) {
                    rightArmVisible = true;
                }

                // get current hand dictionnaries
                var currentPositions                = hand.IsLeft ? leftArmPositions    : rightArmPositions;
                var currentMeasures                 = hand.IsLeft ? leftArmMeasures     : rightArmMeasures;
                var currentRotations                = hand.IsLeft ? leftArmRotations    : rightArmRotations;
                var currentNormals                  = hand.IsLeft ? leftArmNormals      : rightArmNormals;
                var currentDirections               = hand.IsLeft ? leftArmDirections   : rightArmDirections;

                // update measures
                currentMeasures["confidence"]       = hand.Confidence;
                currentMeasures["grab_angle"]       = hand.GrabAngle;
                currentMeasures["grab_strength"]    = hand.GrabStrength;
                currentMeasures["palm_width"]       = hand.PalmWidth;
                currentMeasures["palm_distance"]    = hand.PinchDistance;
                currentMeasures["pinch_strength"]   = hand.PinchStrength;
                currentMeasures["time_visible"]     = hand.TimeVisible;
                currentMeasures["arm_width"]        = hand.Arm.Length;
                currentMeasures["arm_length"]       = hand.Arm.Width;

                // update positions
                currentPositions["palm"]            = from_leap_vector(hand.PalmPosition);
                currentPositions["stabilized_palm"] = from_leap_vector(hand.StabilizedPalmPosition);
                currentPositions["wrist"]           = from_leap_vector(hand.WristPosition);
                currentPositions["elbow"]           = from_leap_vector(hand.Arm.ElbowPosition);
                currentPositions["arm_center"]      = from_leap_vector(hand.Arm.Center);

                // update normals
                currentNormals["palm"]              = from_leap_vector(hand.PalmNormal);

                // update directions
                currentDirections["hand"]           = from_leap_vector(hand.Direction);
                currentDirections["arm"]            = from_leap_vector(hand.Arm.Direction);

                // update rotations
                currentRotations["hand"]            = from_leap_quaternion(hand.Rotation);
                currentRotations["arm"]             = from_leap_quaternion(hand.Arm.Rotation);

                // fingers
                foreach (var finger in hand.Fingers) {

                    string fingerName = fingersName[finger.Type];

                    // update measures
                    currentMeasures[fingerName + "_length"]         = finger.Length;
                    currentMeasures[fingerName + "_width"]          = finger.Width;
                    currentMeasures[fingerName + "_extended"]       = finger.IsExtended ? 1f : 0f;
                    currentMeasures[fingerName + "_time_visible"]   = finger.TimeVisible;

                    // update positions
                    currentPositions[fingerName + "_tip"] = from_leap_vector(finger.TipPosition);

                    // update directions
                    currentDirections[fingerName] = from_leap_vector(finger.Direction);

                    foreach (var bone in finger.bones) {

                        string boneName = string.Format("{0}_{1}", fingerName, bonesName[bone.Type]);

                        // update measures
                        currentMeasures[boneName + "_length"] = bone.Length;
                        currentMeasures[boneName + "_width"]  = bone.Width;

                        // update position
                        currentPositions[boneName]   = from_leap_vector(bone.Center);

                        // update direction
                        currentDirections[boneName]  = from_leap_vector(bone.Direction);

                        // update rotation
                        currentRotations[boneName]   = from_leap_quaternion(bone.Rotation);
                    }
                }
            }
        }    
    }

    public class LeapMotionComponent : ExComponent{

        public LeapMotionFrame leapMotionFrame = null;
        private Leap.Frame m_rawLeapMotionFrame = null;
        private Leap.Unity.LeapXRServiceProvider m_leap = null;


        #region ex_functions
        protected override bool initialize() {

            // signals
            add_signal("new raw frame");
            add_signal("new hands frame");

            // init leap provider
            m_leap = ExVR.Display().cameras().get_eye_camera_transform().gameObject.AddComponent<Leap.Unity.LeapXRServiceProvider>();                        

            return true;
        }

        protected override void clean() {
            Destroy(m_leap);
        }

        protected override void start_experiment() {
            if (!m_leap.IsConnected()) {
                log_warning("LeapMotion not connected.");
            }
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        public override void update_from_current_config() {
            if (currentC.get<bool>("use_transform_as_origin")) {

                currentC.update_transform("transform", transform, false, true, true, true);
                m_leap.deviceOrigin = transform;
                m_leap.deviceOffsetMode = Leap.Unity.LeapXRServiceProvider.DeviceOffsetMode.Transform;
            } else {
                m_leap.deviceOffsetMode = Leap.Unity.LeapXRServiceProvider.DeviceOffsetMode.Default;
                m_leap.deviceOrigin = null;
            }
        }

        protected override void pre_update() {

            if (!m_leap.IsConnected()) {
                return;
            }

            // inif frame
            if (m_rawLeapMotionFrame == null) {
                m_rawLeapMotionFrame = new Leap.Frame();
            }
            // copy last frame
            Leap.CopyFromOtherExtensions.CopyFrom(m_rawLeapMotionFrame, m_leap.CurrentFrame);

            // create new leap motion frame
            leapMotionFrame = new LeapMotionFrame(m_rawLeapMotionFrame);

            // send data
            if (is_updating()) {
                invoke_signal("new raw frame", m_rawLeapMotionFrame);
                invoke_signal("new hands frame", leapMotionFrame);
            }
        }

        public override string format_frame_data_for_global_logger(bool header) {

            if (header) {
                return
                    "LeapMotion: left_arm_visible left_palm_pos_xyz left_stabilized_palm_pos_xyz left_wrist_pos_xyz left_elbow_pos_xyz left_arm_center_pos_xyz " +
                    "left_hand_rot_xyz left_arm_rot_xyz left_hand_dir_xyz left_arm_dir_xyz " +
                    "left_index_tip_pos_xyz left_middle_tip_pos_xyz left_ring_tip_pos_xyz left_pinky_tip_pos_xyz left_thumb_tip_pos_xyz " +
                    "left_index_dir_xyz left_middle_dir_xyz left_ring_dir_xyz left_pinky_dir_xyz left_thumb_dir_xyz " +
                    "right_arm_visbile right_palm_pos_xyz right_stabilized_palm_pos_xyz right_wrist_pos_xyz right_elbow_pos_xyz right_arm_center_pos_xyz " +
                    "right_hand_rot_xyz right_arm_rot_xyz right_hand_dir_xyz right_arm_dir_xyz " +
                    "right_index_tip_pos_xyz right_middle_tip_pos_xyz right_ring_tip_pos_xyz right_pinky_tip_pos_xyz right_thumb_tip_pos_xyz " +
                    "right_index_dir_xyz right_middle_dir_xyz right_ring_dir_xyz right_pinky_dir_xyz right_thumb_dir_xyz";
            }

            if (!m_leap.IsConnected()) {
                return null;
            }

            if (leapMotionFrame == null) {
                return null;
            }


            StringBuilder sb = new StringBuilder();
            var pos = leapMotionFrame.leftArmPositions;
            var rot = leapMotionFrame.leftArmRotations;
            var dir = leapMotionFrame.leftArmDirections;


            sb.AppendFormat("{0} {1} {2} {3} {4} {5} {6} {7} {8} {9} {10} {11} {12} {13} {14} {15} {16} {17} {18} {19}",
                leapMotionFrame.leftArmVisible ? "1" : "0",
                Converter.to_string(pos["palm"], Converter.g7, " "),
                Converter.to_string(pos["stabilized_palm"], Converter.g7, " "),
                Converter.to_string(pos["wrist"], Converter.g7, " "),
                Converter.to_string(pos["elbow"], Converter.g7, " "),
                Converter.to_string(pos["arm_center"], Converter.g7, " "),

                Converter.to_string(rot["hand"].eulerAngles, Converter.g7, " "),
                Converter.to_string(rot["arm"].eulerAngles, Converter.g7, " "),

                Converter.to_string(dir["hand"], Converter.g7, " "),
                Converter.to_string(dir["arm"],  Converter.g7, " "),

                Converter.to_string(pos["index_tip"], Converter.g7, " "),
                Converter.to_string(pos["middle_tip"], Converter.g7, " "),
                Converter.to_string(pos["ring_tip"], Converter.g7, " "),
                Converter.to_string(pos["pinky_tip"], Converter.g7, " "),
                Converter.to_string(pos["thumb_tip"], Converter.g7, " "),

                Converter.to_string(dir["index"], Converter.g7, " "),
                Converter.to_string(dir["middle"], Converter.g7, " "),
                Converter.to_string(dir["ring"], Converter.g7, " "),
                Converter.to_string(dir["pinky"], Converter.g7, " "),
                Converter.to_string(dir["thumb"], Converter.g7, " ")
            );
            
            pos = leapMotionFrame.rightArmPositions;
            rot = leapMotionFrame.rightArmRotations;
            dir = leapMotionFrame.rightArmDirections;

            sb.AppendFormat(" {0} {1} {2} {3} {4} {5} {6} {7} {8} {9} {10} {11} {12} {13} {14} {15} {16} {17} {18} {19}",
                leapMotionFrame.rightArmVisible ? "1" : "0",
                Converter.to_string(pos["palm"], Converter.g7, " "),
                Converter.to_string(pos["stabilized_palm"], Converter.g7, " "),
                Converter.to_string(pos["wrist"], Converter.g7, " "),
                Converter.to_string(pos["elbow"], Converter.g7, " "),
                Converter.to_string(pos["arm_center"], Converter.g7, " "),

                Converter.to_string(rot["hand"].eulerAngles, Converter.g7, " "),
                Converter.to_string(rot["arm"].eulerAngles, Converter.g7, " "),

                Converter.to_string(dir["hand"], Converter.g7, " "),
                Converter.to_string(dir["arm"], Converter.g7, " "),

                Converter.to_string(pos["index_tip"], Converter.g7, " "),
                Converter.to_string(pos["middle_tip"], Converter.g7, " "),
                Converter.to_string(pos["ring_tip"], Converter.g7, " "),
                Converter.to_string(pos["pinky_tip"], Converter.g7, " "),
                Converter.to_string(pos["thumb_tip"], Converter.g7, " "),

                Converter.to_string(dir["index"], Converter.g7, " "),
                Converter.to_string(dir["middle"], Converter.g7, " "),
                Converter.to_string(dir["ring"], Converter.g7, " "),
                Converter.to_string(dir["pinky"], Converter.g7, " "),
                Converter.to_string(dir["thumb"], Converter.g7, " ")
            );

            return sb.ToString();
        }

        #endregion
    }
}

