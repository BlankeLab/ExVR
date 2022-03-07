
#pragma once

// expyvr
#include "config_parameters_widget.hpp"

namespace tool::ex {

class CleanMarkTaskInitConfigParametersW : public ConfigParametersW{

public :

    ExSpinBoxW m_sbWidthColliderResolution;
    ExSpinBoxW m_sbHeightColliderResolution;
    ExLineEditW m_leLeapMotionTrackingComponent;

    ExFloatSpinBoxW m_dsbMaxReachableDistance;
    ExFloatSpinBoxW m_dsbStartLineDistance;

    void insert_widgets() override{

        layout()->addWidget(ui::frame(ui::HBL(),{new QLabel("Leap motion tracking component: "),m_leLeapMotionTrackingComponent()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
        layout()->addWidget(ui::frame(ui::HBL(),{new QLabel("Width collider resolution: "),m_sbWidthColliderResolution()}, LStretch{true}, LMargins{false},QFrame::NoFrame));
        layout()->addWidget(ui::frame(ui::HBL(),{new QLabel("Height collider resolution: "),m_sbHeightColliderResolution()}, LStretch{true}, LMargins{false},QFrame::NoFrame));

        auto distance = ui::frame(ui::HBL(),{ui::TxtW("Max reachable distance (m): "),m_dsbMaxReachableDistance()}, LStretch{true}, LMargins{false},QFrame::NoFrame);
        auto startLineDistance = ui::frame(ui::HBL(),{ui::TxtW("Start line distance (m): "),m_dsbStartLineDistance()}, LStretch{true}, LMargins{false},QFrame::NoFrame);
        layout()->addWidget(ui::frame(ui::VBL(),{distance, startLineDistance}, LStretch{true}, LMargins{false},QFrame::NoFrame));
    }

    void init_and_register_widgets() override{
        m_inputUiElements["width_collider_resolution"]  = m_sbWidthColliderResolution.init_widget(MinV<int>{10}, V<int>{250}, MaxV<int>{1000}, StepV<int>{1});
        m_inputUiElements["height_collider_resolution"] = m_sbHeightColliderResolution.init_widget(MinV<int>{10}, V<int>{250}, MaxV<int>{1000}, StepV<int>{1});
        m_inputUiElements["leap_motion_component"]      = m_leLeapMotionTrackingComponent.init_widget("...");

        m_inputUiElements["max_distance_reachable"]     = m_dsbMaxReachableDistance.init_widget(MinV<qreal>{0.1}, V<qreal>{0.5}, MaxV<qreal>{1.3}, StepV<qreal>{0.01}, 2);
        m_inputUiElements["start_distance"]             = m_dsbStartLineDistance.init_widget(MinV<qreal>{0.1}, V<qreal>{0.1}, MaxV<qreal>{1.3}, StepV<qreal>{0.01}, 2);
    }

    void create_connections() override{

    }

    void late_update_ui() override{}
};


class CleanMarkTaskConfigParametersW : public ConfigParametersW{

public :



    // general
    ExComboBoxIndexW m_mode;
    ExCheckBoxW m_cbDebug;
    ExCheckBoxW m_cbDisplayStartLine;


    // working zone
    DefaultTransformSubPart m_trWorkingZone;

    // mark
    ExFloatSpinBoxW m_dsbDistancePercentageMark;

    // ring
    ExFloatSpinBoxW m_dsbDistancePercentageRing;
    ExVector3dW m_v3dRotationRing;
    ExFloatSpinBoxW m_dsbHeightRing;
    ExFloatSpinBoxW m_dsbAngleRing;
    ExFloatSpinBoxW m_dsbScaleRing;

    // sponge
    ExCheckBoxW m_cbDisplaySponge;
    DefaultTransformSubPart m_trSponge;

    void insert_widgets() override{

        // general
        auto mode = ui::frame(ui::HBL(),{ui::TxtW("Mode: "),m_mode()}, LStretch{true}, LMargins{false},QFrame::NoFrame);

        layout()->addWidget(ui::frame(ui::VBL(),{new QLabel("<b>General</b>"), mode, m_cbDebug(), m_cbDisplayStartLine()}, LStretch{true}, LMargins{true},QFrame::Box));

        // working zone
        add_sub_part_to_layout(m_trWorkingZone);

        // mark
        auto markDistance = ui::frame(ui::HBL(),{ui::TxtW("Distance (%): "),m_dsbDistancePercentageMark()}, LStretch{false}, LMargins{false},QFrame::NoFrame);
        layout()->addWidget(ui::frame(ui::VBL(),{new QLabel("<b>Mark</b>"), markDistance}, LStretch{true}, LMargins{true},QFrame::Box));

        // ring
        auto ringDistance = ui::frame(ui::HBL(),{ui::TxtW("Distance (%): "),m_dsbDistancePercentageRing()}, LStretch{false}, LMargins{false},QFrame::NoFrame);
        auto ringAngle    = ui::frame(ui::HBL(),{ui::TxtW("Angle (°): "),m_dsbAngleRing()}, LStretch{false}, LMargins{false},QFrame::NoFrame);
        auto ringHeight   = ui::frame(ui::HBL(),{ui::TxtW("Height (m): "),m_dsbHeightRing()}, LStretch{false}, LMargins{false},QFrame::NoFrame);
        auto ringScale    = ui::frame(ui::HBL(),{ui::TxtW("Scale: "),m_dsbScaleRing()}, LStretch{false}, LMargins{false},QFrame::NoFrame);
        layout()->addWidget(ui::frame(ui::VBL(),{new QLabel("<b>Ring</b>"), ringDistance, ringAngle, ringHeight,ringScale, m_v3dRotationRing()}, LStretch{true}, LMargins{true},QFrame::Box));

        // sponge
        add_sub_part_to_layout(m_trSponge);
        layout()->addWidget(ui::frame(ui::HBL(),{new QLabel("Display sponge: "), m_cbDisplaySponge()}, LStretch{true}, LMargins{true},QFrame::Box));
    }

    void init_and_register_widgets() override{

        // general
        m_inputUiElements["mode"]                     = m_mode.init_widget({"Clean task", "Display target", "Grab target"});
        m_inputUiElements["display_start_line"]       = m_cbDisplayStartLine.init_widget("Display start line", false);
        m_inputUiElements["debug"]                    = m_cbDebug.init_widget("Debug", true);

        // working zone
        add_sub_part_to_map(m_trWorkingZone.init_widget("Working zone transform", "working_transform"));

        // mark
        m_inputUiElements["distance_percentage_mark"]  = m_dsbDistancePercentageMark.init_widget(MinV<qreal>{10.}, V<qreal>{80.}, MaxV<qreal>{100.}, StepV<qreal>{0.1}, 1);

        // ring
        m_inputUiElements["distance_percentage_ring"]  = m_dsbDistancePercentageRing.init_widget(MinV<qreal>{10.}, V<qreal>{80.}, MaxV<qreal>{100.}, StepV<qreal>{0.1}, 1);
        m_inputUiElements["angle_ring"]                = m_dsbAngleRing.init_widget(MinV<qreal>{-50.}, V<qreal>{0.0}, MaxV<qreal>{50.}, StepV<qreal>{1.}, 1);
        m_inputUiElements["scale_ring"]                = m_dsbScaleRing.init_widget(MinV<qreal>{0.01}, V<qreal>{1.}, MaxV<qreal>{10.}, StepV<qreal>{0.10}, 2);
        m_inputUiElements["rotation_ring"]             = m_v3dRotationRing.init_widget("Rotation ring");
        m_inputUiElements["height_ring"]               = m_dsbHeightRing.init_widget(MinV<qreal>{-1.}, V<qreal>{0.}, MaxV<qreal>{1.}, StepV<qreal>{0.01}, 2.);

        // sponge
        m_inputUiElements["display_sponge"]         = m_cbDisplaySponge.init_widget("Display", false);
        add_sub_part_to_map(m_trSponge.init_widget("Sponge transform", "sponge_transform"));
        m_trSponge.tr.set_rotation_values({0.,0.,90.});
        m_trSponge.tr.set_scale_values({0.01,0.01,0.01});
        m_trSponge.tr.set_trans_decimals(5);
        m_trSponge.tr.set_trans_steps({0.0001,0.0001,0.0001});
        m_trSponge.tr.set_trans_values({-0.0002,0.0008,0.0001});

        m_trWorkingZone.tr.set_rotation_values({90.,0.,0.});
        m_trWorkingZone.tr.set_trans_decimals(3);
        m_trWorkingZone.tr.set_trans_steps({0.001,0.001,0.001});

        m_v3dRotationRing.set_min({-360.,-360.,-360.});
        m_v3dRotationRing.set_max({360.,360.,360.});
        m_v3dRotationRing.set_values({0.,0.,0.});
        m_v3dRotationRing.set_steps({1.,1.,1.});
    }

    void create_connections() override{
        // ...
    }

    void late_update_ui() override{}
};

}
