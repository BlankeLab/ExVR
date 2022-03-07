
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

// local
#include "sub_parts_pw.hpp"
#include "ex_widgets/ex_parameters_generator_w.hpp"

namespace tool::ex {

class ConfigParametersW : public QWidget{

    Q_OBJECT

public :

    ConfigParametersW();
    virtual ~ConfigParametersW(){}

    void set_infos(ComponentKey componentKey, ConfigKey configKey, bool initConfig);

    virtual void update_with_info(QStringView id, QStringView value){ Q_UNUSED(id) Q_UNUSED(value)}
    virtual void insert_widgets(){}
    virtual void init_and_register_widgets(){}
    virtual void create_connections(){}
    virtual void late_update_ui(){}

    // # stretch
    void no_end_stretch();
    void end_stretch();
    void set_stretch(int id, int value);
    // # widgets
    void add_widget(QWidget *w);
    void add_horizontal_widgets(std::vector<QWidget*> widgets,
        LMargins margins = LMargins{false}, QFrame::Shape shape = QFrame::NoFrame, QFrame::Shadow shadow = QFrame::Raised);
    void add_stretched_horizontal_widgets(std::vector<QWidget*> widgets,
        LMargins margins = LMargins{false}, QFrame::Shape shape = QFrame::NoFrame, QFrame::Shadow shadow = QFrame::Raised);
    void add_vertical_widgets(std::vector<QWidget*> widgets,
        LMargins margins = LMargins{false}, QFrame::Shape shape = QFrame::NoFrame, QFrame::Shadow shadow = QFrame::Raised);
    void add_stretched_vertical_widgets(std::vector<QWidget*> widgets,
        LMargins margins = LMargins{false}, QFrame::Shape shape = QFrame::NoFrame, QFrame::Shadow shadow = QFrame::Raised);

    void add_sub_part_widget(ConfigParametersSubPart &subPart);
    void map_sub_part(ConfigParametersSubPart *subPart);

    // # update
    void init_from_args(std::map<QStringView,Arg> &args);
    void update_from_resources();
    void update_from_components();
    void reset_args();

    void add_input_ui(ExBaseW *w);
    void add_inputs_ui(std::vector<ExBaseW*> widgets);

    void add_action_ui(ExBaseW *w);
    void add_non_ui_arg(Arg arg);
    void add_generator_ui(ExBaseW *g);

    inline std::unordered_map<QStringView, ExBaseW*> *input_ui_widgets(){
        return &m_inputUiElements;
    }

public:

    bool initConfig;
    ComponentKey componentKey;
    ConfigKey configKey;

protected:

    QVBoxLayout *m_layout = nullptr;

private:

    std::unordered_map<QStringView, ExBaseW*> m_inputUiElements;
    std::unordered_map<QStringView, ExBaseW*> m_actionUiElements;
    std::unordered_map<QStringView, Arg> m_inputNonUiArguments;
    std::unordered_map<QStringView, ExParametersGeneratorWidgetW*> m_generatorsUiElements;


    bool endStretch = true;
};
}
