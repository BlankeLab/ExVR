
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

#include "keyboard_pw.hpp"

// base
#include "input/keyboard.hpp"

// qt-utility
#include "gui/ex_widgets/ex_line_edit_w.hpp"
#include "gui/ex_widgets/ex_combo_box_text_w.hpp"
#include "gui/ex_widgets/ex_spin_box_w.hpp"

using namespace tool::ex;

struct KeyboardInitConfigParametersW::Impl{
    ExComboBoxTextW keys;
    ExLineEditW code;
    ExSpinBoxW value;
    ExLineEditW lastKeys;
};

KeyboardInitConfigParametersW::KeyboardInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void KeyboardInitConfigParametersW::insert_widgets(){

    QStringList keysList;
    for(const auto &buttonName : input::Keyboard::buttons.tuple_column<1>()){
        keysList << from_view(buttonName);
    }
    m_p->keys.w->addItems(keysList);

    auto l2 = ui::F::gen(ui::L::HB(), {ui::W::txt("Keyboard buttons names:"), m_p->keys()}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    auto l3 = ui::F::gen(ui::L::HB(), {ui::W::txt("C# key code:"), m_p->code()}, LStretch{false}, LMargins{true}, QFrame::NoFrame);
    auto l4 = ui::F::gen(ui::L::HB(), {ui::W::txt("Value:"), m_p->value()}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    auto l5 = ui::F::gen(ui::L::VB(), {ui::W::txt("Last buttons pressed:"), m_p->lastKeys()}, LStretch{true}, LMargins{true}, QFrame::NoFrame);
    add_widget(ui::F::gen(ui::L::VB(), {l2,l3,l4,l5}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
}

void KeyboardInitConfigParametersW::init_and_register_widgets(){
    m_p->code.w->setReadOnly(true);
    m_p->value.w->setReadOnly(true);
    m_p->value.w->setMaximum(1000);
    m_p->lastKeys.w->setReadOnly(true);
}

void KeyboardInitConfigParametersW::create_connections(){
    connect(m_p->keys.w.get(), QOverload<int>::of(&QComboBox::currentIndexChanged), this, [&]{
        auto button = input::Keyboard::get_button(m_p->keys.w->currentText().toStdString());
        if(button.has_value()){
            m_p->value.w->setValue(static_cast<int>(button.value()));
        }
        m_p->code.w->setText(QSL("KeyCode.") % m_p->keys.w->currentText());
    });
}

void KeyboardInitConfigParametersW::late_update_ui(){
    emit m_p->keys.w->currentIndexChanged(0);
}

void KeyboardInitConfigParametersW::update_with_info(QStringView id, QStringView value){

    if(id == QSL("buttons_state_info")){

        QString keys;
        for(const auto &split : value.split(' ')){
            if(split.length() > 0){
                auto buttonName = input::Keyboard::get_name(split.toInt());
                if(buttonName.has_value()){
                    keys += from_view(buttonName.value()) % QSL(" ");
                }
            }
        }
        m_p->lastKeys.w->setText(keys);
    }
}
