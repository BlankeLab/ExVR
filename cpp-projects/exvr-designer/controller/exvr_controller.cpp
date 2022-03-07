
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

#include "exvr_controller.hpp"

// Qt
#include <QTime>
#include <QStringBuilder>

// local
#include "qt_str.hpp"
#include "utility/script_utility.hpp"

// ui
#include "ui_about_dialog.h"

using namespace tool::ex;

using DMW   = DesignerWindow;
using XMLM  = XmlIoManager;
using EXP   = Experiment;
using CON   = ExVrController;
using LAU   = ExpLauncher;
using ROU   = RoutinesManagerTW;
using COM   = ComponentsManagerW;
using ELW   = ElementViewerW;

ExVrController::ExVrController(const QString &nVersion, bool lncoComponents){

    QThread::currentThread()->setObjectName("main");

    // signal/slots/types
    qRegisterMetaType<tool::ex::Arg>("tool::ex::Arg");
    qRegisterMetaType<tool::ex::RowId>("tool::ex::RowId");
    qRegisterMetaType<tool::ex::UiElementKey>("tool::ex::UiElementKey");
    qRegisterMetaType<tool::ex::SetKey>("tool::ex::SetKey");
    qRegisterMetaType<tool::ex::ActionKey>("tool::ex::ActionKey");
    qRegisterMetaType<tool::ex::ConfigKey>("tool::ex::ConfigKey");
    qRegisterMetaType<std::optional<tool::ex::ConfigKey>>("std::optional<tool::ex::ConfigKey>");
    qRegisterMetaType<tool::ex::IntervalKey>("tool::ex::IntervalKey");
    qRegisterMetaType<tool::ex::TimelineKey>("tool::ex::TimelineKey");
    qRegisterMetaType<tool::ex::ConditionKey>("tool::ex::ConditionKey");
    qRegisterMetaType<tool::ex::ElementKey>("tool::ex::ElementKey");
    qRegisterMetaType<tool::ex::ComponentKey>("tool::ex::ComponentKey");
    qRegisterMetaType<tool::ex::ConnectionKey>("tool::ex::ConnectionKey");
    qRegisterMetaType<tool::ex::ConnectorKey>("tool::ex::ConnectorKey");
    qRegisterMetaType<tool::ex::ExpLauncherState>("tool::ex::ExpLauncherState");
    qRegisterMetaType<tool::ex::ExpState>("tool::ex::ExpState");
    qRegisterMetaType<tool::ex::Settings>("tool::ex::Settings");

    qRegisterMetaType<QStringView>("QStringView");

    qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
    qRegisterMetaType<QProcess::ProcessState>("QProcess::ProcessState");
    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");

    // init logging system
    QtLogger::init(QApplication::applicationDirPath() % QSL("/logs"), QSL("designer_log.html"));
    QtLogger::set_html_ui_type_message_color(QtLogger::MessageType::normal,  QColor(189,189,189));
    QtLogger::set_html_ui_type_message_color(QtLogger::MessageType::warning, QColor(243, 158, 3));
    QtLogger::set_html_ui_type_message_color(QtLogger::MessageType::error,   QColor(244,4,4));
    QtLogger::set_html_ui_type_message_color(QtLogger::MessageType::unknow,  Qt::white);

    // init scripting
    CSharpScript::initialize();


    Bench::disable_display();

    QtLogger::message("[CONTROLLER] Generate signals");
    GSignals::init();

    QtLogger::message("[CONTROLLER] Initialize experiment");
    m_experiment = std::make_unique<Experiment>(nVersion);

    QtLogger::message("[CONTROLLER] Read XML manager");
    m_xmlManager = std::make_unique<XmlIoManager>(exp());

    QtLogger::message("[CONTROLLER] Generate UI");
    m_designerWindow = std::make_unique<DesignerWindow>(lncoComponents);

    // generate exp launcher
    QtLogger::message("[CONTROLLER] Generate Exp Launcher");
    m_expLauncher = std::make_unique<ExpLauncher>();
    connect(this , &ExVrController::start_communication_signal, m_expLauncher.get(), &ExpLauncher::init_communication);
    connect(this , &ExVrController::stop_communication_signal, m_expLauncher.get(), &ExpLauncher::stop_communications);

    m_expLauncher->moveToThread(&m_expLauncherT);
    m_expLauncherT.start();
    emit start_communication_signal();

    // dialogs
    m_documentationD.enable_lnco_components(lncoComponents);
//    m_settingsD.setParent(ui());
    m_benchmarkD     = std::make_unique<BenchmarkDialog>();

    // connections
    QtLogger::message("[CONTROLLER] Generate connections", false, true);
    generate_global_signals_connections();
    generate_main_window_connections();
    generate_flow_diagram_connections();
    generate_controller_connections();
    generate_resources_manager_connections();
    generate_logger_connections();
    generate_dialogs_connections();

    // update ui
    QtLogger::message("[CONTROLLER] Update UI from default experiment", false, true);
    m_designerWindow->update_from_experiment(m_experiment.get(), UpdateAll);
    m_designerWindow->show();

    connect(&experimentUpdateTimer, &QTimer::timeout, this, &ExVrController::update_gui_from_experiment);
    experimentUpdateTimer.start(1000/30);
}

bool ExVrController::eventFilter(QObject *watched, QEvent *event){

    Bench::start("ExVrController::Filter::ALL");
    bool ret;
    if(event->type() == QEvent::Paint){
        Bench::start("ExVrController::Filter::Paint");
        ret = QObject::eventFilter(watched, event);
        Bench::stop();
    }else{
        ret = QObject::eventFilter(watched, event);
    }
    Bench::stop();
    return ret;
}


void ExVrController::close_exvr(){

    // wait for the end of the ui update timer
    experimentUpdateTimer.stop();
    while(experimentUpdateTimer.isActive()){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
    }

    QtLogger::message("[CONTROLLER] Close exvr");

    // close exp launcher
    emit close_exp_launcher_signal();

    // stop communication with exp launcher
    emit stop_communication_signal();

    // wait for communication to stop
    QCoreApplication::processEvents( QEventLoop::AllEvents, 500);

    // threads
    QtLogger::message("[CONTROLLER] Stop threads");
    m_expLauncherT.quit();
    if(!m_expLauncherT.wait(1000)){
        m_expLauncherT.terminate();
    }


    // close ui
    QtLogger::message("[CONTROLLER] Clean UI.");
    m_designerWindow->close_program();
    m_designerWindow    = nullptr;

    // close dialogs
    QtLogger::message("[CONTROLLER] Clean dialogs.");
    if(m_componentsInfoD != nullptr){
        m_componentsInfoD->close();
        m_componentsInfoD = nullptr;
    }
    if(m_goToD != nullptr){
        m_goToD->close();
        m_goToD = nullptr;
    }


    m_benchmarkD->close();
    m_benchmarkD        = nullptr;

    m_resourcesD.close();
    m_documentationD.close();
    m_copyToCondD.close();
    m_instancesD.close();

    // clean
    QtLogger::message("[CONTROLLER] Destroy.");
    m_experiment        = nullptr;
    m_currentInstance   = nullptr;
    m_xmlManager        = nullptr;   
    m_expLauncher       = nullptr;

    QtLogger::message("[CONTROLLER] Clean logs.");
    QtLogger::clean();
}

void ExVrController::show_generate_instances_dialog(){

    m_instancesD.show_dialog();

    if(m_instancesD.directoryPath.size() > 0){

        exp()->update_randomization_seed(m_instancesD.randomSeed);

        if(m_instancesD.useBaseName){
            for(int ii = 0; ii < m_instancesD.nbInstances; ++ii){
                auto instance = Instance::generate_from_full_experiment(&exp()->randomizer, *exp(), ii);
                if(!instance){
                    return;
                }

                const QString instanceFileName =
                        m_instancesD.directoryPath % QSL("/") %
                        m_instancesD.baseName %
                        QString::number(ii+m_instancesD.startId) % QSL(".xml");

                if(!xml()->save_instance_file(*instance, instanceFileName)){
                    return;
                }
                QtLogger::message(QSL("[CONTROLLER] Instance [") %  instanceFileName % QSL("] generated."));
            }
        }else if(m_instancesD.useManual){

            for(int ii = 0; ii < m_instancesD.manualNames.size(); ++ii){

                auto instance = Instance::generate_from_full_experiment(&exp()->randomizer, *exp(), ii);
                if(!instance){
                    return;
                }

                const QString instanceFileName =
                        m_instancesD.directoryPath % QSL("/") %
                        m_instancesD.manualNames[ii] % QSL(".xml");
                if(!xml()->save_instance_file(*instance, instanceFileName)){
                    return;
                }
                QtLogger::message(QSL("[CONTROLLER] Instance [") %  instanceFileName % QSL("] generated."));
            }
        }
    }
}

void ExVrController::save_full_exp_with_default_instance(){

    QtLogger::message("[CONTROLLER] Save full experiment with default instance");
    if(!QDir().exists(Paths::tempDir)){
        QDir().mkdir(Paths::tempDir);
    }

    // save experiment file to temp    
    xml()->save_experiment_to_temp();

    // save instance file to temp    
    exp()->update_randomization_seed(0);
    m_currentInstance = Instance::generate_from_full_experiment(&exp()->randomizer, *exp(), 0);
    if(!m_currentInstance){
        QtLogger::error("[CONTROLLER] Invalid instance generated from experiment.");
        return;
    }
    exp()->set_instance_name(m_currentInstance->fileName);

    xml()->save_instance_to_temp(*m_currentInstance);
}

void ExVrController::go_to_current_specific_instance_element(){

    if(m_currentInstance == nullptr){
        QtLogger::error("[CONTROLLER] No current instance defined.");
        return;
    }

    auto element = exp()->selectedElement;
    if(exp()->selectedElement == nullptr){
        QtLogger::error("[CONTROLLER] No selected element.");
        return;
    }

    if(element->type == Element::Type::Routine){
        auto routine = dynamic_cast<Routine*>(element);
        QString selectedCondition = "";
        for(const auto &condition : routine->conditions){
            if(condition->selected){
                selectedCondition = condition->name;
                break;
            }
        }
        if(selectedCondition.length() == 0){
            QtLogger::error("[CONTROLLER] No selected condition from this routine.");
            return;
        }

        int idOrder = 0;
        for(const auto &instanceElement : m_currentInstance->flow){
            if(instanceElement.elem->key() == routine->key() && instanceElement.condition == selectedCondition){
                QtLogger::message(QSL("[CONTROLLER] Got to element ") % QString::number(idOrder));
                emit go_to_specific_instance_element_signal(idOrder);
                return;
            }
            if(instanceElement.elem->is_routine()){
                if(dynamic_cast<Routine*>(instanceElement.elem)->isARandomizer){
                    continue;
                }
            }
            ++idOrder;
        }
    }else{
        auto isi = dynamic_cast<Isi*>(element);

        int idOrder = 0;
        for(const auto &instanceElement : m_currentInstance->flow){
            if(instanceElement.elem->key() == isi->key()){
                emit go_to_specific_instance_element_signal(idOrder);
                return;
            }
            ++idOrder;
        }
    }
}

void ExVrController::show_got_to_specific_instance_element_dialog(){

    if(m_currentInstance == nullptr){
        QtLogger::error("[CONTROLLER] No current instance defined.");
        return;
    }

    QTableWidget *twInstanceElements = new QTableWidget();

    size_t totalRows = 0;
    for(const auto &element : m_currentInstance->flow){
        if(element.elem->type == Element::Type::Routine){
            if(dynamic_cast<Routine*>(element.elem)->isARandomizer){
                continue;
            }
        }
        ++totalRows;
    }

    twInstanceElements->setRowCount(to_signed(totalRows));
    twInstanceElements->setColumnCount(4);
    twInstanceElements->setSelectionBehavior(QAbstractItemView::SelectRows);

    int orderId = 0;
    std::unordered_map<int,std::unordered_map<QString, int>> countIterations;

    twInstanceElements->setHorizontalHeaderLabels({"Element type", "Element name", "Condition/interval", "Condition iteration"});
    for(const auto &element : m_currentInstance->flow){

        if(element.elem->type == Element::Type::Routine){
            if(dynamic_cast<Routine*>(element.elem)->isARandomizer){
                continue;
            }

            twInstanceElements->setItem(orderId, 0, new QTableWidgetItem("Routine "));
        }else{
            twInstanceElements->setItem(orderId, 0, new QTableWidgetItem("ISI "));
        }

        twInstanceElements->setItem(orderId, 1, new QTableWidgetItem(element.elem->name()));
        twInstanceElements->setItem(orderId, 2, new QTableWidgetItem(element.condition));

        if(element.elem->type == Element::Type::Routine){

            if(countIterations.count(element.elem->key()) == 0){
                countIterations[element.elem->key()] = {};
            }

            if(countIterations[element.elem->key()].count(element.condition) == 0){
                countIterations[element.elem->key()][element.condition] = 0;
            }

            twInstanceElements->setItem(orderId, 3, new QTableWidgetItem(QString::number(countIterations[element.elem->key()][element.condition])));
            countIterations[element.elem->key()][element.condition]++;
        }else{
            twInstanceElements->setItem(orderId, 3, new QTableWidgetItem("-"));
        }
        ++orderId;
    }

    m_goToD = std::make_unique<QDialog>();
    m_goToD->setParent(ui());
    m_goToD->setWindowTitle("Go to specific element from current loaded instance:");
    m_goToD->setLayout(new QVBoxLayout());
    m_goToD->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
    m_goToD->setModal(false);

    QString txt = "Select element to reach:";
    m_goToD->layout()->addWidget(new QLabel(txt));
    m_goToD->layout()->addWidget(twInstanceElements);

    auto pbGoTo  = new QPushButton("Go to");
    auto pbClose = new QPushButton("Close");
    m_goToD->layout()->addWidget(ui::F::gen(ui::L::HB(), {pbGoTo, pbClose}, LStretch{true}, LMargins{true}, QFrame::NoFrame));

    connect(pbGoTo,   &QPushButton::clicked, this, [=]{
        QtLogger::message(QSL("[CONTROLLER] Got to element ") % QString::number(twInstanceElements->currentRow()));
        emit go_to_specific_instance_element_signal(twInstanceElements->currentRow());
    });
    connect(pbClose, &QPushButton::clicked, m_goToD.get(), &QDialog::reject);
    m_goToD->show();
}


void ExVrController::load_full_exp_with_default_instance_to_unity(){

    save_full_exp_with_default_instance();

    // load experiment
    emit load_experiment_unity_signal(Paths::tempExp, Paths::tempInstance);
}


void ExVrController::load_selected_routine_with_default_instance_to_unity(){

    if(exp()->selectedElement == nullptr){
        QtLogger::message("[CONTROLLER] No selected element.");
        return;
    }

    if(exp()->selectedElement->type != Element::Type::Routine){
        QtLogger::message("[CONTROLLER] Selected element must be a routine.");
        return;
    }

    auto currentRoutineW = ui()->routines_manager()->current_routine_widget();
    if(!currentRoutineW){
        return;
    }

    // save the whole current experiment
    xml()->save_experiment_file(Paths::tempExp);

    // init instance with only the current routine and the current condition
    exp()->update_randomization_seed(0);
    m_currentInstance = Instance::generate_from_one_routine(exp()->get_routine(ElementKey{currentRoutineW->routine_key()}));
    if(!m_currentInstance){
        return;
    }
    if(!xml()->save_instance_file(*m_currentInstance,Paths::tempInstance)){
        return;
    }
    exp()->set_instance_name(m_currentInstance->fileName);

    // load experiment
    emit load_experiment_unity_signal(Paths::tempExp, Paths::tempInstance);    
}

void ExVrController::load_from_selected_routine_with_default_instance_to_unity(){

    auto currentRoutineW = ui()->routines_manager()->current_routine_widget();
    if(!currentRoutineW){
        return;
    }

    // save the whole current experiment
    xml()->save_experiment_file(Paths::tempExp);

    // init instance with the routines from the selected one to the end
    exp()->update_randomization_seed(0);


    m_currentInstance = Instance::generate_from_element_to_the_end(&exp()->randomizer, *exp(), 0);
    if(!m_currentInstance){
        return;
    }
    if(!xml()->save_instance_file(*m_currentInstance,Paths::tempInstance)){
        return;
    }
    exp()->set_instance_name(m_currentInstance->fileName);

    // load experiment
    emit load_experiment_unity_signal(Paths::tempExp, Paths::tempInstance);
}

void ExVrController::load_until_selected_routine_with_default_instance_to_unity(){

    auto currentRoutineW = ui()->routines_manager()->current_routine_widget();
    if(!currentRoutineW){
        return;
    }

    // save the whole current experiment
    xml()->save_experiment_file(Paths::tempExp);

    // init instance with the routines from the start to the selected one
    exp()->update_randomization_seed(0);
    m_currentInstance = Instance::generate_from_start_to_element(&exp()->randomizer, *exp(), 0);
    if(!m_currentInstance){
        return;
    }
    if(!xml()->save_instance_file(*m_currentInstance,Paths::tempInstance)){
        return;
    }
    exp()->set_instance_name(m_currentInstance->fileName);

    // load experiment
    emit load_experiment_unity_signal(Paths::tempExp, Paths::tempInstance);
}

void ExVrController::show_component_informations_dialog(ComponentKey componentKey){

    if(const auto component = exp()->get_component(componentKey); component != nullptr){

        std_v1<std::tuple<Routine*,std_v1<std::tuple<Condition*, Action*>>>> containingComponent;
        std_v1<std::tuple<Routine*,std_v1<Condition*>>> notContainingComponent;
        std::unordered_map<int, Config*> usedConfigs;
        std::unordered_map<int, Config*> notUsedConfigs;

        auto routines = exp()->get_elements_from_type<Routine>();
        for(const auto &routine : routines){

            if(routine->isARandomizer){
                continue;
            }

            std_v1<std::tuple<Condition*, Action*>> conditionsContainingComponent;
            std_v1<Condition*> conditionsNotContainingComponent;
            for(const auto& condition : routine->conditions){
                if(auto action = condition->get_action_from_component_key(componentKey, false); action != nullptr){
                    conditionsContainingComponent.emplace_back(std::make_tuple(condition.get(), action));
                    usedConfigs[action->config->key()] = action->config;
                }else{
                    conditionsNotContainingComponent.emplace_back(condition.get());
                }
            }

            if(conditionsContainingComponent.size() != 0){
                containingComponent.emplace_back(std::make_tuple(routine, conditionsContainingComponent));
            }else{
                notContainingComponent.emplace_back(std::make_tuple(routine, conditionsNotContainingComponent));
            }
        }

        for(const auto &config : component->configs){
            if(!usedConfigs.contains(config->key())){
                notUsedConfigs[config->key()] = config.get();
            }
        }


        // dialog
        m_componentsInfoD = std::make_unique<QDialog>();
        m_componentsInfoD->setWindowTitle(QSL("Component informations"));
        m_componentsInfoD->setModal(false);
        m_componentsInfoD->setLayout(new QVBoxLayout());

        QString txt = QSL("Component <b>") % component->name() % QSL("</b> of type <b>") % from_view(Component::get_type_name(component->type)) % QSL("</b> identified in the experiment flow:<br>");
        m_componentsInfoD->layout()->addWidget(new QLabel(txt));


        QTextBrowser *tbInside = new QTextBrowser();
        QString insideTxt;
        if(containingComponent.size() > 0){
            for(const auto& [routine, conditionsActions] : containingComponent){
                if(conditionsActions.size() == 1){
                    insideTxt += QSL("Routine: **[") % routine->name() % QSL("]** with **") % QString::number(conditionsActions.size()) % QSL("** condition referencing it.<br />");
                }else{
                    insideTxt += QSL("Routine: **[") % routine->name() % QSL("]** with **") % QString::number(conditionsActions.size()) % QSL("** conditions referencing it.<br />");
                }
                if(conditionsActions.size() > 0){
                    for(const auto &conditionAction : conditionsActions){
                        auto condition = std::get<0>(conditionAction);
                        auto action    = std::get<1>(conditionAction);
                        insideTxt += QSL(" * Condition **[") % condition->name % QSL("]** with config **[") % action->config->name % QSL("]**<br />");
                    }
                }
                insideTxt += "<br />";
            }
        }
        tbInside->setMarkdown(insideTxt);

        QTextBrowser *tbNotInside = new QTextBrowser();
        QString notInsideTxt;
        if(notContainingComponent.size() > 0){
            for(const auto& [routine, conditions] : notContainingComponent){
                if(conditions.size() == routine->conditions.size()){
                    notInsideTxt += QSL("Routine: **[") % routine->name() % QSL("]** with no condition referencing it.<br />");
                }else{
                    notInsideTxt += QSL("Routine: **[") % routine->name() % QSL("]** with **") % QString::number(conditions.size()) % QSL("** conditions not referencing it.<br />");
                }

                for(const auto &condition : conditions){
                    notInsideTxt += QSL(" * Condition **[") % condition->name % QSL("]**<br />");
                }
                notInsideTxt += "<br />";
            }            
        }
        tbNotInside->setMarkdown(notInsideTxt);

        QTextBrowser *tbConfigsUsed = new QTextBrowser();
        QString configsUsedTxt;
        if(usedConfigs.size() > 0){
            for(const auto &config : usedConfigs){
                configsUsedTxt += QSL("Config: **[") % config.second->name % QSL("]** used. <br />");
            }
        }
        tbConfigsUsed->setMarkdown(configsUsedTxt);

        QTextBrowser *tbConfigsNotUsed = new QTextBrowser();
        QString confisNotUsedTxt;
        if(notUsedConfigs.size() > 0){
            for(const auto &config : notUsedConfigs){
                confisNotUsedTxt += QSL("Config: **[") % config.second->name % QSL("]** not used. <br />");
            }
        }
        tbConfigsNotUsed->setMarkdown(confisNotUsedTxt);

        QTabWidget *tw = new QTabWidget();
        m_componentsInfoD->layout()->addWidget(tw);

        QString txtInside = containingComponent.size() > 1 ?
                    (QSL("Inside ") % QString::number(containingComponent.size()) % QSL(" routines.")) :
                    (QSL("Inside ") % QString::number(containingComponent.size()) % QSL(" routine."));

        QString txtNotInside = containingComponent.size() > 1 ?
                    (QSL("Not inside ") % QString::number(notContainingComponent.size()) % QSL(" routines.")) :
                    (QSL("Not inside ") % QString::number(notContainingComponent.size()) % QSL(" routine."));

        QString txtConfigsUsed = usedConfigs.size() > 1 ?
                    (QSL("Has ") % QString::number(usedConfigs.size()) % QSL(" configs used.")) :
                    (QSL("Has ") % QString::number(usedConfigs.size()) % QSL(" config used."));

        QString txtConfigsNotUsed = notUsedConfigs.size() > 1 ?
                    (QSL("Has ") % QString::number(notUsedConfigs.size()) % QSL(" configs not used.")) :
                    (QSL("Has ") % QString::number(notUsedConfigs.size()) % QSL(" config not used."));

        tw->addTab(tbInside,            txtInside);
        tw->addTab(tbNotInside,         txtNotInside);
        tw->addTab(tbConfigsUsed,       txtConfigsUsed);
        tw->addTab(tbConfigsNotUsed,    txtConfigsNotUsed);

        m_componentsInfoD->show();
    }
}

void ExVrController::update_gui_from_experiment(){

    Bench::start("[Update full UI]"sv, false);
    if(exp()->update_flag() & UpdateSettings){
        Bench::start("[Update dialogs]"sv, false);
        m_settingsD.update_from_settings(exp()->settings());
        Bench::stop();
    }

    if(exp()->update_flag() & UpdateResources){ // update experiment components
        Bench::start("[Update resources]"sv, false);
        m_resourcesD.update_from_resources_manager(ResourcesManager::get());
        Bench::stop();
    }

    Bench::start("[Update designer window]"sv);
    if(exp()->update_flag() != 0){
        ui()->update_from_experiment(exp(), exp()->update_flag());
    }
    Bench::stop();

    // infos
    // # components
    Bench::start("[Update components infos]"sv, false);
    for(const auto &componentI : exp()->componentsInfo){
        for(const auto &configI : componentI.second){
            for(const auto &idI : configI.second){
                ui()->components_manager()->update_component_dialog_with_info(
                    ComponentKey{componentI.first},
                    ConfigKey{configI.first},
                    idI.first,
                    idI.second
                );
            }
        }
    }
    Bench::stop();
    // # infos
    Bench::start("[Update connectors infos]"sv, false);
    for(const auto &elementI : exp()->connectorsInfo){
        for(const auto &conditionI : elementI.second){
            for(const auto &connectorI : conditionI.second){
                for(const auto &idI : connectorI.second){
                    ui()->routines_manager()->update_connector_dialog_with_info(
                        ElementKey{elementI.first},
                        ConditionKey{conditionI.first},
                        ConnectorKey{connectorI.first},
                        idI.first,
                        idI.second
                        );
                }
            }
        }
    }
    Bench::stop();

    exp()->componentsInfo.clear();
    exp()->connectorsInfo.clear();
    exp()->reset_update_flag();

    Bench::stop();


    m_benchmarkD->update();


    Bench::display(BenchUnit::milliseconds, 0, true);
    Bench::reset();

    QCoreApplication::processEvents( QEventLoop::AllEvents, 5);
}

void ExVrController::show_add_action_detailed_dialog(ComponentKey componentKey){

    modalDialog = std::make_unique<QDialog>();
    modalDialog->setParent(ui());
    modalDialog->setModal(true);
    modalDialog->setWindowTitle("Specify details for adding component:");
    modalDialog->setLayout(new QVBoxLayout());
    modalDialog->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
    modalDialog->layout()->setSizeConstraint( QLayout::SetFixedSize );

    auto rbCurrentCondition      = new QRadioButton("... current condition of selected routine");
    auto rbAllRoutineConditions  = new QRadioButton("... all conditions of selected routine");
    auto rbAllRoutinesConditions = new QRadioButton("... all conditions of every routine");
    rbCurrentCondition->setChecked(true);
    modalDialog->layout()->addWidget(ui::F::gen(ui::L::VB(),
        {ui::W::txt("Add to ..."), rbCurrentCondition, rbAllRoutineConditions, rbAllRoutinesConditions}, LStretch{true}, LMargins{true}, QFrame::NoFrame));

    modalDialog->layout()->addWidget(ui::W::horizontal_line());

    auto component = exp()->get_component(componentKey);

    auto cbConfigs = new QComboBox();
    cbConfigs->addItems(component->get_configs_name());
    modalDialog->layout()->addWidget(ui::F::gen(ui::L::HB(),
        {ui::W::txt("Select config to use:"), cbConfigs}, LStretch{true}, LMargins{true}, QFrame::NoFrame));

    modalDialog->layout()->addWidget(ui::W::horizontal_line());

    QRadioButton *rbFillU = nullptr;
    QRadioButton *rbEmptyU = nullptr;
    QRadioButton *rbFillV = nullptr;
    QRadioButton *rbEmptyV = nullptr;

    auto to = Component::get_timeline_opt(component->type);
    if (to == Component::TimelineO::Both || to == Component::TimelineO::Update){
        modalDialog->layout()->addWidget(ui::F::gen(ui::L::HB(),
                                                            {ui::W::txt("Update timeline: "), rbFillU = new QRadioButton("Fill"), rbEmptyU = new QRadioButton("Empty")}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
        rbFillU->setChecked(true);
    }
    if (to == Component::TimelineO::Both || to == Component::TimelineO::Visibility){
        modalDialog->layout()->addWidget(ui::F::gen(ui::L::HB(),
                                                            {ui::W::txt("Visibility timeline: "), rbFillV = new QRadioButton("Fill"), rbEmptyV = new QRadioButton("Empty")}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
            rbFillV->setChecked(true);
    }

    modalDialog->layout()->addWidget(ui::W::horizontal_line());

    auto pbOk  = new QPushButton("Ok");
    auto pbCancel = new QPushButton("Cancel");
    modalDialog->layout()->addWidget(ui::F::gen(ui::L::HB(), {pbOk, pbCancel}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
    ui::L::stretch(modalDialog->layout());

    connect(pbOk,   &QPushButton::clicked, this, [=]{
        bool fillU = rbFillU == nullptr ? false : rbFillU->isChecked();
        bool fillV = rbFillV == nullptr ? false : rbFillV->isChecked();

        const auto idConfig = cbConfigs->currentIndex();
        if(rbCurrentCondition->isChecked()){
            if(auto cRoutineW = ui()->routines_manager()->current_routine_widget(); cRoutineW != nullptr){
                if(auto cCondW = cRoutineW->current_condition_widget(); cCondW != nullptr){
                    exp()->add_action(cRoutineW->routine_key(), cCondW->condition_key(), componentKey,
                        ConfigKey{component->configs[idConfig]->key()}, fillU, fillV);
                }
            }
        }else if(rbAllRoutineConditions->isChecked()){
            if(auto cRoutineW = ui()->routines_manager()->current_routine_widget(); cRoutineW != nullptr){
                exp()->add_action_to_all_conditions(cRoutineW->routine_key(), componentKey,
                    ConfigKey{component->configs[idConfig]->key()}, fillU, fillV);
            }
        }else{
            exp()->add_action_to_all_routines_conditions(componentKey,
            ConfigKey{component->configs[idConfig]->key()}, fillU, fillV);
        }
    });
    connect(pbOk,     &QPushButton::clicked, modalDialog.get(), &QDialog::accept);
    connect(pbCancel, &QPushButton::clicked, modalDialog.get(), &QDialog::reject);
    connect(modalDialog.get(), &QDialog::finished, this, [&]{
        modalDialog = nullptr;
    });

    ui::L::stretch(modalDialog->layout());
    modalDialog->open();
}

void ExVrController::show_modify_action_detailed_dialog(ComponentKey componentKey){

    modalDialog = std::make_unique<QDialog>();
    modalDialog->setParent(ui());
    modalDialog->setModal(true);
    modalDialog->setWindowTitle("Specify details for modifying component:");
    modalDialog->setLayout(new QVBoxLayout());
    modalDialog->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);

    modalDialog->layout()->setSizeConstraint( QLayout::SetFixedSize );

    auto rbCurrentCondition      = new QRadioButton("... current condition of selected routine");
    auto rbAllRoutineConditions  = new QRadioButton("... all conditions of selected routine");
    auto rbAllRoutinesConditions = new QRadioButton("... all conditions of every routine");
    rbCurrentCondition->setChecked(true);
    modalDialog->layout()->addWidget(ui::F::gen(ui::L::VB(),
        {ui::W::txt("Modify from ..."), rbCurrentCondition, rbAllRoutineConditions, rbAllRoutinesConditions}, LStretch{false}, LMargins{true}, QFrame::NoFrame));

    auto component = exp()->get_component(componentKey);

    auto cbConfigs = new QComboBox();
    cbConfigs->addItems(component->get_configs_name());

    auto cbChangeConfig = new QCheckBox("Modify config");
    modalDialog->layout()->addWidget(ui::F::gen(ui::L::HB(),
        {cbChangeConfig}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
    modalDialog->layout()->addWidget(ui::F::gen(ui::L::HB(),
        {ui::W::txt("Select config to apply:"), cbConfigs}, LStretch{true}, LMargins{true}, QFrame::NoFrame));

    modalDialog->layout()->addWidget(ui::W::horizontal_line());

    QRadioButton *rbFillU = nullptr;
    QRadioButton *rbEmptyU = nullptr;
    QRadioButton *rbFillV = nullptr;
    QRadioButton *rbEmptyV = nullptr;

    QCheckBox *cbChangeUpdateTimeline = nullptr;
    QCheckBox *cbChangeVisibilityTimeline = nullptr;

    auto to = Component::get_timeline_opt(component->type);
    if (to == Component::TimelineO::Both || to == Component::TimelineO::Update){
        modalDialog->layout()->addWidget(ui::F::gen(ui::L::HB(),
            {cbChangeUpdateTimeline = new QCheckBox("Modify update timeline")}, LStretch{true}, LMargins{false}, QFrame::NoFrame));

        modalDialog->layout()->addWidget(ui::F::gen(ui::L::HB(),
            {rbFillU = new QRadioButton("Fill"), rbEmptyU = new QRadioButton("Empty")}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
        rbFillU->setChecked(true);
    }
    if (to == Component::TimelineO::Both || to == Component::TimelineO::Visibility){

        modalDialog->layout()->addWidget(ui::F::gen(ui::L::HB(),
            {cbChangeVisibilityTimeline = new QCheckBox("Modify visibility timeline")}, LStretch{true}, LMargins{false}, QFrame::NoFrame));

        modalDialog->layout()->addWidget(ui::F::gen(ui::L::HB(),
            {rbFillV = new QRadioButton("Fill"), rbEmptyV = new QRadioButton("Empty")}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
        rbFillV->setChecked(true);
    }

    modalDialog->layout()->addWidget(ui::W::horizontal_line());

    auto pbOk  = new QPushButton("Ok");
    auto pbCancel = new QPushButton("Cancel");
    modalDialog->layout()->addWidget(ui::F::gen(ui::L::HB(), {pbOk, pbCancel}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
    ui::L::stretch(modalDialog->layout());

    connect(pbOk,   &QPushButton::clicked, this, [=]{

        bool modifyC = cbChangeConfig->isChecked();
        bool modifyU = (cbChangeUpdateTimeline != nullptr) ? cbChangeUpdateTimeline->isChecked() : false;
        bool modifyV = (cbChangeVisibilityTimeline != nullptr) ? cbChangeVisibilityTimeline->isChecked() : false;

        bool fillU = rbFillU == nullptr ? false : rbFillU->isChecked();
        bool fillV = rbFillV == nullptr ? false : rbFillV->isChecked();

        const auto idConfig = cbConfigs->currentIndex();
        if(rbCurrentCondition->isChecked()){
            if(auto cRoutineW = ui()->routines_manager()->current_routine_widget(); cRoutineW != nullptr){
                if(auto cCondW = cRoutineW->current_condition_widget(); cCondW != nullptr){
                    exp()->modify_action(cRoutineW->routine_key(), cCondW->condition_key(), componentKey,
                         modifyC, modifyU, modifyV,
                         ConfigKey{component->configs[idConfig]->key()}, fillU, fillV);
                }
            }
        }else if(rbAllRoutineConditions->isChecked()){
            if(auto cRoutineW = ui()->routines_manager()->current_routine_widget(); cRoutineW != nullptr){
                exp()->modify_action_to_all_conditions(cRoutineW->routine_key(), componentKey,
                   modifyC, modifyU, modifyV,
                   ConfigKey{component->configs[idConfig]->key()}, fillU, fillV);
            }
        }else{
            exp()->modify_action_to_all_routines_conditions(componentKey,
                modifyC, modifyU, modifyV,
                ConfigKey{component->configs[idConfig]->key()}, fillU, fillV);
        }
    });
    connect(pbOk,     &QPushButton::clicked, modalDialog.get(), &QDialog::accept);
    connect(pbCancel, &QPushButton::clicked, modalDialog.get(), &QDialog::reject);
    connect(modalDialog.get(), &QDialog::finished, this, [&]{
        modalDialog = nullptr;
    });

    ui::L::stretch(modalDialog->layout());
    modalDialog->open();
}

void ExVrController::show_copy_to_conditions_dialog(ElementKey routineKey, ConditionKey conditionKey){

    m_copyToCondD.update_from_data(
        routineKey,
        conditionKey,
        exp()->get_elements_from_type<Routine>()
    );
    m_copyToCondD.open();
}

void ExVrController::show_play_with_delay_dialog(){

    modalDialog = std::make_unique<QDialog>();
    modalDialog->setParent(ui());
    modalDialog->setWindowTitle("Specify delay to wait before playing:");
    modalDialog->setLayout(new QVBoxLayout());
    modalDialog->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
    modalDialog->setModal(true);

    QSpinBox *sbDelay = new QSpinBox();
    sbDelay->setRange(0,500);
    sbDelay->setValue(lastDelayS);

    auto pbPlay = new QPushButton("Play");
    auto pbCancel = new QPushButton("Cancel");

    modalDialog->layout()->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Delay (s):"), sbDelay}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
    modalDialog->layout()->addWidget(ui::F::gen(ui::L::HB(), {pbPlay, pbCancel}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
    ui::L::stretch(modalDialog->layout());

    connect(pbPlay,   &QPushButton::clicked, modalDialog.get(), &QDialog::accept);
    connect(pbCancel, &QPushButton::clicked, modalDialog.get(), &QDialog::reject);
    connect(modalDialog.get(), &QDialog::finished, this, [&,sbDelay](int ret){
        if(ret == 1){
            lastDelayS = sbDelay->value();
            emit play_delay_experiment_signal(sbDelay->value());
        }
        modalDialog = nullptr;
    });

    modalDialog->open();
}

void ExVrController::show_about_dialog(){

    Ui_AboutD about;
    modalDialog = std::make_unique<QDialog>();
    modalDialog->setWindowTitle("About ExVR");
    modalDialog->setModal(true);    
    about.setupUi(modalDialog.get());
    about.textEdit->setTextInteractionFlags(Qt::TextBrowserInteraction);
    connect(about.pbClose, &QPushButton::clicked, modalDialog.get(), &QDialog::close);
    connect(modalDialog.get(), &QDialog::finished, [&](int){
        modalDialog = nullptr;
    });
    modalDialog->show();
}


void ExVrController::start_specific_instance(){

    QString pathFile = QFileDialog::getOpenFileName(nullptr, "Select experiment associated instance file to start", Paths::dataDir, "XML (*.xml)");
    if(pathFile.length() == 0){
        return;
    }

    // save experiment file to temp
    xml()->save_experiment_file(Paths::tempExp);

    QtLogger::message(QSL("Start specific instance from file: ") % pathFile);
    m_currentInstance = xml()->load_instance_file(pathFile);
    if(!m_currentInstance){
        QtLogger::error(QSL("Cannot start instance from file dues to errors. "));
        QtLogger::error(QSL("Are you sure this instance has been generated with the current experiment?"));
        return;
    }
    m_experiment->set_instance_name(m_currentInstance->fileName);

    // also save to temp for allowing launcher to reload the last one
    xml()->save_instance_file(*m_currentInstance, Paths::tempInstance);

    // load experiment
    emit load_experiment_unity_signal(Paths::tempExp, pathFile);
}


void ExVrController::generate_global_signals_connections(){

    auto s = GSignals::get();

    // -> component infos
    connect(s, &GSignals::show_component_informations_signal,  this, &CON::show_component_informations_dialog);
    // -> about
    connect(s, &GSignals::show_about_signal, this, &CON::show_about_dialog);
    // -> copy to conditions
    connect(s, &GSignals::copy_condition_to_signal, this, &CON::show_copy_to_conditions_dialog);
    // -> resources
    connect(s, &GSignals::show_resources_manager_dialog_signal, res(), &ResourcesManagerDialog::show_section);
    // -> settings
    connect(s, &GSignals::show_settings_dialog_signal, set(), &SettingsDialog::show);
    // -> settings
    connect(s, &GSignals::show_benchmark_dialog_signal, benchmark(), &BenchmarkDialog::show_dialog);

    // -> documentation
    connect(s, &GSignals::display_component_help_window_signal, doc(), &DocumentationDialog::show_components_section);
    connect(s, &GSignals::show_documentation_signal,            doc(), &DocumentationDialog::show_window);
    connect(s, &GSignals::show_connector_node_documentation_signal, this, [&](ElementKey routineKey, ConditionKey conditionKey, ConnectorKey connectorKey){
        if(auto condition = exp()->get_condition(routineKey, conditionKey); condition != nullptr){
            if(auto connector = condition->get_connector_from_key(connectorKey); connector != nullptr){
                doc()->show_connectors_section(connector->type, true);
            }
        }
    });
    connect(s, &GSignals::show_component_node_documentation_signal, this, [&](ComponentKey componentKey){
        if(auto component = exp()->get_component(componentKey); component != nullptr){
            doc()->show_components_section(component->type, true);
        }
    });

    connect(s, &GSignals::sort_components_by_category_signal,         exp(), &EXP::sort_components_by_category);
    connect(s, &GSignals::sort_components_by_type_signal,             exp(), &EXP::sort_components_by_type);
    connect(s, &GSignals::sort_components_by_name_signal,             exp(), &EXP::sort_components_by_name);
    connect(s, &GSignals::update_component_position_signal,           exp(), &EXP::update_component_position);
    connect(s, &GSignals::add_component_signal,                       exp(), &EXP::add_component);
    connect(s, &GSignals::remove_component_signal,                    exp(), &EXP::remove_component);
    connect(s, &GSignals::duplicate_component_signal,                 exp(), &EXP::duplicate_component);
    connect(s, &GSignals::select_routine_condition_signal,            exp(), &EXP::select_routine_condition);
    connect(s, &GSignals::move_routine_condition_down_signal,         exp(), &EXP::move_routine_condition_down);
    connect(s, &GSignals::move_routine_condition_up_signal,           exp(), &EXP::move_routine_condition_up);
    connect(s, &GSignals::update_element_name_signal,                 exp(), &EXP::update_element_name);
    connect(s, &GSignals::select_loop_set_signal,                     exp(), &EXP::select_loop_set);
    connect(s, &GSignals::modify_loop_nb_reps_signal,                 exp(), &EXP::modify_loop_nb_reps);
    connect(s, &GSignals::modify_loop_n_signal,                       exp(), &EXP::modify_loop_N);
    connect(s, &GSignals::modify_loop_type_signal,                    exp(), &EXP::modify_loop_type);
    connect(s, &GSignals::modify_loop_set_name_signal,                exp(), &EXP::modify_loop_set_name);
    connect(s, &GSignals::modify_loop_set_occurrencies_nb_signal,     exp(), &EXP::modify_loop_set_occurrencies_nb);
    connect(s, &GSignals::remove_set_signal,                          exp(), &EXP::remove_set);
    connect(s, &GSignals::move_loop_set_up_signal,                    exp(), &EXP::move_loop_set_up);
    connect(s, &GSignals::move_loop_set_down_signal,                  exp(), &EXP::move_loop_set_down);
    connect(s, &GSignals::load_loop_sets_file_signal,                 exp(), &EXP::load_loop_sets_file);
    connect(s, &GSignals::reload_loop_sets_file_signal,               exp(), &EXP::reload_loop_sets_file);
    connect(s, &GSignals::add_loop_sets_signal,                       exp(), &EXP::add_loop_sets);
    connect(s, &GSignals::add_isi_interval_signal,                    exp(), &EXP::add_isi_interval);
    connect(s, &GSignals::modify_isi_interval_signal,                 exp(), &EXP::modify_isi_interval);
    connect(s, &GSignals::set_isi_randomize_signal,                   exp(), &EXP::set_isi_randomize);
    connect(s, &GSignals::remove_isi_interval_signal,                 exp(), &EXP::remove_isi_interval);
    connect(s, &GSignals::move_isi_interval_up_signal,                exp(), &EXP::move_isi_interval_up);
    connect(s, &GSignals::move_isi_interval_down_signal,              exp(), &EXP::move_isi_interval_down);
    connect(s, &GSignals::set_routine_as_randomizer_signal,           exp(), &EXP::set_routine_as_randomizer);
    connect(s, &GSignals::update_element_informations_signal,         exp(), &EXP::update_element_informations);
    connect(s, &GSignals::toggle_mode_signal,                         exp(), &EXP::toggle_design_mode);
    connect(s, &GSignals::routine_selected_signal,                    [&]{});
    connect(s, &GSignals::delete_actions_signal,                      exp(), &EXP::delete_actions_from_condition);
    connect(s, &GSignals::fill_actions_signal,                        exp(), &EXP::fill_actions_from_condition);
    connect(s, &GSignals::clean_actions_signal,                       exp(), &EXP::clean_actions_from_condition);
    connect(s, &GSignals::routine_condition_selected_signal,          exp(), &EXP::select_routine_condition);
    connect(s, &GSignals::add_action_signal,                          exp(), &EXP::add_action);
    connect(s, &GSignals::fill_action_signal,                         exp(), &EXP::fill_action);
    connect(s, &GSignals::clean_action_signal,                        exp(), &EXP::clean_action);
    connect(s, &GSignals::move_action_up_signal,                      exp(), &EXP::move_action_up);
    connect(s, &GSignals::move_action_down_signal,                    exp(), &EXP::move_action_down);
    connect(s, &GSignals::select_action_config_signal,                exp(), &EXP::select_action_config);
    connect(s, &GSignals::select_config_signal,                       exp(), &EXP::select_config_in_component);
    connect(s, &GSignals::update_timeline_signal,                     exp(), &EXP::update_condition_timeline);
    connect(s, &GSignals::add_interval_signal,                        exp(), &EXP::add_timeline_interval);
    connect(s, &GSignals::remove_interval_signal,                     exp(), &EXP::remove_timeline_interval);
    connect(s, &GSignals::nodes_connection_created_signal,            exp(), &EXP::create_connection);
    connect(s, &GSignals::connector_node_created_signal,              exp(), &EXP::create_connector_node);
    connect(s, &GSignals::connector_node_modified_signal,             exp(), &EXP::modify_connector_node);
    connect(s, &GSignals::duplicate_connector_node_signal,            exp(), &EXP::duplicate_connector_node);
    connect(s, &GSignals::connector_node_moved_signal,                exp(), &EXP::move_connector_node);
    connect(s, &GSignals::connector_input_connection_validity_signal, exp(), &EXP::set_connector_input_connection_validity);
    connect(s, &GSignals::component_node_created_signal,              exp(), &EXP::create_component_node);
    connect(s, &GSignals::component_node_moved_signal,                exp(), &EXP::move_component_node);
    connect(s, &GSignals::delete_nodes_and_connections_signal,        exp(), &EXP::delete_nodes_and_connections);
    connect(s, &GSignals::delete_selected_nodes_signal,               exp(), &EXP::delete_selected_nodes);
    connect(s, &GSignals::delete_connections_signal,                  exp(), &EXP::delete_connections);    
    connect(s, &GSignals::unselect_nodes_and_connections_signal,      exp(), &EXP::unselect_nodes_and_connections);
    connect(s, &GSignals::select_nodes_and_connections_signal,        exp(), &EXP::select_nodes_and_connections);
    connect(s, &GSignals::toggle_follow_condition_mode_signal,        exp(), &EXP::toggle_follow_condition_mode);
    connect(s, &GSignals::arg_removed_signal,                         exp(), &EXP::arg_removed);
    connect(s, &GSignals::swap_arg_signal,                            exp(), &EXP::swap_arg);
    connect(s, &GSignals::new_arg_signal,                             exp(), &EXP::new_arg);
    connect(s, &GSignals::arg_updated_signal,                         exp(), &EXP::arg_updated);
    connect(s, &GSignals::component_name_changed_signal,              exp(), &EXP::update_component_name);
    connect(s, &GSignals::insert_config_signal,                       exp(), &EXP::insert_config_in_component);
    connect(s, &GSignals::copy_config_signal,                         exp(), &EXP::copy_config_from_component);
    connect(s, &GSignals::remove_config_signal,                       exp(), &EXP::remove_config_from_component);
    connect(s, &GSignals::move_config_signal,                         exp(), &EXP::move_config_in_component);
    connect(s, &GSignals::rename_config_signal,                       exp(), &EXP::rename_config_in_component);
    connect(s, &GSignals::exp_state_updated_signal,                   exp(), &EXP::update_exp_state);
    connect(s, &GSignals::exp_launcher_state_updated_signal,          exp(), &EXP::update_exp_launcher_state);
    connect(s, &GSignals::connector_info_update_signal,               exp(), &EXP::update_connector_dialog_with_info);
    connect(s, &GSignals::component_info_update_signal,               exp(), &EXP::update_component_dialog_with_info);
    connect(s, &GSignals::remove_action_from_all_routines_conditions_signal, exp(), &EXP::remove_action_from_all_routines_conditions);
    connect(s, &GSignals::insert_action_to_all_routines_conditions_signal,   exp(), &EXP::add_action_to_all_routines_conditions);
    connect(s, &GSignals::delete_action_signal, this, [&](ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey){
        exp()->remove_action_from_condition(routineKey, conditionKey, actionKey, true);
    });


    connect(s, &GSignals::routine_selected_signal,                    this, [&](ElementKey elementKey){
        exp()->select_element(elementKey);
    });
    connect(s, &GSignals::insert_action_to_selected_routine_signal, this, [&](ComponentKey componentKey){
        if(auto cRoutineW = ui()->routines_manager()->current_routine_widget(); cRoutineW != nullptr){
            if(auto cCondW = cRoutineW->current_condition_widget(); cCondW != nullptr){
                exp()->add_action(cRoutineW->routine_key(), cCondW->condition_key(), componentKey, {}, true, true);
            }
        }
    });
    connect(s, &GSignals::insert_action_to_all_selected_routine_conditions_signal, this, [&](ComponentKey componentKey){
        if(auto cRoutineW = ui()->routines_manager()->current_routine_widget(); cRoutineW != nullptr){
            exp()->add_action_to_all_conditions(cRoutineW->routine_key(), componentKey, {}, true, true);
        }
    });

    connect(s, &GSignals::insert_action_with_details_signal, this, &ExVrController::show_add_action_detailed_dialog);
    connect(s, &GSignals::modify_action_with_details_signal, this, &ExVrController::show_modify_action_detailed_dialog);

    connect(s, &GSignals::remove_action_from_all_selected_routine_conditions_signal, this, [&](ComponentKey componentKey){
        if(auto cRoutineW = ui()->routines_manager()->current_routine_widget(); cRoutineW != nullptr){
            exp()->remove_action_from_all_selected_routine_conditions(cRoutineW->routine_key(), componentKey);
        }
    });
    // -> exp launcher
    connect(s, &GSignals::connector_node_modified_signal, [&](ElementKey routineKey, ConditionKey conditionKey, ConnectorKey connectorKey, QString name, Arg arg){
        if(!exp()->states.neverLoaded){
            exp_launcher()->update_connector_node(routineKey, conditionKey, connectorKey, std::move(name), std::move(arg));
        }
    });
    connect(s, &GSignals::arg_updated_signal, [&](ComponentKey componentKey, ConfigKey configKey, Arg arg, bool initConfig){
        if(!exp()->states.neverLoaded){
            exp_launcher()->update_component_config_argument(componentKey, configKey, std::move(arg), initConfig);
        }
    });
    connect(s, &GSignals::action_signal, [&](ComponentKey componentKey, ConfigKey configKey, QStringView actionName, bool initConfig){
        if(!exp()->states.neverLoaded){
            exp_launcher()->trigger_component_config_action(componentKey, configKey, actionName, initConfig);
        }
    });

    // -> components manager
    auto componentsM = ui()->components_manager();
    connect(s, &GSignals::show_component_custom_menu_signal,          componentsM, &COM::show_howering_component_custom_menu);
    connect(s, &GSignals::toggle_component_parameters_signal,         componentsM, &COM::toggle_component_parameters_dialog);
    connect(s, &GSignals::toggle_selection_component_signal,          componentsM, &COM::toggle_component_selection);
    connect(s, &GSignals::enter_component_signal,                     componentsM, &COM::update_style);
    connect(s, &GSignals::leave_component_signal,                     componentsM, &COM::update_style);
}

void ExVrController::generate_main_window_connections(){


    // ### from main window
    // -> this
    connect(ui(), &DMW::new_experiment_signal,      this, [&]{
        xml()->save_experiment_file(tool::ex::Paths::tempDir % QSL("/deleted.xml"));
        exp_launcher()->stop_experiment();
        exp_launcher()->clean_experiment();
        exp()->new_experiment();
    });
    connect(ui(), &DMW::play_delay_experiment_signal,   this, &ExVrController::show_play_with_delay_dialog);
    // -> xml manager
    connect(ui(), &DMW::save_experiment_signal,                     xml(), &XMLM::save_experiment);
    connect(ui(), &DMW::save_experiment_as_signal,                  xml(), &XMLM::save_experiment_as);
    connect(ui(), &DMW::export_experiment_as_signal,                xml(), &XMLM::export_experiment_to);
    connect(ui(), &DMW::open_temp_experiment_file_signal,           xml(), &XMLM::open_temp_experiment_file);
    connect(ui(), &DMW::open_experiment_directory_signal,           xml(), &XMLM::open_experiment_directory);
    connect(ui(), &DMW::open_temp_instance_file_signal,             xml(), &XMLM::open_temp_instance_file);

    connect(ui(), &DMW::load_experiment_signal,        this, [&](){

        QFileInfo fileInfo(exp()->states.currentExpfilePath);
        QString parentDirPath = "";
        if(fileInfo.exists()){
            parentDirPath = fileInfo.absoluteDir().path();
        }else{
            parentDirPath = Paths::expDir;
        }

        QString path = QFileDialog::getOpenFileName(nullptr, "Experiment file", parentDirPath, "XML (*.xml)");
        if(path.length() > 0){
            if(!exp()->states.neverLoaded){
                exp_launcher()->stop_experiment();
                exp_launcher()->clean_experiment();
            }
            exp()->clean_experiment();
            xml()->load_experiment_file(path);
            exp()->add_to_update_flag(UpdateAll | ResetUI);
        }
    });

    connect(ui(), &DMW::load_dropped_experiment_signal,        this, [&](QString path){
        if(path.length() > 0){
            if(!exp()->states.neverLoaded){
                exp_launcher()->stop_experiment();
                exp_launcher()->clean_experiment();
            }
            exp()->clean_experiment();
            xml()->load_experiment_file(path);
            exp()->add_to_update_flag(UpdateAll | ResetUI);
        }
    });

    connect(ui(), &DMW::open_current_exp_launcher_log_file_signal,  xml(), &XMLM::open_current_exp_launcher_log_file);
    connect(ui(), &DMW::open_current_designer_log_file_signal,      xml(), &XMLM::open_current_designer_log_file);
    connect(ui(), &DMW::open_log_directory_signal,                  xml(), &XMLM::open_log_directory);

    // -> controller
    connect(ui(), &DMW::close_exvr_signal,                                          this, &CON::close_exvr);
    connect(ui(), &DMW::generate_instances_signals,                                 this, &CON::show_generate_instances_dialog);
    connect(ui(), &DMW::load_full_exp_wtih_default_instance_signal,                 this, &CON::load_full_exp_with_default_instance_to_unity);
    connect(ui(), &DMW::load_selected_routine_with_default_instance_signal,         this, &CON::load_selected_routine_with_default_instance_to_unity);
    connect(ui(), &DMW::load_from_selected_routine_with_default_instance_signal,    this, &CON::load_from_selected_routine_with_default_instance_to_unity);
    connect(ui(), &DMW::load_until_selected_routine_with_default_instance_signal,   this, &CON::load_until_selected_routine_with_default_instance_to_unity);
    connect(ui(), &DMW::load_full_exp_with_specific_instance_signal,                this, &CON::start_specific_instance);
    connect(ui(), &DMW::save_experiment_to_temp_signal,                             this, &CON::save_full_exp_with_default_instance);    
    connect(ui(), &DMW::go_to_current_element_signal,                               this, &CON::go_to_current_specific_instance_element);
    connect(ui(), &DMW::go_to_specific_element_signal,                              this, &CON::show_got_to_specific_instance_element_dialog);
    connect(ui(), &DMW::start_experiment_launcher_signal,                           this, [&](){
        emit start_experiment_launcher_signal(*exp()->settings());
    });

    // -> gui
    connect(ui(), &DMW::toggle_actions_connections_separation_signal,               exp(), &EXP::toggle_actions_connections_separation);
    // -> exp launcher    
    connect(ui(), &DMW::exit_experiment_launcher_signal,    exp_launcher(), &LAU::close_program);
    connect(ui(), &DMW::play_experiment_signal,             exp_launcher(), &LAU::start_experiment);
    connect(ui(), &DMW::play_pause_experiment_signal,       exp_launcher(), &LAU::play_pause_experiment);
    connect(ui(), &DMW::pause_experiment_signal,            exp_launcher(), &LAU::pause_experiment);
    connect(ui(), &DMW::stop_experiment_signal,             exp_launcher(), &LAU::stop_experiment);
    connect(ui(), &DMW::next_element_signal,                exp_launcher(), &LAU::next_element);
    connect(ui(), &DMW::previous_element_signal,            exp_launcher(), &LAU::previous_element);    
}



void ExVrController::generate_flow_diagram_connections(){

    auto flowDiagram = ui()->flow_diagram();
    // -> experiment
    connect(flowDiagram, &FlowDiagramW::select_element_signal,          this, [&](ElementKey elementKey){
        exp()->select_element(elementKey);}
    );
    connect(flowDiagram, &FlowDiagramW::unselect_element_signal,        this, [&](){exp()->unselect_all_elements();});
    connect(flowDiagram, &FlowDiagramW::add_element_signal,             exp(), &EXP::add_element);
    connect(flowDiagram, &FlowDiagramW::remove_selected_element_signal, exp(), &EXP::remove_selected_element);
    connect(flowDiagram, &FlowDiagramW::move_element_left_signal,       exp(), &EXP::move_left);
    connect(flowDiagram, &FlowDiagramW::move_element_right_signal,      exp(), &EXP::move_right);
    connect(flowDiagram, &FlowDiagramW::duplicate_element_signal,       exp(), &EXP::duplicate_element);
    connect(flowDiagram, &FlowDiagramW::remove_element_signal,          exp(), &EXP::remove_element_of_key);
    connect(flowDiagram, &FlowDiagramW::clean_current_routine_condition_signal, exp(), &EXP::clean_current_routine_condition);
    connect(flowDiagram, &FlowDiagramW::clean_all_routine_conditions_signal,    exp(), &EXP::clean_all_routine_conditions);
    connect(flowDiagram, &FlowDiagramW::set_duration_for_all_routine_conditions_signal,    exp(), &EXP::set_duration_for_all_routine_conditions);
}

void ExVrController::generate_controller_connections(){
    // -> exp launcher
    connect(this, &CON::load_experiment_unity_signal,           exp_launcher(), &LAU::load_experiment);
    connect(this, &CON::close_exp_launcher_signal,              exp_launcher(), &LAU::close_program);
    connect(this, &CON::go_to_specific_instance_element_signal, exp_launcher(), &LAU::go_to_specific_instance_element);
    connect(this, &CON::play_delay_experiment_signal,           exp_launcher(), &LAU::play_delay_experiment);
    connect(this, &CON::start_experiment_launcher_signal,       exp_launcher(), &LAU::start_program);
}

void ExVrController::generate_resources_manager_connections(){
    // -> experiment
    connect(res(), &ResourcesManagerDialog::add_resources_signal,               exp(),  &EXP::add_resources);
    connect(res(), &ResourcesManagerDialog::update_resource_path_signal,        exp(),  &EXP::update_resource_path);
    connect(res(), &ResourcesManagerDialog::update_resource_alias_signal,       exp(),  &EXP::update_resource_alias);
    connect(res(), &ResourcesManagerDialog::resource_selected_signal,           exp(),  &EXP::select_resource);
    connect(res(), &ResourcesManagerDialog::clean_resources_signal,             exp(),  &EXP::clean_resources);
    connect(res(), &ResourcesManagerDialog::remove_resource_signal,             exp(),  &EXP::remove_resource);
    connect(res(), &ResourcesManagerDialog::update_reload_resource_code_signal, exp(),  &EXP::update_reload_resource_code);
}

void ExVrController::generate_logger_connections(){
    // -> main window
    connect(log(), &QtLogger::message_signal,         ui(), &DMW::insert_log_from_ui);
    connect(log(), &QtLogger::error_signal,           ui(), &DMW::insert_log_from_ui);
    connect(log(), &QtLogger::warning_signal,         ui(), &DMW::insert_log_from_ui);
    connect(log(), &QtLogger::unity_message_signal,   ui(), &DMW::insert_log_from_ui);
    connect(log(), &QtLogger::unity_error_signal,     ui(), &DMW::insert_log_from_ui);
    connect(log(), &QtLogger::unity_warning_signal,   ui(), &DMW::insert_log_from_ui);
    connect(log(), &QtLogger::status_signal,          ui(), &DMW::display_status);
}

void ExVrController::generate_dialogs_connections(){
    // -> settings dialogs
    connect(set(), &SettingsDialog::reset_settings_signal,      exp(), &EXP::reset_settings);
    connect(set(), &SettingsDialog::settings_updated_signal,    exp(), &EXP::update_settings);
    connect(set(), &SettingsDialog::settings_canceled_signal,   this, [&]{
        m_settingsD.update_from_settings(exp()->settings());
        exp()->add_to_update_flag(UpdateSettings);
    });

    connect(&m_copyToCondD, &CopyToConditionDialog::copy_to_conditions_signal, exp(), &Experiment::copy_to_conditions);
}


#include "moc_exvr_controller.cpp"
