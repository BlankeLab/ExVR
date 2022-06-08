
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
#include "data/flow_elements/flow_element.hpp"
#include "gui/settings/display.hpp"

namespace tool::ex {


class ElementO : public QObject{
public:

    ElementO(QString name) :name(name){}
    ElementO(ButtonType type){
        switch(type){
            case ButtonType::AddElement:
                name = "------";
                break;
            case ButtonType::RemoveElement:
                name = "-----";
                break;
            case ButtonType::MoveElement:
                name = "-----";
                break;
            default:
                break;
        }
    }
    static void define_area_height(qreal height){areaHeight = height;}
    static void define_stretch(qreal stretch){areaStretch = stretch;};

    virtual void adapt_size_from_name(QFontMetrics fontMetrics);
    virtual void compute_position(QPointF topLeft, int loopMaxDeepLevel);
    virtual void draw(QPainter &painter, qreal zoomLevel);

    static inline qreal areaHeight = 0.;
    static inline qreal areaStretch = 0.75;
    static inline qreal sizeTxt = 3.;

    QRectF uiAreaRect = QRectF(0., 0., 0., 0.); /**< area rectangle in the display view */
    QRectF uiElemRect = QRectF(0., 0., 0., 0.); /**< rectangle of the element in the display view */

    QString name;
};


class FlowElementO : public ElementO{

public:

    FlowElementO() = delete;
    FlowElementO(FlowElement *element);

    FlowElementO(FlowElementO&&) = delete;
    FlowElementO& operator=(FlowElementO&&) = delete;
    FlowElementO(const FlowElementO&) = delete;
    FlowElementO& operator=(const FlowElementO&) = delete;

    virtual void update(FlowElement *element);
    inline bool is_selected() const noexcept{return m_selected;}

    ElementKey key;
    FlowElement::Type type;
    QString informations;

protected:

    bool m_selected;
    std::vector<ElementKey> m_insideLoopsID;
};

}
