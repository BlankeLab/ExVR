
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
#include <QGraphicsView>

// nodes
#include "nodes/FlowScene.hpp"

namespace tool::ex {

class ExFlowView : public QGraphicsView{

Q_OBJECT

public :

    ExFlowView(QtNodes::FlowScene *scene);
    ExFlowView(const ExFlowView&) = delete;
    ExFlowView operator=(const ExFlowView&) = delete;

    auto wheelEvent(QWheelEvent *event) -> void override;
    auto mousePressEvent(QMouseEvent *event) -> void override;
    auto mouseReleaseEvent(QMouseEvent *event) -> void override;
    auto mouseMoveEvent(QMouseEvent *event) -> void override;
    auto keyPressEvent(QKeyEvent *event) -> void override;
    auto keyReleaseEvent(QKeyEvent *event) -> void override;
    auto contextMenuEvent(QContextMenuEvent *event) -> void override;
    auto showEvent(QShowEvent *event) -> void override;

    auto paintEvent(QPaintEvent *event) -> void override;
    auto resizeEvent(QResizeEvent *event) -> void override;
    auto drawForeground(QPainter *painter, const QRectF &rect) -> void override;
    auto drawItems(QPainter *painter, int numItems, QGraphicsItem *items[], const QStyleOptionGraphicsItem options[]) -> void override;
    auto drawBackground(QPainter* painter, const QRectF& r) -> void override;

signals:

    // mouse
    auto open_context_menu_signal(QPoint pos, QPointF mappedPos) -> void;
    auto mouse_pressed_event_signal(QMouseEvent *event) -> void;
    auto mouse_release_event_signal(QMouseEvent *event) -> void;
    auto mouse_wheel_event_signal(QWheelEvent *event) -> void;
    // keys
    auto delete_signal() -> void;
    auto copy_signal() -> void;
    auto paste_signal() -> void;

public slots:

    auto lock_mouse()->void;
    auto unlock_mouse()->void;

private slots:

    auto scale_up() -> void;
    auto scale_down() -> void;

private:

    // sacle view
    int m_scaleLvl = 3;
    static constexpr int m_maxScaleLvl = 8;

    // mouse
    bool m_leftMouseButtonPressed = false;
    bool m_rightMouseButtonPressed = false;
    bool m_middleMouseButtonPressed = false;
    QPointF m_lastLeftMousePressedButtonPosition;
    QPointF m_lastLeftMouseReleasedButtonPosition;

    // key
    bool m_controlPressed = false;
    bool m_shiftPressed = false;

    QtNodes::FlowScene* m_scene = nullptr;
    bool m_mouseLocked = false;
};
}
