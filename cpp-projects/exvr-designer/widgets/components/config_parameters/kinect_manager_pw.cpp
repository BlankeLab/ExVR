
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

#include "kinect_manager_pw.hpp"

// qt-utility
#include "ex_widgets/ex_combo_box_index_w.hpp"
#include "ex_widgets/ex_checkbox_w.hpp"
#include "ex_widgets/ex_line_edit_w.hpp"
#include "ex_widgets/ex_spin_box_w.hpp"

// local
#include "ex_widgets/ex_resource_w.hpp"

using namespace tool::ex;

struct KinectManagerInitConfigParametersW::Impl{
    ExResourceW config{"config"};
    ExResourceW calib{"calib"};
    ExResourceW camera{"camera"};
    ExComboBoxIndexW mode{"mode"};
    ExLineEditW camarasToUse{"cameras_to_use"};
    ExCheckBoxW debugBypassDevice{"debug_bypass"};
};

KinectManagerInitConfigParametersW::KinectManagerInitConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void KinectManagerInitConfigParametersW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::VB(), {m_p->config(),m_p->calib(),m_p->camera()}, LStretch{false}, LMargins{true}, QFrame::Box));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Cameras mode:"), m_p->mode()}, LStretch{false}, LMargins{true}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Cameras id to use:"), m_p->camarasToUse()}, LStretch{false}, LMargins{true}, QFrame::NoFrame));
    add_widget(m_p->debugBypassDevice());
}

void KinectManagerInitConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_p->config.init_widget(Resource::Type::Text, "Network file: "));
    add_input_ui(m_p->calib.init_widget(Resource::Type::Text,  "Calibration file: "));
    add_input_ui(m_p->camera.init_widget(Resource::Type::Text, "Camera file: "));
    add_input_ui(m_p->mode.init_widget({"Cloud", "Mesh"}, 0));
    add_input_ui(m_p->camarasToUse.init_widget("0 1 2 3 4 5 6 7"));
    add_input_ui(m_p->debugBypassDevice.init_widget("Enable it for testing the experiment without the device", false));
}

struct KinectManagerConfigParametersW::Impl{
    ExSpinBoxW fps{"fps"};
    ExCheckBoxW updateFromCameras{"update_cameras"};
    ExSpinBoxW maxDiffTime{"max_diff_time"};
};

KinectManagerConfigParametersW::KinectManagerConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void KinectManagerConfigParametersW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::HB(),{m_p->updateFromCameras(), ui::W::txt("Camera ask frame rate: "), m_p->fps()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Max diff time(ms): "), m_p->maxDiffTime()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
}

void KinectManagerConfigParametersW::init_and_register_widgets(){
    add_input_ui(m_p->updateFromCameras.init_widget("Update cameras ", true));
    add_input_ui(m_p->fps.init_widget(MinV<int>{1}, V<int>{45}, MaxV<int>{90}, StepV<int>{1}));
    add_input_ui(m_p->maxDiffTime.init_widget(MinV<int>{1}, V<int>{100}, MaxV<int>{1000}, StepV<int>{1}));
}
