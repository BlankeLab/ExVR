
/*******************************************************************************
** exvr-components                                                            **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

#pragma once

// std
#include <string>
#include <memory>

// base
#include "exvr/ex_component.hpp"

// opencv
#include <opencv2/highgui/highgui.hpp>  // Video write

namespace tool::ex{

class VideoSaverComponent : public ExComponent{

public :

    void start_experiment() override;
    void stop_experiment() override;

    int add_frame(int width, int height, char *data, std::string format, int dataTYpe);

private:

    std::unique_ptr<cv::VideoWriter> m_video = nullptr;
    std::string m_videoPath;
    cv::Size m_videoSize;
};
}
