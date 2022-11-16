
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

#include "ex_oob_filtering_w.hpp"

// qt-utility
#include "qt_logger.hpp"

tool::ex::OBBFilteringW::OBBFilteringW(QString name): ExItemW<QFrame>(UiType::Custom, name){

    w->setFrameShadow(QFrame::Raised);
    w->setFrameShape(QFrame::Shape::NoFrame);

    auto layout = new QVBoxLayout();
    w->setLayout(layout);
    layout->addWidget(ui::F::gen(ui::L::HB(),{displayObb(), enableObb(), obbColor()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    layout->addWidget(obbTr());

    connect(&displayObb, &ExBaseW::ui_change_signal, this, &ExBaseW::ui_change_signal);
    connect(&enableObb,  &ExBaseW::ui_change_signal, this, &ExBaseW::ui_change_signal);
    connect(&obbColor,   &ExBaseW::ui_change_signal, this, &ExBaseW::ui_change_signal);
    connect(&obbTr,      &ExBaseW::ui_change_signal, this, &ExBaseW::ui_change_signal);
}

tool::ex::OBBFilteringW *tool::ex::OBBFilteringW::init_widget(QColor color, bool enabled){

    w->setEnabled(enabled);

    DsbSettings pos   = {MinV<qreal>{-10000.}, V<qreal>{0.},MaxV<qreal>{10000.}, StepV<qreal>{0.1}, 3};
    DsbSettings rot   = {MinV<qreal>{-360.}, V<qreal>{0.},MaxV<qreal>{360.}, StepV<qreal>{0.1}, 2};
    DsbSettings scale = {MinV<qreal>{0}, V<qreal>{1.},MaxV<qreal>{10000.}, StepV<qreal>{0.1}, 3};

    obbTr.init_widget("Transform",
                      Vector3dSettings{pos,pos,pos},
                      Vector3dSettings{rot,rot,rot},
                      Vector3dSettings{scale,scale,scale}
                      );

    displayObb.init_widget("Display", true);
    enableObb.init_widget("Enabled", true);
    obbColor.init_widget("Color", color);

    return this;
}

void tool::ex::OBBFilteringW::update_from_arg(const Arg &arg){

    ExItemW::update_from_arg(arg);

    auto values = arg.split_value("[#OBBFW#]");
    if(values.size() != 4){
        QtLogger::error(QSL("OBBFilteringW: Invalid arg ") % QString::number(values.count()) % QSL("->") % arg.value() );
        return;
    }

    Arg sub;
    sub.init_from_bool_str(values[0]);
    enableObb.update_from_arg(sub);

    sub.init_from_bool_str(values[1]);
    displayObb.update_from_arg(sub);

    sub.init_from_color_str(values[2]);
    obbColor.update_from_arg(sub);

    sub.init_from_transform_str(values[3]);
    obbTr.update_from_arg(sub);
}

tool::ex::Arg tool::ex::OBBFilteringW::convert_to_arg() const{

    Arg arg = ExBaseW::convert_to_arg();
    arg.init_from_args({
       enableObb.convert_to_arg(),
       displayObb.convert_to_arg(),
       obbColor.convert_to_arg(),
       obbTr.convert_to_arg()
   }, "[#OBBFW#]");

    return arg;
}

tool::ex::ExBaseW *tool::ex::OBBFilteringW::init_widget_from_any_array(std::vector<std::any> &parameters){
    init_widget(std::any_cast<QColor>(parameters[0]));
    return this;
}
