
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

//unity
using UnityEngine;

namespace Ex{

    public class TheraTrainerPlatformComponent : ExComponent{

        GameObject theraGO = null;

        public Vector3 pelvisTrOrigin;
        public Vector3 leftHandTrOrigin;
        public Vector3 rightHandTrOrigin;
        public Vector3 pelvisRotOrigin;
        public Vector3 leftHandRotOrigin;
        public Vector3 rightHandRotOrigin;

        public Transform pelvis = null;
        public Transform leftHandGrip = null;
        public Transform rightHandGrip = null;

        public Transform originRotuleLeft = null;
        public Transform originRotuleRight = null;
        public Transform originRotuleCenter = null;

        public Transform rotuleLeft = null;
        public Transform rotuleRight = null;
        public Transform rotuleCenter = null;

        public Transform centerRotuleLeft = null;
        public Transform centerRotuleRight = null;

        public Transform mobilePartLeft = null;
        public Transform mobilePartRight = null;
        public Transform mobilePartCenter = null;

        public Transform leftExtender = null;
        public Transform rightExtender = null;

        Quaternion originMobileLeftPartRotation;
        Vector3 originMobileLeftPartPosition;

        Quaternion originMobileRightPartRotation;
        Vector3 originMobileRightPartPosition;

        Quaternion originMobileCenterPartRotation;
        Vector3 originMobileCenterPartPosition;

        public Vector2 rotationT = Vector2.zero;
        public float height = 1.0f;


        #region ex_functions
        protected override bool initialize() {

            // init slots
            add_slot("update rotation", (rot) => {
                rotationT = (Vector2)rot;
            });

            // init gameObject
            theraGO = ExVR.GlobalResources().instantiate_prebab("Components/TheraTrainer", transform);
            if(theraGO == null) {
                return false;
            }

            var thera           = theraGO.transform;
            var mobilePart      = thera.Find("mobile_part");            
            mobilePartLeft      = mobilePart.Find("mobile_part_left");
            mobilePartRight     = mobilePart.Find("mobile_part_right");
            mobilePartCenter    = mobilePart.Find("mobile_part_center");
            pelvis              = mobilePartCenter.Find("pelvis");
            leftHandGrip        = mobilePartCenter.Find("left_hand_grip");
            rightHandGrip       = mobilePartCenter.Find("right_hand_grip");
            rotuleLeft          = mobilePartLeft.Find("rotule_left");
            rotuleRight         = mobilePartRight.Find("rotule_right");
            rotuleCenter        = mobilePartCenter.Find("rotule_center");

            centerRotuleLeft    = mobilePartCenter.Find("rotule_left");
            centerRotuleRight   = mobilePartCenter.Find("rotule_right");

            originRotuleLeft    = thera.Find("origin_rotule_left");
            originRotuleCenter  = mobilePartRight.Find("origin_rotule_center");
            originRotuleRight   = thera.Find("origin_rotule_right");
            leftExtender        = mobilePartLeft.Find("left_extender");
            rightExtender       = mobilePartRight.Find("right_extender");

            pelvisTrOrigin      = pelvis.transform.localPosition;
            pelvisRotOrigin     = pelvis.transform.localEulerAngles;
            leftHandTrOrigin    = leftHandGrip.transform.localPosition;
            leftHandRotOrigin   = leftHandGrip.transform.localEulerAngles;
            rightHandTrOrigin   = rightHandGrip.transform.localPosition;
            rightHandRotOrigin  = rightHandGrip.transform.localEulerAngles;

            return true;
        }

        public override void update_from_current_config() {

            height = currentC.get<float>("height");
            pelvis.gameObject.SetActive(currentC.get<bool>("display_targets"));
            leftHandGrip.gameObject.SetActive(currentC.get<bool>("display_targets"));
            rightHandGrip.gameObject.SetActive(currentC.get<bool>("display_targets"));

            // reset transform
            theraGO.transform.localPosition = Vector3.zero;
            theraGO.transform.localEulerAngles = Vector3.zero;

            mobilePartLeft.localRotation = Quaternion.identity;
            mobilePartLeft.localPosition = Vector3.zero;

            mobilePartRight.localRotation = Quaternion.identity;
            mobilePartRight.localPosition = Vector3.zero;

            mobilePartCenter.localRotation = Quaternion.identity;
            mobilePartCenter.localPosition = Vector3.zero;

            originMobileLeftPartRotation = mobilePartLeft.rotation;
            originMobileLeftPartPosition = mobilePartLeft.position;

            originMobileRightPartRotation = mobilePartRight.rotation;
            originMobileRightPartPosition = mobilePartRight.position;

            originMobileCenterPartRotation = mobilePartCenter.rotation;
            originMobileCenterPartPosition = mobilePartCenter.position;

            // apply offset grip
            pelvis.transform.localPosition = pelvisTrOrigin + currentC.get_vector3("offset_pelvis_pos");
            pelvis.transform.localEulerAngles = pelvisRotOrigin + currentC.get_vector3("offset_pelvis_rot");

            leftHandGrip.transform.localPosition = leftHandTrOrigin + currentC.get_vector3("offset_left_grip_pos");
            leftHandGrip.transform.localEulerAngles = leftHandRotOrigin + currentC.get_vector3("offset_left_grip_rot");

            rightHandGrip.transform.localPosition = rightHandTrOrigin + currentC.get_vector3("offset_right_grip_pos");
            rightHandGrip.transform.localEulerAngles = rightHandRotOrigin + currentC.get_vector3("offset_right_grip_rot");

            // apply rotation
            mobilePartLeft.rotation = originMobileLeftPartRotation;
            mobilePartLeft.position = originMobileLeftPartPosition;

            mobilePartRight.rotation = originMobileRightPartRotation;
            mobilePartRight.position = originMobileRightPartPosition;

            mobilePartCenter.rotation = originMobileCenterPartRotation;
            mobilePartCenter.position = originMobileCenterPartPosition;

            var rotuleLeftForward = mobilePartLeft.worldToLocalMatrix * rotuleLeft.localToWorldMatrix * rotuleLeft.forward;
            var rotuleCenterForward = mobilePartCenter.worldToLocalMatrix * rotuleCenter.localToWorldMatrix * rotuleCenter.forward;
            var rotuleRightForward = mobilePartRight.worldToLocalMatrix * rotuleRight.localToWorldMatrix * rotuleRight.forward;

            var rotuleLeftRight = mobilePartLeft.worldToLocalMatrix * rotuleLeft.localToWorldMatrix * rotuleLeft.right;
            var rotuleRightRight = mobilePartRight.worldToLocalMatrix * rotuleRight.localToWorldMatrix * rotuleRight.right;

            mobilePartLeft.Rotate(rotuleLeftForward, rotationT.x);
            mobilePartLeft.Rotate(rotuleLeftRight, rotationT.y);

            mobilePartRight.Rotate(rotuleRightForward, rotationT.x);
            mobilePartRight.Rotate(rotuleRightRight, rotationT.y);

            mobilePartCenter.Rotate(rotuleCenterForward, rotationT.x);

            mobilePartLeft.position = originMobileLeftPartPosition + originRotuleLeft.position - rotuleLeft.position;
            mobilePartRight.position = originMobileRightPartPosition + originRotuleRight.position - rotuleRight.position;
            mobilePartCenter.position = originMobileCenterPartPosition + originRotuleCenter.position - rotuleCenter.position + originRotuleCenter.up * (height - 1);

            var vec = centerRotuleLeft.position - rotuleLeft.position;
            var l = vec.magnitude * 0.5f;
            leftExtender.localScale = new Vector3(leftExtender.localScale.x, l, leftExtender.localScale.z);
            leftExtender.position = rotuleLeft.position + vec * 0.5f;

            vec = centerRotuleRight.position - rotuleRight.position;
            l = vec.magnitude * 0.5f;
            rightExtender.localScale = new Vector3(rightExtender.localScale.x, l, rightExtender.localScale.z);
            rightExtender.position = rotuleRight.position + vec * 0.5f;

            // apply transform
            if (!currentC.get<bool>("transform_do_not_apply")) {
                currentC.update_transform("transform", theraGO.transform, true);
            }
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        protected override void update() {
            update_from_current_config();
        }

        protected override void set_visibility(bool visible) {
            theraGO.SetActive(visible);
        }

        #endregion
        #region private_functions
        #endregion
        #region public_functions
        #endregion
    }
}

