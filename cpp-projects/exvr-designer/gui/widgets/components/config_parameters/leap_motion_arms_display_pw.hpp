
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
#include "gui/ex_widgets/ex_combo_box_index_w.hpp"
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_combo_box_text_w.hpp"


// local
#include "config_pw.hpp"


namespace tool::ex {

class LeapMotionInitConfigParametersW : public ConfigParametersW{
public:
};

class LeapMotionConfigParametersW : public ConfigParametersW{
public:
    ExCheckBoxW useTransformAsOrigin{"use_transform_as_origin"};
    TransformSubPart tr{"transform"};

    void insert_widgets() override{
        add_sub_part_widget(tr);
        add_widget(ui::F::gen(ui::L::HB(),{useTransformAsOrigin()}, LStretch{false}, LMargins{true}));
    }

    void init_and_register_widgets() override{
        add_input_ui(useTransformAsOrigin.init_widget("Use transform as origin", false));
        map_sub_part(tr.init_widget("Transform"));
    }
};


class LeapMotionArmsDisplayInitConfigParametersW : public ConfigParametersW{

public :

    ExComboBoxTextW m_cbtModel{"model"};
    ExCheckBoxW m_cbTrackFingers{"track_fingers"};

    void insert_widgets() override{
        add_widget(ui::F::gen(ui::L::HB(),{new QLabel("Model: "), m_cbtModel()}, LStretch{false}, LMargins{true}));
        add_widget(ui::F::gen(ui::L::HB(),{m_cbTrackFingers()}, LStretch{false}, LMargins{true}));
    }

    void init_and_register_widgets() override{
        add_input_ui(m_cbtModel.init_widget(
            {"Man black hand", "Man black arm", "Man white hand", "Man white arm"})); // , "Low poly"
        add_input_ui(m_cbTrackFingers.init_widget("Track fingers", true));
    }

    void create_connections() override{

    }

    void late_update_ui() override{}
};


class LeapMotionArmsDisplayConfigParametersW : public ConfigParametersW{


public :

    TransformSubPart m_tr{"model_transform"};
    ExComboBoxIndexW m_cbtPartToDisplay{"part_to_display"};
    ExCheckBoxW m_cbDisplayColliders{"display_colliders"};

    void insert_widgets() override{

        add_sub_part_widget(m_tr);        
        QFrame *frame2 = ui::F::gen(ui::L::HB(),{new QLabel("Part to display: "), m_cbtPartToDisplay()}, LStretch{false}, LMargins{true});
        QFrame *frame3 = ui::F::gen(ui::L::HB(),{m_cbDisplayColliders()}, LStretch{false}, LMargins{true});
        add_widget(ui::F::gen(ui::L::VB(),{frame2, frame3}, LStretch{false}, LMargins{false},QFrame::Box));
    }

    void init_and_register_widgets() override{
        add_input_ui(m_cbtPartToDisplay.init_widget({"Both", "Left", "Right", "None"}));
        add_input_ui(m_cbDisplayColliders.init_widget("Display colliders", false));
        map_sub_part(m_tr.init_widget("Model transform"));
    }

    void create_connections() override{
        // ...
    }


    void late_update_ui() override{}
};

}
