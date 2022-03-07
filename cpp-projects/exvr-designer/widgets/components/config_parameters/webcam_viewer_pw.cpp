
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


#include "webcam_viewer_pw.hpp"

using namespace tool::ex;

void WebcamViewerInitConfigParametersW::insert_widgets(){

    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Device id: "), m_sbDeviceId()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Requested FPS: "), m_sbFPS()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
}

void WebcamViewerInitConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_sbDeviceId.init_widget(MinV<int>{0}, V<int>{0}, MaxV<int>{32}, StepV<int>{1},  true));
    add_input_ui(m_sbFPS.init_widget(MinV<int>{1}, V<int>{30}, MaxV<int>{120}, StepV<int>{1},  true));
}



void WebcamViewerConfigParametersW::insert_widgets(){

    layout()->setContentsMargins(0,0,0,0);
    add_sub_part_widget(m_eye);

    auto useImageF      = ui::F::gen(ui::L::HB(), {m_cbUseVideoSize()}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    auto followCameraF  = ui::F::gen(ui::L::HB(), {m_followEyeCamera()}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    auto topF = ui::F::gen(ui::L::VB(), {useImageF, followCameraF}, LStretch{false}, LMargins{false}, QFrame::Box);
    add_widget(topF);
    add_sub_part_widget(m_cameraSettings);
    add_widget(ui::F::gen(ui::L::HB(), {m_pivot(), ui::W::txt("Distance"),  m_distance()}, LStretch{true}, LMargins{true}, QFrame::Box));
}

void WebcamViewerConfigParametersW::init_and_register_widgets(){

    map_sub_part(m_eye.init_widget());
    add_input_ui(m_followEyeCamera.init_widget("Video always in front of the eyes camera", false));
    add_input_ui(m_cbUseVideoSize.init_widget("Use video original size", true));

    Vector2dSettings pivotSettings= {
        {MinV<qreal>{-0.5}, V<qreal>{0.5}, MaxV<qreal>{1.5}, StepV<qreal>{0.01}, 2},
        {MinV<qreal>{-0.5}, V<qreal>{0.5}, MaxV<qreal>{1.5}, StepV<qreal>{0.01}, 2}
    };
    add_input_ui(m_pivot.init_widget("Pivot", pivotSettings));
    DsbSettings distanceSettings= {MinV<qreal>{0}, V<qreal>{10.}, MaxV<qreal>{1000.}, StepV<qreal>{0.1}, 2};
    add_input_ui(m_distance.init_widget(distanceSettings));

    map_sub_part(m_cameraSettings.init_widget());
}

void WebcamViewerConfigParametersW::create_connections(){
    connect(m_cbUseVideoSize(), &QCheckBox::toggled, this, [&](bool checked){
        m_cameraSettings.set_wh_enable_state(!checked,!checked);
    });

    connect(m_followEyeCamera(), &QCheckBox::toggled, this, [&](bool checked){
        m_pivot.w->setEnabled(checked);
        m_distance.w->setEnabled(checked);
m_cameraSettings.set_position_enable_state(!checked,!checked,!checked);
    });
}

void WebcamViewerConfigParametersW::late_update_ui(){

    bool state1 = !m_cbUseVideoSize()->isChecked();
    m_cameraSettings.set_wh_enable_state(state1,state1);

    m_pivot.w->setEnabled(m_followEyeCamera()->isChecked());
    bool state2 = !m_followEyeCamera()->isChecked();
    m_cameraSettings.set_position_enable_state(state2,state2,state2);
}
