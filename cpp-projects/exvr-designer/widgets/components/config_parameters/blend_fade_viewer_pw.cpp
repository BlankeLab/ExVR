
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

#include "blend_fade_viewer_pw.hpp"

using namespace tool::ex;

void BlendFadeViewerConfigParametersW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::HB(), {blendAndFade(), onlyBlend()},  LStretch{true}, LMargins{true}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Start color"), startColor(), ui::W::txt("End color"), endColor()},  LStretch{true}, LMargins{true}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Duration blend"), durationBlend()},  LStretch{true}, LMargins{true}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Duration fade"), durationFade()},  LStretch{true}, LMargins{true}, QFrame::NoFrame));
}

void BlendFadeViewerConfigParametersW::init_and_register_widgets(){

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(group1,
            {&blendAndFade, &onlyBlend},
            {
                "Blend and fade",
                "Only blend",
            },
            {true,false}
        )
    );

    DsbSettings s = {MinV<qreal>{0.0}, V<qreal>{1.0}, MaxV<qreal>{1000.0}, StepV<qreal>{0.1}, 2};
    add_input_ui(durationBlend.init_widget(s));
    add_input_ui(durationFade.init_widget(s));
    add_input_ui(startColor.init_widget("Select start color" , QColor(0,0,0,0)));
    add_input_ui(endColor.init_widget("Select end color" , QColor(0,0,0,255)));
}

void BlendFadeViewerConfigParametersW::create_connections(){
}

void BlendFadeViewerConfigParametersW::late_update_ui(){
}
