
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

#include "sky_pw.hpp"

using namespace tool::ex;


void SkyConfigParametersW::insert_widgets(){

    add_widget(m_tabs = new QTabWidget());

    // sun light
    auto sunLightL = ui::L::VB();
    sunLightL->setContentsMargins(4,2,4,2);
    sunLightL->setSpacing(4);
    (m_sunLightW = new QWidget())->setLayout(sunLightL);
    {
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_sunRotation(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Intensity:")),1},{m_sunIntensity(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Bound intensity:")),1},{m_sunBounceIntensity(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Color:")),1},{m_sunColor(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Shadow mode:")),1},{m_sunShadowMode(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Shadow strength:")),1},{m_sunShadowStrength(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Shadow bias:")),1},{m_sunShadowBias(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Shadow normal bias:")),1},{m_sunShadowNormalBias(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Shadow near plane:")),1},{m_sunShadowNearPlane(),10}} , 0, LMarginsD{0,0,0,0,2}));
        sunLightL->addStretch();
    }

    // ambient light
    auto ambientLightL = ui::L::VB();
    ambientLightL->setContentsMargins(4,2,4,2);
    ambientLightL->setSpacing(4);
    (m_ambientLightW = new QWidget())->setLayout(ambientLightL);
    {
        ambientLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Mode:")),1},{m_ambientMode(),10}} , 0, LMarginsD{0,0,0,0,2}));
        ambientLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Skybox intensity:")),1},{m_ambientIntensity(),10}} , 0, LMarginsD{0,0,0,0,2}));
        ambientLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Flat color:")),1},{m_ambientFlatColor(),10}} , 0, LMarginsD{0,0,0,0,2}));
        ambientLightL->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Gradient colors:")),1},
            {m_ambientGradientGroundColor(),10},{m_ambientGradientEquatorColor(),10},{m_ambientGradientSkyColor(),10}} , 0, LMarginsD{0,0,0,0,2}));
        ambientLightL->addStretch();
    }

    // skybox
    auto skyboxL = ui::L::VB();
    skyboxL->setContentsMargins(4,2,4,2);
    skyboxL->setSpacing(2);
    (m_skyboxW = new QWidget())->setLayout(skyboxL);
    {
        skyboxL->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt(QSL("Background mode: ")), m_mode()},  LStretch{false}, LMargins{true}, QFrame::NoFrame));
        skyboxL->addWidget(m_skyboxTab = new QTabWidget());

        auto t1 = new QWidget();
        auto l1 = ui::L::VB();
        t1->setLayout(l1);
        m_skyboxTab->addTab(t1, QSL("Color"));
        l1->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt(QSL("Background color: ")), m_backgroundColor()},  LStretch{false}, LMargins{true}, QFrame::NoFrame));
        l1->addStretch();

        auto t2 = new QWidget();
        auto l2 = ui::L::VB();
        t2->setLayout(l2);
        m_skyboxTab->addTab(t2, QSL("6 sided"));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_zPlus(),10},  {m_zPlusInvert(),1}} , 0, LMarginsD{0,0,0,0,2}));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_zMinus(),10}, {m_zMinusInvert(),1}}, 0, LMarginsD{0,0,0,0,2}));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_xPlus(),10},  {m_xPlusInvert(),1}} , 0, LMarginsD{0,0,0,0,2}));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_xMinus(),10}, {m_xMinusInvert(),1}}, 0, LMarginsD{0,0,0,0,2}));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_yPlus(),10},  {m_yPlusInvert(),1}} , 0, LMarginsD{0,0,0,0,2}));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_yMinus(),10}, {m_yMinusInvert(),1}}, 0, LMarginsD{0,0,0,0,2}));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Tint:")),1},     {m_6SidedTintColor(), 4}}, 0, LMarginsD{0,0,0,0,2}));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Exposure:")),1}, {m_6sidedExposure(),  4}}, 0, LMarginsD{0,0,0,0,2}));
        l2->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Rotation:")),1}, {m_6sidedRotation(),  4}}, 0, LMarginsD{0,0,0,0,2}));
        l2->setContentsMargins(4,2,4,2);
        l2->setSpacing(2);
        l2->addStretch();

        auto t3 = new QWidget();
        auto l3 = ui::L::VB();
        t3->setLayout(l3);
        m_skyboxTab->addTab(t3, QSL("Cubemap"));
        l3->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_cubemap(),10},  {m_cubeMapInvert(),1}} , 0, LMarginsD{0,0,0,0,2}));
        l3->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Tint:")),1},       {m_cubeMapTintColor(), 4}}, 0, LMarginsD{0,0,0,0,2}));
        l3->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Exposure:")),1},   {m_cubeMapExposure(), 4}},  0, LMarginsD{0,0,0,0,2}));
        l3->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Rotation:")),1},   {m_cubeMapRotation(), 4}},  0, LMarginsD{0,0,0,0,2}));
        l3->setContentsMargins(4,2,4,2);
        l3->setSpacing(2);
        l3->addStretch();

        auto t4 = new QWidget();
        auto l4 = ui::L::VB();
        t4->setLayout(l4);
        m_skyboxTab->addTab(t4, QSL("Panoramic"));
        l4->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_panoramic(),10}, {m_panoramicInvert(),1}}, 0, LMarginsD{0,0,0,0,2}));
        l4->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Tint:")),1},           {m_panoramicTintColor(), 4}},   0, LMarginsD{0,0,0,0,2}));
        l4->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Exposure:")),1},       {m_panoramicExposure(), 4}},    0, LMarginsD{0,0,0,0,2}));
        l4->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Rotation:")),1},       {m_panoramicRotation(), 4}},    0, LMarginsD{0,0,0,0,2}));
        l4->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Mapping:")),1},        {m_panoramicMapping(), 4}},     0, LMarginsD{0,0,0,0,2}));
        l4->addWidget(ui::F::gen_frame(ui::L::HB(), {{m_panoramicMirrorBack(),1}},                                      0, LMarginsD{0,0,0,0,2}));
        l4->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Type:")),1},           {m_panoramicType(), 4}},        0, LMarginsD{0,0,0,0,2}));
        l4->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Layout:")),1},         {m_panoramicLayout(), 4}},      0, LMarginsD{0,0,0,0,2}));
        l4->setContentsMargins(4,2,4,2);
        l4->setSpacing(2);
        l4->addStretch();

        auto t5 = new QWidget();
        auto l5 = ui::L::VB();
        t5->setLayout(l5);
        m_skyboxTab->addTab(t5, QSL("Procedural"));
        l5->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Sun:")),1},                    {m_proceduralSun(), 4}},                0, LMarginsD{0,0,0,0,2}));
        l5->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Sun size:")),1},               {m_proceduralSunSize(), 4}},            0, LMarginsD{0,0,0,0,2}));
        l5->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Sun convergence:")),1},        {m_proceduralConvergence(), 4}},        0, LMarginsD{0,0,0,0,2}));
        l5->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Atmosphere thickness:")),1},   {m_proceduralAtmoshpereThickness(), 4}},0, LMarginsD{0,0,0,0,2}));
        l5->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Sky tint:")),1},               {m_proceduralSkyTint(), 4}},            0, LMarginsD{0,0,0,0,2}));
        l5->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Ground color:")),1},           {m_proceduralGroundColor(), 4}},        0, LMarginsD{0,0,0,0,2}));
        l5->addWidget(ui::F::gen_frame(ui::L::HB(), {{ui::W::txt(QSL("Exposure:")),1},               {m_proceduralExposure(), 4}},           0, LMarginsD{0,0,0,0,2}));
        l5->setContentsMargins(4,2,4,2);
        l5->setSpacing(2);
        l5->addStretch();
    }

    m_tabs->addTab(m_skyboxW, QSL("Skybox"));
    m_tabs->addTab(m_sunLightW, QSL("Sun light"));
    m_tabs->addTab(m_ambientLightW, QSL("Ambient light"));
}

void SkyConfigParametersW::init_and_register_widgets(){

    // sun light
    add_input_ui(m_sunRotation.init_widget("Rotation:", Vector3dSettings{{MinV<qreal>{-360.}, V<qreal>{0.}, MaxV<qreal>{360.}, StepV<qreal>{0.1}, 2}}));
    add_input_ui(m_sunIntensity.init_widget(MinV<qreal>{0.}, V<qreal>{0.5}, MaxV<qreal>{100.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_sunBounceIntensity.init_widget(MinV<qreal>{0.}, V<qreal>{1.}, MaxV<qreal>{100.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_sunColor.init_widget("Choose color", QColor(255,255,255,255)));
    add_input_ui(m_sunShadowMode.init_widget({"None","Hard","Soft"}, 2));
    add_input_ui(m_sunShadowStrength.init_widget(MinV<qreal>{0.}, V<qreal>{1.}, MaxV<qreal>{1.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_sunShadowBias.init_widget(MinV<qreal>{0.}, V<qreal>{0.05}, MaxV<qreal>{2.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_sunShadowNormalBias.init_widget(MinV<qreal>{0.}, V<qreal>{0.4}, MaxV<qreal>{3.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_sunShadowNearPlane.init_widget(MinV<qreal>{0.}, V<qreal>{0.2}, MaxV<qreal>{10.}, StepV<qreal>{0.05}, 2));

    // ambient light
    add_input_ui(m_ambientIntensity.init_widget(MinV<qreal>{0.}, V<qreal>{1.0}, MaxV<qreal>{100.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_ambientMode.init_widget({"Skybox","Gradient","Flat"}, 0));
    add_input_ui(m_ambientFlatColor.init_widget("Choose flat light color", QColor(54,58,66,255)));
    add_input_ui(m_ambientGradientGroundColor.init_widget("Choose gradient ground light color", QColor(11,12,9,255)));
    add_input_ui(m_ambientGradientEquatorColor.init_widget("Choose gradient equator light color", QColor(29,32,34,255)));
    add_input_ui(m_ambientGradientSkyColor.init_widget("Choose gradient sky light color", QColor(54,58,66,255)));

    // skybox
    add_input_ui(m_panoramic.init_widget(Resource::Type::Image, QSL("Texture")));
    add_input_ui(m_panoramicInvert.init_widget(QSL("invert"), false));
    add_input_ui(m_cubemap.init_widget(Resource::Type::Image, QSL("Texture")));
    add_input_ui(m_cubeMapInvert.init_widget(QSL("invert"), false));

    add_input_ui(m_xMinus.init_widget(Resource::Type::Image, QSL("X- (Right)")));
    add_input_ui(m_yMinus.init_widget(Resource::Type::Image, QSL("Y- (Down)")));
    add_input_ui(m_zMinus.init_widget(Resource::Type::Image, QSL("Z- (Back)")));
    add_input_ui(m_xPlus.init_widget(Resource::Type::Image, QSL("X+ (Left)")));
    add_input_ui(m_yPlus.init_widget(Resource::Type::Image, QSL("Y+ (Up)")));
    add_input_ui(m_zPlus.init_widget(Resource::Type::Image, QSL("Z+ (Front)")));
    add_input_ui(m_xMinusInvert.init_widget(QSL("invert"), false));
    add_input_ui(m_yMinusInvert.init_widget(QSL("invert"), false));
    add_input_ui(m_zMinusInvert.init_widget(QSL("invert"), false));
    add_input_ui(m_xPlusInvert.init_widget(QSL("invert"), false));
    add_input_ui(m_yPlusInvert.init_widget(QSL("invert"), false));
    add_input_ui(m_zPlusInvert.init_widget(QSL("invert"), false));

    add_input_ui(m_mode.init_widget({"Color background", "6 sided skybox", "cubemap skybox", "panoramic skybox", "procedural skybox"}));

    add_input_ui(m_backgroundColor.init_widget("Choose background color", QColor(53,48,48)));

    add_input_ui(m_6SidedTintColor.init_widget("Choose tint", QColor(127,127,127,127)));
    add_input_ui(m_6sidedExposure.init_widget(MinV<qreal>{0.}, V<qreal>{1.}, MaxV<qreal>{8.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_6sidedRotation.init_widget(MinV<qreal>{0.}, V<qreal>{0.}, MaxV<qreal>{360.}, StepV<qreal>{0.05}, 2));

    add_input_ui(m_cubeMapTintColor.init_widget("Choose tint", QColor(127,127,127,127)));
    add_input_ui(m_cubeMapExposure.init_widget(MinV<qreal>{0.}, V<qreal>{1.}, MaxV<qreal>{8.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_cubeMapRotation.init_widget(MinV<qreal>{0.}, V<qreal>{0.}, MaxV<qreal>{360.}, StepV<qreal>{0.05}, 2));

    add_input_ui(m_panoramicTintColor.init_widget("Choose tint", QColor(127,127,127,127)));
    add_input_ui(m_panoramicExposure.init_widget(MinV<qreal>{0.}, V<qreal>{1.}, MaxV<qreal>{8.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_panoramicRotation.init_widget(MinV<qreal>{0.}, V<qreal>{0.}, MaxV<qreal>{360.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_panoramicMapping.init_widget({"6 frames layout", "Latitude longitude layout"}));
    add_input_ui(m_panoramicMirrorBack.init_widget("Mirror back", false));
    add_input_ui(m_panoramicType.init_widget({"360°", "180°"}));
    add_input_ui(m_panoramicLayout.init_widget({"None", "Side by side", "Over under"}));

    add_input_ui(m_proceduralSun.init_widget({"None", "Simple", "High quality"}, 2));
    add_input_ui(m_proceduralSunSize.init_widget(MinV<qreal>{0.}, V<qreal>{0.04}, MaxV<qreal>{1.}, StepV<qreal>{0.01}, 2));
    add_input_ui(m_proceduralConvergence.init_widget(MinV<qreal>{1.}, V<qreal>{5.}, MaxV<qreal>{10.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_proceduralAtmoshpereThickness.init_widget(MinV<qreal>{0.}, V<qreal>{1.}, MaxV<qreal>{5.}, StepV<qreal>{0.05}, 2));
    add_input_ui(m_proceduralSkyTint.init_widget("Choose sky tint", QColor(127,127,127,255)));
    add_input_ui(m_proceduralGroundColor.init_widget("Choose ground color", QColor(95,95,95,255)));
    add_input_ui(m_proceduralExposure.init_widget(MinV<qreal>{0.}, V<qreal>{1.3}, MaxV<qreal>{8.}, StepV<qreal>{0.05}, 2));
}

void SkyConfigParametersW::create_connections(){

}

void SkyConfigParametersW::late_update_ui(){}
