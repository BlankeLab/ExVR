
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
#include <QPainter>
#include <QMouseEvent>

// local
// # data
#include "data/action.hpp"
// # widgets
#include "ui_action.h"
#include "timeline_widget.hpp"

namespace tool::ex{

class ActionW : public QWidget{

    Q_OBJECT

public :

    ActionW(ElementKey routineKey, ConditionKey conditionKey, Action *action);

    void update_from_action(int id, Action *action, qreal scale, qreal factorSize, SecondsTS max);

    constexpr ElementKey routine_key() const noexcept{return m_routineKey;}
    constexpr ConditionKey condition_key() const noexcept{return m_conditionKey;}
    constexpr ActionKey action_key() const noexcept{return m_actionKey;}

protected:

    void mousePressEvent(QMouseEvent *event) override;

public slots:

    void set_minimized_state(bool state);

private:

    ElementKey m_routineKey;
    ConditionKey m_conditionKey;
    ActionKey m_actionKey;

    // ui
    Ui::ActionW m_ui;
    bool m_minimized = false;

    // widgets
    TimelineW *m_timelineUpdateW     = nullptr;
    TimelineW *m_timelineVisibilityW = nullptr;    
};
}

