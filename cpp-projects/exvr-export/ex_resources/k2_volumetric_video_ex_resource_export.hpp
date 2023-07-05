
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
#include "k2_volumetric_video_ex_resource.hpp"

extern "C"{

    DECL_EXPORT tool::ex::K2VolumetricVideoExResource *create_k2_volumetric_video_ex_resource();
    DECL_EXPORT void delete_k2_volumetric_video_ex_resource(tool::ex::K2VolumetricVideoExResource *k2vvr);
    DECL_EXPORT int load_k2_volumetric_video_ex_resource(tool::ex::K2VolumetricVideoExResource *k2vvr, const char* pathVideoFile);

    DECL_EXPORT int get_duration_ms_k2_volumetric_video_ex_resource(tool::ex::K2VolumetricVideoExResource *k2vvr);
    DECL_EXPORT int get_camera_nb_k2_volumetric_video_ex_resource(tool::ex::K2VolumetricVideoExResource *k2vvr);
    DECL_EXPORT int get_camera_cloud_size_video_file(tool::ex::K2VolumetricVideoExResource *k2vvr, int idC);
    DECL_EXPORT void get_cloud_model_transform_k2_volumetric_video_ex_resource(tool::ex::K2VolumetricVideoExResource *k2vvr, int idC, float *model);

    DECL_EXPORT int update_cloud_data_k2_volumetric_video_ex_resource(tool::ex::K2VolumetricVideoExResource *k2vvr, int idC, int timeMs, int maxDiffMs, int loop, float *vertices, float *colors);

    DECL_EXPORT int get_nb_bodies_k2_volumetric_video_ex_resource(tool::ex::K2VolumetricVideoExResource *k2vvr, int idC);
    DECL_EXPORT int is_body_tracked_k2_volumetric_video_ex_resource(tool::ex::K2VolumetricVideoExResource *k2vvr, int idC, int idBody);
    DECL_EXPORT int is_body_restricted_k2_volumetric_video_ex_resource(tool::ex::K2VolumetricVideoExResource *k2vvr, int idC, int idBody);
    DECL_EXPORT int is_body_hand_detected_k2_volumetric_video_ex_resource(tool::ex::K2VolumetricVideoExResource *k2vvr, int idC, int idBody, int leftHand);
    DECL_EXPORT int is_body_hand_confident_k2_volumetric_video_ex_resource(tool::ex::K2VolumetricVideoExResource *k2vvr, int idC, int idBody, int leftHand);
    DECL_EXPORT void body_joints_positions_k2_volumetric_video_ex_resource(tool::ex::K2VolumetricVideoExResource *k2vvr, int idC, int idBody, float *positions);
    DECL_EXPORT void body_joints_rotations_k2_volumetric_video_ex_resource(tool::ex::K2VolumetricVideoExResource *k2vvr, int idC, int idBody, float *rotations);
    DECL_EXPORT void body_joints_states_k2_volumetric_video_ex_resource(tool::ex::K2VolumetricVideoExResource *k2vvr, int idC, int idBody, int *states);
}
