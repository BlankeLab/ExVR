
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
#include <QDialog>

// local
#include "data/flow_elements/routine.hpp"
#include "gui/widgets/list_widget.hpp"
#include "ui_copy_to_conditions.h"

namespace tool::ex {

class CopyToConditionDialog : public QDialog{

    Q_OBJECT

public :

    CopyToConditionDialog();
    void update_from_data(ElementKey currentRoutineKey, ConditionKey currentConditionKey, std::vector<Routine*> routines);

signals:

    void copy_to_conditions_signal(
        ElementKey routineKey, ConditionKey conditionKey,
        std::vector<std::pair<ElementKey,ConditionKey>> conditionsToBeEcrased,
        bool copyActions, bool copyConnections
    );

private slots:

    void update_ui_from_conditions_checkboxes();

private:

    Ui::CopyConditionToUiD ui;
    ui::ListWidget *lw = nullptr;

    ElementKey m_currentRoutine;
    ConditionKey m_currentCondition;
    std::vector<std::pair<const Routine*,std::unique_ptr<ui::ListWidget>>> conditionsPerRoutines;    
};
}
