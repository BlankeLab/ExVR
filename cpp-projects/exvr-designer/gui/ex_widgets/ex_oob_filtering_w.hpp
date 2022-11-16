

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
#include "gui/ex_widgets/ex_item_w.hpp"
#include "gui/ex_widgets/ex_select_color_w.hpp"
#include "gui/ex_widgets/ex_transformation_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"

namespace tool::ex{


class OBBFilteringW : public ExItemW<QFrame>{

public:

    OBBFilteringW(QString name ="");
    OBBFilteringW *init_widget(QColor color, bool enabled = true);

    static std::vector<std::any> generate_init_any_array(QColor color){
        return {
            std::make_any<QColor>(color)
        };
    }

    void update_from_arg(const Arg &arg) override;
    Arg convert_to_arg() const override;
    ExBaseW *init_widget_from_any_array(std::vector<std::any> &parameters) override;

    ExCheckBoxW enableObb{"obb_enabled"};
    ExCheckBoxW displayObb{"obb_display"};
    ExSelectColorW obbColor{"obb_color"};
    ExTransformationW obbTr{"obb_tr"};
};

}
