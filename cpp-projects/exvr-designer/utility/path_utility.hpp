
/***********************************************************************************
** exvr-designer                                                                  **
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

#pragma once

// Qt
#include <QApplication>
#include <QStandardPaths>

// qt-utility
#include "qt_str.hpp"

namespace tool::ex{

struct Paths{

    static void initialize_paths(QString exeDir);

    static inline QString exeDir = "";
    static inline QString exe = "";
    static inline QString dataDir = "";
    static inline QString expLauncherDir = "";
    // exp launcher
    static inline QString expLauncherExe = "";
    // data
    static inline QString expDir = "";
    static inline QString assetsBundlesDir = "";
    static inline QString tempDir = "";
    static inline QString documentationDir = "";
    // log
    static inline QString logsDir = "";
    static inline QString designerLogFile = "";
    static inline QString expLogFile = "";
    // temp
    static inline QString tempExp = "";
    static inline QString tempInstance = "";
    // scripts
    // # python
    static inline QString scriptsPythonDir = "";
    static inline QString templatePythonScript = "";
    // # csharp
    static inline QString scriptsCSharpDir = "";
    static inline QString templateCSharpScript = "";
    // others
    static inline QString documentsDir = "";
    static inline QString desktopDir= "";
};
}
