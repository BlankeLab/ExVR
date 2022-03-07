
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
#include "ex_widgets/ex_float_spin_box_w.hpp"

// local
#include "ex_widgets/ex_tab_w.hpp"
#include "ex_widgets/ex_camera_target_w.hpp"
#include "config_pw.hpp"

namespace tool::ex {

class CameraTrajectoryInitConfigParametersW : public ConfigParametersW{

public :

    void insert_widgets() override{}
    void init_and_register_widgets() override{}
    void create_connections() override{}
    void late_update_ui() override{}
};

class CameraTrajectoryConfigParametersW : public ConfigParametersW{

public :

    ExCheckBoxW cbAddCameraPosition{"add_camera_position"};
    ExCheckBoxW cbComputeTimesWithDistances{"compute_times"};
    ExFloatSpinBoxW dsbGlobalTime{"global_time"};
    ExTabW<ExCameraTargetW> cameraTrajectory{"camera_trajectory"};

    void insert_widgets() override{
        add_widget(cbAddCameraPosition());
        add_widget(ui::F::gen(ui::L::HB(), {cbComputeTimesWithDistances(), ui::W::txt("Global time: "), dsbGlobalTime(),}, LStretch{true}, LMargins{false}, QFrame::NoFrame));

        add_widget(cameraTrajectory());
        add_widget(ui::F::gen(ui::L::HB(), {&cameraTrajectory.pbAddTab, &cameraTrajectory.pbRemoveTab}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
        qobject_cast<QVBoxLayout*>(layout())->setStretchFactor(cameraTrajectory(), 10);

    }

    void init_and_register_widgets() override{

        ui::W::init(&cameraTrajectory.pbAddTab,    "Add tab", true);
        ui::W::init(&cameraTrajectory.pbRemoveTab, "Remove current tab", true);

        std::vector<std::any> parameters;
        parameters.emplace_back(std::make_any<bool>(true));
        add_input_ui(cbAddCameraPosition.init_widget("Add camera current position", true));
        add_input_ui(cbComputeTimesWithDistances.init_widget("Use global time and compute each sub time", true));
        add_input_ui(dsbGlobalTime.init_widget(MinV<qreal>{0.}, V<qreal>{3.},MaxV<qreal>{1000.}, StepV<qreal>{0.1}, 2));
        add_input_ui(cameraTrajectory.init_widget("P", parameters, QTabWidget::TabPosition::West, true));
    }

    void create_connections() override{

        connect(cbComputeTimesWithDistances(), &QCheckBox::clicked, this, [&](bool checked){
            dsbGlobalTime.w->setEnabled(checked);
            for(auto &w : cameraTrajectory.widgets){
                auto camP = dynamic_cast<ExCameraTargetW*>(w.get());
                camP->time()->setEnabled(!checked);
            }
        });

//        connect(&cameraTrajectory(), &QTabWidget::

//        connect(&cameraTrajectory, &ExTabW<ExCameraPositionW>::new_tab_signal, this, [&](int id){
//            auto camP = dynamic_cast<ExCameraPositionW*>(cameraTrajectory()idgets[id].get());
//            camP->time().setEnabled(!cbComputeTimesWithDistances().isChecked());
//        });


//        connect(&pbComputeTimes, &QPushButton::clicked, this, [&]{
//            double totalTime = 0.;
//            std_v1<geo::Pt3d> pts;
//            for(auto &w : cameraTrajectory()idgets){
//                auto camP = dynamic_cast<ExCameraPositionW*>(w.get());
//                totalTime += camP->time().value();
//                pts.emplace_back(geo::Pt3d{camP->translation.x().value(), camP->translation.y().value(), camP->translation.z().value()});
//            }
//            if(pts.size() < 2){
//                return;
//            }

//            double totalDist = 0.;
//            std_v1<double> dist;
//            for(size_t ii = 0; ii < pts.size()-1; ++ii){
//                double d = (pts[ii] - pts[ii+1]).norm();
//                totalDist += d;
//                dist.emplace_back(d);
//            }

//        });
    }

    void late_update_ui() override{

        cameraTrajectory.pbRemoveTab.setEnabled(cameraTrajectory()->count() > 0);
        dsbGlobalTime()->setEnabled(cbComputeTimesWithDistances()->isChecked());
        for(auto &w : cameraTrajectory.widgets){
            auto camP = dynamic_cast<ExCameraTargetW*>(w.get());
            camP->time()->setEnabled(!cbComputeTimesWithDistances()->isChecked());
        }
    }
};

}
