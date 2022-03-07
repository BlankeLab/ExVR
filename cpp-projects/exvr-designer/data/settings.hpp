
/***********************************************************************************
** exvr-designer                                                                  **
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

namespace tool::ex {

struct Settings{

    enum class ExpLauncherDisplayMode : int{
        OpenVR=0, Flat, FlatStereo
    };

    // debug
    bool debug = true;
    bool catchComponentsExceptions = false;
    bool csharpAddDebugInfo = false;

    // camera
    bool neutralX = false;
    bool neutralY = true;
    bool neutralZ = false;
    bool positionalTracking = true;

    // display
    ExpLauncherDisplayMode displayMode = ExpLauncherDisplayMode::OpenVR;
    int stereoCameraFOV = 60;
    bool fullscreen = false;
    int monitorId = 1;
    int resolutionId = 2;
    int customWidth = 1920;
    int customHeight = 1080;

    // input
    bool catchExternalKeyboardKeysEvents = false;

    void reset(){

        // debug
        debug = true;
        catchComponentsExceptions = false;
        csharpAddDebugInfo = false;

        // camera
        neutralX = false;
        neutralY = true;        
        neutralZ = false;
        positionalTracking = true;

        // display
        displayMode = ExpLauncherDisplayMode::OpenVR;
        stereoCameraFOV = 60;
        fullscreen = false;
        monitorId = 1;
        resolutionId = 2;
        customWidth = 1920;
        customHeight = 1080;
    }
};
}
