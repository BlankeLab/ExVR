
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

#include "ex_parameters_generator_w.hpp"


// qt-utility
#include "gui/ex_widgets/ex_color_frame_w.hpp"
#include "gui/ex_widgets/ex_label_w.hpp"
#include "gui/ex_widgets/ex_line_edit_w.hpp"
#include "gui/ex_widgets/ex_select_color_w.hpp"
#include "gui/ex_widgets/ex_slider_w.hpp"
#include "gui/ex_widgets/ex_transformation_w.hpp"
#include "gui/ex_widgets/ex_list_labels_w.hpp"
#include "gui/ex_widgets/ex_spin_box_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_combo_box_index_w.hpp"
#include "gui/ex_widgets/ex_combo_box_text_w.hpp"
#include "gui/ex_widgets/ex_text_edit_w.hpp"
#include "gui/ex_widgets/ex_radio_button_w.hpp"
#include "gui/ex_widgets/ex_curve_w.hpp"
#include "gui/ex_widgets/ex_curve_x_w.hpp"
#include "gui/ex_widgets/ex_vector2d_w.hpp"
#include "gui/ex_widgets/ex_code_editor_w.hpp"
#include "gui/ex_widgets/ex_notepad_w.hpp"

// local
#include "gui/ex_widgets/ex_resource_w.hpp"
#include "gui/ex_widgets/ex_resources_list_w.hpp"
#include "gui/ex_widgets/ex_component_w.hpp"
#include "gui/ex_widgets/ex_components_list_w.hpp"
#include "gui/ex_widgets/ex_tab_w.hpp"
#include "gui/ex_widgets/generation/gen_ui_item_dialog.hpp"

using namespace tool;
using namespace tool::ex;


ExParametersGeneratorWidgetW::ExParametersGeneratorWidgetW(QString name) : ExItemW<QWidget>(UiType::Generator, name){

    ui::W::init(&m_pbAddParam, "Add");
    ui::W::init(&m_pbMoveUp, "Move up");
    ui::W::init(&m_pbMoveDown, "Move down");
    ui::W::init(&m_pbRemoveParam, "Remove");
    m_lwParameters.set_widget_selection(true);

    QStringList items;
    for (auto& t : get_ui_types()) {
        if(is_generative(t)){
            items << from_view(get_name(t));
        }
    }
    ui::W::init(&m_cbTypeParam, items);

    auto l = new QVBoxLayout();
    l->setContentsMargins(5,5,5,5);
    w->setLayout(l);
    w->layout()->addWidget(ui::F::gen(ui::L::HB(),{
        ui::W::txt("Parameter to add "),&m_cbTypeParam}, LStretch{true}, LMargins{false}));
    w->layout()->addWidget(ui::F::gen(ui::L::HB(),{
        &m_pbAddParam, &m_pbMoveUp, &m_pbMoveDown, &m_pbRemoveParam}, LStretch{true}, LMargins{false}));
    w->layout()->addWidget(&m_lwParameters);

    connect(&m_pbAddParam, &QPushButton::clicked, this, [&]{
        int id = m_lwParameters.current_selected_widget_id();
        auto txt = m_cbTypeParam.currentText().toStdString();        
        if(id == -1){
            id = m_lwParameters.count();
        }
        qDebug() << "id " << id << " " << m_lwParameters.current_selected_widget_id();
        add_ui_element_from_dialog(get_ui_type(txt).value(), id);
    });

    connect(&m_pbMoveUp, &QPushButton::clicked, this, [&]{

        int id = m_lwParameters.current_selected_widget_id();
        if(id <= 0 || id >= elementsOrder.count()){
            return;
        }

        m_lwParameters.move_from_to(id-1, id);
        m_lwParameters.set_selected_widget(id -1);
        elementsOrder.swapItemsAt(id-1, id);
        emit swap_ui_signal(elementsOrder.at(id-1), elementsOrder.at(id));
    });

    connect(&m_pbMoveDown, &QPushButton::clicked, this, [&]{

        int id = m_lwParameters.current_selected_widget_id();
        if(id < 0 || id >= elementsOrder.count()-1){
            return;
        }

        m_lwParameters.move_from_to(id, id+1);
        m_lwParameters.set_selected_widget(id +1);
        elementsOrder.swapItemsAt(id, id+1);
        emit swap_ui_signal(elementsOrder.at(id), elementsOrder.at(id+1));
    });

    connect(&m_pbRemoveParam, &QPushButton::clicked, this, [&]{

        int id = m_lwParameters.current_selected_widget_id();
        if(id < 0 || id >= elementsOrder.count()){
            return;
        }

        QStringView nameToRemove = elementsOrder.at(id);
        m_lwParameters.delete_at(id);
        elementsOrder.removeAt(id);

        m_inputUiElements->erase(nameToRemove);
        generatorElements.erase(nameToRemove);

        emit remove_ui_signal(nameToRemove);
    });
}

ExParametersGeneratorWidgetW::~ExParametersGeneratorWidgetW(){
    // delete generator associated elements
    for(auto &elem : generatorElements){
        delete elem.second;
        elem.second = nullptr;
    }
}

ExParametersGeneratorWidgetW *ExParametersGeneratorWidgetW::init_widget(
    umap<QStringView, ExBaseW *> *inputUiElements, bool enabled){

    m_inputUiElements = inputUiElements;
    w->setEnabled(enabled);
    return this;
}


void ExParametersGeneratorWidgetW::update_from_arg(const Arg &arg){

    if(generatorElements.count(arg.name) == 0){
        add_ui_element_from_arg(arg);
    }else {
        generatorElements[arg.name]->update_from_arg(arg);
    }
}

void ExParametersGeneratorWidgetW::update_from_resources(){
    for(const auto &elem : generatorElements){
        elem.second->update_from_resources();
    }
}

void ExParametersGeneratorWidgetW::update_from_components(){
    for(const auto &elem : generatorElements){
        elem.second->update_from_components();
    }
}

Arg ExParametersGeneratorWidgetW::convert_to_arg() const{
    return ExBaseW::convert_to_arg();
}


void ExParametersGeneratorWidgetW::add_ui_element_from_dialog(UiType uiType, int order){

    auto [wElem, exW] =  gen_ui_element(uiType);
    if(wElem == nullptr){
        return;
    }
    exW->set_as_generator();

    QStringList names;
    for(auto &inputUiElement : *m_inputUiElements){
        names << inputUiElement.second->itemName;
    }

    GenUIItemDialog genD(uiType);
    genD.setMinimumWidth(500);
    genD.setMaximumWidth(900);

    switch (uiType) {
    // simple ui
    // # bool
    case UiType::Check_box:{
        genD.add_gen_widget(new CheckBoxGen());
    }break;
    // # reals
    case UiType::Slider_integer :{
        genD.add_gen_widget(new GenSpinboxW("Slider options"));
    }break;
    case UiType::Slider_double :{
        genD.add_gen_widget(new SpinboxDoubleGenW("Slider options"));
    }break;
    case UiType::Spin_box:{
        genD.add_gen_widget(new GenSpinboxW());
    }break;
    case UiType::Float_spin_box :{
        genD.add_gen_widget(new SpinboxFloatGenW());
    }break;
    case UiType::Double_spin_box:{
        genD.add_gen_widget(new SpinboxDoubleGenW());
    }break;
    // # text
    case UiType::Label:{
        genD.add_gen_widget(new TextGenW("Title:"));
    }break;
    case UiType::Line_edit:{
        genD.add_gen_widget(new TextGenW("Value:"));
    }break;
    case UiType::Text_edit:{
        genD.add_gen_widget(new TextGenW("Text:"));
    }break;
    // # combo
    case UiType::Combo_box_text:{
        genD.add_gen_widget(new ComboTextGen());
    }break;
    case UiType::Combo_box_index:{
        genD.add_gen_widget(new ComboTextGen());
    }break;
    // # vector
    case UiType::Vector2D:{
        genD.add_gen_widget(new Vector2dGenW());
    }break;
    case UiType::Vector3D:{
        genD.add_gen_widget(new Vector3dGenW());
    }break;
    // # color
    case UiType::Color_pick:{
        genD.add_gen_widget(new ColorPickGen());
    }break;
    // complex ui
    case UiType::Transformation:{
        genD.add_gen_widget(new TransformGenW());
    }break;
    case UiType::Curve:{
        genD.add_gen_widget(new CurveGen());
    }break;
    case UiType::Notepad:{
        genD.add_gen_widget(new NotepadGen());
    }break;
    case UiType::Code_editor:{
        genD.add_gen_widget(new CodeEditorGen());
    }break;
    // ex data
    case UiType::ResourcesList:{
        genD.add_gen_widget(new ResourceGenW());
    }break;
    case UiType::Resource:{
        genD.add_gen_widget(new ResourceGenW());
    }break;
    case UiType::Component:{
        genD.add_gen_widget(new ComponentGenW());        
    }break;
    case UiType::ComponentsList:{
        genD.add_gen_widget(new ComponentGenW());
    }break;

    default:        
        break;
    }

    if(!genD.show_dialog(names)){
        delete exW;
        return;
    }

    // set order
    exW->update_from_arg(genD.generate_arg(order));

    // add widget to input ui elements
    (*m_inputUiElements)[exW->itemName] = exW;
    generatorElements[exW->itemName] = exW;

    // generate widget line
    auto fl =  ui::L::HB();
    auto f  = ui::F::gen(fl,{ui::W::txt(QSL("<b>") % exW->itemName % QSL("</b>")), wElem}, LStretch{false}, LMargins{true});
    fl->setStretch(0, 1);
    fl->setStretch(1, 30);

    // add item name to generator list
    bool added = false;
    for(int ii = 0; ii < elementsOrder.size(); ++ii){
        if(order <= generatorElements[elementsOrder[ii]]->generatorOrder){
            elementsOrder.insert(ii, exW->itemName);
            m_lwParameters.insert_widget(ii, f);
            added = true;
            break;
        }
    }

    if(!added){
        elementsOrder << exW->itemName;
        m_lwParameters.add_widget(f);
    }

    // setup widget connections
    connect(exW, &ExBaseW::ui_change_signal, this, &ExParametersGeneratorWidgetW::ui_change_signal);

    // send new ui to config
    emit add_ui_signal(exW->convert_to_arg());

}

void ExParametersGeneratorWidgetW::add_ui_element_from_arg(Arg arg){

    auto [wElem, exW] =  gen_ui_element(arg.associated_ui_type());
    if(exW == nullptr){
        return;
    }

    exW->set_as_generator();
    exW->itemName = arg.name;

    // set order
    int currentId = arg.generator->order;
//    if(arg.generator->order >= 0 ){
//        currentId = arg.generator->order;
//    }else{
//        currentId++;
//    }
    exW->generatorOrder = currentId;

    // add widget to input ui elements
    (*m_inputUiElements)[exW->itemName] = exW;
    generatorElements[exW->itemName] = exW;

    // generate widget line
    auto fl =  ui::L::HB();
    auto f  = ui::F::gen(fl,{ui::W::txt(QSL("<b>") % exW->itemName % QSL("</b>")), wElem}, LStretch{false}, LMargins{true});
    fl->setStretch(0, 1);
    fl->setStretch(1, 30);

    // add item name to generator list
    bool added = false;
    for(int ii = 0; ii < elementsOrder.size(); ++ii){
        if(currentId < generatorElements[elementsOrder[ii]]->generatorOrder){
            elementsOrder.insert(ii, exW->itemName);
            m_lwParameters.insert_widget(ii, f);
            added = true;
            break;
        }
    }

    if(!added){
        elementsOrder << exW->itemName;
        m_lwParameters.add_widget(f);
    }

    // setup widget connections
    connect(exW, &ExBaseW::ui_change_signal, this, &ExParametersGeneratorWidgetW::ui_change_signal);

    exW->update_from_arg(arg);
}


std::pair<QWidget*,ExBaseW*> ExParametersGeneratorWidgetW::gen_ui_element(UiType uiType){

    // generate widget
    ExBaseW *exW = nullptr;
    QWidget *wElem = nullptr;

    switch (uiType) {
    // simple ui
    // # bool
    case UiType::Check_box :{
        wElem = dynamic_cast<ExCheckBoxW*>(exW = new ExCheckBoxW())->w.get();
    }break;
    // # reals
    case UiType::Spin_box:{
        wElem = dynamic_cast<ExSpinBoxW*>(exW = new ExSpinBoxW())->w.get();
    }break;
    case UiType::Double_spin_box:{
        wElem = dynamic_cast<ExDoubleSpinBoxW*>(exW = new ExDoubleSpinBoxW())->w.get();
    }break;
    case UiType::Float_spin_box :{
        wElem = dynamic_cast<ExFloatSpinBoxW*>(exW = new ExFloatSpinBoxW())->w.get();
    }break;
    case UiType::Slider_integer :{
        wElem = dynamic_cast<ExSliderIntegerW*>(exW = new ExSliderIntegerW())->w.get();
    }break;
    case UiType::Slider_double :{
        wElem = dynamic_cast<ExSliderFloatW*>(exW = new ExSliderFloatW())->w.get();
    }break;
    // # text
    case UiType::Label:{
        wElem = dynamic_cast<ExLabelW*>(exW = new ExLabelW())->w.get();
    }break;
    case UiType::Line_edit :{
        wElem = dynamic_cast<ExLineEditW*>(exW = new ExLineEditW())->w.get();
    }break;
    case UiType::Text_edit:{
        wElem = dynamic_cast<ExTextEditW*>(exW = new ExTextEditW())->w.get();
    }break;
    // # combo
    case UiType::Combo_box_text:{
        wElem = dynamic_cast<ExComboBoxTextW*>(exW = new ExComboBoxTextW())->w.get();
    }break;
    case UiType::Combo_box_index:{
        wElem = dynamic_cast<ExComboBoxIndexW*>(exW = new ExComboBoxIndexW())->w.get();
    }break;
    // # vector
    case UiType::Vector2D:{
        wElem = dynamic_cast<ExVector2dW*>(exW = new ExVector2dW())->w.get();
    }break;
    case UiType::Vector3D:{
        wElem = dynamic_cast<ExVector3dW*>(exW = new ExVector3dW())->w.get();
    }break;
    // # color
    case UiType::Color_pick:{
        wElem = dynamic_cast<ExSelectColorW*>(exW = new ExSelectColorW())->w.get();
    }break;
    // complex ui
    case UiType::Transformation:{
        wElem = dynamic_cast<ExTransformationW*>(exW = new ExTransformationW())->w.get();
    }break;
    case UiType::Curve:{
        wElem = dynamic_cast<ExCurveW*>(exW = new ExCurveW())->w.get();
    }break;
    case UiType::Notepad:{
        wElem = dynamic_cast<ExNotepadW*>(exW = new ExNotepadW())->w.get();
    }break;
    case UiType::Code_editor:{
        wElem = dynamic_cast<ExCodeEditorW*>(exW = new ExCodeEditorW())->w.get();
        dynamic_cast<ExCodeEditorW*>(exW)->init_widget_as_csharp_editor(QColor(30,30,30), "");
    }break;
    // exvr_data
    case UiType::Resource:{
        wElem = dynamic_cast<ExResourceW*>(exW = new ExResourceW())->w.get();
    }break;
    case UiType::ResourcesList:{
        wElem = dynamic_cast<ExResourcesListW*>(exW = new ExResourcesListW())->w.get();
    }break;
    case UiType::Component:{
        wElem = dynamic_cast<ExComponentW*>(exW = new ExComponentW())->w.get();
    }break;
    case UiType::ComponentsList:{
        wElem = dynamic_cast<ExComponentsListW*>(exW = new ExComponentsListW())->w.get();
    }break;

    default:
        qDebug() << "generator widget not managed for type " << from_view(get_name(uiType));
        return {nullptr,nullptr};
    }
    return {wElem,exW};
}





#include "moc_ex_parameters_generator_w.cpp"
