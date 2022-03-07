
/*******************************************************************************
** lnco-exvr-export                                                           **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

#pragma once

// local
#include "exvr/ex_resource.hpp"
#include "utility/export.hpp"

extern "C"{

    DECL_EXPORT void delete_ex_resource(tool::ex::ExResource *r);
    DECL_EXPORT int initialize_ex_resource(tool::ex::ExResource*r);
    DECL_EXPORT void clean_ex_resource(tool::ex::ExResource*r);

    DECL_EXPORT void init_callbacks_ex_resource(
        tool::ex::ExResource *r,
        StackTraceCB strackTraceCB,
        LogCB logCB,
        LogWarningCB logWarningCB,
        LogErrorCB logErrorCB
    );

    DECL_EXPORT int contains_ex_resource(tool::ex::ExResource*r, const char* paramName);

    DECL_EXPORT void update_parameter_bool_ex_resource(tool::ex::ExResource*r, const char* paramName, int value);
    DECL_EXPORT void update_parameter_int_ex_resource(tool::ex::ExResource*r, const char* paramName, int value);
    DECL_EXPORT void update_parameter_float_ex_resource(tool::ex::ExResource*r, const char* paramName, float value);
    DECL_EXPORT void update_parameter_double_ex_resource(tool::ex::ExResource*r, const char* paramName, double value);
    DECL_EXPORT void update_parameter_string_ex_resource(tool::ex::ExResource*r, const char* paramName, const char* value);

    DECL_EXPORT int get_parameter_int_ex_resource(tool::ex::ExResource*r, const char* paramName);
    DECL_EXPORT float get_parameter_float_ex_resource(tool::ex::ExResource*r, const char* paramName);
    DECL_EXPORT double get_parameter_double_ex_resource(tool::ex::ExResource*r, const char* paramName);
    DECL_EXPORT const char* get_parameter_string_ex_resource(tool::ex::ExResource*r, const char* paramName);

    DECL_EXPORT int get_size_parameter_array_ex_resource(tool::ex::ExResource*r, const char* paramName);
    DECL_EXPORT void get_parameter_array_int_ex_resource(tool::ex::ExResource*r, const char* paramName, int* values);
    DECL_EXPORT void get_parameter_array_float_ex_resource(tool::ex::ExResource*r, const char* paramName, float* values);
    DECL_EXPORT void get_parameter_array_double_ex_resource(tool::ex::ExResource*r, const char* paramName, double* values);

    DECL_EXPORT void update_parameter_array_int_ex_resource(tool::ex::ExResource*r, const char* paramName, int* values, int size);
    DECL_EXPORT void update_parameter_array_float_ex_resource(tool::ex::ExResource*r, const char* paramName, float* values, int size);
    DECL_EXPORT void update_parameter_array_double_ex_resource(tool::ex::ExResource*r, const char* paramName, double* values, int size);
}

