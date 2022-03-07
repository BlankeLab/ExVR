
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

#include "slider_ui_pw.hpp"

using namespace tool::ex;

void SliderUiInitConfigParametersW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::HB(), {m_cbHorizontalSlider()}));
}

void SliderUiInitConfigParametersW::init_and_register_widgets(){
    add_input_ui(m_cbHorizontalSlider.init_widget("Horizontal slider", true));
}

void SliderUiInitConfigParametersW::create_connections(){}

void SliderUiInitConfigParametersW::late_update_ui(){}

void SliderUiConfigParametersW::update_ui_value_type(bool whole){
    m_minValue()->setDecimals(whole ? 0 : 2);
    m_initialValue()->setDecimals(whole ? 0 : 2);
    m_maxValue()->setDecimals(whole ? 0 : 2);
    m_stepValue()->setDecimals(whole ? 0 : 2);

    m_stepValue()->setSingleStep(whole ? 1 : 0.01);
    m_minValue()->setSingleStep(whole ? 1 : 0.01);
    m_initialValue()->setSingleStep(whole ? 1 : 0.01);
    m_maxValue()->setSingleStep(whole ? 1 : 0.01);
}

void SliderUiConfigParametersW::update_ui_value_limits(qreal value){
    Q_UNUSED(value)
    m_initialValue()->setMinimum(m_minValue()->value());
    m_maxValue()->setMinimum(m_minValue()->value());
    m_initialValue()->setMaximum(m_maxValue()->value());
    m_minValue()->setMaximum(m_maxValue()->value());
}

void SliderUiConfigParametersW::insert_widgets(){

    layout()->setContentsMargins(0,0,0,0);
    add_widget(m_tw1 = new QTabWidget());

    QWidget *containerW = new QWidget();
    QVBoxLayout *containerL = new QVBoxLayout();
    containerW->setLayout(containerL);
    containerL->setContentsMargins(2,2,2,2);

    m_tw1->addTab(containerW, "Container");
    containerL->addWidget(ui::F::gen(ui::L::HB(), {m_followEyeCamera()}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
    containerL->addWidget(m_cameraSettings.frame);
    containerL->addWidget(ui::F::gen(ui::L::HB(), {m_pivot(), ui::W::txt("Distance"),  m_distance()}, LStretch{true}, LMargins{true}, QFrame::Box));
    containerL->addStretch();

    QFrame *minMaxVal = ui::F::gen(ui::L::HB(), {
            ui::W::txt("Min "),     m_minValue(),
            ui::W::txt("Value"),    m_initialValue(),
            ui::W::txt("Max"),      m_maxValue(),
            ui::W::txt("Step"),     m_stepValue()
        },LStretch{true}, LMargins{false}
    );
    QFrame *backgroundCol = ui::F::gen(ui::L::VB(), {ui::W::txt("Background"), m_colBackgound()});
    QFrame *handleCol     = ui::F::gen(ui::L::VB(), {ui::W::txt("Handle"),     m_colHandle()});
    QFrame *fillAreaCol   = ui::F::gen(ui::L::VB(), {ui::W::txt("Fill area"),  m_colFillArea()});
    QFrame *restAreaCol   = ui::F::gen(ui::L::VB(), {ui::W::txt("Rest area"),  m_colRestArea()});
    QFrame *colors        = ui::F::gen(ui::L::HB(), {backgroundCol, handleCol, fillAreaCol, restAreaCol}, LStretch{true}, LMargins{false});

    auto v1 = ui::F::gen(ui::L::VB(), {ui::W::txt("<b>Values</b>"), m_wholeValue(), m_randomStart(), minMaxVal}, LStretch{true}, LMargins{false});
    auto st = ui::F::gen(ui::L::HB(), {m_sliderText1(), m_sliderText2()}, LStretch{true}, LMargins{false});
    auto v2 = ui::F::gen(ui::L::VB(), {ui::W::txt("<b>Display</b>"), m_displayMinMax(), m_displayMinMaxValue(), m_displaySliderTextes(), st, m_displayNothing(), m_displaySteps()}, LStretch{true}, LMargins{false});
    auto v3 = ui::F::gen(ui::L::VB(), {ui::W::txt("<b>Colors</b>"), colors}, LStretch{true}, LMargins{false});
    auto sliderTab = ui::F::gen(ui::L::VB(), { v1,v2,v3}, LStretch{true}, LMargins{true});
    sliderTab->layout()->setSpacing(10);

    m_tw1->addTab(sliderTab, "Slider");
    m_tw1->addTab(m_tw2 = new QTabWidget(), "Texts");

    m_tw2->addTab(m_text1Tw = new QTabWidget(), "Description 1");
    m_tw2->addTab(m_text2Tw = new QTabWidget(), "Description 2");
    m_tw2->addTab(m_minTextTw = new QTabWidget(), "Min");
    m_tw2->addTab(m_maxTextTw = new QTabWidget(), "Max");
    m_tw2->addTab(m_valueTextTw = new QTabWidget(), "Current value");

    m_text1Tw->addTab(m_text1.textW, "Text");
    m_text1Tw->addTab(m_text1.settingsW, "Settings");

    m_text2Tw->addTab(m_text2.textW, "Text");
    m_text2Tw->addTab(m_text2.settingsW, "Settings");

    m_minTextTw->addTab(m_minTxt.textW, "Text");
    m_minTextTw->addTab(m_minTxt.settingsW, "Settings");

    m_maxTextTw->addTab(m_maxTxt.textW, "Text");
    m_maxTextTw->addTab(m_maxTxt.settingsW, "Settings");

    m_valueTextTw->addTab(m_valueTxt.textW, "Text");
    m_valueTextTw->addTab(m_valueTxt.settingsW, "Settings");
}

void SliderUiConfigParametersW::init_and_register_widgets(){

    Vector2dSettings pivotSettings= {
        {MinV<qreal>{-0.5}, V<qreal>{0.5}, MaxV<qreal>{1.5}, StepV<qreal>{0.01}, 2},
        {MinV<qreal>{-0.5}, V<qreal>{0.5}, MaxV<qreal>{1.5}, StepV<qreal>{0.01}, 2}
    };
    DsbSettings distanceSettings= {MinV<qreal>{0}, V<qreal>{1.}, MaxV<qreal>{1000.}, StepV<qreal>{0.1}, 2};
    add_input_ui(m_pivot.init_widget("Pivot", pivotSettings));
    add_input_ui(m_distance.init_widget(distanceSettings));
    add_input_ui(m_followEyeCamera.init_widget("Slider always in front of the eyes camera", true));
    map_sub_part(m_cameraSettings.init_widget());

    map_sub_part(m_text1.init_widget("Description txt1"));
    map_sub_part(m_text2.init_widget("Description txt2"));
    map_sub_part(m_minTxt.init_widget());
    map_sub_part(m_maxTxt.init_widget());
    map_sub_part(m_valueTxt.init_widget());

    add_input_ui(m_minValue.init_widget(  MinV<qreal>{-10000}, V<qreal>{0.}, MaxV<qreal>{10000}, StepV<qreal>{0.01},2));
    add_input_ui(m_maxValue.init_widget( MinV<qreal>{-10000}, V<qreal>{1.}, MaxV<qreal>{10000}, StepV<qreal>{0.01},2));
    add_input_ui(m_initialValue.init_widget( MinV<qreal>{-10000}, V<qreal>{0.5}, MaxV<qreal>{10000}, StepV<qreal>{0.01},2));
    add_input_ui(m_stepValue.init_widget( MinV<qreal>{0.01}, V<qreal>{0.1}, MaxV<qreal>{10000}, StepV<qreal>{0.01},2));
    add_input_ui(m_wholeValue.init_widget( "Use whole values", false));
    add_input_ui(m_randomStart.init_widget( "Random start", false));

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(group1,
            {&m_displayMinMax, &m_displayMinMaxValue, &m_displaySliderTextes, &m_displayNothing},
            {
                "Display min/max",
                "Display min/max/value",
                "Display slider descriptions",
                "Display nothing"
            },
            {false, true, false, false}
        )
    );

    add_input_ui(m_displaySteps.init_widget("Display steps", false));

    add_input_ui(m_sliderText1.init_widget("Slider txt1"));
    add_input_ui(m_sliderText2.init_widget("Slider txt2"));

    add_input_ui(m_colBackgound.init_widget("Select background color", Qt::white));
    add_input_ui(m_colHandle.init_widget("Select handle color",        Qt::gray));
    add_input_ui(m_colFillArea.init_widget("Select fill area color",   Qt::gray));
    add_input_ui(m_colRestArea.init_widget("Select rest area color",   Qt::gray));
}

void SliderUiConfigParametersW::create_connections(){
    connect(m_wholeValue(),         &QCheckBox::clicked, this, &SliderUiConfigParametersW::update_ui_value_type);
    connect(m_minValue(),     QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,  &SliderUiConfigParametersW::update_ui_value_limits);
    connect(m_initialValue(), QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,  &SliderUiConfigParametersW::update_ui_value_limits);
    connect(m_maxValue(),     QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,  &SliderUiConfigParametersW::update_ui_value_limits);
    connect(m_stepValue(),     QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,  &SliderUiConfigParametersW::update_ui_value_limits);


    connect(m_followEyeCamera(), &QCheckBox::toggled, this, [&](bool checked){
        m_pivot.w->setEnabled(checked);
        m_distance.w->setEnabled(checked);
        m_cameraSettings.set_position_enable_state(!checked,!checked,!checked);
    });

    connect(m_displayMinMax.w.get(), &QRadioButton::clicked, [&]{
        m_sliderText1.w->setEnabled(false);
        m_sliderText2.w->setEnabled(false);
    });
    connect(m_displayMinMaxValue.w.get(), &QRadioButton::clicked, [&]{
        m_sliderText1.w->setEnabled(false);
        m_sliderText2.w->setEnabled(false);
    });
    connect(m_displaySliderTextes.w.get(), &QRadioButton::clicked, [&]{
        m_sliderText1.w->setEnabled(true);
        m_sliderText2.w->setEnabled(true);
    });
    connect(m_displayNothing.w.get(), &QRadioButton::clicked, [&]{
        m_sliderText1.w->setEnabled(false);
        m_sliderText2.w->setEnabled(false);
    });

    connect(m_randomStart(), &QCheckBox::clicked, this, [&]{
        m_initialValue.w->setEnabled(!m_randomStart.w->isChecked());
    });
}

void SliderUiConfigParametersW::late_update_ui(){

    update_ui_value_type(m_wholeValue()->isChecked());

    m_pivot.w->setEnabled(m_followEyeCamera()->isChecked());
    m_distance.w->setEnabled(m_followEyeCamera()->isChecked());
    bool state2 = !m_followEyeCamera()->isChecked();
    m_cameraSettings.set_position_enable_state(state2,state2,state2);

    m_sliderText1.w->setEnabled(m_displaySliderTextes.w->isChecked());
    m_sliderText2.w->setEnabled(m_displaySliderTextes.w->isChecked());

    m_initialValue.w->setEnabled(!m_randomStart.w->isChecked());
}
