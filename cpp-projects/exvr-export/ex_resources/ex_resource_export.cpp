
/*******************************************************************************
** lnco-exvr-export                                                           **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

#include "ex_resource_export.hpp"

using namespace tool::ex;

void delete_ex_resource(tool::ex::ExResource *r){
    delete r;
}

int initialize_ex_resource(tool::ex::ExResource *r){
    return r->initialize() ? 1 : 0;
}

void init_callbacks_ex_resource(
    tool::ex::ExResource *r,
    StackTraceCB stackTraceCB,
    LogCB logCB,
    LogWarningCB logWarningCB,
    LogErrorCB logErrorCB){

    r->stackTraceCB = std::make_unique<StackTraceCB>(stackTraceCB);
    r->logCB = std::make_unique<LogCB>(logCB);
    r->logWarningCB = std::make_unique<LogWarningCB>(logWarningCB);
    r->logErrorCB = std::make_unique<LogErrorCB>(logErrorCB);
}

void update_parameter_bool_ex_resource(ExResource *r, const char* paramName, int value){
    r->update_parameter<bool>(paramName, value==1);
}
void update_parameter_int_ex_resource(ExResource *r, const char* paramName, int value){
    r->update_parameter<int>(paramName, value);
}
void update_parameter_float_ex_resource(ExResource *r, const char* paramName, float value){
    r->update_parameter<float>(paramName, value);
}
void update_parameter_double_ex_resource(ExResource *r, const char* paramName, double value){
    r->update_parameter<double>(paramName, value);
}

void update_parameter_string_ex_resource(ExResource *r, const char* paramName, const char* value){
    r->update_parameter<std::string>(paramName, std::string{value});
}

void update_parameter_array_int_ex_resource(ExResource *r, const char *paramName, int *values, int size){
    std::vector<int> v(static_cast<size_t>(size));
    std::copy(values, values + size, v.begin());
    r->update_parameter_array<int>(paramName, std::move(v));
}

void update_parameter_array_float_ex_resource(ExResource *r, const char *paramName, float *values, int size){
    std::vector<float> v(static_cast<size_t>(size));
    std::copy(values, values + size, v.begin());
    r->update_parameter_array<float>(paramName, std::move(v));
}

void update_parameter_array_double_ex_resource(ExResource *r, const char *paramName, double *values, int size){
    std::vector<double> v(static_cast<size_t>(size));
    std::copy(values, values + size, v.begin());
    r->update_parameter_array<double>(paramName, std::move(v));
}

int get_size_parameter_array_ex_resource(ExResource *r, const char *paramName){
    return r->get_array_size(paramName);
}

void get_parameter_array_int_ex_resource(ExResource *r, const char *paramName, int *values){
    auto vec = r->get_array<int>(paramName);
    std::copy(std::begin(vec), std::end(vec), values);
}

void get_parameter_array_float_ex_resource(ExResource *r, const char *paramName, float *values){
    auto vec = r->get_array<float>(paramName);
    std::copy(std::begin(vec), std::end(vec), values);
}

void get_parameter_array_double_ex_resource(ExResource *r, const char *paramName, double *values){
    auto vec = r->get_array<double>(paramName);
    std::copy(std::begin(vec), std::end(vec), values);
}

int get_parameter_int_ex_resource(ExResource *r, const char *paramName){
    return r->get<int>(paramName);
}

float get_parameter_float_ex_resource(ExResource *r, const char *paramName){
    return r->get<float>(paramName);
}

double get_parameter_double_ex_resource(ExResource *r, const char *paramName){
    return r->get<double>(paramName);
}

const char *get_parameter_string_ex_resource(ExResource *r, const char *paramName){
    std::string *p = r->get_ptr<std::string>(paramName);
    if(p != nullptr){
        return p->c_str();
    }
    return "";
}

int contains_ex_resource(ExResource *r, const char *paramName){
    return r->contains(paramName) ? 1 : 0;
}

void clean_ex_resource(tool::ex::ExResource *r){
    r->clean();
}
