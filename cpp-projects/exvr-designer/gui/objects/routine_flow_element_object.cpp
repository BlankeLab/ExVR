
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

#include "routine_flow_element_object.hpp"

using namespace tool::ex;

RoutineFlowElementO::RoutineFlowElementO(Routine *routine) : MovableFlowElementO(routine){
    isARandomizer = routine->isARandomizer;
}

void RoutineFlowElementO::adapt_size_from_name(QFontMetrics fontMetrics){
    MovableFlowElementO::adapt_size_from_name(fontMetrics);
}

void RoutineFlowElementO::compute_position(QPointF topLeft, int loopMaxDeepLevel){
    MovableFlowElementO::compute_position(topLeft, loopMaxDeepLevel);
}


void RoutineFlowElementO::draw(QPainter &painter, qreal zoomLevel){

    MovableFlowElementO::draw(painter, zoomLevel);

    // draw rectangle
    QPen pen;
    pen.setWidthF(zoomLevel*1.1);
    if(!isARandomizer){
        pen.setColor(display::Colors::line_box(is_selected(), type));
    }else{
        pen.setColor(is_selected() ? QColor(140,165,206) : QColor("#000000"));
    }

    painter.setPen(pen);

    if(!isARandomizer){
        painter.setBrush(display::Colors::fill_box(is_selected(), type));
    }else{
        painter.setBrush(QColor(140,165,206));
    }
    painter.drawRoundedRect(uiElemRect, zoomLevel*4.,zoomLevel*4., Qt::AbsoluteSize);

    // draw name
    pen.setColor(display::Colors::text(is_selected(), type));
    painter.setPen(pen);
    painter.drawText(uiElemRect,  Qt::AlignCenter, name);

//    pen.setColor(Qt::red);
//    pen.setWidthF(zoomLevel*50.0);
//    painter.setPen(pen);
//    painter.drawText(QRect(uiAreaRect.left()),  Qt::AlignCenter, "12");
}

void RoutineFlowElementO::update(FlowElement *element){
    MovableFlowElementO::update(element);
    isARandomizer = dynamic_cast<Routine*>(element)->isARandomizer;
}
