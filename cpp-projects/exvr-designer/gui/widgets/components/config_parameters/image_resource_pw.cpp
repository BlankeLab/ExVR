
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

#include "image_resource_pw.hpp"

// local
#include "gui/ex_widgets/ex_resources_list_w.hpp"
#include "gui/ex_widgets/ex_line_edit_w.hpp"
#include "gui/ex_widgets/ex_radio_button_w.hpp"
#include "gui/ex_widgets/ex_spin_box_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"

using namespace tool::ex;

struct ImageResourceInitConfigParametersW::Impl{
    ExResourcesListW imagesList{"images_list"};
};

ImageResourceInitConfigParametersW::ImageResourceInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void ImageResourceInitConfigParametersW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("<b>List of images to use:</b>"), m_p->imagesList()}, LStretch{false}, LMargins{true}, QFrame::Box));
    no_end_stretch();
}

void ImageResourceInitConfigParametersW::init_and_register_widgets(){
    add_input_ui(m_p->imagesList.init_widget(Resource::Type::Image, "Resources:"));
}

void ImageResourceInitConfigParametersW::create_connections(){
    connect(&m_p->imagesList, &ExResourcesListW::ui_change_signal, this, [&]{

    });
}

void ImageResourceInitConfigParametersW::late_update_ui(){}

struct ImageResourceConfigParametersW::Impl{
    ExCheckBoxW startExp{"start_exp"};
    ExCheckBoxW startRoutine{"start_routine"};
    ExCheckBoxW stopRoutine{"stop_routine"};
    QButtonGroup group1;
    ExRadioButtonW sendAlias{"use_alias"};
    ExRadioButtonW sendId{"use_id"};
    ExLineEditW alias{"alias"};
    ExSpinBoxW id{"id"};
};

ImageResourceConfigParametersW::ImageResourceConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void ImageResourceConfigParametersW::insert_widgets(){
    layout()->setContentsMargins(0,0,0,0);    
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("Send current image when:"), m_p->startExp(),m_p->startRoutine(),m_p->stopRoutine()}, LStretch{false}, LMargins{true}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Using:"), m_p->sendAlias(),m_p->sendId()}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("From init config images resources list:")}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
    add_widget(ui::F::gen(ui::L::HB(), {ui::W::txt("Id"), m_p->id(), ui::W::txt("Alias"), m_p->alias()}, LStretch{false}, LMargins{true}, QFrame::NoFrame));
}

void ImageResourceConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_p->startExp.init_widget(" experiment starts", false));
    add_input_ui(m_p->startRoutine.init_widget(" routine starts", true));
    add_input_ui(m_p->stopRoutine.init_widget(" routine stops", false));
    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
        {&m_p->sendId, &m_p->sendAlias},
        {
            "id",
            "alias",
        },
        {true,false}
    ));

    add_input_ui(m_p->alias.init_widget(""));
    add_input_ui(m_p->id.init_widget(MinV<int>{0}, V<int>{0}, MaxV<int>{10000}, StepV<int>{1}));
}

void ImageResourceConfigParametersW::create_connections(){
}

void ImageResourceConfigParametersW::late_update_ui(){
}
