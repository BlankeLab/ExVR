
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

#include "camera_trajectory_file_pw.hpp"


// qt-utility
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_float_spin_box_w.hpp"
#include "gui/ex_widgets/ex_select_color_w.hpp"
#include "gui/ex_widgets/ex_curve_w.hpp"

// local
#include "gui/ex_widgets/ex_resource_w.hpp"


using namespace tool::ex;

struct CameraTrajectoryFileConfigParametersW::Impl{
    ExFloatSpinBoxW duration{"duration"};
    ExCheckBoxW useSphericalInterpolation{"spherical_interpolation"};
    ExCheckBoxW inverseOrder{"inverse"};

    ExResourceW trajectoryFile{"trajectory_file"};
    ExSelectColorW startColor{"start_color"};
    ExSelectColorW endColor{"end_color"};
    ExCurveW speed{"speed"};
};

CameraTrajectoryFileConfigParametersW::CameraTrajectoryFileConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}


void tool::ex::CameraTrajectoryFileConfigParametersW::insert_widgets(){

    add_widget(ui::F::gen(ui::L::HB(), {m_p->trajectoryFile()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Trajectory duration: "), m_p->duration()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Start color: "), m_p->startColor(), ui::W::txt("End color: "), m_p->endColor()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {m_p->useSphericalInterpolation(), m_p->inverseOrder()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    add_widget(m_p->speed());
}

void tool::ex::CameraTrajectoryFileConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_p->trajectoryFile.init_widget(Resource::Type::Text, "Trajectory file"));
    add_input_ui(m_p->duration.init_widget(MinV<qreal>{0.}, V<qreal>{3.},MaxV<qreal>{1000.}, StepV<qreal>{0.1}, 2));
    add_input_ui(m_p->startColor.init_widget("Choose start color", Qt::red));
    add_input_ui(m_p->endColor.init_widget("Choose end color", Qt::blue));
    add_input_ui(m_p->useSphericalInterpolation.init_widget("Spherical interpolation", true));
    add_input_ui(m_p->inverseOrder.init_widget("Inverse order", false));
    add_input_ui(m_p->speed.init_widget("Trajectory speed"));
}
