
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

#include "variable_delay_ndm.hpp"

using namespace tool::ex;


void VariableDelayFEmbeddedW::initialize(){

    // init widget
    set_title(QSL("Variable delay"));
    w->init_widget(QSL("Delay curve"), QSL("Time(s)"), QSL("Delay(ms)"), {0,10}, {0,1000});

    // set widget connections
    w->minX.w->setEnabled(false);
    connect(w.get(), &ExCurveW::ui_change_signal, this, &VariableDelayFEmbeddedW::compute_data_signal);

    // add widget to ui
    add_row_in_dialog("", w->w.get());
}


void VariableDelayNodeDataModel::compute(){

    unlock_embedded_widget();

    if(check_infinity_loop(false)){
        return;
    }

    auto inputs = get_inputs();
    if(!inputs[1]){
        set_invalid_state(QSL("Missing entree #2."));
        invalidate_data();
        return;
    }

    set_valid_state();
    propagate_default_runtime({generate_default_runtime_any_data()});
}


#include "moc_variable_delay_ndm.cpp"
