
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

// nodes
#include "nodes/StyleCollection.hpp"
#include "nodes/Connection.hpp"
#include "nodes/Node.hpp"

using namespace tool::ex;

ExFlowView::ExFlowView(QtNodes::FlowScene *scene){

    QGraphicsView::setScene(m_scene = scene);

    // options
    setDragMode(QGraphicsView::ScrollHandDrag);
    setRenderHint(QPainter::Antialiasing);

    // style
    setBackgroundBrush(QtNodes::StyleCollection::flowViewStyle().BackgroundColor);

    // scrollbar policy
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setCacheMode(QGraphicsView::CacheBackground);
}

auto ExFlowView::wheelEvent(QWheelEvent *event) -> void{

    bool isHoveringNode = false;
    for(auto &node : m_scene->allNodes()){
        if(node->nodeGeometry().hovered()){
            isHoveringNode = true;
            break;
        }
    }

    QPoint delta = event->angleDelta();
    if (delta.y() == 0){
        event->ignore();
        return;
    }

    const auto d = delta.y() / std::abs(delta.y());
    if(!isHoveringNode){
        if (d > 0.0){
            scale_up();
        }else{
            scale_down();
        }
    }

    emit mouse_wheel_event_signal(event);
}

auto ExFlowView::mousePressEvent(QMouseEvent *event) -> void{

    if(!m_mouseLocked){
        QGraphicsView::mousePressEvent(event);
    }

    if(event->button()== Qt::MouseButton::LeftButton){
        m_leftMouseButtonPressed = true;
        m_lastLeftMousePressedButtonPosition = mapToScene(event->pos());
    }else if(event->button()== Qt::MouseButton::RightButton){
        m_rightMouseButtonPressed = true;
    }else if(event->button()== Qt::MouseButton::MiddleButton){
        m_middleMouseButtonPressed = true;
    }

    emit mouse_pressed_event_signal(event);
}

auto ExFlowView::mouseReleaseEvent(QMouseEvent *event) -> void{

    if(!m_mouseLocked){
        QGraphicsView::mouseReleaseEvent(event);
    }

    if(event->button()== Qt::MouseButton::LeftButton){
        m_leftMouseButtonPressed = false;
        m_lastLeftMouseReleasedButtonPosition = mapToScene(event->pos());
    }else if(event->button()== Qt::MouseButton::RightButton){
        m_rightMouseButtonPressed = false;
    }else if(event->button()== Qt::MouseButton::MiddleButton){
        m_middleMouseButtonPressed = false;
    }
    emit mouse_release_event_signal(event);
}

auto ExFlowView::mouseMoveEvent(QMouseEvent *event) -> void{

    if(!m_mouseLocked){
        QGraphicsView::mouseMoveEvent(event);
    }

    bool itemHovered = false;
    for(const auto &node : m_scene->nodes()){
        if(node.second->nodeGeometry().hovered()){
            itemHovered = true;
            break;
        }
    }
    for(const auto &connection : m_scene->connections()){
        if(connection.second->connectionGeometry().hovered()){
            itemHovered = true;
            break;
        }
    }
    if(itemHovered){
        return;
    }

    if (m_scene->mouseGrabberItem() == nullptr && event->buttons() == Qt::LeftButton){
        // Make sure shift is not being pressed
        if ((event->modifiers() & Qt::ShiftModifier) == 0){
            QPointF difference = m_lastLeftMousePressedButtonPosition - mapToScene(event->pos());
            setSceneRect(sceneRect().translated(difference.x(), difference.y()));
        }
    }
}

auto ExFlowView::keyPressEvent(QKeyEvent *event) -> void{

    switch (event->key()) {
        case Qt::Key::Key_Escape:
            m_scene->clearSelection();
            break;
        case Qt::Key::Key_Control:
            m_controlPressed = true;
            break;
        case Qt::Key::Key_Shift:
            setDragMode(QGraphicsView::RubberBandDrag);
            m_shiftPressed = true;
            break;
        case Qt::Key::Key_C:
            if(m_controlPressed){
                emit copy_signal();
            }
            break;
        case Qt::Key::Key_V:
            if(m_controlPressed){
                emit paste_signal();
            }
            break;
    }
    QGraphicsView::keyPressEvent(event);
}

auto ExFlowView::keyReleaseEvent(QKeyEvent *event) -> void{

    switch (event->key()) {
        case Qt::Key::Key_Delete:
            emit delete_signal();
            break;
        case Qt::Key::Key_Control:
            m_controlPressed = false;
            break;
        case Qt::Key::Key_Shift:
            setDragMode(QGraphicsView::ScrollHandDrag);
            m_shiftPressed = false;
            break;
    }    

    QGraphicsView::keyReleaseEvent(event);
}

auto ExFlowView::contextMenuEvent(QContextMenuEvent *event) -> void{
    if(!m_mouseLocked){
        emit open_context_menu_signal(event->globalPos(),mapToScene(event->pos()));
    }
}

auto ExFlowView::showEvent(QShowEvent *event) -> void{
    m_scene->setSceneRect(this->rect());
    QGraphicsView::showEvent(event);
}

auto ExFlowView::drawForeground(QPainter *painter, const QRectF &rect) -> void{
    tool::Bench::start("ExFlowView::drawForeground");
    QGraphicsView::drawForeground(painter, rect);
    tool::Bench::stop();
}

auto ExFlowView::drawItems(QPainter *painter, int numItems, QGraphicsItem *items[], const QStyleOptionGraphicsItem options[]) -> void{
    tool::Bench::start("ExFlowView::drawItems");
    QGraphicsView::drawItems(painter, numItems, items, options);
    tool::Bench::stop();
}

auto ExFlowView::drawBackground(QPainter *painter, const QRectF &r) -> void{

    tool::Bench::start("ExFlowView::drawBackground");
    QGraphicsView::drawBackground(painter, r);

    const QRect windowRect = rect();
    const QPointF tl = mapToScene(windowRect.topLeft());
    const QPointF br = mapToScene(windowRect.bottomRight());

    auto drawGrid = [&](const double gridStep) {

        const double left   = std::floor(tl.x() / gridStep - 0.5);
        const double right  = std::floor(br.x() / gridStep + 1.0);
        const double bottom = std::floor(tl.y() / gridStep - 0.5);
        const double top    = std::floor(br.y() / gridStep + 1.0);

        const auto leftGridStep   = left * gridStep;
        const auto rightGridStep  = right * gridStep;
        const auto bottomGridStep = bottom * gridStep;
        const auto topGridStep    = top * gridStep;

        std::vector<QLineF> lines;
        lines.reserve((right-left) + (top-bottom));

        // vertical lines
        for (int xi = int(left); xi <= int(right); ++xi){
            lines.emplace_back(
                xi * gridStep, bottomGridStep, xi * gridStep, topGridStep
            );
        }

        // horizontal lines
        for (int yi = int(bottom); yi <= int(top); ++yi){
            lines.emplace_back(
                leftGridStep, yi * gridStep, rightGridStep, yi * gridStep
            );
        }

        painter->drawLines(lines.data(), static_cast<int>(lines.size()));
    };

    const auto& flowViewStyle = QtNodes::StyleCollection::flowViewStyle();
    QPen pfine(flowViewStyle.FineGridColor, 1.0);
    painter->setPen(pfine);
    drawGrid(15);

    QPen p(flowViewStyle.CoarseGridColor, 1.0);
    painter->setPen(p);
    drawGrid(150);
    tool::Bench::stop();
}

auto ExFlowView::lock_mouse() -> void{
    m_mouseLocked = true;
}

auto ExFlowView::unlock_mouse() -> void{
    m_mouseLocked = false;
}

auto ExFlowView::paintEvent(QPaintEvent *event) -> void{
    tool::Bench::start("ExFlowView::paintEvent"sv);
    QGraphicsView::paintEvent(event);
    tool::Bench::stop();
}

auto ExFlowView::resizeEvent(QResizeEvent *event) -> void{
    tool::Bench::start("ExFlowView::resizeEvent"sv);
    QGraphicsView::resizeEvent(event);
    tool::Bench::stop();
}

auto ExFlowView::scale_up() -> void{
    if(m_scaleLvl < m_maxScaleLvl){
        const double step   = 1.2;
        const double factor = std::pow(step, 1.0);
        QTransform t = transform();
        if (t.m11() > 2.0){
            return;
        }
        scale(factor, factor);
        ++m_scaleLvl;
    }
}

auto ExFlowView::scale_down() -> void{
    if(m_scaleLvl > 0){
        double const step   = 1.2;
        double const factor = std::pow(step, -1.0);
        scale(factor, factor);
        --m_scaleLvl;
    }
}

