
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

// base
#include "exvr/ex_component.hpp"
#include "camera/kinect4/k4_volumetric_video.hpp"

// local
#include "ex_resources/k4_volumetric_video_ex_resource.hpp"

namespace tool::ex {

class K4VolumetricVideoExComponent : public ExComponent{
public:

    tool::camera::K4VolumetricVideo *resource = nullptr;
    std::vector<std::unique_ptr<tool::camera::K4FrameUncompressor>> uncompressors; // dedicated uncompressors for enabling multithreads when using the same video resource

    K4VolumetricVideoExComponent(tool::ex::K4VolumetricVideoExResource *resourceExport) : resource(&resourceExport->video){
    }

    bool initialize() override{
        auto nbCams = resource->nb_cameras();
        audioData.resize(nbCams);
        uncompressors.resize(nbCams);
        for(auto &uncompressor : uncompressors){
            uncompressor = std::make_unique<tool::camera::K4FrameUncompressor>();
        }

        return true;
    }

    std::vector<std::vector<float>> audioData;
};
}
