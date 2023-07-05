
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

#include "k4_manager_ex_component_export.hpp"

using namespace tool;
using namespace tool::ex;

K4ManagerExComponent* create_k4_manager_ex_component(){
    return new K4ManagerExComponent();
}

void retrieve_cloud_frame_data_k4_manager_ex_component(
    tool::ex::K4ManagerExComponent *c, int idCamera, int idCurrentFrame,
    tool::camera::K4VertexMeshData *vertices, int *lastFrameState){

    auto ret = c->get_cloud_frame_data(idCamera, idCurrentFrame, vertices);
    lastFrameState[0] = std::get<0>(ret) ? 1 : 0;
    lastFrameState[1] = static_cast<int>(std::get<1>(ret));
    lastFrameState[2] = static_cast<int>(std::get<2>(ret));
}


// store frame
