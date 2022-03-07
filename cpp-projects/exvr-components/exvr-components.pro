# /*******************************************************************************
# ** exvr-components                                                            **
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
####################################### repo
EXVR_REPOSITORY_DIR      = $$PWD"/../.."

####################################### PRI
# defines compiling options
include(../exvr-settings.pri)
# defines projects paths and variables
include(../exvr-projects.pri)
# defines thirdparty includes and libs
include(../exvr-thirdparty.pri)

####################################### TARGET
equals(CFG, "debug"){
    TARGET = exvr-componentsd
}
equals(CFG, "release"){
    TARGET = exvr-components
}

####################################### TEMPLATE
equals(EXVR_COMPONENTS_TARGET, "lib"){
    TEMPLATE = lib
    CONFIG += staticlib
    CONFIG -= console
}
equals(EXVR_COMPONENTS_TARGET, "app"){
    TEMPLATE = app
    CONFIG += console
}

####################################### BUILD FILES
OBJECTS_DIR = $$EXVR_COMPONENTS_OBJ
DESTDIR     = $$EXVR_COMPONENTS_DEST

####################################### CONFIG
CONFIG -= qt

####################################### INCLUDES
INCLUDEPATH += \
    # base
    $$BASE_INCLUDES\
    # local
    ex_components \
    # third party
    $$OPENCV_INCLUDES \
    $$BOOST_INCLUDES \
    $$PYTHON_INCLUDES \
    $$BOOST_INCLUDES \
    $$SIGNALS_INCLUDES \

####################################### LIBRAIRIES

LIBS +=  \
    $$BASE_LIB \
    # third party
    $$OPENCV_LIBS \
    $$PYTHON_LIBS \
    $$BOOST_LIBS \

####################################### PROJECT FILES

HEADERS += \
    # ex_components
    ex_components/python_script_component.hpp \
    ex_components/video_saver_component.hpp \

SOURCES += \
    # ex_components
    ex_components/python_script_component.cpp \
    ex_components/video_saver_component.cpp \
    # main
    main.cpp \


