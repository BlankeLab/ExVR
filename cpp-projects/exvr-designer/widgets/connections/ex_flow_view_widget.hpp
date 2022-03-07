
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
#include <QWheelEvent>
#include <QMenu>

// nodes
#include "nodes/FlowView.hpp"
#include "nodes/FlowScene.hpp"
#include "nodes/Node.hpp"


// local
#include "data_models/base_node_data_model.hpp"

namespace tool::ex {

class ExFlowView : public QtNodes::FlowView{

Q_OBJECT

public :

    ExFlowView(QtNodes::FlowScene *scene) : QtNodes::FlowView(scene){
    }


    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

signals:

    void open_context_menu_signal(QPoint pos, QPointF mappedPos, BaseNodeDataModel *nodeModelUnderMouse, QtNodes::Connection *connectionUnderMouse);
    void delete_selection_signal();

    void mouse_pressed_event_signal(QMouseEvent *event);
    void mouse_release_event_signal(QMouseEvent *event);

private:


    int scaleLvl = 3;
    const int maxScaleLvl = 8;

    bool leftClick = false;

    bool controlPressed = false;

    QPointF p;
    int minX = -1000;
    int maxX = 1000;

    QtNodes::Node* hoveredNode = nullptr;
    QtNodes::Connection *hoveredConnection = nullptr;

};
}
