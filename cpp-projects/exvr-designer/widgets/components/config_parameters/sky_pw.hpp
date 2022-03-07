
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

#pragma once

// qt-utility
#include "ex_widgets/ex_vector3d_w.hpp"
#include "ex_widgets/ex_float_spin_box_w.hpp"
#include "ex_widgets/ex_select_color_w.hpp"
#include "ex_widgets/ex_combo_box_index_w.hpp"
#include "ex_widgets/ex_checkbox_w.hpp"

// local
#include "ex_widgets/ex_resource_w.hpp"
#include "config_pw.hpp"

namespace tool::ex {

class SkyInitConfigParametersW : public ConfigParametersW{

public :

    void insert_widgets() override{}
    void init_and_register_widgets() override{}
    void create_connections() override{}
    void late_update_ui() override{}

};

class SkyConfigParametersW : public ConfigParametersW{

public :

    QTabWidget *m_tabs = nullptr;

    // sun light
    QWidget *m_sunLightW = nullptr;
    QTabWidget *m_sunLightTab = nullptr;
    ExVector3dW m_sunRotation{"sun_rotation"};
    ExFloatSpinBoxW m_sunIntensity{"sun_intensity"};
    ExFloatSpinBoxW m_sunBounceIntensity{"sun_bounce_intensity"};
    ExSelectColorW m_sunColor{"sun_color"};
    ExComboBoxIndexW m_sunShadowMode{"sun_shadow_mode"};
    ExFloatSpinBoxW m_sunShadowStrength{"sun_shadow_strength"};
    ExFloatSpinBoxW m_sunShadowBias{"sun_shadow_bias"};
    ExFloatSpinBoxW m_sunShadowNormalBias{"sun_shadow_normal_bias"};
    ExFloatSpinBoxW m_sunShadowNearPlane{"sun_shadow_near_plane"};

    // ambient light
    QWidget *m_ambientLightW = nullptr;
    QTabWidget *m_ambientTab = nullptr;
    ExComboBoxIndexW m_ambientMode{"ambient_mode"};
    ExFloatSpinBoxW m_ambientIntensity{"ambient_intensity"};
    ExSelectColorW m_ambientFlatColor{"ambient_color"};
    ExSelectColorW m_ambientGradientGroundColor{"ground_color"};
    ExSelectColorW m_ambientGradientEquatorColor{"equator_color"};
    ExSelectColorW m_ambientGradientSkyColor{"sky_color"};

    // skybox
    QWidget *m_skyboxW = nullptr;
    QTabWidget *m_skyboxTab = nullptr;
    ExCheckBoxW m_zPlusInvert{"Z+_invert"};
    ExCheckBoxW m_zMinusInvert{"Z-_invert"};
    ExCheckBoxW m_xPlusInvert{"X+_invert"};
    ExCheckBoxW m_xMinusInvert{"X-_invert"};
    ExCheckBoxW m_yPlusInvert{"Y+_invert"};
    ExCheckBoxW m_yMinusInvert{"Y-_invert"};
    ExCheckBoxW m_panoramicInvert{"panoramic_invert"};
    ExCheckBoxW m_cubeMapInvert{"cubemap_invert"};

    ExResourceW m_xMinus{"X-"};
    ExResourceW m_yMinus{"Y-"};
    ExResourceW m_zMinus{"Z-"};
    ExResourceW m_xPlus{"X+"};
    ExResourceW m_yPlus{"Y+"};
    ExResourceW m_zPlus{"Z+"};
    ExResourceW m_panoramic{"panoramic"};
    ExResourceW m_cubemap{"cubemap"};

    ExComboBoxIndexW m_mode{"mode"};
    ExSelectColorW m_backgroundColor{"background_color"};

    ExSelectColorW m_6SidedTintColor{"6sided-tint"};
    ExFloatSpinBoxW m_6sidedExposure{"6sided-exposure"};
    ExFloatSpinBoxW m_6sidedRotation{"6sided-rotation"};

    ExSelectColorW m_cubeMapTintColor{"cubemap-tint"};
    ExFloatSpinBoxW m_cubeMapExposure{"cubemap-exposure"};
    ExFloatSpinBoxW m_cubeMapRotation{"cubemap-rotation"};

    ExSelectColorW m_panoramicTintColor{"panoramic-tint"};
    ExFloatSpinBoxW m_panoramicExposure{"panoramic-exposure"};
    ExFloatSpinBoxW m_panoramicRotation{"panoramic-rotation"};
    ExComboBoxIndexW m_panoramicMapping{"panoramic-mapping"};
    ExCheckBoxW m_panoramicMirrorBack{"panoramic-mirror_back"};
    ExComboBoxIndexW m_panoramicType{"panoramic-type"};
    ExComboBoxIndexW m_panoramicLayout{"panoramic-layout"};

    ExComboBoxIndexW m_proceduralSun{"sun"};
    ExFloatSpinBoxW m_proceduralSunSize{"sun-size"};
    ExFloatSpinBoxW m_proceduralConvergence{"convergence"};
    ExFloatSpinBoxW m_proceduralAtmoshpereThickness{"atmosphere-thickness"};
    ExSelectColorW m_proceduralSkyTint{"procedural-sky-tint"};
    ExSelectColorW m_proceduralGroundColor{"procedural-ground-color"};
    ExFloatSpinBoxW m_proceduralExposure{"procedural-exposure"};

    void insert_widgets() override;
    void init_and_register_widgets() override;
    void create_connections() override;
    void late_update_ui() override;
};

}
