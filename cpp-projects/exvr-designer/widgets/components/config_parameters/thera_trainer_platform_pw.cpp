
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

#include "thera_trainer_platform_pw.hpp"

using namespace tool::ex;


void TheraTrainerPlatformConfigParametersW::insert_widgets() {
    add_sub_part_widget(m_transfo);
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Pelvis height (m): "), m_height()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Offsets: ")}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {m_displayGrips()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {m_offsetPelvisHandGripTr()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {m_offsetPelvisHandGripRot()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {m_offsetRightHandGripTr()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {m_offsetRightHandGripRot()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {m_offsetLeftHandGripTr()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {m_offsetLeftHandGripRot()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
}

void TheraTrainerPlatformConfigParametersW::init_and_register_widgets(){

    map_sub_part(m_transfo.init_widget());

    DsbSettings offsetP{MinV<qreal>{-2.},V<qreal>{0.},MaxV<qreal>{2.},StepV<qreal>{0.01}, 2};
    DsbSettings offsetR{MinV<qreal>{-360.},V<qreal>{0.},MaxV<qreal>{360.},StepV<qreal>{1}, 2};

    add_input_ui(m_height.init_widget(MinV<qreal>{0.5},V<qreal>{1.},MaxV<qreal>{1.5},StepV<qreal>{0.01}, 2));
    add_input_ui(m_displayGrips.init_widget("Display humanoid targets: ", false));
    add_input_ui(m_offsetPelvisHandGripTr.init_widget("Pelvis pos:", Vector3dSettings{offsetP,offsetP,offsetP}));
    add_input_ui(m_offsetPelvisHandGripRot.init_widget("Pelvis rot:", Vector3dSettings{offsetR,offsetR,offsetR}));
    add_input_ui(m_offsetRightHandGripTr.init_widget("Right hand grip pos:", Vector3dSettings{offsetP,offsetP,offsetP}));
    add_input_ui(m_offsetRightHandGripRot.init_widget("Right hand grip rot:", Vector3dSettings{offsetR,offsetR,offsetR}));
    add_input_ui(m_offsetLeftHandGripTr.init_widget("Left hand grip pos:", Vector3dSettings{offsetP,offsetP,offsetP}));
    add_input_ui(m_offsetLeftHandGripRot.init_widget("Left hand grip rot:", Vector3dSettings{offsetR,offsetR,offsetR}));
}
