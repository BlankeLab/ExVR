
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

#include "flow_element_object.hpp"

// qt-utility
#include "qt_str.hpp"

using namespace tool::ex;



void ElementO::adapt_size_from_name(QFontMetrics fontMetrics){

    QRect rect = fontMetrics.boundingRect(name);
    qreal minAreaWidth = 1.2*rect.width();
    qreal maxAreaWidth = 1.7*rect.width();
    qreal areaWidth = minAreaWidth + areaStretch*(maxAreaWidth - minAreaWidth);
    uiElemRect = QRectF(QPointF(0.,0.), QSizeF(0.9*areaWidth, 0.8*areaHeight));
    uiAreaRect = QRectF(QPointF(0.,0.), QSizeF(areaWidth, areaHeight));
}

void ElementO::compute_position(QPointF topLeft, int loopMaxDeepLevel){

    uiAreaRect = QRectF(topLeft, uiAreaRect.size());
    QPointF midRect(topLeft.x() + uiAreaRect.width()*0.5, loopMaxDeepLevel*FlowElementO::areaHeight + uiAreaRect.height()*0.5);
    uiElemRect = QRectF(QPointF(midRect.x()-uiElemRect.width() * 0.5,midRect.y()-uiElemRect.height() * 0.5), uiElemRect.size());
}

void ElementO::draw(QPainter &painter, qreal zoomLevel){
    Q_UNUSED(painter)
    Q_UNUSED(zoomLevel)    
}

FlowElementO::FlowElementO(FlowElement *element) :
    ElementO(element->name()),
    key(ElementKey{element->key()}),
    type(element->type()),
    m_selected(element->is_selected()), m_insideLoopsID(element->insideLoopsID){
}

void FlowElementO::update(FlowElement *element){
    key             = ElementKey{element->key()};
    name            = element->name();
    type            = element->type();
    m_selected      = element->is_selected();
    informations    = element->informations;
    m_insideLoopsID = element->insideLoopsID;
}
