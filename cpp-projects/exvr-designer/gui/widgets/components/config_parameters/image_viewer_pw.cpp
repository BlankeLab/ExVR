
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

#include "image_viewer_pw.hpp"


// qt-utility
#include "gui/ex_widgets/ex_vector2d_w.hpp"
#include "gui/ex_widgets/ex_double_spin_box_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_radio_button_w.hpp"

// local
#include "gui/ex_widgets/ex_resource_w.hpp"
#include "gui/ex_widgets/ex_component_w.hpp"

using namespace tool::ex;

struct ImageViewerConfigParametersW::Impl{
    ExResourceW image{"image"};
    ExComponentW component{"resource_component"};
    QButtonGroup group1;
    ExRadioButtonW useResource{"use_resource"};
    ExRadioButtonW useComponent{"use_component"};
    ExRadioButtonW useNothing{"use_nothing"};
    WordSpaceCanvasSubPart wscsp;
};


ImageViewerConfigParametersW::ImageViewerConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void ImageViewerConfigParametersW::insert_widgets(){
    layout()->setContentsMargins(0,0,0,0);

    auto rbs = ui::F::gen(ui::L::HB(), {m_p->useResource(), m_p->useComponent(), m_p->useNothing()}, LStretch{true}, LMargins{false}, QFrame::NoFrame);
    add_widget(ui::F::gen(ui::L::VB(), {ui::W::txt("<b>Image settings</b>"), rbs, m_p->image(), m_p->component()}, LStretch{false}, LMargins{true}, QFrame::Box));
    add_sub_part_widget(m_p->wscsp);
}

void ImageViewerConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_p->image.init_widget(Resource::Type::Image, "Resource: "));
    add_input_ui(m_p->component.init_widget(Component::Type::Image_resource, "Component"));
    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
            {&m_p->useResource, &m_p->useComponent, &m_p->useNothing},
        {
            "Use resource",
            "Use component",
            "Use nothing"
        },
            {true, false, false}
        )
    );

    map_sub_part(m_p->wscsp.init_widget());
}

void ImageViewerConfigParametersW::create_connections(){
    m_p->wscsp.create_connections();
}

void ImageViewerConfigParametersW::late_update_ui(){
    m_p->wscsp.late_update_ui();
}
