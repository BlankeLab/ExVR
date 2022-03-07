
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
#include "ex_widgets/ex_float_spin_box_w.hpp"
#include "ex_widgets/ex_spin_box_w.hpp"
#include "ex_widgets/ex_checkbox_w.hpp"

// local
#include "ex_widgets/ex_resource_w.hpp"
#include "config_pw.hpp"

namespace tool::ex {

class ScanerVideoInitConfigParametersW : public ConfigParametersW{

public :

    ExResourceW m_scanerVideo{"scaner_video"};

    void insert_widgets() override{
        add_widget(ui::F::gen(ui::L::HB(), {m_scanerVideo()}, LStretch{false}, LMargins{true}, QFrame::Box));
    }

    void init_and_register_widgets() override{
        add_input_ui(m_scanerVideo.init_widget(Resource::Type::ScanerVideo, "Scaner video resource: "));
    }

    void create_connections() override{}
    void late_update_ui() override{}

    virtual void update_with_info(QStringView id, QStringView value) override{

//        if(id == "buttons_state_info"){
//            if(value.length() == 0){
//                for(const auto &t : input::Keyboard::mapping){
//                    if(buttonsDsb.count(std::get<0>(t)) > 0){
//                        std::get<1>(buttonsDsb[std::get<0>(t)])->update(false);
//                        std::get<2>(buttonsDsb[std::get<0>(t)])->update(false);
//                        std::get<3>(buttonsDsb[std::get<0>(t)])->update(false);
//                    }
//                }
//                return;
//            }else{
//                const auto split = value.split("%");
//                for(const auto &button : split){
//                    const auto subSplit = button.split(",");
//                    auto buttonId = static_cast<input::Keyboard::Button>(subSplit[0].toInt());
//                    if(buttonsDsb.count(buttonId)){
//                        std::get<1>(buttonsDsb[buttonId])->update(subSplit[1] == "1");
//                        std::get<2>(buttonsDsb[buttonId])->update(subSplit[2] == "1");
//                        std::get<3>(buttonsDsb[buttonId])->update(subSplit[3] == "1");
//                    }
//                }
//            }
//        }
    }

};

class ScanerVideoConfigParametersW : public ConfigParametersW{

public :

    TransformSubPart m_transfo{"global_transform"};
    ExFloatSpinBoxW m_sizePoints{"size_points"};
    ExSpinBoxW m_fps{"fps"};
    ExCheckBoxW m_loop{"loop"};
    ExCheckBoxW m_displayClouds{"display_clouds"};
    ExCheckBoxW m_displayColliders{"display_colliders"};
    ExSpinBoxW m_maxDiffTimeMs{"max_diff_time_ms"};

    void insert_widgets() override{
        add_sub_part_widget(m_transfo);
        add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Size points: "),m_sizePoints()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
        add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Video ask frame rate: "), m_fps()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
        add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Max time difference (ms): "), m_maxDiffTimeMs()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
        add_widget(ui::F::gen(ui::L::HB(),{m_loop()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
        add_widget(ui::F::gen(ui::L::HB(),{m_displayClouds()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
        add_widget(ui::F::gen(ui::L::HB(),{m_displayColliders()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
    }

    void init_and_register_widgets() override{
        map_sub_part(m_transfo.init_widget("Global model transform"));
        add_input_ui(m_sizePoints.init_widget(MinV<qreal>{0.0001}, V<qreal>{0.0030}, MaxV<qreal>{0.05}, StepV<qreal>{0.0001},4));
        add_input_ui(m_fps.init_widget(MinV<int>{1}, V<int>{30}, MaxV<int>{40}, StepV<int>{1}));
        add_input_ui(m_maxDiffTimeMs.init_widget(MinV<int>{1}, V<int>{100}, MaxV<int>{600}, StepV<int>{10}));
        add_input_ui(m_loop.init_widget("Loop ", true));
        add_input_ui(m_displayClouds.init_widget("Display clouds ", true));
        add_input_ui(m_displayColliders.init_widget("Display colliders ", false));
    }

    void create_connections() override{}
    void late_update_ui() override{}
};


}
