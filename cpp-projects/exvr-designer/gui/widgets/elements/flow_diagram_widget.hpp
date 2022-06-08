
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
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>
#include <QSlider>
#include <QMenu>

// local
#include "gui/objects/flow_sequence_object.hpp"


namespace tool::ex {

struct ClickableElements{

    QVector<QPair<QRectF,std::unique_ptr<FlowElementO>>> elements;

    // inused
    QVector<QPair<QRectF,std::unique_ptr<FlowElementO>>> nodes;
    QVector<QPair<QRectF,std::unique_ptr<FlowElementO>>> loops;
    QVector<QPair<QRectF,std::unique_ptr<FlowElementO>>> ISIs;
    QVector<QPair<QRectF,std::unique_ptr<FlowElementO>>> routines;

};

class FlowDiagramW;
using FlowDiagramWUP = std::unique_ptr<FlowDiagramW>;

class FlowDiagramW : public QWidget{

    Q_OBJECT

public :

    FlowDiagramW(QSlider *zoomSlider, QPushButton *resizeButton);

    void update_from_experiment(Experiment *exp);
    void reset();

    void contextMenuEvent(QContextMenuEvent *event) override;

public slots:

    void auto_resize();
    void zoom(int value);

    void update_stretch(qreal stretch);
    void update_size_text(qreal sizeTxt);


protected:

    void draw_arrow_line(QPainter &painter, const QLineF &line, qreal m_zoomLevel);

    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:

    void generate_signals();

    qreal m_zoomLevel = 3.0;
    const qreal m_minZoomLevel = 0.5;
    const qreal m_maxZoomLevel = 5.0;

    int m_maximumDeepLevel = 0;

    FlowSequenceO m_flowSequence;

};
}

