
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
#include "loop_flow_element.hpp"
#include "routine_flow_element.hpp"
#include "isi_flow_element.hpp"
#include "node_flow_element.hpp"
// # experiment
#include "experiment/experiment.hpp"

namespace tool::ex {

class FlowSequence;
using FlowSequenceUP = std::unique_ptr<FlowSequence>;

class FlowSequence : public QObject{

    Q_OBJECT

public:

    void reset();
    void update_from_experiment(Experiment *exp);

    FlowElement *mouse_on_element(const QPoint &mousePos);
    LoopFlowElement *mouse_on_loop(const QPoint &mousePos);

    void check_click_on_elements(QPoint clickPos) noexcept;    
    void draw(QPainter &painter, qreal zoomLevel);

    FlowElement* current_selection() const;

signals:

    void select_element_signal(ElementKey elementKey);
    void unselect_element_signal();
    void add_element_signal(Element::Type type, size_t index);
    void remove_selected_element_signal();
    void move_element_left_signal(size_t id);
    void move_element_right_signal(size_t id);

private:

    size_t flow_position(FlowElement *elem) const;


public:

    std_v1<NodeFlowElement*>        nodesElements;
    std_v1<RoutineFlowElement*>     routinesElements;
    std_v1<IsiFlowElement*>         ISIsElements;
    std_v1<LoopNodeFlowElement*>    loopsStart;
    std_v1<LoopNodeFlowElement*>    loopsEnd;
    std_v1<LoopFlowElementUP>       loopsElements; // not in elements
    std_v1<FlowElementUP>           elements; // all elements
};
}


