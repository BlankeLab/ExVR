
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

#include "exp_launcher_process.hpp"

// Qt
#include <QWidget>

// qt-utility
#include "qt_logger.hpp"

using namespace tool::ex;

ExpLauncherProcess::ExpLauncherProcess(QObject *parent){

    Q_UNUSED(parent)

    // define environment variables
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.remove("PATH");
    setProcessEnvironment(env);

    // conections for getting exp launcher output
    connect(this, &ExpLauncherProcess::readyReadStandardOutput, this, &ExpLauncherProcess::read_standard_output);
    connect(this, &ExpLauncherProcess::readyReadStandardError, this, &ExpLauncherProcess::read_error_output);

    connect(this, &ExpLauncherProcess::started, this, [&]{
        QtLogger::message(QSL("[STATE] ExVR-exp has started."));
        isRunning = true;
    });
    connect(this, &ExpLauncherProcess::errorOccurred, this, [&](QProcess::ProcessError error){

        switch (error) {
            case QProcess::FailedToStart:
                QtLogger::error(QSL("ExVR-exp process error: failed to start."));
            break;
            case QProcess::Crashed:
//                Logger::error(QSL("ExVR-exp process error: crashed."));
            break;
            case QProcess::Timedout:
                QtLogger::error(QSL("ExVR-exp process error: start timeout"));
            break;
            case QProcess::WriteError:
                QtLogger::error(QSL("ExVR-exp process error: write error."));
            break;
            case QProcess::ReadError:
                QtLogger::error(QSL("ExVR-exp process error: read error."));
            break;
            case QProcess::UnknownError:
                QtLogger::error(QSL("ExVR-exp process error: unknow."));
            break;
        }
    });
    connect(this, &ExpLauncherProcess::stateChanged, this, [&](QProcess::ProcessState newState){
        switch (newState) {
            case QProcess::ProcessState::Running:
                QtLogger::message(QSL("[STATE] ExVR-exp is running."));
            break;
            case QProcess::ProcessState::Starting:
                QtLogger::message(QSL("[STATE] ExVR-exp is starting."));
            break;
            case QProcess::ProcessState::NotRunning:
                QtLogger::message(QSL("[STATE] ExVR-exp is not running."));
            isRunning = false;
            break;
        }
    });

    connect(this, QOverload<int, ExpLauncherProcess::ExitStatus>::of(&QProcess::finished), this, [&](int exitCode, QProcess::ExitStatus exitStatus){

        Q_UNUSED(exitStatus)
        if(exitCode == 1){
            QtLogger::message(QSL("[STATE] Normal exit"));
        }else{
            // Logger::error("Crash exit");
        }
    });
}


void ExpLauncherProcess::start_program(QString command, Settings settings, quint16 readingPort){

    QStringList args;
    QString displayMode = "";
    if(settings.displayMode == Settings::ExpLauncherDisplayMode::OpenVR){
        displayMode = "OpenVR";
    }else if (settings.displayMode == Settings::ExpLauncherDisplayMode::Flat){
        displayMode = "None";
    }else if (settings.displayMode == Settings::ExpLauncherDisplayMode::FlatStereo){
        displayMode = "None";
    }

    args << QString::number(readingPort) << (Paths::exeDir.contains("debug") ? QSL("1") : QSL("0"));
    args << "-vrmode" << displayMode;
    args << "-monitor" << QString::number(settings.monitorId);
    args << "-screen-fullscreen" << (settings.fullscreen ? QSL("1") : QSL("0"));

    QString width, height;
    switch (settings.resolutionId) {
        case 0: // custom
            width  = QString::number(settings.customWidth);
            height = QString::number(settings.customHeight);
            break;
        case 1: // 1280x1024
            width  = "1280";
            height = "1024";
            break;
        case 2: // 1920x1080
            width  = "1920";
            height = "1080";
            break;
        case 3: // 1920x1200
            width  = "1920";
            height = "1200";
            break;
        case 4: // 2560x1440
            width  = "2560";
            height = "1440";
            break;
    }
    args << "-screen-width" << width;
    args << "-screen-height" << height;
    args << "-screen-quality" << "Fantastic";

    start(command, args);
    waitForStarted();
}

void ExpLauncherProcess::read_standard_output(){
    emit standard_output_signal(QString::fromUtf8(readAllStandardOutput()));
}

void ExpLauncherProcess::read_error_output(){
    emit error_output_signal(QString::fromUtf8(readAllStandardError()));
}


#include "moc_exp_launcher_process.cpp"
