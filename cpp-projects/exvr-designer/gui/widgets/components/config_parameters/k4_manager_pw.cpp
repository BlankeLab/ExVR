
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

#include "k4_manager_pw.hpp"

// qt-utility
#include "gui/ex_widgets/ex_radio_button_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_line_edit_w.hpp"
#include "gui/ex_widgets/ex_label_w.hpp"
#include "qt_logger.hpp"

// local
#include "gui/ex_widgets/ex_resource_w.hpp"

using namespace tool::ex;

struct K4ManagerInitConfigParametersW::Impl{
    ExResourceW networkS{"network_settings"};

    QButtonGroup deviceG;
    ExRadioButtonW deviceDefault{"device_init_default"};
    ExRadioButtonW deviceFile{"device_init_file"};
    ExRadioButtonW deviceNothing{"device_init_nothing"};
    ExResourceW deviceS{"device_settings"};

    QButtonGroup colorG;
    ExRadioButtonW colorDefault{"color_init_default"};
    ExRadioButtonW colorFile{"color_init_file"};
    ExRadioButtonW colorNothing{"color_init_nothing"};
    ExResourceW colorS{"color_settings"};

    QButtonGroup filtersG;
    ExRadioButtonW filtersDefault{"filters_init_default"};
    ExRadioButtonW filtersFile{"filters_init_file"};
    ExRadioButtonW filtersNothing{"filters_init_nothing"};
    ExResourceW filters{"filters"};

    ExResourceW models{"models"};

    ExLineEditW camarasToUse{"cameras_to_use"};
    ExCheckBoxW debugBypassDevice{"debug_bypass"};
    ExLabelW infos{"infos"};

    QString configInfos =
        "Nb connections: -\n"
        "Network file loaded: -\n"
        "Device settings file loaded: -\n"
        "Filters file loaded: -\n"
        "Color settings file loaded: -\n"
        "Models file loaded: -";
    QStringList grabbersInfos = {};
};

K4ManagerInitConfigParametersW::K4ManagerInitConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void K4ManagerInitConfigParametersW::insert_widgets(){

    auto initDevice = ui::F::gen(ui::L::HB(), {ui::W::txt("Init device settings with:"), m_p->deviceFile(),m_p->deviceDefault(),m_p->deviceNothing()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    auto initColor = ui::F::gen(ui::L::HB(), {ui::W::txt("Init color settings with:"), m_p->colorFile(),m_p->colorDefault(),m_p->colorNothing()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    auto initFilters = ui::F::gen(ui::L::HB(), {ui::W::txt("Init filters settings with:"), m_p->filtersFile(),m_p->filtersDefault(),m_p->filtersNothing()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);

    add_widget(ui::F::gen(ui::L::VB(),
        {m_p->networkS(), initDevice,initFilters, initColor,m_p->deviceS(), m_p->colorS(),m_p->filters(),m_p->models()}, LStretch{false}, LMargins{true}, QFrame::Box)
    );

    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Grabbers id to use (ex:\"0;1;2\"):"), m_p->camarasToUse()}, LStretch{false}, LMargins{true}, QFrame::NoFrame));
    add_widget(m_p->debugBypassDevice());
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("Infos:"), m_p->infos()}, LStretch{false}, LMargins{true}, QFrame::Box));
}

void K4ManagerInitConfigParametersW::init_and_register_widgets(){

    add_inputs_ui(ExRadioButtonW::init_group_widgets(m_p->deviceG, {&m_p->deviceFile, &m_p->deviceDefault, &m_p->deviceNothing}, {"file", "default", "nothing"}, {true, false, false}));
    add_inputs_ui(ExRadioButtonW::init_group_widgets(m_p->filtersG, {&m_p->filtersFile, &m_p->filtersDefault, &m_p->filtersNothing}, {"file", "default", "nothing"}, {true, false, false}));
    add_inputs_ui(ExRadioButtonW::init_group_widgets(m_p->colorG, {&m_p->colorFile, &m_p->colorDefault, &m_p->colorNothing}, {"file", "default", "nothing"}, {true, false, false}));


    add_input_ui(m_p->networkS.init_widget(Resource::Type::Text, "Network settings file: "));
    add_input_ui(m_p->deviceS.init_widget(Resource::Type::Text, "Device settings file: "));
    add_input_ui(m_p->colorS.init_widget(Resource::Type::Text, "Color settings file: "));
    add_input_ui(m_p->filters.init_widget(Resource::Type::Text, "Filters file: "));
    add_input_ui(m_p->models.init_widget(Resource::Type::Text, "Models file: "));

    add_input_ui(m_p->camarasToUse.init_widget("0;1;2;3;4"));
    add_input_ui(m_p->debugBypassDevice.init_widget("Enable it for testing the experiment without the device", false));
}

void K4ManagerInitConfigParametersW::update_with_info(QStringView id, QStringView value){
    if(id == QSL("config_infos")){
        auto split = value.split('%');
        if(split.size() >= 5){

            m_p->configInfos =  QString("Nb connections: [%1]\n"
                "Network file loaded: [%2]\n"
                "Device settings file loaded: [%3]\n"
                "Filters file loaded: [%4]\n"
                "Color settings file loaded: [%5]\n"
                "Models file loaded: [%6]").arg(
                split[0],
                split[1] == '1' ? "YES" : "NO",
                split[2] == '1' ? "YES" : "NO",
                split[3] == '1' ? "YES" : "NO",
                split[4] == '1' ? "YES" : "NO",
                split[5] == '1' ? "YES" : "NO"
            );
            m_p->infos.w->setText(m_p->configInfos % QSL("\n") % m_p->grabbersInfos.join("\n"));
        }
    }else if(id.contains(QSL("grabber_infos_"))){
        auto splitId = id.split('_');
        if(splitId.size() == 4){
            auto id    = splitId[2].toInt();
            auto count = splitId[3].toInt();
            auto connectedState = value == QSL("1");

            while(m_p->grabbersInfos.size() < count){
                m_p->grabbersInfos.append("");
            }

            m_p->grabbersInfos[id] = connectedState ? QString("Grabber [%1] connected.").arg(id) :  QString("Grabber [%1] not connected.").arg(id);
            m_p->infos.w->setText(m_p->configInfos % QSL("\n") % m_p->grabbersInfos.join("\n"));
        }
    }
}

struct K4ManagerConfigParametersW::Impl{
    ExLabelW infos{"infos"};
    QStringList frameInfos = {};
};

K4ManagerConfigParametersW::K4ManagerConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void K4ManagerConfigParametersW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("Infos:"), m_p->infos()}, LStretch{false}, LMargins{true}, QFrame::Box));
}

void K4ManagerConfigParametersW::init_and_register_widgets(){
}

void K4ManagerConfigParametersW::update_with_info(QStringView id, QStringView value){

    if(id.contains(QSL("frame_infos_"))){
        auto splitId = id.split('_');
        if(splitId.size() == 4){
            auto id    = splitId[2].toInt();
            auto count = splitId[3].toInt();

            while(m_p->frameInfos.size() < count){
                m_p->frameInfos.append("");
            }

            m_p->frameInfos[id] = QString("[%1]: [%2]").arg(id).arg(value);
            m_p->infos.w->setText(m_p->frameInfos.join("\n"));
        }
    }
}
