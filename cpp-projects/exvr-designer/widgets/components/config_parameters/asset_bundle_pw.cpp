
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

#include "asset_bundle_pw.hpp"


// qt-utility
#include "ex_widgets/ex_checkbox_w.hpp"
#include "ex_widgets/ex_line_edit_w.hpp"

// local
#include "ex_widgets/ex_resource_w.hpp"


using namespace tool::ex;

struct AssetBundleInitConfigParametersW::Impl{
    TransformSubPart transfo{"init_transform"};
    ExResourceW assetBundle {"asset_bundle"};
    ExLineEditW leSubObjectName{"name_sub_object"};
    ExCheckBoxW displayHierarchy{"display_hierarchy"};
    QTextEdit *hierarchy = nullptr;
};

AssetBundleInitConfigParametersW::AssetBundleInitConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void AssetBundleInitConfigParametersW::insert_widgets(){

    add_widget(ui::F::gen(ui::L::HB(), {m_p->assetBundle()}, LStretch{false}, LMargins{true}, QFrame::Box));
    add_widget(ui::F::gen(ui::L::VB(),{
        ui::W::txt(QSL("Sub object to load in bundle (if empty, first object of the hierarchy will be load): ")),
        m_p->leSubObjectName(), m_p->displayHierarchy(), m_p->hierarchy = new QTextEdit()}, LStretch{false}, LMargins{true},QFrame::Box)
    );
    add_sub_part_widget(m_p->transfo);
    m_p->hierarchy->setReadOnly(true);
    no_end_stretch();
}

void AssetBundleInitConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
    add_input_ui(m_p->assetBundle.init_widget(Resource::Type::AssetBundle, QSL("Asset bundle resource: ")));
    add_input_ui(m_p->leSubObjectName.init_widget(""));
    add_input_ui(m_p->displayHierarchy.init_widget(QSL("Display asset bundle content hierarchy"), false));
}


void AssetBundleInitConfigParametersW::update_with_info(QStringView id, QStringView value){
    if(id == QSL("hierarchy")){
        m_p->hierarchy->setText(value.toString());
    }
}

struct AssetBundleConfigParametersW::Impl{
    TransformSubPart transfo{"transform"};
};


AssetBundleConfigParametersW::AssetBundleConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void AssetBundleConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);
}

void AssetBundleConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Config transform</b> (applied when routine starts)<b>")));
}
