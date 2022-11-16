
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

#include "video_resource_pw.hpp"

// qt-utility
#include "gui/ex_widgets/ex_checkbox_w.hpp"

// local
#include "gui/ex_widgets/ex_resource_w.hpp"


using namespace tool::ex;

struct VideoResourceInitConfigParametersW::Impl{
};

VideoResourceInitConfigParametersW::VideoResourceInitConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}


struct VideoResourceConfigParametersW::Impl{
    ExResourceW video{"video"};
    ExCheckBoxW playNewRoutine{"play_at_new_routine"};
    ExCheckBoxW playImages{"play_images"};
    ExCheckBoxW playAudio{"play_audio"};
};

VideoResourceConfigParametersW::VideoResourceConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void VideoResourceConfigParametersW::insert_widgets(){
    add_widget(ui::F::gen(ui::L::VB(), {m_p->video(), m_p->playNewRoutine(), m_p->playAudio(), m_p->playImages()}, LStretch{false}, LMargins{true}, QFrame::Box));
}

void VideoResourceConfigParametersW::init_and_register_widgets(){
    add_input_ui(m_p->video.init_widget(Resource::Type::Video, "Video resource: "));
    add_input_ui(m_p->playNewRoutine.init_widget("Play when routine starts", true));
    add_input_ui(m_p->playImages.init_widget("Play images", true));
    add_input_ui(m_p->playAudio.init_widget("Play audio", true));
}
