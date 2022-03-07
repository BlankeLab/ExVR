
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

#include "text_viewer_pw.hpp"

// qt-utility
#include "ex_widgets/ex_select_color_w.hpp"
#include "ex_widgets/ex_text_edit_w.hpp"
#include "ex_widgets/ex_checkbox_w.hpp"
#include "ex_widgets/ex_radio_button_w.hpp"
#include "ex_widgets/ex_vector2d_w.hpp"

// local
#include "ex_widgets/ex_resource_w.hpp"

using namespace tool::ex;

struct TextViewerConfigParametersW::Impl{

    EyeRendererSubPart eye;
    QTabWidget tw;
    QTabWidget textTw;
    WordSpaceCanvasSubPart cameraSettings;
    ExCheckBoxW followEyeCamera{"use_eye_camera"};
    ExVector2dW pivot{"pivot"};
    ExFloatSpinBoxW distance{"distance"};
    ExSelectColorW backgroundColor{"background_color"};
    TextSubPart text {"t"};
};


TextViewerConfigParametersW::TextViewerConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}


void TextViewerConfigParametersW::insert_widgets(){

    layout()->setContentsMargins(0,0,0,0);
    add_sub_part_widget(m_p->eye);
    add_widget(&m_p->tw);

    QWidget *containerW = new QWidget();
    QVBoxLayout *containerL = new QVBoxLayout();
    containerW->setLayout(containerL);
    containerL->setContentsMargins(2,2,2,2);

    m_p->tw.addTab(containerW, "Container");
    containerL->addWidget(ui::F::gen(ui::L::HB(), {m_p->followEyeCamera()}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
    containerL->addWidget(m_p->cameraSettings.frame);
    containerL->addWidget(ui::F::gen(ui::L::HB(), {m_p->pivot(), ui::W::txt("Distance"),  m_p->distance()}, LStretch{true}, LMargins{true}, QFrame::Box));
    containerL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Background color"),  m_p->backgroundColor()}, LStretch{true}, LMargins{true}, QFrame::Box));
    containerL->addStretch();

    m_p->tw.addTab(&m_p->textTw, "Content");
    m_p->textTw.addTab(m_p->text.textW, "Text");
    m_p->textTw.addTab(m_p->text.settingsW, "Settings");
}

void TextViewerConfigParametersW::init_and_register_widgets(){

    Vector2dSettings pivotSettings= {
        {MinV<qreal>{-5}, V<qreal>{0.5}, MaxV<qreal>{5}, StepV<qreal>{0.01}, 2},
        {MinV<qreal>{-5}, V<qreal>{0.5}, MaxV<qreal>{5}, StepV<qreal>{0.01}, 2}
    };
    DsbSettings distanceSettings= {MinV<qreal>{0}, V<qreal>{1.}, MaxV<qreal>{1000.}, StepV<qreal>{0.1}, 2};
    add_input_ui(m_p->pivot.init_widget("Pivot", pivotSettings));
    add_input_ui(m_p->distance.init_widget(distanceSettings));
    add_input_ui(m_p->followEyeCamera.init_widget("Text always in front of the eyes camera", true));
    add_input_ui(m_p->backgroundColor.init_widget("Select background color", QColor(255,255,255,255)));

    map_sub_part(m_p->eye.init_widget());
    map_sub_part(m_p->cameraSettings.init_widget());
    map_sub_part(m_p->text.init_widget());
}

void TextViewerConfigParametersW::create_connections(){

    connect(m_p->followEyeCamera(), &QCheckBox::toggled, this, [&](bool checked){
        m_p->pivot.w->setEnabled(checked);
        m_p->distance.w->setEnabled(checked);
        m_p->cameraSettings.set_position_enable_state(!checked,!checked,!checked);
    });
}

void TextViewerConfigParametersW::late_update_ui(){
    m_p->pivot.w->setEnabled(m_p->followEyeCamera()->isChecked());
    m_p->distance.w->setEnabled(m_p->followEyeCamera()->isChecked());

    bool state = !m_p->followEyeCamera()->isChecked();
    m_p->cameraSettings.set_position_enable_state(state,state,state);
}
