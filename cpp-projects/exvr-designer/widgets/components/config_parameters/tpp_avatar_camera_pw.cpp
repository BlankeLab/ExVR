
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

#include "tpp_avatar_camera_pw.hpp"

// qt-utility
#include "ex_widgets/ex_checkbox_w.hpp"
#include "ex_widgets/ex_vector3d_w.hpp"
#include "ex_widgets/ex_radio_button_w.hpp"

// local
#include "ex_widgets/ex_component_w.hpp"

using namespace tool::ex;

struct TPPAvatarCameraInitConfigParametersW::Impl{
    ExComponentW avatar{"avatar"};
};

TPPAvatarCameraInitConfigParametersW::TPPAvatarCameraInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){}

void tool::ex::TPPAvatarCameraInitConfigParametersW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::HB(), {m_p->avatar()}, LStretch{false}, LMargins{true}, QFrame::NoFrame));
}

void tool::ex::TPPAvatarCameraInitConfigParametersW::init_and_register_widgets(){
    add_input_ui(m_p->avatar.init_widget(Component::T::Humanoid_avatar, "Avatar"));
}


struct TPPAvatarCameraConfigParametersW::Impl{

    ExCheckBoxW moveNeutralCameraToHead{"move_neutral_camera_to_head"};
    ExCheckBoxW neutralPitch{"neutral_pitch"};
    ExCheckBoxW neutralYaw{"neutral_yaw"};
    ExCheckBoxW neutralRoll{"neutral_roll"};
    ExVector3dW neutralCameraToHeadOffsetPos{"neutral_camera_to_head_offset_pos"};
    ExVector3dW neutralCameraToHeadOffsetRot{"neutral_camera_to_head_offset_rot"};

    QButtonGroup group1;
    ExRadioButtonW updateEyesCameraFromHead{"update_eyes_camera_from_head"};
    ExRadioButtonW doNotUpdate{"no_not_update"};
    ExRadioButtonW updateHeadFromEyesCamera{"update_head_from_eyes_camera"};
};


TPPAvatarCameraConfigParametersW::TPPAvatarCameraConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void tool::ex::TPPAvatarCameraConfigParametersW::insert_widgets(){

    add_widget(ui::F::gen(ui::L::VB(),{
        ui::F::gen(ui::L::HB(), {m_p->moveNeutralCameraToHead()}, LStretch{true}, LMargins{false}, QFrame::NoFrame),
        ui::F::gen(ui::L::HB(), {ui::W::txt("Using axes:")}, LStretch{true}, LMargins{false}, QFrame::NoFrame),
        ui::F::gen(ui::L::HB(), {m_p->neutralPitch(), m_p->neutralYaw(), m_p->neutralRoll()}, LStretch{true}, LMargins{false}, QFrame::NoFrame),
        ui::F::gen(ui::L::HB(), {ui::W::txt("With offset from neutral camera:")}, LStretch{true}, LMargins{false}, QFrame::NoFrame),
        ui::F::gen(ui::L::HB(), {m_p->neutralCameraToHeadOffsetPos()}, LStretch{true}, LMargins{false}, QFrame::NoFrame),
        ui::F::gen(ui::L::HB(), {m_p->neutralCameraToHeadOffsetRot()}, LStretch{true}, LMargins{false}, QFrame::NoFrame)
    }, LStretch{false}, LMargins{true}, QFrame::Box));


    add_widget(ui::W::horizontal_line());

    add_widget(ui::F::gen(ui::L::VB(),{
        ui::F::gen(ui::L::HB(), {ui::W::txt("Action to do:")}, LStretch{true}, LMargins{false}, QFrame::NoFrame),
        ui::F::gen(ui::L::VB(), {m_p->updateEyesCameraFromHead(), m_p->updateHeadFromEyesCamera(), m_p->doNotUpdate()}, LStretch{false}, LMargins{true}, QFrame::NoFrame),
    }, LStretch{false}, LMargins{true}, QFrame::Box));
}

void tool::ex::TPPAvatarCameraConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_p->moveNeutralCameraToHead.init_widget("Move neutral camera to head (when routine starts)", true));
    add_input_ui(m_p->neutralPitch.init_widget("Pitch", false));
    add_input_ui(m_p->neutralYaw.init_widget("Yaw", true));
    add_input_ui(m_p->neutralRoll.init_widget("Roll", false));

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
            {&m_p->updateEyesCameraFromHead, &m_p->updateHeadFromEyesCamera, &m_p->doNotUpdate},
            {
                "Update eyes camera from avatar head (every frame)",
                "Update avatar head from eyes camera (every frame)",
                "Do nothing"
            },
            {false, false, true}
        )
    );

    DsbSettings offsetS{MinV<qreal>{-5.},V<qreal>{0.1},MaxV<qreal>{5.},StepV<qreal>{0.01}, 3};
    DsbSettings offsetRot{MinV<qreal>{-180.},V<qreal>{0.},MaxV<qreal>{180.},StepV<qreal>{0.1}, 2};
    Vector3dSettings offsetVecPos = {offsetS,offsetS,offsetS};
    Vector3dSettings offsetVecRot = {offsetRot,offsetRot,offsetRot};

    add_input_ui(m_p->neutralCameraToHeadOffsetPos.init_widget("Position:", offsetVecPos));
    m_p->neutralCameraToHeadOffsetPos.x.w->setValue(0);
    m_p->neutralCameraToHeadOffsetPos.z.w->setValue(-2.);
    add_input_ui(m_p->neutralCameraToHeadOffsetRot.init_widget("Rotation:", offsetVecRot));
}

