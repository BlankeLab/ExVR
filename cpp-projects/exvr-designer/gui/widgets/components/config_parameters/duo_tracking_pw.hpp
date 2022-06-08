

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

//#pragma once

//// local
//#include "config_pw.hpp"
////#include "base/ex_transformation_widget.hpp"

//namespace tool::ex {

////params.scale             = 3;  // [0, 3] - [No Scale, Scale X, Scale Y, Scale X&Y]
////params.mode              = 0;  // [0, 3] - [BM, SGBM, BM_HQ, SGBM_HQ]
////params.preFilterCap      = 63; // [1, 63]
////params.numDisparities    = 4;  // [2, 16]
////params.sadWindowSize     = 10; // [2, 10]
////params.uniqenessRatio    = 1;  // [1, 100]
////params.speckleWindowSize = 60; // [0, 256]
////params.speckleRange      = 20; // [0, 32]


//class DuoTrackingInitConfigParametersW : public ConfigParametersW{

//public :

//    void insert_widgets() override{}
//    void init_and_register_widgets() override{}
//    void create_connections() override{}
//    void late_update_ui() override{}
//};


//class DuoTrackingConfigParametersW : public ConfigParametersW{

//public :

//    ExTransformationW m_transfo;

//    ExSliderFloatW m_slGain;
//    ExSliderFloatW m_slExposure;
//    ExSliderFloatW m_slLed;

//    ExRadioButtonW m_rbVerticalFlip;
//    ExRadioButtonW m_rbHorizontalFlip;
//    ExRadioButtonW m_rbNoFlip;

//    ExSliderIntegerW m_slMinColor;
//    ExSliderIntegerW m_slMaxColor;
//    ExFloatSpinBoxW m_dsbFactorColor;

//    ExSliderIntegerW m_slMinDistance;
//    ExSliderIntegerW m_slMaxDistance;

//    ExFloatSpinBoxW m_dsbSizePts;

//    ExSpinBoxW m_sbMode;
//    ExSliderIntegerW m_slPreFilterCap;
//    ExSliderIntegerW m_slSadWindowSize;
//    ExSliderIntegerW m_slUniqenessRatio;
//    ExSliderIntegerW m_slSpeckleWindowSize;
//    ExSliderIntegerW m_slSpeckleRange;


//    void insert_widgets() override{
//        add_widget(ui::F::gen(ui::L::VB(), {m_transfo()}, LStretch{true}, LMargins{true}, QFrame::NoFrame));

//        add_widget(m_slGain());
//        add_widget(m_slExposure());
//        add_widget(m_slLed());

//        add_widget(ui::F::gen(ui::L::HB(),
//            {ui::W::txt("Flip:"), m_rbVerticalFlip(), m_rbHorizontalFlip(), m_rbNoFlip()}, LStretch{true}, LMargins{true}, QFrame::Box));
//        add_widget(m_slMinColor());
//        add_widget(m_slMaxColor());

//        add_widget(ui::F::gen(ui::L::HB(),
//            {ui::W::txt("Factor col:"), m_dsbFactorColor(), ui::W::txt("Size pts:"), m_dsbSizePts()},LStretch{true}, LMargins{true}, QFrame::Box));

//        add_widget(m_slMinDistance());
//        add_widget(m_slMaxDistance());

//        add_widget(ui::F::gen(ui::L::HB(),
//            {ui::W::txt("Dense mode:"), m_sbMode()}, LStretch{true}, LMargins{true}, QFrame::Box));

//        add_widget(m_slPreFilterCap());
//        add_widget(m_slSadWindowSize());
//        add_widget(m_slUniqenessRatio());
//        add_widget(m_slSpeckleWindowSize());
//        add_widget(m_slSpeckleRange());
//    }

//    void init_and_register_widgets() override{

//        // register ui elements
//        DsbSettings s1 = DsbSettings{MinV<qreal>{-10000.}, V<qreal>{0.001}, MaxV<qreal>{10000.}, StepV<qreal>{0.001}, 3};
//        DsbSettings s2 = DsbSettings{MinV<qreal>{-10000.}, V<qreal>{-0.001}, MaxV<qreal>{10000.}, StepV<qreal>{0.001}, 3};
//        DsbSettings s3 = DsbSettings{MinV<qreal>{-10000.}, V<qreal>{0.001}, MaxV<qreal>{10000.}, StepV<qreal>{0.001}, 3};
//        m_inputUiElements["transformation"] = m_transfo.init_widget("Transformation", {s1,s1,s1}, {s2,s2,s2}, {s3,s3,s3}, true);
//        m_inputUiElements["gain"] = m_slGain.init_widget("Gain", MinV<qreal>{0.}, V<qreal>{22.}, MaxV<qreal>{100.}, StepV<qreal>{0.1});
//        m_inputUiElements["exposure"] = m_slExposure.init_widget("Exposure", MinV<qreal>{0.}, V<qreal>{42}, MaxV<qreal>{100.}, StepV<qreal>{0.1});
//        m_inputUiElements["led"] = m_slLed.init_widget("LED", MinV<qreal>{0.}, V<qreal>{25}, MaxV<qreal>{100.}, StepV<qreal>{0.1});

//        m_inputUiElements["vertical_flip"]  =  m_rbVerticalFlip.init_widget("Vertical", false, true);
//        m_inputUiElements["horizontal_flip"]  =  m_rbHorizontalFlip.init_widget("Horizontal", false, true);
//        m_inputUiElements["no_flip"]  =  m_rbNoFlip.init_widget("None", true, true);

//        m_inputUiElements["factor_col"] = m_dsbFactorColor.init_widget(MinV<qreal>{0.}, V<qreal>{1.}, MaxV<qreal>{10.}, StepV<qreal>{0.01},2);
//        m_inputUiElements["min_col"] = m_slMinColor.init_widget("Threshold min color", MinV<int>{-1}, V<int>{20}, MaxV<int>{256}, StepV<int>{1});
//        m_inputUiElements["max_col"] = m_slMaxColor.init_widget("Threshold max color", MinV<int>{-1}, V<int>{255}, MaxV<int>{256}, StepV<int>{1});

//        m_inputUiElements["min_dist"] = m_slMinDistance.init_widget("Threshold min distance", MinV<int>{0}, V<int>{0}, MaxV<int>{10000}, StepV<int>{10});
//        m_inputUiElements["max_dist"] = m_slMaxDistance.init_widget("Threshold max distance", MinV<int>{0}, V<int>{2500}, MaxV<int>{10000}, StepV<int>{10});

//        m_inputUiElements["size_pts"] = m_dsbSizePts.init_widget(MinV<qreal>{0.0001}, V<qreal>{0.0005}, MaxV<qreal>{10.}, StepV<qreal>{0.0001},4);

//        m_inputUiElements["dense_mode"]                = m_sbMode.init_widget(MinV<int>{0}, V<int>{0}, MaxV<int>{3}, StepV<int>{1});
//        m_inputUiElements["dense_pre_filter_cap"]      = m_slPreFilterCap.init_widget("Pre filter cap", MinV<int>{1}, V<int>{63}, MaxV<int>{63}, StepV<int>{1});
//        m_inputUiElements["dense_sad_window_size"]     = m_slSadWindowSize.init_widget("Sad windows size", MinV<int>{2}, V<int>{4}, MaxV<int>{10}, StepV<int>{1});
//        m_inputUiElements["dense_uniqeness_ratio"]     = m_slUniqenessRatio.init_widget("Uniqeness ratio", MinV<int>{1}, V<int>{1}, MaxV<int>{10}, StepV<int>{1});
//        m_inputUiElements["dense_speckle_window_size"] = m_slSpeckleWindowSize.init_widget("Speckle windowsize", MinV<int>{0}, V<int>{60}, MaxV<int>{256}, StepV<int>{1});
//        m_inputUiElements["dense_speckle_range"]       = m_slSpeckleRange.init_widget("Speckle range", MinV<int>{0}, V<int>{20}, MaxV<int>{32}, StepV<int>{1});
//    }


//    void create_connections() override{}
//    void late_update_ui() override{}
//};

//}
