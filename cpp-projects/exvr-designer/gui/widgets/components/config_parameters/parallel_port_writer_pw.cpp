
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


#include "parallel_port_writer_pw.hpp"

// qt-utility
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_double_spin_box_w.hpp"
#include "gui/ex_widgets/ex_spin_box_w.hpp"
#include "gui/ex_widgets/ex_line_edit_w.hpp"

using namespace tool::ex;

struct ParallelPortWriterInitConfigParametersW::Impl{
    ExCheckBoxW int16Mode{"int16_mode"};
};

struct ParallelPortWriterConfigParametersW::Impl{
    ExLineEditW port{"port"};
    ExDoubleSpinBoxW pulseTime{"pulse_time"};
};


ParallelPortWriterInitConfigParametersW::ParallelPortWriterInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void ParallelPortWriterInitConfigParametersW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::VB(),{m_p->int16Mode()}, LStretch{false}, LMargins{true},QFrame::NoFrame));
}

void ParallelPortWriterInitConfigParametersW::init_and_register_widgets(){
    add_input_ui(m_p->int16Mode.init_widget("Use 16 bits integers as sending values", false));
}

ParallelPortWriterConfigParametersW::ParallelPortWriterConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void ParallelPortWriterConfigParametersW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Port (hex): "), m_p->port()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Pulse time: "), m_p->pulseTime()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
}

void ParallelPortWriterConfigParametersW::init_and_register_widgets(){
    add_input_ui(m_p->port.init_widget("0x378"));
    add_input_ui(m_p->pulseTime.init_widget(MinV<qreal>{0.00}, V<qreal>{1.0}, MaxV<qreal>{10.}, StepV<qreal>{0.01}, 2));
}
