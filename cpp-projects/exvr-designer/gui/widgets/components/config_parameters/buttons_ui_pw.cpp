

#include "buttons_ui_pw.hpp"

// qt-utility
#include "gui/ex_widgets/ex_select_color_w.hpp"
#include "gui/ex_widgets/ex_text_edit_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_radio_button_w.hpp"
#include "gui/ex_widgets/ex_vector2d_w.hpp"
#include "gui/ex_widgets/ex_spin_box_w.hpp"

using namespace tool::ex;

struct ButtonsUiConfigParametersW::Impl{

    QTabWidget tw;
    WordSpaceCanvasSubPart wscsp;

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
    add_widget(&m_p->tw);

    {
        QWidget *tW = new QWidget();
        QVBoxLayout *tL = new QVBoxLayout();
        tW->setLayout(tL);
        tL->setContentsMargins(2,2,2,2);

        m_p->tw.addTab(tW, "Container");
        tL->addWidget(m_p->wscsp.frame);
        tL->addStretch();
    }

    {
        QTabWidget *tabW = new QTabWidget();

        QWidget *tW = new QWidget();
        QVBoxLayout *tL = new QVBoxLayout();
        tW->setLayout(tL);
        tL->setContentsMargins(2,2,2,2);

        tL->addWidget(ui::F::gen(ui::L::VB(), {
            ui::W::txt("Text for each button:"),
            ui::W::txt("Use '_' to separate two buttons horizontally or ' ' to add a space between them.\nReplace button name by #image_alias for using a resource image instead of a text."),
            m_p->buttonsText()}, LStretch{false}, LMargins{true}, QFrame::NoFrame));

        tL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("When routine starts")}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
        tL->addWidget(ui::F::gen(ui::L::HB(), {m_p->updateId(),  m_p->id(), m_p->randomizeId(), m_p->noUpdateId()}, LStretch{true}, LMargins{true}, QFrame::NoFrame));

        tL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Horizontal spacing"),  m_p->horizontalSpacing()}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
        tL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Vertical spacing"),  m_p->verticalSpacing()}, LStretch{true}, LMargins{true}, QFrame::NoFrame));

        tabW->addTab(tW, "Content");
        tabW->addTab(m_p->buttonsTextS.settingsW, "Settings");
        m_p->tw.addTab(tabW, "Buttons");
    }

    m_p->tw.addTab(m_p->rightText.frame,    "Right text");
    m_p->tw.addTab(m_p->leftText.frame,     "Left text");
    m_p->tw.addTab(m_p->topText.frame,      "Top text");
    m_p->tw.addTab(m_p->bottomText.frame,   "Bottom text");
}

void ButtonsUiConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_p->buttonsColor.init_widget("Select button color", QColor(255,255,255,255)));
    add_input_ui(m_p->buttonsText.init_widget("Button1_Button2_Button3\nButton4_Button5_Button6", Qt::TextFormat::PlainText));
    m_p->buttonsText.set_information("Separe buttons text of the same line by using '_' and separe between lines by using return line.");

    add_inputs_ui(
            ExRadioButtonW::init_group_widgets(m_p->gr,
            {&m_p->randomizeId, &m_p->updateId, &m_p->noUpdateId},
            {"Randomize button","Update button","Do nothing"},
            {true, false, false}
        )
    );

    add_input_ui(m_p->id.init_widget(MinV<int>{0}, V<int>{0}, MaxV<int>{50}, StepV<int>{1}));
    map_sub_part(m_p->wscsp.init_widget());

    map_sub_part(m_p->leftText.init_widget("Text"));
    map_sub_part(m_p->rightText.init_widget(""));
    map_sub_part(m_p->topText.init_widget(""));
    map_sub_part(m_p->bottomText.init_widget(""));
    map_sub_part(m_p->buttonsTextS.init_widget(""));

    add_input_ui(m_p->verticalSpacing.init_widget(MinV<int>{0}, V<int>{1}, MaxV<int>{500}, StepV<int>{1}));
    add_input_ui(m_p->horizontalSpacing.init_widget(MinV<int>{0}, V<int>{1}, MaxV<int>{500}, StepV<int>{1}));
}

void ButtonsUiConfigParametersW::create_connections(){
    m_p->wscsp.create_connections();
}

void ButtonsUiConfigParametersW::late_update_ui(){
    m_p->wscsp.late_update_ui();
}
