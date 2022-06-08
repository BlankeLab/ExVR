
/***********************************************************************************
** exvr-designer                                                                  **
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

#pragma once

// qt-utility
#include "gui/ex_widgets/ex_combo_box_index_w.hpp"
#include "gui/ex_widgets/ex_select_color_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_float_spin_box_w.hpp"
#include "gui/ex_widgets/ex_vector3d_w.hpp"
#include "gui/ex_widgets/ex_radio_button_w.hpp"

// local
#include "gui/ex_widgets/ex_resource_w.hpp"
#include "config_pw.hpp"

namespace tool::ex {

class HumanoidControllerInitConfigParametersW : public ConfigParametersW{

private:

    ExResourceW humanoidAssetBundle{"humanoid"};
    ExComboBoxIndexW skinColor{"skin_color"};
    ExSelectColorW shirtColor{"shirt_color"};
    ExSelectColorW pantsColor{"pants_color"};
    ExCheckBoxW addHmdMesh{"add_hmd_mesh"};
    ExCheckBoxW callStartRoutineOnce{"only_call_start_routine_once"};

public:

    void insert_widgets() override;
    void init_and_register_widgets() override;
    void create_connections() override;
    void late_update_ui() override;
};

class HumanoidControllerConfigParametersW : public ConfigParametersW{

private:

    QTabWidget *tabPositionsW = nullptr;
    QWidget *globalW = nullptr;
    QWidget *stiffnessW = nullptr;
    QWidget *spineW = nullptr;
    QWidget *armsW = nullptr;
    QWidget *legsW = nullptr;
    QWidget *leftFingersW = nullptr;
    QWidget *rightFingersW = nullptr;

    // global
    ExCheckBoxW enableInverseKinematics {"enable_ik"};
    TransformSubPart transform {"transform"};
    ExCheckBoxW displayTargets  {"display_targets"};
    ExCheckBoxW moveEyeCameraToHead {"move_eye_camera_to_head"};
    ExCheckBoxW pitch {"pitch"};
    ExCheckBoxW yaw {"yaw"};
    ExCheckBoxW roll  {"roll"};

    ExCheckBoxW moveTransformsToTargets  {"move_transforms_to_targets"};
    QButtonGroup group1;
    ExRadioButtonW updateHeadWithEyeCamera  {"update_head_with_eye_camera"};
    ExRadioButtonW updateHeadWithRelativeEyeCamera {"update_head_with_relative_eye_camera"};
    ExRadioButtonW updateEyeCameraWithHead {"update_eye_camera_with_head"};
    ExCheckBoxW useInputForMovingHeadCameraToRelativeEye {"use_input_connection"};
    ExRadioButtonW doNotMove {"do_not_move"};

    ExFloatSpinBoxW globalIkWeight {"global_ik_w"};
    ExVector3dW headTrackingOffsetPos = {"head_tracking_offset_pos"};
    ExVector3dW headTrackingOffsetRot = {"head_tracking_offset_rot"};

    QButtonGroup group2;
    ExRadioButtonW pitchYawRollRot = {"pitch_yaw_roll"};
    ExRadioButtonW yawRollPitchRot = {"yaw_roll_pitch"};

    // spine
    ExFloatSpinBoxW m_headRotationWeight {"head_rot_w"};
    ExFloatSpinBoxW m_headPositionWeight {"head_pos_w"};
    ExFloatSpinBoxW m_pelvisRotationWeight {"pelvis_rot_w"};
    ExFloatSpinBoxW m_pelvisPositionWeight {"pelvis_pos_w"};
    ExFloatSpinBoxW m_chestGoalWeight {"chest_goal_w"};
    ExVector3dW m_chestGoal = {"chest"};
    TransformSubPart m_head = {"head_transform"};
    TransformSubPart m_pelvis = {"pelvis_transform"};

    ExFloatSpinBoxW m_bodyPosStiffness {"body_pos_stiffness"};         //  Range(0 1 0.55) Determines how much the body will follow the position of the head.
    ExFloatSpinBoxW m_bodyRotStiffness {"body_rot_stiffness"};         //  Range(0 1 0.1) Determines how much the body will follow the rotation of the head.
    ExFloatSpinBoxW m_neckStiffness {"neck_stiffness"};            // Range(0 1 0.2)  Determines how much the chest will rotate to the rotation of the head.
    ExFloatSpinBoxW m_maintainPelvisPosition {"maintain_pelvis_position"};   // Range(0 1 0.2)  How much will the pelvis maintain it's animated position?
    ExFloatSpinBoxW m_maxRootAngle {"max_root_angle"};

    // arms
    ExFloatSpinBoxW m_leftArmRotationWeight {"left_arm_rot_w"};
    ExFloatSpinBoxW m_leftArmPositionWeight {"left_arm_pos_w"};
    ExFloatSpinBoxW m_rightArmRotationWeight {"right_arm_rot_w"};
    ExFloatSpinBoxW m_rightArmPositionWeight {"right_arm_pos_w"};
    ExFloatSpinBoxW m_leftArmBendWeight {"left_arm_bend_w"};
    ExFloatSpinBoxW m_rightArmBendWeight {"right_arm_bend_w"};
    ExVector3dW m_leftArmBend = {"left_arm_bend_goal"};
    ExVector3dW m_rightArmBend = {"right_arm_bend_goal"};
    TransformSubPart m_leftArm = {"left_arm_transform"};
    TransformSubPart m_rightArm = {"right_arm_transform"};

    ExFloatSpinBoxW m_leftShoulderRotationWeight {"left_shoulder_rotation_weight"};
    ExFloatSpinBoxW m_rightShoulderRotationWeight {"right_shoulder_rotation_weight"};
    ExFloatSpinBoxW m_leftSwivelRotationOffset = {"left_swivel_rotation_offset"};
    ExFloatSpinBoxW m_rightSwivelRotationOffset = {"right_swivel_rotation_offset"};

    // legs
    ExFloatSpinBoxW m_leftLegRotationWeight {"left_leg_rot_w"};
    ExFloatSpinBoxW m_leftLegPositionWeight {"left_leg_pos_w"};
    ExFloatSpinBoxW m_rightLegRotationWeight {"right_leg_rot_w"};
    ExFloatSpinBoxW m_rightLegPositionWeight {"right_leg_pos_w"};
    ExFloatSpinBoxW m_leftLegBendWeight {"left_leg_bend_w"};
    ExFloatSpinBoxW m_rightLegBendWeight {"right_leg_bend_w"};
    ExVector3dW m_leftLegBend = {"left_leg_bend_goal"};
    ExVector3dW m_rightLegBend = {"right_leg_bend_goal"};
    TransformSubPart m_leftLeg = {"left_leg_transform"};
    TransformSubPart m_rightLeg = {"right_leg_transform"};

    // fingers
    ExFloatSpinBoxW m_leftFingersWeight {"left_fingers_w"};
    ExVector3dW m_leftIndexRot1 {"left_hand_index_1"};
    ExVector3dW m_leftIndexRot2 {"left_hand_index_2"};
    ExVector3dW m_leftIndexRot3 {"left_hand_index_3"};
    ExVector3dW m_leftMiddleRot1 {"left_hand_middle_1"};
    ExVector3dW m_leftMiddleRot2 {"left_hand_middle_2"};
    ExVector3dW m_leftMiddleRot3 {"left_hand_middle_3"};
    ExVector3dW m_leftRingRot1 {"left_hand_ring_1"};
    ExVector3dW m_leftRingRot2 {"left_hand_ring_2"};
    ExVector3dW m_leftRingRot3 {"left_hand_ring_3"};
    ExVector3dW m_leftPinkyRot1 {"left_hand_pinky_1"};
    ExVector3dW m_leftPinkyRot2 {"left_hand_pinky_2"};
    ExVector3dW m_leftPinkyRot3 {"left_hand_pinky_3"};
    ExVector3dW m_leftThumbRot1 {"left_hand_thumb_1"};
    ExVector3dW m_leftThumbRot2 {"left_hand_thumb_2"};
    ExVector3dW m_leftThumbRot3 {"left_hand_thumb_3"};
    ExFloatSpinBoxW m_rightFingersWeight {"right_fingers_w"};
    ExVector3dW m_rightIndexRot1 {"right_hand_index_1"};
    ExVector3dW m_rightIndexRot2 {"right_hand_index_2"};
    ExVector3dW m_rightIndexRot3 {"right_hand_index_3"};
    ExVector3dW m_rightMiddleRot1 {"right_hand_middle_1"};
    ExVector3dW m_rightMiddleRot2 {"right_hand_middle_2"};
    ExVector3dW m_rightMiddleRot3 {"right_hand_middle_3"};
    ExVector3dW m_rightRingRot1 {"right_hand_ring_1"};
    ExVector3dW m_rightRingRot2 {"right_hand_ring_2"};
    ExVector3dW m_rightRingRot3 {"right_hand_ring_3"};
    ExVector3dW m_rightPinkyRot1 {"right_hand_pinky_1"};
    ExVector3dW m_rightPinkyRot2 {"right_hand_pinky_2"};
    ExVector3dW m_rightPinkyRot3 {"right_hand_pinky_3"};
    ExVector3dW m_rightThumbRot1 {"right_hand_thumb_1"};
    ExVector3dW m_rightThumbRot2 {"right_hand_thumb_2"};
    ExVector3dW m_rightThumbRot3 {"right_hand_thumb_3"};


public :

    void insert_widgets() override;

    void init_and_register_widgets() override;

    void create_connections() override;
    void late_update_ui() override;
};

}

