
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
#include "condition_widget.hpp"
#include "data/flow_elements/routine.hpp"

namespace tool::ex{

class RoutineTabW;
using TimelinesRoutineTabWUP = std::unique_ptr<RoutineTabW>;

class RoutineTabW : public QTabWidget{

    Q_OBJECT

public:

    RoutineTabW(Routine *routine);
    ConditionW *current_condition_widget();    

    void reset();
    void close_all_windows();

    void update_from_routine(GUI *gui, Routine *routine);
    void update_from_connector_info(ConditionKey conditionKey, ConnectorKey connectorKey, QStringView id, QStringView value);

    ConditionW *condition_widget(RowId tabId);

    constexpr ElementKey routine_key() const noexcept{return m_routineKey;}

private :

    ElementKey m_routineKey;
};
}
