
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
#include <QFrame>
#include <QComboBox>

// qt-utility
#include "gui/ex_widgets/ex_item_w.hpp"

// local
#include "data/flow_elements/routine.hpp"

namespace tool::ex{


class ExConditionW : public ExItemW<QFrame>{

    Q_OBJECT
public:

    ExConditionW(QString name = "");
    ExConditionW *init_widget(bool enabled = true);

    void update_from_arg(const Arg &arg) override;
    Arg convert_to_arg() const override;
    void update_from_components() override;

    QString display()const;

private:

    void update_routines_list_widget();
    void update_conditions_list_widget();

    QLabel *m_routineTitle = nullptr;
    QLabel *m_conditionTitle = nullptr;
    QComboBox *m_routineNames = nullptr;
    QComboBox *m_conditionNames = nullptr;

    ElementKey m_currentRoutineKey = {-1};
    ConditionKey m_currentConditionKey = {-1};
};

}
