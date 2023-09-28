
/***********************************************************************************
** exvr-export                                                                    **
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


// std
#include <iostream>
#include <memory>
//#include <thread>
//#include <chrono>
//#include <iostream>
//#include <vector>
//#include <map>
//#include <any>


#include "ex_components/k4_manager_ex_component.hpp"
#include "ex_components/k4_manager_ex_component_export.hpp"

using namespace tool;
using namespace tool::ex;
using namespace tool::camera;



auto test_k4_manager() -> void{

    std::vector<K4VertexMeshData> meshData;
    meshData.resize(600000);

    tool::ex::ExExperiment exp;
    exp.generate_logger("","");

    auto manager = create_k4_manager_ex_component();
    manager->set_exp(&exp);
    manager->set(ParametersContainer::InitConfig, "debug_bypass", 0);

    std::string pathN = "D:/DEV/EPFL/lnco-exvr/exvr/toolset/cpp-projects/_build/bin/k4-scaner-manager/config/network/network_FLORIAN-PC.config";
    manager->set(ParametersContainer::Dynamic, "path_network_settings_file", pathN);

    Logger::message("initialize\n");
    auto res = manager->initialize();
    Logger::message(std::format("initialize ret {}\n", res));
    manager->start_experiment();
    manager->start_routine();

    size_t currentFrameId = 0;
    auto nbCameras = manager->get<int>(ParametersContainer::Dynamic, "nb_connections");
    std::vector<int> framesId;
    framesId.resize(nbCameras);
    std::fill(framesId.begin(), framesId.end(), 0);

    for(int ii = 0; ii < 100; ++ii){

        manager->pre_update();
        manager->update();
        manager->post_update();

        auto nb = manager->get<int>(ParametersContainer::Dynamic, "nb_frames_received");
        Logger::message(std::format("update [{}]\n", nb));

        for(size_t jj = 0; jj < nbCameras; ++jj){

            auto ret = manager->get_cloud_frame_data(jj,framesId[jj], meshData.data());
            if(std::get<0>(ret)){
                framesId[jj] = std::get<1>(ret);
                Logger::message(std::format("re c[{}] [{}][{}][{}]\n", jj, std::get<0>(ret),std::get<1>(ret),std::get<2>(ret)));
            }
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
    }

    manager->stop_routine();
    manager->stop_experiment();
    manager->clean();
    delete manager;
}

#include "ex_resources/k4_volumetric_video_ex_resource_export.hpp"
#include "ex_components/k4_volumetric_video_ex_component_export.hpp"

auto test_k4_video()  -> void{

    auto videoResource = create_k4_volumetric_video_ex_resource();

    Logger::no_file_init();
    Logger::get()->message_signal.connect([&](std::string message){
        std::cout << message;
    });
    Logger::get()->error_signal.connect([&](std::string error){
        std::cerr << error;
    });
    Logger::message("initialize\n");


    std::string path = "D:/DATA/Kinect videos/adriel.kvid";
    std::cout << "init video from path " << path << "\n";
    videoResource->set(ParametersContainer::Dynamic, "path_file", path);
    if(!videoResource->initialize()){
        std::cerr << "Cannot initialize video from path: " << path << "\n";
        return;
    }




    int nbCameras = get_cameras_nb_k4_volumetric_video_ex_resource(videoResource);
    std::cout << "nb cameras: " << nbCameras << "\n";
    for(int ii = 0; ii < nbCameras; ++ii){

        std::cout << "model: " << videoResource->video.get_camera_data(ii)->transform << "\n";

        std::cout << "cam " << ii << " nb frames " << get_nb_frames_k4_volumetric_video_ex_resource(videoResource, ii) << "\n";
        std::cout << "duration: " << get_duration_ms_k4_volumetric_video_ex_resource(videoResource,ii) << "\n";
    }

    return;
    auto videoComponent = create_k4_volumetric_video_ex_component(videoResource);


    std::vector<tool::camera::K4VertexMeshData> data;

    for(int idC = 0; idC < nbCameras; ++idC){
        for(int idF = 0; idF < get_nb_frames_k4_volumetric_video_ex_resource(videoResource, idC); ++idF){

            auto cData = videoResource->video.get_camera_data(idC);
            auto &d = cData->frames[idF];
            //            std::cout <<
            //                " idc " << d->idCapture << " vvc " << d->validVerticesCount << " " <<
            //                " eco " << d->encodedColorData.size() <<
            //                " ede " << d->encodedDepthData.size() <<
            //                " ein " << d->encodedInfraData.size() <<
            //                " pco " << d->encodedProcessedColorData.size() <<
            //                " ecl " << d->encodedCloudData.size() <<
            //                " imu " << d->imuSample.has_value() <<
            //                " aud " << d->audioFrames.size() <<
            //                " cal " << d->calibration.has_value() << "\n";


                        data.resize(d->validVerticesCount);;
            std::cout << "uncompress " << idC << " " << idF << " ";
            int success = uncompress_frame_vmd_k4_volumetric_video_ex_component(videoComponent, idC, idF, data.data());
            std::cout << "-> " << success << "\n";
        }
    }


    delete videoResource;
    delete videoComponent;
}


int main(int argc, char *argv[]){

    test_k4_video();


    //    auto scaner = create_scaner_video_file();

//    auto loaded = load_scaner_video_file(scaner, "G:/vid1.kvid");
//    if(!loaded){
//        std::cerr << "Cannot load file.\n";
//        return -1;
//    }

//    int duration = get_duration_ms_scaner_video_file(scaner);
//    int nbCameras = get_camera_nb_scaner_video_file(scaner);
//    std::cout << "Duration: " << duration << "\n";
//    std::cout << "nbCameras: " << nbCameras << "\n";

//    for(int ii = 0; ii < nbCameras; ++ii){
//        int size = get_camera_cloud_size_video_file(scaner, ii);
//        std::cout << "Cam " << ii << " has cloud size of " << size << "\n";
//    }

//    std::vector<float> vertices(512*424*3);
//    std::vector<float> colors(512*424*4);
//    for(int ii = 0; ii < duration; ii+=300){
//        std::cout << "Time: " << ii << "\n";
//        for(int jj = 0; jj < nbCameras; ++jj){
//            update_cloud_data_scaner_video_file(scaner, jj, ii, 300, false, vertices.data(), colors.data());
//        }
//    }


//    delete scaner;






//    DECL_EXPORT int get_duration_ms_scaner_video_file(tool::camera::ScanerVideoFile *scanerVideoFile);
//    DECL_EXPORT int get_camera_nb_scaner_video_file(tool::camera::ScanerVideoFile *scanerVideoFile);
//    DECL_EXPORT int get_camera_cloud_size_video_file(tool::camera::ScanerVideoFile *scanerVideoFile, int idC);
//    DECL_EXPORT void get_cloud_model_transform_scaner_video_file(tool::camera::ScanerVideoFile *scanerVideoFile, int idC, float *model);

//    DECL_EXPORT int update_cloud_data_scaner_video_file(tool::camera::ScanerVideoFile *scanerVideoFile, int idC, int timeMs, int maxDiffMs, int loop, float *vertices, float *colors);

//    DECL_EXPORT int get_nb_bodies_scaner_video_file(tool::camera::ScanerVideoFile *scanerVideoFile, int idC);
//    DECL_EXPORT int is_body_tracked_scaner_video_file(tool::camera::ScanerVideoFile *scanerVideoFile, int idC, int idBody);
//    DECL_EXPORT int is_body_restricted_scaner_video_file(tool::camera::ScanerVideoFile *scanerVideoFile, int idC, int idBody);
//    DECL_EXPORT int is_body_hand_detected_scaner_video_file(tool::camera::ScanerVideoFile *scanerVideoFile, int idC, int idBody, int leftHand);
//    DECL_EXPORT int is_body_hand_confident_scaner_video_file(tool::camera::ScanerVideoFile *scanerVideoFile, int idC, int idBody, int leftHand);
//    DECL_EXPORT void body_joints_positions_scaner_video_file(tool::camera::ScanerVideoFile *scanerVideoFile, int idC, int idBody, float *positions);
//    DECL_EXPORT void body_joints_rotations_scaner_video_file(tool::camera::ScanerVideoFile *scanerVideoFile, int idC, int idBody, float *rotations);
//    DECL_EXPORT void body_joints_states_scaner_video_file(tool::camera::ScanerVideoFile *scanerVideoFile, int idC, int idBody, int *states);



//    ScanerVideoFile video;
////    std::string name = "L:/Users/Florian/_VIDEOS/7-kinects.kvid";
//    std::string name = "G:/vid1.kvid";
////    std::string name = "G:/7-kinects.kvid";
////    std::string name = "C:/Users/lance/Desktop/test.kvid";
//    std::cout << "Load video " << name << "\n";
//    if(!video.load(name)){
//        std::cout << "Cannot load video : " << name << "\n";
//        return -1;
//    }

//    const bool displayModelMatrix = false;

//    std::cout << "#### Video: " << name << "\n";
//    std::cout << "Duration: " << video.duration.count()*0.000001 << "ms\n";// (start: " << video.startingTimeStamp << " end: " << video.endingTimeStamp << " )\n";
//    std::cout << "Nb Cameras: " << video.nbCameras << ", cloud nb: " << video.nbClouds << "\n";

//    for(size_t ii = 0; ii < video.nbCameras; ++ii){
//        std::cout << "\nCamera " << ii << "\n";
//        const size_t sizeCloud = (video.data.count(ii) == 0) ? 0 : video.data[ii].size();
//        const bool hasIntrincis = video.intrinsics.count(ii) != 0;
//        const bool hasModel = video.models.count(ii) != 0;
//        const size_t sizeBodiesId = (video.bodiesId.count(ii) == 0) ? 0 : video.bodiesId[ii].size();

//        std::cout << "Nb clouds : " << sizeCloud << "\n";
//        if(hasIntrincis){
//            const auto &vInt = video.intrinsics[ii];
//            std::cout << "Intrinsics : " << vInt[0] << " " << vInt[1] << " " << vInt[2] << " " << vInt[3] << "\n";
//        }else{
//            std::cout << "No intrinsics\n";
//        }

//        if(hasModel){
//            if(displayModelMatrix){
//                std::cout << "Model maxtrix:\n" << video.models[ii] << "\n";
//            }else{
//                std::cout << "Has model matrix\n";
//            }
//        }else{
//            std::cout << "No model matrix\n";
//        }

//        std::cout << "Nb bodies id : " << sizeBodiesId << "\n";

//        std::cout <<  "## Cloud data ##\n";

//        size_t firstCloudId = 0;
//        std::int64_t firstTimeStamp = 0;
//        for(size_t jj = 0; jj < sizeCloud; ++jj){
//            auto &cloud = video.data[ii][jj];
//            if(jj == 0){
//                firstCloudId = cloud.id;
//                firstTimeStamp = cloud.timeStamp;
//                std::cout << cloud.id-firstCloudId << " " << cloud.timeStamp-firstTimeStamp << "\n";
//            }else{
//                auto &pCloud = video.data[ii][jj-1];
//                std::cout << "Frame: " << cloud.id <<  " -> " <<  (cloud.id - pCloud.id) << " " << duration_cast<milliseconds>(nanoseconds(cloud.timeStamp-pCloud.timeStamp)).count() << "ms " << cloud.colors.size() << " " << cloud.depth.size() <<  "\n";
//            }
//        }
//    }


//    auto durationMs = video.duration.count()*0.000001;
//    for(size_t ii = 0; ii < video.nbCameras; ++ii){ //

//        std::cout << "\nCamera " << ii << "\n";
//        if(video.data[ii].size() > 0){
//            auto t1 = video.data[ii][0].timeStamp;
//            auto t2 = video.data[ii][video.data[ii].size()-1].timeStamp;
//            std::cout << "Diff " << (t2-t1)*0.00001 << "ms\n";
//        }


//        for(int jj = 0; jj < durationMs; jj+=30){
//            auto frame = video.get_data(ii, jj);
//            std::cout << "Time wanted: " << jj << "ms, cloud id " << frame.idCloud << " with found time " << frame.foundTimeMs << "\n";
//        }
//    }

    return 0;
}





//// std
//#include <iostream>
//#include <fstream>
//#include <thread>
//#include <memory>
//#include <string>
//#include <sstream>
//#include <string>
//#include <fstream>
//#include <vector>
//#include <chrono>

//// base
//#include "utility/time.hpp"


//// local
////#include "kinect2.hpp"
//#include "python_script_component.hpp"

//using namespace std::chrono_literals;

//void py_compot_test(){
//    std::cout << "1\n";
//    tool::ex::PythonScriptComponent component;
//    component.initialize();
//    //    std::cout << "2\n";
//    ////    const auto pathScript = get<std::string>(ParametersContainer::InitConfig, "path_script");
//    ////    const auto moduleName = get<std::string>(ParametersContainer::InitConfig, "class_name");

//    //    component.update_parameter<std::string>(ParametersContainer::InitConfig, "path_script", "G:/DEV/Projets/RealityEx/test2.py");
//    //    component.update_parameter<std::string>(ParametersContainer::InitConfig, "class_name", "DefaultComponent");
//    //    component.initialize();
//    std::cout << "3\n";
//    //    component.start_experiment();
//    ////    component.start_routine();
//    ////    component.set_visibility(10);
//    ////    component.stop_routine();
//    ////    component.stop_experiment();
//}

//#include "video_saver_component.hpp"

//#include <filesystem>
//#include <opencv2/world.hpp>
//#include <opencv2/video.hpp>
//#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
//#include <opencv2/highgui/highgui.hpp>  // Video write

//int chapter1_main(int argc, char *argv[]){

//    namespace fs = std::filesystem;
//    std::string videoPath =  "D:/test.avi";//get<std::string>(ParametersContainer::InitConfig, "path_video");
//    auto path = fs::path(videoPath);
//    const auto stem = path.stem();


//    cv::Size S = cv::Size(500,500);

//    int id = 0;
//    while(fs::exists(path)){
//        path = fs::path(path).replace_filename((fs::path(stem).concat("_" + std::to_string(id++))).replace_extension(".avi"));
//        if(id > 500){
//            return 0;
//        }
//    }

//    std::cout << "2\n";
//    cv::VideoWriter video;

//    video.open(path.string(),CV_FOURCC('M', 'J', 'P', 'G'), 30., S, true);

//    cv::Mat mat(S.width,S.height, CV_8UC3, cv::Scalar(0,0,255));
//    video << mat;

//    std::cout << "not exist: " << path << "\n";
//    return 0;
//}
