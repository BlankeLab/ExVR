
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

#include "humanoid_controller_pw.hpp"


using namespace tool::ex;

void HumanoidControllerInitConfigParametersW::insert_widgets(){

    auto bundle = ui::F::gen(ui::L::HB(), {humanoidAssetBundle()}, LStretch{false}, LMargins{true}, QFrame::NoFrame);
    auto skin = ui::F::gen(ui::L::HB(), {ui::W::txt("Skin:"), skinColor()}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    auto colors = ui::F::gen(ui::L::HB(), {ui::W::txt("Shirt:"), shirtColor(), ui::W::txt("Pants:"), pantsColor()}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    add_widget(
        ui::F::gen(ui::L::VB(), {
            bundle,
            ui::W::horizontal_line(),
            ui::W::txt("Colors"),
            skin,
            colors,
            ui::W::horizontal_line(),
            callStartRoutineOnce(),
            addHmdMesh(),
        },
        LStretch{false}, LMargins{true}, QFrame::Box)
    );
}

void HumanoidControllerInitConfigParametersW::init_and_register_widgets(){
    add_input_ui(humanoidAssetBundle.init_widget(Resource::Type::AssetBundle, "Humanoid asset bundle resource: "));
    QStringList items;
    items
        << "253, 231, 173"
        << "212, 158, 122"
        << "212, 158, 122"
        << "178, 102, 68"
        << "248, 217, 152"
        << "236, 192, 145"
        << "173, 138, 96"
        << "127, 68, 34"
        << "254, 227, 197"
        << "242, 194, 128"
        << "147, 95, 55"
        << "95, 51, 16"
        << "249, 212, 160"
        << "187, 101, 54"
        << "115, 63, 23"
        << "41, 23, 9"
        << "original mesh";

    add_input_ui(skinColor.init_widget(items, 16));

    std::vector<QColor> colors;
    colors.emplace_back(253, 231, 173, 255);
    colors.emplace_back(212, 158, 122, 255);
    colors.emplace_back(212, 158, 122, 255);
    colors.emplace_back(178, 102, 68, 255);
    colors.emplace_back(248, 217, 152, 255);
    colors.emplace_back(236, 192, 145, 255);
    colors.emplace_back(173, 138, 96, 255);
    colors.emplace_back(127, 68, 34, 255);
    colors.emplace_back(254, 227, 197, 255);
    colors.emplace_back(242, 194, 128, 255);
    colors.emplace_back(147, 95, 55, 255);
    colors.emplace_back(95, 51, 16, 255);
    colors.emplace_back(249, 212, 160, 255);
    colors.emplace_back(187, 101, 54, 255);
    colors.emplace_back(115, 63, 23, 255);
    colors.emplace_back(41, 23, 9, 255);

    QPixmap p(25,25);
    for(size_t ii = 0; ii < colors.size(); ++ii){
        p.fill(colors[ii]);
        skinColor.w->setItemIcon(static_cast<int>(ii), QIcon(p));
    }

    add_input_ui(shirtColor.init_widget("Choose shirt color to apply on texture", QColor(255,255,255,255)));
    add_input_ui(pantsColor.init_widget("Choose Pants color to apply on texture", QColor(255,255,255,255)));
    add_input_ui(addHmdMesh.init_widget("Add VR HMD mesh on head:", false));
    add_input_ui(callStartRoutineOnce.init_widget("Call start_routine only once", false));
}

void HumanoidControllerInitConfigParametersW::create_connections(){
}

void HumanoidControllerInitConfigParametersW::late_update_ui(){}

void HumanoidControllerConfigParametersW::insert_widgets(){

    // tab
    add_widget(tabPositionsW = new QTabWidget());
    tabPositionsW->setTabPosition(QTabWidget::TabPosition::North);
    tabPositionsW->addTab(globalW = new QWidget(), "Global");
    tabPositionsW->addTab(stiffnessW = new QWidget(), "Stiffness");
    tabPositionsW->addTab(spineW = new QWidget(), "Spine");
    tabPositionsW->addTab(armsW = new QWidget(), "Arms");
    tabPositionsW->addTab(legsW = new QWidget(), "Legs");
    tabPositionsW->addTab(leftFingersW = new QWidget(), "L. fingers");
    tabPositionsW->addTab(rightFingersW = new QWidget(), "R. fingers");

    // global
    QVBoxLayout *globalL = nullptr;
    globalW->setLayout(globalL = ui::L::VB());
    globalL->addWidget(ui::F::gen(ui::L::HB(), {enableInverseKinematics(),ui::W::txt("Global IK weight: "), globalIkWeight()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    globalL->addWidget(ui::W::horizontal_line());
    globalL->addWidget(transform.frame);
    globalL->addWidget(ui::F::gen(ui::L::HB(), {moveTransformsToTargets()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    globalL->addWidget(ui::F::gen(ui::L::HB(), {moveEyeCameraToHead()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    globalL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("  use: "), pitch(), yaw(), roll()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));

    globalL->addWidget(ui::W::horizontal_line());
    globalL->addWidget(
        ui::F::gen(ui::L::VB(), {
            updateHeadWithEyeCamera(),
            updateHeadWithRelativeEyeCamera(),
            ui::F::gen(ui::L::HB(), {ui::W::txt("  "), useInputForMovingHeadCameraToRelativeEye()}, LStretch{true}, LMargins{false}, QFrame::NoFrame),
            updateEyeCameraWithHead(),
            doNotMove()
        },
        LStretch{true}, LMargins{false}, QFrame::NoFrame)
    );

    globalL->addWidget(ui::W::horizontal_line());
    globalL->addWidget(ui::F::gen(ui::L::HB(), {headTrackingOffsetPos()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    globalL->addWidget(ui::F::gen(ui::L::HB(), {headTrackingOffsetRot()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    globalL->addWidget(ui::W::horizontal_line());
    globalL->addWidget(ui::F::gen(ui::L::HB(), {displayTargets()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    globalL->addWidget(ui::F::gen(ui::L::VB(), {pitchYawRollRot(), yawRollPitchRot()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    globalL->addStretch();

    // stiffness
    QVBoxLayout *stiffnessL = nullptr;
    stiffnessW->setLayout(stiffnessL = ui::L::VB());
    stiffnessL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("<b>Determines how much: </b>")}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    stiffnessL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("... the body will follow the position of the head: "), m_bodyPosStiffness()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    stiffnessL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("... the body will follow the rotation of the head: "), m_bodyRotStiffness()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    stiffnessL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("... the chest will rotate to the rotation of the head: "), m_neckStiffness()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    stiffnessL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("... the pelvis maintain it's animated position: "), m_maintainPelvisPosition()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    stiffnessL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Max head rotation angle before turning root: "), m_maxRootAngle()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    stiffnessL->addStretch();

    // spine
    QVBoxLayout *spineL = nullptr;
    spineW->setLayout(spineL = ui::L::VB());
    spineL->addWidget(m_head.frame);
    spineL->addWidget(
        ui::F::gen(ui::L::HB(), {
            ui::W::txt("<b>Head weights</b>"),
            ui::W::txt("position: "), m_headPositionWeight(),
            ui::W::txt("rotation: "), m_headRotationWeight()
        },
        LStretch{true}, LMargins{true}, QFrame::Box)
    );
    spineL->addWidget(m_pelvis.frame);
    spineL->addWidget(
        ui::F::gen(ui::L::HB(), {
            ui::W::txt("<b>Pelvis weights</b>"),
            ui::W::txt("position: "), m_pelvisPositionWeight(),
            ui::W::txt("rotation: "), m_pelvisRotationWeight()
        },
        LStretch{true}, LMargins{true}, QFrame::Box)
    );
    spineL->addWidget(ui::F::gen(ui::L::HB(), {m_chestGoal(), ui::W::txt("weight: "), m_chestGoalWeight()}, LStretch{true}, LMargins{true}, QFrame::Box));
    spineL->addStretch();

    // arms
    QVBoxLayout *amrsL = nullptr;
    armsW->setLayout(amrsL = ui::L::VB());
    amrsL->addWidget(m_leftArm.frame);
    amrsL->addWidget(
        ui::F::gen(ui::L::HB(), {
            ui::W::txt("<b>Left arm weights</b>"),
            ui::W::txt("position: "), m_leftArmPositionWeight(),
            ui::W::txt("rotation: "), m_leftArmRotationWeight()
        },
        LStretch{true}, LMargins{true}, QFrame::Box)
    );
    amrsL->addWidget(
        ui::F::gen(ui::L::HB(), {
            ui::W::txt("Left shoulder rot weight: "), m_leftShoulderRotationWeight(),
            ui::W::txt("Left swivel rot offset: "), m_leftSwivelRotationOffset()
        },
        LStretch{true}, LMargins{true}, QFrame::Box)
    );

    amrsL->addWidget(
        ui::F::gen(ui::L::HB(), {
            m_leftArmBend(),
            ui::W::txt("weight: "), m_leftArmBendWeight()
        },
        LStretch{true}, LMargins{true}, QFrame::Box)
    );

    amrsL->addWidget(m_rightArm.frame);
    amrsL->addWidget(
        ui::F::gen(ui::L::HB(), {
            ui::W::txt("<b>Right arm weights</b>"),
            ui::W::txt("position: "), m_rightArmPositionWeight(),
            ui::W::txt("rotation: "), m_rightArmRotationWeight()
        },
        LStretch{true}, LMargins{true}, QFrame::Box)
    );
    amrsL->addWidget(
        ui::F::gen(ui::L::HB(), {
            ui::W::txt("Right shoulder rot weight: "), m_rightShoulderRotationWeight(),
            ui::W::txt("Right swivel rot offset: "), m_rightSwivelRotationOffset()
        },
        LStretch{true}, LMargins{true}, QFrame::Box)
    );

    amrsL->addWidget(
        ui::F::gen(ui::L::HB(), {
            m_rightArmBend(),
            ui::W::txt("weight: "), m_rightArmBendWeight()
        },
        LStretch{true}, LMargins{true}, QFrame::Box)
    );
    amrsL->addStretch();

    // legs
    QVBoxLayout *legsL = nullptr;
    legsW->setLayout(legsL = ui::L::VB());
    legsL->addWidget(m_leftLeg.frame);
    legsL->addWidget(
        ui::F::gen(ui::L::HB(), {
            ui::W::txt("<b>Left leg weights</b>"),
            ui::W::txt("position: "), m_leftLegPositionWeight(),
            ui::W::txt("rotation: "), m_leftLegRotationWeight()
        },
        LStretch{true}, LMargins{true}, QFrame::Box)
    );
    legsL->addWidget(
        ui::F::gen(ui::L::HB(), {
            m_leftLegBend(),
            ui::W::txt("weight: "), m_leftLegBendWeight()
        },
        LStretch{true}, LMargins{true}, QFrame::Box)
    );

    legsL->addWidget(m_rightLeg.frame);
    legsL->addWidget(
        ui::F::gen(ui::L::HB(), {
            ui::W::txt("<b>Right leg weights</b>"),
            ui::W::txt("position: "), m_rightLegPositionWeight(),
            ui::W::txt("rotation: "), m_rightLegRotationWeight()
        },
        LStretch{true}, LMargins{true}, QFrame::Box)
    );
    legsL->addWidget(
        ui::F::gen(ui::L::HB(), {
            m_rightLegBend(),
            ui::W::txt("weight: "), m_rightLegBendWeight()
        },
        LStretch{true}, LMargins{true}, QFrame::Box)
    );
    legsL->addStretch();

    // left fingers
    QVBoxLayout *leftFingersL = nullptr;
    leftFingersW->setLayout(leftFingersL = ui::L::VB());

    auto leftIndex = ui::F::gen(ui::L::VB(), {
        m_leftIndexRot1(),
        m_leftIndexRot2(),
        m_leftIndexRot3()
    }, LStretch{true}, LMargins{false}, QFrame::NoFrame);

    auto leftMiddle = ui::F::gen(ui::L::VB(), {
        m_leftMiddleRot1(),
        m_leftMiddleRot2(),
        m_leftMiddleRot3()
    }, LStretch{true}, LMargins{false}, QFrame::NoFrame);

    auto leftRing = ui::F::gen(ui::L::VB(), {
        m_leftRingRot1(),
        m_leftRingRot2(),
        m_leftRingRot3()
    }, LStretch{true}, LMargins{false}, QFrame::NoFrame);

    auto leftPinky = ui::F::gen(ui::L::VB(), {
        m_leftPinkyRot1(),
        m_leftPinkyRot2(),
        m_leftPinkyRot3()
    }, LStretch{true}, LMargins{false}, QFrame::NoFrame);

    auto leftThumb = ui::F::gen(ui::L::VB(), {
        m_leftThumbRot1(),
        m_leftThumbRot2(),
        m_leftThumbRot3()
    }, LStretch{true}, LMargins{false}, QFrame::NoFrame);

    leftFingersL->addWidget(
        ui::F::gen(ui::L::VB(), {
            ui::W::txt("<b>Left fingers rotations: </b>"),
            leftIndex,
            ui::W::horizontal_line(),
            leftMiddle,
            ui::W::horizontal_line(),
            leftRing,
            ui::W::horizontal_line(),
            leftPinky,
            ui::W::horizontal_line(),
            leftThumb,
            ui::F::gen(ui::L::HB(), {
               ui::W::txt("<b>Weight: </b>"), m_leftFingersWeight()
            },
            LStretch{true}, LMargins{false}, QFrame::NoFrame)
        },
        LStretch{true}, LMargins{true}, QFrame::Box)
    );

    // right fingers
    QVBoxLayout *rightFingersL = nullptr;
    rightFingersW->setLayout(rightFingersL = ui::L::VB());

    auto rightIndex = ui::F::gen(ui::L::VB(), {
        m_rightIndexRot1(),
        m_rightIndexRot2(),
        m_rightIndexRot3()
    }, LStretch{true}, LMargins{false}, QFrame::NoFrame);

    auto rightMiddle = ui::F::gen(ui::L::VB(), {
        m_rightMiddleRot1(),
        m_rightMiddleRot2(),
        m_rightMiddleRot3()
    }, LStretch{true}, LMargins{false}, QFrame::NoFrame);

    auto rightRing = ui::F::gen(ui::L::VB(), {
        m_rightRingRot1(),
        m_rightRingRot2(),
        m_rightRingRot3()
    }, LStretch{true}, LMargins{false}, QFrame::NoFrame);

    auto rightPinky = ui::F::gen(ui::L::VB(), {
        m_rightPinkyRot1(),
        m_rightPinkyRot2(),
        m_rightPinkyRot3()
    }, LStretch{true}, LMargins{false}, QFrame::NoFrame);

    auto rightThumb = ui::F::gen(ui::L::VB(), {
        m_rightThumbRot1(),
        m_rightThumbRot2(),
        m_rightThumbRot3()
    }, LStretch{true}, LMargins{false}, QFrame::NoFrame);

    rightFingersL->addWidget(
        ui::F::gen(ui::L::VB(), {
            ui::W::txt("<b>Right fingers rotations: </b>"),
            rightIndex,
            ui::W::horizontal_line(),
            rightMiddle,
            ui::W::horizontal_line(),
            rightRing,
            ui::W::horizontal_line(),
            rightPinky,
            ui::W::horizontal_line(),
            rightThumb,
            ui::F::gen(ui::L::HB(), {
               ui::W::txt("<b>Weight: </b>"),m_rightFingersWeight()
            },
            LStretch{true}, LMargins{false}, QFrame::NoFrame)
        },
        LStretch{true}, LMargins{true}, QFrame::Box)
    );
}

void HumanoidControllerConfigParametersW::init_and_register_widgets(){

    map_sub_part(transform.init_widget());
    add_input_ui(displayTargets.init_widget("Display targets", false));
    add_input_ui(moveTransformsToTargets.init_widget("Move avatar to fit targets (called at routine start)", true));
    add_input_ui(moveEyeCameraToHead.init_widget("Move eye camera to fit avatar head with neutral position (called at routine start)", false));

    add_input_ui(pitch.init_widget("pitch", false));
    add_input_ui(yaw.init_widget("yaw", true));
    add_input_ui(roll.init_widget("roll", false));


    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(group1,
            {&updateHeadWithEyeCamera, &updateHeadWithRelativeEyeCamera, &updateEyeCameraWithHead, &doNotMove},
            {
                "Update avatar head with eye camera (called every frame)",
                "Update avatar head with relative eye camera (called every frame)",
                "Update eye camera with avatar head (called every frame)",
                "Do not move head/eye camera"
            },
            {false, false, false, true}
        )
    );

    add_input_ui(useInputForMovingHeadCameraToRelativeEye.init_widget("Use input connections instead of eye camera", false));

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(group2,
            {&pitchYawRollRot, &yawRollPitchRot},
            {
                "Use Pitch/Yaw/Roll axis order",
                "Use Yaw/Roll/Pitch axis order",
            },
            {true, false}
        )
    );

    DsbSettings offsetS{MinV<qreal>{-2.},V<qreal>{0.},MaxV<qreal>{2.},StepV<qreal>{0.01}, 2};
    add_input_ui(headTrackingOffsetPos.init_widget("Head offset position:", Vector3dSettings{offsetS,offsetS,offsetS}));
    headTrackingOffsetPos.x.w->setValue(0);
    headTrackingOffsetPos.y.w->setValue(0.1);
    headTrackingOffsetPos.z.w->setValue(0.1);

    DsbSettings offsetRot{MinV<qreal>{-180.},V<qreal>{0.},MaxV<qreal>{180.},StepV<qreal>{0.1}, 2};
    Vector3dSettings offsetVecRot = {offsetRot,offsetRot,offsetRot};
    add_input_ui(headTrackingOffsetRot.init_widget("Head offset rotation:", offsetVecRot));

    map_sub_part(m_head.init_widget("Head target transform"));
    m_head.set_enable_state(true,true,false);
    map_sub_part(m_pelvis.init_widget("Pelvis target transform"));
    m_pelvis.set_enable_state(true,true,false);
    map_sub_part(m_leftArm.init_widget("Left arm target transform"));
    m_leftArm.set_enable_state(true,true,false);
    map_sub_part(m_rightArm.init_widget("Right arm target transform"));
    m_rightArm.set_enable_state(true,true,false);
    map_sub_part(m_leftLeg.init_widget("Left leg target transform"));
    m_leftLeg.set_enable_state(true,true,false);
    map_sub_part(m_rightLeg.init_widget("Right leg target transform"));
    m_rightLeg.set_enable_state(true,true,false);

    add_input_ui(m_chestGoal.init_widget("<b>Chest goal</b>")); // translation
    add_input_ui(m_leftArmBend.init_widget("<b>Left arm bend goal</b>")); // translation
    add_input_ui(m_rightArmBend.init_widget("<b>Right arm bend goal</b>")); // translation
    add_input_ui(m_leftLegBend.init_widget("<b>Left leg bend goal</b>")); // translation
    add_input_ui(m_rightLegBend.init_widget("<b>Right leg bend goal</b>")); // translation

    add_input_ui(m_leftSwivelRotationOffset.init_widget(offsetRot));
    add_input_ui(m_rightSwivelRotationOffset.init_widget(offsetRot));

    DsbSettings factorS{MinV<qreal>{0.},V<qreal>{1.},MaxV<qreal>{1.},StepV<qreal>{0.05}, 2};

    add_input_ui(m_leftShoulderRotationWeight.init_widget(factorS));
    add_input_ui(m_rightShoulderRotationWeight.init_widget(factorS));

    add_input_ui(enableInverseKinematics.init_widget("Enable inverse kinematics", true));
    add_input_ui(globalIkWeight.init_widget(factorS));

    add_input_ui(m_headRotationWeight.init_widget(factorS));
    add_input_ui(m_headPositionWeight.init_widget(factorS));
    add_input_ui(m_pelvisRotationWeight.init_widget(factorS));
    add_input_ui(m_pelvisPositionWeight.init_widget(factorS));
    add_input_ui(m_chestGoalWeight.init_widget(factorS));
    m_chestGoalWeight.w->setValue(0.);

    add_input_ui(m_bodyPosStiffness.init_widget(DsbSettings{MinV<qreal>{0.},V<qreal>{0.55},MaxV<qreal>{1.},StepV<qreal>{0.01}, 2}));
    add_input_ui(m_bodyRotStiffness.init_widget(DsbSettings{MinV<qreal>{0.},V<qreal>{0.1},MaxV<qreal>{1.},StepV<qreal>{0.01}, 2}));
    add_input_ui(m_neckStiffness.init_widget(DsbSettings{MinV<qreal>{0.},V<qreal>{0.2},MaxV<qreal>{1.},StepV<qreal>{0.01}, 2}));
    add_input_ui(m_maintainPelvisPosition.init_widget(DsbSettings{MinV<qreal>{0.},V<qreal>{0.2},MaxV<qreal>{1.},StepV<qreal>{0.01}, 2}));
    add_input_ui(m_maxRootAngle.init_widget(DsbSettings{MinV<qreal>{0.},V<qreal>{25.},MaxV<qreal>{180.},StepV<qreal>{0.1}, 2}));

    add_input_ui(m_leftLegRotationWeight.init_widget(factorS));
    add_input_ui(m_leftLegPositionWeight.init_widget(factorS));
    add_input_ui(m_leftLegBendWeight.init_widget(factorS));
    m_leftLegBendWeight.w->setValue(0.);
    add_input_ui(m_rightLegRotationWeight.init_widget(factorS));
    add_input_ui(m_rightLegPositionWeight.init_widget(factorS));
    add_input_ui(m_rightLegBendWeight.init_widget(factorS));
    m_rightLegBendWeight.w->setValue(0.);

    add_input_ui(m_leftArmRotationWeight.init_widget(factorS));
    add_input_ui(m_leftArmPositionWeight.init_widget(factorS));
    add_input_ui(m_leftArmBendWeight.init_widget(factorS));
    m_leftArmBendWeight.w->setValue(0.);
    add_input_ui(m_rightArmRotationWeight.init_widget(factorS));
    add_input_ui(m_rightArmPositionWeight.init_widget(factorS));
    add_input_ui(m_rightArmBendWeight.init_widget(factorS));
    m_rightArmBendWeight.w->setValue(0.);

    add_input_ui(m_leftIndexRot1.init_widget("<b>Index</b> bone 1:", offsetVecRot));
    add_input_ui(m_leftIndexRot2.init_widget("<b>Index</b> bone 2:", offsetVecRot));
    add_input_ui(m_leftIndexRot3.init_widget("<b>Index</b> bone 3:", offsetVecRot));
    m_leftIndexRot1.set_enable_state({true,false,true});
    m_leftIndexRot2.set_enable_state({true,false,false});
    m_leftIndexRot3.set_enable_state({true,false,false});

    add_input_ui(m_leftMiddleRot1.init_widget("<b>Middle</b> bone 1: ", offsetVecRot));
    add_input_ui(m_leftMiddleRot2.init_widget("<b>Middle</b> bone 2: ", offsetVecRot));
    add_input_ui(m_leftMiddleRot3.init_widget("<b>Middle</b> bone 3: ", offsetVecRot));
    m_leftMiddleRot1.set_enable_state({true,false,true});
    m_leftMiddleRot2.set_enable_state({true,false,false});
    m_leftMiddleRot3.set_enable_state({true,false,false});

    add_input_ui(m_leftRingRot1.init_widget("<b>Ring</b> bone 1: ", offsetVecRot));
    add_input_ui(m_leftRingRot2.init_widget("<b>Ring</b> bone 2: ", offsetVecRot));
    add_input_ui(m_leftRingRot3.init_widget("<b>Ring</b> bone 3: ", offsetVecRot));
    m_leftRingRot1.set_enable_state({true,false,true});
    m_leftRingRot2.set_enable_state({true,false,false});
    m_leftRingRot3.set_enable_state({true,false,false});

    add_input_ui(m_leftPinkyRot1.init_widget("<b>Pinky</b> bone 1: ", offsetVecRot));
    add_input_ui(m_leftPinkyRot2.init_widget("<b>Pinky</b> bone 2: ", offsetVecRot));
    add_input_ui(m_leftPinkyRot3.init_widget("<b>Pinky</b> bone 3: ", offsetVecRot));
    m_leftPinkyRot1.set_enable_state({true,false,true});
    m_leftPinkyRot2.set_enable_state({true,false,false});
    m_leftPinkyRot3.set_enable_state({true,false,false});

    add_input_ui(m_leftThumbRot1.init_widget("<b>Thumb</b> bone 1: ", offsetVecRot));
    add_input_ui(m_leftThumbRot2.init_widget("<b>Thumb</b> bone 2: ", offsetVecRot));
    add_input_ui(m_leftThumbRot3.init_widget("<b>Thumb</b> bone 3: ", offsetVecRot));
    m_leftThumbRot1.set_enable_state({true,false,true});
    m_leftThumbRot2.set_enable_state({true,false,false});
    m_leftThumbRot3.set_enable_state({true,false,false});

    add_input_ui(m_rightIndexRot1.init_widget("<b>Index</b> bone 1: ", offsetVecRot));
    add_input_ui(m_rightIndexRot2.init_widget("<b>Index</b> bone 2: ", offsetVecRot));
    add_input_ui(m_rightIndexRot3.init_widget("<b>Index</b> bone 3: ", offsetVecRot));
    m_rightIndexRot1.set_enable_state({true,false,true});
    m_rightIndexRot2.set_enable_state({true,false,false});
    m_rightIndexRot3.set_enable_state({true,false,false});

    add_input_ui(m_rightMiddleRot1.init_widget("<b>Middle</b> bone 1: ", offsetVecRot));
    add_input_ui(m_rightMiddleRot2.init_widget("<b>Middle</b> bone 2: ", offsetVecRot));
    add_input_ui(m_rightMiddleRot3.init_widget("<b>Middle</b> bone 3: ", offsetVecRot));
    m_rightMiddleRot1.set_enable_state({true,false,true});
    m_rightMiddleRot2.set_enable_state({true,false,false});
    m_rightMiddleRot3.set_enable_state({true,false,false});

    add_input_ui(m_rightRingRot1.init_widget("<b>Ring</b> bone 1: ", offsetVecRot));
    add_input_ui(m_rightRingRot2.init_widget("<b>Ring</b> bone 2: ", offsetVecRot));
    add_input_ui(m_rightRingRot3.init_widget("<b>Ring</b> bone 3: ", offsetVecRot));
    m_rightRingRot1.set_enable_state({true,false,true});
    m_rightRingRot2.set_enable_state({true,false,false});
    m_rightRingRot3.set_enable_state({true,false,false});

    add_input_ui(m_rightPinkyRot1.init_widget("<b>Pinky</b> bone 1: ", offsetVecRot));
    add_input_ui(m_rightPinkyRot2.init_widget("<b>Pinky</b> bone 2: ", offsetVecRot));
    add_input_ui(m_rightPinkyRot3.init_widget("<b>Pinky</b> bone 3: ", offsetVecRot));
    m_rightPinkyRot1.set_enable_state({true,false,true});
    m_rightPinkyRot2.set_enable_state({true,false,false});
    m_rightPinkyRot3.set_enable_state({true,false,false});

    add_input_ui(m_rightThumbRot1.init_widget("<b>Thumb</b> bone 1: ", offsetVecRot));
    add_input_ui(m_rightThumbRot2.init_widget("<b>Thumb</b> bone 2: ", offsetVecRot));
    add_input_ui(m_rightThumbRot3.init_widget("<b>Thumb</b> bone 3: ", offsetVecRot));
    m_rightThumbRot1.set_enable_state({true,false,true});
    m_rightThumbRot2.set_enable_state({true,false,false});
    m_rightThumbRot3.set_enable_state({true,false,false});

    add_input_ui(m_leftFingersWeight.init_widget(factorS));
    add_input_ui(m_rightFingersWeight.init_widget(factorS));
}

void HumanoidControllerConfigParametersW::create_connections(){

    connect(updateHeadWithEyeCamera.w.get(), &QRadioButton::clicked, this, [&]{
        useInputForMovingHeadCameraToRelativeEye.w->setEnabled(updateHeadWithRelativeEyeCamera.w->isChecked());

    });
    connect(updateHeadWithRelativeEyeCamera.w.get(), &QRadioButton::clicked, this, [&]{
        useInputForMovingHeadCameraToRelativeEye.w->setEnabled(updateHeadWithRelativeEyeCamera.w->isChecked());

    });
    connect(doNotMove.w.get(), &QRadioButton::clicked, this, [&]{
        useInputForMovingHeadCameraToRelativeEye.w->setEnabled(updateHeadWithRelativeEyeCamera.w->isChecked());

    });
}

void HumanoidControllerConfigParametersW::late_update_ui(){
    useInputForMovingHeadCameraToRelativeEye.w->setEnabled(updateHeadWithRelativeEyeCamera.w->isChecked());
}
