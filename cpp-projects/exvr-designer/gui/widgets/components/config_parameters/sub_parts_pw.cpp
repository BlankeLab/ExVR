
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

#include "sub_parts_pw.hpp"

// qt-utility
#include "gui/ex_widgets/ex_transformation_w.hpp"
#include "gui/ex_widgets/ex_float_spin_box_w.hpp"
#include "gui/ex_widgets/ex_spin_box_w.hpp"
#include "gui/ex_widgets/ex_vector3d_w.hpp"
#include "gui/ex_widgets/ex_combo_box_text_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_select_color_w.hpp"
#include "gui/ex_widgets/ex_text_edit_w.hpp"
#include "gui/ex_widgets/ex_radio_button_w.hpp"
#include "gui/ex_widgets/ex_list_labels_w.hpp"
#include "gui/ex_widgets/ex_line_edit_w.hpp"
#include "gui/ex_widgets/ex_combo_box_index_w.hpp"
#include "gui/ex_widgets/ex_slider_w.hpp"
#include "gui/ex_widgets/ex_pushbutton_w.hpp"
#include "gui/ex_widgets/ex_double_spin_box_w.hpp"
#include "gui/ex_widgets/ex_curve_w.hpp"
#include "gui/ex_widgets/ex_color_frame_w.hpp"
#include "gui/ex_widgets/ex_label_w.hpp"

// local
#include "gui/ex_widgets/ex_parameters_generator_w.hpp"
#include "gui/ex_widgets/ex_resource_w.hpp"
#include "gui/ex_widgets/ex_components_list_w.hpp"
#include "gui/ex_widgets/ex_tab_w.hpp"
#include "gui/ex_widgets/ex_camera_target_w.hpp"
#include "gui/ex_widgets/ex_component_w.hpp"

using namespace tool::ex;

struct TransformSubPart::Impl{

    Impl(QString name) : tr({name}), doNotApply({name + QSL("_do_not_apply")}){}
    ExTransformationW tr;
    ExCheckBoxW doNotApply;
};

TransformSubPart::TransformSubPart(QString name) : m_p(std::make_unique<Impl>(name)){
    frame = ui::F::gen(ui::L::VB(),{m_p->tr()}, LStretch{false},LMargins{false});
    m_p->tr.w->layout()->addWidget(m_p->doNotApply());
}

TransformSubPart::~TransformSubPart(){}

TransformSubPart *TransformSubPart::init_widget(QString title){

    DsbSettings s1 = {MinV<qreal>{-10000.}, V<qreal>{0}, MaxV<qreal>{10000.}, StepV<qreal>{0.01}, 3};
    DsbSettings s2 = {MinV<qreal>{-10000.}, V<qreal>{0}, MaxV<qreal>{10000.}, StepV<qreal>{0.1},  3};
    DsbSettings s3 = {MinV<qreal>{-10000.}, V<qreal>{1}, MaxV<qreal>{10000.}, StepV<qreal>{0.01}, 3};

    Vector3dSettings trS    = {s1,s1,s1};
    Vector3dSettings rotS   = {s2,s2,s2};
    Vector3dSettings scS    = {s3,s3,s3};

    add_input_ui(m_p->tr.init_widget(title,trS,rotS,scS));
    add_input_ui(m_p->doNotApply.init_widget(QSL("Do not apply"), false));

    return this;
}

void TransformSubPart::set_enable_state(bool s1, bool s2, bool s3){
    m_p->tr.set_enable_state(s1,s2,s3);
}

void TransformSubPart::set_scale_values(tool::geo::Pt3d values){
    m_p->tr.set_scale_values(values);
}

void TransformSubPart::set_rotation_values(geo::Pt3<qreal> v){
    m_p->tr.set_rotation_values(v);
}

void TransformSubPart::set_trans_decimals(int d){
    m_p->tr.set_trans_decimals(d);
}

void TransformSubPart::set_trans_steps(geo::Pt3<qreal> s){
    m_p->tr.set_trans_steps(s);
}

struct WordSpaceCameraCanvasSubPart::Impl{
    // todo : wscc_
    ExFloatSpinBoxW distance {"distance"};
    ExFloatSpinBoxW scaleFactor {"scale_factor"};
    ExSpinBoxW width {"width"};
    ExSpinBoxW height {"height"};
    ExFloatSpinBoxW pivotX {"pivot_x"};
    ExFloatSpinBoxW pivotY {"pivot_y"};
    ExVector3dW rotation {"rotation"};
};

WordSpaceCameraCanvasSubPart::WordSpaceCameraCanvasSubPart() : m_p(std::make_unique<Impl>()){

    auto f0 = ui::F::gen(ui::L::HB(),{ui::W::txt("Distance: "),m_p->distance()}, LStretch{true},LMargins{false});
    auto f1 = ui::F::gen(ui::L::HB(),{ui::W::txt("Scale factor: "), m_p->scaleFactor(),
                                      ui::W::txt("Width: "),m_p->width(), ui::W::txt("Heght: "),m_p->height()}, LStretch{true},LMargins{false});
    auto f2 = ui::F::gen(ui::L::HB(),{ui::W::txt("Pivots x: "),m_p->pivotX(), ui::W::txt("y: "),m_p->pivotY()}, LStretch{true},LMargins{false});
    auto f3 = ui::F::gen(ui::L::HB(),{m_p->rotation()}, LStretch{true},LMargins{false});
    frame   = ui::F::gen(ui::L::VB(),{f0,f1,f2,f3}, LStretch{false},LMargins{true},QFrame::Shape::Box);
}

WordSpaceCameraCanvasSubPart::~WordSpaceCameraCanvasSubPart(){  }


WordSpaceCameraCanvasSubPart *WordSpaceCameraCanvasSubPart::init_widget(){
    add_input_ui(m_p->scaleFactor.init_widget({MinV<qreal>{-10000.}, V<qreal>{1}, MaxV<qreal>{10000.}, StepV<qreal>{0.01}, 2}));
    add_input_ui(m_p->distance.init_widget({MinV<qreal>{-10000.}, V<qreal>{10}, MaxV<qreal>{10000.}, StepV<qreal>{0.1}, 1}));
    add_input_ui(m_p->width.init_widget(MinV<int>{-10000}, V<int>{500}, MaxV<int>{10000}, StepV<int>{1}));
    add_input_ui(m_p->height.init_widget(MinV<int>{-10000}, V<int>{500}, MaxV<int>{10000}, StepV<int>{1}));
    add_input_ui(m_p->pivotX.init_widget({MinV<qreal>{-10.}, V<qreal>{0.5}, MaxV<qreal>{10.}, StepV<qreal>{0.01}, 2}));
    add_input_ui(m_p->pivotY.init_widget({MinV<qreal>{-10.}, V<qreal>{0.5}, MaxV<qreal>{10.}, StepV<qreal>{0.01}, 2}));
    add_input_ui(m_p->rotation.init_widget("Rotation: "));
    m_p->rotation.set_steps({1.,1.,1.});
    return this;
}

void WordSpaceCameraCanvasSubPart::set_wh_enable_state(bool w, bool h){
    m_p->width.w->setEnabled(w);
    m_p->height.w->setEnabled(h);
}

struct WordSpaceCanvasSubPart::Impl{
    // todo : wsc_
    ExSpinBoxW width {"width"};
    ExSpinBoxW height {"height"};
    ExFloatSpinBoxW scaleFactor {"scale_factor"};
    ExVector3dW position {"position"};
    ExVector3dW rotation {"rotation"};
};


WordSpaceCanvasSubPart::WordSpaceCanvasSubPart() : m_p(std::make_unique<Impl>()){

    auto l = new QGridLayout();
    l->addWidget(ui::W::txt("<b>Size</b>"),     0, 0, 1, 6);
    l->addWidget(ui::W::txt("scale factor: "),  1, 0, 1, 1);
    l->addWidget(m_p->scaleFactor(),                 1, 1, 1, 1);
    l->addWidget(ui::W::txt("width: "),         1, 2, 1, 1);
    l->addWidget(m_p->width(),                       1, 3, 1, 1);
    l->addWidget(ui::W::txt("height: "),        1, 4, 1, 1);
    l->addWidget(m_p->height(),                      1, 5, 1, 1);

    l->addWidget(m_p->position.laTitle,  2, 0, 1, 6);
    l->addWidget(m_p->position.xText,    3, 0, 1, 1);
    l->addWidget(m_p->position.x(),      3, 1, 1, 1);
    l->addWidget(m_p->position.yText,    3, 2, 1, 1);
    l->addWidget(m_p->position.y(),      3, 3, 1, 1);
    l->addWidget(m_p->position.zText,    3, 4, 1, 1);
    l->addWidget(m_p->position.z(),      3, 5, 1, 1);

    l->addWidget(m_p->rotation.laTitle,  4, 0, 1, 6);
    l->addWidget(m_p->rotation.xText,    5, 0, 1, 1);
    l->addWidget(m_p->rotation.x(),      5, 1, 1, 1);
    l->addWidget(m_p->rotation.yText,    5, 2, 1, 1);
    l->addWidget(m_p->rotation.y(),      5, 3, 1, 1);
    l->addWidget(m_p->rotation.zText,    5, 4, 1, 1);
    l->addWidget(m_p->rotation.z(),      5, 5, 1, 1);

    frame = ui::F::gen_frame(l, {}, 0, LMarginsD{6,4,6,4,4}, QFrame::Shape::Box);
}

WordSpaceCanvasSubPart::~WordSpaceCanvasSubPart(){}

WordSpaceCanvasSubPart *WordSpaceCanvasSubPart::init_widget(){
    add_input_ui(m_p->scaleFactor.init_widget({MinV<qreal>{0.}, V<qreal>{0.25}, MaxV<qreal>{1000.}, StepV<qreal>{0.01}, 2}));
    add_input_ui(m_p->width.init_widget(MinV<int>{-10000}, V<int>{400}, MaxV<int>{10000}, StepV<int>{1}));
    add_input_ui(m_p->height.init_widget(MinV<int>{-10000}, V<int>{400}, MaxV<int>{10000}, StepV<int>{1}));
    add_input_ui(m_p->position.init_widget("<b>Position</b>"));
    add_input_ui(m_p->rotation.init_widget("<b>Rotation</b>"));
    m_p->rotation.set_steps({1.,1.,1.});
    m_p->position.set_values({0.,0.,10.});
    return this;
}

void WordSpaceCanvasSubPart::set_wh_enable_state(bool w, bool h){
    m_p->width.w->setEnabled(w);
    m_p->height.w->setEnabled(h);
}

void WordSpaceCanvasSubPart::set_position_enable_state(bool x, bool y, bool z){
    m_p->position.set_enable_state({x,y,z});
}

struct EyeRendererSubPart::Impl{
    QLabel t;
    ExComboBoxTextW eye {"eye_to_render"};
};

EyeRendererSubPart::EyeRendererSubPart(): m_p(std::make_unique<Impl>()){
    frame = ui::F::gen(ui::L::HB(),{&m_p->t,m_p->eye()}, LStretch{false},LMargins{true});
}

EyeRendererSubPart::~EyeRendererSubPart(){}

EyeRendererSubPart *EyeRendererSubPart::init_widget(){
    ui::W::init_label(&m_p->t, "Eyes to render: ", true);
    add_input_ui(m_p->eye.init_widget({"Both eyes", "Left eye", "Right eye"}, Index{0}, true));
    return this;
}

void ConfigParametersSubPart::block_signals(bool state){
    for(auto &element : inputUiElements){
        element.second->blockSignals(state);
    }
}

void ConfigParametersSubPart::add_input_ui(ExBaseW *w){
    QStringView uiName = w->itemName;
    inputUiElements.emplace_back(uiName, w);
}

void ConfigParametersSubPart::add_inputs_ui(std::vector<ExBaseW *> widgets){
    for(const auto &w : widgets){
        add_input_ui(w);
    }
}

struct TextSubPart::Impl{
    Impl(QString name, bool removeInputText):
          resourceText({name + "_text_resource"}),
          text({name + "_text"}),
          wrapping({name + "_wrap"}),
          richtext({name + "_rich_text"}),
          automaticFontSize({name + "_auto_size"}),
          fontSize({name + "_font_size"}),
          italic({name + "_italic"}),
          bold({name + "_bold"}),
          highlight({name + "_highlight"}),
          underline({name + "_under_line"}),
          none({name + "_normal"}),
          lowerCase({name + "_lower_case"}),
          upperCase({name + "_upper_case"}),
          alignment({name + "_alignment"}),
          paragraphSpacing({name + "_paragraph_spacing"}),
          lineSpacing({name + "_line_spacing"}),
          wordSpacing({name + "_word_spacing"}),
          characterSpacing({name + "_character_spacing"}),
          faceColor({name + "_face_color"}),
          outlineColor({name + "_outline_color"}),
          outlineWidth({name + "_outline_width"}),
          noInputText(removeInputText){

    }

    // text
    ExResourceW resourceText;
    ExTextEditW text;
    ExCheckBoxW wrapping;
    ExCheckBoxW richtext;
    ExCheckBoxW automaticFontSize;
    ExFloatSpinBoxW fontSize;
    // font type
    // settings
    // # style
    ExCheckBoxW italic;
    ExCheckBoxW bold;
    ExCheckBoxW highlight;
    ExCheckBoxW underline;
    QButtonGroup group1;
    ExRadioButtonW none;
    ExRadioButtonW lowerCase;
    ExRadioButtonW upperCase;
    // # alignment
    ExComboBoxTextW alignment;
    // # spacing
    ExFloatSpinBoxW paragraphSpacing;
    ExFloatSpinBoxW lineSpacing;
    ExFloatSpinBoxW wordSpacing;
    ExFloatSpinBoxW characterSpacing;
    // # colors
    ExSelectColorW faceColor;
    ExSelectColorW outlineColor;
    // # outline
    ExFloatSpinBoxW outlineWidth;

    bool noInputText = false;
};


TextSubPart::TextSubPart(QString name, bool removeInputText) : m_p(std::make_unique<Impl>(name, removeInputText)){

    // text
    textW = new QWidget();
    QVBoxLayout *textL = new QVBoxLayout();
    textW->setLayout(textL);
    textL->setContentsMargins(2,2,2,2);

    if(!m_p->noInputText){
        auto text1  = ui::F::gen(ui::L::HB(), {m_p->resourceText()}, LStretch{false}, LMargins{true}, QFrame::NoFrame);
        auto text2  = ui::F::gen(ui::L::HB(), {m_p->text()}, LStretch{false}, LMargins{true}, QFrame::NoFrame);
        auto text3  = ui::F::gen(ui::L::HB(), {ui::W::txt("Font size"), m_p->fontSize(), m_p->automaticFontSize()}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
        textL->addWidget(ui::F::gen(ui::L::VB(), {ui::W::txt("Text"), text1, text2, text3, m_p->richtext(), m_p->wrapping()}, LStretch{true}, LMargins{true}, QFrame::Box));
    }else{
        textL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Font size"), m_p->fontSize(), m_p->automaticFontSize()}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
    }
    textL->addStretch();

    // settings
    settingsW = new QWidget();
    QVBoxLayout *settingsL = new QVBoxLayout();
    settingsW->setLayout(settingsL);
    settingsL->setContentsMargins(2,2,2,2);

    auto style1 = ui::F::gen(ui::L::HB(), {m_p->none(), m_p->lowerCase(), m_p->upperCase()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    auto style2 = ui::F::gen(ui::L::HB(), {m_p->italic(), m_p->bold(), m_p->highlight(), m_p->underline()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    settingsL->addWidget(ui::F::gen(ui::L::VB(), {ui::W::txt("Style"), style1, style2}, LStretch{true}, LMargins{true}, QFrame::Box));

    auto spacing1 = ui::F::gen(ui::L::HB(), {ui::W::txt("Paragraph"),m_p->paragraphSpacing(),ui::W::txt("Line"), m_p->lineSpacing()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    auto spacing2 = ui::F::gen(ui::L::HB(), {ui::W::txt("Word"),m_p->wordSpacing(),ui::W::txt("Character"), m_p->characterSpacing()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    settingsL->addWidget(ui::F::gen(ui::L::VB(), {ui::W::txt("Spacing"), spacing1, spacing2}, LStretch{true}, LMargins{true}, QFrame::Box));

    auto colors = ui::F::gen(ui::L::HB(), {ui::W::txt("Text"),ui::W::txt("Face"),m_p->faceColor(), ui::W::txt("Outline"), m_p->outlineColor()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    settingsL->addWidget(ui::F::gen(ui::L::VB(), {ui::W::txt("Colors"), colors}, LStretch{true}, LMargins{true}, QFrame::Box));
    settingsL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Outline width"), m_p->outlineWidth()}, LStretch{true}, LMargins{true}, QFrame::Box));
    settingsL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Alignment"), m_p->alignment()}, LStretch{true}, LMargins{true}, QFrame::Box));
    settingsL->addStretch();
}

TextSubPart::~TextSubPart(){}

TextSubPart *TextSubPart::init_widget(QString defaultText){

    DsbSettings spacingSettings= {MinV<qreal>{-10.}, V<qreal>{0.}, MaxV<qreal>{10.}, StepV<qreal>{0.01}, 2};
    DsbSettings fontSizeSettings= {MinV<qreal>{0.5}, V<qreal>{25.}, MaxV<qreal>{150.}, StepV<qreal>{0.1}, 1};
    DsbSettings outlineSettings= {MinV<qreal>{0.}, V<qreal>{0.}, MaxV<qreal>{10.}, StepV<qreal>{0.01}, 2};


    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
            {&m_p->none, &m_p->lowerCase, &m_p->upperCase},
            {
                "Normal",
                "Lower case",
                "Upper case"
            },
            {true, false, false}
        )
    );

    add_input_ui(m_p->paragraphSpacing.init_widget(spacingSettings));
    add_input_ui(m_p->lineSpacing.init_widget(spacingSettings));
    add_input_ui(m_p->wordSpacing.init_widget(spacingSettings));
    add_input_ui(m_p->characterSpacing.init_widget(spacingSettings));
    add_input_ui(m_p->faceColor.init_widget("Select face color", QColor(0,0,0,255)));
    add_input_ui(m_p->outlineColor.init_widget("Select outline color", QColor(0,0,0,255)));
    add_input_ui(m_p->outlineWidth.init_widget(outlineSettings));

    add_input_ui(m_p->italic.init_widget("Italic", false));
    add_input_ui(m_p->bold.init_widget("Bold", false));
    add_input_ui(m_p->highlight.init_widget("Highlight", false));
    add_input_ui(m_p->underline.init_widget("Underline", false));
    add_input_ui(m_p->alignment.init_widget({
        "Middle left","Middle right", "Middle justified", "Middle centered",
        "Bottom left","Bottom right", "Bottom justified", "Bottom centered",
        "Top left","Top right", "Top justified", "Top centered"},
        Index({3})
    ));

    if(!m_p->noInputText){
        add_input_ui(m_p->richtext.init_widget("Use rich text", true));
        add_input_ui(m_p->text.init_widget(defaultText, Qt::TextFormat::PlainText));
        add_input_ui(m_p->resourceText.init_widget(Resource::Type::Text, "Text resource: "));
    }

    add_input_ui(m_p->wrapping.init_widget("Wrap text", true));
    add_input_ui(m_p->automaticFontSize.init_widget("Automatic", false));
    add_input_ui(m_p->fontSize.init_widget(fontSizeSettings));

    return this;
}

