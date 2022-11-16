
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


#include "fixation_cross_viewer_pw.hpp"

// qt-utility
#include "gui/ex_widgets/ex_vector2d_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_select_color_w.hpp"

using namespace tool::ex;

void FixationCrossViewerInitConfigParametersW::insert_widgets(){}

void FixationCrossViewerInitConfigParametersW::init_and_register_widgets(){}

void FixationCrossViewerInitConfigParametersW::create_connections(){}

void FixationCrossViewerInitConfigParametersW::late_update_ui(){}


struct FixationCrossViewerConfigParametersW::Impl{
    ExFloatSpinBoxW crossSizeFactor{"cross-size-factor"};
    ExSelectColorW crossColor{"cross-color"};
    WordSpaceCanvasSubPart wscsp;
};

FixationCrossViewerConfigParametersW::FixationCrossViewerConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void FixationCrossViewerConfigParametersW::insert_widgets(){

    layout()->setContentsMargins(0,0,0,0);
    auto crossF  = ui::F::gen(ui::L::HB(), {ui::W::txt("Color:"), m_p->crossColor(), ui::W::txt("Size factor"), m_p->crossSizeFactor()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("<b>Fixation cross settings</b>"), crossF}, LStretch{false}, LMargins{true}, QFrame::Box));
    add_sub_part_widget(m_p->wscsp);
}

void FixationCrossViewerConfigParametersW::init_and_register_widgets(){
    add_input_ui(m_p->crossColor.init_widget("Select cross color", Qt::white));
    DsbSettings factorSettings= {MinV<qreal>{0}, V<qreal>{0.3}, MaxV<qreal>{1.}, StepV<qreal>{0.01}, 3};
    add_input_ui(m_p->crossSizeFactor.init_widget(factorSettings));
    map_sub_part(m_p->wscsp.init_widget());
    m_p->wscsp.set_background_color(QColor(0,0,0,0));
}

void FixationCrossViewerConfigParametersW::create_connections(){
    m_p->wscsp.create_connections();
}

void FixationCrossViewerConfigParametersW::late_update_ui(){
    m_p->wscsp.late_update_ui();
}
