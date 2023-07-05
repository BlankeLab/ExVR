
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

#include "k4_volumetric_video_ex_component_export.hpp"

using namespace tool::ex;
using namespace tool::geo;
using namespace tool::camera;


using namespace tool;

K4VolumetricVideoExComponent *create_k4_volumetric_video_ex_component(K4VolumetricVideoExResource *resourceExport){
    return new K4VolumetricVideoExComponent(resourceExport);
}

int uncompress_frame_c4f_k4_volumetric_video_ex_component(K4VolumetricVideoExComponent *vvC, int idC, int idFrame, tool::geo::Pt3f *vertices, tool::geo::Pt4f *colors){
    if(auto frame = vvC->resource->get_compressed_frame(idC, idFrame).lock()){
        return vvC->uncompressors[idC]->uncompress(frame.get(),  vertices, colors) ? 1 : 0;
    }
    return 0;
}

int uncompress_frame_c3i_k4_volumetric_video_ex_component(K4VolumetricVideoExComponent *vvC, int idC, int idFrame, tool::geo::Pt3f *vertices, tool::geo::Pt4<uint8_t> *colors){
    if(auto frame = vvC->resource->get_compressed_frame(idC, idFrame).lock()){
        return vvC->uncompressors[idC]->uncompress(frame.get(),  vertices, colors) ? 1 : 0;
    }
    return 0;
}

int uncompress_frame_vmd_k4_volumetric_video_ex_component(K4VolumetricVideoExComponent *vvC, int idC, int idFrame, tool::camera::K4VertexMeshData *vertices){
    if(auto frame = vvC->resource->get_compressed_frame(idC, idFrame).lock()){
        return vvC->uncompressors[idC]->uncompress(frame.get(), vertices) ? 1 : 0;
    }
    return 0;
}

int process_audio_k4_volumetric_video_ex_component(K4VolumetricVideoExComponent *vvC, int idCamera){
    if(idCamera < vvC->audioData.size()){
        vvC->resource->get_audio_samples_all_channels(idCamera, vvC->audioData[idCamera]);
        return static_cast<int>(vvC->audioData[idCamera].size());
    }
    return -1;
}

void copy_audio_samples_k4_volumetric_video_ex_component(K4VolumetricVideoExComponent *volumetricVideoComponent, int idCamera, float *audioSamples){
    std::copy(std::begin(volumetricVideoComponent->audioData[idCamera]), std::end(volumetricVideoComponent->audioData[idCamera]), audioSamples);
}

