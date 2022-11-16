
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

#include "text_viewer_pw.hpp"

// qt-utility
#include "gui/ex_widgets/ex_select_color_w.hpp"
#include "gui/ex_widgets/ex_text_edit_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_radio_button_w.hpp"
#include "gui/ex_widgets/ex_vector2d_w.hpp"

// local
#include "gui/ex_widgets/ex_resource_w.hpp"

using namespace tool::ex;

struct TextViewerConfigParametersW::Impl{
    WordSpaceCanvasSubPart wscsp;
    TextSubPart tsp {"t"};
};

TextViewerConfigParametersW::TextViewerConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void TextViewerConfigParametersW::insert_widgets(){
    layout()->setContentsMargins(0,0,0,0);
    add_sub_part_widget(m_p->tsp);
    add_sub_part_widget(m_p->wscsp);
}

void TextViewerConfigParametersW::init_and_register_widgets(){    
    map_sub_part(m_p->wscsp.init_widget());
    m_p->wscsp.set_background_color(QColor(255,255,255,0));
    map_sub_part(m_p->tsp.init_widget());    
}

void TextViewerConfigParametersW::create_connections(){
    m_p->wscsp.create_connections();
}

void TextViewerConfigParametersW::late_update_ui(){
    m_p->wscsp.late_update_ui();
}
