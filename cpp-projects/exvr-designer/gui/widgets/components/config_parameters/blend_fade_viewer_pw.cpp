
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

// qt-utility
#include "gui/ex_widgets/ex_select_color_w.hpp"
#include "gui/ex_widgets/ex_float_spin_box_w.hpp"
#include "gui/ex_widgets/ex_radio_button_w.hpp"

using namespace tool::ex;

struct BlendFadeViewerConfigParametersW::Impl{
    QButtonGroup group1;
    ExRadioButtonW onlyBlend{"only_blend"};
    ExRadioButtonW blendAndFade{"blend_and_fade"};

    ExSelectColorW startColor{"start_color"};
    ExSelectColorW endColor{"end_color"};

    ExFloatSpinBoxW durationBlend{"duration_blend"};
    ExFloatSpinBoxW durationFade{"duration_fade"};
};

BlendFadeViewerConfigParametersW::BlendFadeViewerConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}


void BlendFadeViewerConfigParametersW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::HB(), {m_p->blendAndFade(), m_p->onlyBlend()},  LStretch{true}, LMargins{true}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Start color"), m_p->startColor(), ui::W::txt("End color"), m_p->endColor()},  LStretch{true}, LMargins{true}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Duration blend"), m_p->durationBlend()},  LStretch{true}, LMargins{true}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Duration fade"), m_p->durationFade()},  LStretch{true}, LMargins{true}, QFrame::NoFrame));
}

void BlendFadeViewerConfigParametersW::init_and_register_widgets(){

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
            {&m_p->blendAndFade, &m_p->onlyBlend},
            {
                "Blend and fade",
                "Only blend",
            },
            {true,false}
        )
    );

    DsbSettings s = {MinV<qreal>{0.0}, V<qreal>{1.0}, MaxV<qreal>{1000.0}, StepV<qreal>{0.1}, 2};
    add_input_ui(m_p->durationBlend.init_widget(s));
    add_input_ui(m_p->durationFade.init_widget(s));
    add_input_ui(m_p->startColor.init_widget("Select start color" , QColor(0,0,0,0)));
    add_input_ui(m_p->endColor.init_widget("Select end color" , QColor(0,0,0,255)));
}

