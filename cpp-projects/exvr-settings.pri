
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

# debug/release
CFG = ""
CONFIG(debug, debug|release){
    CFG = "debug"
    $$TARGET = $$TARGET"-d"
}

CONFIG(release, debug|release){
    CFG = "release"
}

# CONFIG
CONFIG += warn_on

# OS
COMPILER = ""   # vs / gcc
OS = ""         # Windows / Linux
EXT_O = ""      # .obj / .o

win32:{
    OS = "Windows"
    COMPILER = "vs"
    EXT_O = ".obj"
}

unix:!macx{
    OS = "Linux"
    COMPILER = "gcc"
    EXT_O = ".o"
}

equals(QT_MAJOR_VERSION, 6){
    CONFIG+=c++20
}

# ARCH
equals(COMPILER, "gcc"){ # GCC (TODO)
    # FLAGS
    QMAKE_CXXFLAGS += -fopenmp -O3
    QMAKE_LFLAGS   += -fopenmp
}
equals(COMPILER, "vs"){ # Visual studio

    # set cl flags
    equals(QT_MAJOR_VERSION, 5){
        QMAKE_CXXFLAGS *= -std:c++20    # The latest draft C++ standard preview features.
    }

    QMAKE_CXXFLAGS *= -MP               # Build with Multiple Processes
    # warnings levels
    QMAKE_CXXFLAGS -= -W0
    QMAKE_CXXFLAGS -= -W1
    QMAKE_CXXFLAGS -= -W2
    QMAKE_CXXFLAGS *= -W3
    # optimization
    QMAKE_CXXFLAGS -= -O1               # Creates small code.
    QMAKE_CXXFLAGS *= -O2               # Creates fast code.

    #cl -c
    #-nologo:               Suppresses the display of the copyright banner when the compiler starts up and display of informational messages during compiling.
    #-Zc:wchar_t:           Parse wchar_t as a built-in type according to the C++ standard.
    #-FS:                   Forces writes to the PDB file to be serialized through MSPDBSRV.EXE.
    #-Zc:rvalueCast:        Enforce Standard C++ explicit type conversion rules (off by default).
    #-Zc:inline             Remove unreferenced functions or data if they're COMDAT or have internal linkage only (off by default).
    #-Zc:strictStrings      Disable string-literal to char* or wchar_t* conversion (off by default).
    #-Zc:throwingNew        Assume operator new throws on failure (off by default).
    #-Zc:referenceBinding   A UDT temporary won't bind to a non-const lvalue reference (off by default).
    #-Zc:__cplusplus        Enable the __cplusplus macro to report the supported standard (off by default).
    #-O1                    Creates small code.
    #-O2                    Creates fast code.
    #/std:c++latest         The latest draft C++ standard preview features.
    #/openmp                Enables #pragma omp in source code.
    #-MP                    Build with Multiple Processes
    #-MD                    Compiles to create a multithreaded DLL, by using MSVCRT.lib.
}
