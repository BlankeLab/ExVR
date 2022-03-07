
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
#include "ex_widgets/ex_checkbox_w.hpp"

// local
#include "ex_widgets/ex_resource_w.hpp"
#include "config_pw.hpp"

namespace tool::ex {


class VideoFileCameraInitConfigParametersW : public ConfigParametersW{

public :

    EyeRendererSubPart m_eye;
    ExResourceW m_video{"video"};

    void insert_widgets() override{
        add_sub_part_widget(m_eye);
        add_widget(ui::F::gen(ui::L::HB(), {m_video()}, LStretch{false}, LMargins{true}, QFrame::Box));
    }

    void init_and_register_widgets() override{
        map_sub_part(m_eye.init_widget());
        add_input_ui(m_video.init_widget(Resource::Type::Video, "Video resource: "));
    }

    void create_connections() override{}    
    void late_update_ui() override{}
};


class VideoFileCameraConfigParametersW : public ConfigParametersW{

public :

    WordSpaceCameraCanvasSubPart m_cameraSettings;
    ExCheckBoxW m_cbUseVideoSize{"use_original_size"};

    void insert_widgets() override{
        layout()->setContentsMargins(0,0,0,0);
        add_sub_part_widget(m_cameraSettings);
        add_widget(ui::F::gen(ui::L::HB(), {m_cbUseVideoSize()}, LStretch{true}, LMargins{true}, QFrame::Box));
    }

    void init_and_register_widgets() override{
        map_sub_part(m_cameraSettings.init_widget());
        add_input_ui(m_cbUseVideoSize.init_widget("Use video original size", true, true));
    }

    void create_connections() override{
        connect(m_cbUseVideoSize(), &QCheckBox::toggled, this, [&](bool checked){
            m_cameraSettings.set_wh_enable_state(!checked,!checked);
        });

    }

    void late_update_ui() override{
        bool state = !m_cbUseVideoSize()->isChecked();
        m_cameraSettings.set_wh_enable_state(state,state);
    }
};

}
