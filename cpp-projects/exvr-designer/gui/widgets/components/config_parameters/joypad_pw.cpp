
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

#include "joypad_pw.hpp"

// base
#include "input/joypad.hpp"

// qt-utility
#include "gui/ex_widgets/ex_list_labels_w.hpp"
#include "gui/ex_widgets/ex_line_edit_w.hpp"
#include "gui/ex_widgets/ex_color_frame_w.hpp"
#include "gui/ex_widgets/ex_double_spin_box_w.hpp"

using namespace tool::ex;

struct JoypadInitConfigParametersW::Impl{

    ExListLabelsW devicesLL;

    std::map<input::Joypad::Axis, std::unique_ptr<ExLineEditW>> axesP1;
    std::map<input::Joypad::Axis, std::unique_ptr<ExLineEditW>> axesP2;
    std::map<input::Joypad::Axis, std::unique_ptr<ExDoubleSpinBoxW>> deadAxesP1;
    std::map<input::Joypad::Axis, std::unique_ptr<ExDoubleSpinBoxW>> deadAxesP2;

    std::map<input::Joypad::Button, std::unique_ptr<ExColorFrameW>> buttonsP1;
    std::map<input::Joypad::Button, std::unique_ptr<ExColorFrameW>> buttonsP2;

    std::vector<QString> axisNames;
    std::vector<ExLineEditW*> axes1Le;
    std::vector<ExLineEditW*> axes2Le;
    std::vector<ExDoubleSpinBoxW*> axes1Dead;
    std::vector<ExDoubleSpinBoxW*> axes2Dead;

    std::vector<QString> buttonsNames;
    std::vector<ExColorFrameW*> buttons1Cf;
    std::vector<ExColorFrameW*> buttons2Cf;
};

JoypadInitConfigParametersW::JoypadInitConfigParametersW() : ConfigParametersW(), m_p(std::make_unique<Impl>()){

    const auto axes1 = input::Joypad::axes_from_player(1);
    const auto axes2 = input::Joypad::axes_from_player(2);
    const DsbSettings deadS = {
        MinV<qreal>{-0.},
        V<qreal>{0.10},
        MaxV<qreal>{1.},
        StepV<qreal>{0.01},
        2
    };
    for(size_t ii = 0; ii < axes1.size(); ++ii){

        auto le1 = std::make_unique<ExLineEditW>();
        m_p->axes1Le.emplace_back(le1.get());
        le1->init_widget(QSL("0"), false)->init_tooltip(QSL("Joypad axis code: <b>") %  QString::number(input::Joypad::get_code(axes1[ii])) % QSL("</b>"));
        m_p->axesP1[axes1[ii]] = std::move(le1);

        auto de1 = std::make_unique<ExDoubleSpinBoxW>(QSL("dead_zone_") % QString::number(input::Joypad::get_code(axes1[ii])));
        m_p->axes1Dead.emplace_back(de1.get());
        de1->init_widget(deadS);
        m_p->deadAxesP1[axes1[ii]] = std::move(de1);

        auto le2 = std::make_unique<ExLineEditW>();
        m_p->axes2Le.emplace_back(le2.get());
        le2->init_widget(QSL("0"), false)->init_tooltip(QSL("Joypad axis code: <b>") %  QString::number(input::Joypad::get_code(axes2[ii])) % QSL("</b>"));
        m_p->axesP2[axes2[ii]] = std::move(le2);

        auto de2 = std::make_unique<ExDoubleSpinBoxW>(QSL("dead_zone_") % QString::number(input::Joypad::get_code(axes2[ii])));
        m_p->axes2Dead.emplace_back(de2.get());
        de2->init_widget(deadS);
        m_p->deadAxesP2[axes2[ii]] = std::move(de2);

        m_p->axisNames.emplace_back(from_view(input::Joypad::get_name(axes1[ii])).split("_")[0]);
    }

    const auto buttons1 = input::Joypad::buttons_from_player(1);
    const auto buttons2 = input::Joypad::buttons_from_player(2);
    for(size_t ii = 0; ii < buttons1.size(); ++ii){

        auto cf1 = std::make_unique<ExColorFrameW>();
        m_p->buttons1Cf.emplace_back(cf1.get());
        cf1->init_widget(false, false)->init_tooltip(QSL("Joypad button code: <b>") %  QString::number(input::Joypad::get_code(buttons1[ii])) % QSL("</b>"));
        m_p->buttonsP1[buttons1[ii]] = std::move(cf1);

        auto cf2 = std::make_unique<ExColorFrameW>();
        m_p->buttons2Cf.emplace_back(cf2.get());
        cf2->init_widget(false, false)->init_tooltip(QSL("Joypad button code: <b>") %  QString::number(input::Joypad::get_code(buttons2[ii])) % QSL("</b>"));
        m_p->buttonsP2[buttons2[ii]] = std::move(cf2);

        m_p->buttonsNames.emplace_back(from_view(input::Joypad::get_name(buttons1[ii])).split("_")[0]);
    }
}

void JoypadInitConfigParametersW::insert_widgets(){

    // insert ui elements
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("<b>Controllers detected:</b>"),m_p->devicesLL()}, LStretch{false}, LMargins{true}, QFrame::Box));

    QFrame *allAxis = ui::F::gen(ui::L::G(), {}, LStretch{true}, LMargins{true}, QFrame::Box);
    QGridLayout *gl = dynamic_cast<QGridLayout*>(allAxis->layout());
    gl->addWidget(ui::W::txt("<b>Axis</b>"),           0, 0, 1, 1);
    gl->addWidget(ui::W::txt("<b>Value J1</b>"),       0, 1, 1, 1);
    gl->addWidget(ui::W::txt("<b>Dead zone J1</b>"),   0, 2, 1, 1);
    gl->addWidget(ui::W::txt("<b>Value J2</b>"),       0, 3, 1, 1);
    gl->addWidget(ui::W::txt("<b>Dead zone J2</b>"),   0, 4, 1, 1);

    for(size_t ii = 0; ii < m_p->axisNames.size(); ++ii){
        gl->addWidget(ui::W::txt(m_p->axisNames[ii]),    to_int(ii + 1), 0, 1, 1);
        gl->addWidget(m_p->axes1Le[ii]->w.get(),         to_int(ii + 1), 1, 1, 1);
        gl->addWidget(m_p->axes1Dead[ii]->w.get(),       to_int(ii + 1), 2, 1, 1);
        gl->addWidget(m_p->axes2Le[ii]->w.get(),         to_int(ii + 1), 3, 1, 1);
        gl->addWidget(m_p->axes2Dead[ii]->w.get(),       to_int(ii + 1), 4, 1, 1);
    }


    QFrame *allButtons = ui::F::gen(ui::L::G(), {}, LStretch{true}, LMargins{true}, QFrame::Box);
    gl = dynamic_cast<QGridLayout*>(allButtons->layout());
    gl->addWidget(ui::W::txt("<b>Buttons</b>"), 0, 0, 1, 1);
    gl->addWidget(ui::W::txt("<b>J1</b>"),      0, 1, 1, 1);
    gl->addWidget(ui::W::txt("<b>J2</b>"),      0, 2, 1, 1);

    for(size_t ii = 0; ii < m_p->buttonsNames.size(); ++ii){
        gl->addWidget(ui::W::txt(m_p->buttonsNames[ii]), to_int(ii + 1), 0, 1, 1);
        gl->addWidget(m_p->buttons1Cf[ii]->w.get(),      to_int(ii + 1), 1, 1, 1);
        gl->addWidget(m_p->buttons2Cf[ii]->w.get(),      to_int(ii + 1), 2, 1, 1);
    }

    add_widget(ui::F::gen(ui::L::HB(), {allAxis,allButtons}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    no_end_stretch();
}

void JoypadInitConfigParametersW::init_and_register_widgets(){

    m_p->devicesLL.init_widget(false);

    for(const auto &dz : m_p->axes1Dead){
        add_input_ui(dz);
    }
    for(const auto &dz : m_p->axes2Dead){
        add_input_ui(dz);
    }
}

void JoypadInitConfigParametersW::update_with_info(QStringView id, QStringView value){

    if(id == QSL("axes_state_info")){

        for(auto &axisUI : m_p->axesP1){
            axisUI.second->w->setText("0.00");
        }
        for(auto &axisUI : m_p->axesP2){
            axisUI.second->w->setText("0.00");
        }

        for(auto split : value.split('%')){

            if(split.length() == 0){
                break;
            }

            const auto subSplit   = split.split(',');
            auto axis             = input::Joypad::get_axis(subSplit[0].toInt());
            const auto value      = subSplit[1].toString();

            if(axis.has_value()){
                if(input::Joypad::get_player(axis.value()) == 1){
                    m_p->axesP1[axis.value()]->w->setText(value);
                }else{
                    m_p->axesP2[axis.value()]->w->setText(value);
                }
            }
        }

    }else if(id == QSL("buttons_state_info")){


        for(auto &buttonUI : m_p->buttonsP1){
            buttonUI.second->update(false);
        }
        for(auto &buttonUI : m_p->buttonsP2){
            buttonUI.second->update(false);
        }

        for(auto split : value.split('%')){

            if(split.length() == 0){
                break;
            }

            const auto subSplit   = split.split(',');
            auto button           = input::Joypad::get_button(subSplit[0].toInt());
            if(button.has_value()){
                const auto state      = subSplit[1].toString() == "1";
                if(input::Joypad::get_player(button.value()) == 1){
                    m_p->buttonsP1[button.value()]->update(state);
                }else{
                    m_p->buttonsP2[button.value()]->update(state);
                }
            }
        }

    }else if(id == QSL("joysticks_info")){
        m_p->devicesLL.w->clear();
        int id = 0;
        for(const auto &split : value.split('%')){
            if(split.length() == 0){
                break;
            }
            m_p->devicesLL.w->addItem(QSL("J") % QString::number(id++) % QSL(" ") % split);
        }
    }
}

