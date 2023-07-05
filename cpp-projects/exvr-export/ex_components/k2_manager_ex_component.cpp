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

#include "k2_manager_ex_component.hpp"

// std
#include <chrono>
#include <format>

// base
#include "utility/format.hpp"
#include "geometry/point4.hpp"
#include "geometry/quaternion.hpp"
#include "network/network_utility.hpp"
#include "camera/kinect2/k2_config_files.hpp"

using namespace std::chrono;
using namespace tool;
using namespace tool::ex;
using namespace tool::camera;
using namespace tool::geo;
using namespace tool::network;

//std::uint64_t jointPosX : 32, jointPosY : 32;
//std::uint64_t jointPosZ : 32, jointQuaX : 32;
//std::uint64_t jointQuaY : 32, jointQuaZ : 32;
//std::uint64_t jointQuaW : 32, jointState : 2, jointid : 5,
//              bodyId : 17, bodyTracked : 1, bodyRestricted : 1,
//              leftHandState : 2, rightHandState : 2, leftHandConfidence : 1, rightHandConfidence : 1;


K2ManagerExComponent::~K2ManagerExComponent(){
    clean();
}

bool K2ManagerExComponent::initialize(){

    // init dynamic values    
    set<int>(ParametersContainer::Dynamic, "network_config_file_loaded", 0);
    set<int>(ParametersContainer::Dynamic, "calibration_config_file_loaded", 0);
    set<int>(ParametersContainer::Dynamic, "camera_config_file_loaded", 0);    
    set<int>(ParametersContainer::Dynamic, "grabbers_count", 0);

    // read config files
    // # network
    bool networkFileLoaded = false;
    const auto pathNetwork = get<std::string>(ParametersContainer::Dynamic, "path_network");
    if(pathNetwork.size() > 0){
        networkInfos = K2ConfigFiles::read_manager_network_config_file(pathNetwork);
        if(networkInfos.size() > 0){
            networkFileLoaded = true;
        }else{
            log_error("Invalid network config file.");
        }
    }else{
        log_error("Empty network config file path.");
    }

    // # calibration
    bool calibFileLoaded = false;
    const auto pathCalib   = get<std::string>(ParametersContainer::Dynamic, "path_calib");
    if(pathCalib.size() > 0){
        calibrationsM = K2ConfigFiles::read_manager_calibration_file(pathCalib);
        if(calibrationsM.size() > 0){
            calibFileLoaded = true;
        }else{
            log_error("Invalid calibration config file.");
        }
    }else{
        log_message("No calib config file defined. Idendity matrices will be used for calibration instead.");
        for(size_t ii = 0; ii < networkInfos.size(); ++ii){
            calibrationsM.push_back(Mat4d::identity());
        }
    }

    // # settings
    bool cameraFileLoaded = false;
    const auto pathCamera  = get<std::string>(ParametersContainer::Dynamic, "path_camera");
    if(pathCamera.size() > 0){
        grabbersSettings = K2ConfigFiles::read_grabber_settings_config_file(pathCamera);
        if(grabbersSettings.has_value()){
            cameraFileLoaded = true;
        }else{
            log_error("Invalid camera config file.");
        }
    }

    // check integrity
    if(calibrationsM.size() != networkInfos.size()){

        log_error(std::format(
            "Network and calibration files are not compatible: {} nodes against {} nodes, idendity matrices will be used for calibration instead.",
            std::to_string(calibrationsM.size()), std::to_string(networkInfos.size()))
        );

        calibrationsM.resize(networkInfos.size());
        for(auto &m : calibrationsM){
            m = geo::Mat4d::identity();
        }
    }

    // transmit calibration matrices
    for(size_t ii = 0; ii < calibrationsM.size(); ++ii){
        std::vector<float> m(16, 0.f);
        for(size_t jj = 0; jj < 16; ++jj){
            m[jj] = static_cast<float>(calibrationsM[ii].array[jj]);
        }
        std::string name = "tr_" + std::to_string(ii);
        set_array(ParametersContainer::Dynamic, name, m);
    }

    set<int>(ParametersContainer::Dynamic, "network_config_file_loaded",      networkFileLoaded ? 1 : 0);
    set<int>(ParametersContainer::Dynamic, "calibration_config_file_loaded",  calibFileLoaded ? 1 : 0);
    set<int>(ParametersContainer::Dynamic, "camera_config_file_loaded",       cameraFileLoaded ? 1 : 0);
    set<int>(ParametersContainer::Dynamic, "grabbers_count", static_cast<int>(networkInfos.size()));

    if(!networkFileLoaded){
        log_error("Network file must be defined before continuing.");
        return false;
    }

    debugBypass = get<int>(ParametersContainer::InitConfig, "debug_bypass") == 1;
    if(debugBypass){
        return true;
    }

    // init udp manager
    auto localInterfaces = Interface::list_local_interfaces(Protocol::ipv4);
    udpManager = std::make_unique<UdpReaderManager>(localInterfaces);
    udpManager->message_signal.connect([&](std::string message){
        log_message(message);
    });
    udpManager->error_signal.connect([&](std::string error){
        log_error(error);
    });

    const int mode = get<int>(ParametersContainer::InitConfig, "mode");
    for(size_t ii = 0; ii < networkInfos.size(); ++ii){

        log_message(std::format("Init grabber: {} {}", std::to_string(ii), networkInfos[ii].name));

        // init grabber
        auto grabber = std::make_unique<scan::GrabberController>(udpManager.get(), &localInterfaces, ii, networkInfos[ii]);

        // set connections
        grabber->stack_trace_message_signal.connect([&](std::string trace){
            stack_trace_log(trace);
        });
        grabber->frame_dropped_signal.connect([&](size_t id, size_t idFrame, size_t /** totalNbFramesLost*/, size_t /**totalNbPacketsLost*/){
            // ask for new frame
            log_error(std::format("Frame n° {}, lost from interface {}", idFrame, id));
        });
        grabber->frame_processed_signal.connect([&](size_t id, size_t totalNbFrames, std::int64_t timeStampGet, std::int64_t timeProcessing){
            (void)timeProcessing;
            // ask for new frame
            set<int>(ParametersContainer::Dynamic, std::format("id_frame_{}", id), static_cast<int>(totalNbFrames));

            lastFrameGetTime = nanoseconds(high_resolution_clock::now().time_since_epoch());
            std::int64_t getMsTime = duration_cast<milliseconds>(lastFrameGetTime - nanoseconds(timeStampGet)).count();
            set<int>(ParametersContainer::Dynamic, std::format("id_time_{}", id), static_cast<int>(getMsTime));
        });

        grabber->update_cloud_data_signal.connect([&](size_t id, camera::K2CloudDisplayData *cloudData){
            if(!grabbersCloudData.count(id)){
                grabbersCloudData[id] = cloudData;
            }
        });
        grabber->update_mesh_data_signal.connect([&](size_t id, camera::K2MeshDisplayData *meshData){
            if(!grabbersMeshData.count(id)){
                grabbersMeshData[id] = meshData;
            }
        });
        grabber->update_bodies_data_signal.connect([&](size_t id, camera::K2BodiesDisplayData *bodiesData){
            if(!grabbersBodiesData.count(id)){
                grabbersBodiesData[id] = bodiesData;
            }
        });

        // store grabber
        grabbers.emplace_back(std::move(grabber));
    }

    if(!udpManager->enable_reading()){
        log_error("Cannot enable reading from UDP");
        return false;
    }

    // connect grabbers and open cameras
    for(auto &grabber : grabbers){

        log_message(std::format("Connect grabber: {}",grabber->idC));
        if(!grabber->connect_grabber()){
            return false;
        }

        log_message(std::format("Open grabber camera: {}", grabber->idC));
        if(mode == 0){
            grabber->open_camera_mode(camera::K2FrameRequest::compressed_color_cloud);
        }else{
            grabber->open_camera_mode(camera::K2FrameRequest::compressed_color_mesh);
        }

        if(grabbersSettings.has_value()){
            log_message(std::format("Set grabber camera settings: {}", grabber->idC));
            grabber->set_camera_parameters(grabbersSettings.value());
        }
    }

    return true;
}

void K2ManagerExComponent::ask_for_frame(){

    if(debugBypass){
        return;
    }

    for(auto &grabber : grabbers){

        auto now = nanoseconds(high_resolution_clock::now().time_since_epoch());
        std::int64_t diffTime = duration_cast<milliseconds>(nanoseconds(now) - lastFrameGetTime).count();
        set<int>(ParametersContainer::Dynamic, "id_diff_time_" + std::to_string(grabber->idC), static_cast<int>(diffTime));

        grabber->ask_new_frame();
    }
}

void K2ManagerExComponent::clean(){

    if(cleaned || debugBypass){
        return;
    }

    log_message("Clean scene scaner component");

    // connect grabbers and open cameras
    for(auto &grabber : grabbers){
        grabber->close_camera_mode();
    }
    std::this_thread::sleep_for (std::chrono::milliseconds(50));

    for(auto &grabber : grabbers){
        grabber->disconnect_grabber();
    }
    std::this_thread::sleep_for (std::chrono::milliseconds(50));

    for(auto &grabber : grabbers){
        grabber->quit_threads();
    }
    std::this_thread::sleep_for (std::chrono::milliseconds(50));

    if(udpManager != nullptr){
        udpManager->disable_reading();
        udpManager = nullptr;
    }

    grabbers.clear();
    grabbersCloudData.clear();
    grabbersMeshData.clear();
    grabbersBodiesData.clear();

    cleaned = true;
}

void K2ManagerExComponent::start_experiment(){
    startExperimentTime = nanoseconds(high_resolution_clock::now().time_since_epoch());
}

size_t K2ManagerExComponent::update_cloud(size_t idCamera, geo::Pt3f *points, geo::Pt4f *colors){

    set<int>(ParametersContainer::Dynamic, std::format("nb_pts_{}",idCamera), 0);
    set<int>(ParametersContainer::Dynamic, std::format("nb_tris_{}", idCamera), 0);

    if(debugBypass){
        return 0;
    }

    if(grabbersCloudData.count(idCamera) == 0){
        return 0;
    }

    grabbersCloudData[idCamera]->dataLocker.lock();
    const auto sizePts = grabbersCloudData[idCamera]->sizePts;
    std::copy(grabbersCloudData[idCamera]->points->begin(), grabbersCloudData[idCamera]->points->begin() + static_cast<int>(sizePts), points);
    for(size_t ii = 0; ii < sizePts; ++ii){
        colors[ii].x() = (*grabbersCloudData[idCamera]->colors)[ii].x();
        colors[ii].y() = (*grabbersCloudData[idCamera]->colors)[ii].y();
        colors[ii].z() = (*grabbersCloudData[idCamera]->colors)[ii].z();
        colors[ii].w() = 1.f;
    }

    set<int>(ParametersContainer::Dynamic, std::format("nb_pts_{}",idCamera), static_cast<int>(sizePts));
    grabbersCloudData[idCamera]->dataLocker.unlock();

    return sizePts;
}

void K2ManagerExComponent::update_mesh(size_t idCamera, Pt3f *points, Pt4f *colors, Pt3<std::int32_t> *idTris){

    set<int>(ParametersContainer::Dynamic, std::format("nb_pts_{}",idCamera), 0);
    set<int>(ParametersContainer::Dynamic, std::format("nb_tris_{}", idCamera), 0);

    if(debugBypass){
        return;
    }

    if(grabbersMeshData.count(idCamera) == 0){
        return;
    }

    grabbersMeshData[idCamera]->dataLocker.lock();
    const auto sizePts = grabbersMeshData[idCamera]->sizePts;
    const auto sizeTri = grabbersMeshData[idCamera]->sizeTris;
    std::copy(grabbersMeshData[idCamera]->points->begin(), grabbersMeshData[idCamera]->points->begin() + static_cast<int>(sizePts), points);
    for(size_t ii = 0; ii < sizePts; ++ii){
        colors[ii].x() = (*grabbersMeshData[idCamera]->colors)[ii].x();
        colors[ii].y() = (*grabbersMeshData[idCamera]->colors)[ii].y();
        colors[ii].z() = (*grabbersMeshData[idCamera]->colors)[ii].z();
        colors[ii].w() = 1.f;
    }
    std::copy(grabbersMeshData[idCamera]->triangles->begin(), grabbersMeshData[idCamera]->triangles->begin() + static_cast<int>(sizeTri), reinterpret_cast<Pt3<std::uint32_t>*>(idTris));

    set<int>(ParametersContainer::Dynamic, std::format("nb_pts_{}",idCamera), static_cast<int>(sizePts));
    set<int>(ParametersContainer::Dynamic, std::format("nb_tris_{}", idCamera), static_cast<int>(sizeTri));

    grabbersMeshData[idCamera]->dataLocker.unlock();
}

void K2ManagerExComponent::update_bodies(size_t idCamera, int *bodiesInfo, int *jointsType, int *jointsState, Pt3f *jointsPosition, Pt3f *jointsRotation){


    if(grabbersBodiesData.count(idCamera) == 0 || debugBypass){ // not data for this camera

        for(size_t ii = 0; ii < camera::k2_bodies_count; ++ii){

            bodiesInfo[ii*3+0] = -1;
            bodiesInfo[ii*3+1] = -1;
            bodiesInfo[ii*3+2] = 0;
            bodiesInfo[ii*3+3] = 0;

            for(size_t jj = 0; jj < camera::k2_body_joints_count; ++jj){
                jointsState[ii] = static_cast<int>(camera::K2TrackingStateT::not_tracked);
                jointsPosition[ii] = {0.f,0.f,0.f};
                jointsRotation[ii] = {0.f,0.f,0.f};
            }
        }
        return;
    }


    grabbersBodiesData[idCamera]->dataLocker.lock();
    size_t idJoint = 0;    

    for(const auto &body : grabbersBodiesData[idCamera]->bodies){
        for(const auto &joint : body.joints){

            jointsType[idJoint] = static_cast<int>(joint.first);
            jointsState[idJoint] = static_cast<int>(joint.second.state);

            if((joint.second.state != camera::K2TrackingStateT::not_tracked) && body.tracked){

                jointsPosition[idJoint] = joint.second.pos;

                const auto q = joint.second.rotQuaternion.conv<double>();
                geo::Quatd quat(q.x(), q.y(), q.z(), q.w());
                geo::Pt3d angle = euler_angles(quat) * d180_PI<double>;


                jointsRotation[idJoint] = angle.conv<float>();
            }else{
                jointsPosition[idJoint] = geo::Pt3f();
                jointsRotation[idJoint] = geo::Pt3f();
            }

            ++idJoint;
        }
    }

    for(size_t ii = 0; ii < grabbersBodiesData[idCamera]->bodies.size(); ++ii){

        auto idB = grabbersBodiesData[idCamera]->bodies[ii].id;
        bodiesInfo[ii*3 + 0] = static_cast<int>(idB & 0xffffffffUL);
        bodiesInfo[ii*3 + 1] = static_cast<int>(idB >> 32);
        bodiesInfo[ii*3 + 2] = grabbersBodiesData[idCamera]->bodies[ii].tracked ? 1 : 0;
        bodiesInfo[ii*3 + 3] = grabbersBodiesData[idCamera]->bodies[ii].restricted ? 1 : 0;

    }
    grabbersBodiesData[idCamera]->dataLocker.unlock();
}

