
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


#include "k2_volumetric_video_ex_resource_export.hpp"

// base
#include "geometry/point3.hpp"
#include "geometry/point4.hpp"

using namespace tool;
using namespace tool::ex;
using namespace tool::geo;
using namespace tool::camera;

K2VolumetricVideoExResource *create_k2_volumetric_video_ex_resource(){
    return new K2VolumetricVideoExResource();
}

void delete_k2_volumetric_video_ex_resource(K2VolumetricVideoExResource *k2vvr){
    delete k2vvr;
}

int load_k2_volumetric_video_ex_resource(K2VolumetricVideoExResource *k2vvr, const char *pathVideoFile){
    return k2vvr->load(pathVideoFile) ? 1 : 0;
}

int get_duration_ms_k2_volumetric_video_ex_resource(K2VolumetricVideoExResource *k2vvr){
    return static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(k2vvr->duration).count());
}

int get_camera_nb_k2_volumetric_video_ex_resource(K2VolumetricVideoExResource *k2vvr){
    return static_cast<int>(k2vvr->data.size());
}

int get_camera_cloud_size_video_file(K2VolumetricVideoExResource *k2vvr, int idC){

    const size_t idCam = static_cast<size_t>(idC);
    if(idCam < k2vvr->nbCameras){
        return static_cast<int>(k2vvr->data[idCam].size());
    }

    return -1;
}

int update_cloud_data_k2_volumetric_video_ex_resource(K2VolumetricVideoExResource *k2vvr, int idC, int timeMs, int maxDiffMs, int loop, float *vertices, float *colors){

    // retrieve data corresponding to timestamp
    size_t idCam = static_cast<size_t>(idC);

    if(loop == 1){
        timeMs = timeMs % get_duration_ms_k2_volumetric_video_ex_resource(k2vvr);
    }

    auto frame = k2vvr->get_data(idCam, timeMs);
    if(frame.closestCloud == nullptr){
        return -1; // invalid frame
    }

    if(std::abs(frame.foundTimeMs-timeMs) > maxDiffMs){
        return -1; // frame too old
    }

    // check if new frame id
    if(static_cast<int>(frame.idCloud) == k2vvr->cameras[idCam]->currentCloudId){
        return -1; // same frame, do nothing
    }

    // uncompress data
    k2vvr->uncompress(idCam, frame.closestCloud);

    // copy vertices
    auto camData = k2vvr->cameras[idCam].get();
    std::copy(camData->cloudData.points->begin(),
              camData->cloudData.points->begin() + static_cast<int>(camData->cloudData.sizePts), reinterpret_cast<geo::Pt3f*>(vertices));

    // copy colors
    // TODO: 4 bytes?
    auto cols =reinterpret_cast<geo::Pt4f*>(colors);
    for(size_t ii = 0; ii < camData->cloudData.sizePts; ++ii){
        cols[ii].x() = (*camData->cloudData.colors)[ii].x();
        cols[ii].y() = (*camData->cloudData.colors)[ii].y();
        cols[ii].z() = (*camData->cloudData.colors)[ii].z();
        cols[ii].w() = 1.f;
    }

    // return cloud size
    return static_cast<int>(camData->cloudData.sizePts);
}

void get_cloud_model_transform_k2_volumetric_video_ex_resource(K2VolumetricVideoExResource *k2vvr, int idC, float *model){
    if(k2vvr->models.count(static_cast<size_t>(idC)) == 0){
        return;
    }
    auto tr = k2vvr->models[static_cast<size_t>(idC)].conv<float>();
    std::copy(std::begin(tr.array), std::end(tr.array), model);
}


int get_nb_bodies_k2_volumetric_video_ex_resource(K2VolumetricVideoExResource *k2vvr, int idC){

    if(k2vvr->bodiesId.count(static_cast<size_t>(idC)) == 0){
        return 0;
    }
    return static_cast<int>(k2vvr->bodiesId[static_cast<size_t>(idC)].size());
}

int is_body_tracked_k2_volumetric_video_ex_resource(K2VolumetricVideoExResource *k2vvr, int idC, int idBody){

    auto camData = k2vvr->cameras[static_cast<size_t>(idC)].get();
    for(auto &bodyData : camData->bodiesData.bodies){
        if(bodyData.id == static_cast<size_t>(idBody)){
            return bodyData.tracked ? 1 : 0;
        }
    }

    return 0;
}

int is_body_restricted_k2_volumetric_video_ex_resource(K2VolumetricVideoExResource *k2vvr, int idC, int idBody){

    auto camData = k2vvr->cameras[static_cast<size_t>(idC)].get();
    for(auto &bodyData : camData->bodiesData.bodies){
        if(bodyData.id == static_cast<size_t>(idBody)){
            return bodyData.restricted ? 1 : 0;
        }
    }

    return 0;
}

int is_body_hand_detected_k2_volumetric_video_ex_resource(K2VolumetricVideoExResource *k2vvr, int idC, int idBody, int leftHand){

    auto camData = k2vvr->cameras[static_cast<size_t>(idC)].get();
    for(auto &bodyData : camData->bodiesData.bodies){
        if(bodyData.id == static_cast<size_t>(idBody)){
            if(leftHand == 1){
                return static_cast<int>(bodyData.leftHandState);
            }else{
                return static_cast<int>(bodyData.rightHandState);
            }
        }
    }

    return 0;
}

int is_body_hand_confident_k2_volumetric_video_ex_resource(K2VolumetricVideoExResource *k2vvr, int idC, int idBody, int leftHand){

    auto camData = k2vvr->cameras[static_cast<size_t>(idC)].get();
    for(auto &bodyData : camData->bodiesData.bodies){
        if(bodyData.id == static_cast<size_t>(idBody)){
            return (leftHand == 1 ? bodyData.leftHandHightConfidence : bodyData.rightHandHightConfidence) ? 1 : 0;
        }
    }

    return 0;
}

void body_joints_positions_k2_volumetric_video_ex_resource(K2VolumetricVideoExResource *k2vvr, int idC, int idBody, float *positions){

    auto camData = k2vvr->cameras[static_cast<size_t>(idC)].get();
    for(auto &bodyData : camData->bodiesData.bodies){
        if(bodyData.id == static_cast<size_t>(idBody)){
            for(size_t ii = 0; ii < bodyData.joints.size(); ++ii){
                const auto type = static_cast<K2BodyJointType>(ii);
                std::copy(std::begin(bodyData.joints[type].pos.array),  std::end(bodyData.joints[type].pos.array), positions + ii*3);
            }
            return;
        }
    }
}

void body_joints_rotations_k2_volumetric_video_ex_resource(K2VolumetricVideoExResource *k2vvr, int idC, int idBody, float *rotations){

    auto camData = k2vvr->cameras[static_cast<size_t>(idC)].get();
    for(auto &bodyData : camData->bodiesData.bodies){
        if(bodyData.id == static_cast<size_t>(idBody)){
            for(size_t ii = 0; ii < bodyData.joints.size(); ++ii){
                const auto type = static_cast<K2BodyJointType>(ii);

                geo::Pt3d angle;
                const auto q = bodyData.joints[type].rotQuaternion.conv<double>();
                double sinr_cosp = 2. * q.w() * q.x() + q.y() * q.z();
                double cosr_cosp = 1. - 2. * (q.x() * q.x() + q.y() * q.y());
                angle.x() = rad_2_deg(std::atan2(sinr_cosp, cosr_cosp));

                double sinp = 2. * (q.w() * q.y() - q.z() * q.x());
                if (std::abs(sinp) >= 1){
                    angle.y() = rad_2_deg(std::copysign(PI<double> / 2., sinp)); // use 90 degrees if out of range
                }else{
                    angle.y() = rad_2_deg(std::asin(sinp));
                }
                // yaw (z-axis rotation)
                double siny_cosp = 2. * (q.w() * q.z() + q.x() * q.y());
                double cosy_cosp = 1. - 2. * (q.y() * q.y() + q.z() * q.z());
                angle.z() = rad_2_deg(std::atan2(siny_cosp, cosy_cosp));

                rotations[ii*3 + 0] = static_cast<float>(angle.x());
                rotations[ii*3 + 1] = static_cast<float>(angle.y());
                rotations[ii*3 + 2] = static_cast<float>(angle.z());
//                std::copy(std::begin(bodyData.joints[type].rotQuaternion.v),  std::end(bodyData.joints[type].rotQuaternion.v), rotations + ii*4);
            }
            return;
        }
    }
}

void body_joints_states_k2_volumetric_video_ex_resource(K2VolumetricVideoExResource *k2vvr, int idC, int idBody, int *states){
    auto camData = k2vvr->cameras[static_cast<size_t>(idC)].get();
    for(auto &bodyData : camData->bodiesData.bodies){
        if(bodyData.id == static_cast<size_t>(idBody)){
            for(size_t ii = 0; ii < bodyData.joints.size(); ++ii){
                const auto type = static_cast<K2BodyJointType>(ii);
                states[ii] = static_cast<int>(bodyData.joints[type].state);
            }
            return;
        }
    }
}
