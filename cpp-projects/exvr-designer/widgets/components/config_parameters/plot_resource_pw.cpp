
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

#include "plot_resource_pw.hpp"


using namespace tool::ex;


void PlotResourceConfigParametersW::insert_widgets(){
    layout()->setContentsMargins(0,0,0,0);
    add_widget(ui::F::gen(ui::L::HB(), {m_plot()}, LStretch{false}, LMargins{true}, QFrame::Box));
    add_widget(ui::F::gen(ui::L::VB(), {m_randomSelection(), m_plotsList()}, LStretch{false}, LMargins{true}, QFrame::Box));
    no_end_stretch();
}

void PlotResourceConfigParametersW::init_and_register_widgets(){
    add_input_ui(m_plot.init_widget(Resource::Type::Plot, "Selected plot resource: "));
    add_input_ui(m_randomSelection.init_widget("Random resource selection from list: ", false));
    add_input_ui(m_plotsList.init_widget(Resource::Type::Plot, "Plots list"));
}

void PlotResourceConfigParametersW::create_connections(){
    connect(m_randomSelection(), &QCheckBox::clicked, this, [&]{
        m_plot.w->setEnabled(!m_randomSelection.w->isChecked());
        m_plotsList.w->setEnabled(m_randomSelection.w->isChecked());
    });
}

void PlotResourceConfigParametersW::late_update_ui(){
    m_plot.w->setEnabled(!m_randomSelection.w->isChecked());
    m_plotsList.w->setEnabled(m_randomSelection.w->isChecked());
}
