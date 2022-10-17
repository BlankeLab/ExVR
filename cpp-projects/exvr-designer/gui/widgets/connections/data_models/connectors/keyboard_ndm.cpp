
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

#include "keyboard_ndm.hpp"

// base
#include "input/keyboard.hpp"

// qt-utility
#include "qt_logger.hpp"

using namespace tool::ex;


ExKeyboardButtonTrigger::ExKeyboardButtonTrigger(QString name) : ExItemW<QWidget>(UiType::KeyboardButtonTrigger, name){
    connect(&timeMode, &ExComboBoxTextW::ui_change_signal, this, &ExKeyboardButtonTrigger::ui_change_signal);
    connect(&keys,     &ExComboBoxTextW::ui_change_signal, this, &ExKeyboardButtonTrigger::ui_change_signal);
}

ExKeyboardButtonTrigger *ExKeyboardButtonTrigger::init_widget(bool enabled){

    QStringList items;
    for(const auto &keyName : input::Keyboard::buttons.tuple_column<1>()){
        items << from_view(keyName);
    }
    keys.init_widget(items, Index{0}, enabled);

    timeMode.init_widget({"Time since exp", "Time since routine"}, Index{0}, enabled);

    auto l = ui::L::VB();
    l->setContentsMargins(1,1,1,1);
    w->setLayout(l);

    l->setContentsMargins(1,1,1,1);
    l->addWidget(ui::W::txt("Key to check: "));
    l->addWidget(keys());

    l->addWidget(ui::W::txt("Time mode: "));
    l->addWidget(timeMode());

    return this;
}

void ExKeyboardButtonTrigger::update_from_arg(const Arg &arg){

    ExItemW::update_from_arg(arg);

    if(arg.dimensions_nb() == 0){
        // old format
        keys.update_from_arg(arg);
    }else{
        auto args = arg.split_value_to_atoms_args();
        if(args.size() == 2){
            keys.update_from_arg(args[0]);
            timeMode.update_from_arg(args[1]);
        }else{
            QtLogger::error("ExKeyboardButtonTrigger::Invalid arg.");
        }
    }
}

Arg ExKeyboardButtonTrigger::convert_to_arg() const{

    auto arg = ExItemW::convert_to_arg();
    arg.init_from(
        {
            keys.convert_to_arg().value(),
            timeMode.convert_to_arg().value(),
        },
        "%"
    );

    return arg;
}


void ExKeyboardButtonTriggerEmbeddedW::initialize(){

    w->init_widget();

    // set widget connections
    connect(w.get(), &ExKeyboardButtonTrigger::ui_change_signal, this, [=]{
        auto button = input::Keyboard::get_button(w->keys.w->currentText().toStdString());
        if(button.has_value()){
            auto v1 = std::make_shared<IntData>(static_cast<int>(button.value()));
            emit update_internal_data_signal({0}, {v1});
            emit compute_data_signal();
        }
    });

    // add widget to ui
    add_row_in_dialog("", w->w.get());
}

QString ExKeyboardButtonTriggerEmbeddedW::button(){
    return w->keys.w->currentText();
}


void KeyboardButtonEmbeddedW::initialize(){

    // init widget
    QStringList items;
    for(const auto &keyName : input::Keyboard::buttons.tuple_column<1>()){
        items << from_view(keyName);
    }
    w->init_widget(items);

    // set widget connections
    connect(w.get(), &ExComboBoxTextW::ui_change_signal, this, [=]{
        auto button = input::Keyboard::get_button(w->w->currentText().toStdString());
        if(button.has_value()){
            auto v1 = std::make_shared<IntData>(static_cast<int>(button.value()));
            emit update_internal_data_signal({0}, {v1});
            emit compute_data_signal();
        }
    });

    // add widget to ui
    add_row_in_dialog(QSL("Keyboard button: "), w->w.get());
}

void CheckKeyboardButtonNodeDataModel::compute(){

    if(check_infinity_loop()){
        return;
    }

    if(embedded_w()->w->timeMode.w->currentIndex() == 0){
        m_caption = from_view(Connector::get_caption(m_type)) % QSL(" (Exp time)");
    }else{
        m_caption = from_view(Connector::get_caption(m_type)) % QSL(" (Routine time)");
    }

    auto inputs = get_inputs();

    // no inputs
    if(!has_inputs(inputs)){
        set_embedded_widget_text(embedded_w()->button());
        set_invalid_state(QSL("Missing 1 entree."));
        return;
    }

    // runtime inputs
    if(check_if_runtime_inputs(inputs)){
        propagate_default_runtime(
            {
                std::make_shared<RealData>(),
                std::make_shared<RealData>(),
                std::make_shared<RealData>()
            });
        set_embedded_widget_text(embedded_w()->button());
        return;
    }

    // cast
    auto data = dcast<KeyboardButtonEventData>(inputs[0]);
    if(!data){
        set_invalid_cast();
        return;
    }

    auto interD = dcast<KeyboardButtonEventData>(interData[0]);
    if(data->value().code == interD->value().code){

        // propagate
        bool down    = data->value().firstFrameDown;
        bool up      = data->value().firstFrameUp;
        bool pressed = data->value().pressed;

        std::vector<size_t> indices;
        std::vector<std::shared_ptr<BaseNodeData>> outputs;
        if(down){
            indices.emplace_back(0);
            outputs.emplace_back(std::make_shared<RealData>());
        }
        if(up){
            indices.emplace_back(1);
            outputs.emplace_back(std::make_shared<RealData>());
        }
        if(pressed){
            indices.emplace_back(2);
            outputs.emplace_back(std::make_shared<RealData>());
        }

        propagate_data(embedded_w()->button(), std::move(outputs), std::move(indices));
    }else{
        set_embedded_widget_text(embedded_w()->button());
    }
}

void CheckKeyboardButtonNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
    outPortsInfo[0].caption = QSL("down ms (") % get_name(io.outTypes[0]) % QSL(")");
    outPortsInfo[1].caption = QSL("up ms (") % get_name(io.outTypes[1]) % QSL(")");
    outPortsInfo[2].caption = QSL("pressed ms (") % get_name(io.outTypes[2]) % QSL(")");
}

void FilterKeyboardButtonNodeDataModel::compute(){

    if(check_infinity_loop()){
        return;
    }

    auto inputs = get_inputs();

    // no inputs
    if(!has_inputs(inputs)){
        set_embedded_widget_text(embedded_w()->w->w->currentText());
        set_invalid_state(QSL("Missing 1 entree."));
        return;
    }

    // runtime inputs
    if(check_if_runtime_inputs(inputs)){
        propagate_default_runtime(
            {
                std::make_shared<KeyboardButtonEventData>()
            });
        set_embedded_widget_text(embedded_w()->w->w->currentText());
        return;
    }

    // cast
    auto data = dcast<KeyboardButtonEventData>(inputs[0]);
    if(!data){
        set_invalid_cast();
        return;
    }

    auto interD = dcast<KeyboardButtonEventData>(interData[0]);
    if(data->value().code == interD->value().code){
        // ...
    }else{
        set_embedded_widget_text(embedded_w()->w->w->currentText());
    }
}


void FilterKeyboardButtonNodeDataModel::init_ports_caption(){
    const auto io = Connector::get_io(m_type);
    for(size_t ii = 0; ii < io.inNb; ++ii){
        inPortsInfo[ii].caption = QSL("in (") % get_name(io.inTypes[ii]) % QSL(")");
    }
    for(size_t ii = 0; ii < io.outNb; ++ii){
        outPortsInfo[ii].caption = QSL("out (") % get_name(io.outTypes[ii]) % QSL(")");
    }
}




#include "moc_keyboard_ndm.cpp"
