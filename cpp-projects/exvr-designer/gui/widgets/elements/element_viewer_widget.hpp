
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
#include <QWidget>
#include <QToolBox>

// local
#include "flow_diagram_widget.hpp"

// # ui
#include "ui_isi.h"
#include "ui_loop.h"
#include "ui_routine.h"

// ui
namespace Ui {
    class RoutineW;
    class IsiW;
    class LoopW;
}

namespace tool::ex {


class ElementViewerW;
using ElementDisplayWUP = std::unique_ptr<ElementViewerW>;

class ElementViewerW : public QWidget{

    Q_OBJECT

public :

    ElementViewerW(QTabWidget *twCurrentElement);
    void reset();

public slots:

    void update_from_current_element(FlowElement *elem);
    void update_set_name(RowId id);
    void update_set_occurencies(RowId id);

private:

    void init_no_selection_ui();
    void init_routine_ui();
    void init_loop_ui();
    void init_isi_ui();

    void update_no_selection_ui();
    void update_loop_ui(Loop *loop);
    void update_routine_ui(Routine *routine);
    void update_isi_ui(Isi *isi);

    // current loop style index
    int m_currentLoopIndex = 0;


    // layout
    QHBoxLayout *m_layout = nullptr;

    // ui    
    std::unique_ptr<Ui::RoutineW> m_routineUI   = nullptr;
    std::unique_ptr<Ui::LoopW>    m_loopUI      = nullptr;
    std::unique_ptr<Ui::IsiW>     m_isiUI       = nullptr;

    // widgets
    QWidget m_noSelectionW;
    QTableWidget *m_noSelectionTW = nullptr;
    QLabel *m_allElementLa = nullptr;    
//    QListWidget *m_allElementsLW = nullptr;
    QWidget m_routineW;
    QWidget m_loopW;
    QWidget m_isiW;
    QTabWidget *m_toolBox = nullptr;




    ElementKey m_currentElementId = {-1};

};
}
