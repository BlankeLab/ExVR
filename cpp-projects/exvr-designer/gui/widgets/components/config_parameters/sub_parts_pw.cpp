
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
#include "gui/ex_widgets/ex_vector2d_w.hpp"
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

    ExCheckBoxW followEyeCamera{"use_eye_camera"};
    ExCheckBoxW useOriginalSize{"use_original_size"};

    ExSpinBoxW width {"width"};
    ExSpinBoxW height {"height"};
    ExFloatSpinBoxW scaleFactor {"scale_factor"};
    ExVector3dW position {"position"};
    ExVector3dW rotation {"rotation"};

    ExVector2dW pivot{"pivot"};
    ExFloatSpinBoxW distance{"distance"};

    ExSelectColorW backgroundColor{"background_color"};

    ExComboBoxTextW eye {"eye_to_render"};
};


WordSpaceCanvasSubPart::WordSpaceCanvasSubPart() : m_p(std::make_unique<Impl>()){

    auto l = new QGridLayout();
    int idC = 0;
    l->addWidget(ui::W::txt("<b>Viewer settings</b>"),  idC++, 0, 1, 7);
    l->addWidget(m_p->followEyeCamera(),        ++idC, 0, 1, 7);
    l->addWidget(m_p->useOriginalSize(),        ++idC, 0, 1, 7);
    l->addWidget(ui::W::horizontal_line(),      ++idC, 0, 1, 7);
    l->addWidget(ui::W::txt("[Size]"),            ++idC, 0, 1, 1);
    l->addWidget(ui::W::txt("scale factor: "),  idC,   1, 1, 1);
    l->addWidget(m_p->scaleFactor(),            idC,   2, 1, 1);
    l->addWidget(ui::W::txt("width: "),         idC,   3, 1, 1);
    l->addWidget(m_p->width(),                  idC,   4, 1, 1);
    l->addWidget(ui::W::txt("height: "),        idC,   5, 1, 1);
    l->addWidget(m_p->height(),                 idC,   6, 1, 1);

    l->addWidget(m_p->position.laTitle,         ++idC, 0, 1, 1);
    l->addWidget(m_p->position.xText,           idC,   1, 1, 1);
    l->addWidget(m_p->position.x(),             idC,   2, 1, 1);
    l->addWidget(m_p->position.yText,           idC,   3, 1, 1);
    l->addWidget(m_p->position.y(),             idC,   4, 1, 1);
    l->addWidget(m_p->position.zText,           idC,   5, 1, 1);
    l->addWidget(m_p->position.z(),             idC,   6, 1, 1);

    l->addWidget(m_p->pivot.laTitle,            ++idC, 0, 1, 1);
    l->addWidget(m_p->pivot.xText,              idC,   1, 1, 1);
    l->addWidget(m_p->pivot.x(),                idC,   2, 1, 1);
    l->addWidget(m_p->pivot.yText,              idC,   3, 1, 1);
    l->addWidget(m_p->pivot.y(),                idC,   4, 1, 1);
    l->addWidget(ui::W::txt("distance: "),      idC,   5, 1, 1);
    l->addWidget(m_p->distance(),               idC,   6, 1, 1);

    l->addWidget(m_p->rotation.laTitle,         ++idC, 0, 1, 1);
    l->addWidget(m_p->rotation.xText,           idC,   1, 1, 1);
    l->addWidget(m_p->rotation.x(),             idC,   2, 1, 1);
    l->addWidget(m_p->rotation.yText,           idC,   3, 1, 1);
    l->addWidget(m_p->rotation.y(),             idC,   4, 1, 1);
    l->addWidget(m_p->rotation.zText,           idC,   5, 1, 1);
    l->addWidget(m_p->rotation.z(),             idC,   6, 1, 1);

    l->addWidget(ui::W::horizontal_line(),      ++idC, 0, 1, 7);

    auto bgc = ui::F::gen(ui::L::HB(), {ui::W::txt("Background color"),  m_p->backgroundColor()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    l->addWidget(bgc,                           ++idC, 0, 1, 7);

    l->addWidget(ui::W::horizontal_line(),      ++idC, 0, 1, 7);
    l->addWidget(ui::W::txt("Eyes to render:"),  ++idC, 0, 1, 3);
    l->addWidget(m_p->eye(),                    idC  , 3, 1, 4);

    frame = ui::F::gen_frame(l, {}, 0, LMarginsD{6,4,6,4,4}, QFrame::Shape::Box);
}

WordSpaceCanvasSubPart::~WordSpaceCanvasSubPart(){}

WordSpaceCanvasSubPart *WordSpaceCanvasSubPart::init_widget(){
    add_input_ui(m_p->followEyeCamera.init_widget("always in front of the eyes camera", true));
    add_input_ui(m_p->scaleFactor.init_widget({MinV<qreal>{0.}, V<qreal>{0.25}, MaxV<qreal>{1000.}, StepV<qreal>{0.01}, 2}));
    add_input_ui(m_p->width.init_widget(MinV<int>{-10000}, V<int>{400}, MaxV<int>{10000}, StepV<int>{1}));
    add_input_ui(m_p->height.init_widget(MinV<int>{-10000}, V<int>{400}, MaxV<int>{10000}, StepV<int>{1}));
    add_input_ui(m_p->position.init_widget("[Position]"));
    add_input_ui(m_p->rotation.init_widget("[Rotation]"));
    add_input_ui(m_p->eye.init_widget({"Both eyes", "Left eye", "Right eye"}, Index{0}, true));
    add_input_ui(m_p->useOriginalSize.init_widget("use original size", true));
    add_input_ui(m_p->backgroundColor.init_widget("Background color", QColor(255,255,255,255)));
    m_p->rotation.set_steps({1.,1.,1.});
    m_p->position.set_values({0.,0.,10.});

    Vector2dSettings pivotSettings= {
        {MinV<qreal>{-0.5}, V<qreal>{0.5}, MaxV<qreal>{1.5}, StepV<qreal>{0.01}, 2},
        {MinV<qreal>{-0.5}, V<qreal>{0.5}, MaxV<qreal>{1.5}, StepV<qreal>{0.01}, 2}
    };
    add_input_ui(m_p->pivot.init_widget("[Pivot]", pivotSettings));
    DsbSettings distanceSettings= {MinV<qreal>{0}, V<qreal>{1.}, MaxV<qreal>{1000.}, StepV<qreal>{0.1}, 2};
    add_input_ui(m_p->distance.init_widget(distanceSettings));

    return this;
}

void WordSpaceCanvasSubPart::set_wh_enable_state(bool w, bool h){
    m_p->width.w->setEnabled(w);
    m_p->height.w->setEnabled(h);
}

void WordSpaceCanvasSubPart::set_position_enable_state(bool x, bool y, bool z){
    m_p->position.set_enable_state({x,y,z});
}

void WordSpaceCanvasSubPart::set_background_color(QColor bgCol){
    m_p->backgroundColor.set_color(bgCol);
}

void WordSpaceCanvasSubPart::create_connections() {

    connect(m_p->useOriginalSize(), &QCheckBox::toggled, this, [&](bool checked){
        set_wh_enable_state(!checked,!checked);
    });
    connect(m_p->followEyeCamera(), &QCheckBox::toggled, this, [&](bool checked){
        m_p->pivot.w->setEnabled(checked);
        m_p->distance.w->setEnabled(checked);
        set_position_enable_state(!checked,!checked,!checked);
    });
}

void WordSpaceCanvasSubPart::late_update_ui() {

    bool state1 = !m_p->useOriginalSize()->isChecked();
    set_wh_enable_state(state1,state1);

    m_p->pivot.w->setEnabled(m_p->followEyeCamera()->isChecked());
    bool state2 = !m_p->followEyeCamera()->isChecked();
    set_position_enable_state(state2,state2,state2);
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
          resourceComponent({name + "_text_component"}),
          useResource({name + "_use_resource"}),
          useComponent({name + "_use_component"}),
          useText({name + "_use_text"}),
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
    ExComponentW resourceComponent;
    QButtonGroup group2;
    ExRadioButtonW useResource;
    ExRadioButtonW useComponent;
    ExRadioButtonW useText;

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

    auto l = new QGridLayout();
    int idC = 0;
    l->addWidget(ui::W::txt("<b>Text settings</b>"),  idC++, 0, 1, 7);

    if(!m_p->noInputText){

        l->addWidget(m_p->useResource(),         ++idC, 0, 1, 1);
        l->addWidget(m_p->resourceText(),          idC, 1, 1, 6);

        l->addWidget(m_p->useComponent(),        ++idC, 0, 1, 1);
        l->addWidget(m_p->resourceComponent(),     idC, 1, 1, 6);

        l->addWidget(m_p->useText(),             ++idC, 0, 1, 6);
        l->addWidget(m_p->text(),                ++idC, 0, 5, 6);

        idC += 4;
        auto rt = ui::F::gen(ui::L::HB(), {m_p->richtext(), m_p->wrapping()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
        l->addWidget(rt,                         ++idC, 1, 1, 6);
        l->addWidget(ui::W::horizontal_line(),   ++idC, 0, 1, 7);
    }

    auto fontSize = ui::F::gen(ui::L::HB(), {ui::W::txt("[Sizes]"), ui::W::txt("font"), m_p->fontSize(), m_p->automaticFontSize(), ui::W::txt("outline"), m_p->outlineWidth()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    l->addWidget(fontSize,                  ++idC, 0, 1, 7);
    l->addWidget(ui::W::horizontal_line(),  ++idC, 0, 1, 7);

    auto alignment = ui::F::gen(ui::L::HB(), {ui::W::txt("[Alignment]"), m_p->alignment()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    l->addWidget(alignment,                  ++idC, 0, 1, 7);
    l->addWidget(ui::W::horizontal_line(),  ++idC, 0, 1, 7);

    l->addWidget(ui::W::txt("[Style]"),   ++idC, 0, 1, 1);
    auto style = ui::F::gen(ui::L::HB(), {m_p->none(), m_p->lowerCase(), m_p->upperCase(), m_p->italic(), m_p->bold(), m_p->highlight(), m_p->underline()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    l->addWidget(style,                  idC,   1, 1, 6);
    l->addWidget(ui::W::horizontal_line(),  ++idC, 0, 1, 7);

    l->addWidget(ui::W::txt("[Spacing]"), ++idC, 0, 1, 1);
    auto spacing = ui::F::gen(ui::L::HB(), {ui::W::txt("par:"),m_p->paragraphSpacing(),ui::W::txt("line:"), m_p->lineSpacing(), ui::W::txt("word:"),m_p->wordSpacing(),ui::W::txt("char:"), m_p->characterSpacing()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    l->addWidget(spacing,                  idC, 1, 1, 6);

    l->addWidget(ui::W::txt("[Colors]"), ++idC, 0, 1, 1);
    auto colors = ui::F::gen(ui::L::HB(), {ui::W::txt("Text"),ui::W::txt("Face"),m_p->faceColor(), ui::W::txt("Outline"), m_p->outlineColor()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    l->addWidget(colors,                  idC, 1, 1, 6);

    frame = ui::F::gen_frame(l, {}, 0, LMarginsD{6,4,6,4,4}, QFrame::Shape::Box);
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
                "normal",
                "lower case",
                "upper case"
            },
            {true, false, false}
        )
    );

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group2,
       {&m_p->useResource, &m_p->useComponent, &m_p->useText},
       {
           "resource",
           "component",
           "text"
       },
       {false, false, true}
       )
    );

    add_input_ui(m_p->paragraphSpacing.init_widget(spacingSettings));
    add_input_ui(m_p->lineSpacing.init_widget(spacingSettings));
    add_input_ui(m_p->wordSpacing.init_widget(spacingSettings));
    add_input_ui(m_p->characterSpacing.init_widget(spacingSettings));
    add_input_ui(m_p->faceColor.init_widget("face", QColor(0,0,0,255)));
    add_input_ui(m_p->outlineColor.init_widget("outline", QColor(0,0,0,255)));
    add_input_ui(m_p->outlineWidth.init_widget(outlineSettings));

    add_input_ui(m_p->italic.init_widget("italic", false));
    add_input_ui(m_p->bold.init_widget("bold", false));
    add_input_ui(m_p->highlight.init_widget("highlight", false));
    add_input_ui(m_p->underline.init_widget("underline", false));
    add_input_ui(m_p->alignment.init_widget({
        "Middle left","Middle right", "Middle justified", "Middle centered",
        "Bottom left","Bottom right", "Bottom justified", "Bottom centered",
        "Top left","Top right", "Top justified", "Top centered"},
        Index({3})
    ));

    if(!m_p->noInputText){
        add_input_ui(m_p->richtext.init_widget("use rich text", true));
        add_input_ui(m_p->text.init_widget(defaultText, Qt::TextFormat::PlainText));
        add_input_ui(m_p->resourceText.init_widget(Resource::Type::Text, "Resource: "));
        add_input_ui(m_p->resourceComponent.init_widget(Component::Type::Text_resource, "Component: "));
    }

    add_input_ui(m_p->wrapping.init_widget("wrap text", true));
    add_input_ui(m_p->automaticFontSize.init_widget("automatic", false));
    add_input_ui(m_p->fontSize.init_widget(fontSizeSettings));

    return this;
}

