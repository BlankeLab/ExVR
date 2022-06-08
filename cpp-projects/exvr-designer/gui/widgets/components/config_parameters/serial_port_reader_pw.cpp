
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

#include "serial_port_reader_pw.hpp"


// qt-utility
#include "gui/ex_widgets/ex_line_edit_w.hpp"
#include "gui/ex_widgets/ex_spin_box_w.hpp"
#include "gui/ex_widgets/ex_radio_button_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_text_edit_w.hpp"
#include "gui/ex_widgets/ex_double_spin_box_w.hpp"

using namespace tool::ex;

struct SerialPortReaderInitParameterW::Impl{
    ExLineEditW leReaderPort{"port_to_read"};
};

SerialPortReaderInitParameterW::SerialPortReaderInitParameterW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void SerialPortReaderInitParameterW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Reader port:"), m_p->leReaderPort()}, LStretch{true}, LMargins{false}));
}

void SerialPortReaderInitParameterW::init_and_register_widgets(){
    add_input_ui(m_p->leReaderPort.init_widget("COM1"));
}


struct SerialPortReaderConfigParametersW::Impl{

    QButtonGroup group1;
    ExRadioButtonW rbIntMessage{"int_mode"};
    ExRadioButtonW rbStringMessage{"string_mode"};
};

SerialPortReaderConfigParametersW::SerialPortReaderConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void SerialPortReaderConfigParametersW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::VB(), {m_p->rbIntMessage(), m_p->rbStringMessage()}, LStretch{true}, LMargins{false}));
}


void SerialPortReaderConfigParametersW::init_and_register_widgets(){

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
        {&m_p->rbIntMessage, &m_p->rbStringMessage},
        {
            "Integers message",
            "String message"
        },
        {
            true, false}
        )
    );
}

