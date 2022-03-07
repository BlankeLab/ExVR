
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

#pragma once

// qt-utility
#include "ex_widgets/ex_combo_box_text_w.hpp"
#include "ex_widgets/ex_radio_button_w.hpp"
#include "ex_widgets/ex_double_spin_box_w.hpp"
#include "ex_widgets/ex_checkbox_w.hpp"
#include "ex_widgets/ex_vector3d_w.hpp"

// local
#include "ex_widgets/ex_components_list_w.hpp"
#include "config_pw.hpp"

namespace tool::ex {

class MriInitConfigParametersW : public ConfigParametersW{

public :

    void insert_widgets() override;
    void init_and_register_widgets() override;
    void create_connections() override;
    void late_update_ui() override;

    ExComponentW humanoidController{"humanoid_controller"};
    ExComponentW keyboard{"keyboard"};
    ExComponentsListW loggersList{"loggers"};
    ExComboBoxTextW triggerKey{"trigger_key"};

};

class MriConfigParametersW : public ConfigParametersW{

public :

    TransformSubPart transform{"transform"};

    ExCheckBoxW goNextWhenTrigger{"trigger_go_next"};

    QButtonGroup group1;
    ExRadioButtonW solve{"solve"};
    ExRadioButtonW dissolve{"dissolve"};
    ExRadioButtonW move_table_inside{"move_table_inside"};
    ExRadioButtonW move_table_outside{"move_table_outside"};
    ExRadioButtonW move_table_qualisys{"move_table_qualisys"};
    ExRadioButtonW nothing{"nothing"};

    ExDoubleSpinBoxW duration{"duration"};
    ExDoubleSpinBoxW distance{"distance"};
    ExCheckBoxW room{"process_room"};
    ExCheckBoxW magnet{"process_magnet"};

    ExCheckBoxW initQualisysPositions{"init_qualisys"};
    ExCheckBoxW enableQualisysArmsTracking{"qualisys_arms_tracking"};
    ExVector3dW targetHeadTrOffset{"target_head_tr_offset"};
    ExVector3dW targetHeadRotOffset{"target_head_rot_offset"};

    void insert_widgets() override;
    void init_and_register_widgets() override;
    void create_connections() override;
    void late_update_ui() override;
};

}
