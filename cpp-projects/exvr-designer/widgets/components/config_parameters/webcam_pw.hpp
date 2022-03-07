
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
#include "ex_widgets/ex_spin_box_w.hpp"

// local
#include "config_pw.hpp"

namespace tool::ex {

class WebcamInitConfigParametersW : public ConfigParametersW{

public :

    ExSpinBoxW m_sbDeviceId{"device_id"};
    ExSpinBoxW m_sbFPS{"requested_fps"};
    ExSpinBoxW m_sbWidth{"requested_width"};
    ExSpinBoxW m_sbHeight{"requested_height"};


    void insert_widgets() override{
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Device id: "), m_sbDeviceId()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Requested FPS:"), m_sbFPS()}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Requested width:"), m_sbWidth()}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Requested height:"), m_sbHeight()}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
    }

    void init_and_register_widgets() override{

        add_input_ui(m_sbDeviceId.init_widget(MinV<int>{0}, V<int>{0}, MaxV<int>{32}, StepV<int>{1},  true));
        add_input_ui(m_sbFPS.init_widget(MinV<int>{1}, V<int>{30}, MaxV<int>{120}, StepV<int>{1},  true));
        add_input_ui(m_sbWidth.init_widget(MinV<int>{1}, V<int>{1024}, MaxV<int>{6000}, StepV<int>{1},  true));
        add_input_ui(m_sbHeight.init_widget(MinV<int>{1}, V<int>{768}, MaxV<int>{6000}, StepV<int>{1},  true));
    }

    void create_connections() override{}
    void late_update_ui() override{}
};

class WebcamConfigParametersW : public ConfigParametersW{

public :

    void insert_widgets() override{
    }

    void init_and_register_widgets() override{
    }

    void create_connections() override{
    }

    void late_update_ui() override{
    }
};

}
