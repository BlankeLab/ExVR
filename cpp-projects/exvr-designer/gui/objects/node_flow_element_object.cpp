
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

#include "node_flow_element_object.hpp"


using namespace tool::ex;

NodeFlowElementO::NodeFlowElementO(NodeFlow *node) : FlowElementO(node){
    addRoutine = std::make_unique<AddButtonO>(FlowElement::Type::Routine);
//    addIsi     = std::make_unique<AddButtonO>(FlowElement::Type::Isi);
    addLoop    = std::make_unique<AddButtonO>(FlowElement::Type::Loop);
}

void NodeFlowElementO::adapt_size_from_name(QFontMetrics fontMetrics){

    QRect rect = fontMetrics.boundingRect(name);
    qreal minAreaWidth = 0.4*rect.width();
    qreal maxAreaWidth = 0.8*rect.width();
    qreal areaWidth = minAreaWidth + areaStretch*(maxAreaWidth - minAreaWidth);

    qreal min = std::min(0.6*areaWidth, 0.8*areaHeight);
    uiElemRect = QRectF(QPointF(0.,0.), QSizeF(min, min));
    uiAreaRect = QRectF(QPointF(0.,0.), QSizeF(areaWidth, areaHeight));

    addRoutine->adapt_size_from_name(fontMetrics);
//    addIsi->adapt_size_from_name(fontMetrics);
    addLoop->adapt_size_from_name(fontMetrics);
}

void NodeFlowElementO::draw(QPainter &painter, qreal zoomLevel){

    // draw ellipse
    QPen pen;
    pen.setWidthF(zoomLevel*1.1);
    pen.setColor(display::Colors::line_box(is_selected(), type));
    painter.setPen(pen);
    painter.setBrush(display::Colors::fill_box(is_selected(), type));
    painter.drawEllipse(uiElemRect);

    if(is_selected()){
        draw_add_buttons(painter,zoomLevel);
    }        
}

void NodeFlowElementO::compute_position(QPointF topLeft, int loopMaxDeepLevel){

    FlowElementO::compute_position(topLeft, loopMaxDeepLevel);
    qreal heightButtons = uiAreaRect.size().height() / 2.;
    qreal heightOffset  = heightButtons/2.;
    qreal widthButton = uiAreaRect.size().width()*2.75;
    qreal posXButtons = uiElemRect.center().x()-0.5*widthButton;
    addRoutine->uiElemRect = QRectF(QPointF(posXButtons, uiAreaRect.y() + areaHeight + heightOffset), QSizeF(widthButton, heightButtons));
    addLoop->uiElemRect    = QRectF(QPointF(posXButtons, uiAreaRect.y() + areaHeight + heightButtons + 2*heightOffset), QSizeF(widthButton, heightButtons));
//    addIsi->uiElemRect     = QRectF(QPointF(posXButtons, uiAreaRect.y() + areaHeight + 2*heightButtons + 3*heightOffset), QSizeF(widthButton, heightButtons));
}

void NodeFlowElementO::draw_add_buttons(QPainter &painter, qreal zoomLevel){

    QPen pen;
    pen.setColor(display::Colors::flowElements.at(FlowElement::Type::Node).selectedLineBoxColor);
    pen.setWidthF(zoomLevel*1.1);
    painter.setPen(pen);

//    QLineF line(addIsi->uiElemRect.center(), QPointF(uiElemRect.center().x(),  uiElemRect.bottom()));
    QLineF line(addLoop->uiElemRect.center(), QPointF(uiElemRect.center().x(),  uiElemRect.bottom()));
    painter.drawLine(line);    

    pen.setColor(display::Colors::flowElements.at(FlowElement::Type::Routine).selectedLineBoxColor);
    painter.setPen(pen);
    painter.setBrush(display::Colors::flowElements.at(FlowElement::Type::Routine).selectedFillBoxColor);
    painter.drawRoundedRect(addRoutine->uiElemRect, zoomLevel*4.,zoomLevel*4., Qt::AbsoluteSize);

    pen.setColor(display::Colors::flowElements.at(FlowElement::Type::Loop).selectedLineBoxColor);
    painter.setPen(pen);
    painter.setBrush(display::Colors::flowElements.at(FlowElement::Type::Loop).selectedFillBoxColor);
    painter.drawRoundedRect(addLoop->uiElemRect, zoomLevel*4.,zoomLevel*4., Qt::AbsoluteSize);

//    pen.setColor(display::Colors::flowElements.at(FlowElement::Type::Isi).selectedLineBoxColor);
//    painter.setPen(pen);
//    painter.setBrush(display::Colors::flowElements.at(FlowElement::Type::Isi).selectedFillBoxColor);
//    painter.drawRoundedRect(addIsi->uiElemRect, zoomLevel*4.,zoomLevel*4., Qt::AbsoluteSize);

    pen.setColor(display::Colors::flowElements.at(FlowElement::Type::Routine).selectedTextColor);
    painter.setPen(pen);
    painter.drawText(addRoutine->uiElemRect,    Qt::AlignCenter, "routine");

    pen.setColor(display::Colors::flowElements.at(FlowElement::Type::Loop).selectedTextColor);
    painter.setPen(pen);
    painter.drawText(addLoop->uiElemRect,       Qt::AlignCenter, "loop");

//    pen.setColor(display::Colors::flowElements.at(FlowElement::Type::Isi).selectedTextColor);
//    painter.setPen(pen);
//    painter.drawText(addIsi->uiElemRect,        Qt::AlignCenter, "ISI");
}
