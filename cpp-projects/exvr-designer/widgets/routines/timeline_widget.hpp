
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
#include <QTimer>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

// base
#include "utility/vector.hpp"

#include "data/id_key.hpp"

// local
#include "data/timeline.hpp"

namespace tool::ex{

class TimelineW : public QWidget{
public :

    TimelineW(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey, Timeline *timeline, bool drawAxe, bool updateTimeline);

    QRectF interval_to_rect(const Interval &i)const;
    Interval rect_to_interval(const QRectF &r) const;

    void update_from_timeline(Timeline *timeline, qreal m_scale, qreal m_factorSize, SecondsTS max);

    constexpr ElementKey routine_key() const noexcept{return m_routineKey;}
    constexpr ConditionKey condition_key() const noexcept{return m_conditionKey;}
    constexpr ActionKey action_key() const noexcept{return m_actionKey;}
    constexpr TimelineKey timeline_key() const noexcept{return m_timelineKey;}

protected:

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

public :

    Timeline::Type type;

private:

    ElementKey m_routineKey;
    ConditionKey m_conditionKey;
    ActionKey m_actionKey;
    TimelineKey m_timelineKey;

    bool m_drawAxe = false;
    bool m_updateTimeline = false;
    bool m_clickOn = false;

    Qt::MouseButton m_currentButtonPressed;
    QPoint m_currentMousePos;

    static constexpr qreal m_widthOffset = 50.;
    static constexpr qreal m_widthPart = 100;

    qreal m_scale = 1.0;
    qreal m_factorSize = 1.0;
    qreal m_widthTotal;
    qreal m_timelineWidth;
    SecondsTS m_maxLength{10.};   

    QRectF timelineR;
    QRectF leftTextPartR;
    QRectF rightPartR;
    QRectF rightTopIntervalsR;
    QRectF rightBottomAxisR;

    std_v1<QRectF> m_elementsPartsAreas;
    std_v1<QRectF> m_intervalsAreas;
    static constexpr QColor backgroundCol = QColor(255,255,255);
    static constexpr QColor invalidIntervals = QColor(255,0,0);
    static constexpr QColor intervalAreaCol = QColor(81,219,51);
    static constexpr QColor elementAreaCol = QColor(50,50,50,100);
    static constexpr QColor lineCol = QColor(255,255,255);
    static constexpr QColor textCol = QColor(0,0,0);
};
}

