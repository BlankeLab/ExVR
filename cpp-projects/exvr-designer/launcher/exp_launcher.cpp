
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

#include "exp_launcher.hpp"

// std
#include <optional>

// qt-utility
#include "qt_logger.hpp"

// local
#include "experiment/global_signals.hpp"

using namespace tool::ex;

void ExpLauncher::stop_communications(){

    if(QThread::currentThread()->objectName() != QSL("exp_launcher_thread")){
        qWarning() << "Error: call to stop communications from wrong thread.";
        return;
    }

    if(m_expLauncherCommunication){
        m_expLauncherCommunication->clean();
        m_expLauncherCommunication = nullptr;
    }
}


void ExpLauncher::init_communication(){

    QThread::currentThread()->setObjectName(QSL("exp_launcher_thread"));
    m_expLauncherCommunication = std::make_unique<ExpLauncherCommunication>();
    connect(m_expLauncherCommunication.get(), &ExpLauncherCommunication::message_received_signal, this, &ExpLauncher::message_from_exp_launcher);
    connect(m_expLauncherCommunication.get(), &ExpLauncherCommunication::error_signal, this, &ExpLauncher::error_message_from_exp_launcher);    
    connect(this, &ExpLauncher::stop_exp_launcher_signal, this, &ExpLauncher::close_exp_launcher_process);
}


std::optional<QStringView> ExpLauncher::extract_balise(QStringView &message){

    int idStart = message.indexOf(SBalise);
    int idEnd   = message.indexOf(EBalise);

    if(idStart == -1 || idEnd == -1){
        return std::nullopt;
    }

    auto leftBalise = message.mid(idStart, idEnd - idStart + EBalise.size());
    message = message.right(message.size() - idEnd - EBalise.size());
    return leftBalise;
}

std::optional<QStringView> ExpLauncher::extract_balise_message(QStringView balise, QStringView start, QStringView end){
    if(balise.startsWith(start) && balise.endsWith(end)){
        return balise.mid(start.size(), balise.size() - start.size() - end.size());
    }
    return std::nullopt;
}


std::optional<QStringView> look_for_balise(QStringView message, QStringView start, QStringView end){
    int idStart = message.indexOf(start) + start.size();
    int idEnd   = message.indexOf(end);
    if(idStart != -1 && idEnd != -1){
        return message.mid(idStart, idEnd - idStart);
    }
    return std::nullopt;
}

void ExpLauncher::error_message_from_exp_launcher(QStringView error){
    QtLogger::unity_error(error, true, false);
}

QString ExpLauncher::gen_command(QStringList messages){
    return StartCmd % messages.join(Sep) % EndCmd;
}

void ExpLauncher::message_from_exp_launcher(QString m){

    if(m.length() == 0){
        return;
    }

    if(messagesReceived.size() > 2000){
        messagesReceived.erase(messagesReceived.begin(), messagesReceived.begin() + 1000);
    }

    messagesReceived.emplace_back(std::move(m));
    QStringView message = messagesReceived[messagesReceived.size()-1];


    while(message.length() > 0){
        if(auto balise = extract_balise(message); balise.has_value()){

            // LOG
            if(auto log = extract_balise_message(balise.value(), SLog, ELog); log.has_value()){
                QtLogger::unity_message(log.value(), true, false);
                continue;
            }

            // INFO COMPONENT
            if(auto infoComponent = extract_balise_message(balise.value(), SInfoComponent, EInfoComponent); infoComponent.has_value()){

                // 0 id component
                // 1 id config
                // 2 info key
                // 3 info value
                auto info = infoComponent.value();
                int count = info.count('|');
                if(count <= 2){
                    QtLogger::error(QSL("Invalid component info from exp-launcher: ") % info, true, true);
                    return;
                }

                int idSep = info.indexOf('|');
                int componentKey = info.left(idSep).toInt();
                info = info.mid(idSep+1);

                idSep = info.indexOf('|');
                int configKey = info.left(idSep).toInt();
                info = info.mid(idSep+1);

                idSep = info.indexOf('|');
                QStringView infoKey   = info.left(idSep);
                QStringView infoValue = info.mid(idSep+1);

                emit GSignals::get()->component_info_update_signal(ComponentKey{componentKey}, ConfigKey{configKey}, infoKey, infoValue);
                continue;
            }

            // INFO CONNECTOR
            if(auto infoConnector = extract_balise_message(balise.value(), SInfoConnector, EInfoConnector); infoConnector.has_value()){

                // 0 id routine
                // 1 id condition
                // 2 id connector
                // 3 info key
                // 4 info value
                auto info = infoConnector.value();
                int count = info.count('|');
                if(count <= 3){
                    QtLogger::error(QSL("Invalid connector info from exp-launcher: ") % info, true, true);
                    return;
                }

                int idSep = info.indexOf('|');
                int elementKey = info.left(idSep).toInt();
                info = info.mid(idSep+1);

                idSep = info.indexOf('|');
                int conditionKey = info.left(idSep).toInt();
                info = info.mid(idSep+1);

                idSep = info.indexOf('|');
                int connectorKey = info.left(idSep).toInt();
                info = info.mid(idSep+1);

                idSep = info.indexOf('|');
                QStringView infoKey   = info.left(idSep);
                QStringView infoValue = info.mid(idSep+1);

                emit GSignals::get()->connector_info_update_signal(ElementKey{elementKey}, ConditionKey{conditionKey}, ConnectorKey{connectorKey}, infoKey, infoValue);
                continue;
            }

            // EXPERIMENT STATE
            if(auto exp = extract_balise_message(balise.value(), SExpState, EExpState); exp.has_value()){

                auto expState = exp.value();
                int count = expState.count('|');
                if(count == 0){
                    emit GSignals::get()->exp_state_updated_signal(static_cast<ExpState>(expState.toInt()), QSL(""));
                }else if(count == 8){
                    int idSep = expState.indexOf('|');
                    auto state = static_cast<ExpState>(expState.left(idSep).toInt());
                    QStringView infos = expState.mid(idSep+1);
                    emit GSignals::get()->exp_state_updated_signal(state, infos);
                }else{
                    QtLogger::error(QSL("Invalid experiment state info from exp-launcher: ") % expState, true, true);
                }
                continue;
            }


            // EXPERIMENT LAUNCHER STATE
            if(auto expL = extract_balise_message(balise.value(), SExpLauncherState, EExpLauncherState);  expL.has_value()){

                auto expLState = expL.value();
                int count = expLState.count('|');
                if(count == 0){

                    auto state = static_cast<ExpLauncherState>(expLState.toInt());
                    if(state == ExpLauncherState::Closing){
                        close_program();
                    }
                    emit GSignals::get()->exp_launcher_state_updated_signal(state, QSL(""));

                }else if(count == 2){

                    int idSep = expLState.indexOf('|');
                    auto state = static_cast<ExpLauncherState>(expLState.left(idSep).toInt());
                    expLState = expLState.mid(idSep+1);

                    auto infos = expLState;

                    if(state == ExpLauncherState::Starting){

                        idSep = expLState.indexOf('|');
                        m_expLauncherCommunication->writingPort = static_cast<quint16>(expLState.left(idSep).toInt());
                        expLState = expLState.mid(idSep+1);

                        editor = static_cast<quint16>(expLState.toInt()) == 1;

                        if(editor){
                            // send designer path to exp launcher (useful is using designer from an other path with exp-launcher editor)
                            QtLogger::message("ExVR-exp started (UnityEditor)");
                            QtLogger::message(QSL("Send designer path to editor: [") % Paths::exeDir % QSL("]"));
                            update_designer_dir(Paths::exeDir);
                        }else{
                            QtLogger::message("ExVR-exp started");
                        }
                    }
                    emit GSignals::get()->exp_launcher_state_updated_signal(state, infos);

                }else{
                    QtLogger::error(QSL("Invalid experiment launcher state info from exp-launcher: ") % expLState, true, true);
                }
                continue;
            }

            // ERROR
            if(auto error = extract_balise_message(balise.value(), SError, EError); error.has_value()){
                QtLogger::unity_error(error.value(), true, false);
                continue;
            }

            // WARNING
            if(auto warning = extract_balise_message(balise.value(), SWarning, EWarning); warning.has_value()){
                QtLogger::unity_warning(warning.value(), true, false);
                continue;
            }

        }else{
            break;
        }
    }
}

void ExpLauncher::start_program(Settings settings){

    // if already launched close it
    close_program();

    m_expLauncherProcess = std::make_unique<ExpLauncherProcess>();
    m_expLauncherProcess->start_program( Paths::expLauncherExe, settings, m_expLauncherCommunication->readingPort);
    connect(m_expLauncherProcess.get(), &ExpLauncherProcess::standard_output_signal, this, &ExpLauncher::message_from_exp_launcher);
    connect(m_expLauncherProcess.get(), &ExpLauncherProcess::error_output_signal, this, &ExpLauncher::error_message_from_exp_launcher);
    connect(m_expLauncherProcess.get(), QOverload<int, ExpLauncherProcess::ExitStatus>::of(&ExpLauncherProcess::finished),
          [=](int exitCode, ExpLauncherProcess::ExitStatus exitStatus){ /* ... */
        Q_UNUSED(exitStatus)
        Q_UNUSED(exitCode)
        // emit exp_launcher_state_updated_signal(ExpLauncherState::Closing, {});
    });
}

void ExpLauncher::load_experiment(QString expFilePath, QString instanceFilePath){
    if(m_expLauncherProcess || editor){
        m_expLauncherCommunication->send_udp_command(gen_command({to_command_id(ExpLauncherCommand::Load), expFilePath, instanceFilePath}));
    }else{
        QtLogger::message(QSL("You need to start the Experiment launcher before loading an experiment."));
    }
}

void ExpLauncher::update_designer_dir(QString designerDirPath){
    if(m_expLauncherProcess || editor){
        m_expLauncherCommunication->send_udp_command(gen_command({to_command_id(ExpLauncherCommand::UpdateDesignerDir),designerDirPath}));
    }
}

void ExpLauncher::start_experiment(){
    if(m_expLauncherProcess || editor){
        m_expLauncherCommunication->send_udp_command(gen_command({to_command_id(ExpLauncherCommand::Play)}));
    }
}

void ExpLauncher::clean_experiment(){
    if(m_expLauncherProcess || editor){
        m_expLauncherCommunication->send_udp_command(gen_command({to_command_id(ExpLauncherCommand::Clean)}));
    }
}

void ExpLauncher::pause_experiment(){
    if(m_expLauncherProcess || editor){
        m_expLauncherCommunication->send_udp_command(gen_command({to_command_id(ExpLauncherCommand::Pause)}));
    }
}

void ExpLauncher::stop_experiment(){
    if(m_expLauncherProcess || editor){
        m_expLauncherCommunication->send_udp_command(gen_command({to_command_id(ExpLauncherCommand::Stop)}));
    }
}

void ExpLauncher::play_pause_experiment(){
    if(m_expLauncherProcess || editor){
        m_expLauncherCommunication->send_udp_command(gen_command({to_command_id(ExpLauncherCommand::PlayPause)}));
    }
}

void ExpLauncher::play_delay_experiment(int seconds){
    if(m_expLauncherProcess || editor){
        m_expLauncherCommunication->send_udp_command(gen_command({to_command_id(ExpLauncherCommand::PlayDelay),QString::number(seconds)}));
    }
}

void ExpLauncher::next_element(){
    if(m_expLauncherProcess || editor){
        m_expLauncherCommunication->send_udp_command(gen_command({to_command_id(ExpLauncherCommand::Next)}));
    }
}

void ExpLauncher::previous_element(){
    if(m_expLauncherProcess || editor){
        m_expLauncherCommunication->send_udp_command(gen_command({to_command_id(ExpLauncherCommand::Previous)}));
    }
}

void ExpLauncher::go_to_specific_instance_element(int elementOrderId){
    if(m_expLauncherProcess || editor){
        m_expLauncherCommunication->send_udp_command(gen_command({to_command_id(ExpLauncherCommand::GoToSpecificInstanceElement), QString::number(elementOrderId)}));
    }
}

void ExpLauncher::update_component_config_argument(ComponentKey componentKey, ConfigKey configKey, Arg arg, bool initConfig){

    if((!m_expLauncherProcess && !editor) || initConfig ){
        return;
    }

    const QString componentKeyStr    = QString::number(componentKey.v);
    const QString configKeyStr       = QString::number(configKey.v);
    const QString unityTypeStr       = from_view(get_unity_type_string(arg.unity_type()));
    const QString dimStr             = QString::number(arg.dimensions_nb());

    QString sizesStr;
    for(int ii = 0; ii < arg.dimensions_nb(); ++ii){
        if(ii != 0){
            sizesStr += QSL(" ");
        }
        sizesStr += QString::number(arg.size_dimension(ii));
    }    

    QString command =
        StartCmd %
        to_command_id(ExpLauncherCommand::UpdateComponent) % Sep %
        componentKeyStr % Sep % configKeyStr % Sep %
        arg.name % Sep % arg.value() % Sep % unityTypeStr % Sep % dimStr % Sep % sizesStr % Sep % arg.separator() %
        EndCmd;

    m_expLauncherCommunication->send_udp_command(std::move(command));
}

void ExpLauncher::trigger_component_config_action(ComponentKey componentKey, ConfigKey configKey, QStringView actionName, bool initConfig){

    Q_UNUSED(initConfig)

    if(!m_expLauncherProcess && !editor){
        return;
    }

    const QString componentKeyStr    = QString::number(componentKey.v);
    const QString configKeyStr       = QString::number(configKey.v);

    m_expLauncherCommunication->send_udp_command(
            gen_command({to_command_id(ExpLauncherCommand::Action), componentKeyStr, configKeyStr, actionName.toString()}));
}

void ExpLauncher::update_connector_node(ElementKey routineKey, ConditionKey conditionKey, ConnectorKey connectorKey, QString name, Arg arg){

    Q_UNUSED(name)

    if(!m_expLauncherProcess && !editor){
        return;
    }

    const QString routineKeyStr   = QString::number(routineKey.v);
    const QString conditionKeyStr = QString::number(conditionKey.v);
    const QString connectorKeyStr = QString::number(connectorKey.v);

    const QString unityTypeStr    = from_view(get_unity_type_string(arg.unity_type()));
    const QString dimStr          = QString::number(arg.dimensions_nb());

    QString sizesStr;
    for(int ii = 0; ii < arg.dimensions_nb(); ++ii){
        if(ii != 0){
            sizesStr += " ";
        }
        sizesStr += QString::number(arg.size_dimension(ii));
    }

    m_expLauncherCommunication->send_udp_command(gen_command({to_command_id(ExpLauncherCommand::UpdateConnector), routineKeyStr, conditionKeyStr, connectorKeyStr, "value", arg.value(), unityTypeStr, dimStr, sizesStr, arg.separator()}));
}

void ExpLauncher::close_program(){

    if(m_expLauncherProcess || editor){
        m_expLauncherCommunication->send_udp_command(gen_command({to_command_id(ExpLauncherCommand::Quit)}));
        if(!editor){
            QTimer::singleShot(500, this, &ExpLauncher::stop_exp_launcher_signal);
        }else{
            QtLogger::message("Close ExVR-exp (UnityEditor)");
            editor = false;
        }
    }
}

auto ExpLauncher::close_exp_launcher_process() -> void{

    if(QThread::currentThread()->objectName() != QSL("exp_launcher_thread")){
        qWarning() << "Error: call to close program from wrong thread.";
        return;
    }
    if(m_expLauncherProcess != nullptr){
        QtLogger::message("Close ExVR-exp");
        m_expLauncherProcess->close();
        if(!m_expLauncherProcess->waitForFinished(500)){
            m_expLauncherProcess->kill();
        }
        m_expLauncherProcess = nullptr;
        emit GSignals::get()->exp_state_updated_signal(ExpState::NotLoaded, QSL(""));
        emit GSignals::get()->exp_launcher_state_updated_signal(ExpLauncherState::NotStarted, {});
    }
}

#include "moc_exp_launcher.cpp"
