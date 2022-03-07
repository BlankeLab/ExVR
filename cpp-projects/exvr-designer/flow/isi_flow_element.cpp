
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


#include "isi_flow_element.hpp"


using namespace tool::ex;

IsiFlowElement::IsiFlowElement(Isi *isi) : MovableFlowElement(isi){
}

void IsiFlowElement::adapt_size_from_name(QFontMetrics fontMetrics){
    MovableFlowElement::adapt_size_from_name(fontMetrics);
}

void IsiFlowElement::compute_position(QPointF topLeft, int loopMaxDeepLevel){
    MovableFlowElement::compute_position(topLeft, loopMaxDeepLevel);
}

void IsiFlowElement::draw(QPainter &painter, qreal zoomLevel){

    MovableFlowElement::draw(painter, zoomLevel);

    // draw rectangle
    QPen pen;
    pen.setWidthF(zoomLevel*1.2);
    pen.setColor(is_selected() ? colors.selectedLineBoxColor : colors.unselectedLineBoxColor);
    painter.setPen(pen);
    painter.setBrush(is_selected() ? colors.selectedFillBoxColor : colors.unselectedFillBoxColor);
    painter.drawRoundedRect(uiElemRect, zoomLevel*4.,zoomLevel*4., Qt::AbsoluteSize);

    // draw name
    pen.setColor(is_selected() ? colors.selectedTextColor : colors.unselectedTextColor);    
    painter.setPen(pen);
    painter.drawText(uiElemRect,  Qt::AlignCenter, name);
}

