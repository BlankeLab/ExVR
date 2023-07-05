
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

#include "ex_element_export.hpp"

using namespace tool::ex;

void set_exp_ex_element(tool::ex::ExElement *e, tool::ex::ExExperiment *exp){
    e->set_exp(exp);
}

ParametersContainer to_pc(int containerId){
    return static_cast<ParametersContainer>(containerId);
}

void set_parameter_bool_ex_element(ExElement *e, int containerId, const char* paramName, int value){
    e->set<bool>(to_pc(containerId), paramName, value==1);
}
void set_parameter_int_ex_element(ExElement *e, int containerId, const char* paramName, int value){
    e->set<int>(to_pc(containerId),paramName, value);
}
void set_parameter_float_ex_element(ExElement *e, int containerId, const char* paramName, float value){
    e->set<float>(to_pc(containerId),paramName, value);
}
void set_parameter_double_ex_element(ExElement *e, int containerId, const char* paramName, double value){
    e->set<double>(to_pc(containerId),paramName, value);
}
void set_parameter_string_ex_element(ExElement *e, int containerId, const char* paramName, const char* value){
    e->set<std::string>(to_pc(containerId),paramName, std::string{value});
}

void set_parameter_array_int_ex_element(ExElement *e, int containerId, const char *paramName, int *values, int size){
    std::vector<int> v(static_cast<size_t>(size));
    std::copy(values, values + size, v.begin());
    e->set_array<int>(to_pc(containerId), paramName, std::move(v));
}

void set_parameter_array_float_ex_element(ExElement *e, int containerId, const char *paramName, float *values, int size){
    std::vector<float> v(static_cast<size_t>(size));
    std::copy(values, values + size, v.begin());
    e->set_array<float>(to_pc(containerId), paramName, std::move(v));
}

void set_parameter_array_double_ex_element(ExElement *e, int containerId, const char *paramName, double *values, int size){
    std::vector<double> v(static_cast<size_t>(size));
    std::copy(values, values + size, v.begin());
    e->set_array<double>(to_pc(containerId), paramName, std::move(v));
}

int get_size_parameter_array_ex_element(ExElement *e, int containerId, const char *paramName){
    return e->get_array_size(to_pc(containerId), paramName);
}

void get_parameter_array_int_ex_element(ExElement *e, int containerId, const char *paramName, int *values){
    auto vec = e->get_array<int>(to_pc(containerId), paramName);
    std::copy(std::begin(vec), std::end(vec), values);
}

void get_parameter_array_float_ex_element(ExElement *e, int containerId, const char *paramName, float *values){
    auto vec = e->get_array<float>(to_pc(containerId), paramName);
    std::copy(std::begin(vec), std::end(vec), values);
}

void get_parameter_array_double_ex_element(ExElement *e, int containerId, const char *paramName, double *values){
    auto vec = e->get_array<double>(to_pc(containerId), paramName);
    std::copy(std::begin(vec), std::end(vec), values);
}

int get_parameter_int_ex_element(ExElement *e, int containerId, const char *paramName){
    return e->get<int>(to_pc(containerId), paramName);
}

float get_parameter_float_ex_element(ExElement *e, int containerId, const char *paramName){
    return e->get<float>(to_pc(containerId), paramName);
}

double get_parameter_double_ex_element(ExElement *e, int containerId, const char *paramName){
    return e->get<double>(to_pc(containerId), paramName);
}

const char *get_parameter_string_ex_element(ExElement *e, int containerId, const char *paramName){
    auto strPtr = e->get_ptr<std::string>(to_pc(containerId), paramName);
    if(strPtr != nullptr){
        return strPtr->c_str();
    }
    return "";
}

int contains_ex_element(ExElement *e, int containerId, const char *paramName){
    return e->contains(to_pc(containerId), paramName) ? 1 : 0;
}

