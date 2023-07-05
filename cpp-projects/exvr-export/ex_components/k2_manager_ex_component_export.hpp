
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
#include "utility/export.hpp"

// local
#include "k2_manager_ex_component.hpp"

extern "C"{
    DECL_EXPORT tool::ex::K2ManagerExComponent* create_k2_manager_ex_component();
    DECL_EXPORT int update_cloud_k2_manager_ex_component(tool::ex::K2ManagerExComponent *c, int idC, float *vertices, float *colors);
    DECL_EXPORT void update_mesh_k2_manager_ex_component(tool::ex::K2ManagerExComponent *c, int idC, float *vertices, float *colors, int* idTris);
    DECL_EXPORT void update_bodies_k2_manager_ex_component(tool::ex::K2ManagerExComponent *c,
            int idC, int *bodiesInfo, int *jointsType, int *jointsState, float *jointsPosition, float *jointsRotation);
    DECL_EXPORT void ask_for_frame_k2_manager_ex_component(tool::ex::K2ManagerExComponent *c);
}


