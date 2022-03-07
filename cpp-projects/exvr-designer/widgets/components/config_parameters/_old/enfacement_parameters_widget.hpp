
#pragma once

// expyvr
#include "config_parameters_widget.hpp"

namespace tool::ex {


class EnfacementInitConfigParametersW : public ConfigParametersW{

public :

    void insert_widgets() override{}
    void init_and_register_widgets() override{}
    void create_connections() override{}
    void late_update_ui() override{}
};


class EnfacementConfigParametersW : public ConfigParametersW{

public :


    ExFloatSpinBoxW m_dsbDelay;


    void insert_widgets() override{
        layout()->addWidget(ui::frame(ui::HBL(),
            {ui::TxtW("Tracking delay: "),m_dsbDelay()}, LStretch{true}, LMargins{true}, QFrame::Box));
    }
    void init_and_register_widgets() override{
        m_inputUiElements["delay"] = m_dsbDelay.init_widget(MinV<qreal>{0}, V<qreal>{0}, MaxV<qreal>{100000.}, StepV<qreal>{1.}, 2);
    }

    void create_connections() override{}
    void late_update_ui() override{}
};


}
