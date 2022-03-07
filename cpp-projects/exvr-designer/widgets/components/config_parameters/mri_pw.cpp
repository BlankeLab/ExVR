
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

void MriInitConfigParametersW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("Avatar to use in the MRI:"), humanoidController()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("Keyboard used for catching MRI triggers:"), keyboard()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Trigger key:"), triggerKey()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("Write trigger line to theses loggers:"),loggersList()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));    
    no_end_stretch();
}

void MriInitConfigParametersW::init_and_register_widgets(){

    QStringList items;
    for(const auto &keyName : input::Keyboard::buttons.tuple_column<1>()){
        items << from_view(keyName);
    }

    add_input_ui(humanoidController.init_widget(Component::Type::Humanoid_controller, "Humanoid controller:"));
    add_input_ui(keyboard.init_widget(Component::Type::Keyboard, "Keyboard"));
    add_input_ui(triggerKey.init_widget(items));
    add_input_ui(loggersList.init_widget(Component::Type::Logger, "Loggers"));
}

void MriInitConfigParametersW::create_connections(){
}

void MriInitConfigParametersW::late_update_ui(){}

void MriConfigParametersW::insert_widgets(){

    add_sub_part_widget(transform);
    add_widget(ui::F::gen(ui::L::HB(), {goNextWhenTrigger()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("Action:"), solve(), dissolve(), move_table_inside(), move_table_outside(), move_table_qualisys(), nothing()}, LStretch{false}, LMargins{true}, QFrame::NoFrame));
    add_widget(ui::W::horizontal_line());
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Duration:"), duration()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Distance:"), distance()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Objects to be solved/dissolved:"), room(), magnet()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::W::horizontal_line());
    add_widget(ui::F::gen(ui::L::HB(), {initQualisysPositions()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {enableQualisysArmsTracking()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::W::horizontal_line());
    add_widget(ui::F::gen(ui::L::HB(), {targetHeadTrOffset()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {targetHeadRotOffset()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
}

void MriConfigParametersW::init_and_register_widgets(){

    map_sub_part(transform.init_widget());    

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(group1,
            {&solve, &dissolve, &move_table_inside, &move_table_outside, &move_table_qualisys, &nothing},
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

    add_input_ui(duration.init_widget(MinV<qreal>{0.01},V<qreal>{5.},MaxV<qreal>{25.},StepV<qreal>{0.01}, 2));
    add_input_ui(distance.init_widget(MinV<qreal>{0.00},V<qreal>{1.0},MaxV<qreal>{2.5},StepV<qreal>{0.01}, 2));
    add_input_ui(room.init_widget("room", true));
    add_input_ui(magnet.init_widget("magnet", true));
    add_input_ui(initQualisysPositions.init_widget("Init Qualisys positions", false));
    add_input_ui(enableQualisysArmsTracking.init_widget("Enable Qualisys arms tracking", false));
    add_input_ui(goNextWhenTrigger.init_widget("Go next when receive trigger", false));

    DsbSettings offsetTr{MinV<qreal>{-2.},V<qreal>{0.},MaxV<qreal>{2.},StepV<qreal>{0.01}, 2};
    auto offsetVecTr = Vector3dSettings{offsetTr,offsetTr,offsetTr};
    add_input_ui(targetHeadTrOffset.init_widget("Target head translation offset", offsetVecTr));

    DsbSettings offsetRot{MinV<qreal>{-180.},V<qreal>{0.},MaxV<qreal>{180.},StepV<qreal>{0.1}, 2};
    Vector3dSettings offsetVecRot = {offsetRot,offsetRot,offsetRot};
    add_input_ui(targetHeadRotOffset.init_widget("Target head rotation offset", offsetVecRot));
}

void MriConfigParametersW::create_connections(){}

void MriConfigParametersW::late_update_ui(){}
