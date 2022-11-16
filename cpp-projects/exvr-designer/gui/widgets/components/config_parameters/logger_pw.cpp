
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

#include "logger_pw.hpp"

// qt-utility
#include "gui/ex_widgets/ex_checkbox_w.hpp"
#include "gui/ex_widgets/ex_radio_button_w.hpp"
#include "gui/ex_widgets/ex_line_edit_w.hpp"

// local
#include "gui/ex_widgets/ex_resource_w.hpp"
#include "gui/ex_widgets/ex_component_w.hpp"
#include "gui/ex_widgets/ex_components_list_w.hpp"

using namespace tool::ex;
using namespace tool::ui;

struct LoggerInitConfigParametersW::Impl{

    ExResourceW resource{"directory"};

    ExCheckBoxW addCurrentInstanceToFileName{"add_current_instance_to_file_name"};
    ExCheckBoxW addDateToFileName{"add_date_to_file_name"};
    ExCheckBoxW dontWriteIfFileExists{"dont_write_if_file_exists"};
    ExCheckBoxW insertNewRoutineInfo{"insert_new_routine_infos"};

    QButtonGroup group1;
    ExRadioButtonW addToEndIfFileExists{"add_to_end_if_file_exists"};
    ExRadioButtonW replaceIfFileExists{"replace_if_file_exists"};

    ExLineEditW baseFileName{"base_file_name"};
    ExLineEditW dateTimeFormat{"date_time_format"};
    ExLineEditW fileExtension{"file_extension"};

    ExCheckBoxW addHeaderLine{"add_header_line"};
    ExLineEditW headerLine{"header_line"};
};

LoggerInitConfigParametersW::LoggerInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void LoggerInitConfigParametersW::insert_widgets(){

    add_widget(F::gen(L::VB(),{m_p->resource()}, LStretch{false}));
    add_widget(F::gen(L::VB(),
        {m_p->dontWriteIfFileExists(), m_p->insertNewRoutineInfo(), m_p->replaceIfFileExists(), m_p->addToEndIfFileExists()},
        LStretch{false}, LMargins{true}, QFrame::Box)
    );

    auto l1 = F::gen(L::HB(), {W::txt("Logging file name:"), m_p->baseFileName()}, LStretch{false}, LMargins{false});
    auto l2 = F::gen(L::HB(), {m_p->addCurrentInstanceToFileName()}, LStretch{true}, LMargins{false});
    auto l3 = F::gen(L::HB(), {m_p->addDateToFileName()}, LStretch{true}, LMargins{false});
    auto l4 = F::gen(L::HB(), {W::txt("Date format:"),    m_p->dateTimeFormat()},  LStretch{true}, LMargins{false});
    auto l5 = F::gen(L::HB(), {W::txt("File extension:"), m_p->fileExtension()}, LStretch{true}, LMargins{false});
    auto l6 = F::gen(L::HB(), {m_p->addHeaderLine()}, LStretch{true}, LMargins{false});
    auto l7 = F::gen(L::HB(), {W::txt("Header line:"), m_p->headerLine()}, LStretch{false}, LMargins{false});

    add_widget(F::gen(L::VB(),
        {l1,l2,l3,l4,l5,l6,l7},
        LStretch{false}, LMargins{true}, QFrame::Box)
    );
}

void LoggerInitConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_p->resource.init_widget(Resource::Type::Directory, "Directory to use:"));
    add_input_ui(m_p->addCurrentInstanceToFileName.init_widget("Add current instance to name", true));
    add_input_ui(m_p->addDateToFileName.init_widget("Add current date to name", true));
    add_input_ui(m_p->dontWriteIfFileExists.init_widget("Dont write if file already exists", false));
    add_input_ui(m_p->insertNewRoutineInfo.init_widget("Insert info at each new routine", false));

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
            {&m_p->replaceIfFileExists, &m_p->addToEndIfFileExists},
            {
                "Replace file if exists",
                "Add to end of file if exists",
            },
            {true, false}
        )
    );

    add_input_ui(m_p->baseFileName.init_widget("log"));
    add_input_ui(m_p->dateTimeFormat.init_widget("yyyy-MM-dd_H-mm-ss"));
    add_input_ui(m_p->fileExtension.init_widget("txt"));

    add_input_ui(m_p->addHeaderLine.init_widget("Add header line", false));
    add_input_ui(m_p->headerLine.init_widget(""));
}

void LoggerInitConfigParametersW::create_connections(){
    connect(m_p->addDateToFileName(), &QCheckBox::clicked, this, [&](bool checked){
        m_p->dateTimeFormat.w->setEnabled(checked);
    });
    connect(m_p->addHeaderLine(), &QCheckBox::clicked, this, [&](bool checked){
        m_p->headerLine.w->setEnabled(checked);
    });
}

void LoggerInitConfigParametersW::late_update_ui(){
    m_p->dateTimeFormat.w->setEnabled(m_p->addDateToFileName.w->isChecked());
    m_p->headerLine.w->setEnabled(m_p->addHeaderLine.w->isChecked());
}


struct LoggerColumnsInitConfigParametersW::Impl{

    ExResourceW resource{"directory"};

    ExCheckBoxW addCurrentInstanceToFileName{"add_current_instance_to_file_name"};
    ExCheckBoxW addDateToFileName{"add_date_to_file_name"};
    ExCheckBoxW dontWriteIfFileExists{"dont_write_if_file_exists"};

    QButtonGroup group1;
    ExRadioButtonW addToEndIfFileExists{"add_to_end_if_file_exists"};
    ExRadioButtonW replaceIfFileExists{"replace_if_file_exists"};

    ExLineEditW baseFileName{"base_file_name"};
    ExLineEditW dateTimeFormat{"date_time_format"};
    ExLineEditW fileExtension{"file_extension"};

    ExCheckBoxW addHeaderLine{"add_header_line"};
    ExLineEditW headerLine{"header_line"};

    ExLineEditW separator{"separator"};
    ExCheckBoxW writeAtEnfOfEachFrame{"write_each_frame"};
};

LoggerColumnsInitConfigParametersW::LoggerColumnsInitConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void LoggerColumnsInitConfigParametersW::insert_widgets(){

    add_widget(F::gen(L::VB(),{m_p->resource()}, LStretch{false}));
    add_widget(F::gen(L::VB(),
        {m_p->dontWriteIfFileExists(), m_p->replaceIfFileExists(), m_p->addToEndIfFileExists(), m_p->writeAtEnfOfEachFrame()},
        LStretch{false}, LMargins{true}, QFrame::Box)
    );

    auto l1 = F::gen(L::HB(), {W::txt("Logging file name:"), m_p->baseFileName()}, LStretch{false}, LMargins{false});
    auto l2 = F::gen(L::HB(), {m_p->addCurrentInstanceToFileName()}, LStretch{true}, LMargins{false});
    auto l3 = F::gen(L::HB(), {m_p->addDateToFileName()}, LStretch{true}, LMargins{false});
    auto l4 = F::gen(L::HB(), {W::txt("Date format:"),    m_p->dateTimeFormat()},  LStretch{true}, LMargins{false});
    auto l5 = F::gen(L::HB(), {W::txt("File extension:"), m_p->fileExtension()}, LStretch{true}, LMargins{false});
    auto l6 = F::gen(L::HB(), {W::txt("Columns separator:"), m_p->separator()}, LStretch{false}, LMargins{false});
    auto l7 = F::gen(L::HB(), {m_p->addHeaderLine()}, LStretch{true}, LMargins{false});
    auto l8 = F::gen(L::HB(), {W::txt("Header line:"), m_p->headerLine()}, LStretch{false}, LMargins{false});

    add_widget(F::gen(L::VB(),
        {l1,l2,l3,l4,l5,l6,l7,l8},
        LStretch{false}, LMargins{true}, QFrame::Box)
    );
}

void LoggerColumnsInitConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_p->resource.init_widget(Resource::Type::Directory, "Directory to use:"));
    add_input_ui(m_p->addCurrentInstanceToFileName.init_widget("Add current instance to name", true));
    add_input_ui(m_p->addDateToFileName.init_widget("Add current date to name", true));
    add_input_ui(m_p->dontWriteIfFileExists.init_widget("Dont write if file already exists", false));

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
            {&m_p->replaceIfFileExists, &m_p->addToEndIfFileExists},
            {
                "Replace file if exists",
                "Add to end of file if exists",
            },
            {true, false}
        )
    );


    add_input_ui(m_p->baseFileName.init_widget("log"));
    add_input_ui(m_p->dateTimeFormat.init_widget("yyyy-MM-dd_H-mm-ss"));
    add_input_ui(m_p->fileExtension.init_widget("csv"));

    add_input_ui(m_p->addHeaderLine.init_widget("Add header line", false));
    add_input_ui(m_p->headerLine.init_widget(""));

    add_input_ui(m_p->writeAtEnfOfEachFrame.init_widget("Write columns at the end of each frame", true));
    add_input_ui(m_p->separator.init_widget(";"));
}

void LoggerColumnsInitConfigParametersW::create_connections(){

    connect(m_p->addDateToFileName(), &QCheckBox::clicked, this, [&](bool checked){
        m_p->dateTimeFormat.w->setEnabled(checked);
    });
    connect(m_p->addHeaderLine(), &QCheckBox::clicked, this, [&](bool checked){
        m_p->headerLine.w->setEnabled(checked);
    });
}

void LoggerColumnsInitConfigParametersW::late_update_ui(){
    m_p->dateTimeFormat.w->setEnabled(m_p->addDateToFileName.w->isChecked());
    m_p->headerLine.w->setEnabled(m_p->addHeaderLine.w->isChecked());
}

struct LoggerConditionInitConfigParametersW::Impl{

    ExResourceW resource{"directory"};

    ExCheckBoxW dontWriteIfFileExists{"dont_write_if_file_exists"};
    ExCheckBoxW addCurrentInstanceToFileName{"add_current_instance_to_file_name"};

    QButtonGroup group1;
    ExRadioButtonW addRoutineName{"add_routine"};
    ExRadioButtonW addConditionName{"add_condition"};
    ExRadioButtonW addBoth{"add_both"};

    QButtonGroup group2;
    ExRadioButtonW addToEndIfFileExists{"add_to_end_if_file_exists"};
    ExRadioButtonW replaceIfFileExists{"replace_if_file_exists"};

    ExLineEditW baseFileName{"base_file_name"};
    ExLineEditW fileExtension{"file_extension"};

    ExCheckBoxW addHeaderLine{"add_header_line"};
    ExLineEditW headerLine{"header_line"};
};

LoggerConditionInitConfigParametersW::LoggerConditionInitConfigParametersW():  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void LoggerConditionInitConfigParametersW::insert_widgets(){

    add_widget(F::gen(L::VB(),{m_p->resource()}, LStretch{false}));
    add_widget(F::gen(L::VB(),
        {m_p->dontWriteIfFileExists(), m_p->replaceIfFileExists(), m_p->addToEndIfFileExists()},
        LStretch{false}, LMargins{true}, QFrame::Box)
    );

    auto l1 = F::gen(L::HB(), {W::txt("Logging file base name:"), m_p->baseFileName()}, LStretch{false}, LMargins{false});
    auto l2 = F::gen(L::VB(), {m_p->addRoutineName(), m_p->addConditionName(), m_p->addBoth(), m_p->addCurrentInstanceToFileName()}, LStretch{false}, LMargins{false});
    auto l3 = F::gen(L::HB(), {W::txt("File extension:"), m_p->fileExtension()}, LStretch{true}, LMargins{false});
    auto l4 = F::gen(L::HB(), {m_p->addHeaderLine()}, LStretch{true}, LMargins{false});
    auto l5 = F::gen(L::HB(), {W::txt("Header line:"), m_p->headerLine()}, LStretch{false}, LMargins{false});

    add_widget(F::gen(L::VB(),
        {l1,l2,l3,l4,l5},
        LStretch{false}, LMargins{true}, QFrame::Box)
    );
}

void LoggerConditionInitConfigParametersW::init_and_register_widgets(){

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group1,
            {&m_p->addRoutineName, &m_p->addConditionName, &m_p->addBoth},
            {
                "Add routine to file name",
                "Add condition to file name",
                "Add both",
            },
            {false, false, true}
        )
    );

    add_input_ui(m_p->addCurrentInstanceToFileName.init_widget("Add current instance to name", true));
    add_input_ui(m_p->resource.init_widget(Resource::Type::Directory, "Directory to use:"));

    add_input_ui(m_p->dontWriteIfFileExists.init_widget("Dont write if file already exists", false));

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(m_p->group2,
            {&m_p->replaceIfFileExists, &m_p->addToEndIfFileExists},
            {
                "Replace file if exists",
                "Add to end of file if exists",
            },
            {true, false}
        )
    );
    add_input_ui(m_p->baseFileName.init_widget("log"));

    add_input_ui(m_p->fileExtension.init_widget("txt"));

    add_input_ui(m_p->addHeaderLine.init_widget("Add header line", false));
    add_input_ui(m_p->headerLine.init_widget(""));
}

void LoggerConditionInitConfigParametersW::create_connections(){
    connect(m_p->addHeaderLine(), &QCheckBox::clicked, this, [&](bool checked){
        m_p->headerLine.w->setEnabled(checked);
    });
}

void LoggerConditionInitConfigParametersW::late_update_ui(){
    m_p->headerLine.w->setEnabled(m_p->addHeaderLine.w->isChecked());
}




struct LoggerExperimentInitConfigParametersW::Impl{

    ExResourceW resource{"directory"};

    ExCheckBoxW addCurrentInstanceToFileName{"add_current_instance_to_file_name"};
    ExCheckBoxW addDateToFileName{"add_date_to_file_name"};
    ExCheckBoxW dontWriteIfFileExists{"dont_write_if_file_exists"};


    QButtonGroup group1;
    ExRadioButtonW addToEndIfFileExists{"add_to_end_if_file_exists"};
    ExRadioButtonW replaceIfFileExists{"replace_if_file_exists"};

    ExLineEditW baseFileName{"base_file_name"};
    ExLineEditW dateTimeFormat{"date_time_format"};
    ExLineEditW fileExtension{"file_extension"};

    ExCheckBoxW addHeaderLine{"add_header_line"};

    ExLineEditW separator{"separator"};
    ExCheckBoxW eachFrame{"each_frame"};
    ExCheckBoxW addTimeExp{"time_exp"};
    ExCheckBoxW addTimeRoutine{"time_routine"};
    ExCheckBoxW addRoutine{"routine"};
    ExCheckBoxW addRoutineIter{"routine_iter"};
    ExCheckBoxW addCondition{"condition"};
    ExCheckBoxW addConditionIter{"condition_iter"};
    ExCheckBoxW addFrameId{"frame_id"};
};

LoggerExperimentInitConfigParametersW::LoggerExperimentInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void LoggerExperimentInitConfigParametersW::insert_widgets(){

    add_widget(F::gen(L::VB(),{m_p->resource()}, LStretch{false}));
    add_widget(F::gen(L::VB(),
        {m_p->dontWriteIfFileExists(), m_p->replaceIfFileExists(), m_p->addToEndIfFileExists()},
        LStretch{false}, LMargins{true}, QFrame::Box)
    );

    add_widget(F::gen(L::VB(),
    {
        F::gen(L::HB(), {W::txt("Logging file name:"), m_p->baseFileName()}, LStretch{false}, LMargins{false}),
        F::gen(L::HB(), {m_p->addCurrentInstanceToFileName()}, LStretch{true}, LMargins{false}),
        F::gen(L::HB(), {m_p->addDateToFileName()}, LStretch{true}, LMargins{false}),
        F::gen(L::HB(), {W::txt("Date format:"),    m_p->dateTimeFormat()},  LStretch{true}, LMargins{false}),
        F::gen(L::HB(), {W::txt("File extension:"), m_p->fileExtension()}, LStretch{true}, LMargins{false}),
        F::gen(L::HB(), {m_p->addHeaderLine()}, LStretch{true}, LMargins{false})},
        LStretch{false}, LMargins{true}, QFrame::Box)
    );

    add_widget(F::gen(L::VB(),
    {
        F::gen(L::HB(), {W::txt("Columns separator:"), m_p->separator()}, LStretch{false}, LMargins{false}),
        F::gen(L::HB(), {m_p->eachFrame()}, LStretch{true}, LMargins{false}),
        F::gen(L::HB(), {W::txt("Add columns:")}, LStretch{true}, LMargins{false}),
        F::gen(L::HB(), {m_p->addTimeExp(), m_p->addTimeRoutine()}, LStretch{true}, LMargins{false}),
        F::gen(L::HB(), {m_p->addRoutine(), m_p->addRoutineIter()}, LStretch{true}, LMargins{false}),
        F::gen(L::HB(), {m_p->addCondition(), m_p->addConditionIter()}, LStretch{true}, LMargins{false}),
        F::gen(L::HB(), {m_p->addFrameId()}, LStretch{true}, LMargins{false}),
        },LStretch{false}, LMargins{true}, QFrame::Box)
    );
}

void LoggerExperimentInitConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_p->resource.init_widget(Resource::Type::Directory, "Directory to use:"));
    add_input_ui(m_p->addCurrentInstanceToFileName.init_widget("Add current instance to name", true));
    add_input_ui(m_p->addDateToFileName.init_widget("Add current date to name", true));
    add_input_ui(m_p->dontWriteIfFileExists.init_widget("Dont write if file already exists", false));

    add_inputs_ui(
        ExRadioButtonW::init_group_widgets(
            m_p->group1,
            {
                &m_p->replaceIfFileExists,
                &m_p->addToEndIfFileExists
            },
            {
                "Replace file if exists",
                "Add to end of file if exists",
            },
            {
                true, false
            }
        )
    );

    add_input_ui(m_p->baseFileName.init_widget("log"));
    add_input_ui(m_p->dateTimeFormat.init_widget("yyyy-MM-dd_H-mm-ss"));
    add_input_ui(m_p->fileExtension.init_widget("txt"));
    add_input_ui(m_p->addHeaderLine.init_widget("Add header line", true));
    add_input_ui(m_p->separator.init_widget(";"));
    add_input_ui(m_p->eachFrame.init_widget("Write at each frame (if false write when routine starts)", false));
    add_input_ui(m_p->addTimeExp.init_widget("time exp", true));
    add_input_ui(m_p->addTimeRoutine.init_widget("time routine", false));
    add_input_ui(m_p->addRoutine.init_widget("routine name", true));
    add_input_ui(m_p->addRoutineIter.init_widget("routine iteration", true));
    add_input_ui(m_p->addCondition.init_widget("condition name", true));
    add_input_ui(m_p->addConditionIter.init_widget("condition iteration", true));
    add_input_ui(m_p->addFrameId.init_widget("frame id", false));
}

void LoggerExperimentInitConfigParametersW::create_connections(){
    connect(m_p->addDateToFileName(), &QCheckBox::clicked, this, [&](bool checked){
        m_p->dateTimeFormat.w->setEnabled(checked);
    });
}

void LoggerExperimentInitConfigParametersW::late_update_ui(){
    m_p->dateTimeFormat.w->setEnabled(m_p->addDateToFileName.w->isChecked());
}

struct GlobalLoggerInitConfigParametersW::Impl{

    QWidget *mainW = nullptr;
    QTabWidget *mainTab = nullptr;

    QWidget *inputW = nullptr;
    ExComponentsListW inputsComponents{"inputs_components"};

    QWidget *networkW = nullptr;
    ExComponentsListW networkComponents{"network_components"};

    QWidget *uiW = nullptr;
    ExComponentsListW uiComponents{"ui_components"};

    QWidget *trackingW = nullptr;
    ExComponentsListW trackingComponents{"tracking_components"};

    // directories
    ExResourceW resource{"directory"};
    ExCheckBoxW addCurrentInstanceToSubDirectoryName{"add_current_instance_to_sub_directory_name"};
    ExCheckBoxW addDateToSubDirectoryName{"add_date_to_sub_directory_name"};
    ExLineEditW baseSubDirectoryName{"base_sub_directory_name"};
    ExLineEditW dateTimeFormat{"date_time_format"};
    ExLineEditW fileExtension{"file_extension"};

    // # experience
    ExCheckBoxW expEllapsedExpTime{"ellapsed_exp_time"};
    ExCheckBoxW expFrameId{"frame_id"};
    ExCheckBoxW expStartFrameTime{"start_frame_time"};    
    // # element
    ExCheckBoxW expEllapsedElementTime{"ellapsed_element_time"};
    ExCheckBoxW expElementOrder{"element_order"};
    ExCheckBoxW expRoutineName{"routine_name"};
    ExCheckBoxW expElementIteration{"element_iteration"};
    // # condition
    ExCheckBoxW expConditionName{"condition_name"};
    ExCheckBoxW expConditionNbCalls{"condition_nb_calls"};
    ExCheckBoxW expActionsCount{"actions_count"};
    ExCheckBoxW expConnectorsCount{"connectors_count"};
    ExCheckBoxW expConnectionsCount{"connections_count"};
    ExCheckBoxW expConditionDuration{"condition_duration"};
    // # camera
    ExCheckBoxW expEyesCamera{"eyes_camera"};
    ExCheckBoxW expCalibration{"camera_calibration"};
    ExCheckBoxW expCameraRig{"camera_rig"};
    // # misc
    ExCheckBoxW expFramerate{"framerate"};
};

GlobalLoggerInitConfigParametersW::GlobalLoggerInitConfigParametersW() :  ConfigParametersW(), m_p(std::make_unique<Impl>()){
}

void GlobalLoggerInitConfigParametersW::insert_widgets(){

    add_widget(F::gen(L::VB(),{m_p->resource()}, LStretch{false}));

    auto l1 = F::gen(L::HB(), {W::txt("Global logging directory name:"), m_p->baseSubDirectoryName()}, LStretch{false}, LMargins{false});
    auto l2 = F::gen(L::HB(), {m_p->addCurrentInstanceToSubDirectoryName()}, LStretch{true}, LMargins{false});
    auto l3 = F::gen(L::HB(), {m_p->addDateToSubDirectoryName()}, LStretch{true}, LMargins{false});
    auto l4 = F::gen(L::HB(), {W::txt("Date format:"),    m_p->dateTimeFormat()},  LStretch{true}, LMargins{false});
    auto l5 = F::gen(L::HB(), {W::txt("File extension:"), m_p->fileExtension()}, LStretch{true}, LMargins{false});
    add_widget(F::gen(L::VB(),
        {l1,l2,l3,l4,l5},
        LStretch{false}, LMargins{true}, QFrame::Box)
    );

    auto tw = new QTabWidget();
    tw->addTab(m_p->mainW = new QWidget(), "Exp");
    tw->addTab(m_p->inputW = new QWidget(), "Input device");
    tw->addTab(m_p->networkW = new QWidget(), "Network");
    tw->addTab(m_p->uiW = new QWidget(), "User interface");
    tw->addTab(m_p->trackingW = new QWidget(), "Acquisition device");

    auto l = ui::L::VB();
    m_p->mainW->setLayout(l);
    l->addWidget(F::gen(L::VB(),{ui::W::txt("<b>Data to log</b>")}, LStretch{false}));
    l->addWidget(F::gen(L::VB(),{ui::W::txt("Experience:"), F::gen(L::VB(),{m_p->expEllapsedExpTime(), m_p->expFrameId(), m_p->expStartFrameTime()},
            LStretch{false})}, LStretch{false}, LMargins{false}));
    l->addWidget(F::gen(L::VB(),{ui::W::txt("Routine:"), F::gen(L::VB(),{m_p->expEllapsedElementTime(), m_p->expElementOrder(),
            m_p->expRoutineName(), m_p->expElementIteration()},LStretch{false})}, LStretch{false}, LMargins{false}));
    l->addWidget(F::gen(L::VB(),{ui::W::txt("Condition:"),F::gen(L::VB(),{m_p->expConditionName(), m_p->expConditionNbCalls(),
            m_p->expActionsCount(), m_p->expConnectorsCount(), m_p->expConnectionsCount(), m_p->expConditionDuration()},LStretch{false})}, LStretch{false}, LMargins{false}));
    l->addWidget(F::gen(L::VB(),{ui::W::txt("Cameras (position,rotation):"), F::gen(L::VB(),{m_p->expEyesCamera(), m_p->expCalibration(), m_p->expCameraRig()},LStretch{false})}, LStretch{false}, LMargins{false}));
    l->addWidget(F::gen(L::VB(),{ui::W::txt("Misc:"), F::gen(L::VB(),{m_p->expFramerate()},LStretch{false})}, LStretch{false}, LMargins{false}));
    l->addStretch(0);

    l = ui::L::VB();
    m_p->inputW->setLayout(l);
    l->addWidget(F::gen(L::VB(),{m_p->inputsComponents()}, LStretch{false}));

    l = ui::L::VB();
    m_p->networkW->setLayout(l);
    l->addWidget(F::gen(L::VB(),{m_p->networkComponents()}, LStretch{false}));

    l = ui::L::VB();
    m_p->uiW->setLayout(l);
    l->addWidget(F::gen(L::VB(),{m_p->uiComponents()}, LStretch{false}));

    l = ui::L::VB();
    m_p->trackingW->setLayout(l);
    l->addWidget(F::gen(L::VB(),{m_p->trackingComponents()}, LStretch{false}));

    add_widget(F::gen(L::VB(),{tw}, LStretch{false}, LMargins{false}));

    no_end_stretch();
}

void GlobalLoggerInitConfigParametersW::init_and_register_widgets(){

    // directories/files
    add_input_ui(m_p->inputsComponents.init_widget(Component::Category::Input, true, "Input device components to log"));
    add_input_ui(m_p->networkComponents.init_widget(Component::Category::Network, true, "Network components to log"));
    add_input_ui(m_p->uiComponents.init_widget(Component::Category::UI, true, "User interface components to log"));
    add_input_ui(m_p->trackingComponents.init_widget(Component::Category::Acquisition, true, "Acquisition device components to log"));
    add_input_ui(m_p->resource.init_widget(Resource::Type::Directory, "Directory to use:"));
    add_input_ui(m_p->addCurrentInstanceToSubDirectoryName.init_widget("Add current instance to directory name", true));
    add_input_ui(m_p->addDateToSubDirectoryName.init_widget("Add current date to directory name", true));
    add_input_ui(m_p->baseSubDirectoryName.init_widget("global_log"));
    add_input_ui(m_p->dateTimeFormat.init_widget("yyyy-MM-dd_H-mm-ss"));
    add_input_ui(m_p->fileExtension.init_widget("csv"));

    // # experience
    add_input_ui(m_p->expEllapsedExpTime.init_widget("Ellapsed time (ms) since starting of the experiment", true));
    add_input_ui(m_p->expFrameId.init_widget("Frame id", true));
    add_input_ui(m_p->expStartFrameTime.init_widget("Start frame time (ms)", true));
    // # routine
    add_input_ui(m_p->expEllapsedElementTime.init_widget("Ellasped time (ms) since starting of the routine", true));
    add_input_ui(m_p->expElementOrder.init_widget("Order id", true));
    add_input_ui(m_p->expRoutineName.init_widget("Name", true));
    add_input_ui(m_p->expElementIteration.init_widget("Iteration", true));
    // # condition
    add_input_ui(m_p->expConditionName.init_widget("Name", true));
    add_input_ui(m_p->expConditionNbCalls.init_widget("Nb calls", true));
    add_input_ui(m_p->expActionsCount.init_widget("Nb actions", true));
    add_input_ui(m_p->expConnectorsCount.init_widget("Nb connectors", true));
    add_input_ui(m_p->expConnectionsCount.init_widget("Nb connections", true));
    add_input_ui(m_p->expConditionDuration.init_widget("Duration", true));
    // # camera
    add_input_ui(m_p->expEyesCamera.init_widget("Eyes", true));
    add_input_ui(m_p->expCalibration.init_widget("Calibration", true));
    add_input_ui(m_p->expCameraRig.init_widget("Camera rig", true));
    // # misc
    add_input_ui(m_p->expFramerate.init_widget("Framerate (average on last second)", true));

}
