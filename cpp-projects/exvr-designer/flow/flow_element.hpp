
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
#include <QPainter>
#include <QFontMetrics>
#include <QGraphicsObject>

// local
#include "data/element.hpp"
#include "settings/display.hpp"

namespace tool::ex {

class FlowElement;
using FlowElementUP = std::unique_ptr<FlowElement>;


class FlowElement : public QObject{


public:

    FlowElement() = default;
    FlowElement(FlowElement&&) = delete;
    FlowElement& operator=(FlowElement&&) = delete;
    FlowElement(const FlowElement&) = delete;
    FlowElement& operator=(const FlowElement&) = delete;
    FlowElement(Element *element);

    virtual void update(Element *element);

    static void define_area_height(qreal height);
    static void define_stretch(qreal stretch);

    virtual void adapt_size_from_name(QFontMetrics fontMetrics);
    virtual void compute_position(QPointF topLeft, int loopMaxDeepLevel);

    virtual void draw(QPainter &painter, qreal zoomLevel);

    inline bool is_selected() const noexcept{return m_selected;}

    // ui       
    static inline qreal areaHeight = 0.;
    static inline qreal areaStretch = 0.75;
    static inline qreal sizeTxt = 3.;
    QRectF uiAreaRect = QRectF(0., 0., 0., 0.); /**< area rectangle in the display view */
    QRectF uiElemRect = QRectF(0., 0., 0., 0.); /**< rectangle of the element in the display view */
    display::Colors::FlowElement colors;

    ElementKey key;
    QString name;
    Element::Type type;
    QString informations;

protected:

    bool m_selected;
    std_v1<int> m_insideLoopsID;    
};

}
