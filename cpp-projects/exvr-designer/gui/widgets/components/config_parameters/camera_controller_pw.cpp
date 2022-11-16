
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

#include "camera_controller_pw.hpp"


// qt-utility
#include "gui/ex_widgets/ex_radio_button_w.hpp"
#include "gui/ex_widgets/ex_vector3d_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"


using namespace tool::ex;

struct CameraControllerInitConfigParametersW::Impl{
    QButtonGroup group1;
    ExRadioButtonW useEye {"use_eye"};
    ExRadioButtonW useNeutral {"use_neutral"};
    ExCheckBoxW cbStartExperiment {"start_experiment"};
    ExVector3dW position {"position"};
    ExVector3dW rotation {"rotation"};
};

CameraControllerInitConfigParametersW::CameraControllerInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void CameraControllerInitConfigParametersW::insert_widgets(){

    add_widget(ui::W::txt("<b>Initialization camera</b>"));

    add_widget(
        ui::F::gen(ui::L::VB(), {
            ui::W::txt("<b>Apply to:</b>"),
            m_p->useNeutral(),
            m_p->useEye(),
            ui::W::horizontal_line(),
            ui::W::txt("<b>Apply when:</b>"),
            m_p->cbStartExperiment(),
            ui::W::horizontal_line(),
            ui::W::txt("<b>Values:</b>"),
            m_p->position(),
            m_p->rotation()
        },
        LStretch{false}, LMargins{true}, QFrame::Box)
    );
}

void CameraControllerInitConfigParametersW::init_and_register_widgets(){

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
            {&m_p->useNeutral, &m_p->useEye},
            {
                "calibration",
                "eye camera",
            },
            {true, false}
        )
    );

    add_input_ui(m_p->cbStartExperiment.init_widget("Start experiment", true, true));

    DsbSettings offsetPos{MinV<qreal>{-1000.},V<qreal>{0.},MaxV<qreal>{1000.},StepV<qreal>{0.01}, 3};
    DsbSettings offsetRot{MinV<qreal>{-360.},V<qreal>{0.},MaxV<qreal>{360.},StepV<qreal>{0.1}, 3};
    add_input_ui(m_p->position.init_widget("position: ", {offsetPos,offsetPos,offsetPos}));
    add_input_ui(m_p->rotation.init_widget("rotation: ", {offsetRot,offsetRot,offsetRot}));
}

void CameraControllerInitConfigParametersW::create_connections(){}

void CameraControllerInitConfigParametersW::late_update_ui(){}


struct CameraControllerConfigParametersW::Impl{
    QButtonGroup group1;
    ExRadioButtonW useEye {"use_eye"};
    ExRadioButtonW useNeutral {"use_neutral"};
    ExCheckBoxW cbStartRoutine {"start_routine"};
    ExCheckBoxW cbUpdateOn {"update_on"};

    QPushButton *copyNeutralToCurrentConfig  = nullptr;
    QPushButton *copyEyeToCurrentConfig = nullptr;
    ExVector3dW position {"position"};
    ExVector3dW rotation {"rotation"};
    ExVector3dW currentNeutralPosition {"neutral_position"};
    ExVector3dW currentNeutralRotation {"neutral_rotation"};
    ExVector3dW currentEyePosition {"eye_rotation"};
    ExVector3dW currentEyeRotation {"eye_rotation"};

    ExCheckBoxW enableDebugMouseCameraMovements {"debug_camera"};
    ExFloatSpinBoxW mSpeed {"m_speed"};
    ExFloatSpinBoxW rxSpeed {"rx_speed"};
    ExFloatSpinBoxW rySpeed {"ry_speed"};
    ExFloatSpinBoxW rzSpeed {"rz_speed"};
};

CameraControllerConfigParametersW::CameraControllerConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}


void CameraControllerConfigParametersW::insert_widgets(){

    add_widget(ui::W::txt("<b>Config camera</b>"));


    add_widget(
        ui::F::gen(ui::L::VB(), {
            ui::W::txt("<b>Apply to:</b>"),
            m_p->useNeutral(),
            m_p->useEye(),
            ui::W::horizontal_line(),
            ui::W::txt("<b>Apply when:</b>"),
            m_p->cbStartRoutine(),
            m_p->cbUpdateOn(),
            ui::W::txt("<b>Values:</b>"),
            m_p->position(),
            m_p->rotation(),
            ui::W::horizontal_line(),
            m_p->enableDebugMouseCameraMovements(),
            ui::W::txt("<b>Debug speed:</b>"),
            ui::F::gen(ui::L::HB(), {ui::W::txt("M"),m_p->mSpeed(),ui::W::txt("RX"),m_p->rxSpeed(),ui::W::txt("RY"),m_p->rySpeed(),ui::W::txt("RZ"), m_p->rzSpeed()}, LStretch{true}, LMargins{false}, QFrame::NoFrame)
        },
        LStretch{false}, LMargins{true}, QFrame::Box)
    );

    add_widget(
        ui::F::gen(ui::L::VB(), {
            ui::W::txt("<b>Current calibration</b>"),
            m_p->currentNeutralPosition(),
            m_p->currentNeutralRotation(),
            ui::F::gen(ui::L::HB(), {m_p->copyNeutralToCurrentConfig = new QPushButton("Copy to values")}, LStretch{true}, LMargins{false}, QFrame::NoFrame)
        },
        LStretch{false}, LMargins{true}, QFrame::Box)
    );

    add_widget(
        ui::F::gen(ui::L::VB(), {
            ui::W::txt("<b>Current eye camera</b>"),
            m_p->currentEyePosition(),
            m_p->currentEyeRotation(),
            ui::F::gen(ui::L::HB(), {m_p->copyEyeToCurrentConfig = new QPushButton("Copy to values")}, LStretch{true}, LMargins{false}, QFrame::NoFrame)
        },
        LStretch{false}, LMargins{true}, QFrame::Box)
    );
}

void CameraControllerConfigParametersW::init_and_register_widgets(){

    DsbSettings offsetPos{MinV<qreal>{-1000.},V<qreal>{0.},MaxV<qreal>{1000.},StepV<qreal>{0.01}, 3};
    Vector3dSettings offsetVecPos = {offsetPos,offsetPos,offsetPos};
    DsbSettings offsetRot{MinV<qreal>{-360.},V<qreal>{0.},MaxV<qreal>{360.},StepV<qreal>{0.1}, 3};
    Vector3dSettings offsetVecRot = {offsetRot,offsetRot,offsetRot};

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
            {&m_p->useNeutral, &m_p->useEye},
            {
                "calibration",
                "eye camera",
            },
            {true, false}
        )
    );

    add_input_ui(m_p->position.init_widget("position: ", offsetVecPos));
    add_input_ui(m_p->rotation.init_widget("rotation: ", offsetVecRot));

    add_input_ui(m_p->cbStartRoutine.init_widget("Start routine", false, true));
    add_input_ui(m_p->cbUpdateOn.init_widget("Start timeline update block", false, true));

    m_p->position.init_widget("position: ", offsetVecPos);
    m_p->rotation.init_widget("rotation: ", offsetVecRot);
    m_p->currentNeutralPosition.init_widget("position: ", offsetVecPos, false);
    m_p->currentNeutralRotation.init_widget("rotation: ", offsetVecRot, false);
    m_p->currentEyePosition.init_widget("position: ", offsetVecPos, false);
    m_p->currentEyeRotation.init_widget("rotation: ", offsetVecRot, false);

    add_input_ui(m_p->enableDebugMouseCameraMovements.init_widget("Enable debug camera movements (move with mouse middle click, rotate with right click)", false));
    add_input_ui(m_p->mSpeed.init_widget({MinR{0.0}, ValR{1.0}, MaxR{50.0},StepV<qreal>{0.1}, 2}));
    add_input_ui(m_p->rxSpeed.init_widget({MinR{0.0}, ValR{2.0}, MaxR{10.0},StepV<qreal>{0.1}, 2}));
    add_input_ui(m_p->rySpeed.init_widget({MinR{0.0}, ValR{2.0}, MaxR{10.0},StepV<qreal>{0.1}, 2}));
    add_input_ui(m_p->rzSpeed.init_widget({MinR{0.0}, ValR{2.0}, MaxR{10.0},StepV<qreal>{0.1}, 2}));
}

void CameraControllerConfigParametersW::update_with_info(QStringView id, QStringView value){

    if(id == QSL("cameras_info")){

        auto split = value.toString().replace(",", " ").split(" ");
        if(split.size() < 12){
            return;
        }

        m_p->currentNeutralPosition.set_values({
            str::Convertor::to_double(split[0]),
            str::Convertor::to_double(split[1]),
            str::Convertor::to_double(split[2])
        });

        m_p->currentNeutralRotation.set_values({
            str::Convertor::to_double(split[3]),
            str::Convertor::to_double(split[4]),
            str::Convertor::to_double(split[5])
        });

        m_p->currentEyePosition.set_values({
            str::Convertor::to_double(split[6]),
            str::Convertor::to_double(split[7]),
            str::Convertor::to_double(split[8])
        });

        m_p->currentEyeRotation.set_values({
            str::Convertor::to_double(split[9]),
            str::Convertor::to_double(split[10]),
            str::Convertor::to_double(split[11])
        });
    }
}

void CameraControllerConfigParametersW::create_connections(){

    connect(m_p->copyNeutralToCurrentConfig, &QPushButton::clicked, this, [&]{
        m_p->position.set_values(m_p->currentNeutralPosition.values());
        m_p->rotation.set_values(m_p->currentNeutralRotation.values());
        m_p->position.trigger_ui_change();
        m_p->rotation.trigger_ui_change();
    });

    connect(m_p->copyEyeToCurrentConfig, &QPushButton::clicked, this, [&]{
        m_p->position.set_values(m_p->currentEyePosition.values());
        m_p->rotation.set_values(m_p->currentEyeRotation.values());
        m_p->position.trigger_ui_change();
        m_p->rotation.trigger_ui_change();
    });
}

void CameraControllerConfigParametersW::late_update_ui(){


}
