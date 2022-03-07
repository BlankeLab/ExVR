
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

#include "timeline_widget.hpp"

// local
#include "experiment/global_signals.hpp"

using namespace tool;
using namespace tool::ex;

TimelineW::TimelineW(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey, Timeline *timeline, bool drawAxe, bool updateTimeline) :
      type(timeline->type), m_routineKey(routineKey), m_conditionKey(conditionKey), m_actionKey(actionKey), m_timelineKey(TimelineKey{timeline->key()}), m_drawAxe(drawAxe), m_updateTimeline(updateTimeline){

    setMouseTracking(true);
    setObjectName(QSL("timeline"));
    setStyleSheet(QSL("QWidget[objectName=\"timeline\"] {background-color:white;}"));
}

QRectF TimelineW::interval_to_rect(const Interval &i) const{

    const qreal startX = (i.start.v/m_maxLength.v) * rightTopIntervalsR.width();
    const qreal endX   = (i.end.v/m_maxLength.v)   * rightTopIntervalsR.width();
    return QRectF{
        rightTopIntervalsR.x() + startX,
        rightTopIntervalsR.y(),
        endX-startX,
        rightTopIntervalsR.height()
    };
}

Interval TimelineW::rect_to_interval(const QRectF &r) const{
    return Interval{
        SecondsTS{m_maxLength.v*(r.x()-m_widthOffset)/m_timelineWidth},
        SecondsTS{m_maxLength.v*(r.x()+r.width()-m_widthOffset)/m_timelineWidth},
        IntervalKey{-1}
    };
}

void TimelineW::update_from_timeline(Timeline *timeline, qreal scale, qreal factorSize, SecondsTS max){

    m_scale      = scale;
    m_factorSize = factorSize;
    m_maxLength  = max;

    m_timelineWidth = m_factorSize*m_widthPart*m_maxLength.v/m_scale;
    setMinimumWidth(std::round(m_timelineWidth+m_widthOffset));
    setMaximumWidth(std::round(m_timelineWidth+m_widthOffset));
    setMinimumHeight(25);
    setMaximumHeight(25);

    timelineR = rect();

    leftTextPartR = QRectF{
        timelineR.topLeft(),
        QSizeF{m_widthOffset, timelineR.height()}
    };

    rightPartR = QRectF{
        leftTextPartR.topRight(),
        QSizeF{timelineR.width()-leftTextPartR.width(), timelineR.height()}
    };

    rightTopIntervalsR = QRectF{
        rightPartR.topLeft(),
        QSizeF{rightPartR.width(), rightPartR.height() * 2./3.}
    };

    rightBottomAxisR = QRectF{
        rightPartR.topLeft() +
        QPointF{0, rightTopIntervalsR.height()},
        QSizeF{rightPartR.width(), rightPartR.height() / 3.}
    };


    if(timeline){
        m_intervalsAreas.resize(timeline->intervals.size());
        for(size_t ii = 0; ii < timeline->intervals.size(); ++ii){
            m_intervalsAreas[ii] = interval_to_rect(timeline->intervals[ii]);
        }
    }

    size_t nbParts = std::round(m_maxLength.v/m_scale);
    m_elementsPartsAreas.resize(nbParts);
    for(size_t ii = 0; ii < nbParts; ++ii){
        m_elementsPartsAreas[ii] = QRectF{
            rightTopIntervalsR.x() + ii* m_factorSize*m_widthPart,
            rightTopIntervalsR.y(),
            m_factorSize*m_widthPart,
            rightTopIntervalsR.height()
        };
    }

    update();

}

void TimelineW::paintEvent(QPaintEvent *event){

    Q_UNUSED(event)


    QPainter p(this);

    // fill whole widget with white
    p.fillRect(timelineR,  backgroundCol);

    // fill intervals with red
    p.fillRect(rightTopIntervalsR,  invalidIntervals);

    // debug
    // p.fillRect(rightBottomAxisR,  Qt::yellow);

    // draw left text
    p.drawText(leftTextPartR, (type == Timeline::Type::Visibility ? QSL("Visibility") : QSL("Update")));


    // draw intervals
    for(auto &part : m_intervalsAreas){
        p.fillRect(part,intervalAreaCol);
        // debug
        // p.setPen(Qt::blue);
        // p.drawLine(part.topLeft(),part.bottomRight());
        // p.setPen(Qt::black);
    }

    for(auto &part : m_elementsPartsAreas){
        if(part.contains(m_currentMousePos)){
            p.fillRect(part, elementAreaCol);
        }
    }

    if(m_drawAxe){
        p.setPen(textCol);
        p.drawText(rightBottomAxisR.bottomLeft(), QSL("0"));
    }

    for(size_t ii = 0; ii < m_elementsPartsAreas.size(); ++ii){
        const auto &part = m_elementsPartsAreas[ii];
        if(ii < m_elementsPartsAreas.size()-1){
            p.setPen(lineCol);
            p.drawLine(QLineF{part.topRight(),part.bottomRight()});
        }
        if(m_drawAxe){
            p.setPen(textCol);
            p.drawText(QPointF{
                part.x() + m_elementsPartsAreas[0].width(), rightBottomAxisR.bottomLeft().y()},
                QString::number(m_maxLength.v*(part.bottomRight().x()-m_widthOffset)/m_timelineWidth)
            );
        }
    }    
}

void TimelineW::mousePressEvent(QMouseEvent *event){
    m_clickOn = true;
    m_currentButtonPressed = event->button();
    mouseMoveEvent(event);
}

void TimelineW::mouseReleaseEvent(QMouseEvent *event){
    Q_UNUSED(event)
    m_clickOn = false;
}

void TimelineW::mouseMoveEvent(QMouseEvent *event){

    m_currentMousePos = event->pos();
    if(m_clickOn){
        for(const auto &element : m_elementsPartsAreas){
            if(QRectF(element.x(), element.y(), element.width(), element.height() ).contains(m_currentMousePos)){
                if(m_currentButtonPressed == Qt::MouseButton::LeftButton){
                    emit GSignals::get()->add_interval_signal(routine_key(), condition_key(), action_key(), m_updateTimeline, timeline_key(), rect_to_interval(element));
                }else if(m_currentButtonPressed == Qt::MouseButton::RightButton){
                    emit GSignals::get()->remove_interval_signal(routine_key(), condition_key(), action_key(), m_updateTimeline, timeline_key(), rect_to_interval(element));
                }
                return;
            }
        }
    }else{
        update();
    }
}

void TimelineW::leaveEvent(QEvent *event){
    Q_UNUSED(event)
    m_currentMousePos = QPoint(-1,-1);
    m_clickOn = false;
    update();
}
