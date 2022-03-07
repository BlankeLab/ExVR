
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
#include "ex_widgets/ex_vector2d_w.hpp"
#include "ex_widgets/ex_spin_box_w.hpp"
#include "ex_widgets/ex_checkbox_w.hpp"

// local
#include "config_pw.hpp"

namespace tool::ex {

class WebcamViewerInitConfigParametersW : public ConfigParametersW{

public :

    ExSpinBoxW m_sbDeviceId{"device_id"};
    ExSpinBoxW m_sbFPS{"requested_fps"};

    void insert_widgets() override;
    void init_and_register_widgets() override;
    void create_connections() override{}
    void late_update_ui() override{}
};

class WebcamViewerConfigParametersW : public ConfigParametersW{

public :

    EyeRendererSubPart m_eye;

    WordSpaceCanvasSubPart m_cameraSettings;
    ExCheckBoxW m_followEyeCamera{"use_eye_camera"};
    ExCheckBoxW m_cbUseVideoSize{"use_original_size"};
    ExVector2dW m_pivot{"pivot"};
    ExFloatSpinBoxW m_distance{"distance"};

    void insert_widgets() override;
    void init_and_register_widgets() override;
    void create_connections() override;
    void late_update_ui() override;
};

}
