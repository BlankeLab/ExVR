
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
#include "exvr/ex_experiment.hpp"

// local
#include "utility/export.hpp"

extern "C"{

    DECL_EXPORT tool::ex::ExExperiment *create_ex_experiment();
    DECL_EXPORT void delete_ex_experiment(tool::ex::ExExperiment *e);


    DECL_EXPORT void init_callbacks_ex_experiment(
        tool::ex::ExExperiment *e,
        LogMessageCB logMessageCB,
        LogWarningCB logWarningCB,
        LogErrorCB logErrorCB,
        LogMessageIdCB logMessageIdCB,
        LogWarningIdCB logWarningIdCB,
        LogErrorIdCB logErrorIdCB,
        StackTraceCB stackTraceCB,
        EllapsedTimeExpMsCB ellapsedTimeExpMsCB,
        EllapsedTimeRoutineMsCB ellapsedTimeRoutineMsCB,
        GetCB getCB,
        IsVisibleCB isVisibleCB,
        IsUpdatingCB isUpdatingCB,
        IsClosedCB isClosedCB,
        NextCB nextCB,
        PreviousCB previousCB,
        CloseCB closeCB,
        SignalBoolCB signalBoolCB,
        SignalIntCB signalIntCB,
        SignalFloatCB signalFloatCB,
        SignalDoubleCB signalDoubleCB,
        SignalStringCB signalStringCB
    );

    DECL_EXPORT int init_logger_ex_experiment(
        tool::ex::ExExperiment *e,
        const char*pathDir,
        const char*fileName
    );

    DECL_EXPORT void init_logger_no_file_ex_experiment(tool::ex::ExExperiment *e);

    DECL_EXPORT void test_call_backs_ex_experiment(tool::ex::ExExperiment *e);

    DECL_EXPORT tool::Logger *get_logger_ptr_ex_experiment(tool::ex::ExExperiment *e);
    DECL_EXPORT void test_log_ex_experiment(tool::Logger *logger);


//    DECL_EXPORT void convert_to_chars(double *);
}
