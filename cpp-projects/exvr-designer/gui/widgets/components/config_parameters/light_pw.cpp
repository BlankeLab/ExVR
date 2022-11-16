
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

#include "light_pw.hpp"

// qt-utility
#include "gui/ex_widgets/ex_float_spin_box_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_select_color_w.hpp"
#include "gui/ex_widgets/ex_combo_box_index_w.hpp"

// local
#include "gui/ex_widgets/ex_resource_w.hpp"

using namespace tool::ex;
using namespace tool::ui;

struct LightInitConfigParametersW::Impl{
    TransformSubPart transfo = {"init_transform"};
};

LightInitConfigParametersW::LightInitConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void LightInitConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);
}

void LightInitConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
}

struct LightConfigParametersW::Impl{

    TransformSubPart transfo = {"transform"};

    ExComboBoxIndexW type{"type"};
    ExFloatSpinBoxW intensity{"intensity"};
    ExSelectColorW color{"color"};
    ExCheckBoxW displaySource{"display_source"};
    ExResourceW cookieT{"cookie_t"};
    ExFloatSpinBoxW range{"range"};

    ExFloatSpinBoxW spotAngle{"spot_angle"};
//    ExFloatSpinBoxW innerSpotAngle{"inner_spot_angle"};

    ExFloatSpinBoxW cookieSize{"cookie_size"};
    ExFloatSpinBoxW bounceIntensity{"bounce_intensity"};

    ExComboBoxIndexW shadows{"shadows"};
    ExFloatSpinBoxW shadowStrength{"shadow_strength"};
    ExFloatSpinBoxW shadowBias{"shadow_bias"};
    ExFloatSpinBoxW shadowNormalBias{"shadow_normal_bias"};
    ExFloatSpinBoxW shadowNearPlane{"shadow_near_plane"};
};

LightConfigParametersW::LightConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){

}

void LightConfigParametersW::insert_widgets(){

    add_sub_part_widget(m_p->transfo);

    add_widget(F::gen(L::HB(), {W::txt("<b>Type:</b>"),m_p->type()}, LStretch{true}, LMargins{true}));
    auto m1 = F::gen(L::HB(), {W::txt("Intensity: "),m_p->intensity(), W::txt("Range (Point and Spot): "),m_p->range()}, LStretch{true}, LMargins{true});
    auto m2 = F::gen(L::HB(), {W::txt("Color: "),m_p->color()}, LStretch{true}, LMargins{true});
    auto m3 = F::gen(L::HB(), {W::txt("Cookie: "),m_p->cookieT()}, LStretch{false}, LMargins{true});
    auto m4 = F::gen(L::HB(), {m_p->displaySource()}, LStretch{true}, LMargins{true});
    add_widget(F::gen(L::VB(), {W::txt("<b>Common light settings</b>"), m1, m2, m3, m4}, LStretch{false}, LMargins{true}, QFrame::Box));

    auto m5 = F::gen(L::HB(), {W::txt("Angle: "),m_p->spotAngle()}, LStretch{true}, LMargins{true}); // , W::txt("Inner angle: "),m_p->innerSpotAngle()
    add_widget(F::gen(L::VB(), {W::txt("<b>Spot light settings</b>"), m5}, LStretch{false}, LMargins{true}, QFrame::Box));

    auto m6 = F::gen(L::HB(), {W::txt("Cookie size: "),m_p->cookieSize(), W::txt("Bounce intensity: "),m_p->bounceIntensity()}, LStretch{true}, LMargins{true});
    add_widget(F::gen(L::VB(), {W::txt("<b>Directional light settings</b>"), m6}, LStretch{false}, LMargins{true}, QFrame::Box));

    auto m7 = F::gen(L::HB(), {W::txt("Mode: "),m_p->shadows(), W::txt("Strength: "),m_p->shadowStrength()}, LStretch{true}, LMargins{true});
    auto m8 = F::gen(L::HB(), {W::txt("Bias: "),m_p->shadowBias(), W::txt("Normal bias: "),m_p->shadowNormalBias()}, LStretch{true}, LMargins{true});
    auto m9 = F::gen(L::HB(), {W::txt("Near plane: "),m_p->shadowNearPlane()}, LStretch{true}, LMargins{true});
    add_widget(F::gen(L::VB(), {W::txt("<b>Shadows settings</b>"), m7, m8, m9}, LStretch{false}, LMargins{true}, QFrame::Box));
}

void LightConfigParametersW::init_and_register_widgets(){

    map_sub_part(m_p->transfo.init_widget(QSL("Config transform</b> (applied when routine starts)<b>")));

    // light
    add_input_ui(m_p->type.init_widget({"Point", "Spot", "Directional"},0));
    add_input_ui(m_p->intensity.init_widget(MinV<qreal>{0.0}, V<qreal>{1.0}, MaxV<qreal>{100000.}, StepV<qreal>{0.1},2));
    add_input_ui(m_p->color.init_widget("Color", Qt::white));
    add_input_ui(m_p->displaySource.init_widget("Display source (debug)", true));
    add_input_ui(m_p->cookieT.init_widget(Resource::Type::Image, "Cookie: "));
    add_input_ui(m_p->range.init_widget(MinV<qreal>{0.0}, V<qreal>{10.0}, MaxV<qreal>{100000.}, StepV<qreal>{0.1},1));
    add_input_ui(m_p->spotAngle.init_widget(MinV<qreal>{1.0}, V<qreal>{30.0}, MaxV<qreal>{179.}, StepV<qreal>{0.1},2));
//    add_input_ui(m_p->innerSpotAngle.init_widget(MinV<qreal>{1.0}, V<qreal>{21.80}, MaxV<qreal>{179.}, StepV<qreal>{0.1},2));
    add_input_ui(m_p->cookieSize.init_widget(MinV<qreal>{0.0}, V<qreal>{10.0}, MaxV<qreal>{100000.}, StepV<qreal>{0.1},1));
    add_input_ui(m_p->bounceIntensity.init_widget(MinV<qreal>{0.0}, V<qreal>{1.0}, MaxV<qreal>{100000.}, StepV<qreal>{0.1},1));

    // shadows
    add_input_ui(m_p->shadows.init_widget({"None", "Hard", "Soft"},2));
    add_input_ui(m_p->shadowStrength.init_widget(MinV<qreal>{0.0}, V<qreal>{1.0}, MaxV<qreal>{1.}, StepV<qreal>{0.01},2));
    add_input_ui(m_p->shadowBias.init_widget(MinV<qreal>{0.0}, V<qreal>{0.05}, MaxV<qreal>{2.}, StepV<qreal>{0.01},2));
    add_input_ui(m_p->shadowNormalBias.init_widget(MinV<qreal>{0.0}, V<qreal>{0.4}, MaxV<qreal>{3.}, StepV<qreal>{0.01},2));
    add_input_ui(m_p->shadowNearPlane.init_widget(MinV<qreal>{0.1}, V<qreal>{0.2}, MaxV<qreal>{10.}, StepV<qreal>{0.01},2));
}


