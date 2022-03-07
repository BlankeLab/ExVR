
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

#include <iostream>

//#include "body_capture_export.hpp"

#include <thread>
#include <chrono>

#include <iostream>
#include <vector>
#include <map>
#include <any>

//#include "socket_communication_export.hpp"

//#include "scaner_video_file_export.hpp"

int main(int argc, char *argv[]){



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
