
/*******************************************************************************
** exvr-components                                                            **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

// std
#include <iostream>
#include <fstream>
#include <thread>
#include <memory>
#include <string>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>

// base
#include "utility/time.hpp"


// local
//#include "kinect2.hpp"
#include "python_script_component.hpp"

using namespace std::chrono_literals;

void py_compot_test(){
    std::cout << "1\n";
    tool::ex::PythonScriptComponent component;
    component.initialize();
//    std::cout << "2\n";
////    const auto pathScript = get<std::string>(ParametersContainer::InitConfig, "path_script");
////    const auto moduleName = get<std::string>(ParametersContainer::InitConfig, "class_name");

//    component.update_parameter<std::string>(ParametersContainer::InitConfig, "path_script", "G:/DEV/Projets/RealityEx/test2.py");
//    component.update_parameter<std::string>(ParametersContainer::InitConfig, "class_name", "DefaultComponent");
//    component.initialize();
    std::cout << "3\n";
//    component.start_experiment();
////    component.start_routine();
////    component.set_visibility(10);
////    component.stop_routine();
////    component.stop_experiment();
}

#include "video_saver_component.hpp"

#include <filesystem>
#include <opencv2/world.hpp>
#include <opencv2/video.hpp>
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>  // Video write

int chapter1_main(int argc, char *argv[]){

    namespace fs = std::filesystem;
    std::string videoPath =  "D:/test.avi";//get<std::string>(ParametersContainer::InitConfig, "path_video");
    auto path = fs::path(videoPath);
    const auto stem = path.stem();


    cv::Size S = cv::Size(500,500);

    int id = 0;
    while(fs::exists(path)){
        path = fs::path(path).replace_filename((fs::path(stem).concat("_" + std::to_string(id++))).replace_extension(".avi"));
        if(id > 500){
            return 0;
        }
    }

    std::cout << "2\n";
    cv::VideoWriter video;

    video.open(path.string(),CV_FOURCC('M', 'J', 'P', 'G'), 30., S, true);

    cv::Mat mat(S.width,S.height, CV_8UC3, cv::Scalar(0,0,255));
    video << mat;

    std::cout << "not exist: " << path << "\n";
    return 0;
}
