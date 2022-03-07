
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

#include "serial_port_writer_pw.hpp"

// qt-utility
#include "ex_widgets/ex_line_edit_w.hpp"
#include "ex_widgets/ex_spin_box_w.hpp"
#include "ex_widgets/ex_radio_button_w.hpp"
#include "ex_widgets/ex_checkbox_w.hpp"
#include "ex_widgets/ex_text_edit_w.hpp"
#include "ex_widgets/ex_double_spin_box_w.hpp"

using namespace tool::ex;

struct SerialPortWriterInitParameterW::Impl{
    ExLineEditW leWritingPort{"port_to_write"};
    ExSpinBoxW sbBaudRate{"baud_rate"};
};

SerialPortWriterInitParameterW::SerialPortWriterInitParameterW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void SerialPortWriterInitParameterW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Writing port:"), m_p->leWritingPort()}, LStretch{true}, LMargins{false}));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Baud rate:"), m_p->sbBaudRate()}, LStretch{true}, LMargins{false}));
}

void SerialPortWriterInitParameterW::init_and_register_widgets(){
    add_input_ui(m_p->leWritingPort.init_widget("COM1"));
    add_input_ui(m_p->sbBaudRate.init_widget(MinV<int>{0}, V<int>{9600}, MaxV<int>{100000000}, StepV<int>{100}));
}


struct SerialPortWriterConfigParametersW::Impl{

    QButtonGroup group1;
    ExRadioButtonW rbBitsMessage{"bits_mode"};
    ExRadioButtonW rbIntMessage{"int_mode"};
    ExRadioButtonW rbStringMessage{"string_mode"};

    ExTextEditW teMessage{"message"};
    ExCheckBoxW cbSendWhendRoutineStarts{"send_new_routine"};
    ExCheckBoxW cbSendWhenNewTimelineBlock{"send_new_update_block"};
    ExCheckBoxW cbSendWhenEndTimelineBlock{"send_end_update_block"};
    ExCheckBoxW cbSendEveryFrame{"send_every_frame"};

    ExDoubleSpinBoxW pulseTime{"pulse_time"};
};

SerialPortWriterConfigParametersW::SerialPortWriterConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void SerialPortWriterConfigParametersW::insert_widgets(){

    add_widget(ui::F::gen(ui::L::VB(), {
        ui::W::txt("Send message:"),
        m_p->cbSendWhendRoutineStarts(),
        m_p->cbSendWhenNewTimelineBlock(),
        m_p->cbSendWhenEndTimelineBlock(),
        m_p->cbSendEveryFrame()}, LStretch{true}, LMargins{false} )
    );
    add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Pulse time:"), m_p->pulseTime()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
    add_widget(ui::W::horizontal_line());
    add_widget(ui::F::gen(ui::L::VB(), {m_p->rbBitsMessage(), m_p->rbIntMessage(), m_p->rbStringMessage()}, LStretch{true}, LMargins{false}));
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("Message to sent:"),  m_p->teMessage()}, LStretch{false}, LMargins{false}));

    set_stretch(0,1);
    set_stretch(1,1);
    set_stretch(2,1);
    set_stretch(3,1);
    set_stretch(4,50);

    no_end_stretch();
}

void SerialPortWriterConfigParametersW::init_and_register_widgets(){

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
        {&m_p->rbBitsMessage, &m_p->rbIntMessage, &m_p->rbStringMessage},
        {
            "Bits message (ex: 0101_0100 0101_0110...)",
            "Integers message (ex: 254 13 0 255...)",
            "String message (ex: blabla...)"
        },
        {
            false, false, true}
        )
    );

    add_input_ui(m_p->teMessage.init_widget(""));
    add_input_ui(m_p->cbSendWhendRoutineStarts.init_widget("... when routine starts ", false));
    add_input_ui(m_p->cbSendWhenNewTimelineBlock.init_widget("... when new update timeline block starts ", false));
    add_input_ui(m_p->cbSendWhenEndTimelineBlock.init_widget("... when update timeline block ends", false));
    add_input_ui(m_p->cbSendEveryFrame.init_widget("... at every frame", false));
    add_input_ui(m_p->pulseTime.init_widget(MinV<qreal>{0.00}, V<qreal>{1.0}, MaxV<qreal>{10.}, StepV<qreal>{0.01}, 2));
}

