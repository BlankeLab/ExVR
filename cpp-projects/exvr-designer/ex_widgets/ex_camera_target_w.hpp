
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

// Qt
#include <QGridLayout>

// qt-utility
#include "ex_widgets/ex_vector3d_w.hpp"
#include "ex_widgets/ex_checkbox_w.hpp"
#include "ex_widgets/ex_select_color_w.hpp"
#include "ex_widgets/ex_curve_w.hpp"

// local
// # data
#include "data/config.hpp"
// # utility
#include "qt_str.hpp"


namespace tool::ex {

class ExCameraTargetW : public ExItemW<QFrame>{

public:

    ExCameraTargetW(QString name ="");
    ExCameraTargetW *init_widget(bool enabled = true);

    void update_from_arg(const Arg &arg) override;
    Arg convert_to_arg() const override;
    void set_as_generator() override;

    ExBaseW *init_widget2(std_v1<std::any> parameters) override;

public:

    ExCheckBoxW displayCamera = {"display_camera"};
    ExCheckBoxW sphericInterpolation = {"spheric_interpolation"};
    ExFloatSpinBoxW time = {"time"};
    ExVector3dW translation = {"translation"};
    ExVector3dW rotation = {"rotation"};
    ExSelectColorW color = {"color"};
    ExCurveW speed      = {"speed"};
};

}
