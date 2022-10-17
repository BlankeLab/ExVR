
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
#include <QMainWindow>
#include <QDesktopWidget>
#include <QDockWidget>

// local
// # experiment
#include "experiment/experiment.hpp"
// # launcher
#include "launcher/exp_launcher.hpp"
// # widgets
#include "elements/flow_diagram_widget.hpp"
#include "elements/element_viewer_widget.hpp"
#include "components/components_manager_widget.hpp"
#include "routines/routines_manager_tab_widget.hpp"
#include "gui/dialogs/resources_manager_dialog.hpp"
#include "gui/dialogs/documentation_dialog.hpp"
#include "gui/dialogs/settings_dialog.hpp"
#include "gui/dialogs/generate_instances_dialog.hpp"
// # ui
#include "ui_designer_main_window.h"

// ui
namespace Ui {
    class ExVrMainUI;
    class SettingsUI;
}


namespace tool::ex {

class DesignerWindow;
using DesignerWindowUP = std::unique_ptr<DesignerWindow>;

class DesignerWindow : public QMainWindow{

    Q_OBJECT

public:

    explicit DesignerWindow(bool lncoComponents, QWidget *parent = nullptr);
    void set_window_settings();
    void close_program();

    RoutinesManagerTW* routines_manager(){return m_routinesW.get();}
    ComponentsManagerW* components_manager(){return m_componentsW.get();}
    FlowDiagramW* flow_diagram(){return m_flowDiagramW.get();}
    ElementViewerW *element_viewer(){return  m_elementViewerW.get();}

public slots:

    // update
    void update_from_experiment(Experiment *experiment, int update);

    // logs    
    // # ui
    void display_status(QString status, int ms);
    void insert_log_from_ui(QStringView log);

    void dragEnterEvent(QDragEnterEvent * evt) override{
        if(evt->mimeData()->hasUrls()){
            evt->accept();
        }
    }

    void dropEvent(QDropEvent * evt) override{
        auto urls = evt->mimeData()->urls();
        for(const auto &url : urls) {
            if(url.path().endsWith(".xml")){
                qDebug() << "load: " << url.path().remove(0,1);
                emit load_dropped_experiment_signal(url.path().remove(0,1));
            }
        }
    }
private:

    void update_main_ui(Experiment *experiment);

signals :

    // experiment
    // # I/O
    void new_experiment_signal();
    void save_experiment_signal();
    void save_experiment_as_signal();
    void load_experiment_signal();
    void save_experiment_to_temp_signal();
    void load_dropped_experiment_signal(QString path);
    void open_temp_experiment_file_signal();
    void open_temp_instance_file_signal();
    void open_experiment_directory_signal();    
    void export_experiment_as_signal();
    void import_experiment_subparts_signal();
    void open_current_exp_launcher_log_file_signal();
    void open_current_designer_log_file_signal();
    void open_log_directory_signal();
    // # load exp to unity
    void load_full_exp_wtih_default_instance_signal();
    void load_selected_routine_with_default_instance_signal();
    void load_from_selected_routine_with_default_instance_signal();
    void load_until_selected_routine_with_default_instance_signal();
    // ## debug
    void load_full_exp_with_02duration_signal();
    // # flow
    void play_experiment_signal();
    void play_pause_experiment_signal();
    void play_delay_experiment_signal();
    void pause_experiment_signal();
    void stop_experiment_signal();
    void next_element_signal();    
    void previous_element_signal();
    void go_to_current_element_signal();
    void go_to_specific_element_signal();

    // gui
    void toggle_actions_connections_separation_signal();

    // instances
    void generate_instances_signals();
    void load_full_exp_with_specific_instance_signal();    

    // unity
    void start_experiment_launcher_signal();
    void exit_experiment_launcher_signal();

    // local
    void close_exvr_signal();

protected:

    void closeEvent(QCloseEvent *event) override;

private:

    void create_actions();
    void create_menu();
    void create_toolbar();
    void create_logger();

    void create_flow_diagram();
    void create_components_manager(bool lncoComponents);
    void create_routines_manager();
    void create_element_viewer();

private:

    // loaded ui
    Ui::ExVrMainUI m_ui;

    // generated ui
    // # docks
    QDockWidget *m_dwComponents         = nullptr;
    QDockWidget *m_dwLogs               = nullptr;
    QDockWidget *m_dwElementViewer      = nullptr;
    // # experiment widgets
    FlowDiagramWUP m_flowDiagramW       = nullptr;
    ElementDisplayWUP m_elementViewerW  = nullptr;
    ComponentsManagerWUP m_componentsW  = nullptr;
    RoutinesManagerTWUP m_routinesW     = nullptr;
    // # tool buttons associated to actions
    QToolButton m_newFileButton;
    QToolButton m_saveAsButton;
    QToolButton m_openButton;
    QToolButton m_loadFullExpWithDefaultInstanceButton;
    QToolButton m_loadFromSelectedRoutineWithDefaultInstanceButton;
    QToolButton m_loadUntilSelectedRoutineWithDefaultInstanceButton;
    QToolButton m_loadFullExpWithSpecificInstanceButton;
    QToolButton m_loadSelectedRoutineWithDefaultInstanceButton;
    QToolButton m_generateInstancesButton;
    QToolButton m_expLauncherButton;
    QToolButton m_exitExpLauncherButton;
    QToolButton m_playButton;
    QToolButton m_playPauseButton;
    QToolButton m_playDelayButton;
    QToolButton m_pauseButton;
    QToolButton m_stopButton;
    QToolButton m_nextButton;
    QToolButton m_gotoCurrentButton;
    QToolButton m_gotoButton;
    QToolButton m_previousButton;
    QToolButton m_clearLogsButton;
    QToolButton m_settingsButton;
    QToolButton m_showResourcesManagerButton;
    QToolButton m_focusFlowButton;
    QToolButton m_focusRoutinesButton;
    QToolButton m_focusBothButton;
    QToolButton m_followButton;
    QToolButton m_toggleActionsConnectionsSeparationButton;
    QToolButton m_showHelpButton;

    // # status & loading
    QLabel *m_statusTxt = nullptr;
    QProgressBar *m_loadingBar = nullptr;

    // actions
    // # software
    QAction m_startExperimentLauncherAct;
    QAction m_exitExVRAct;
    QAction m_exitExpLauncherAct;
    // # experiment
    // ## I/O
    QAction m_newExperimentAct;
    QAction m_saveExperimentAct;
    QAction m_saveAsExperimentAct;
    QAction m_exportExperimentAct;
    QAction m_loadExperimentAct;
    QAction m_importSubExperimentAct;
    // ## control from unity
    QAction m_playExperimentAct;
    QAction m_playDelayAct;
    QAction m_playPauseAct;
    QAction m_pauseExperimentAct;
    QAction m_stopExperimentAct;
    QAction m_nextAct;
    QAction m_gotoCurrentAct;
    QAction m_gotoAct;
    QAction m_previousAct;
    // ## load experiment from unity
    QAction m_loadFullExpWithDefaultInstanceAct;
    QAction m_loadFullExpWithSpecificInstanceAct;
    QAction m_loadSelectedRoutineWithDefaultInstanceAct;
    QAction m_loadFromSelectedRoutineWithDefaultInstanceAct;
    QAction m_loadUntilSelectedRoutineWithDefaultInstanceAct;
    // # instance
    QAction m_generateInstancesAct;
    // # logs
    QAction m_clearLogsAct;
    // # others
    QAction m_saveExpToTempAct;
    QAction m_openTempExpAct;
    QAction m_openExpDirectoryAct;
    QAction m_openTempInstanceAct;
    QAction m_openLogsDirectoryAct;
    QAction m_openCurrentDesignerLogAct;
    QAction m_openCurrentExpLauncherLogAct;
    // # dialogs
    QAction m_showDocumentationEditorAct;
    QAction m_aboutAct;
    QAction m_showHelpAct;
    QAction m_settingsAct;
    // # debug
    QAction m_benchmarkAct;
    QAction m_crashAct;
    QAction m_deleteUnusedComponentAct;
    QAction m_displayKeysAct;
    QAction m_fullLoadWith0DurationAct;
    // # resources
    QAction m_showResourcesManagerDialogAct;
    QAction m_showCSharpScriptDirectoryAct;
    // # ui
    QAction m_focusFlowAct;
    QAction m_focusRoutinesAct;
    QAction m_focusBothAct;
    QAction m_toggleActionsConnectionsSeparationAct;
    QAction m_switchModeAct;
    QAction m_toggleFollowConditionAct;

    // timers    
    QTimer m_statusTimer;
    QTimer m_logTimer;

    // logs
    QStringList logs;
    QTextBrowser *m_tbLogs = nullptr;
};
}



