
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


// std
#include <memory>

// Qt
#include <QTabWidget>

// local
#include "routine_tab_widget.hpp"
#include "experiment/experiment.hpp"

namespace tool::ex{

class RoutinesManagerTW;
using RoutinesManagerTWUP = std::unique_ptr<RoutinesManagerTW>;


class RoutinesManagerTW : public QTabWidget{

    Q_OBJECT

public :

    RoutinesManagerTW();

    void close_all_windows();

    RoutineTabW* current_routine_widget();
    RoutineTabW *routine_widget(RowId tabId);

public slots:

    void update_from_experiment(Experiment *exp);
    void reset();
    void update_connector_dialog_with_info(ElementKey elementKey, ConditionKey conditionKey, ConnectorKey connectorKey, QStringView id, QStringView value);
};
}
