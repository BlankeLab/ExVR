
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
#include "flow/flow_sequence.hpp"

// debug
#include <QDebug>


namespace tool::ex {

struct ClickableElements{

    QVector<QPair<QRectF,FlowElementUP>> elements;

    // inused
    QVector<QPair<QRectF,FlowElementUP>> nodes;
    QVector<QPair<QRectF,FlowElementUP>> loops;
    QVector<QPair<QRectF,FlowElementUP>> ISIs;
    QVector<QPair<QRectF,FlowElementUP>> routines;

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

signals:

    void select_element_signal(ElementKey elementKey);
    void unselect_element_signal();
    void add_element_signal(Element::Type type, size_t index);
    void remove_selected_element_signal();
    void move_element_left_signal(size_t id);
    void move_element_right_signal(size_t id);

    void duplicate_element_signal(ElementKey elementKey);
    void remove_element_signal(ElementKey elementKey);
    void clean_current_routine_condition_signal(ElementKey routineKey);
    void clean_all_routine_conditions_signal(ElementKey routineKey);
    void set_duration_for_all_routine_conditions_signal(ElementKey routineKey);

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

    FlowSequence m_flowSequence;

};
}

