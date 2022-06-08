
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
#include "gui/ex_widgets/ex_select_color_w.hpp"
#include "gui/ex_widgets/ex_spin_box_w.hpp"

// local
#include "config_pw.hpp"

namespace tool::ex {

class CubeInitConfigParametersW : public ConfigParametersW{

public :

    TransformSubPart m_transfo{"init_transform"};
    ExFloatSpinBoxW m_dsbSize{"size"};
    ExCheckBoxW m_transparent{"transparent"};

    void insert_widgets() override{
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Cube size: "),m_dsbSize()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {m_transparent()}, LStretch{true}, LMargins{true}));
        add_sub_part_widget(m_transfo);
    }

    void init_and_register_widgets() override{
        map_sub_part(m_transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
        add_input_ui(m_dsbSize.init_widget(MinV<qreal>{0.1}, V<qreal>{1.}, MaxV<qreal>{1000.}, StepV<qreal>{0.1},2));
        add_input_ui(m_transparent.init_widget("Is transparent?", false));
    }

    void create_connections() override{}
    void late_update_ui() override{}
};


class CubeConfigParametersW : public ConfigParametersW{

public :

    TransformSubPart m_transfo{"transform"};
    ExSelectColorW m_color{"color"};
    ExFloatSpinBoxW m_metallic{"metallic"};
    ExFloatSpinBoxW m_smoothness{"smoothness"};

    void insert_widgets() override{
        add_sub_part_widget(m_transfo);
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Color: "),m_color()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Metallic: "),m_metallic()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Smoothness: "),m_smoothness()}, LStretch{true}, LMargins{true}));
    }

    void init_and_register_widgets() override{
        map_sub_part(m_transfo.init_widget(QSL("Config transform</b> (applied when routine starts)<b>")));
        add_input_ui(m_color.init_widget("Color", Qt::gray));
        add_input_ui(m_metallic.init_widget(MinV<qreal>{0.0}, V<qreal>{0.0}, MaxV<qreal>{1.}, StepV<qreal>{0.01},2));
        add_input_ui(m_smoothness.init_widget(MinV<qreal>{0.0}, V<qreal>{0.0}, MaxV<qreal>{1.}, StepV<qreal>{0.01},2));
    }

    void create_connections() override{}
    void late_update_ui() override{}
};


class CylinderInitConfigParametersW : public ConfigParametersW{

public :

    TransformSubPart m_transfo{"init_transform"};
    ExSpinBoxW m_nbSides{"nb_sides"};
    ExFloatSpinBoxW m_radius{"radius"};
    ExFloatSpinBoxW m_length{"length"};
    ExCheckBoxW m_transparent{"transparent"};

    void insert_widgets() override{
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Number sides: "),m_nbSides()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Radius: "),m_radius()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Length: "),m_length()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {m_transparent()}, LStretch{true}, LMargins{true}));
        add_sub_part_widget(m_transfo);
    }

    void init_and_register_widgets() override{
        map_sub_part(m_transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
        add_input_ui(m_nbSides.init_widget(MinV<int>{8}, V<int>{18}, MaxV<int>{100}, StepV<int>{1}));
        add_input_ui(m_radius.init_widget(MinV<qreal>{0.01}, V<qreal>{0.5}, MaxV<qreal>{1000.}, StepV<qreal>{0.1},2));
        add_input_ui(m_length.init_widget(MinV<qreal>{0.01}, V<qreal>{1.0}, MaxV<qreal>{1000.}, StepV<qreal>{0.1},2));
        add_input_ui(m_transparent.init_widget("Is transparent?", false));
    }

    void create_connections() override{}
    void late_update_ui() override{}
};


class CylinderConfigParametersW : public ConfigParametersW{

public :

    TransformSubPart m_transfo{"transform"};
    ExSelectColorW m_color{"color"};
    ExFloatSpinBoxW m_metallic{"metallic"};
    ExFloatSpinBoxW m_smoothness{"smoothness"};

    void insert_widgets() override{
        add_sub_part_widget(m_transfo);
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Color: "),m_color()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Metallic: "),m_metallic()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Smoothness: "),m_smoothness()}, LStretch{true}, LMargins{true}));
    }

    void init_and_register_widgets() override{
        map_sub_part(m_transfo.init_widget(QSL("Config transform</b> (applied when routine starts)<b>")));
        add_input_ui(m_color.init_widget("Color", Qt::gray));
        add_input_ui(m_metallic.init_widget(MinV<qreal>{0.0}, V<qreal>{0.0}, MaxV<qreal>{1.}, StepV<qreal>{0.01},2));
        add_input_ui(m_smoothness.init_widget(MinV<qreal>{0.0}, V<qreal>{0.0}, MaxV<qreal>{1.}, StepV<qreal>{0.01},2));
    }

    void create_connections() override{}
    void late_update_ui() override{}
};



class SphereInitConfigParametersW : public ConfigParametersW{

public :

    TransformSubPart m_transfo{"init_transform"};
    ExFloatSpinBoxW m_dsbRadius{"radius"};
    ExSpinBoxW m_sbLong{"nb_log"};
    ExSpinBoxW m_sbLat{"nb_lat"};
    ExCheckBoxW m_transparent{"transparent"};

    void insert_widgets() override{
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Sphere radius: "),m_dsbRadius()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Long nb triangles: "),m_sbLong()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Lat nb triangles: "),m_sbLat()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {m_transparent()}, LStretch{true}, LMargins{true}));
        add_sub_part_widget(m_transfo);
    }

    void init_and_register_widgets() override{
        map_sub_part(m_transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
        add_input_ui(m_dsbRadius.init_widget(MinV<qreal>{0.01}, V<qreal>{0.05}, MaxV<qreal>{1000.}, StepV<qreal>{0.01},2));
        add_input_ui(m_sbLong.init_widget(MinV<int>{12}, V<int>{24}, MaxV<int>{100}, StepV<int>{1},1));
        add_input_ui(m_sbLat.init_widget(MinV<int>{8}, V<int>{16}, MaxV<int>{100}, StepV<int>{1},1));
        add_input_ui(m_transparent.init_widget("Is transparent?", false));
    }

    void create_connections() override{}
    void late_update_ui() override{}
};


class SphereConfigParametersW : public ConfigParametersW{

public :

    TransformSubPart m_transfo{"transform"};
    ExSelectColorW m_color{"color"};
    ExFloatSpinBoxW m_metallic{"metallic"};
    ExFloatSpinBoxW m_smoothness{"smoothness"};

    void insert_widgets() override{
        add_sub_part_widget(m_transfo);
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Color: "),m_color()}, LStretch{true}, LMargins{true}));        
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Metallic: "),m_metallic()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Smoothness: "),m_smoothness()}, LStretch{true}, LMargins{true}));
    }

    void init_and_register_widgets() override{
        map_sub_part(m_transfo.init_widget(QSL("Config transform</b> (applied when routine starts)<b>")));
        add_input_ui(m_color.init_widget("Color", Qt::gray));
        add_input_ui(m_metallic.init_widget(MinV<qreal>{0.0}, V<qreal>{0.0}, MaxV<qreal>{1.}, StepV<qreal>{0.01},2));
        add_input_ui(m_smoothness.init_widget(MinV<qreal>{0.0}, V<qreal>{0.0}, MaxV<qreal>{1.}, StepV<qreal>{0.01},2));
    }

    void create_connections() override{}
    void late_update_ui() override{}
};

class TorusInitConfigParametersW : public ConfigParametersW{

public :

    TransformSubPart m_transfo {"init_transform"};
    ExFloatSpinBoxW m_outerRadius{"outer_radius"};
    ExFloatSpinBoxW m_innerRadius{"inner_radius"};
    ExSpinBoxW m_sidesCount{"sides_count"};
    ExSpinBoxW m_ringsCount{"rings_count"};
    ExCheckBoxW m_transparent{"transparent"};

    void insert_widgets() override{        
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Outer radius: "),m_outerRadius()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Inner radius: "),m_innerRadius()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Sides count: "),m_sidesCount()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Rings count: "),m_ringsCount()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {m_transparent()}, LStretch{true}, LMargins{true}));
        add_sub_part_widget(m_transfo);
    }

    void init_and_register_widgets() override{
        map_sub_part(m_transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
        add_input_ui(m_outerRadius.init_widget(MinV<qreal>{0.01}, V<qreal>{0.3}, MaxV<qreal>{1000.}, StepV<qreal>{0.01},2));
        add_input_ui(m_innerRadius.init_widget(MinV<qreal>{0.01}, V<qreal>{0.15}, MaxV<qreal>{1000.}, StepV<qreal>{0.01},2));
        add_input_ui(m_sidesCount.init_widget(MinV<int>{12}, V<int>{24}, MaxV<int>{1000}, StepV<int>{1},1));
        add_input_ui(m_ringsCount.init_widget(MinV<int>{12}, V<int>{24}, MaxV<int>{1000}, StepV<int>{1},1));
        add_input_ui(m_transparent.init_widget("Is transparent?", false));
    }

    void create_connections() override{}
    void late_update_ui() override{}

};


class TorusConfigParametersW : public ConfigParametersW{

public :

    TransformSubPart m_transfo{"transform"};
    ExSelectColorW m_color{"color"};
    ExFloatSpinBoxW m_metallic{"metallic"};
    ExFloatSpinBoxW m_smoothness{"smoothness"};

    void insert_widgets() override{
        add_sub_part_widget(m_transfo);
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Color: "),m_color()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Metallic: "),m_metallic()}, LStretch{true}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Smoothness: "),m_smoothness()}, LStretch{true}, LMargins{true}));
    }

    void init_and_register_widgets() override{
        map_sub_part(m_transfo.init_widget(QSL("Config transform</b> (applied when routine starts)<b>")));
        add_input_ui(m_color.init_widget("Color", Qt::gray));
        add_input_ui(m_metallic.init_widget(MinV<qreal>{0.0}, V<qreal>{0.0}, MaxV<qreal>{1.}, StepV<qreal>{0.01},2));
        add_input_ui(m_smoothness.init_widget(MinV<qreal>{0.0}, V<qreal>{0.0}, MaxV<qreal>{1.}, StepV<qreal>{0.01},2));
    }

    void create_connections() override{}
    void late_update_ui() override{}
};


class LinesInitConfigParametersW : public ConfigParametersW{
public :

    TransformSubPart m_transfo = {"init_transform"};

    void insert_widgets() override{
        add_sub_part_widget(m_transfo);
    }
    void init_and_register_widgets() override{
        map_sub_part(m_transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
    }
    void create_connections() override{}
    void late_update_ui() override{}
};


class LinesConfigParametersW : public ConfigParametersW{

public :

    TransformSubPart m_transfo = {"transform"};

    void insert_widgets() override{
        add_sub_part_widget(m_transfo);
    }

    void init_and_register_widgets() override{
        map_sub_part(m_transfo.init_widget(QSL("Config transform</b> (applied when routine starts)<b>")));
    }

    void create_connections() override{}
    void late_update_ui() override{}
};


class LandmarkInitConfigParametersW : public ConfigParametersW{
public :

    TransformSubPart m_transfo = {"init_transform"};

    void insert_widgets() override{
        add_sub_part_widget(m_transfo);
    }
    void init_and_register_widgets() override{
        map_sub_part(m_transfo.init_widget(QSL("Init transform</b> (applied when experiment starts)<b>")));
    }
    void create_connections() override{}
    void late_update_ui() override{}
};

class LandmarkConfigParametersW : public ConfigParametersW{

public :

    TransformSubPart m_transfo = {"transform"};
    ExSelectColorW m_color{"color"};

    void insert_widgets() override{
        add_sub_part_widget(m_transfo);
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Color: "),m_color()}, LStretch{true}, LMargins{true}));
    }

    void init_and_register_widgets() override{
        map_sub_part(m_transfo.init_widget(QSL("Config transform</b> (applied when routine starts)<b>")));
        add_input_ui(m_color.init_widget("Color", Qt::gray));
    }

    void create_connections() override{}
    void late_update_ui() override{}
};


}
