
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
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class LeapMotionFrame{

        public bool leftArmVisible = false;
        public bool rightArmVisible = false;

        public Dictionary<Leap.Finger.FingerType, string> fingersName = new Dictionary<Leap.Finger.FingerType, string>();
        public Dictionary<Leap.Bone.BoneType, string> bonesName = new Dictionary<Leap.Bone.BoneType, string>();

        public Dictionary<string, float> leftArmMeasures = new Dictionary<string, float>();
        public Dictionary<string, float> rightArmMeasures = new Dictionary<string, float>();

        public Dictionary<string, Vector3> leftArmPositions = new Dictionary<string, Vector3>();
        public Dictionary<string, Vector3> rightArmPositions = new Dictionary<string, Vector3>();

        public Dictionary<string, Vector3> leftArmDirections = new Dictionary<string, Vector3>();
        public Dictionary<string, Vector3> rightArmDirections = new Dictionary<string, Vector3>();

        public Dictionary<string, Vector3> leftArmNormals = new Dictionary<string, Vector3>();
        public Dictionary<string, Vector3> rightArmNormals = new Dictionary<string, Vector3>();

        public Dictionary<string, Quaternion> leftArmRotations = new Dictionary<string, Quaternion>();
        public Dictionary<string, Quaternion> rightArmRotations = new Dictionary<string, Quaternion>();

        public Vector3 arm_position(bool left, string positionName) {

            var arm = left ? leftArmPositions : rightArmPositions;
            if (arm.ContainsKey(positionName)) {
                return arm[positionName];
            }
            ExVR.Log().error("Position " + positionName + " doesn't exist");
            return new Vector3();
        }

        public Vector3 arm_normal(bool left, string normalName) {

            var arm = left ? leftArmNormals : rightArmNormals;
            if (arm.ContainsKey(normalName)) {
                return arm[normalName];
            }
            ExVR.Log().error("Normal " + normalName + " doesn't exist");
            return new Vector3();
        }

        public float arm_measure(bool left, string measureName) {
            var armInfo = left ? leftArmMeasures : rightArmMeasures;
            if (armInfo.ContainsKey(measureName)) {
                return armInfo[measureName];
            }
            ExVR.Log().error("Measure " + measureName + " doesn't exist");
            return 0f;
        }

        public Vector3 arm_direction(bool left, string directionName) {
            var armDirection = left ? leftArmDirections : rightArmDirections;
            if (armDirection.ContainsKey(directionName)) {
                return armDirection[directionName];
            }
            ExVR.Log().error("Direction " + directionName + " doesn't exist");
            return new Vector3();
        }

        public Quaternion arm_rotation(bool left, string rotationName) {
            var armRotations = left ? leftArmRotations : rightArmRotations;
            if (armRotations.ContainsKey(rotationName)) {
                return armRotations[rotationName];
            }
            ExVR.Log().error("Rotation " + rotationName + " doesn't exist");
            return new Quaternion();
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

            // init names
            fingersName[Leap.Finger.FingerType.TYPE_INDEX] = "index";
            fingersName[Leap.Finger.FingerType.TYPE_MIDDLE] = "middle";
            fingersName[Leap.Finger.FingerType.TYPE_RING] = "ring";
            fingersName[Leap.Finger.FingerType.TYPE_PINKY] = "pinky";
            fingersName[Leap.Finger.FingerType.TYPE_THUMB] = "thumb";

            bonesName[Leap.Bone.BoneType.TYPE_DISTAL] = "distal";
            bonesName[Leap.Bone.BoneType.TYPE_INTERMEDIATE] = "intermediate";
            bonesName[Leap.Bone.BoneType.TYPE_METACARPAL] = "metacarpal";
            bonesName[Leap.Bone.BoneType.TYPE_PROXIMAL] = "proximal";

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
                var currentPositions = hand.IsLeft ? leftArmPositions : rightArmPositions;
                var currentMeasures = hand.IsLeft ? leftArmMeasures : rightArmMeasures;
                var currentRotations = hand.IsLeft ? leftArmRotations : rightArmRotations;
                var currentNormals = hand.IsLeft ? leftArmNormals : rightArmNormals;
                var currentDirections = hand.IsLeft ? leftArmDirections : rightArmDirections;

                // update measures
                currentMeasures["confidence"] = hand.Confidence;
                currentMeasures["grab_angle"] = hand.GrabAngle;
                currentMeasures["grab_strength"] = hand.GrabStrength;
                currentMeasures["palm_width"] = hand.PalmWidth;
                currentMeasures["palm_distance"] = hand.PinchDistance;
                currentMeasures["pinch_strength"] = hand.PinchStrength;
                currentMeasures["time_visible"] = hand.TimeVisible;
                currentMeasures["arm_width"] = hand.Arm.Length;
                currentMeasures["arm_length"] = hand.Arm.Width;

                // update positions
                currentPositions["palm"] = from_leap_vector(hand.PalmPosition);
                currentPositions["stabilized_palm"] = from_leap_vector(hand.StabilizedPalmPosition);
                currentPositions["wrist"] = from_leap_vector(hand.WristPosition);
                currentPositions["elbow"] = from_leap_vector(hand.Arm.ElbowPosition);
                currentPositions["arm_center"] = from_leap_vector(hand.Arm.Center);

                // update normals
                currentNormals["palm"] = from_leap_vector(hand.PalmNormal);

                // update directions
                currentDirections["hand"] = from_leap_vector(hand.Direction);
                currentDirections["arm"] = from_leap_vector(hand.Arm.Direction);

                // update rotations
                currentRotations["hand"] = from_leap_quaternion(hand.Rotation);
                currentRotations["arm"] = from_leap_quaternion(hand.Arm.Rotation);

                // fingers
                foreach (var finger in hand.Fingers) {

                    string fingerName = fingersName[finger.Type];

                    // update measures
                    currentMeasures[fingerName + "_length"] = finger.Length;
                    currentMeasures[fingerName + "_width"] = finger.Width;
                    currentMeasures[fingerName + "_extended"] = finger.IsExtended ? 1f : 0f;
                    currentMeasures[fingerName + "_time_visible"] = finger.TimeVisible;

                    // update positions
                    currentPositions[fingerName + "_tip"] = from_leap_vector(finger.TipPosition);

                    // update directions
                    currentDirections[fingerName] = from_leap_vector(finger.Direction);

                    foreach (var bone in finger.bones) {

                        string boneName = bonesName[bone.Type];

                        // update measures
                        currentMeasures[fingerName + "_" + boneName + "_length"] = bone.Length;
                        currentMeasures[fingerName + "_" + boneName + "_width"] = bone.Width;

                        // update position
                        currentPositions[fingerName + "_" + boneName] = from_leap_vector(bone.Center);

                        // update direction
                        currentDirections[fingerName + "_" + boneName] = from_leap_vector(bone.Direction);

                        // update rotation
                        currentRotations[fingerName + "_" + boneName] = from_leap_quaternion(bone.Rotation);
                    }
                }
            }
        }    
    }

    public class LeapMotionComponent : ExComponent{

        private Leap.Unity.LeapXRServiceProvider m_leap = null;

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

        protected override void pre_update() {

            if (!m_leap.IsConnected()) {
                return;
            }

            var frame = new Leap.Frame();
            Leap.CopyFromOtherExtensions.CopyFrom(frame, m_leap.CurrentFrame);
            invoke_signal("new raw frame", frame);
            invoke_signal("new hands frame", new LeapMotionFrame(frame));
        }
    }
}

