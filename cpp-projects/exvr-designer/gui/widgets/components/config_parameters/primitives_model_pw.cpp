


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

#include "primitives_model_pw.hpp"

// qt-utility
#include "gui/ex_widgets/ex_float_spin_box_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_select_color_w.hpp"
#include "gui/ex_widgets/ex_spin_box_w.hpp"
#include "gui/ex_widgets/ex_combo_box_index_w.hpp"

// local
#include "gui/ex_widgets/ex_resource_w.hpp"

using namespace tool::ex;
using namespace tool::ui;

struct ModelConfigParametersW::Impl{
    TransformSubPart transfo{"transform"};
    ExSelectColorW color{"color"};
    ExResourceW rgbaT{"rgba_t"};
    ExResourceW normalT{"normal_t"};
    ExResourceW heightT{"height_t"};
    ExFloatSpinBoxW metallic{"metallic"};
    ExFloatSpinBoxW smoothness{"smoothness"};
    ExComboBoxIndexW shadowMode{"shadow_mode"};
    ExCheckBoxW receiveShadows{"receive_shadows"};
    ExCheckBoxW enableCollider{"enable_collider"};
    ExCheckBoxW enableGravity{"enable_gravity"};
    ExCheckBoxW detectCollisions{"detect_collisions"};
    ExFloatSpinBoxW mass{"mass"};
    ExFloatSpinBoxW drag{"drag"};
};

ModelConfigParametersW::ModelConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void ModelConfigParametersW::insert_widgets(){

    add_sub_part_widget(m_p->transfo);

    auto matW = F::gen(L::HB(), {
        W::txt("Metallic: "),m_p->metallic(),
        W::txt("Smoothness: "),m_p->smoothness(),
        W::txt("Color: "),m_p->color()}, LStretch{true}, LMargins{true});
    add_widget(F::gen(L::VB(), {W::txt("<b>Material</b>"), matW, m_p->rgbaT(),m_p->normalT(),m_p->heightT()}, LStretch{false}, LMargins{true}, QFrame::Box));

    auto sW = F::gen(L::HB(), {
        W::txt("Mode: "),m_p->shadowMode(),
        m_p->receiveShadows()}, LStretch{true}, LMargins{true});
    add_widget(F::gen(L::VB(), {W::txt("<b>Shadows</b>"), sW},  LStretch{false}, LMargins{true}, QFrame::Box));

    auto p1W = F::gen(L::HB(), {
        m_p->enableCollider(),
        m_p->enableGravity(),
        m_p->detectCollisions()}, LStretch{true}, LMargins{true});
    auto p2W = F::gen(L::HB(), {
        W::txt("Mass: "),m_p->mass(),
        W::txt("Drag: "),m_p->drag()}, LStretch{true}, LMargins{true});
    add_widget(F::gen(L::VB(), {W::txt("<b>Physics</b>"), p1W, p2W},  LStretch{false}, LMargins{true}, QFrame::Box));
}

void ModelConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Config transform</b> (applied when routine starts)<b>")));
    add_input_ui(m_p->color.init_widget("Color", Qt::gray));
    add_input_ui(m_p->rgbaT.init_widget(Resource::Type::Image, "RGBA texture: "));
    add_input_ui(m_p->normalT.init_widget(Resource::Type::Image, "Normal texture: "));
    add_input_ui(m_p->heightT.init_widget(Resource::Type::Image, "Height texture: "));
    add_input_ui(m_p->metallic.init_widget(MinV<qreal>{0.0}, V<qreal>{0.0}, MaxV<qreal>{1.}, StepV<qreal>{0.01},2));
    add_input_ui(m_p->smoothness.init_widget(MinV<qreal>{0.0}, V<qreal>{0.0}, MaxV<qreal>{1.}, StepV<qreal>{0.01},2));
    add_input_ui(m_p->shadowMode.init_widget({"Off", "On", "Two sides", "Shadow only"},1));
    add_input_ui(m_p->receiveShadows.init_widget("Receive", true));
    add_input_ui(m_p->enableCollider.init_widget("Enable collider", true));
    add_input_ui(m_p->enableGravity.init_widget("Enable gravity", false));
    add_input_ui(m_p->detectCollisions.init_widget("Detect collisions", false));
    add_input_ui(m_p->mass.init_widget(MinV<qreal>{0.0}, V<qreal>{1.0}, MaxV<qreal>{1000000000.}, StepV<qreal>{0.1},2));
    add_input_ui(m_p->drag.init_widget(MinV<qreal>{0.0}, V<qreal>{0.0}, MaxV<qreal>{1000000000.}, StepV<qreal>{0.1},2));

    if(type == Component::Type::Plane){
        m_p->transfo.set_rotation_values({90.0,0.0,0.0});
    }
}



struct CubeInitConfigParametersW::Impl{
    TransformSubPart transfo{"init_transform"};
    ExFloatSpinBoxW dsbSize{"size"};
    ExCheckBoxW transparent{"transparent"};

};

CubeInitConfigParametersW::CubeInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void CubeInitConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);
    auto w1 = F::gen(L::HB(), {W::txt("Cube size: "),m_p->dsbSize()}, LStretch{true}, LMargins{true});
    auto w2 = F::gen(L::HB(), {m_p->transparent()}, LStretch{true}, LMargins{true});
    add_widget(F::gen(L::VB(), {W::txt("<b>Procedural generation:</b>"), w1,w2},  LStretch{false}, LMargins{true}, QFrame::Box));
}

void CubeInitConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
    add_input_ui(m_p->dsbSize.init_widget(MinV<qreal>{0.1}, V<qreal>{1.}, MaxV<qreal>{1000.}, StepV<qreal>{0.1},2));
    add_input_ui(m_p->transparent.init_widget("Is transparent?", false));
}


struct PlaneInitConfigParametersW::Impl{
    TransformSubPart transfo{"init_transform"};
    ExFloatSpinBoxW width{"width"};
    ExFloatSpinBoxW height{"height"};
    ExCheckBoxW transparent{"transparent"};

};

PlaneInitConfigParametersW::PlaneInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void PlaneInitConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);
    auto w1 = F::gen(L::HB(), {W::txt("Width: "),m_p->width()}, LStretch{true}, LMargins{true});
    auto w2 = F::gen(L::HB(), {W::txt("Height: "),m_p->height()}, LStretch{true}, LMargins{true});
    auto w3 = F::gen(L::HB(), {m_p->transparent()}, LStretch{true}, LMargins{true});
    add_widget(F::gen(L::VB(), {W::txt("<b>Procedural generation:</b>"), w1,w2,w3},  LStretch{false}, LMargins{true}, QFrame::Box));
}

void PlaneInitConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));    
    add_input_ui(m_p->width.init_widget(MinV<qreal>{0.1}, V<qreal>{10.}, MaxV<qreal>{1000.}, StepV<qreal>{0.1},2));
    add_input_ui(m_p->height.init_widget(MinV<qreal>{0.1}, V<qreal>{10.}, MaxV<qreal>{1000.}, StepV<qreal>{0.1},2));
    add_input_ui(m_p->transparent.init_widget("Is transparent?", false));
}


struct CylinderInitConfigParametersW::Impl{
    TransformSubPart transfo{"init_transform"};
    ExSpinBoxW nbSides{"nb_sides"};
    ExFloatSpinBoxW radius{"radius"};
    ExFloatSpinBoxW length{"length"};
    ExCheckBoxW transparent{"transparent"};
};

CylinderInitConfigParametersW::CylinderInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void CylinderInitConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);
    auto w1 = F::gen(L::HB(), {W::txt("Number sides: "),m_p->nbSides()}, LStretch{true}, LMargins{true});
    auto w2 = F::gen(L::HB(), {W::txt("Radius: "),m_p->radius()}, LStretch{true}, LMargins{true});
    auto w3 = F::gen(L::HB(), {W::txt("Length: "),m_p->length()}, LStretch{true}, LMargins{true});
    auto w4 = F::gen(L::HB(), {m_p->transparent()}, LStretch{true}, LMargins{true});
    add_widget(F::gen(L::VB(), {W::txt("<b>Procedural generation:</b>"), w1,w2,w3,w4},  LStretch{false}, LMargins{true}, QFrame::Box));
}

void CylinderInitConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
    add_input_ui(m_p->nbSides.init_widget(MinV<int>{8}, V<int>{18}, MaxV<int>{100}, StepV<int>{1}));
    add_input_ui(m_p->radius.init_widget(MinV<qreal>{0.01}, V<qreal>{0.5}, MaxV<qreal>{1000.}, StepV<qreal>{0.1},2));
    add_input_ui(m_p->length.init_widget(MinV<qreal>{0.01}, V<qreal>{1.0}, MaxV<qreal>{1000.}, StepV<qreal>{0.1},2));
    add_input_ui(m_p->transparent.init_widget("Is transparent?", false));
}


struct SphereInitConfigParametersW::Impl{
    TransformSubPart transfo{"init_transform"};
    ExFloatSpinBoxW dsbRadius{"radius"};
    ExSpinBoxW sbLong{"nb_log"};
    ExSpinBoxW sbLat{"nb_lat"};
    ExCheckBoxW transparent{"transparent"};
};

SphereInitConfigParametersW::SphereInitConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){

}

void SphereInitConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);
    auto w1 = F::gen(L::HB(), {W::txt("Sphere radius: "),m_p->dsbRadius()}, LStretch{true}, LMargins{true});
    auto w2 = F::gen(L::HB(), {W::txt("Long nb triangles: "),m_p->sbLong()}, LStretch{true}, LMargins{true});
    auto w3 = F::gen(L::HB(), {W::txt("Lat nb triangles: "),m_p->sbLat()}, LStretch{true}, LMargins{true});
    auto w4 = F::gen(L::HB(), {m_p->transparent()}, LStretch{true}, LMargins{true});
    add_widget(F::gen(L::VB(), {W::txt("<b>Procedural generation:</b>"), w1,w2,w3,w4},  LStretch{false}, LMargins{true}, QFrame::Box));
}

void SphereInitConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
    add_input_ui(m_p->dsbRadius.init_widget(MinV<qreal>{0.01}, V<qreal>{0.25}, MaxV<qreal>{1000.}, StepV<qreal>{0.01},2));
    add_input_ui(m_p->sbLong.init_widget(MinV<int>{12}, V<int>{24}, MaxV<int>{100}, StepV<int>{1},1));
    add_input_ui(m_p->sbLat.init_widget(MinV<int>{8}, V<int>{16}, MaxV<int>{100}, StepV<int>{1},1));
    add_input_ui(m_p->transparent.init_widget("Is transparent?", false));
}


struct TorusInitConfigParametersW::Impl{
    TransformSubPart transfo {"init_transform"};
    ExFloatSpinBoxW outerRadius{"outer_radius"};
    ExFloatSpinBoxW innerRadius{"inner_radius"};
    ExSpinBoxW sidesCount{"sides_count"};
    ExSpinBoxW ringsCount{"rings_count"};
    ExCheckBoxW transparent{"transparent"};
};

TorusInitConfigParametersW::TorusInitConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){

}

void TorusInitConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);
    auto w1 = F::gen(L::HB(), {W::txt("Outer radius: "),m_p->outerRadius()}, LStretch{true}, LMargins{true});
    auto w2 = F::gen(L::HB(), {W::txt("Inner radius: "),m_p->innerRadius()}, LStretch{true}, LMargins{true});
    auto w3 = F::gen(L::HB(), {W::txt("Sides count: "),m_p->sidesCount()}, LStretch{true}, LMargins{true});
    auto w4 = F::gen(L::HB(), {W::txt("Rings count: "),m_p->ringsCount()}, LStretch{true}, LMargins{true});
    auto w5 = F::gen(L::HB(), {m_p->transparent()}, LStretch{true}, LMargins{true});
    add_widget(F::gen(L::VB(), {W::txt("<b>Procedural generation:</b>"), w1,w2,w3,w4,w5},  LStretch{false}, LMargins{true}, QFrame::Box));
}

void TorusInitConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
    add_input_ui(m_p->outerRadius.init_widget(MinV<qreal>{0.01}, V<qreal>{0.3}, MaxV<qreal>{1000.}, StepV<qreal>{0.01},2));
    add_input_ui(m_p->innerRadius.init_widget(MinV<qreal>{0.01}, V<qreal>{0.15}, MaxV<qreal>{1000.}, StepV<qreal>{0.01},2));
    add_input_ui(m_p->sidesCount.init_widget(MinV<int>{12}, V<int>{24}, MaxV<int>{1000}, StepV<int>{1},1));
    add_input_ui(m_p->ringsCount.init_widget(MinV<int>{12}, V<int>{24}, MaxV<int>{1000}, StepV<int>{1},1));
    add_input_ui(m_p->transparent.init_widget("Is transparent?", false));
}


struct LinesInitConfigParametersW::Impl{
    TransformSubPart transfo = {"init_transform"};
};

LinesInitConfigParametersW::LinesInitConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){

}

void LinesInitConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);
}

void LinesInitConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
}

struct LinesConfigParametersW::Impl{
    TransformSubPart transfo = {"transform"};
};


LinesConfigParametersW::LinesConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){

}

void LinesConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);
}

void LinesConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Config transform</b> (applied when routine starts)<b>")));
}

struct LandmarkInitConfigParametersW::Impl{
    TransformSubPart transfo = {"init_transform"};
};


LandmarkInitConfigParametersW::LandmarkInitConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){

}

void LandmarkInitConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);
}

void LandmarkInitConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
}

struct LandmarkConfigParametersW::Impl{
    TransformSubPart transfo = {"transform"};
    ExSelectColorW color{"color"};
};


LandmarkConfigParametersW::LandmarkConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){

}

void LandmarkConfigParametersW::insert_widgets(){
    add_sub_part_widget(m_p->transfo);
    add_widget(F::gen(L::HB(), {W::txt("Color: "),m_p->color()}, LStretch{true}, LMargins{true}));
}

void LandmarkConfigParametersW::init_and_register_widgets(){
    map_sub_part(m_p->transfo.init_widget(QSL("Config transform</b> (applied when routine starts)<b>")));
    add_input_ui(m_p->color.init_widget("Color", Qt::gray));
}
