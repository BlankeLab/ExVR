
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
#include "ex_widgets/ex_line_edit_w.hpp"
#include "ex_widgets/ex_spin_box_w.hpp"
#include "ex_widgets/ex_float_spin_box_w.hpp"
#include "ex_widgets/ex_radio_button_w.hpp"

// local
#include "config_pw.hpp"

namespace tool::ex {

class FopRobotInitConfigParametersW : public ConfigParametersW{

public :

    ExSpinBoxW  m_readingPort{"read_port"};
    ExLineEditW m_readingAddress{"read_address"};

    ExSpinBoxW  m_writingPort{"write_port"};
    ExLineEditW m_writingAddress{"write_address"};

    void insert_widgets() override{
        add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Reading address:"), m_readingAddress()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
        add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Reading port:"),    m_readingPort()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
        add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Writing address:"), m_writingAddress()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
        add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Writing port:"),    m_writingPort()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
    }

    void init_and_register_widgets() override{

        add_input_ui(m_readingAddress.init_widget("127.0.0.1"));
        add_input_ui(m_readingPort.init_widget(MinV<int>{0},V<int>{5554}, MaxV<int>{44000}, StepV<int>{1}));
        add_input_ui(m_writingAddress.init_widget("127.0.0.1"));
        add_input_ui(m_writingPort.init_widget(MinV<int>{0},V<int>{5555}, MaxV<int>{44000}, StepV<int>{1}));
    }

    void create_connections() override{}
    void late_update_ui() override{}
};


class FopRobotConfigParametersW : public ConfigParametersW{

public :

    QButtonGroup group1;
    ExRadioButtonW m_delayControl{"delay_control"};
    ExRadioButtonW m_forceControl{"force_control"};
    ExRadioButtonW m_testTouch{"test_touch"};
    ExRadioButtonW m_positionControl{"position_control"};
    ExRadioButtonW m_none{"none"};

    ExSpinBoxW m_delay{"delay"};
    ExFloatSpinBoxW m_forceRatio{"force_ratio"};

    void insert_widgets() override{

        add_widget(ui::F::gen(ui::L::VB(),{
                ui::W::txt("Action to do:"),
                m_delayControl(),
                m_forceControl(),
                m_testTouch(),
                m_positionControl(),
                m_none()
            }, LStretch{true}, LMargins{false},QFrame::NoFrame)
        );

        add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Delay:"),       m_delay()},        LStretch{true}, LMargins{false},QFrame::NoFrame));
        add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Force ratio:"), m_forceRatio()},   LStretch{true}, LMargins{false},QFrame::NoFrame));
    }

    void init_and_register_widgets() override{

        add_inputs_ui(
            ExRadioButtonW::init_group_widgets(group1,
                {&m_delayControl, &m_forceControl, &m_testTouch, &m_positionControl, &m_none},
                {
                    "Delay control",
                    "Force control",
                    "Test touch",
                    "Position control",
                    "None",
                },
                {true, false, false, false, false}
            )
        );

        add_input_ui(m_delay.init_widget(MinV<int>{0}, V<int>{500}, MaxV<int>{5000}, StepV<int>{1}));
        add_input_ui(m_forceRatio.init_widget(MinV<qreal>{0.f}, V<qreal>{1.f}, MaxV<qreal>{10000.f}, StepV<qreal>{0.01f}, 2));

    }

    void create_connections() override{}
    void late_update_ui() override{}
};

}
