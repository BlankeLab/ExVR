
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
//#include "gui/ex_widgets/ex_combo_box_index_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"
//#include "gui/ex_widgets/ex_line_edit_w.hpp"
//#include "gui/ex_widgets/ex_spin_box_w.hpp"
#include "gui/ex_widgets/ex_label_w.hpp"

// local
#include "gui/ex_widgets/ex_resource_w.hpp"

using namespace tool::ex;

struct K4ManagerInitConfigParametersW::Impl{
    ExResourceW networkS{"network_settings"};
    ExResourceW deviceS{"device_settings"};
    ExResourceW colorS{"color_settings"};
    ExResourceW filters{"filters"};
    ExResourceW models{"models"};
//    ExResourceW calib{"calib"};
//    ExResourceW camera{"camera"};
//    ExComboBoxIndexW mode{"mode"};
//    ExLineEditW camarasToUse{"cameras_to_use"};
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
    add_widget(ui::F::gen(ui::L::VB(),
        {m_p->networkS(), ui::W::txt("Optional:"),m_p->deviceS(),m_p->colorS(),m_p->filters(),m_p->models()}, LStretch{false}, LMargins{true}, QFrame::Box)
    );
//    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Cameras mode:"), m_p->mode()}, LStretch{false}, LMargins{true}, QFrame::NoFrame));
//    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Grabbers id to use (ex:\"0;1;2\"):"), m_p->camarasToUse()}, LStretch{false}, LMargins{true}, QFrame::NoFrame));
    add_widget(m_p->debugBypassDevice());
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("Infos:"), m_p->infos()}, LStretch{false}, LMargins{true}, QFrame::Box));
}

void K4ManagerInitConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_p->networkS.init_widget(Resource::Type::Text, "Network settings file: "));
    add_input_ui(m_p->deviceS.init_widget(Resource::Type::Text, "Device settings file: "));
    add_input_ui(m_p->colorS.init_widget(Resource::Type::Text, "Color settings file: "));
    add_input_ui(m_p->filters.init_widget(Resource::Type::Text, "Filters file: "));
    add_input_ui(m_p->models.init_widget(Resource::Type::Text, "Models file: "));

//    add_input_ui(m_p->mode.init_widget({"Cloud", "Mesh"}, 0));
//    add_input_ui(m_p->camarasToUse.init_widget("0 1 2 3 4 5 6 7"));
    add_input_ui(m_p->debugBypassDevice.init_widget("Enable it for testing the experiment without the device", false));
//    //    add_input_ui(m_p->disableDisplayOnGrabber.init_widget("Disable display on grabber", true));
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
    }else if(id == QSL("grabber_infos")){
        auto split = value.split('%');

        if(split.size() >= 2){
            auto id = split[0].toInt();
            while(id <= m_p->grabbersInfos.size()){
                m_p->grabbersInfos.append("");
            }
            if(split[1].toInt() == 1){
                m_p->grabbersInfos[id] = QString("Grabber [%1] connected.");
            }else{
                m_p->grabbersInfos[id] = QString("Grabber [%1] not connected.");
            }

            m_p->infos.w->setText(m_p->configInfos % QSL("\n") % m_p->grabbersInfos.join("\n"));
        }
    }
}

struct K4ManagerConfigParametersW::Impl{
//    ExSpinBoxW fps{"fps"};
//    ExCheckBoxW updateFromCameras{"update_cameras"};
//    ExSpinBoxW maxDiffTime{"max_diff_time"};
//    ExLabelW infos{"infos"};
//    std::vector<QString> infosStr;
};

K4ManagerConfigParametersW::K4ManagerConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void K4ManagerConfigParametersW::insert_widgets(){
//    add_widget(ui::F::gen(ui::L::HB(),{m_p->updateFromCameras(), ui::W::txt("Camera ask frame rate: "), m_p->fps()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
//    add_widget(ui::F::gen(ui::L::HB(),{ui::W::txt("Max diff time(ms): "), m_p->maxDiffTime()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
//    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("Infos:"), m_p->infos()}, LStretch{false}, LMargins{true}, QFrame::Box));
}

void K4ManagerConfigParametersW::init_and_register_widgets(){
//    add_input_ui(m_p->updateFromCameras.init_widget("Update cameras ", true));
//    add_input_ui(m_p->fps.init_widget(MinV<int>{1}, V<int>{45}, MaxV<int>{90}, StepV<int>{1}));
//    add_input_ui(m_p->maxDiffTime.init_widget(MinV<int>{1}, V<int>{100}, MaxV<int>{1000}, StepV<int>{1}));
}

void K4ManagerConfigParametersW::update_with_info(QStringView id, QStringView value){
//    if(id == QSL("frame_info")){
//        auto split = value.split('%');
//        if(split.size() >= 5){
//            auto id = split[0].toInt();
//            if(id >= m_p->infosStr.size()){
//                m_p->infosStr.resize(id+1);
//            }

//            m_p->infosStr[id] = QString("Grabber id: [%1], frame id: [%2], diff time: [%3], nb pts: [%4], nb tris: [%5]\n").arg(
//                split[0], split[1], split[2], split[3], split[4]
//                );

//            QString infos;
//            for(const auto &info : m_p->infosStr){
//                infos += info;
//            }
//            m_p->infos.w->setText(infos);
//        }
//    }
}
