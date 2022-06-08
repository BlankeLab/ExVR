
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


void call_slot_ex_component(ExComponent *c, int index){
    c->slot(index);
}
