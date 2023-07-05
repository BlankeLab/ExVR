
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
#include "k4_volumetric_video_ex_resource.hpp"

extern "C"{

    DECL_EXPORT tool::ex::K4VolumetricVideoExResource *create_k4_volumetric_video_ex_resource();
    DECL_EXPORT int get_cameras_nb_k4_volumetric_video_ex_resource(tool::ex::K4VolumetricVideoExResource *vvR);
    DECL_EXPORT int get_nb_frames_k4_volumetric_video_ex_resource(tool::ex::K4VolumetricVideoExResource *vvR, int idC);
    DECL_EXPORT float get_duration_ms_k4_volumetric_video_ex_resource(tool::ex::K4VolumetricVideoExResource *vvR, int idC);
    DECL_EXPORT void get_camera_transform_k4_volumetric_video_ex_resource(tool::ex::K4VolumetricVideoExResource *vvR, int idC, float *model);
    DECL_EXPORT int get_id_frame_from_time_ms_k4_volumetric_video_ex_resource(tool::ex::K4VolumetricVideoExResource *vvR, int idC, float timeMs);
    DECL_EXPORT int get_valid_vertices_count_k4_volumetric_video_ex_resource(tool::ex::K4VolumetricVideoExResource *vvR, int idC, int idF);
    DECL_EXPORT int get_audio_data_total_size_k4_volumetric_video_ex_resource(tool::ex::K4VolumetricVideoExResource *vvR, int idC);
}
