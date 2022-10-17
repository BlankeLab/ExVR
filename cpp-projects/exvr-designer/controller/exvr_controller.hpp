
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
#include <QObject>
#include <QDesktopWidget>
#include <QMetaEnum>

// qt-utility
#include "qt_logger.hpp"

// local
// # exp
#include "experiment/global_signals.hpp"
// # IO
#include "IO/xml_io_manager.hpp"
// # resources
#include "resources/resources_manager.hpp"
// # gui
// ## widgets
#include "gui/widgets/designer_window.hpp"
// ## dialogs
#include "gui/dialogs/copy_to_conditions_dialog.hpp"
#include "gui/dialogs/add_component_to_conditions_dialog.hpp"
#include "gui/dialogs/benchmark_dialog.hpp"
#include "gui/dialogs/import_sub_exp_dialog.hpp"
// ## ui
#include "ui_copy_to_conditions.h"



namespace tool::ex {

class ExVrController : public QObject{

    Q_OBJECT

public :

    ExVrController(const QString &nVersion, bool lncoComponents);

    inline Experiment *exp(){return ExperimentManager::get()->current();}
    inline DesignerWindow *ui(){return m_designerWindow.get();}
    inline ExpLauncher *exp_launcher(){return m_expLauncher.get();}
    inline XmlIoManager *xml(){return m_xmlManager.get();}
    inline QtLogger *log(){return QtLogger::get();}
    // dialogs
    inline SettingsDialog *set(){return &m_settingsD;}
    inline BenchmarkDialog *benchmark(){return m_benchmarkD.get();}
    inline DocumentationDialog *doc(){return &m_documentationD;}
    inline ResourcesManagerDialog *res(){return &m_resourcesD;}

    bool eventFilter(QObject *watched, QEvent *event);

private slots:

    // program
    void close_exvr();

    // exp
    // # load
    void load_full_exp_with_default_instance_to_unity();
    void load_selected_routine_with_default_instance_to_unity();
    void load_from_selected_routine_with_default_instance_to_unity();
    void load_until_selected_routine_with_default_instance_to_unity();
    // ## debug
    void load_full_exp_with_02duration();
    // # save
    void save_full_exp_with_default_instance();
    // # start
    void start_specific_instance();
    // # go to
    void go_to_current_specific_instance_element();

    // instances
    void generate_instances(QString directoryPath, unsigned int seed, bool manual, int nbInstances, int startId, QString baseName, QStringList manualNames);

    // dialogs
    // # modal    
    void show_add_action_detailed_dialog(ComponentKey componentKey);
    void show_modify_action_detailed_dialog(ComponentKey componentKey);
    void show_copy_to_conditions_dialog(ElementKey routineKey, ConditionKey conditionKey);
    void show_play_with_delay_dialog();
    void show_about_dialog();
    // # non modal
    void show_got_to_specific_instance_element_dialog();
    void show_component_informations_dialog(ComponentKey componentKey);
    void show_import_dialog();

    // data
    void update_gui_from_experiment();

signals:

    // exp
    // # load
    void load_experiment_unity_signal(QString expFilePath, QString instanceFilePath);
    // # play
    void play_delay_experiment_signal(int delaySeconds);
    // # stop
    void close_exp_launcher_signal();
    // # goto
    void go_to_specific_instance_element_signal(int elementOrderId);

    // exp launcher
    // # start
    void start_experiment_launcher_signal(tool::ex::Settings settings);
    // # IPC
    void start_communication_signal();
    void stop_communication_signal();

private:

    // generate connections
    void generate_global_signals_connections();
    void generate_main_window_connections();
    void generate_controller_connections();
    void generate_resources_manager_connections();
    void generate_logger_connections();
    void generate_dialogs_connections();

private :

    // data    
    std::unique_ptr<Instance> m_currentInstance = nullptr;

    // I/O
    std::unique_ptr<XmlIoManager> m_xmlManager = nullptr;

    // ui    
    // # widgets
    std::unique_ptr<DesignerWindow> m_designerWindow = nullptr;
    // # dialogs
    // ## non modal
    std::unique_ptr<QDialog> m_componentsInfoD              = nullptr;
    std::unique_ptr<ImportSubExpDialog> m_importD           = nullptr;
    std::unique_ptr<QDialog> m_goToD                        = nullptr;
    std::unique_ptr<BenchmarkDialog> m_benchmarkD           = nullptr;
    GenerateInstancesDialog m_instancesD;
    ResourcesManagerDialog m_resourcesD;
    DocumentationDialog m_documentationD;

    // ## modal
    std::unique_ptr<QDialog> modalDialog = nullptr;
    std::unique_ptr<AddComponentToConditionsDialog> addComponentToCondsD = nullptr;
    CopyToConditionDialog m_copyToCondD;
    SettingsDialog m_settingsD;

    std::unordered_map<QEvent::Type, std::unordered_map<QString, int>> countEvents;


    // experiment launcher
    std::unique_ptr<ExpLauncher> m_expLauncher = nullptr;
    QThread m_expLauncherT;

    // timers
    QTimer experimentUpdateTimer;

    // gui data
    int lastDelayS = 10;
};

}
