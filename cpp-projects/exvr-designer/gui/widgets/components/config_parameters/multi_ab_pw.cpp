
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

#include "multi_ab_pw.hpp"

using namespace tool::ex;

struct MultiABInitConfigParametersW::Impl{
    TransformSubPart transfo{"init_transform"};
    ExResourcesListW abList{"ab_list"};
};

MultiABInitConfigParametersW::MultiABInitConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void MultiABInitConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);
    add_widget(ui::F::gen(ui::L::VB(),{
        ui::W::txt(QSL("<b>Asset bundle</b>")),
        m_p->abList()}, LStretch{false}, LMargins{true}, QFrame::Box
    ));
    no_end_stretch();
}

void MultiABInitConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
    add_input_ui(m_p->abList.init_widget(Resource::Type::AssetBundle, "Resources to load:"));
}

struct MultiABConfigParametersW::Impl{
    TransformSubPart transfo{"transform"};
    ExResourceW currentAB{"ab_alias"};
};

MultiABConfigParametersW::MultiABConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void MultiABConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);
    add_widget(ui::F::gen(ui::L::VB(),{
        ui::W::txt(QSL("<b>Asset bundle</b>")),
        m_p->currentAB()}, LStretch{false}, LMargins{true}, QFrame::Box
    ));
}

void MultiABConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Config transform</b> (applied when routine starts)<b>")));
    add_input_ui(m_p->currentAB.init_widget(Resource::Type::AssetBundle,QSL("Current resource")));

}

