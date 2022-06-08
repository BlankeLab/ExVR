
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
}

