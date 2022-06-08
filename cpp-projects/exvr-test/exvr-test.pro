

# /*******************************************************************************
# ** exvr-test                                                                  **
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
    TARGET = exvr-testd
}
equals(CFG, "release"){
    TARGET = exvr-test
}

####################################### TEMPLATE
equals(EXVR_TEST_TARGET, "app"){
    TEMPLATE = app
    CONFIG += console
}

####################################### BUILD FILES
OBJECTS_DIR = $$EXVR_TEST_OBJ
MOC_DIR     = $$EXVR_TEST_MOC
RCC_DIR     = $$EXVR_TEST_RCC
UI_DIR      = $$EXVR_TEST_UI
DESTDIR     = $$EXVR_TEST_DEST

####################################### CONFIG
QT  += core gui network concurrent widgets opengl

######################################## INCLUDES
INCLUDEPATH += \
    # base
    $$BASE_INCLUDES \
    # qt-utility
    $$QT_UTILITY_INCLUDES \
    # exvr-designer
    $$EXVR_DESIGNER_INCLUDES \
    # catch
    $$CATCH_INCLUDES \
    $$QWT_INCLUDES \
    $$NODES_INCLUDES \
    $$MAGIC_ENUM_INCLUDES\

######################################## LIBRAIRIES

LIBS +=  \
    # base
    $$BASE_LIB \
    # qt-utility
    $$QT_UTILITY_LIB \
    # exvr-designer
    $$EXVR_DESIGNER_OBJ"/ExVR-designer_pch.obj" \
    $$EXVR_DESIGNER_OBJ"/xml_io_manager.obj" \
#    $$EXVR_DESIGNER_OBJ"/moc_xml_io_manager.obj" \
    $$EXVR_DESIGNER_OBJ"/experiment.obj" \
    $$EXVR_DESIGNER_OBJ"/randomizer.obj" \
    $$EXVR_DESIGNER_OBJ"/component.obj" \
    $$EXVR_DESIGNER_OBJ"/timeline.obj" \
    $$EXVR_DESIGNER_OBJ"/config.obj" \
    $$EXVR_DESIGNER_OBJ"/action.obj" \
    $$EXVR_DESIGNER_OBJ"/condition.obj" \
    $$EXVR_DESIGNER_OBJ"/routine.obj" \
    $$EXVR_DESIGNER_OBJ"/isi.obj" \
    $$EXVR_DESIGNER_OBJ"/loop.obj" \
    $$EXVR_DESIGNER_OBJ"/interval.obj" \
    $$EXVR_DESIGNER_OBJ"/components_manager.obj" \
    $$EXVR_DESIGNER_OBJ"/resources_manager.obj" \
    $$EXVR_DESIGNER_OBJ"/connector.obj" \
    $$EXVR_DESIGNER_OBJ"/path_utility.obj" \
    # third-party
    $$QWT_LIBS \
    $$NODES_LIB \


######################################## PROJECT FILES

#HEADERS += \

SOURCES += \
    exvr-test-main.cpp \
    exvr-designer_tests.cpp \


