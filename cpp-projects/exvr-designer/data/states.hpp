
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

// base
#include "utility/tuple_array.hpp"

// qt-utility
#include "data/unity_types.hpp"

// local
#include "flow_elements/flow_element.hpp"

namespace tool::ex {

    using namespace std::literals::string_view_literals;

    enum class ExpLauncherCommand : int{
        Load=0, Play, Pause, Stop, Next, Previous, UpdateComponent, UpdateConnector, Quit,
        Action, GoToSpecificInstanceElement, PlayPause, PlayDelay, Error, Clean,
        SizeEnum
    };

    enum class ExpLauncherState : int{
        NotStarted=0,
        Starting,
        Idle,
        LoadingExperiment,
        Closing,
        SizeEnum
    };

    enum class ExpState : int{
        NotLoaded = 0,
        Loaded,
        Running,
        Paused,
        SizeEnum
    };

    using Description = std::string_view;
    using TExpLauncherState = std::tuple<ExpLauncherState, Description>;
    static constexpr TupleArray<ExpLauncherState::SizeEnum, TExpLauncherState> expLauncherStates = {{
        TExpLauncherState
        {ExpLauncherState::NotStarted,          "[ExVR-exp] ExVR-exp not launched."sv},
        {ExpLauncherState::Starting,            "[ExVR-exp] ExVR-exp is starting."sv},
        {ExpLauncherState::Idle,                "[ExVR-exp] Idle."sv},
        {ExpLauncherState::LoadingExperiment,   "[ExVR-exp] ExVR-exp is loading an experiment..."sv},
        {ExpLauncherState::Closing,             "[ExVR-exp] ExVR-exp is closing."sv},
    }};

    [[maybe_unused]] static Description get_description(ExpLauncherState s) {
        return expLauncherStates.at<0,1>(s);
    }

    using TExpState = std::tuple<ExpState, Description>;
    static constexpr TupleArray<ExpState::SizeEnum, TExpState> expStates = {{
        TExpState
        {ExpState::NotLoaded,    "[ExVR-exp] <b>No experiment loaded.</b>"sv},
        {ExpState::Loaded,       "[ExVR-exp] <b>Experiment loaded</b>"sv},
        {ExpState::Running,      "[ExVR-exp] <b>Experiment is running</b>"sv},
        {ExpState::Paused,       "[ExVR-exp] <b>Experiment paused</b>"sv},
    }};

    [[maybe_unused]] static Description get_description(ExpState s) {
        return expStates.at<0,1>(s);
    }

    [[maybe_unused]] static QString to_command_id(ExpLauncherCommand cmd){
        return QString::number(static_cast<int>(cmd));
    }

    enum class FontStyle : int {
        Normal = 0, Bold = 1, Italic =2, Bold_and_italic =3,
        SizeEnum};

    enum class Alignment : int {
        UpperLeft = 0, UpperCenter = 1, UpperRight =2, MiddleLeft =3, MiddleCenter = 4, MiddleRight = 5, LowerLeft = 6, LowerCenter =7, LowerRight = 8,
        SizeEnum};

    enum class TextOverlflow : int {
        Wrap = 0, Overflow = 1,
        SizeEnum};


    struct States{

        States(QString nVersion) : numVersion(nVersion){
            auto split = numVersion.split(".");
            majorNumVersion = split[0].toInt();
            minorNumVersion = split[1].toInt();
        }

        void reset(){
            neverLoaded         = true;
            currentElementKey   = -1;
            currentConditionKey = -1;
            currentElementName  = "";
            currentTypeSpecificInfo = "";
            experimentTimeS = 0.0;
            currentElementTimeS = 0.0;
            currentIntervalEndTimeS = 0.0;
            currentOrder = "";
            nbCalls = "";
        }

        const QString numVersion;
        int majorNumVersion;
        int minorNumVersion;
        int maximumDeepLevel = -1;
        unsigned int randomizationSeed = 0;

        QString loadedExpDesignerVersion = "unknow";
        QString designerPathUsedForLoadedExp = "unknow";


        QString currentExpfilePath;
        QString currentName = "unknow";
        QString currentMode = "designer";
        QString currentInstanceName = "debug-instance.xml";

        ExpLauncherState explauncherState = ExpLauncherState::NotStarted;
        ExpState expState = ExpState::NotLoaded;

        bool followsCurrentCondition = false;

        bool neverLoaded = true;
        int currentElementKey=-1;
        int currentConditionKey=-1;
        FlowElement::Type currentElementType;
        QString currentElementName = "";

        QString currentTypeSpecificInfo = "";
        double experimentTimeS = 0.0;
        double currentElementTimeS = 0.0;
        double currentIntervalEndTimeS = 0.0;
        QString currentOrder = "";
        QString nbCalls = "";


    };

}
