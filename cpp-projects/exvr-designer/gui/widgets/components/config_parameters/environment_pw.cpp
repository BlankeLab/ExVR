
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

#include "environment_pw.hpp"

// qt-utility
#include "gui/ex_widgets/ex_vector2d_w.hpp"
#include "gui/ex_widgets/ex_vector3d_w.hpp"
#include "gui/ex_widgets/ex_float_spin_box_w.hpp"
#include "gui/ex_widgets/ex_select_color_w.hpp"
#include "gui/ex_widgets/ex_combo_box_index_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_slider_w.hpp"

// local
#include "gui/ex_widgets/ex_resource_w.hpp"

using namespace tool::ex;

struct PostProcessConfigParametersW::Impl{

    ExCheckBoxW cgEnable{"cg_enable"}; // false
    ExCheckBoxW cgWbTemperatureEnable{"cg_wb_temp_enable"}; // false
    ExSliderFloatW cgWbTemperature{"cg_wb_temp"}; // -100 - 0 100
    ExCheckBoxW cgWbTintEnable{"cg_wb_tint_enable"}; // false
    ExSliderFloatW cgWbTint{"cg_wb_tint"}; // -100 - 0 100
    ExCheckBoxW cgTonePostExposureEnable{"cg_tone_post_exposure_enable"}; // false
    ExSliderFloatW cgTonePostExposure{"cg_tone_post_exposure"}; // -10 0 10
    ExCheckBoxW cgToneColorFilterEnable{"cg_tone_color_filter_enable"}; // false
    ExSelectColorW cgToneColorFilter{"cg_tone_color_filter"}; // white
    ExCheckBoxW cgToneHueShiftEnable{"cg_tone_hue_shift_enable"}; // false
    ExSliderIntegerW cgToneHueShift{"cg_tone_hue_shift"}; // -180 0 180
    ExCheckBoxW cgToneSaturationEnable{"cg_tone_saturation_enable"}; // false
    ExSliderIntegerW cgToneSaturation{"cg_tone_saturation"}; // -100 0 100
    ExCheckBoxW cgToneContrastEnable{"cg_tone_contrast_enable"}; // false
    ExSliderIntegerW cgToneContrast{"cg_tone_contrast"}; // -100 0 100

    ExCheckBoxW vEnable{"v_enable"}; // false
    ExCheckBoxW vColorEnable{"v_color_enable"}; // false
    ExSelectColorW vColor{"v_color"}; // black
    ExCheckBoxW vCenterEnable{"v_center_enable"}; // false
    ExVector2dW vCenter{"v_center"}; // 0.5 0.5
    ExCheckBoxW vIntensityEnable{"v_intensity_enable"}; // false
    ExSliderFloatW vIntensity{"v_intensity"}; // 0 0.5 1
    ExCheckBoxW vSmoothnessEnable{"v_smoothness_enable"}; // false
    ExSliderFloatW vSmoothness{"v_smoothness"}; // 0 0.5 1
    ExCheckBoxW vRoundnessEnable{"v_roundness_enable"}; // false
    ExSliderFloatW vRoundness{"v_roundness"}; // 0 0.5 1
    ExCheckBoxW vRoundedEnable{"v_rounded_enable"}; // false
    ExCheckBoxW vRounded{"v_rounded"}; // false

    ExCheckBoxW aoEnable{"ao_enable"}; // true
    ExCheckBoxW aoIntensityEnable{"ao_intensity_enable"}; // true
    ExSliderFloatW aoIntensity{"ao_intensity"}; // 0 - 1.58 - 4
    ExCheckBoxW aoThicknessEnable{"ao_thickness_enable"}; // true
    ExSliderFloatW aoThickness{"ao_thickness"}; // 1 - 1.96 - 10
    ExCheckBoxW aoColorEnable{"ao_color_enable"}; // true
    ExSelectColorW aoColor{"ao_color"}; // black

    QTabWidget *effectsTab = nullptr;
    QWidget *cgW = nullptr;
    QWidget *vW = nullptr;
    QWidget *aoW = nullptr;
};

PostProcessConfigParametersW::PostProcessConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void PostProcessConfigParametersW::insert_widgets(){

    add_widget(m_p->effectsTab = new QTabWidget());

    m_p->effectsTab->addTab(m_p->cgW = new QWidget(), "Color grading");
    auto cgL = ui::L::VB();
    m_p->cgW->setLayout(cgL);
    cgL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->cgEnable(),10}} , 0, LMarginsD{0,0,0,0,2}));
    cgL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt("White balance"),1}} , 0, LMarginsD{0,0,0,0,2}));
    cgL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->cgWbTemperatureEnable(),1}, {m_p->cgWbTemperature(),30}} , 0, LMarginsD{0,0,0,0,2}));
    cgL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->cgWbTintEnable(),1}, {m_p->cgWbTint(),30}} , 0, LMarginsD{0,0,0,0,2}));
    cgL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt("Tone"),1}} , 0, LMarginsD{0,0,0,0,2}));
    cgL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->cgTonePostExposureEnable(),1}, {m_p->cgTonePostExposure(),30}} , 0, LMarginsD{0,0,0,0,2}));
    cgL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->cgToneColorFilterEnable(),1}, {m_p->cgToneColorFilter(),30}} , 0, LMarginsD{0,0,0,0,2}));
    cgL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->cgToneHueShiftEnable(),1}, {m_p->cgToneHueShift(),30}} , 0, LMarginsD{0,0,0,0,2}));
    cgL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->cgToneSaturationEnable(),1}, {m_p->cgToneSaturation(),30}} , 0, LMarginsD{0,0,0,0,2}));
    cgL->addStretch();

    m_p->effectsTab->addTab(m_p->vW = new QWidget(), "Vignette");
    auto vL = ui::L::VB();
    m_p->vW->setLayout(vL);
    vL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->vEnable(),10}} , 0, LMarginsD{0,0,0,0,2}));
    vL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->vColorEnable(),1}, {m_p->vColor(),30}} , 0, LMarginsD{0,0,0,0,2}));
    vL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->vCenterEnable(),1}, {m_p->vCenter(),30}} , 0, LMarginsD{0,0,0,0,2}));
    vL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->vIntensityEnable(),1}, {m_p->vIntensity(),30}} , 0, LMarginsD{0,0,0,0,2}));
    vL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->vSmoothnessEnable(),1}, {m_p->vSmoothness(),30}} , 0, LMarginsD{0,0,0,0,2}));
    vL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->vRoundnessEnable(),1}, {m_p->vRoundness(),30}} , 0, LMarginsD{0,0,0,0,2}));
    vL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->vRoundedEnable(),1}, {m_p->vRounded(),30}} , 0, LMarginsD{0,0,0,0,2}));
    vL->addStretch();

    m_p->effectsTab->addTab(m_p->aoW = new QWidget(), "Ambient Occlusion");
    auto aoL = ui::L::VB();
    m_p->aoW->setLayout(aoL);
    aoL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->aoEnable(),10}} , 0, LMarginsD{0,0,0,0,2}));
    aoL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->aoIntensityEnable(),1}, {m_p->aoIntensity(),30}} , 0, LMarginsD{0,0,0,0,2}));
    aoL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->aoThicknessEnable(),1}, {m_p->aoThickness(),30}} , 0, LMarginsD{0,0,0,0,2}));
    aoL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->aoColorEnable(),1}, {m_p->aoColor(),30}} , 0, LMarginsD{0,0,0,0,2}));
    aoL->addStretch();
    no_end_stretch();
}

void PostProcessConfigParametersW::init_and_register_widgets(){

    // CG
    add_input_ui(m_p->cgEnable.init_widget("Enable", true));
    add_input_ui(m_p->cgWbTemperatureEnable.init_widget("Temperature", false));
    add_input_ui(m_p->cgWbTemperature.init_widget("", MinR{-100.}, ValR{0.}, MaxR{100.}, StepR{0.1}));
    add_input_ui(m_p->cgWbTintEnable.init_widget("Tint", false));
    add_input_ui(m_p->cgWbTint.init_widget("", MinR{-100.}, ValR{0.}, MaxR{100.}, StepR{0.1}));
    add_input_ui(m_p->cgTonePostExposureEnable.init_widget("Exposure", false));
    add_input_ui(m_p->cgTonePostExposure.init_widget("", MinR{-10.}, ValR{0.}, MaxR{10.}, StepR{0.1}));
    add_input_ui(m_p->cgToneColorFilterEnable.init_widget("Color filter", false));
    add_input_ui(m_p->cgToneColorFilter.init_widget("", Qt::white));
    add_input_ui(m_p->cgToneHueShiftEnable.init_widget("Hue shift", false));
    add_input_ui(m_p->cgToneHueShift.init_widget("", MinI{-180}, ValI{0}, MaxI{180}, StepI{1}));
    add_input_ui(m_p->cgToneSaturationEnable.init_widget("Saturation", false));
    add_input_ui(m_p->cgToneSaturation.init_widget("", MinI{-100}, ValI{0}, MaxI{100}, StepI{1}));
    add_input_ui(m_p->cgToneContrastEnable.init_widget("Contrast", false));
    add_input_ui(m_p->cgToneContrast.init_widget("", MinI{-100}, ValI{0}, MaxI{100}, StepI{1}));

    // V
    add_input_ui(m_p->vEnable.init_widget("Enable", true));
    add_input_ui(m_p->vColorEnable.init_widget("Color", false));
    add_input_ui(m_p->vColor.init_widget("", Qt::black));
    add_input_ui(m_p->vCenterEnable.init_widget("Center", false));
    add_input_ui(m_p->vCenter.init_widget("", Vector2dSettings{{MinR{-1.}, ValR{0.5}, MaxR{1.}, StepR{0.01}, 2},{MinR{-1.}, ValR{0.5}, MaxR{1.}, StepR{0.01}, 2}}));
    add_input_ui(m_p->vIntensityEnable.init_widget("Intensity", false));
    add_input_ui(m_p->vIntensity.init_widget("", MinR{0.}, ValR{0.5}, MaxR{1.}, StepR{0.1}));
    add_input_ui(m_p->vSmoothnessEnable.init_widget("Smoothness", false));
    add_input_ui(m_p->vSmoothness.init_widget("", MinR{0.}, ValR{0.5}, MaxR{1.}, StepR{0.1}));
    add_input_ui(m_p->vRoundnessEnable.init_widget("Roundness", false));
    add_input_ui(m_p->vRoundness.init_widget("", MinR{0.}, ValR{0.5}, MaxR{1.}, StepR{0.1}));
    add_input_ui(m_p->vRoundedEnable.init_widget("Rounded", false));
    add_input_ui(m_p->vRounded.init_widget("", false));

    // AO
    add_input_ui(m_p->aoEnable.init_widget("Enable", true));
    add_input_ui(m_p->aoIntensityEnable.init_widget("Intensity", true));
    add_input_ui(m_p->aoIntensity.init_widget("", MinR{0.}, ValR{1.58}, MaxR{4.}, StepR{0.01}));
    add_input_ui(m_p->aoThicknessEnable.init_widget("Thickness", true));
    add_input_ui(m_p->aoThickness.init_widget("", MinR{1.}, ValR{1.96}, MaxR{10.}, StepR{0.01}));
    add_input_ui(m_p->aoColorEnable.init_widget("Color", true));
    add_input_ui(m_p->aoColor.init_widget("", Qt::black));
}


struct SkyConfigParametersW::Impl{

    QTabWidget *tabs = nullptr;

    // sun light
    QWidget *sunLightW = nullptr;
    QTabWidget *sunLightTab = nullptr;
    ExVector3dW sunRotation{"sun_rotation"};
    ExFloatSpinBoxW sunIntensity{"sun_intensity"};
    ExFloatSpinBoxW sunBounceIntensity{"sun_bounce_intensity"};
    ExSelectColorW sunColor{"sun_color"};
    ExComboBoxIndexW sunShadowMode{"sun_shadow_mode"};
    ExFloatSpinBoxW sunShadowStrength{"sun_shadow_strength"};
    ExFloatSpinBoxW sunShadowBias{"sun_shadow_bias"};
    ExFloatSpinBoxW sunShadowNormalBias{"sun_shadow_normal_bias"};
    ExFloatSpinBoxW sunShadowNearPlane{"sun_shadow_near_plane"};

    // ambient light
    QWidget *ambientLightW = nullptr;
    QTabWidget *ambientTab = nullptr;
    ExComboBoxIndexW ambientMode{"ambient_mode"};
    ExFloatSpinBoxW ambientIntensity{"ambient_intensity"};
    ExSelectColorW ambientFlatColor{"ambient_color"};
    ExSelectColorW ambientGradientGroundColor{"ground_color"};
    ExSelectColorW ambientGradientEquatorColor{"equator_color"};
    ExSelectColorW ambientGradientSkyColor{"sky_color"};

    // skybox
    QWidget *skyboxW = nullptr;
    QTabWidget *skyboxTab = nullptr;
    ExCheckBoxW zPlusInvert{"Z+_invert"};
    ExCheckBoxW zMinusInvert{"Z-_invert"};
    ExCheckBoxW xPlusInvert{"X+_invert"};
    ExCheckBoxW xMinusInvert{"X-_invert"};
    ExCheckBoxW yPlusInvert{"Y+_invert"};
    ExCheckBoxW yMinusInvert{"Y-_invert"};
    ExCheckBoxW panoramicInvert{"panoramic_invert"};
    ExCheckBoxW cubeMapInvert{"cubemap_invert"};

    ExResourceW xMinus{"X-"};
    ExResourceW yMinus{"Y-"};
    ExResourceW zMinus{"Z-"};
    ExResourceW xPlus{"X+"};
    ExResourceW yPlus{"Y+"};
    ExResourceW zPlus{"Z+"};
    ExResourceW panoramic{"panoramic"};
    ExResourceW cubemap{"cubemap"};

    ExComboBoxIndexW mode{"mode"};
    ExSelectColorW backgroundColor{"background_color"};

    ExSelectColorW sixSidedTintColor{"6sided-tint"};
    ExFloatSpinBoxW sixSidedExposure{"6sided-exposure"};
    ExFloatSpinBoxW sixSidedRotation{"6sided-rotation"};

    ExSelectColorW cubeMapTintColor{"cubemap-tint"};
    ExFloatSpinBoxW cubeMapExposure{"cubemap-exposure"};
    ExFloatSpinBoxW cubeMapRotation{"cubemap-rotation"};

    ExSelectColorW panoramicTintColor{"panoramic-tint"};
    ExFloatSpinBoxW panoramicExposure{"panoramic-exposure"};
    ExFloatSpinBoxW panoramicRotation{"panoramic-rotation"};
    ExComboBoxIndexW panoramicMapping{"panoramic-mapping"};
    ExCheckBoxW panoramicMirrorBack{"panoramic-mirror_back"};
    ExComboBoxIndexW panoramicType{"panoramic-type"};
    ExComboBoxIndexW panoramicLayout{"panoramic-layout"};

    ExComboBoxIndexW proceduralSun{"sun"};
    ExFloatSpinBoxW proceduralSunSize{"sun-size"};
    ExFloatSpinBoxW proceduralConvergence{"convergence"};
    ExFloatSpinBoxW proceduralAtmoshpereThickness{"atmosphere-thickness"};
    ExSelectColorW proceduralSkyTint{"procedural-sky-tint"};
    ExSelectColorW proceduralGroundColor{"procedural-ground-color"};
    ExFloatSpinBoxW proceduralExposure{"procedural-exposure"};
};



SkyConfigParametersW::SkyConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}



void SkyConfigParametersW::insert_widgets(){

    add_widget(m_p->tabs = new QTabWidget());

    // sun light
    auto sunLightL = ui::L::VB();
    sunLightL->setContentsMargins(4,2,4,2);
    sunLightL->setSpacing(4);
    (m_p->sunLightW = new QWidget())->setLayout(sunLightL);
    {
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->sunRotation(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Intensity:")),1},{m_p->sunIntensity(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Bound intensity:")),1},{m_p->sunBounceIntensity(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Color:")),1},{m_p->sunColor(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Shadow mode:")),1},{m_p->sunShadowMode(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Shadow strength:")),1},{m_p->sunShadowStrength(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Shadow bias:")),1},{m_p->sunShadowBias(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Shadow normal bias:")),1},{m_p->sunShadowNormalBias(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Shadow near plane:")),1},{m_p->sunShadowNearPlane(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addStretch();
    }

    // ambient light
    auto ambientLightL = ui::L::VB();
    ambientLightL->setContentsMargins(4,2,4,2);
    ambientLightL->setSpacing(4);
    (m_p->ambientLightW = new QWidget())->setLayout(ambientLightL);
    {
        ambientLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Mode:")),1},{m_p->ambientMode(),10}} , 0, LMarginsD{0,0,0,0,2}));
        ambientLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Skybox intensity:")),1},{m_p->ambientIntensity(),10}} , 0, LMarginsD{0,0,0,0,2}));
        ambientLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Flat color:")),1},{m_p->ambientFlatColor(),10}} , 0, LMarginsD{0,0,0,0,2}));
        ambientLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Gradient colors:")),1},
            {m_p->ambientGradientGroundColor(),10},{m_p->ambientGradientEquatorColor(),10},{m_p->ambientGradientSkyColor(),10}} , 0, LMarginsD{0,0,0,0,2}));
        ambientLightL->addStretch();
    }

    // skybox
    auto skyboxL = ui::L::VB();
    skyboxL->setContentsMargins(4,2,4,2);
    skyboxL->setSpacing(2);
    (m_p->skyboxW = new QWidget())->setLayout(skyboxL);
    {
        skyboxL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt(QSL("Background mode: ")), m_p->mode()},  LStretch{false}, LMargins{true}, QFrame::NoFrame));
        skyboxL->addWidget(m_p->skyboxTab = new QTabWidget());

        auto t1 = new QWidget();
        auto l1 = ui::L::VB();
        t1->setLayout(l1);
        m_p->skyboxTab->addTab(t1, QSL("Color"));
        l1->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt(QSL("Background color: ")), m_p->backgroundColor()},  LStretch{false}, LMargins{true}, QFrame::NoFrame));
        l1->addStretch();

        auto t2 = new QWidget();
        auto l2 = ui::L::VB();
        t2->setLayout(l2);
        m_p->skyboxTab->addTab(t2, QSL("6 sided"));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->zPlus(),10},  {m_p->zPlusInvert(),1}} , 0, LMarginsD{0,0,0,0,2}));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->zMinus(),10}, {m_p->zMinusInvert(),1}}, 0, LMarginsD{0,0,0,0,2}));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->xPlus(),10},  {m_p->xPlusInvert(),1}} , 0, LMarginsD{0,0,0,0,2}));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->xMinus(),10}, {m_p->xMinusInvert(),1}}, 0, LMarginsD{0,0,0,0,2}));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->yPlus(),10},  {m_p->yPlusInvert(),1}} , 0, LMarginsD{0,0,0,0,2}));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->yMinus(),10}, {m_p->yMinusInvert(),1}}, 0, LMarginsD{0,0,0,0,2}));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Tint:")),1},     {m_p->sixSidedTintColor(), 4}}, 0, LMarginsD{0,0,0,0,2}));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Exposure:")),1}, {m_p->sixSidedExposure(),  4}}, 0, LMarginsD{0,0,0,0,2}));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Rotation:")),1}, {m_p->sixSidedRotation(),  4}}, 0, LMarginsD{0,0,0,0,2}));
        l2->setContentsMargins(4,2,4,2);
        l2->setSpacing(2);
        l2->addStretch();

        auto t3 = new QWidget();
        auto l3 = ui::L::VB();
        t3->setLayout(l3);
        m_p->skyboxTab->addTab(t3, QSL("Cubemap"));
        l3->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->cubemap(),10},  {m_p->cubeMapInvert(),1}} , 0, LMarginsD{0,0,0,0,2}));
        l3->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Tint:")),1},       {m_p->cubeMapTintColor(), 4}}, 0, LMarginsD{0,0,0,0,2}));
        l3->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Exposure:")),1},   {m_p->cubeMapExposure(), 4}},  0, LMarginsD{0,0,0,0,2}));
        l3->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Rotation:")),1},   {m_p->cubeMapRotation(), 4}},  0, LMarginsD{0,0,0,0,2}));
        l3->setContentsMargins(4,2,4,2);
        l3->setSpacing(2);
        l3->addStretch();

        auto t4 = new QWidget();
        auto l4 = ui::L::VB();
        t4->setLayout(l4);
        m_p->skyboxTab->addTab(t4, QSL("Panoramic"));
        l4->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->panoramic(),10}, {m_p->panoramicInvert(),1}}, 0, LMarginsD{0,0,0,0,2}));
        l4->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Tint:")),1},           {m_p->panoramicTintColor(), 4}},   0, LMarginsD{0,0,0,0,2}));
        l4->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Exposure:")),1},       {m_p->panoramicExposure(), 4}},    0, LMarginsD{0,0,0,0,2}));
        l4->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Rotation:")),1},       {m_p->panoramicRotation(), 4}},    0, LMarginsD{0,0,0,0,2}));
        l4->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Mapping:")),1},        {m_p->panoramicMapping(), 4}},     0, LMarginsD{0,0,0,0,2}));
        l4->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_p->panoramicMirrorBack(),1}},                                      0, LMarginsD{0,0,0,0,2}));
        l4->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Type:")),1},           {m_p->panoramicType(), 4}},        0, LMarginsD{0,0,0,0,2}));
        l4->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Layout:")),1},         {m_p->panoramicLayout(), 4}},      0, LMarginsD{0,0,0,0,2}));
        l4->setContentsMargins(4,2,4,2);
        l4->setSpacing(2);
        l4->addStretch();

        auto t5 = new QWidget();
        auto l5 = ui::L::VB();
        t5->setLayout(l5);
        m_p->skyboxTab->addTab(t5, QSL("Procedural"));
        l5->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Sun:")),1},                    {m_p->proceduralSun(), 4}},                0, LMarginsD{0,0,0,0,2}));
        l5->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Sun size:")),1},               {m_p->proceduralSunSize(), 4}},            0, LMarginsD{0,0,0,0,2}));
        l5->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Sun convergence:")),1},        {m_p->proceduralConvergence(), 4}},        0, LMarginsD{0,0,0,0,2}));
        l5->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Atmosphere thickness:")),1},   {m_p->proceduralAtmoshpereThickness(), 4}},0, LMarginsD{0,0,0,0,2}));
        l5->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Sky tint:")),1},               {m_p->proceduralSkyTint(), 4}},            0, LMarginsD{0,0,0,0,2}));
        l5->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Ground color:")),1},           {m_p->proceduralGroundColor(), 4}},        0, LMarginsD{0,0,0,0,2}));
        l5->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Exposure:")),1},               {m_p->proceduralExposure(), 4}},           0, LMarginsD{0,0,0,0,2}));
        l5->setContentsMargins(4,2,4,2);
        l5->setSpacing(2);
        l5->addStretch();
    }

    m_p->tabs->addTab(m_p->skyboxW, QSL("Skybox"));
    m_p->tabs->addTab(m_p->sunLightW, QSL("Sun light"));
    m_p->tabs->addTab(m_p->ambientLightW, QSL("Ambient light"));
}

void SkyConfigParametersW::init_and_register_widgets(){

    // sun light
    add_input_ui(m_p->sunRotation.init_widget("Rotation:", Vector3dSettings{{MinV<qreal>{-360.}, V<qreal>{0.}, MaxV<qreal>{360.}, StepV<qreal>{0.1}, 2}}));
    add_input_ui(m_p->sunIntensity.init_widget(MinV<qreal>{0.}, V<qreal>{0.5}, MaxV<qreal>{100.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_p->sunBounceIntensity.init_widget(MinV<qreal>{0.}, V<qreal>{1.}, MaxV<qreal>{100.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_p->sunColor.init_widget("Choose color", QColor(255,255,255,255)));
    add_input_ui(m_p->sunShadowMode.init_widget({"None","Hard","Soft"}, 2));
    add_input_ui(m_p->sunShadowStrength.init_widget(MinV<qreal>{0.}, V<qreal>{1.}, MaxV<qreal>{1.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_p->sunShadowBias.init_widget(MinV<qreal>{0.}, V<qreal>{0.05}, MaxV<qreal>{2.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_p->sunShadowNormalBias.init_widget(MinV<qreal>{0.}, V<qreal>{0.4}, MaxV<qreal>{3.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_p->sunShadowNearPlane.init_widget(MinV<qreal>{0.}, V<qreal>{0.2}, MaxV<qreal>{10.}, StepV<qreal>{0.05}, 2));

    // ambient light
    add_input_ui(m_p->ambientIntensity.init_widget(MinV<qreal>{0.}, V<qreal>{1.0}, MaxV<qreal>{100.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_p->ambientMode.init_widget({"Skybox","Gradient","Flat"}, 0));
    add_input_ui(m_p->ambientFlatColor.init_widget("Choose flat light color", QColor(54,58,66,255)));
    add_input_ui(m_p->ambientGradientGroundColor.init_widget("Choose gradient ground light color", QColor(11,12,9,255)));
    add_input_ui(m_p->ambientGradientEquatorColor.init_widget("Choose gradient equator light color", QColor(29,32,34,255)));
    add_input_ui(m_p->ambientGradientSkyColor.init_widget("Choose gradient sky light color", QColor(54,58,66,255)));

    // skybox
    add_input_ui(m_p->panoramic.init_widget(Resource::Type::Image, QSL("Texture")));
    add_input_ui(m_p->panoramicInvert.init_widget(QSL("invert"), false));
    add_input_ui(m_p->cubemap.init_widget(Resource::Type::Image, QSL("Texture")));
    add_input_ui(m_p->cubeMapInvert.init_widget(QSL("invert"), false));

    add_input_ui(m_p->xMinus.init_widget(Resource::Type::Image, QSL("X- (Right)")));
    add_input_ui(m_p->yMinus.init_widget(Resource::Type::Image, QSL("Y- (Down)")));
    add_input_ui(m_p->zMinus.init_widget(Resource::Type::Image, QSL("Z- (Back)")));
    add_input_ui(m_p->xPlus.init_widget(Resource::Type::Image, QSL("X+ (Left)")));
    add_input_ui(m_p->yPlus.init_widget(Resource::Type::Image, QSL("Y+ (Up)")));
    add_input_ui(m_p->zPlus.init_widget(Resource::Type::Image, QSL("Z+ (Front)")));
    add_input_ui(m_p->xMinusInvert.init_widget(QSL("invert"), false));
    add_input_ui(m_p->yMinusInvert.init_widget(QSL("invert"), false));
    add_input_ui(m_p->zMinusInvert.init_widget(QSL("invert"), false));
    add_input_ui(m_p->xPlusInvert.init_widget(QSL("invert"), false));
    add_input_ui(m_p->yPlusInvert.init_widget(QSL("invert"), false));
    add_input_ui(m_p->zPlusInvert.init_widget(QSL("invert"), false));

    add_input_ui(m_p->mode.init_widget({"Color background", "6 sided skybox", "cubemap skybox", "panoramic skybox", "procedural skybox"}));

    add_input_ui(m_p->backgroundColor.init_widget("Choose background color", QColor(53,48,48)));

    add_input_ui(m_p->sixSidedTintColor.init_widget("Choose tint", QColor(127,127,127,127)));
    add_input_ui(m_p->sixSidedExposure.init_widget(MinV<qreal>{0.}, V<qreal>{1.}, MaxV<qreal>{8.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_p->sixSidedRotation.init_widget(MinV<qreal>{0.}, V<qreal>{0.}, MaxV<qreal>{360.}, StepV<qreal>{0.05}, 2));

    add_input_ui(m_p->cubeMapTintColor.init_widget("Choose tint", QColor(127,127,127,127)));
    add_input_ui(m_p->cubeMapExposure.init_widget(MinV<qreal>{0.}, V<qreal>{1.}, MaxV<qreal>{8.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_p->cubeMapRotation.init_widget(MinV<qreal>{0.}, V<qreal>{0.}, MaxV<qreal>{360.}, StepV<qreal>{0.05}, 2));

    add_input_ui(m_p->panoramicTintColor.init_widget("Choose tint", QColor(127,127,127,127)));
    add_input_ui(m_p->panoramicExposure.init_widget(MinV<qreal>{0.}, V<qreal>{1.}, MaxV<qreal>{8.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_p->panoramicRotation.init_widget(MinV<qreal>{0.}, V<qreal>{0.}, MaxV<qreal>{360.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_p->panoramicMapping.init_widget({"6 frames layout", "Latitude longitude layout"}));
    add_input_ui(m_p->panoramicMirrorBack.init_widget("Mirror back", false));
    add_input_ui(m_p->panoramicType.init_widget({"360°", "180°"}));
    add_input_ui(m_p->panoramicLayout.init_widget({"None", "Side by side", "Over under"}));

    add_input_ui(m_p->proceduralSun.init_widget({"None", "Simple", "High quality"}, 2));
    add_input_ui(m_p->proceduralSunSize.init_widget(MinV<qreal>{0.}, V<qreal>{0.04}, MaxV<qreal>{1.}, StepV<qreal>{0.01}, 2));
    add_input_ui(m_p->proceduralConvergence.init_widget(MinV<qreal>{1.}, V<qreal>{5.}, MaxV<qreal>{10.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_p->proceduralAtmoshpereThickness.init_widget(MinV<qreal>{0.}, V<qreal>{1.}, MaxV<qreal>{5.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_p->proceduralSkyTint.init_widget("Choose sky tint", QColor(127,127,127,255)));
    add_input_ui(m_p->proceduralGroundColor.init_widget("Choose ground color", QColor(95,95,95,255)));
    add_input_ui(m_p->proceduralExposure.init_widget(MinV<qreal>{0.}, V<qreal>{1.3}, MaxV<qreal>{8.}, StepV<qreal>{0.05}, 2));
}


