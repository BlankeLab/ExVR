
#pragma once

// expyvr
#include "config_parameters_widget.hpp"

namespace tool::ex {

class BodyCaptureInitConfigParametersW : public ConfigParametersW{

public :

    ExLineEditW m_lePathDepth;
    QPushButton m_pbSetPathDepth;

    ExLineEditW m_lePathSlaves;
    QPushButton m_pbSetPathSlave;


    void insert_widgets() override{
        QFrame *frame1 = ui::frame(ui::HBL(),{ui::TxtW("Path kinect depth space calib file: "),m_lePathDepth(), &m_pbSetPathDepth}, LStretch{false}, LMargins{true});
        QFrame *frame2 = ui::frame(ui::HBL(),{ui::TxtW("Path slaves calib file: "),m_lePathSlaves(), &m_pbSetPathSlave}, LStretch{false}, LMargins{true});
        layout()->addWidget(ui::frame(ui::VBL(),{frame1, frame2}, LStretch{true}, LMargins{false},QFrame::Box));
    }

    void init_and_register_widgets() override{
        m_neutralUiElements["set_path_depth"]   = ui::init_w(&m_pbSetPathDepth, "Set");
        m_neutralUiElements["set_path_slave"]   = ui::init_w(&m_pbSetPathSlave, "Set");
        m_inputUiElements["path_depth"]         = m_lePathDepth.init_widget("");
        m_inputUiElements["path_slaves"]        = m_lePathSlaves.init_widget("");
    }

    void create_connections() override{
        connect(&m_pbSetPathDepth, &QPushButton::clicked, this, [&]{
            QString path = QFileDialog::getOpenFileName(nullptr, "Kinect depth calib file", "", "CALIB (*.calib)");
            if(path.size() > 0){
                m_lePathDepth.w->setText(path);
                emit m_lePathDepth.w->textChanged(m_lePathDepth.w->text());
            }
        });
        connect(&m_pbSetPathSlave, &QPushButton::clicked, this, [&]{
            QString path = QFileDialog::getOpenFileName(nullptr, "Salves calib file", "", "CALIB (*.calib)");
            if(path.size() > 0){
                m_lePathSlaves.w->setText(path);
                emit m_lePathSlaves.w->textChanged(m_lePathSlaves.w->text());
            }
        });
    }

    void late_update_ui() override{}
};


class BodyCaptureConfigParametersW : public ConfigParametersW{

public :

    ExTransformationW m_transfo;

    ExFloatSpinBoxW m_dsbCustomScale;

    ExFloatSpinBoxW m_dsbMinDist;
    ExFloatSpinBoxW m_dsbMaxDist;

    ExSpinBoxW m_sbMinHeight;
    ExSpinBoxW m_sbMaxHeight;


    void insert_widgets() override{

        layout()->addWidget(ui::frame(ui::VBL(),{m_transfo()}, LStretch{true}, LMargins{true},QFrame::NoFrame));
        layout()->addWidget(ui::frame(ui::HBL(), {ui::TxtW("Custom scale:"), m_dsbCustomScale()}, LStretch{true}, LMargins{true}, QFrame::Box));
        layout()->addWidget(ui::frame(ui::HBL(), {ui::TxtW("Min distance:"), m_dsbMinDist()}, LStretch{true}, LMargins{true}, QFrame::Box));
        layout()->addWidget(ui::frame(ui::HBL(), {ui::TxtW("Max distance:"), m_dsbMaxDist()}, LStretch{true}, LMargins{true}, QFrame::Box));
        layout()->addWidget(ui::frame(ui::HBL(), {ui::TxtW("Min height:"), m_sbMinHeight()}, LStretch{true}, LMargins{true}, QFrame::Box));
        layout()->addWidget(ui::frame(ui::HBL(), {ui::TxtW("Max height:"), m_sbMaxHeight()}, LStretch{true}, LMargins{true}, QFrame::Box));
    }

    void init_and_register_widgets() override{

        DsbSettings s1 = DsbSettings{MinV<qreal>{-10000.}, V<qreal>{1}, MaxV<qreal>{10000.}, StepV<qreal>{0.01}, 3};
        DsbSettings s2 = DsbSettings{MinV<qreal>{-10000.}, V<qreal>{0}, MaxV<qreal>{10000.}, StepV<qreal>{0.01}, 3};
        DsbSettings s3 = DsbSettings{MinV<qreal>{-10000.}, V<qreal>{90}, MaxV<qreal>{10000.}, StepV<qreal>{0.1}, 3};
        m_inputUiElements["transformation"] = m_transfo.init_widget("Cloud transformation", {s1,s1,s1}, {s2,s2,s2}, {s3,s3,s3}, true);
        m_inputUiElements["customScale"]    = m_dsbCustomScale.init_widget(MinV<qreal>{-1.}, V<qreal>{0.}, MaxV<qreal>{200.}, StepV<qreal>{0.1},2);
        m_inputUiElements["minDistance"]    = m_dsbMinDist.init_widget(MinV<qreal>{0.}, V<qreal>{0.}, MaxV<qreal>{10000.}, StepV<qreal>{10},1);
        m_inputUiElements["maxDistance"]    = m_dsbMaxDist.init_widget(MinV<qreal>{0.}, V<qreal>{3000.}, MaxV<qreal>{10000.}, StepV<qreal>{10},1);
        m_inputUiElements["minHeight"]      = m_sbMinHeight.init_widget(MinV<int>{0}, V<int>{30}, MaxV<int>{424}, StepV<int>{1},1);
        m_inputUiElements["maxHeight"]      = m_sbMaxHeight.init_widget(MinV<int>{0}, V<int>{394}, MaxV<int>{424}, StepV<int>{1},1);
    }

    void create_connections() override{
        // ...
    }


    void late_update_ui() override{}
};

}
