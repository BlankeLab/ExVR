

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

PROJECT_NAME = all-exvr

TEMPLATE = subdirs

SUBDIRS = base opengl-utility 3d-engine qt-utility nodes tool-test demos \
          exvr-designer exvr-components exvr-export exvr-test\

# where to find the sub projects
## toolbox
base.subdir                     = toolbox/cpp-projects/base
opengl-utility.subdir           = toolbox/cpp-projects/opengl-utility
3d-engine.subdir                = toolbox/cpp-projects/3d-engine
qt-utility.subdir               = toolbox/cpp-projects/qt-utility
nodes.subdir                    = toolbox/cpp-projects/nodes
tool-test.subdir                = toolbox/cpp-projects/tool-test
demos.subdir                    = toolbox/cpp-projects/demos
## exvr
exvr-designer.subdir            = cpp-projects/exvr-designer
exvr-components.subdir          = cpp-projects/exvr-components
exvr-export.subdir              = cpp-projects/exvr-export
exvr-test.subdir                = cpp-projects/exvr-test

# dependencies
## toolbox
opengl-utility.depends          = base
3d-engine.depends               = opengl-utility
qt-utility.depends              = opengl-utility
nodes.depends                   = base
tool-test.depends               = opengl-utility
demos.depends                   = 3d-engine
## exvr
exvr-designer.depends           = qt-utility nodes
exvr-components                 = qt-utility
exvr-export.depends             = exvr-components
exvr-test.depends               = exvr-designer
