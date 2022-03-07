
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

// local
#include "config_pw.hpp"

namespace tool::ex {

class UdpWriterInitConfigParametersW : public ConfigParametersW{

public :

    ExSpinBoxW m_sbWritingPort{"writing_port"};
    ExLineEditW m_leWritingIpAdress{"writing_address"};

    void insert_widgets() override{
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Writing port:"), m_sbWritingPort()}, LStretch{true}, LMargins{false}));
        add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Writing IP adress (or hostname):"), m_leWritingIpAdress()}, LStretch{true}, LMargins{false}));
    }

    void init_and_register_widgets() override{        
        add_input_ui(m_sbWritingPort.init_widget( MinV<int>{0}, V<int>{8060}, MaxV<int>{100000}, StepV<int>{1}));
        add_input_ui(m_leWritingIpAdress.init_widget("127.0.0.1"));
    }

    void create_connections() override{}
    void late_update_ui() override{}
};


class UdpWriterConfigParametersW : public ConfigParametersW{

public :

    void insert_widgets() override{}
    void init_and_register_widgets() override{}
    void create_connections() override{}
    void late_update_ui() override{}
};

}
