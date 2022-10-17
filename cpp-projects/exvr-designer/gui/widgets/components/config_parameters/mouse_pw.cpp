
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

#include "mouse_pw.hpp"

// base
#include "input/mouse.hpp"

using namespace tool::ex;

struct MouseInitConfigParametersW::Impl{
    QComboBox keys;
    QLineEdit code;
    QSpinBox value;
    QLineEdit lastKeys;
    QLineEdit mousePos;
};

MouseInitConfigParametersW::MouseInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}


void MouseInitConfigParametersW::insert_widgets(){

    QStringList keysList;
    for(const auto &buttonName : input::Mouse::buttons.tuple_column<1>()){
        keysList << from_view(buttonName);
    }
    m_p->keys.addItems(keysList);

    auto l1 = ui::F::gen(ui::L::HB(), {ui::W::txt("Mouse buttons")}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    auto l2 = ui::F::gen(ui::L::HB(), {ui::W::txt("Name:"), &m_p->keys}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    auto l3 = ui::F::gen(ui::L::HB(), {ui::W::txt("C# key code:"), &m_p->code}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    auto l4 = ui::F::gen(ui::L::HB(), {ui::W::txt("Value:"), &m_p->value}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    auto l5 = ui::F::gen(ui::L::VB(), {ui::W::txt("Last buttons pressed:"), &m_p->lastKeys}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    auto l6 = ui::F::gen(ui::L::VB(), {ui::W::txt("Mouse position:"), &m_p->mousePos}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    add_widget(ui::F::gen(ui::L::VB(), {l1,l2,l3,l4,l5, l6}, LStretch{true}, LMargins{false}, QFrame::Box));
    ui::L::stretch(layout());

}

void MouseInitConfigParametersW::init_and_register_widgets(){
    m_p->code.setReadOnly(true);
    m_p->value.setReadOnly(true);
    m_p->value.setMaximum(1000);
    m_p->lastKeys.setReadOnly(true);
    m_p->mousePos.setReadOnly(true);
}

void MouseInitConfigParametersW::create_connections(){
    connect(&m_p->keys, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&]{
        auto button = input::Mouse::get_button(m_p->keys.currentText().toStdString());
        if(button.has_value()){
            m_p->value.setValue(static_cast<int>(button.value()));
        }
        m_p->code.setText("KeyCode." + m_p->keys.currentText());
    });
}

void MouseInitConfigParametersW::late_update_ui(){
    emit m_p->keys.currentIndexChanged(0);
}

void MouseInitConfigParametersW::update_with_info(QStringView id, QStringView value){

    if(id == QSL("buttons_state_info")){
        QString keys;
        for(const auto &split : value.split(',')){
            auto buttonName = input::Mouse::get_name(split.toInt());
            if(buttonName.has_value()){
                keys += from_view(buttonName.value()) % QSL(" ");
            }
        }
        m_p->lastKeys.setText(keys);
    }else if (id == QSL("axis_state_info")){

    }else if (id == QSL("position_state_info")){
        auto split = value.split(',');
        m_p->mousePos.setText(QSL("(") % split[0] % QSL(",") % split[1] % QSL(")"));
    }
}
