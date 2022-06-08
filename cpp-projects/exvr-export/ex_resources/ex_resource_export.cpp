
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

void clean_ex_resource(tool::ex::ExResource *r){
    r->clean();
}
