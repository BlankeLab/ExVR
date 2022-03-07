
# /*******************************************************************************
# ** exvr                                                                       **
# ** MIT License                                                                **
# ** Copyright (c) [2018] [Florian Lance]                                       **
# **                                                                            **
# ** Permission is hereby granted, free of charge, to any person obtaining a    **
# ** copy of this software and associated documentation files (the "Software"), **
# ** to deal in the Software without restriction, including without limitation  **
# ** the rights to use, copy, modify, merge, publish, distribute, sublicense,   **
# ** and/or sell copies of the Software, and to permit persons to whom the      **
# ** Software is furnished to do so, subject to the following conditions:       **
# **                                                                            **
# ** The above copyright notice and this permission notice shall be included in **
# ** all copies or substantial portions of the Software.                        **
# **                                                                            **
# ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR **
# ** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   **
# ** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    **
# ** THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER **
# ** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    **
# ** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        **
# ** DEALINGS IN THE SOFTWARE.                                                  **
# **                                                                            **
# ********************************************************************************/

TOOLBOX_REPOSITORY_DIR = $$EXVR_REPOSITORY_DIR"/toolbox"
include(../toolbox/cpp-projects/tb-projects.pri)

# lib
## exvr-components
#EXVR_COMPONENTS_TARGET   = "app" # debug
EXVR_COMPONENTS_TARGET   = "lib"

# dll
## exvr-export
#EXVR_EXPORT_TARGET       = "app" # debug
EXVR_EXPORT_TARGET       = "dll"

# app
## exvr-designer
EXVR_DESIGNER_TARGET    = "app"
EXVR_TEST_TARGET        = "app"

# projects list
EXVR_CPP_PROJECTS +=\
    EXVR_COMPONENTS:exvr-components:exvr-componentsd \
    EXVR_EXPORT:exvr-export:exvr-exportd \
    EXVR_DESIGNER:exvr-designer:exvr-designerd \
    EXVR_TEST:exvr-test:exvr-testd \

# sub dir
EXVR_CPP_PROJECTS_DIR       = $$EXVR_REPOSITORY_DIR"/cpp-projects"
EXVR_CPP_THIRDPARTY_DIR     = $$EXVR_CPP_PROJECTS_DIR"/_thirdparty"
EXVR_CPP_RESOURCES_DIR      = $$EXVR_CPP_PROJECTS_DIR"/_resources"

# generate projects variables
for(project_dir, EXVR_CPP_PROJECTS):{
    generate_variables($$EXVR_CPP_PROJECTS_DIR, $$section(project_dir, :, 0, 0), $$section(project_dir, :, 1, 1), $$section(project_dir, :, 2, 2))
}



