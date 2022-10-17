
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

    public class LeapMotionTrackingComponent : ExComponent{

        Dictionary<Leap.Finger.FingerType, string> fingersName  = new Dictionary<Leap.Finger.FingerType, string>();
        Dictionary<Leap.Bone.BoneType, string> bonesName        = new Dictionary<Leap.Bone.BoneType, string>();

        GameObject leftArmParent = null;
        GameObject rightArmParent = null;

        Dictionary<string, GameObject> leftArmElements = new Dictionary<string, GameObject>();
        Dictionary<string, GameObject> rightArmElements = new Dictionary<string, GameObject>();

        protected override bool initialize() {

            // init slots
            add_slot("update tracking", (frame) => { update_tracking((LeapMotionFrame)frame); });

            // init names
            fingersName[Leap.Finger.FingerType.TYPE_INDEX]  = "index";
            fingersName[Leap.Finger.FingerType.TYPE_MIDDLE] = "middle";
            fingersName[Leap.Finger.FingerType.TYPE_RING]   = "ring";
            fingersName[Leap.Finger.FingerType.TYPE_PINKY]  = "pinky";
            fingersName[Leap.Finger.FingerType.TYPE_THUMB]  = "thumb";

            bonesName[Leap.Bone.BoneType.TYPE_DISTAL]       = "distal";
            bonesName[Leap.Bone.BoneType.TYPE_INTERMEDIATE] = "intermediate";
            bonesName[Leap.Bone.BoneType.TYPE_METACARPAL]   = "metacarpal";
            bonesName[Leap.Bone.BoneType.TYPE_PROXIMAL]     = "proximal";

            // elements parents
            leftArmParent   = GO.generate_empty_object("left_arm", transform);
            rightArmParent  = GO.generate_empty_object("right_arm", transform);

            bool[] leftHand = new bool[] { true, false };
            foreach(var isLeftHand in leftHand) {

                var armParent           = isLeftHand ? leftArmParent.transform : rightArmParent.transform;
                var currentElements     = isLeftHand ? leftArmElements   : rightArmElements;

                // generate elements
                currentElements["elbow"] = GO.generate_sphere("elbow",  armParent, 0.02f, Color.blue);
                currentElements["arm"]   = GO.generate_landmark("arm",    armParent, Color.blue);
                currentElements["wrist"] = GO.generate_sphere("wrist",  armParent, 0.02f, Color.yellow);
                currentElements["hand"]  = GO.generate_landmark("hand",  armParent, Color.cyan);
                currentElements["hand"].transform.localScale = new Vector3(1.2f, 1.2f, 1.2f);

                foreach (var finger in fingersName) {
                    currentElements[finger.Value + "_tip"] = GO.generate_sphere(finger.Value + "_tip", armParent, 0.005f, Color.green);

                    foreach (var bone in bonesName) {
                        currentElements[finger.Value + "_" + bone.Value] = GO.generate_landmark(finger.Value + "_" + bone.Value, armParent, Color.red);
                        currentElements[finger.Value + "_" + bone.Value].transform.localScale = new Vector3(0.5f, 0.5f, 0.5f);
                    }
                }
            }

            return true;
        }


        public Transform arm_element(bool left, string elementName) {
            var armElements = left ? leftArmElements : rightArmElements;
            if (armElements.ContainsKey(elementName)) {
                return armElements[elementName].transform;
            }
            log_error(string.Format("Element {0} doesn't exist", elementName));
            return null;
        }

        protected override void start_routine() {

            foreach (var elements in leftArmElements) {
                elements.Value.transform.position = new Vector3();
                elements.Value.transform.eulerAngles = new Vector3();
            }
            foreach (var elements in rightArmElements) {
                elements.Value.transform.position = new Vector3();
                elements.Value.transform.eulerAngles = new Vector3();
            }
        }

        protected override void set_visibility(bool visibility) {
            leftArmParent.SetActive(visibility);
            rightArmParent.SetActive(visibility);
        }


        public void update_tracking(LeapMotionFrame frame) {

            if(frame == null) {
                return;
            }

            if (frame.leftArmVisible) {

                var currentElements = leftArmElements;
                var currentPositions = frame.leftArmPositions;
                var currentRotations = frame.leftArmRotations;

                // update elements
                // # position & rotation
                currentElements["hand"].transform.position = currentPositions["palm"];
                currentElements["hand"].transform.rotation = currentRotations["hand"];
                currentElements["arm"].transform.position = currentPositions["arm_center"];
                currentElements["arm"].transform.rotation = currentRotations["arm"];
                // # only position
                currentElements["wrist"].transform.position = currentPositions["wrist"];
                currentElements["elbow"].transform.position = currentPositions["elbow"];

                // fingers
                foreach (var finger in LeapMotionFrame.fingersName) {

                    // update elements
                    currentElements[finger.Value + "_tip"].transform.position = currentPositions[finger.Value + "_tip"];

                    foreach (var bone in LeapMotionFrame.bonesName) {

                        // update elements
                        currentElements[finger.Value + "_" + bone.Value].transform.position = currentPositions[finger.Value + "_" + bone.Value];
                        currentElements[finger.Value + "_" + bone.Value].transform.rotation = currentRotations[finger.Value + "_" + bone.Value];
                    }
                }
            }

            if (frame.rightArmVisible) {

                var currentElements = rightArmElements;
                var currentPositions = frame.rightArmPositions;
                var currentRotations = frame.rightArmRotations;

                // update elements
                // # position & rotation
                currentElements["hand"].transform.position = currentPositions["palm"];
                currentElements["hand"].transform.rotation = currentRotations["hand"];
                currentElements["arm"].transform.position = currentPositions["arm_center"];
                currentElements["arm"].transform.rotation = currentRotations["arm"];
                // # only position
                currentElements["wrist"].transform.position = currentPositions["wrist"];
                currentElements["elbow"].transform.position = currentPositions["elbow"];

                // fingers
                foreach (var finger in LeapMotionFrame.fingersName) {

                    // update elements
                    currentElements[finger.Value + "_tip"].transform.position = currentPositions[finger.Value + "_tip"];

                    foreach (var bone in LeapMotionFrame.bonesName) {

                        // update elements
                        currentElements[finger.Value + "_" + bone.Value].transform.position = currentPositions[finger.Value + "_" + bone.Value];
                        currentElements[finger.Value + "_" + bone.Value].transform.rotation = currentRotations[finger.Value + "_" + bone.Value];
                    }
                }
            }
        }
    }
}

