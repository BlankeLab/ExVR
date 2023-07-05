
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

#pragma once


// std
#include <numeric>
#include <chrono>
#include <set>

// turbo-jpeg
#include <turbojpeg.h>

// base
#include "exvr/ex_resource.hpp"
#include "geometry/matrix4.hpp"
#include "data/integers_encoder.hpp"
#include "camera/kinect2/k2_types.hpp"

namespace tool::ex {

struct CloudData{

    std::uint32_t id;
    std::int64_t timeStamp;
    std::vector<std::uint32_t> depth;
    std::vector<unsigned char> colors;
    std::array<tool::camera::K2BodyInfos,6> bodies;
};

struct CameraData{

    tjhandle jpegDecompressor;
    data::IntegersEncoder depthCompressor;

    int currentCloudId = -1;

    std::vector<unsigned char> bufferDecompressColor;
    std::vector<std::uint16_t> decodedDepth;
    std::vector<bool> validityDepth2Rgb;
    std::vector<std::uint32_t> validIdPerPointMesh;
    tool::camera::K2CloudDisplayData cloudData;
    tool::camera::K2BodiesDisplayData bodiesData;

    CameraData();
};

struct FrameData{
    size_t idCloud = 0;
    std::int64_t foundTimeMs = 0;
    CloudData *closestCloud = nullptr;
};

struct K2VolumetricVideoExResource : public ExResource{

    std::vector<std::unique_ptr<CameraData>> cameras;

    std::chrono::nanoseconds duration;

    size_t nbClouds  = 0;
    size_t nbCameras = 0;

    std::unordered_map<size_t, std::vector<float>> intrinsics;
    std::unordered_map<size_t, std::vector<CloudData>> data;
    std::unordered_map<size_t, geo::Mat4d> models;
    std::unordered_map<size_t, std::set<int>> bodiesId;

    K2VolumetricVideoExResource(){
        // TODO...
    }

    FrameData get_data(size_t idCamera, int targetTimeMilliseconds);
    bool load(const std::string &path);
    void uncompress(size_t idCamera, CloudData *cloud);
};
}






