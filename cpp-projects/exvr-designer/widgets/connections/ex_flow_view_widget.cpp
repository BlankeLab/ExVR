
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

#include "ex_flow_view_widget.hpp"

// base
#include "utility/benchmark.hpp"

using namespace tool::ex;



void ExFlowView::paintEvent(QPaintEvent *event){
    tool::Bench::start("ExFlowView::paintEvent");
    QtNodes::FlowView::paintEvent(event);
    tool::Bench::stop();
}

void ExFlowView::resizeEvent(QResizeEvent *event){
    tool::Bench::start("ExFlowView::resizeEvent");
    QtNodes::FlowView::resizeEvent(event);
    tool::Bench::stop();
}

void ExFlowView::wheelEvent(QWheelEvent *event){

    // propagate wheel event to embedded widget
    if(hoveredNode){
        for(const auto &node : scene()->nodes()){
            if(node.second->nodeGeometry().hovered()){
                dynamic_cast<BaseNodeDataModel*>(node.second->nodeDataModel())->wheel_event(event);
                return;
            }
        }
        return;
    }

    QPoint delta = event->angleDelta();
    if (delta.y() == 0){
        event->ignore();
        return;
    }

    const auto d = delta.y() / std::abs(delta.y());

    if (d > 0.0){
        if(scaleLvl < maxScaleLvl){
            scaleUp();
            ++scaleLvl;
        }
    }else{
        if(scaleLvl > 0){
            scaleDown();
            --scaleLvl;
        }
    }
}

void ExFlowView::mousePressEvent(QMouseEvent *event){

    tool::Bench::start("ExFlowView::mousePressEvent");

    auto scenePos = mapToScene(event->pos());
    if(event->button()== Qt::MouseButton::LeftButton){
        leftClick = true;
        p = scenePos;
    }

    FlowView::mousePressEvent(event);
    emit mouse_pressed_event_signal(event);

    tool::Bench::stop();
}

void ExFlowView::mouseReleaseEvent(QMouseEvent *event){

    tool::Bench::start("ExFlowView::mouseReleaseEvent");

    leftClick = false;
    FlowView::mouseReleaseEvent(event);
    emit mouse_release_event_signal(event);

    tool::Bench::stop();
}

void ExFlowView::mouseMoveEvent(QMouseEvent *event){

    Bench::start("ExFlowView::mouseMoveEvent");

    FlowView::mouseMoveEvent(event);

    // check if mouse hovering a node
    hoveredNode = nullptr;
    for(const auto &node : scene()->nodes()){
        if(node.second->nodeGeometry().hovered()){
            hoveredNode = node.second.get();
            break;
        }
    }
    // check if mouse hovering a connection
    hoveredConnection = nullptr;
    for(const auto &connection : scene()->connections()){
        if(connection.second->connectionGeometry().hovered()){
            hoveredConnection = connection.second.get();
            break;
        }
    }

    Bench::stop();
}


void ExFlowView::keyPressEvent(QKeyEvent *event){

    switch (event->key()) {
        case Qt::Key::Key_Control:
            controlPressed = true;
            break;
    }
    FlowView::keyPressEvent(event);
}

void ExFlowView::keyReleaseEvent(QKeyEvent *event){

    switch (event->key()) {
        case Qt::Key::Key_Delete:
            emit delete_selection_signal();
            return;
        case Qt::Key::Key_Control:
            controlPressed = false;
            break;
    }
    FlowView::keyReleaseEvent(event);
}

void ExFlowView::contextMenuEvent(QContextMenuEvent *event){
    BaseNodeDataModel *nodeModelUnderMouse = hoveredNode ? dynamic_cast<BaseNodeDataModel*>(hoveredNode->nodeDataModel()) : nullptr;
    emit open_context_menu_signal(event->globalPos(), mapToScene(event->pos()), nodeModelUnderMouse, hoveredConnection);
}
