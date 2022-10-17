
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

// qt-utility
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_vector2d_w.hpp"
#include "gui/ex_widgets/ex_float_spin_box_w.hpp"
#include "gui/ex_widgets/ex_radio_button_w.hpp"
#include "gui/ex_widgets/ex_select_color_w.hpp"
#include "gui/ex_widgets/ex_line_edit_w.hpp"

using namespace tool::ex;

struct SliderUiInitConfigParametersW::Impl{
    ExCheckBoxW horizontalSlider{"horizontal_slider"};
};

SliderUiInitConfigParametersW::SliderUiInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void SliderUiInitConfigParametersW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::HB(), {m_p->horizontalSlider()}));
}

void SliderUiInitConfigParametersW::init_and_register_widgets(){
    add_input_ui(m_p->horizontalSlider.init_widget("Horizontal slider", true));
}

struct SliderUiConfigParametersW::Impl{

    QTabWidget *tw1 = nullptr;
    QTabWidget *tw2 = nullptr;
    QTabWidget *text1Tw = nullptr;
    QTabWidget *text2Tw = nullptr;
    QTabWidget *minTextTw = nullptr;
    QTabWidget *maxTextTw = nullptr;
    QTabWidget *valueTextTw = nullptr;

    WordSpaceCanvasSubPart cameraSettings;
    ExCheckBoxW followEyeCamera{"use_eye_camera"};
    ExVector2dW pivot{"pivot"};
    ExFloatSpinBoxW distance{"distance"};

    // textes settings
    TextSubPart text1 {"t1"};
    TextSubPart text2 {"t2"};
    TextSubPart minTxt {"tmin", true};
    TextSubPart maxTxt {"tmax", true};
    TextSubPart valueTxt {"tvalue", true};

    // slider settings
    QButtonGroup group1;
    ExRadioButtonW displayMinMax{"display_min_max"};
    ExRadioButtonW displayMinMaxValue{"display_min_max_value"};
    ExRadioButtonW displaySliderTextes{"display_slider_textes_text"};
    ExRadioButtonW displayNothing{"display_nothing"};
    ExCheckBoxW displaySteps{"display_steps"};

    ExLineEditW sliderText1{"slider_text1"};
    ExLineEditW sliderText2{"slider_text2"};

    ExSelectColorW colBackgound{"background_color"};
    ExSelectColorW colHandle{"handle_color"};
    ExSelectColorW colFillArea{"fill_area_color"};
    ExSelectColorW colRestArea{"rest_area_color"};

    ExCheckBoxW wholeValue{"whole"};

    QButtonGroup group2;
    ExRadioButtonW normalStart{"normal_start"};
    ExRadioButtonW onceStart{"once_start"};
    ExRadioButtonW randomStart{"random_start"};
    ExRadioButtonW onceRandomStart{"once_random_start"};
    ExRadioButtonW doNothing{"do_nothing"};

    ExFloatSpinBoxW minValue{"min"};
    ExFloatSpinBoxW initialValue{"value"};
    ExFloatSpinBoxW maxValue{"max"};
    ExFloatSpinBoxW stepValue{"step"};
};


SliderUiConfigParametersW::SliderUiConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void SliderUiConfigParametersW::update_ui_value_type(bool whole){
    m_p->minValue()->setDecimals(whole ? 0 : 2);
    m_p->initialValue()->setDecimals(whole ? 0 : 2);
    m_p->maxValue()->setDecimals(whole ? 0 : 2);
    m_p->stepValue()->setDecimals(whole ? 0 : 2);

    m_p->stepValue()->setSingleStep(whole ? 1 : 0.01);
    m_p->minValue()->setSingleStep(whole ? 1 : 0.01);
    m_p->initialValue()->setSingleStep(whole ? 1 : 0.01);
    m_p->maxValue()->setSingleStep(whole ? 1 : 0.01);
}

void SliderUiConfigParametersW::update_ui_value_limits(qreal value){
    Q_UNUSED(value)
    m_p->initialValue()->setMinimum(m_p->minValue()->value());
    m_p->maxValue()->setMinimum(m_p->minValue()->value());
    m_p->initialValue()->setMaximum(m_p->maxValue()->value());
    m_p->minValue()->setMaximum(m_p->maxValue()->value());
}

void SliderUiConfigParametersW::insert_widgets(){

    layout()->setContentsMargins(0,0,0,0);
    add_widget(m_p->tw1 = new QTabWidget());

    QWidget *containerW = new QWidget();
    QVBoxLayout *containerL = new QVBoxLayout();
    containerW->setLayout(containerL);
    containerL->setContentsMargins(2,2,2,2);

    m_p->tw1->addTab(containerW, "Container");
    containerL->addWidget(ui::F::gen(ui::L::HB(), {m_p->followEyeCamera()}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
    containerL->addWidget(m_p->cameraSettings.frame);
    containerL->addWidget(ui::F::gen(ui::L::HB(), {m_p->pivot(), ui::W::txt("Distance"),  m_p->distance()}, LStretch{true}, LMargins{true}, QFrame::Box));
    containerL->addStretch();

    QFrame *minMaxVal = ui::F::gen(ui::L::HB(), {
            ui::W::txt("Min "),     m_p->minValue(),
            ui::W::txt("Value"),    m_p->initialValue(),
            ui::W::txt("Max"),      m_p->maxValue(),
            ui::W::txt("Step"),     m_p->stepValue()
        },LStretch{true}, LMargins{false}
    );
    QFrame *backgroundCol = ui::F::gen(ui::L::VB(), {ui::W::txt("Background"), m_p->colBackgound()});
    QFrame *handleCol     = ui::F::gen(ui::L::VB(), {ui::W::txt("Handle"),     m_p->colHandle()});
    QFrame *fillAreaCol   = ui::F::gen(ui::L::VB(), {ui::W::txt("Fill area"),  m_p->colFillArea()});
    QFrame *restAreaCol   = ui::F::gen(ui::L::VB(), {ui::W::txt("Rest area"),  m_p->colRestArea()});
    QFrame *colors        = ui::F::gen(ui::L::HB(), {backgroundCol, handleCol, fillAreaCol, restAreaCol}, LStretch{true}, LMargins{false});

    auto v1 = ui::F::gen(ui::L::VB(), {ui::W::txt("<b>Values</b>"), m_p->wholeValue(), minMaxVal}, LStretch{true}, LMargins{false});
    auto v2 = ui::F::gen(ui::L::VB(), {ui::W::txt("<b>Init slider with:</b>"), m_p->normalStart(),m_p->onceStart(),m_p->randomStart(),m_p->onceRandomStart()}, LStretch{true}, LMargins{false});
    auto st = ui::F::gen(ui::L::HB(), {m_p->sliderText1(), m_p->sliderText2()}, LStretch{true}, LMargins{false});
    auto v3 = ui::F::gen(ui::L::VB(), {ui::W::txt("<b>Display</b>"), m_p->displayMinMax(), m_p->displayMinMaxValue(), m_p->displaySliderTextes(), st, m_p->displayNothing(), m_p->displaySteps()}, LStretch{true}, LMargins{false});
    auto v4 = ui::F::gen(ui::L::VB(), {ui::W::txt("<b>Colors</b>"), colors}, LStretch{true}, LMargins{false});
    auto sliderTab = ui::F::gen(ui::L::VB(), {v1,v2,v3,v4}, LStretch{true}, LMargins{true});
    sliderTab->layout()->setSpacing(10);

    m_p->tw1->addTab(sliderTab, "Slider");
    m_p->tw1->addTab(m_p->tw2 = new QTabWidget(), "Texts");

    m_p->tw2->addTab(m_p->text1Tw = new QTabWidget(), "Description 1");
    m_p->tw2->addTab(m_p->text2Tw = new QTabWidget(), "Description 2");
    m_p->tw2->addTab(m_p->minTextTw = new QTabWidget(), "Min");
    m_p->tw2->addTab(m_p->maxTextTw = new QTabWidget(), "Max");
    m_p->tw2->addTab(m_p->valueTextTw = new QTabWidget(), "Current value");

    m_p->text1Tw->addTab(m_p->text1.textW, "Text");
    m_p->text1Tw->addTab(m_p->text1.settingsW, "Settings");

    m_p->text2Tw->addTab(m_p->text2.textW, "Text");
    m_p->text2Tw->addTab(m_p->text2.settingsW, "Settings");

    m_p->minTextTw->addTab(m_p->minTxt.textW, "Text");
    m_p->minTextTw->addTab(m_p->minTxt.settingsW, "Settings");

    m_p->maxTextTw->addTab(m_p->maxTxt.textW, "Text");
    m_p->maxTextTw->addTab(m_p->maxTxt.settingsW, "Settings");

    m_p->valueTextTw->addTab(m_p->valueTxt.textW, "Text");
    m_p->valueTextTw->addTab(m_p->valueTxt.settingsW, "Settings");
}

void SliderUiConfigParametersW::init_and_register_widgets(){

    Vector2dSettings pivotSettings= {
        {MinV<qreal>{-0.5}, V<qreal>{0.5}, MaxV<qreal>{1.5}, StepV<qreal>{0.01}, 2},
        {MinV<qreal>{-0.5}, V<qreal>{0.5}, MaxV<qreal>{1.5}, StepV<qreal>{0.01}, 2}
    };
    DsbSettings distanceSettings= {MinV<qreal>{0}, V<qreal>{1.}, MaxV<qreal>{1000.}, StepV<qreal>{0.1}, 2};
    add_input_ui(m_p->pivot.init_widget("Pivot", pivotSettings));
    add_input_ui(m_p->distance.init_widget(distanceSettings));
    add_input_ui(m_p->followEyeCamera.init_widget("Slider always in front of the eyes camera", true));
    map_sub_part(m_p->cameraSettings.init_widget());

    map_sub_part(m_p->text1.init_widget("Description txt1"));
    map_sub_part(m_p->text2.init_widget("Description txt2"));
    map_sub_part(m_p->minTxt.init_widget());
    map_sub_part(m_p->maxTxt.init_widget());
    map_sub_part(m_p->valueTxt.init_widget());

    add_input_ui(m_p->minValue.init_widget(  MinV<qreal>{-10000}, V<qreal>{0.}, MaxV<qreal>{10000}, StepV<qreal>{0.01},2));
    add_input_ui(m_p->maxValue.init_widget( MinV<qreal>{-10000}, V<qreal>{1.}, MaxV<qreal>{10000}, StepV<qreal>{0.01},2));
    add_input_ui(m_p->initialValue.init_widget( MinV<qreal>{-10000}, V<qreal>{0.5}, MaxV<qreal>{10000}, StepV<qreal>{0.01},2));
    add_input_ui(m_p->stepValue.init_widget( MinV<qreal>{0.01}, V<qreal>{0.1}, MaxV<qreal>{10000}, StepV<qreal>{0.01},2));
    add_input_ui(m_p->wholeValue.init_widget( "Use whole values", false));

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
            {&m_p->displayMinMax, &m_p->displayMinMaxValue, &m_p->displaySliderTextes, &m_p->displayNothing},
            {
                "Display min/max",
                "Display min/max/value",
                "Display slider descriptions",
                "Display nothing"
            },
            {false, true, false, false}
        )
    );

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group2,
           {&m_p->normalStart, &m_p->onceStart, &m_p->randomStart, &m_p->onceRandomStart, &m_p->doNothing},
           {
               "\"value\"",
               "\"value\" only once during the experiment",
               "random value between \"min\" and \"max\"",
               "random value between \"min\" and \"max\" only once during the experiment",
               "do nothing"
           },
           {true, false, false, false, false}
        )
    );

    add_input_ui(m_p->displaySteps.init_widget("Display steps", false, false));

    add_input_ui(m_p->sliderText1.init_widget("Slider txt1"));
    add_input_ui(m_p->sliderText2.init_widget("Slider txt2"));

    add_input_ui(m_p->colBackgound.init_widget("Select background color", Qt::white));
    add_input_ui(m_p->colHandle.init_widget("Select handle color",        Qt::gray));
    add_input_ui(m_p->colFillArea.init_widget("Select fill area color",   Qt::gray));
    add_input_ui(m_p->colRestArea.init_widget("Select rest area color",   Qt::gray));
}

void SliderUiConfigParametersW::create_connections(){

    connect(m_p->wholeValue(),   &QCheckBox::clicked, this, &SliderUiConfigParametersW::update_ui_value_type);
    connect(m_p->minValue(),     QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,  &SliderUiConfigParametersW::update_ui_value_limits);
    connect(m_p->initialValue(), QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,  &SliderUiConfigParametersW::update_ui_value_limits);
    connect(m_p->maxValue(),     QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,  &SliderUiConfigParametersW::update_ui_value_limits);
    connect(m_p->stepValue(),    QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,  &SliderUiConfigParametersW::update_ui_value_limits);

    connect(m_p->followEyeCamera(), &QCheckBox::toggled, this, [&](bool checked){
        m_p->pivot.w->setEnabled(checked);
        m_p->distance.w->setEnabled(checked);
        m_p->cameraSettings.set_position_enable_state(!checked,!checked,!checked);
    });
    connect(m_p->displayMinMax.w.get(), &QRadioButton::clicked, [&]{
        m_p->sliderText1.w->setEnabled(false);
        m_p->sliderText2.w->setEnabled(false);
    });
    connect(m_p->displayMinMaxValue.w.get(), &QRadioButton::clicked, [&]{
        m_p->sliderText1.w->setEnabled(false);
        m_p->sliderText2.w->setEnabled(false);
    });
    connect(m_p->displaySliderTextes.w.get(), &QRadioButton::clicked, [&]{
        m_p->sliderText1.w->setEnabled(true);
        m_p->sliderText2.w->setEnabled(true);
    });
    connect(m_p->displayNothing.w.get(), &QRadioButton::clicked, [&]{
        m_p->sliderText1.w->setEnabled(false);
        m_p->sliderText2.w->setEnabled(false);
    });


    connect(m_p->normalStart(), &QRadioButton::clicked, this, [&]{
        m_p->initialValue.w->setEnabled(!m_p->randomStart.w->isChecked() && !m_p->onceRandomStart.w->isChecked());
    });
    connect(m_p->onceStart(), &QRadioButton::clicked, this, [&]{
        m_p->initialValue.w->setEnabled(!m_p->randomStart.w->isChecked() && !m_p->onceRandomStart.w->isChecked());
    });
    connect(m_p->randomStart(), &QRadioButton::clicked, this, [&]{
        m_p->initialValue.w->setEnabled(!m_p->randomStart.w->isChecked() && !m_p->onceRandomStart.w->isChecked());
    });
    connect(m_p->onceRandomStart(), &QRadioButton::clicked, this, [&]{
        m_p->initialValue.w->setEnabled(!m_p->randomStart.w->isChecked() && !m_p->onceRandomStart.w->isChecked());
    });
}

void SliderUiConfigParametersW::late_update_ui(){

    update_ui_value_type(m_p->wholeValue()->isChecked());

    m_p->pivot.w->setEnabled(m_p->followEyeCamera()->isChecked());
    m_p->distance.w->setEnabled(m_p->followEyeCamera()->isChecked());
    bool state2 = !m_p->followEyeCamera()->isChecked();
    m_p->cameraSettings.set_position_enable_state(state2,state2,state2);

    m_p->sliderText1.w->setEnabled(m_p->displaySliderTextes.w->isChecked());
    m_p->sliderText2.w->setEnabled(m_p->displaySliderTextes.w->isChecked());

    m_p->initialValue.w->setEnabled(!m_p->randomStart.w->isChecked() && !m_p->onceRandomStart.w->isChecked());
}
