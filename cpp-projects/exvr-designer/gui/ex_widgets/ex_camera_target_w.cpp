
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

#include "ex_camera_target_w.hpp"

using namespace tool;
using namespace tool::ex;

ExCameraTargetW::ExCameraTargetW(QString name) : ExItemW<QFrame>(UiType::Custom, name){

    w->setFrameShadow(QFrame::Raised);
    w->setFrameShape(QFrame::Shape::NoFrame);

    auto layout = new QVBoxLayout();
    w->setLayout(layout);

    layout->addWidget(ui::F::gen(ui::L::HB(),{ui::W::txt("Time for reaching target: "), time()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    layout->addWidget(ui::F::gen(ui::L::HB(),{displayCamera.w.get(), sphericInterpolation()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    layout->addWidget(translation());
    layout->addWidget(rotation());
    layout->addWidget(ui::F::gen(ui::L::HB(),{ui::W::txt("Camera color: "), color()}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    layout->addWidget(speed());

    connect(&time,          &ExBaseW::ui_change_signal,     this, &ExBaseW::ui_change_signal);
    connect(&displayCamera, &ExBaseW::ui_change_signal,     this, &ExBaseW::ui_change_signal);
    connect(&sphericInterpolation, &ExBaseW::ui_change_signal,     this, &ExBaseW::ui_change_signal);
    connect(&translation,   &ExBaseW::ui_change_signal,     this, &ExBaseW::ui_change_signal);
    connect(&rotation,      &ExBaseW::ui_change_signal,     this, &ExBaseW::ui_change_signal);
    connect(&color,         &ExBaseW::ui_change_signal,     this, &ExBaseW::ui_change_signal);
    connect(&speed,         &ExBaseW::ui_change_signal,     this, &ExBaseW::ui_change_signal);
}

ExCameraTargetW *ExCameraTargetW::init_widget(bool enabled){

    w->setEnabled(enabled);
    DsbSettings s1 = {MinV<qreal>{-10000.}, V<qreal>{0.},MaxV<qreal>{10000.}, StepV<qreal>{0.1}, 2};
    time.init_widget(MinV<qreal>{0.}, V<qreal>{3.},MaxV<qreal>{1000.}, StepV<qreal>{0.1}, 2);
    displayCamera.init_widget("Display camera", true);
    sphericInterpolation.init_widget("Spheric interpolation", true);
    translation.init_widget("Translation: ", Vector3dSettings{s1,s1,s1});
    rotation.init_widget("Rotation: ", Vector3dSettings{s1,s1,s1});
    color.init_widget("Pick camera object color", Qt::red);
    speed.init_widget("Time factor");

    return this;
}

ExBaseW *ExCameraTargetW::init_widget_from_any_array(std::vector<std::any> &parameters){
    init_widget(std::any_cast<bool>(parameters[0]));
    return this;
}



void ExCameraTargetW::update_from_arg(const Arg &arg){

    ExItemW::update_from_arg(arg);

    auto size = arg.count();
    if(size != 7){
        qWarning() << "Invalid argument value " << arg.name << " for camera position widget.";
        return;
    }

    w->blockSignals(true);

    auto values = arg.split_value("[#1#]");
    std::array<Arg, 7> args;
    args[0].init_from_double_str(values[0]);    // time
    args[1].init_from_bool_str(values[1]);      // display
    args[2].init_from_bool_str(values[2]);      // inter
    args[3].init_from_vector3_str(values[3]);   // trans
    args[4].init_from_vector3_str(values[4]);   // rot
    args[5].init_from_color_str(values[5]);     // color
    args[6].init_from_curve_str(values[6], " ");// speed

    time.update_from_arg(args[0]);
    displayCamera.update_from_arg(args[1]);
    sphericInterpolation.update_from_arg(args[2]);
    translation.update_from_arg(args[3]);
    rotation.update_from_arg(args[4]);
    color.update_from_arg(args[5]);
    speed.update_from_arg(args[6]);

    w->blockSignals(false);
}


Arg ExCameraTargetW::convert_to_arg() const{

    Arg arg = ExBaseW::convert_to_arg();
    std::vector<Arg> args = {
        time.convert_to_arg(),
        displayCamera.convert_to_arg(),
        sphericInterpolation.convert_to_arg(),
        translation.convert_to_arg(),
        rotation.convert_to_arg(),
        color.convert_to_arg(),
        speed.convert_to_arg(),
    };
    arg.init_from_args(args, "[#1#]");

    return arg;
}

void ExCameraTargetW::set_as_generator(){
    ExBaseW::set_as_generator();
    displayCamera.set_as_generator();
    sphericInterpolation.set_as_generator();
    time.set_as_generator();
    translation.set_as_generator();
    rotation.set_as_generator();
    color.set_as_generator();
    speed.set_as_generator();
}


