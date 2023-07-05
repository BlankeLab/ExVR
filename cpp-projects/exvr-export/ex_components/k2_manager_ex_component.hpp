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
#include <any>
#include <map>

// base
#include "geometry/matrix4.hpp"
#include "exvr/ex_component.hpp"
#include "grabber_controller.hpp"

namespace tool::ex {


class K2ManagerExComponent : public ExComponent{

public:

    ~K2ManagerExComponent() override;

    bool initialize() override;
    void clean() override;
    void start_experiment() override;

    size_t update_cloud(size_t idCamera, geo::Pt3f *points, geo::Pt4f *colors);
    void update_mesh(size_t idCamera, geo::Pt3f *points, geo::Pt4f *colors, geo::Pt3<std::int32_t> *idTris);
    void update_bodies(size_t idCamera, int *bodiesInfo, int *jointsType, int *jointsState, geo::Pt3f *jointsPosition, geo::Pt3f *jointsRotation);

    void ask_for_frame();

    std::vector<tool::scan::GrabberControllerUP> grabbers;
    std::unordered_map<size_t, camera::K2CloudDisplayData*> grabbersCloudData;
    std::unordered_map<size_t, camera::K2MeshDisplayData*> grabbersMeshData;
    std::unordered_map<size_t, camera::K2BodiesDisplayData*> grabbersBodiesData;
    tool::network::UdpReaderManagerUP udpManager = nullptr;

    std::unordered_map<std::uint64_t, int> bodiesId;
    std::vector<camera::K2GrabberTargetInfo> networkInfos;
    std::vector<geo::Mat4d> calibrationsM;

    bool cleaned = false;
    bool debugBypass = false;

    std::chrono::nanoseconds startExperimentTime;
    std::chrono::nanoseconds lastFrameGetTime;

    std::optional<camera::K2Settings> grabbersSettings;
};
}
