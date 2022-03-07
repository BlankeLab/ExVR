
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

// local
#include "exvr/ex_component.hpp"
#include "utility/export.hpp"

extern "C"{

    DECL_EXPORT void delete_ex_component(tool::ex::ExComponent *c);

    DECL_EXPORT int initialize_ex_component(tool::ex::ExComponent*c);
    DECL_EXPORT void clean_ex_component(tool::ex::ExComponent*c);

    DECL_EXPORT void start_experiment_ex_component(tool::ex::ExComponent*c);    
    DECL_EXPORT void stop_experiment_ex_component(tool::ex::ExComponent*c);

    DECL_EXPORT void set_current_config_ex_component(tool::ex::ExComponent*c, const char* configName);
    DECL_EXPORT void update_from_current_config_ex_component(tool::ex::ExComponent*c);
    DECL_EXPORT void pre_start_routine_ex_component(tool::ex::ExComponent*c);
    DECL_EXPORT void start_routine_ex_component(tool::ex::ExComponent*c);
    DECL_EXPORT void post_start_routine_ex_component(tool::ex::ExComponent*c);
    DECL_EXPORT void stop_routine_ex_component(tool::ex::ExComponent*c);

    DECL_EXPORT void on_gui_ex_component(tool::ex::ExComponent*c);
    DECL_EXPORT void pre_update_ex_component(tool::ex::ExComponent*c);
    DECL_EXPORT void update_ex_component(tool::ex::ExComponent*c);
    DECL_EXPORT void post_update_ex_component(tool::ex::ExComponent*c);

    DECL_EXPORT void set_visibility_ex_component(tool::ex::ExComponent*c,int visible);
    DECL_EXPORT void set_update_state_ex_component(tool::ex::ExComponent*c,int doUpdate);
    DECL_EXPORT void play_ex_component(tool::ex::ExComponent*c);
    DECL_EXPORT void pause_ex_component(tool::ex::ExComponent*c);

    DECL_EXPORT void update_parameter_from_gui_ex_component(tool::ex::ExComponent*c, const char* argToUpdate);
    DECL_EXPORT void action_from_gui_ex_component(tool::ex::ExComponent*c, int initConfig, const char* action);

    DECL_EXPORT void call_slot_ex_component(tool::ex::ExComponent*c, int index);

    DECL_EXPORT int contains_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName);

    DECL_EXPORT void update_parameter_bool_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName, int value);
    DECL_EXPORT void update_parameter_int_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName, int value);
    DECL_EXPORT void update_parameter_float_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName, float value);
    DECL_EXPORT void update_parameter_double_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName, double value);
    DECL_EXPORT void update_parameter_string_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName, const char* value);

    DECL_EXPORT int get_parameter_int_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName);
    DECL_EXPORT float get_parameter_float_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName);
    DECL_EXPORT double get_parameter_double_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName);
    DECL_EXPORT const char* get_parameter_string_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName);

    DECL_EXPORT int get_size_parameter_array_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName);
    DECL_EXPORT void get_parameter_array_int_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName, int* values);
    DECL_EXPORT void get_parameter_array_float_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName, float* values);
    DECL_EXPORT void get_parameter_array_double_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName, double* values);

//    DECL_EXPORT void update_parameter_array_bool_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName, bool* values, int size);
    DECL_EXPORT void update_parameter_array_int_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName, int* values, int size);
    DECL_EXPORT void update_parameter_array_float_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName, float* values, int size);
    DECL_EXPORT void update_parameter_array_double_ex_component(tool::ex::ExComponent*c, int containerId, const char* paramName, double* values, int size);

    DECL_EXPORT void init_callbacks_ex_component(
        tool::ex::ExComponent*c,
        StackTraceCB stackTraceCB,
        LogCB logCB,
        LogWarningCB logWarningCB,
        LogErrorCB logErrorCB,
        EllapsedTimeExpMsCB ellapsedTimeExpMsCB,
        EllapsedTimeRoutineMsCB ellapsedTimeRoutineMsCB,
        GetCB getCB,
        IsInitializedCB isInitializedCB,
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
}

