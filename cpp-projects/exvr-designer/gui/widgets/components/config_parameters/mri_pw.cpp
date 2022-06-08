
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

#include "mri_pw.hpp"

using namespace tool::ex;

struct MriInitConfigParametersW::Impl{
    ExComponentW humanoidController{"humanoid_controller"};
    ExComponentW keyboard{"keyboard"};
    ExComponentsListW loggersList{"loggers"};
    ExComboBoxTextW triggerKey{"trigger_key"};
};


MriInitConfigParametersW::MriInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}



void MriInitConfigParametersW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("Avatar to use in the MRI:"), m_p->humanoidController()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("Keyboard used for catching MRI triggers:"), m_p->keyboard()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Trigger key:"), m_p->triggerKey()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("Write trigger line to theses loggers:"),m_p->loggersList()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    no_end_stretch();
}

void MriInitConfigParametersW::init_and_register_widgets(){

    QStringList items;
    for(const auto &keyName : input::Keyboard::buttons.tuple_column<1>()){
        items << from_view(keyName);
    }

    add_input_ui(m_p->humanoidController.init_widget(Component::Type::Humanoid_controller, "Humanoid controller:"));
    add_input_ui(m_p->keyboard.init_widget(Component::Type::Keyboard, "Keyboard"));
    add_input_ui(m_p->triggerKey.init_widget(items));
    add_input_ui(m_p->loggersList.init_widget(Component::Type::Logger, "Loggers"));
}

void MriInitConfigParametersW::create_connections(){
}

void MriInitConfigParametersW::late_update_ui(){}


struct MriConfigParametersW::Impl{

    TransformSubPart transform{"transform"};

    ExCheckBoxW goNextWhenTrigger{"trigger_go_next"};

    QButtonGroup group1;
    ExRadioButtonW solve{"solve"};
    ExRadioButtonW dissolve{"dissolve"};
    ExRadioButtonW move_table_inside{"move_table_inside"};
    ExRadioButtonW move_table_outside{"move_table_outside"};
    ExRadioButtonW move_table_qualisys{"move_table_qualisys"};
    ExRadioButtonW nothing{"nothing"};

    ExDoubleSpinBoxW duration{"duration"};
    ExDoubleSpinBoxW distance{"distance"};
    ExCheckBoxW room{"process_room"};
    ExCheckBoxW magnet{"process_magnet"};

    ExCheckBoxW initQualisysPositions{"init_qualisys"};
    ExCheckBoxW enableQualisysArmsTracking{"qualisys_arms_tracking"};
    ExVector3dW targetHeadTrOffset{"target_head_tr_offset"};
    ExVector3dW targetHeadRotOffset{"target_head_rot_offset"};

};


MriConfigParametersW::MriConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}




void MriConfigParametersW::insert_widgets(){

    add_sub_part_widget(m_p->transform);
    add_widget(ui::F::gen(ui::L::HB(), {m_p->goNextWhenTrigger()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("Action:"), m_p->solve(), m_p->dissolve(), m_p->move_table_inside(), m_p->move_table_outside(), m_p->move_table_qualisys(), m_p->nothing()}, LStretch{false}, LMargins{true}, QFrame::NoFrame));
    add_widget(ui::W::horizontal_line());
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Duration:"), m_p->duration()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Distance:"), m_p->distance()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Objects to be solved/dissolved:"), m_p->room(), m_p->magnet()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::W::horizontal_line());
    add_widget(ui::F::gen(ui::L::HB(), {m_p->initQualisysPositions()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {m_p->enableQualisysArmsTracking()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::W::horizontal_line());
    add_widget(ui::F::gen(ui::L::HB(), {m_p->targetHeadTrOffset()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {m_p->targetHeadRotOffset()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
}

void MriConfigParametersW::init_and_register_widgets(){

    map_sub_part(m_p->transform.init_widget());

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
            {&m_p->solve, &m_p->dissolve, &m_p->move_table_inside, &m_p->move_table_outside, &m_p->move_table_qualisys, &m_p->nothing},
            {
                "solve",
                "dissolve",
                "move table inside",
                "move table outside",
                "move table using qualisys",
                "nothing"
            },
            {false, false, false, false, false, true}
        )
    );

    add_input_ui(m_p->duration.init_widget(MinV<qreal>{0.01},V<qreal>{5.},MaxV<qreal>{100.},StepV<qreal>{0.01}, 2));
    add_input_ui(m_p->distance.init_widget(MinV<qreal>{0.00},V<qreal>{1.0},MaxV<qreal>{2.5},StepV<qreal>{0.01}, 2));
    add_input_ui(m_p->room.init_widget("room", true));
    add_input_ui(m_p->magnet.init_widget("magnet", true));
    add_input_ui(m_p->initQualisysPositions.init_widget("Init Qualisys positions", false));
    add_input_ui(m_p->enableQualisysArmsTracking.init_widget("Enable Qualisys arms tracking", false));
    add_input_ui(m_p->goNextWhenTrigger.init_widget("Go next when receive trigger", false));

    DsbSettings offsetTr{MinV<qreal>{-2.},V<qreal>{0.},MaxV<qreal>{2.},StepV<qreal>{0.01}, 2};
    auto offsetVecTr = Vector3dSettings{offsetTr,offsetTr,offsetTr};
    add_input_ui(m_p->targetHeadTrOffset.init_widget("Target head translation offset", offsetVecTr));

    DsbSettings offsetRot{MinV<qreal>{-180.},V<qreal>{0.},MaxV<qreal>{180.},StepV<qreal>{0.1}, 2};
    Vector3dSettings offsetVecRot = {offsetRot,offsetRot,offsetRot};
    add_input_ui(m_p->targetHeadRotOffset.init_widget("Target head rotation offset", offsetVecRot));
}

void MriConfigParametersW::create_connections(){}

void MriConfigParametersW::late_update_ui(){}
