
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

#include "k2_manager_ex_component_export.hpp"

// base
#include "geometry/point3.hpp"
#include "geometry/point4.hpp"

using namespace tool;
using namespace tool::geo;
using namespace tool::ex;
using namespace tool::camera;

K2ManagerExComponent *create_k2_manager_ex_component(){
    return new K2ManagerExComponent();
}

int update_cloud_k2_manager_ex_component(K2ManagerExComponent *c, int idC, float *vertices, float *colors){
    return static_cast<int>(c->update_cloud(static_cast<size_t>(idC), reinterpret_cast<Pt3f*>(vertices), reinterpret_cast<Pt4f*>(colors)));
}

void update_mesh_k2_manager_ex_component(K2ManagerExComponent *c, int idC, float *vertices, float *colors, int *idTris){
    c->update_mesh(static_cast<size_t>(idC), reinterpret_cast<Pt3f*>(vertices), reinterpret_cast<Pt4f*>(colors), reinterpret_cast<Pt3<int>*>(idTris));
}

void update_bodies_k2_manager_ex_component(K2ManagerExComponent *c, int idC, int *bodiesInfo, int *jointsType, int *jointsState, float *jointsPosition, float *jointsRotation){
    c->update_bodies(static_cast<size_t>(idC), bodiesInfo, jointsType, jointsState, reinterpret_cast<Pt3f*>(jointsPosition), reinterpret_cast<Pt3f*>(jointsRotation));
}

void ask_for_frame_k2_manager_ex_component(K2ManagerExComponent *c){
    c->ask_for_frame();
}
