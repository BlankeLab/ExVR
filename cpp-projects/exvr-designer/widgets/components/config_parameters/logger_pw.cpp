
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
#include "ex_widgets/ex_checkbox_w.hpp"
#include "ex_widgets/ex_radio_button_w.hpp"
#include "ex_widgets/ex_line_edit_w.hpp"

// local
#include "ex_widgets/ex_resource_w.hpp"

using namespace tool::ex;

struct LoggerInitConfigParametersW::Impl{

    ExResourceW resource{"directory"};

    ExCheckBoxW streamToFile{"stream_to_file"};
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

    add_widget(ui::F::gen(ui::L::VB(),{m_p->resource()}, LStretch{false}));
    add_widget(ui::F::gen(ui::L::VB(),
        {m_p->streamToFile(), m_p->dontWriteIfFileExists(), m_p->insertNewRoutineInfo(), m_p->replaceIfFileExists(), m_p->addToEndIfFileExists()},
        LStretch{false}, LMargins{true}, QFrame::Box)
    );

    auto l1 = ui::F::gen(ui::L::HB(), {ui::W::txt("Logging file name:"), m_p->baseFileName()}, LStretch{false}, LMargins{false});
    auto l2 = ui::F::gen(ui::L::HB(), {m_p->addCurrentInstanceToFileName()}, LStretch{true}, LMargins{false});
    auto l3 = ui::F::gen(ui::L::HB(), {m_p->addDateToFileName()}, LStretch{true}, LMargins{false});
    auto l4 = ui::F::gen(ui::L::HB(), {ui::W::txt("Date format:"),    m_p->dateTimeFormat()},  LStretch{true}, LMargins{false});
    auto l5 = ui::F::gen(ui::L::HB(), {ui::W::txt("File extension:"), m_p->fileExtension()}, LStretch{true}, LMargins{false});
    auto l6 = ui::F::gen(ui::L::HB(), {m_p->addHeaderLine()}, LStretch{true}, LMargins{false});
    auto l7 = ui::F::gen(ui::L::HB(), {ui::W::txt("Header line:"), m_p->headerLine()}, LStretch{false}, LMargins{false});

    add_widget(ui::F::gen(ui::L::VB(),
        {l1,l2,l3,l4,l5,l6,l7},
        LStretch{false}, LMargins{true}, QFrame::Box)
    );
}

void LoggerInitConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_p->resource.init_widget(Resource::Type::Directory, "Directory to use:"));
    add_input_ui(m_p->streamToFile.init_widget("Stream to file", true));
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

    ExCheckBoxW streamToFile{"stream_to_file"};
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

    add_widget(ui::F::gen(ui::L::VB(),{m_p->resource()}, LStretch{false}));
    add_widget(ui::F::gen(ui::L::VB(),
        {m_p->streamToFile(), m_p->dontWriteIfFileExists(), m_p->replaceIfFileExists(), m_p->addToEndIfFileExists(), m_p->writeAtEnfOfEachFrame()},
        LStretch{false}, LMargins{true}, QFrame::Box)
    );

    auto l1 = ui::F::gen(ui::L::HB(), {ui::W::txt("Logging file name:"), m_p->baseFileName()}, LStretch{false}, LMargins{false});
    auto l2 = ui::F::gen(ui::L::HB(), {m_p->addCurrentInstanceToFileName()}, LStretch{true}, LMargins{false});
    auto l3 = ui::F::gen(ui::L::HB(), {m_p->addDateToFileName()}, LStretch{true}, LMargins{false});
    auto l4 = ui::F::gen(ui::L::HB(), {ui::W::txt("Date format:"),    m_p->dateTimeFormat()},  LStretch{true}, LMargins{false});
    auto l5 = ui::F::gen(ui::L::HB(), {ui::W::txt("File extension:"), m_p->fileExtension()}, LStretch{true}, LMargins{false});
    auto l6 = ui::F::gen(ui::L::HB(), {ui::W::txt("Columns separator:"), m_p->separator()}, LStretch{false}, LMargins{false});
    auto l7 = ui::F::gen(ui::L::HB(), {m_p->addHeaderLine()}, LStretch{true}, LMargins{false});
    auto l8 = ui::F::gen(ui::L::HB(), {ui::W::txt("Header line:"), m_p->headerLine()}, LStretch{false}, LMargins{false});

    add_widget(ui::F::gen(ui::L::VB(),
        {l1,l2,l3,l4,l5,l6,l7,l8},
        LStretch{false}, LMargins{true}, QFrame::Box)
    );
}

void LoggerColumnsInitConfigParametersW::init_and_register_widgets(){

    add_input_ui(m_p->resource.init_widget(Resource::Type::Directory, "Directory to use:"));
    add_input_ui(m_p->streamToFile.init_widget("Stream to file", true));
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

    ExCheckBoxW streamToFile{"stream_to_file"};
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

    add_widget(ui::F::gen(ui::L::VB(),{m_p->resource()}, LStretch{false}));
    add_widget(ui::F::gen(ui::L::VB(),
        {m_p->streamToFile(), m_p->dontWriteIfFileExists(), m_p->replaceIfFileExists(), m_p->addToEndIfFileExists()},
        LStretch{false}, LMargins{true}, QFrame::Box)
    );

    auto l1 = ui::F::gen(ui::L::HB(), {ui::W::txt("Logging file base name:"), m_p->baseFileName()}, LStretch{false}, LMargins{false});
    auto l2 = ui::F::gen(ui::L::VB(), {m_p->addRoutineName(), m_p->addConditionName(), m_p->addBoth(), m_p->addCurrentInstanceToFileName()}, LStretch{false}, LMargins{false});
    auto l3 = ui::F::gen(ui::L::HB(), {ui::W::txt("File extension:"), m_p->fileExtension()}, LStretch{true}, LMargins{false});
    auto l4 = ui::F::gen(ui::L::HB(), {m_p->addHeaderLine()}, LStretch{true}, LMargins{false});
    auto l5 = ui::F::gen(ui::L::HB(), {ui::W::txt("Header line:"), m_p->headerLine()}, LStretch{false}, LMargins{false});

    add_widget(ui::F::gen(ui::L::VB(),
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
    add_input_ui(m_p->streamToFile.init_widget("Stream to file", true));

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

