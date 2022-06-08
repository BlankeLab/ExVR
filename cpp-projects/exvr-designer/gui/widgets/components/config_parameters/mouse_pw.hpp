
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

// base
#include "input/mouse.hpp"

// local
#include "config_pw.hpp"

namespace tool::ex {



class MouseInitConfigParametersW : public ConfigParametersW{

    QComboBox m_keys;
    QLineEdit m_code;
    QSpinBox m_value;
    QLineEdit m_lastKeys;


public :

    void insert_widgets() override{

        QStringList keysList;
        for(const auto &buttonName : input::Mouse::buttons.tuple_column<1>()){
            keysList << from_view(buttonName);
        }
        m_keys.addItems(keysList);

        auto l1 = ui::F::gen(ui::L::HB(), {ui::W::txt("Mouse buttons")}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
        auto l2 = ui::F::gen(ui::L::HB(), {ui::W::txt("Name:"), &m_keys}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
        auto l3 = ui::F::gen(ui::L::HB(), {ui::W::txt("C# key code:"), &m_code}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
        auto l4 = ui::F::gen(ui::L::HB(), {ui::W::txt("Value:"), &m_value}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
        auto l5 = ui::F::gen(ui::L::VB(), {ui::W::txt("Last buttons pressed:"), &m_lastKeys}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
        add_widget(ui::F::gen(ui::L::VB(), {l1,l2,l3,l4,l5}, LStretch{true}, LMargins{false}, QFrame::Box));
        ui::L::stretch(layout());

    }
    void init_and_register_widgets() override{
        m_code.setReadOnly(true);
        m_value.setReadOnly(true);
        m_value.setMaximum(1000);
        m_lastKeys.setReadOnly(true);
    }

    void create_connections() override{
        connect(&m_keys, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&]{
            auto button = input::Mouse::get_button(m_keys.currentText().toStdString());
            if(button.has_value()){
                m_value.setValue(static_cast<int>(button.value()));
            }
            m_code.setText("KeyCode." + m_keys.currentText());
        });
    }

    void late_update_ui() override{
        emit m_keys.currentIndexChanged(0);
    }


public:
    virtual void update_with_info(QStringView id, QStringView value) override{

        if(id == QSL("buttons_state_info")){
            QString keys;
            for(const auto &split : value.split(',')){
                auto buttonName = input::Mouse::get_name(split.toInt());
                if(buttonName.has_value()){
                    keys += from_view(buttonName.value()) % QSL(" ");
                }
            }
            m_lastKeys.setText(keys);
        }
    }
};


class MouseConfigParametersW : public ConfigParametersW{

public :

    void insert_widgets() override{}
    void init_and_register_widgets() override{}
    void create_connections() override{}
    void late_update_ui() override{}
};

}
