
# /*******************************************************************************
# ** exvr-export                                                                **
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
    TARGET = exvr-exportd-app
}
equals(CFG, "release"){
    TARGET = exvr-export-app
}

####################################### TEMPLATE
TEMPLATE = app
CONFIG += console


####################################### BUILD FILES
OBJECTS_DIR = $$EXVR_EXPORT_OBJ
DESTDIR     = $$EXVR_EXPORT_DEST

####################################### CONFIG
CONFIG -= qt

######################################## INCLUDES
INCLUDEPATH += \
    # base
    $$BASE_INCLUDES \
    # exvr-export
    $$EXVR_EXPORT_INCLUDES \
    # scaner-component
    $$SCANER_COMPONENT_INCLUDES \
    # local
    ex_components\
    utility\
    # opencv
    $$OPENCV_INCLUDES \
    # boost
    $$BOOST_INCLUDES \
    # kinect2
    $$KINECT2_INCLUDES \
    # turbojpg
    $$TURBOJPG_INCLUDES \
    # fastfor
    $$FASTPFOR_INCLUDES \
    # GSL
    $$GSL_INCLUDES \
    # open3D
    $$OPEN3D_INCLUDES \
    # eigen
    $$EIGEN_INCLUDES\
    # kinect4
    $$KINECT4_INCLUDES\
    # turbopfor
    $$TURBOPFOR_INCLUDES \
    # python
    $$PYTHON_INCLUDES \


######################################## LIBRAIRIES

PRE_TARGETDEPS += \
    # tool
    $$BASE_LIB_DEP \
    $$SCANER_COMPONENT_DEP \

LIBS +=  \
    # tool
    $$BASE_LIB \
    $$SCANER_COMPONENT_LIB\
    $$EXVR_EXPORT_OBJ"\py*.obj"\
    $$EXVR_EXPORT_OBJ"\vi*.obj"\
    $$EXVR_EXPORT_OBJ"\ex_*.obj"\
    $$EXVR_EXPORT_OBJ"\k2_*.obj"\
    $$EXVR_EXPORT_OBJ"\k4_*.obj"\
    # thirdparty
    $$OPENCV_LIBS \
    $$WINDOWS_LIBS \
    $$BOOST_LIBS \
    $$TURBOJPG_LIBS\
    $$FASTPFOR_LIBS\
    $$EIGEN_LIBS\
    $$OPEN3D_LIBS \
    $$KINECT4_LIBS \
    $$TURBOPFOR_LIBS \
    $$PYTHON_LIBS \

######################################## PROJECT FILES

HEADERS += \

SOURCES += \
    exvr_export_main.cpp

