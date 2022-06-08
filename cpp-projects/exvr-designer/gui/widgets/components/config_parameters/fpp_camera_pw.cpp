
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

#include "fpp_camera_pw.hpp"

// qt-utility
//#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_vector3d_w.hpp"
#include "gui/ex_widgets/ex_radio_button_w.hpp"

using namespace tool::ex;

struct FPPCameraInitConfigParametersW::Impl{
    QButtonGroup group1;
    TransformSubPart transfo{"init_transform"};
    ExRadioButtonW useEye {"use_eye"};
    ExRadioButtonW useNeutral {"use_neutral"};
};

FPPCameraInitConfigParametersW::FPPCameraInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){}

void tool::ex::FPPCameraInitConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);

    add_widget(
        ui::F::gen(ui::L::VB(), {
            ui::W::txt("<b>Apply to:</b>"),
            m_p->useNeutral(),
            m_p->useEye()
        },
        LStretch{false}, LMargins{true}, QFrame::Box)
    );
}

void tool::ex::FPPCameraInitConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
            {&m_p->useNeutral, &m_p->useEye},
            {
                "start neutral camera",
                "eye camera",
            },
            {true, false}
        )
    );
}

void tool::ex::FPPCameraInitConfigParametersW::create_connections(){}
void tool::ex::FPPCameraInitConfigParametersW::late_update_ui(){}

struct FPPCameraConfigParametersW::Impl{
    QButtonGroup group1;
    TransformSubPart transfo{"transform"};
    ExRadioButtonW useEye {"use_eye"};
    ExRadioButtonW useNeutral {"use_neutral"};
    ExVector3dW currentNeutralPosition {"neutral_position"};
    ExVector3dW currentNeutralRotation {"neutral_rotation"};
    ExVector3dW currentEyePosition {"eye_rotation"};
    ExVector3dW currentEyeRotation {"eye_rotation"};
};


FPPCameraConfigParametersW::FPPCameraConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void FPPCameraConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);

    add_widget(
        ui::F::gen(ui::L::VB(), {
            ui::W::txt("<b>Apply to:</b>"),
            m_p->useNeutral(),
            m_p->useEye()
        },
        LStretch{false}, LMargins{true}, QFrame::Box)
    );

    add_widget(
        ui::F::gen(ui::L::VB(), {
            ui::W::txt("<b>Current start neutral camera</b>"),
            m_p->currentNeutralPosition(),
            m_p->currentNeutralRotation()
        },
        LStretch{false}, LMargins{true}, QFrame::Box)
    );

    add_widget(
        ui::F::gen(ui::L::VB(), {
            ui::W::txt("<b>Current eye camera</b>"),
            m_p->currentEyePosition(),
            m_p->currentEyeRotation()
        },
        LStretch{false}, LMargins{true}, QFrame::Box)
    );
}

void FPPCameraConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Config transform</b> (applied when routine starts)<b>")));

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
            {&m_p->useNeutral, &m_p->useEye},
            {
                "start neutral camera",
                "eye camera",
            },
            {true, false}
        )
    );

    DsbSettings offsetPos{MinV<qreal>{-1000.},V<qreal>{0.},MaxV<qreal>{1000.},StepV<qreal>{0.01}, 3};
    Vector3dSettings offsetVecPos = {offsetPos,offsetPos,offsetPos};
    DsbSettings offsetRot{MinV<qreal>{-360.},V<qreal>{0.},MaxV<qreal>{360.},StepV<qreal>{0.1}, 3};
    Vector3dSettings offsetVecRot = {offsetRot,offsetRot,offsetRot};

    m_p->currentNeutralPosition.init_widget("position: ", offsetVecPos, false);
    m_p->currentNeutralRotation.init_widget("rotation: ", offsetVecRot, false);
    m_p->currentEyePosition.init_widget("position: ", offsetVecPos, false);
    m_p->currentEyeRotation.init_widget("rotation: ", offsetVecRot, false);

}

void FPPCameraConfigParametersW::create_connections(){}
void FPPCameraConfigParametersW::late_update_ui(){}

void FPPCameraConfigParametersW::update_with_info(QStringView id, QStringView value){

    if(id == QSL("cameras_info")){

        auto split = value.toString().remove(",").split(" ");
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
