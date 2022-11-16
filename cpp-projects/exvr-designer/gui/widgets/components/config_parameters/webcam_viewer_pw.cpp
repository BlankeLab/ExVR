
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


struct WebcamViewerInitConfigParametersW::Impl{
    ExSpinBoxW deviceId{"device_id"};
    ExSpinBoxW fps{"requested_fps"};
};


WebcamViewerInitConfigParametersW::WebcamViewerInitConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void WebcamViewerInitConfigParametersW::insert_widgets(){

    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Device id: "), m_p->deviceId()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Requested FPS: "), m_p->fps()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
}

void WebcamViewerInitConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_p->deviceId.init_widget(MinV<int>{0}, V<int>{0}, MaxV<int>{32}, StepV<int>{1},  true));
    add_input_ui(m_p->fps.init_widget(MinV<int>{1}, V<int>{30}, MaxV<int>{120}, StepV<int>{1},  true));
}

struct WebcamViewerConfigParametersW::Impl{
    WordSpaceCanvasSubPart wscsp;
};

WebcamViewerConfigParametersW::WebcamViewerConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void WebcamViewerConfigParametersW::insert_widgets(){
    layout()->setContentsMargins(0,0,0,0);
    add_sub_part_widget(m_p->wscsp);
}

void WebcamViewerConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->wscsp.init_widget());
}


void WebcamViewerConfigParametersW::create_connections(){
    m_p->wscsp.create_connections();
}

void WebcamViewerConfigParametersW::late_update_ui(){
    m_p->wscsp.late_update_ui();
}

