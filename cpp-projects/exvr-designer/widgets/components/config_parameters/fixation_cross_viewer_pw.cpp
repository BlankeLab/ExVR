
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
#include "ex_widgets/ex_vector2d_w.hpp"
#include "ex_widgets/ex_checkbox_w.hpp"
#include "ex_widgets/ex_select_color_w.hpp"

using namespace tool::ex;

void FixationCrossViewerInitConfigParametersW::insert_widgets(){}

void FixationCrossViewerInitConfigParametersW::init_and_register_widgets(){}

void FixationCrossViewerInitConfigParametersW::create_connections(){}

void FixationCrossViewerInitConfigParametersW::late_update_ui(){}


struct FixationCrossViewerConfigParametersW::Impl{

    EyeRendererSubPart eye;
    ExFloatSpinBoxW crossSizeFactor{"cross-size-factor"};
    ExSelectColorW backgroundColor{"background-color"};
    ExSelectColorW crossColor{"cross-color"};
    WordSpaceCanvasSubPart cameraSettings;
    ExCheckBoxW followEyeCamera{"use_eye_camera"};
    ExVector2dW pivot{"pivot"};
    ExFloatSpinBoxW distance{"distance"};
};

FixationCrossViewerConfigParametersW::FixationCrossViewerConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void FixationCrossViewerConfigParametersW::insert_widgets(){

    layout()->setContentsMargins(0,0,0,0);
    add_sub_part_widget(m_p->eye);

    auto followCameraF  = ui::F::gen(ui::L::HB(), {m_p->followEyeCamera()}, LStretch{true}, LMargins{}, QFrame::NoFrame);
    auto colorsF  = ui::F::gen(ui::L::HB(), {ui::W::txt("Background color:"), m_p->backgroundColor(), ui::W::txt("Cross color:"), m_p->crossColor()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    auto crossF  = ui::F::gen(ui::L::HB(), {ui::W::txt("Cross size factor"), m_p->crossSizeFactor()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    add_widget(ui::F::gen(ui::L::VB(), {followCameraF, colorsF, crossF}, LStretch{false}, LMargins{true}, QFrame::Box));
    add_sub_part_widget(m_p->cameraSettings);
    add_widget(ui::F::gen(ui::L::HB(), {m_p->pivot(), ui::W::txt("Distance"),  m_p->distance()}, LStretch{true}, LMargins{true}, QFrame::Box));
}

void FixationCrossViewerConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_p->followEyeCamera.init_widget("Cross always in front of the eyes camera", true));
    add_input_ui(m_p->backgroundColor.init_widget("Select background color", Qt::black));
    add_input_ui(m_p->crossColor.init_widget("Select cross color", Qt::white));

    DsbSettings factorSettings= {MinV<qreal>{0}, V<qreal>{0.3}, MaxV<qreal>{1.}, StepV<qreal>{0.01}, 3};
    add_input_ui(m_p->crossSizeFactor.init_widget(factorSettings));

    Vector2dSettings pivotSettings= {
        {MinV<qreal>{-0.5}, V<qreal>{0.5}, MaxV<qreal>{1.5}, StepV<qreal>{0.01}, 2},
        {MinV<qreal>{-0.5}, V<qreal>{0.5}, MaxV<qreal>{1.5}, StepV<qreal>{0.01}, 2}
    };
    add_input_ui(m_p->pivot.init_widget("Pivot", pivotSettings));
    DsbSettings distanceSettings= {MinV<qreal>{0}, V<qreal>{1.}, MaxV<qreal>{1000.}, StepV<qreal>{0.1}, 2};
    add_input_ui(m_p->distance.init_widget(distanceSettings));

    map_sub_part(m_p->eye.init_widget());
    map_sub_part(m_p->cameraSettings.init_widget());
}

void FixationCrossViewerConfigParametersW::create_connections(){

    connect(m_p->followEyeCamera(), &QCheckBox::toggled, this, [&](bool checked){
        m_p->pivot.w->setEnabled(checked);
        m_p->distance.w->setEnabled(checked);
        m_p->cameraSettings.set_position_enable_state(!checked,!checked,!checked);
    });
}

void FixationCrossViewerConfigParametersW::late_update_ui(){

    m_p->pivot.w->setEnabled(m_p->followEyeCamera()->isChecked());
    m_p->distance.w->setEnabled(m_p->followEyeCamera()->isChecked());
    bool state2 = !m_p->followEyeCamera()->isChecked();
    m_p->cameraSettings.set_position_enable_state(state2,state2,state2);

}
