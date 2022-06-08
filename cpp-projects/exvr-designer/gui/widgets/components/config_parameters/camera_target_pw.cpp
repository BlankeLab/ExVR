
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

#include "camera_target_pw.hpp"

// qt-utility
#include "gui/ex_widgets/ex_float_spin_box_w.hpp"
#include "gui/ex_widgets/ex_radio_button_w.hpp"
#include "gui/ex_widgets/ex_vector3d_w.hpp"
#include "gui/ex_widgets/ex_curve_w.hpp"
#include "gui/ex_widgets/ex_line_edit_w.hpp"

using namespace tool::ex;

struct CameraTargetConfigParametersW::Impl{

    // settings
    ExFloatSpinBoxW duration{"duration"};
    ExCheckBoxW sphericalInterpolation{"spherical_linear_interpolation"};


    ExCheckBoxW pitch{"pitch"};
    ExCheckBoxW yaw{"yaw"};
    ExCheckBoxW roll{"roll"};

    // # choice
    QButtonGroup   buttonGroup1;
    ExRadioButtonW moveToTarget{"move_to_target"};
    ExRadioButtonW moveBack{"move_back"};
    ExRadioButtonW doNothing{"do_nothing"};
    ExCheckBoxW teleport{"teleport"};
    ExCheckBoxW doNotSaveTraj{"do_not_save_traj"};

    QButtonGroup   buttonGroup2;
    ExRadioButtonW useNeutralCamera {"use_neutral"};
    ExRadioButtonW useEyeCamera{"use_eye"};

    // # move to target
    ExLineEditW componentName{"target_component"};
    QButtonGroup   buttonGroup3;
    ExRadioButtonW relatetiveToEye{"relative_to_eye"};
    ExRadioButtonW absolute{"absolute"};
    ExVector3dW targetPos{"target_pos"};
    ExVector3dW targetRot{"target_rot"};

    QButtonGroup   buttonGroup4;
    ExRadioButtonW usingTime{"use_time"};
    ExRadioButtonW usingFactor{"use_factor"};
    ExFloatSpinBoxW factorOffset{"factor_offset"};
    ExFloatSpinBoxW factorFactor{"factor_factor"};

    // curves
    QTabWidget curves;
    ExCurveW speedCurve{"speed_curve"};
};

CameraTargetConfigParametersW::CameraTargetConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void CameraTargetConfigParametersW::insert_widgets(){

    add_widget(ui::W::txt("<b>Action</b>"));
    add_widget(ui::F::gen(ui::L::HB(), {m_p->moveToTarget(), m_p->moveBack(), m_p->doNothing()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {m_p->teleport(), m_p->doNotSaveTraj()}, LStretch{true}, LMargins{false},QFrame::NoFrame));

    add_widget(ui::W::txt("<b>Camera</b>"));
    add_widget(ui::F::gen(ui::L::HB(), {m_p->useNeutralCamera(), m_p->useEyeCamera()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {m_p->pitch(), m_p->yaw(), m_p->roll()},LStretch{true}, LMargins{false},QFrame::NoFrame));

    add_widget(ui::W::txt("<b>Progress</b>"));
    add_widget(ui::F::gen(ui::L::HB(), {m_p->usingTime(), ui::W::txt("Movement duration: "), m_p->duration()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {m_p->usingFactor(),
            ui::W::txt("add: "), m_p->factorOffset(), ui::W::txt("multiply by: "), m_p->factorFactor()}, LStretch{true}, LMargins{false},QFrame::NoFrame));

    add_widget(ui::W::txt("<b>Interpolation</b>"));
    add_widget(ui::F::gen(ui::L::HB(), {m_p->sphericalInterpolation()}, LStretch{true}, LMargins{false},QFrame::NoFrame));

    add_widget(ui::W::txt("<b>Target</b>"));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Name of the component position to match: "), m_p->componentName()}, LStretch{false}, LMargins{false}));
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("If name empty, use position and rotation below:"), m_p->targetPos(), m_p->targetRot()}, LStretch{false}, LMargins{false}));
    add_widget(ui::F::gen(ui::L::HB(), {m_p->absolute(), m_p->relatetiveToEye()}, LStretch{true}, LMargins{false}));
    add_widget(&m_p->curves);

    m_p->curves.addTab(m_p->speedCurve(), "Factor curve");
}


void CameraTargetConfigParametersW::init_and_register_widgets(){

    // settings
    add_input_ui(m_p->duration.init_widget(MinV<qreal>{0}, V<qreal>{5.}, MaxV<qreal>{500.}, StepV<qreal>{0.1}, 2, true));
    add_input_ui(m_p->sphericalInterpolation.init_widget("Use spherical linear interpolation ", true));

    // axies
    add_input_ui(m_p->pitch.init_widget("Use pitch", false));
    add_input_ui(m_p->yaw.init_widget("Use yaw", true));
    add_input_ui(m_p->roll.init_widget("use roll", false));

    // actions
    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->buttonGroup1,
            {&m_p->moveToTarget, &m_p->moveBack, &m_p->doNothing},
            {
                "Move to target",
                "Move back to previous target",
                "Do nothing"
            },
            {true, false, false}
        )
    );
    add_input_ui(m_p->teleport.init_widget("Teleport", false));
    add_input_ui(m_p->doNotSaveTraj.init_widget("Do not save trajectory", false));

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->buttonGroup3,
            {&m_p->relatetiveToEye, &m_p->absolute},
            {
                "Relative to eye",
                "Absolute"
            },
            {false, true}
        )
    );

    // cameras
    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->buttonGroup2,
            {&m_p->useNeutralCamera, &m_p->useEyeCamera},
            {
                "Use start neutral camera",
                "Use eye camera"
            },
            {true, false}
        )
    );

    // progress
    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->buttonGroup4,
            {&m_p->usingTime, &m_p->usingFactor},
            {
                "Use elapsed time",
                "Use input factor [0-1]"
            },
            {true, false}
        )
    );
    add_input_ui(m_p->factorOffset.init_widget(MinV<qreal>{-100}, V<qreal>{0.}, MaxV<qreal>{100.}, StepV<qreal>{0.1}, 2, true));
    add_input_ui(m_p->factorFactor.init_widget(MinV<qreal>{0}, V<qreal>{1.}, MaxV<qreal>{100.}, StepV<qreal>{0.1}, 2, true));

    // move to target
    add_input_ui(m_p->componentName.init_widget(""));
    DsbSettings s1 = {MinV<qreal>{-10000.}, V<qreal>{0.},MaxV<qreal>{10000.}, StepV<qreal>{0.1}, 3};
    add_input_ui(m_p->targetPos.init_widget("Position: ", Vector3dSettings{s1,s1,s1}, true));
    add_input_ui(m_p->targetRot.init_widget("Rotation: ", Vector3dSettings{s1,s1,s1}, true));

    // curves
    add_input_ui(m_p->speedCurve.init_widget("Factor speed curve"));
    m_p->speedCurve.minX.w->setEnabled(false);
    m_p->speedCurve.maxX.w->setEnabled(false);
    m_p->speedCurve.minY.w->setEnabled(false);
    m_p->speedCurve.maxY.w->setEnabled(false);
    m_p->speedCurve.firstY.w->setMinimum(0.);
    m_p->speedCurve.firstY.w->setMaximum(1.);
    m_p->speedCurve.lastY.w->setMinimum(0.);
    m_p->speedCurve.lastY.w->setMaximum(1.);
    m_p->speedCurve.currentCurveId.w->setEnabled(false);
    m_p->speedCurve.currentCurveId.w->setValue(0);
}
