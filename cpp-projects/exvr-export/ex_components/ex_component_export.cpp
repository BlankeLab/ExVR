
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

#include "ex_component_export.hpp"

using namespace tool;
using namespace tool::ex;

void delete_ex_component(ExComponent *c){delete c;}

int initialize_ex_component(ExComponent*c){return c->initialize()?1:0;}
void clean_ex_component(ExComponent*c) {c->clean();}

void start_experiment_ex_component(ExComponent*c){c->start_experiment();}
void stop_experiment_ex_component(ExComponent*c){c->stop_experiment();}

void set_current_config_ex_component(ExComponent *c, const char *configName){c->set_current_config(configName);}
void update_from_current_config_ex_component(ExComponent *c){c->update_from_current_config();}
void pre_start_routine_ex_component(ExComponent *c){c->pre_start_routine();}
void start_routine_ex_component(ExComponent*c){c->start_routine();}
void post_start_routine_ex_component(ExComponent *c){c->post_start_routine();}
void stop_routine_ex_component(ExComponent*c){c->stop_routine();}

void on_gui_ex_component(ExComponent *c){c->on_gui();}
void pre_update_ex_component(ExComponent *c){c->pre_update();}
void update_ex_component(ExComponent*c){c->update();}
void post_update_ex_component(ExComponent *c){c->post_update();}

void set_visibility_ex_component(ExComponent*c,int visible){c->set_visibility(visible==1);}
void set_update_state_ex_component(ExComponent *c, int doUpdate){c->set_update_state(doUpdate==1);}
void play_ex_component(ExComponent*c) {c->play();}
void pause_ex_component(ExComponent*c) {c->pause();}

void update_parameter_from_gui_ex_component(ExComponent *c, const char *argToUpdate){c->update_parameter_from_gui(argToUpdate);}
void action_from_gui_ex_component(ExComponent *c, int initConfig, const char *action){c->action_from_gui(initConfig == 1, action);}


void call_slot_ex_component(ExComponent *c, int index){c->slot(index);}

ParametersContainer to_container(int containerId){
    return static_cast<ParametersContainer>(containerId);
}

void update_parameter_bool_ex_component(ExComponent *c, int containerId, const char* paramName, int value){
    c->update_parameter<bool>(to_container(containerId), paramName, value==1);
}
void update_parameter_int_ex_component(ExComponent *c, int containerId, const char* paramName, int value){
    c->update_parameter<int>(to_container(containerId),paramName, value);
}
void update_parameter_float_ex_component(ExComponent *c, int containerId, const char* paramName, float value){
    c->update_parameter<float>(to_container(containerId),paramName, value);
}
void update_parameter_double_ex_component(ExComponent *c, int containerId, const char* paramName, double value){
    c->update_parameter<double>(to_container(containerId),paramName, value);
}
void update_parameter_string_ex_component(ExComponent *c, int containerId, const char* paramName, const char* value){
    c->update_parameter<std::string>(to_container(containerId),paramName, std::string{value});
}

void update_parameter_array_int_ex_component(ExComponent *c, int containerId, const char *paramName, int *values, int size){
    std_v1<int> v(static_cast<size_t>(size));
    std::copy(values, values + size, v.begin());
    c->update_parameter_array<int>(to_container(containerId), paramName, std::move(v));
}

void update_parameter_array_float_ex_component(ExComponent *c, int containerId, const char *paramName, float *values, int size){
    std_v1<float> v(static_cast<size_t>(size));
    std::copy(values, values + size, v.begin());
    c->update_parameter_array<float>(to_container(containerId), paramName, std::move(v));
}

void update_parameter_array_double_ex_component(ExComponent *c, int containerId, const char *paramName, double *values, int size){
    std_v1<double> v(static_cast<size_t>(size));
    std::copy(values, values + size, v.begin());
    c->update_parameter_array<double>(to_container(containerId), paramName, std::move(v));
}

int get_size_parameter_array_ex_component(ExComponent *c, int containerId, const char *paramName){
    return c->get_array_size(to_container(containerId), paramName);
}

void get_parameter_array_int_ex_component(ExComponent *c, int containerId, const char *paramName, int *values){
    auto vec = c->get_array<int>(to_container(containerId), paramName);
    std::copy(std::begin(vec), std::end(vec), values);
}

void get_parameter_array_float_ex_component(ExComponent *c, int containerId, const char *paramName, float *values){
    auto vec = c->get_array<float>(to_container(containerId), paramName);
    std::copy(std::begin(vec), std::end(vec), values);
}

void get_parameter_array_double_ex_component(ExComponent *c, int containerId, const char *paramName, double *values){
    auto vec = c->get_array<double>(to_container(containerId), paramName);
    std::copy(std::begin(vec), std::end(vec), values);
}

int get_parameter_int_ex_component(ExComponent *c, int containerId, const char *paramName){
    return c->get<int>(to_container(containerId), paramName);
}

float get_parameter_float_ex_component(ExComponent *c, int containerId, const char *paramName){
    return c->get<float>(to_container(containerId), paramName);
}

double get_parameter_double_ex_component(ExComponent *c, int containerId, const char *paramName){
    return c->get<double>(to_container(containerId), paramName);
}

const char *get_parameter_string_ex_component(ExComponent *c, int containerId, const char *paramName){
    return c->const_get<std::string>(to_container(containerId), paramName).c_str();
}

int contains_ex_component(ExComponent *c, int containerId, const char *paramName){
    return c->contains(to_container(containerId), paramName) ? 1 : 0;
}

void init_callbacks_ex_component(
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
    SignalStringCB signalStringCB){

    c->stackTraceCB             = std::make_unique<StackTraceCB>(stackTraceCB);
    c->logCB                    = std::make_unique<LogCB>(logCB);
    c->logWarningCB             = std::make_unique<LogWarningCB>(logWarningCB);
    c->logErrorCB               = std::make_unique<LogErrorCB>(logErrorCB);
    c->ellapsedTimeExpMsCB      = std::make_unique<EllapsedTimeExpMsCB>(ellapsedTimeExpMsCB);
    c->ellapsedTimeRoutineMsCB  = std::make_unique<EllapsedTimeRoutineMsCB>(ellapsedTimeRoutineMsCB);
    c->getCB                    = std::make_unique<GetCB>(getCB);
    c->isInitializedCB          = std::make_unique<IsInitializedCB>(isInitializedCB);
    c->isVisibleCB              = std::make_unique<IsVisibleCB>(isVisibleCB);
    c->isUpdatingCB             = std::make_unique<IsUpdatingCB>(isUpdatingCB);
    c->isClosedCB               = std::make_unique<IsClosedCB>(isClosedCB);
    c->nextCB                   = std::make_unique<NextCB>(nextCB);
    c->previousCB               = std::make_unique<PreviousCB>(previousCB);
    c->closeCB                  = std::make_unique<CloseCB>(closeCB);
    c->signalBoolCB             = std::make_unique<SignalBoolCB>(signalBoolCB);
    c->signalIntCB              = std::make_unique<SignalIntCB>(signalIntCB);
    c->signalFloatCB            = std::make_unique<SignalFloatCB>(signalFloatCB);
    c->signalDoubleCB           = std::make_unique<SignalDoubleCB>(signalDoubleCB);
    c->signalStringCB           = std::make_unique<SignalStringCB>(signalStringCB);
}




