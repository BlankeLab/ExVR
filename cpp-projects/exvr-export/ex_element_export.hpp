
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
#include "exvr/ex_element.hpp"

// local
#include "utility/export.hpp"

extern "C"{

    DECL_EXPORT void set_exp_ex_element(tool::ex::ExElement *e, tool::ex::ExExperiment *exp);
    DECL_EXPORT int contains_ex_element(tool::ex::ExElement *e, int containerId, const char* paramName);

    DECL_EXPORT int get_parameter_int_ex_element(tool::ex::ExElement*e, int containerId, const char* paramName);
    DECL_EXPORT float get_parameter_float_ex_element(tool::ex::ExElement*e, int containerId, const char* paramName);
    DECL_EXPORT double get_parameter_double_ex_element(tool::ex::ExElement*e, int containerId, const char* paramName);
    DECL_EXPORT const char* get_parameter_string_ex_element(tool::ex::ExElement*e, int containerId, const char* paramName);

    DECL_EXPORT int get_size_parameter_array_ex_element(tool::ex::ExElement*e, int containerId, const char* paramName);
    DECL_EXPORT void get_parameter_array_int_ex_element(tool::ex::ExElement*e, int containerId, const char* paramName, int* values);
    DECL_EXPORT void get_parameter_array_float_ex_element(tool::ex::ExElement*e, int containerId, const char* paramName, float* values);
    DECL_EXPORT void get_parameter_array_double_ex_element(tool::ex::ExElement*e, int containerId, const char* paramName, double* values);

    DECL_EXPORT void set_parameter_bool_ex_element(tool::ex::ExElement*e, int containerId, const char* paramName, int value);
    DECL_EXPORT void set_parameter_int_ex_element(tool::ex::ExElement*e, int containerId, const char* paramName, int value);
    DECL_EXPORT void set_parameter_float_ex_element(tool::ex::ExElement*e, int containerId, const char* paramName, float value);
    DECL_EXPORT void set_parameter_double_ex_element(tool::ex::ExElement*e, int containerId, const char* paramName, double value);
    DECL_EXPORT void set_parameter_string_ex_element(tool::ex::ExElement*e, int containerId, const char* paramName, const char* value);

    DECL_EXPORT void set_parameter_array_int_ex_element(tool::ex::ExElement*e, int containerId, const char* paramName, int* values, int size);
    DECL_EXPORT void set_parameter_array_float_ex_element(tool::ex::ExElement*e, int containerId, const char* paramName, float* values, int size);
    DECL_EXPORT void set_parameter_array_double_ex_element(tool::ex::ExElement*e, int containerId, const char* paramName, double* values, int size);
}
