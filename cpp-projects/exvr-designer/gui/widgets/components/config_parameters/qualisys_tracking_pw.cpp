
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

#include "qualisys_tracking_pw.hpp"

void tool::ex::QualisysTrackingInitConfigParametersW::insert_widgets(){

    auto radioF = ui::F::gen(ui::L::VB(), {connectToFirstAvailable(), specifyServer()}, LStretch{false}, LMargins{true}, QFrame::NoFrame);
    auto hostNameF = ui::F::gen(ui::L::HB(), {ui::W::txt("Host name:"), hostName()}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    auto ipAdressF = ui::F::gen(ui::L::HB(), {ui::W::txt("Ip address:"), ipAdress()}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    auto serverInfosF = ui::F::gen(ui::L::VB(), {ui::W::txt("<b>Server infos:</b>"), hostNameF, hostNameF, ipAdressF}, LStretch{false}, LMargins{true}, QFrame::NoFrame);
    add_widget(ui::F::gen(ui::L::VB(), {radioF, serverInfosF}, LStretch{false}, LMargins{true}, QFrame::Box));
}

void tool::ex::QualisysTrackingInitConfigParametersW::init_and_register_widgets(){

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(group1,
            {&connectToFirstAvailable, &specifyServer},
            {
                "Connect to first server available",
                "Connect to server matching infos",
            },
            {true, false}
        )
    );

    add_input_ui(hostName.init_widget(""));
    add_input_ui(ipAdress.init_widget(""));
}

void tool::ex::QualisysTrackingInitConfigParametersW::create_connections(){

}

void tool::ex::QualisysTrackingInitConfigParametersW::late_update_ui(){}

void tool::ex::QualisysTrackingConfigParametersW::insert_widgets(){
    add_sub_part_widget(transform);
    auto objectsF = ui::F::gen(ui::L::VB(), {ui::W::txt("Objects names to track (one per line):"), objects()}, LStretch{false}, LMargins{true}, QFrame::NoFrame);
    add_widget(ui::F::gen(ui::L::VB(), {objectsF}, LStretch{false}, LMargins{true}, QFrame::Box));
}

void tool::ex::QualisysTrackingConfigParametersW::init_and_register_widgets(){
    map_sub_part(transform.init_widget());
    add_input_ui(objects.init_widget("6DOF object name 1\n6DOF object name 2", Qt::TextFormat::PlainText));
}

void tool::ex::QualisysTrackingConfigParametersW::create_connections(){

}

void tool::ex::QualisysTrackingConfigParametersW::late_update_ui(){

}
