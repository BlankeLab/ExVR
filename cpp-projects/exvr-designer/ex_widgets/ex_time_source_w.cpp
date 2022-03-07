
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

#include "ex_time_source_w.hpp"

using namespace tool::ex;

ExTimeW::ExTimeW(QString name) : ExItemW<QWidget>(UiType::Time, name){
    connect(&typeT, &ExComboBoxTextW::ui_change_signal, this, &ExTimeW::ui_change_signal);
    connect(&modeT, &ExComboBoxTextW::ui_change_signal, this, &ExTimeW::ui_change_signal);
    connect(&frequency, &ExSpinBoxW::ui_change_signal, this, &ExTimeW::ui_change_signal);
}

ExTimeW *ExTimeW::init_widget(bool enabled){


    typeT.init_widget({"Time since exp", "Time since routine"}, Index{0}, enabled);
    frequency.init_widget(MinV<int>{1}, V<int>{30}, MaxV<int>{90}, StepV<int>{1}, enabled);

    modeT.init_widget({"When triggered", "At each frame"}, Index{0}, enabled);

    auto l = ui::L::VB();
    l->setContentsMargins(1,1,1,1);
    w->setLayout(l);

    auto h1 = ui::L::VB();
    h1->setContentsMargins(1,1,1,1);
    h1->addWidget(ui::W::txt("Type: "));
    h1->addWidget(typeT());
    h1->addWidget(ui::W::txt("Mode: "));
    h1->addWidget(modeT());

    auto w1 = new QWidget();
    w1->setLayout(h1);

    auto h2 = ui::L::VB();
    h2->setContentsMargins(1,1,1,1);
    h2->addWidget(ui::W::txt("Max frequency (FPS): "));
    h2->addWidget(frequency());
    auto w2 = new QWidget();
    w2->setLayout(h2);

    l->addWidget(w1);
    l->addWidget(w2);

    return this;
}

void ExTimeW::update_from_arg(const Arg &arg){

    ExItemW::update_from_arg(arg);

    auto split = arg.value().split("%");
    if(split.size() < 3){
        return;
    }

    w->blockSignals(true);

    Arg typeA = arg;
    Arg freqA = arg;
    Arg modeA = arg;

    typeA.init_from(split[0]);
    freqA.init_from_int_str(split[1]);
    modeA.init_from(split[2]);

    typeT.update_from_arg(typeA);
    frequency.update_from_arg(freqA);
    modeT.update_from_arg(modeA);

    w->blockSignals(false);
}

Arg ExTimeW::convert_to_arg() const{

    auto typeA = typeT.convert_to_arg();
    auto freqA = frequency.convert_to_arg();
    auto modeA = modeT.convert_to_arg();

    QStringList list;
    list << std::move(typeA.value());
    list << std::move(freqA.value());
    list << std::move(modeA.value());

    auto arg = ExItemW::convert_to_arg();
    arg.init_from(list, "%");
    return arg;
}

void ExTimeW::set_as_generator(){
    ExBaseW::set_as_generator();
    typeT.set_as_generator();
    frequency.set_as_generator();
    modeT.set_as_generator();
}
