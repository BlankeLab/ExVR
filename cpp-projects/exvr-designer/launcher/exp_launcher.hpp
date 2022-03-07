
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

// std
#include <memory>
#include <optional>
#include <string_view>
#include <tuple>

// Qt
#include <QTimer>

// base
#include "utility/tuple_array.hpp"

// local
#include "utility/path_utility.hpp"
#include "data/config.hpp"
#include "data/states.hpp"
#include "data/settings.hpp"
#include "exp_launcher_process.hpp"
#include "exp_launcher_communication.hpp"

namespace tool::ex {

using namespace std::literals::string_view_literals;

class ExpLauncher;
using ExpLauncherUP = std::unique_ptr<ExpLauncher>;

class ExpLauncher : public QObject{

    Q_OBJECT

public slots:

    void stop_communications();

    void init_communication();
    void start_program(Settings settings);

    void load_experiment(QString expFilePath, QString instanceFilePath);
    void start_experiment();

    void clean_experiment();
    void stop_experiment();
    void play_pause_experiment();
    void play_delay_experiment(int seconds);
    void pause_experiment();
    void next_element();
    void previous_element();
    void go_to_specific_instance_element(int elementOrderId);

    void update_component_config_argument(ComponentKey componentKey, ConfigKey configKey, Arg arg, bool initConfig);
    void trigger_component_config_action(ComponentKey componentKey, ConfigKey configKey, QStringView actionName, bool initConfig);
    void update_connector_node(ElementKey routineKey,ConditionKey conditionKey, ConnectorKey connectorKey, QString name, Arg arg);

    void close_program();


private slots :

    void message_from_exp_launcher(QString message);
    void error_message_from_exp_launcher(QStringView error);

private :

    static inline const QString Sep       = QSL("|");
    static inline const QString StartCmd  = QSL("[%]");
    static inline const QString EndCmd    = QSL("[/%]");

    static inline const QString SLog           = QSL("[$L]");
    static inline const QString ELog           = QSL("[/L$]");
    static inline const QString SInfoComponent = QSL("[$I1]");
    static inline const QString EInfoComponent = QSL("[/I1$]");
    static inline const QString SInfoConnector = QSL("[$I2]");
    static inline const QString EInfoConnector = QSL("[/I2$]");

    static inline const QString SExpState = QSL("[$EX]");
    static inline const QString EExpState = QSL("[/EX$]");

    static inline const QString SExpLauncherState = QSL("[$EL]");
    static inline const QString EExpLauncherState = QSL("[/EL$]");

    static inline const QString SError = QSL("[$E]");
    static inline const QString EError = QSL("[/E$]");

    static inline const QString SWarning = QSL("[$W]");
    static inline const QString EWarning = QSL("[/W$]");

    QString gen_command(QStringList messages);

    // process
    bool editor = false;
    std::unique_ptr<ExpLauncherProcess> m_expLauncherProcess = nullptr;
    std::unique_ptr<ExpLauncherCommunication> m_expLauncherCommunication = nullptr;

    // message from launcher
    std::vector<QString> messagesReceived;
};
}
