
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

// local
#include "ex_widgets/ex_resource_w.hpp"
#include "config_pw.hpp"

namespace tool::ex {

class FlagPoleInitConfigParametersW : public ConfigParametersW{

public :

    TransformSubPart m_transfo{"init_transform"};
    ExFloatSpinBoxW m_flagWidth{"flag_width"};
    ExFloatSpinBoxW m_flagHeight{"flag_height"};
    ExFloatSpinBoxW m_poleHeight{"pole_height"};
    ExSpinBoxW m_flagNbVerticesH{"flag_nb_vertices_horiz"};
    ExSpinBoxW m_flagNbVerticesV{"flag_nb_vertices_verti"};

    void insert_widgets() override{

        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Pole height: "), m_poleHeight()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Flag width: "), m_flagWidth()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Flag height: "), m_flagHeight()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Flag horizontal vertices number: "), m_flagNbVerticesH()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Flag vertical vertices number: "), m_flagNbVerticesV()}, LStretch{true}, LMargins{true}));
        add_sub_part_widget(m_transfo);
    }

    void init_and_register_widgets() override{
        map_sub_part(m_transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
        add_input_ui(m_poleHeight.init_widget(MinV<qreal>{0.0}, V<qreal>{4.0}, MaxV<qreal>{10.}, StepV<qreal>{0.01},2));
        add_input_ui(m_flagWidth.init_widget(MinV<qreal>{0.0}, V<qreal>{0.62}, MaxV<qreal>{10.}, StepV<qreal>{0.01},2));
        add_input_ui(m_flagHeight.init_widget(MinV<qreal>{0.0}, V<qreal>{0.925}, MaxV<qreal>{10.}, StepV<qreal>{0.01},2));
        add_input_ui(m_flagNbVerticesH.init_widget(MinV<int>{4}, V<int>{13}, MaxV<int>{50}, StepV<int>{1}));
        add_input_ui(m_flagNbVerticesV.init_widget(MinV<int>{4}, V<int>{20}, MaxV<int>{50}, StepV<int>{1}));
    }

    void create_connections() override{}
    void late_update_ui() override{}
};


class FlagPoleConfigParametersW : public ConfigParametersW{

public :

    TransformSubPart m_transfo{"transform"};
    ExResourceW m_currentTexture{"flag_image"};
    ExFloatSpinBoxW m_currentHeight{"height"};
    ExFloatSpinBoxW m_currentClothMaxDistance{"cloth_max_dist"};

    void insert_widgets() override{
        add_sub_part_widget(m_transfo);
        add_widget(ui::F::gen(ui::L::HB(), {m_currentTexture()}, LStretch{false}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Flag position: "), m_currentHeight()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Cloth max dist: "), m_currentClothMaxDistance()}, LStretch{false}, LMargins{true}));
    }

    void init_and_register_widgets() override{
        map_sub_part(m_transfo.init_widget(QSL("Config transform</b> (applied when routine starts)<b>")));
        add_input_ui(m_currentTexture.init_widget(Resource::Type::Image, "Flag texture"));
        add_input_ui(m_currentHeight.init_widget(MinV<qreal>{0.0}, V<qreal>{0.0}, MaxV<qreal>{1.}, StepV<qreal>{0.01},2));
        add_input_ui(m_currentClothMaxDistance.init_widget(MinV<qreal>{0.0}, V<qreal>{0.3}, MaxV<qreal>{1.}, StepV<qreal>{0.01},2));
    }

    void create_connections() override{
        // ...
    }

    void late_update_ui() override{}
};

}
