
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


#include "k2_volumetric_video_ex_resource.hpp"

// std
#include <set>
#include <iostream>
#include <sstream>

// base
#include "geometry/point3.hpp"
#include "geometry/point4.hpp"
#include "utility/io_fstream.hpp"

using namespace tool;
using namespace tool::ex;
using namespace tool::geo;
using namespace tool::camera;


CameraData::CameraData(){

    jpegDecompressor = tjInitDecompress();
    bufferDecompressColor.resize(k2_color_width*k2_color_height*4);
    decodedDepth.resize(k2_depth_count*2);

    validityDepth2Rgb.resize(k2_depth_count);
    validIdPerPointMesh.resize(k2_depth_count);
}


FrameData K2VolumetricVideoExResource::get_data(size_t idCamera, int targetTimeMilliseconds){

    // check nb of cameras
    if(idCamera >= data.size()){
        return {0, 0, nullptr};
    }

    // check cloud size
    if(data[idCamera].size() == 0){
        return {0, 0, nullptr};
    }

    // convert input researched time to nanoseconds
    auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(targetTimeMilliseconds)).count();

    FrameData frame;
    auto minTicks = std::numeric_limits<std::int64_t>::max();
    auto firstCameraTimeStamp = data[idCamera][0].timeStamp;
    for(size_t ii = 0; ii < data[idCamera].size(); ++ii){

        // get current cloud
        auto &cloud = data[idCamera][ii];

        // get current time of the cloud relative to start
        auto currT = cloud.timeStamp-firstCameraTimeStamp;

        // compute diff with input researched time
        auto diff = std::abs(currT-time);

        // update minimum difference
        if(minTicks > diff){
            minTicks = diff;
            frame.closestCloud = &cloud;
            frame.idCloud = ii;
            frame.foundTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::nanoseconds(currT)).count();
        }
    }

    return frame;
}

bool K2VolumetricVideoExResource::load(const std::string &path){

    std::ifstream inConfigFile(path, std::ios_base::binary);
    if(!inConfigFile.is_open()){
        std::cerr << "Cannot open video file with path: " << path << "\n";
        return false;
    }

    // read total nb of clouds
    read(inConfigFile, &nbClouds);

    // read nb of cameras
    read(inConfigFile, &nbCameras);


    for(size_t ii = 0; ii < nbCameras; ++ii){

        // read camera transformation
        geo::Mat4d model;
        read(inConfigFile, &model);
        models[ii] = model;

        // read camera intrinsics
        intrinsics[ii] = std::vector<float>(4);
        // focalLengthX
        // focalLengthY
        // principalPointX
        // principalPointY
        read_array(inConfigFile, intrinsics[ii].data(), 4);

        data[ii] = {};
        bodiesId[ii] = {};
    }

    // read clouds data
    size_t idCamera, sizeDepths;
    long unsigned int sizeColors;
    for(size_t ii = 0; ii < nbClouds; ++ii){

        CloudData cloudData;
        read(inConfigFile, &cloudData.timeStamp);
        read(inConfigFile, &idCamera);

        // read frame id
        read(inConfigFile, &cloudData.id); // frame id

        // read sizes
        read(inConfigFile, &sizeDepths);
        read(inConfigFile, &sizeColors);

        // read compressed data
        cloudData.depth.resize(sizeDepths);
        inConfigFile.read(reinterpret_cast<char*>(cloudData.depth.data()), static_cast<std::streamsize>(sizeDepths*sizeof (std::uint32_t)));
        cloudData.colors.resize(sizeColors);
        inConfigFile.read(reinterpret_cast<char*>(cloudData.colors.data()),  static_cast<std::streamsize>(sizeColors*sizeof (unsigned char)));

        // read bodies
        for(size_t jj = 0; jj < 6; ++jj){

            std::uint64_t id;
            size_t nbJoints;
            geo::Pt2f lean;
            int engaged, leanTracking, leftHandState, rightHandState, leftHandConfidence, rightHandConfidence;
            bool tracked, restricted;
            read(inConfigFile, &id);
            read(inConfigFile, &engaged);
            read(inConfigFile, &tracked);
            read(inConfigFile, &restricted);
            read(inConfigFile, &leanTracking);
            read(inConfigFile, &leftHandState);
            read(inConfigFile, &rightHandState);
            read(inConfigFile, &leftHandConfidence);
            read(inConfigFile, &rightHandConfidence);
            read(inConfigFile, &lean);
            read(inConfigFile, &nbJoints);

            camera::K2BodyInfos &body = cloudData.bodies[jj];
            body.id             = id;
            body.engaged        = static_cast<K2Detection>(engaged);
            body.tracked        = tracked;
            body.restricted     = restricted;
            body.leanTracking   = static_cast<K2TrackingStateT>(leanTracking);
            body.leftHandState  = static_cast<K2HandStateT>(leftHandState);
            body.rightHandState = static_cast<K2HandStateT>(rightHandState);
            body.leftHandHightConfidence  = leftHandConfidence == 1;
            body.rightHandHightConfidence = rightHandConfidence == 1;
            body.lean           = lean;

            // read joints
            for(size_t kk = 0; kk < nbJoints; ++kk){
                camera::K2BodyJoint joint;
                auto jointT = read<camera::K2BodyJointType>(inConfigFile);
                read(inConfigFile, &joint.pos);
                read(inConfigFile, &joint.rotQuaternion);
                read(inConfigFile, &joint.state);
                body.joints[jointT] = joint;
            }
        }

        data[idCamera].emplace_back(std::move(cloudData));
    }

    // retrieve bodies id
    for(auto &cloudsCam : data){

        if(bodiesId.count(cloudsCam.first) == 0){
            bodiesId[cloudsCam.first] = {};
        }

        for(auto &cloud : cloudsCam.second){
            for(auto &body : cloud.bodies){
                if(body.tracked){
                    auto currId = static_cast<int>(body.id);
                    if(bodiesId[cloudsCam.first].count(currId) == 0){
                        bodiesId[cloudsCam.first].insert(currId);
                    }
                }
            }
        }
    }

    // sort by timestamp
    auto sortCloud = [](const CloudData &lhs, const CloudData &rhs){
        return lhs.timeStamp < rhs.timeStamp;
    };
    for(auto &cameraData : data){
        auto &vec = cameraData.second;
        std::sort(std::begin(vec), std::end(vec), sortCloud);
    }


    // resize camera data
    cameras.clear();
    for(size_t ii = 0; ii < data.size(); ++ii){
        cameras.emplace_back(std::make_unique<CameraData>());
    }

    std::int64_t maxDuration = 0;
    for(size_t ii = 0; ii < nbCameras; ++ii){
        if(data[ii].size() > 1){
            const auto t1 = data[ii][0].timeStamp;
            const auto t2 = data[ii][data[ii].size()-1].timeStamp;
            const auto diff =(t2-t1);
            if(diff > maxDuration){
                maxDuration = diff;
            }
        }
    }
    duration = std::chrono::nanoseconds(maxDuration);

    return true;
}

void K2VolumetricVideoExResource::uncompress(size_t idCamera, CloudData *cloud){

    if(!cloud){
        // empty cloud
        return;
    }

    auto c = cameras[idCamera].get();
    c->currentCloudId = static_cast<int>(cloud->id);

    // get size
    const auto sizeDepthCoded   = cloud->depth.size();
    const auto sizeColorJpeg    = cloud->colors.size();

    // uncompress depths
    c->depthCompressor.decode(
        reinterpret_cast<std::uint32_t*>(cloud->depth.data()), sizeDepthCoded,
        reinterpret_cast<std::uint32_t*>(c->decodedDepth.data()), k2_depth_count/2
        );

    // uncompress colors
    tjDecompress2(
        c->jpegDecompressor,
        cloud->colors.data(), static_cast<unsigned long>(sizeColorJpeg),
        c->bufferDecompressColor.data(), k2_depth_width, 0/*pitch*/, k2_depth_height,
        TJPF_RGBA, TJFLAG_FASTDCT
        );

    // bodies infos
    std::copy(cloud->bodies.begin(), cloud->bodies.end(), c->bodiesData.bodies.begin());


    c->cloudData.sizePts = 0;

    size_t currentId = 0;
    size_t currentNbPoints = 0;

    float toF = 1.f/255.f;


    auto t4 = std::chrono::high_resolution_clock::now();

    auto &camIntrinsics = intrinsics[idCamera];
    for(size_t ii = 0; ii < k2_depth_height; ++ii){
        for(size_t jj = 0; jj < k2_depth_width; ++jj){

            if(c->decodedDepth[currentId] != 0){

                auto d = static_cast<float>(c->decodedDepth[currentId])*0.001f;

                // compute x,y coordinates from depth and intrinsics
                // x = (u - principalPointX) / focalLengthX * d;
                // y = (v - principalPointY) / focalLengthY * d;
                // z = d;
                (*c->cloudData.points)[currentNbPoints] = geo::Pt3f{
                    (jj - camIntrinsics[2]) / camIntrinsics[0] *d,
                    -(ii - camIntrinsics[3]) / camIntrinsics[1] *d,
                    d
                };

                // color
                const size_t idColor = currentId*4;
                (*c->cloudData.colors)[currentNbPoints] = {
                    c->bufferDecompressColor[idColor + 0]*toF,
                    c->bufferDecompressColor[idColor + 1]*toF,
                    c->bufferDecompressColor[idColor + 2]*toF
                };

                // process normal
                //                geo::Vec3f dir;
                //                if(ii != 0 && jj != 0 && ii != kinect2_depth_height -1 && jj != kinect2_depth_width-1){
                //                    const float dzdx = (c->decodedDepth[(jj+1)*kinect2_depth_width+(ii)] - c->decodedDepth[(jj-1)*kinect2_depth_width+(ii)]) / 2.f;
                //                    const float dzdy = (c->decodedDepth[(jj)*kinect2_depth_width+(ii+1)] - c->decodedDepth[(jj)*kinect2_depth_width+(ii-1)]) / 2.f;
                //                    dir = geo::Vec3f{-dzdx, -dzdy, 1.f};
                //                    dir.normalize();
                //                }
                //                (*c->cloudData.normals)[currentNbPoints] = dir;

                ++currentNbPoints;
            }
            ++currentId;
        }
    }



    c->cloudData.sizePts = currentNbPoints;


    auto t5 = std::chrono::high_resolution_clock::now();
    auto d4 = std::chrono::duration_cast<std::chrono::microseconds>(t5-t4);
    std::cout << "times: "  << d4.count()  <<"\n";
}


