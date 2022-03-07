
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

#include "path_utility.hpp"

using namespace tool::ex;

void Paths::initialize_paths(QString exeDir){

    Paths::exeDir           = exeDir;
    exe                     = exeDir % QSL("/ExVR.exe");
    dataDir                 = exeDir % QSL("/data");
    expLauncherDir          = exeDir % QSL("/exp-launcher");
    // exp launcher
    expLauncherExe           = expLauncherDir % QSL("/ExVR-exp.exe");
    // data
    expDir                  = dataDir % QSL("/experiments");    
    assetsBundlesDir        = dataDir % QSL("/assetsBundles");
    tempDir                 = dataDir % QSL("/temp");
    documentationDir        = dataDir % QSL("/documentation");
    // log
    logsDir                 = exeDir  % QSL("/logs");
    designerLogFile         = logsDir % QSL("/designer_log.html");
    expLogFile              = logsDir % QSL("/exp_log.txt");

    // temp
    tempExp                 = tempDir % QSL("/exp.xml");
    tempInstance            = tempDir % QSL("/debug-instance.xml");
    // scripts
    // # python
    scriptsPythonDir        = dataDir % QSL("/scripts/python");
    templatePythonScript    = scriptsPythonDir % QSL("/template/default.py");
    // # csharp
    scriptsCSharpDir        = dataDir % QSL("/scripts/csharp");
    templateCSharpScript    = scriptsCSharpDir % QSL("/template/default.cs");
    // others
    documentsDir            = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).value(0);
    desktopDir              = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).value(0);
}


//QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).value(0);


//Path type	macOS	Windows
//DesktopLocation	"~/Desktop"	"C:/Users/<USER>/Desktop"
//DocumentsLocation	"~/Documents"	"C:/Users/<USER>/Documents"
//FontsLocation	"/System/Library/Fonts" (not writable)	"C:/Windows/Fonts" (not writable)
//ApplicationsLocation	"/Applications" (not writable)	"C:/Users/<USER>/AppData/Roaming/Microsoft/Windows/Start Menu/Programs"
//MusicLocation	"~/Music"	"C:/Users/<USER>/Music"
//MoviesLocation	"~/Movies"	"C:/Users/<USER>/Videos"
//PicturesLocation	"~/Pictures"	"C:/Users/<USER>/Pictures"
//TempLocation	randomly generated by the OS	"C:/Users/<USER>/AppData/Local/Temp"
//HomeLocation	"~"	"C:/Users/<USER>"
//DataLocation	"~/Library/Application Support/<APPNAME>", "/Library/Application Support/<APPNAME>". "<APPDIR>/../Resources"	"C:/Users/<USER>/AppData/Local/<APPNAME>", "C:/ProgramData/<APPNAME>", "<APPDIR>", "<APPDIR>/data", "<APPDIR>/data/<APPNAME>"
//CacheLocation	"~/Library/Caches/<APPNAME>", "/Library/Caches/<APPNAME>"	"C:/Users/<USER>/AppData/Local/<APPNAME>/cache"
//GenericDataLocation	"~/Library/Application Support", "/Library/Application Support"	"C:/Users/<USER>/AppData/Local", "C:/ProgramData", "<APPDIR>", "<APPDIR>/data"
//RuntimeLocation	"~/Library/Application Support"	"C:/Users/<USER>"
//ConfigLocation	"~/Library/Preferences"	"C:/Users/<USER>/AppData/Local/<APPNAME>", "C:/ProgramData/<APPNAME>"
//GenericConfigLocation	"~/Library/Preferences"	"C:/Users/<USER>/AppData/Local", "C:/ProgramData"
//DownloadLocation	"~/Downloads"	"C:/Users/<USER>/Documents"
//GenericCacheLocation	"~/Library/Caches", "/Library/Caches"	"C:/Users/<USER>/AppData/Local/cache"
//AppDataLocation	"~/Library/Application Support/<APPNAME>", "/Library/Application Support/<APPNAME>". "<APPDIR>/../Resources"	"C:/Users/<USER>/AppData/Roaming/<APPNAME>", "C:/ProgramData/<APPNAME>", "<APPDIR>", "<APPDIR>/data", "<APPDIR>/data/<APPNAME>"
//AppLocalDataLocation	"~/Library/Application Support/<APPNAME>", "/Library/Application Support/<APPNAME>". "<APPDIR>/../Resources"	"C:/Users/<USER>/AppData/Local/<APPNAME>", "C:/ProgramData/<APPNAME>", "<APPDIR>", "<APPDIR>/data", "<APPDIR>/data/<APPNAME>"
//AppConfigLocation	"~/Library/Preferences/<APPNAME>"	"C:/Users/<USER>/AppData/Local/<APPNAME>", "C:/ProgramData/<APPNAME>"

//Path type	Linux
//DesktopLocation	"~/Desktop"
//DocumentsLocation	"~/Documents"
//FontsLocation	"~/.fonts"
//ApplicationsLocation	"~/.local/share/applications", "/usr/local/share/applications", "/usr/share/applications"
//MusicLocation	"~/Music"
//MoviesLocation	"~/Videos"
//PicturesLocation	"~/Pictures"
//TempLocation	"/tmp"
//HomeLocation	"~"
//DataLocation	"~/.local/share/<APPNAME>", "/usr/local/share/<APPNAME>", "/usr/share/<APPNAME>"
//CacheLocation	"~/.cache/<APPNAME>"
//GenericDataLocation	"~/.local/share", "/usr/local/share", "/usr/share"
//RuntimeLocation	"/run/user/<USER>"
//ConfigLocation	"~/.config", "/etc/xdg"
//GenericConfigLocation	"~/.config", "/etc/xdg"
//DownloadLocation	"~/Downloads"
//GenericCacheLocation	"~/.cache"
//AppDataLocation	"~/.local/share/<APPNAME>", "/usr/local/share/<APPNAME>", "/usr/share/<APPNAME>"
//AppLocalDataLocation	"~/.local/share/<APPNAME>", "/usr/local/share/<APPNAME>", "/usr/share/<APPNAME>"
//AppConfigLocation	"~/.config/<APPNAME>", "/etc/xdg/<APPNAME>"
