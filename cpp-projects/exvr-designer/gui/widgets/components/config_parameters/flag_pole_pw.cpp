
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

#include "flag_pole_pw.hpp"

using namespace tool::ex;
using namespace tool::ui;

struct FlagPoleInitConfigParametersW::Impl{
    TransformSubPart transfo{"init_transform"};
    ExFloatSpinBoxW flagWidth{"flag_width"};
    ExFloatSpinBoxW flagHeight{"flag_height"};
    ExFloatSpinBoxW poleHeight{"pole_height"};
    ExSpinBoxW flagNbVerticesH{"flag_nb_vertices_horiz"};
    ExSpinBoxW flagNbVerticesV{"flag_nb_vertices_verti"};
};

FlagPoleInitConfigParametersW::FlagPoleInitConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void FlagPoleInitConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);
    auto w1 = F::gen(L::HB(), {W::txt("Pole height: "), m_p->poleHeight()}, LStretch{true}, LMargins{true});
    auto w2 = F::gen(L::HB(), {W::txt("Flag width: "), m_p->flagWidth()}, LStretch{true}, LMargins{true});
    auto w3 = F::gen(L::HB(), {W::txt("Flag height: "), m_p->flagHeight()}, LStretch{true}, LMargins{true});
    auto w4 = F::gen(L::HB(), {W::txt("Flag horizontal vertices number: "), m_p->flagNbVerticesH()}, LStretch{true}, LMargins{true});
    auto w5 = F::gen(L::HB(), {W::txt("Flag vertical vertices number: "), m_p->flagNbVerticesV()}, LStretch{true}, LMargins{true});
    add_widget(F::gen(L::VB(), {W::txt("<b>Procedural generation:</b>"), w1,w2,w3,w4,w5},  LStretch{false}, LMargins{true}, QFrame::Box));
}

void FlagPoleInitConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
    add_input_ui(m_p->poleHeight.init_widget(MinV<qreal>{0.0}, V<qreal>{4.0}, MaxV<qreal>{10.}, StepV<qreal>{0.01},2));
    add_input_ui(m_p->flagWidth.init_widget(MinV<qreal>{0.0}, V<qreal>{0.62}, MaxV<qreal>{10.}, StepV<qreal>{0.01},2));
    add_input_ui(m_p->flagHeight.init_widget(MinV<qreal>{0.0}, V<qreal>{0.925}, MaxV<qreal>{10.}, StepV<qreal>{0.01},2));
    add_input_ui(m_p->flagNbVerticesH.init_widget(MinV<int>{4}, V<int>{13}, MaxV<int>{50}, StepV<int>{1}));
    add_input_ui(m_p->flagNbVerticesV.init_widget(MinV<int>{4}, V<int>{20}, MaxV<int>{50}, StepV<int>{1}));
}

struct FlagPoleConfigParametersW::Impl{
    TransformSubPart transfo{"transform"};
    ExResourceW currentTexture{"flag_image"};
    ExFloatSpinBoxW currentHeight{"height"};
    ExFloatSpinBoxW currentClothMaxDistance{"cloth_max_dist"};
};

FlagPoleConfigParametersW::FlagPoleConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void FlagPoleConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);
    auto w1 = F::gen(L::HB(), {m_p->currentTexture()}, LStretch{false}, LMargins{true});
    auto w2 = F::gen(L::HB(), {W::txt("Flag position: "), m_p->currentHeight()}, LStretch{true}, LMargins{true});
    auto w3 = F::gen(L::HB(), {W::txt("Cloth max dist: "), m_p->currentClothMaxDistance()}, LStretch{true}, LMargins{true});
    add_widget(F::gen(L::VB(), {W::txt("<b>Flag settings:</b>"), w1,w2,w3},  LStretch{false}, LMargins{true}, QFrame::Box));
}

void FlagPoleConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Config transform</b> (applied when routine starts)<b>")));
    add_input_ui(m_p->currentTexture.init_widget(Resource::Type::Image, "Flag texture"));
    add_input_ui(m_p->currentHeight.init_widget(MinV<qreal>{0.0}, V<qreal>{0.0}, MaxV<qreal>{1.}, StepV<qreal>{0.01},2));
    add_input_ui(m_p->currentClothMaxDistance.init_widget(MinV<qreal>{0.0}, V<qreal>{0.3}, MaxV<qreal>{1.}, StepV<qreal>{0.01},2));
}
