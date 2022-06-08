
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
#include <QObject>

// local
// # flow
#include "loop_flow_element_object.hpp"
#include "routine_flow_element_object.hpp"
#include "isi_flow_element_object.hpp"
#include "node_flow_element_object.hpp"
// # experiment
#include "experiment/experiment.hpp"

namespace tool::ex {

class FlowSequenceO : public QObject{

public:

    void reset();
    void update_from_experiment(Experiment *exp);

    RowId mouse_on_element_id(const QPoint &mousePos);
    FlowElementO *mouse_on_element(const QPoint &mousePos);
    LoopFlowElementO *mouse_on_loop(const QPoint &mousePos);

    void check_click_on_elements(QPoint clickPos) noexcept;    
    void draw(QPainter &painter, qreal zoomLevel);

    FlowElementO* current_selection() const;

private:

    size_t flow_position(FlowElementO *elem) const;


public:

    std::vector<NodeFlowElementO*>                  nodesElements;
    std::vector<RoutineFlowElementO*>               routinesElements;
    std::vector<IsiFlowElementO*>                   ISIsElements;
    std::vector<LoopNodeFlowElementO*>              loopsStart;
    std::vector<LoopNodeFlowElementO*>              loopsEnd;
    std::vector<std::unique_ptr<LoopFlowElementO>>  loopsElements; // not in elements
    std::vector<std::unique_ptr<FlowElementO>>      elements; // all elements
};
}


