
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
#include "gui/ex_widgets/ex_float_spin_box_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_line_edit_w.hpp"

// local
#include "config_pw.hpp"

namespace tool::ex{


class TargetToGrabInitConfigParametersW : public ConfigParametersW{

public :

    ExLineEditW m_leTargetComponentName{"shape_component"};
    ExFloatSpinBoxW m_dsbMaxReachableDistance{"max_distance_reachable"};
    ExFloatSpinBoxW m_dsbStartLineDistance{"start_distance"};

    void insert_widgets() override{
        add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Target component name: "),       m_leTargetComponentName(), ui::W::txt("(target must be before in condition order)")}, LStretch{true}, LMargins{false},QFrame::NoFrame));
        add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Max reachable distance (m): "),  m_dsbMaxReachableDistance()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
        add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Start line distance (m): "),     m_dsbStartLineDistance()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
    }

    void init_and_register_widgets() override{
        add_input_ui(m_leTargetComponentName.init_widget("..."));
        add_input_ui(m_dsbMaxReachableDistance.init_widget(MinV<qreal>{0.1}, V<qreal>{0.5}, MaxV<qreal>{1.3}, StepV<qreal>{0.01}, 2));
        add_input_ui(m_dsbStartLineDistance.init_widget(MinV<qreal>{-1.0}, V<qreal>{0.1}, MaxV<qreal>{1.3}, StepV<qreal>{0.01}, 2));
    }

    void create_connections() override{}
    void late_update_ui() override{}
};


class TargetToGrabConfigParametersW : public ConfigParametersW{

public :

    // general
    ExCheckBoxW m_cbDebug{"debug"};
    ExCheckBoxW m_cbDisplayStartLine{"display_start_line"};

    // working zone
    TransformSubPart m_trWorkingZone{"working_transform"};

    // target
    ExFloatSpinBoxW m_dsbDistancePercentageTarget{"distance_percentage_target"};
    ExFloatSpinBoxW m_dsbHeightTarget{"height_target"};
    ExFloatSpinBoxW m_dsbAngleTarget{"angle_target"};


    void insert_widgets() override{

        // general
        add_widget(ui::F::gen(ui::L::VB(),{new QLabel("<b>General</b>"), m_cbDebug(), m_cbDisplayStartLine()}, LStretch{true}, LMargins{true},QFrame::Box));

        // target
        auto targetDistance = ui::F::gen(ui::L::HB(),{ui::W::txt("Distance (%): "),m_dsbDistancePercentageTarget()}, LStretch{false}, LMargins{false},QFrame::NoFrame);
        auto targetAngle    = ui::F::gen(ui::L::HB(),{ui::W::txt("Angle (°): "),m_dsbAngleTarget()}, LStretch{false}, LMargins{false},QFrame::NoFrame);
        auto targetHeight   = ui::F::gen(ui::L::HB(),{ui::W::txt("Height (m): "),m_dsbHeightTarget()}, LStretch{false}, LMargins{false},QFrame::NoFrame);
        add_widget(ui::F::gen(ui::L::VB(),{new QLabel("<b>Target: </b>"), targetDistance, targetAngle, targetHeight}, LStretch{true}, LMargins{true},QFrame::Box));
        add_sub_part_widget(m_trWorkingZone);
    }

    void init_and_register_widgets() override{

        // general
        add_input_ui(m_cbDebug.init_widget("Debug", false));
        add_input_ui(m_cbDisplayStartLine.init_widget("Display start line", true));

        // working zone
        map_sub_part(m_trWorkingZone.init_widget("Working zone transform"));
        m_trWorkingZone.set_rotation_values({0.,0.,0.});
        m_trWorkingZone.set_trans_decimals(3);
        m_trWorkingZone.set_trans_steps({0.001,0.001,0.001});

        // target        
        add_input_ui(m_dsbDistancePercentageTarget.init_widget(MinV<qreal>{10.}, V<qreal>{80.}, MaxV<qreal>{100.}, StepV<qreal>{0.1}, 1));
        add_input_ui(m_dsbHeightTarget.init_widget(MinV<qreal>{-1.}, V<qreal>{0.}, MaxV<qreal>{1.}, StepV<qreal>{0.01}, 2.));
        add_input_ui(m_dsbAngleTarget.init_widget(MinV<qreal>{-50.}, V<qreal>{0.0}, MaxV<qreal>{50.}, StepV<qreal>{1.}, 1));
    }

    void create_connections() override{}
    void late_update_ui() override{}
};
}
