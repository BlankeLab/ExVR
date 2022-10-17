
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
#include "gui/ex_widgets/ex_radio_button_w.hpp"
#include "gui/ex_widgets/ex_vector3d_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"

// local
#include "config_pw.hpp"

namespace tool::ex {

class CameraInitConfigParametersW : public ConfigParametersW{

public :

    QButtonGroup group1;
    ExRadioButtonW useEye {"use_eye"};
    ExRadioButtonW useNeutral {"use_neutral"};
    ExCheckBoxW cbStartExperiment {"start_experiment"};
    ExVector3dW position {"position"};
    ExVector3dW rotation {"rotation"};

    void insert_widgets() override;
    void init_and_register_widgets() override;
    void create_connections() override;
    void late_update_ui() override;
};

class CameraConfigParametersW : public ConfigParametersW{

public :



    QButtonGroup group1;
    ExRadioButtonW useEye {"use_eye"};
    ExRadioButtonW useNeutral {"use_neutral"};
    ExCheckBoxW cbStartRoutine {"start_routine"};
    ExCheckBoxW cbUpdateOn {"update_on"};

    QPushButton *copyNeutralToCurrentConfig  = nullptr;
    QPushButton *copyEyeToCurrentConfig = nullptr;
    ExVector3dW position {"position"};
    ExVector3dW rotation {"rotation"};
    ExVector3dW currentNeutralPosition {"neutral_position"};
    ExVector3dW currentNeutralRotation {"neutral_rotation"};
    ExVector3dW currentEyePosition {"eye_rotation"};
    ExVector3dW currentEyeRotation {"eye_rotation"};

    ExCheckBoxW enableDebugMouseCameraMovements {"debug_camera"};
    ExFloatSpinBoxW mSpeed {"m_speed"};
    ExFloatSpinBoxW rxSpeed {"rx_speed"};
    ExFloatSpinBoxW rySpeed {"ry_speed"};
    ExFloatSpinBoxW rzSpeed {"rz_speed"};

    void insert_widgets() override;
    void init_and_register_widgets() override;
    void update_with_info(QStringView id, QStringView value) override;
    void create_connections() override;
    void late_update_ui() override;
};

}
