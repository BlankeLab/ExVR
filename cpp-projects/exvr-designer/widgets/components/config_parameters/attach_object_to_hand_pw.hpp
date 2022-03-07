
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

// local
#include "config_pw.hpp"


namespace tool::ex {

class AttachObjectToHandInitConfigParametersW : public ConfigParametersW{

public :

//    ExLineEditW m_leLeapMotionTrackingComponent;

    void insert_widgets() override{
//        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Leap motion tracking component: "),m_leLeapMotionTrackingComponent()}, LStretch{true}, LMargins{true},QFrame::Box));
    }

    void init_and_register_widgets() override{
//        m_inputUiElements["leap_motion_component"]  = m_leLeapMotionTrackingComponent.init_widget("");
    }

    void create_connections() override{}
    void late_update_ui() override{}
};


class AttachObjectToHandConfigParametersW : public ConfigParametersW{

public :

//    DefaultTransformSubPart m_transfo;
//    ExComboBoxTextW m_armToAttach;
//    ExComboBoxTextW m_fingerToAttach;


    void insert_widgets() override{
//        add_sub_part_to_layout(m_transfo);
//        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Arm to attach: "),      m_armToAttach()}, LStretch{true}, LMargins{true},QFrame::Box));
//        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Finger to attach: "),   m_fingerToAttach()}, LStretch{true}, LMargins{true},QFrame::Box));
    }

    void init_and_register_widgets() override{

//        add_sub_part_to_map(m_transfo.init_widget("sponge_transform"));
//        m_transfo.tr.set_rotation_values({180.,0.,90.});
//        m_inputUiElements["arm"]    = m_armToAttach.init_widget({"Left","Right"});
//        m_inputUiElements["finger"] = m_fingerToAttach.init_widget({"Thumb","Index", "Pinky", "Middle", "Ring"});
    }

    void create_connections() override{}
    void late_update_ui() override{}
};
}
