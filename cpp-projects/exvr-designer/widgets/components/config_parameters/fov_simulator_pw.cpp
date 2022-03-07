
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


#include "fov_simulator_pw.hpp"

using namespace tool::ex;


void FovSimulatorInitConfigParametersW::insert_widgets(){

    auto distanceF  = ui::F::gen(ui::L::HB(), {ui::W::txt("Distance"),distance()}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    auto horiFovF   = ui::F::gen(ui::L::HB(), {ui::W::txt("Horizontal FOV(°)"), horizontalFov()}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    auto vertiFovF  = ui::F::gen(ui::L::HB(), {ui::W::txt("Vertical FOV(°)"), verticalFov()}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    add_widget(ui::F::gen(ui::L::VB(), {distanceF, horiFovF, vertiFovF}, LStretch{false}, LMargins{true}, QFrame::Box));
}

void FovSimulatorInitConfigParametersW::init_and_register_widgets(){

    DsbSettings distanceSettings= {MinV<qreal>{0}, V<qreal>{0.15}, MaxV<qreal>{15.}, StepV<qreal>{0.01}, 2};
    add_input_ui(distance.init_widget(distanceSettings));

    DsbSettings horiFovSettings= {MinV<qreal>{0}, V<qreal>{52.1}, MaxV<qreal>{200.}, StepV<qreal>{0.1}, 2};
    DsbSettings vertiFovSettings= {MinV<qreal>{0}, V<qreal>{34}, MaxV<qreal>{200.}, StepV<qreal>{0.1}, 2};
    add_input_ui(horizontalFov.init_widget(horiFovSettings));
    add_input_ui(verticalFov.init_widget(vertiFovSettings));
}


void FovSimulatorConfigParametersW::insert_widgets(){
    auto colorF     = ui::F::gen(ui::L::HB(), {ui::W::txt("Screen color"), color()}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    add_widget(ui::F::gen(ui::L::VB(), {colorF}, LStretch{false}, LMargins{true}, QFrame::Box));
}

void FovSimulatorConfigParametersW::init_and_register_widgets(){
    add_input_ui(color.init_widget("Screen color", QColor(0,0,0,255)));
}

void FovSimulatorConfigParametersW::create_connections(){}
void FovSimulatorConfigParametersW::late_update_ui(){}

