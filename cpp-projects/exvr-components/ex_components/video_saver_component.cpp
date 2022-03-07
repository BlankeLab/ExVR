
/*******************************************************************************
** exvr-components                                                            **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

#include "video_saver_component.hpp"

// std
#include <filesystem>


using namespace tool::ex;

int VideoSaverComponent::add_frame(int width, int height, char *data, std::string format, int dataTYpe){

    if(m_video == nullptr){
        return 0;
    }

    if(m_videoSize.width != width || m_videoSize.height != height){
        log_error("Can add frame to video: invalid size: " + std::to_string(width) + " "+  std::to_string(height) + " instead of " + std::to_string(m_videoSize.width) + " "+  std::to_string(m_videoSize.height));
        return 0;
    }


    if(dataTYpe == 0){
        if(format == "rgba"){
            cv::Mat src(height, width, CV_8UC3, reinterpret_cast<unsigned*>(&data[0]));
            cv::Mat dst;
            cv::flip(src,dst, 0);
            (*m_video) << dst;
            return 1;
        }
    }
    return 0;
}


void VideoSaverComponent::start_experiment(){

    namespace fs = std::filesystem;
    std::string videoPath =  get<std::string>(ParametersContainer::InitConfig, "path_video");
    auto path = fs::path(videoPath);
    const auto stem = path.stem();

    int id = 0;
    while(fs::exists(path)){
        path = fs::path(path).replace_filename((fs::path(stem).concat("_" + std::to_string(id++))).replace_extension(".avi"));
        if(id > 5000){
            return;
        }
    }
    m_videoPath = path.string();
    m_videoSize = cv::Size(get<int>(ParametersContainer::InitConfig, "width"), get<int>(ParametersContainer::InitConfig, "height"));

    m_video = std::make_unique<cv::VideoWriter>();
    if(!m_video->open(m_videoPath,CV_FOURCC('M', 'J', 'P', 'G'), 1.0*get<int>(ParametersContainer::InitConfig, "fps"), m_videoSize, true)){
        log_error("Cannot write video to: " + m_videoPath);
        m_video = nullptr;
    }
}

void VideoSaverComponent::stop_experiment(){
    if(m_video){
        m_video->release();
        m_video = nullptr;
    }
}

