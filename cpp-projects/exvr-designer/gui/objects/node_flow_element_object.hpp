
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
#include <QVector>

// local
#include "data/flow_elements/node_flow.hpp"
#include "add_button_object.hpp"

namespace tool::ex {

class NodeFlowElementO : public FlowElementO{

public:

    NodeFlowElementO(NodeFlow *node);

    void adapt_size_from_name(QFontMetrics fontMetrics) override;
    void draw(QPainter &painter, qreal zoomLevel) override;
    void compute_position(QPointF topLeft, int loopMaxDeepLevel) override;
    void draw_add_buttons(QPainter &painter, qreal zoomLevel);

    std::unique_ptr<AddButtonO> addRoutine = nullptr;
    std::unique_ptr<AddButtonO> addIsi     = nullptr;
    std::unique_ptr<AddButtonO> addLoop    = nullptr;
};
}
