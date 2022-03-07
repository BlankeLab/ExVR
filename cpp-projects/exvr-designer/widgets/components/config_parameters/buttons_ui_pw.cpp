

#include "buttons_ui_pw.hpp"

// qt-utility
#include "ex_widgets/ex_select_color_w.hpp"
#include "ex_widgets/ex_text_edit_w.hpp"
#include "ex_widgets/ex_checkbox_w.hpp"
#include "ex_widgets/ex_radio_button_w.hpp"
#include "ex_widgets/ex_vector2d_w.hpp"
#include "ex_widgets/ex_spin_box_w.hpp"

using namespace tool::ex;

struct ButtonsUiConfigParametersW::Impl{
    EyeRendererSubPart eye;
    QTabWidget tw;
    WordSpaceCanvasSubPart cameraSettings;
    ExCheckBoxW followEyeCamera{"use_eye_camera"};
    ExVector2dW pivot{"pivot"};
    ExFloatSpinBoxW distance{"distance"};
    ExSelectColorW backgroundColor{"background_color"};
    ExTextEditW buttonsText{"buttons_text"};
    ExSelectColorW buttonsColor{"buttons_color"};
    ExSpinBoxW horizontalSpacing{"h_spacing"};
    ExSpinBoxW verticalSpacing{"v_spacing"};

    ExRadioButtonW randomizeId{"randomize_id"};
    ExRadioButtonW updateId{"update_id"};
    ExRadioButtonW noUpdateId{"no_update_id"};
    QButtonGroup gr;
    ExSpinBoxW id{"id"};

    TextSubPart buttonsTextS {"buttons_ts"};
    TextSubPart leftText {"left_t"};
    TextSubPart rightText {"right_t"};
    TextSubPart topText {"top_t"};
    TextSubPart bottomText {"bottom_t"};
};


ButtonsUiConfigParametersW::ButtonsUiConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}


void ButtonsUiConfigParametersW::insert_widgets(){

    layout()->setContentsMargins(0,0,0,0);
    add_sub_part_widget(m_p->eye);
    add_widget(&m_p->tw);

    {
        QWidget *tW = new QWidget();
        QVBoxLayout *tL = new QVBoxLayout();
        tW->setLayout(tL);
        tL->setContentsMargins(2,2,2,2);

        m_p->tw.addTab(tW, "Container");
        tL->addWidget(ui::F::gen(ui::L::HB(), {m_p->followEyeCamera()}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
        tL->addWidget(m_p->cameraSettings.frame);
        tL->addWidget(ui::F::gen(ui::L::HB(), {m_p->pivot(), ui::W::txt("Distance"),  m_p->distance()}, LStretch{true}, LMargins{true}, QFrame::Box));
        tL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Background color"),  m_p->backgroundColor(), ui::W::txt("Buttons color"),  m_p->buttonsColor()}, LStretch{true}, LMargins{true}, QFrame::Box));
        tL->addStretch();
    }

    {
        QTabWidget *tabW = new QTabWidget();

        QWidget *tW = new QWidget();
        QVBoxLayout *tL = new QVBoxLayout();
        tW->setLayout(tL);
        tL->setContentsMargins(2,2,2,2);

        tL->addWidget(ui::F::gen(ui::L::VB(), {ui::W::txt("Text for each button:"),  m_p->buttonsText()}, LStretch{false}, LMargins{true}, QFrame::NoFrame));

        tL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("When routine starts")}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
        tL->addWidget(ui::F::gen(ui::L::HB(), {m_p->updateId(),  m_p->id(), m_p->randomizeId(), m_p->noUpdateId()}, LStretch{true}, LMargins{true}, QFrame::NoFrame));

        tL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Horizontal spacing"),  m_p->horizontalSpacing()}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
        tL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Vertical spacing"),  m_p->verticalSpacing()}, LStretch{true}, LMargins{true}, QFrame::NoFrame));

        tabW->addTab(tW, "Content");
        tabW->addTab(m_p->buttonsTextS.settingsW, "Settings");
        m_p->tw.addTab(tabW, "Buttons");
    }

    {
        QTabWidget *tabW = new QTabWidget();
        tabW->addTab(m_p->leftText.textW, "Text");
        tabW->addTab(m_p->leftText.settingsW, "Settings");
        m_p->tw.addTab(tabW, "Left text");
    }

    {
        QTabWidget *tabW = new QTabWidget();
        tabW->addTab(m_p->rightText.textW, "Text");
        tabW->addTab(m_p->rightText.settingsW, "Settings");
        m_p->tw.addTab(tabW, "Right text");
    }

    {
        QTabWidget *tabW = new QTabWidget();
        tabW->addTab(m_p->topText.textW, "Text");
        tabW->addTab(m_p->topText.settingsW, "Settings");
        m_p->tw.addTab(tabW, "Top text");
    }

    {
        QTabWidget *tabW = new QTabWidget();
        tabW->addTab(m_p->bottomText.textW, "Text");
        tabW->addTab(m_p->bottomText.settingsW, "Settings");
        m_p->tw.addTab(tabW, "Bottom text");
    }
}

void ButtonsUiConfigParametersW::init_and_register_widgets(){

    Vector2dSettings pivotSettings= {
        {MinV<qreal>{-5}, V<qreal>{0.5}, MaxV<qreal>{5}, StepV<qreal>{0.01}, 2},
        {MinV<qreal>{-5}, V<qreal>{0.5}, MaxV<qreal>{5}, StepV<qreal>{0.01}, 2}
    };
    DsbSettings distanceSettings= {MinV<qreal>{0}, V<qreal>{1.}, MaxV<qreal>{1000.}, StepV<qreal>{0.1}, 2};
    add_input_ui(m_p->pivot.init_widget("Pivot", pivotSettings));
    add_input_ui(m_p->distance.init_widget(distanceSettings));
    add_input_ui(m_p->followEyeCamera.init_widget("Text always in front of the eyes camera", true));
    add_input_ui(m_p->backgroundColor.init_widget("Select background color", QColor(255,255,255,0)));
    add_input_ui(m_p->buttonsColor.init_widget("Select button color", QColor(255,255,255,255)));
    add_input_ui(m_p->buttonsText.init_widget("Button1_Button2_Button3\nButton4_Button5_Button6"));
    m_p->buttonsText.set_information("Separe buttons text of the same line by using '_' and separe between lines by using return line.");

    add_inputs_ui(
            ExRadioButtonW::init_group_widgets(m_p->gr,
            {&m_p->randomizeId, &m_p->updateId, &m_p->noUpdateId},
            {"Randomize button","Update button","Do nothing"},
            {true, false, false}
        )
    );
    add_input_ui(m_p->id.init_widget(MinV<int>{0}, V<int>{0}, MaxV<int>{50}, StepV<int>{1}));


    map_sub_part(m_p->eye.init_widget());
    map_sub_part(m_p->cameraSettings.init_widget());

    map_sub_part(m_p->leftText.init_widget("Text"));
    map_sub_part(m_p->rightText.init_widget(""));
    map_sub_part(m_p->topText.init_widget(""));
    map_sub_part(m_p->bottomText.init_widget(""));
    map_sub_part(m_p->buttonsTextS.init_widget(""));

    add_input_ui(m_p->verticalSpacing.init_widget(MinV<int>{0}, V<int>{1}, MaxV<int>{500}, StepV<int>{1}));
    add_input_ui(m_p->horizontalSpacing.init_widget(MinV<int>{0}, V<int>{1}, MaxV<int>{500}, StepV<int>{1}));

}

void ButtonsUiConfigParametersW::create_connections(){
    connect(m_p->followEyeCamera(), &QCheckBox::toggled, this, [&](bool checked){
        m_p->pivot.w->setEnabled(checked);
        m_p->distance.w->setEnabled(checked);
        m_p->cameraSettings.set_position_enable_state(!checked,!checked,!checked);
    });
}

void ButtonsUiConfigParametersW::late_update_ui(){
    m_p->pivot.w->setEnabled(m_p->followEyeCamera()->isChecked());
    m_p->distance.w->setEnabled(m_p->followEyeCamera()->isChecked());

    bool state = !m_p->followEyeCamera()->isChecked();
    m_p->cameraSettings.set_position_enable_state(state,state,state);
}
