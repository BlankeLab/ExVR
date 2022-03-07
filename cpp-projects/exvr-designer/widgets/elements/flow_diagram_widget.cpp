
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

#include "flow_diagram_widget.hpp"

// Qt
#include <QScrollArea>
#include <QScrollBar>
#include <QInputDialog>
#include <QToolTip>

using namespace tool::ex;


FlowDiagramW::FlowDiagramW(QSlider *zoomSlider, QPushButton *resizeButton){

    setFocusPolicy(Qt::StrongFocus);
    setObjectName("FlowDiagram");
    setMouseTracking(true);

    // connections
    // # ui
    connect(resizeButton, &QPushButton::clicked, this, [=]{
        auto_resize();
        zoomSlider->blockSignals(true);
        zoomSlider->setValue(static_cast<int>(m_zoomLevel*10));
        zoomSlider->blockSignals(false);
    });
    connect(zoomSlider, &QSlider::valueChanged, this, [&](int value){
        m_zoomLevel = 0.1*value;
        update();
    });

    // # flow sequence
    connect(&m_flowSequence, &FlowSequence::select_element_signal,          this, &FlowDiagramW::select_element_signal);
    connect(&m_flowSequence, &FlowSequence::unselect_element_signal,        this, &FlowDiagramW::unselect_element_signal);
    connect(&m_flowSequence, &FlowSequence::add_element_signal,             this, &FlowDiagramW::add_element_signal);
    connect(&m_flowSequence, &FlowSequence::remove_selected_element_signal, this, &FlowDiagramW::remove_selected_element_signal);
    connect(&m_flowSequence, &FlowSequence::move_element_left_signal,       this, &FlowDiagramW::move_element_left_signal);
    connect(&m_flowSequence, &FlowSequence::move_element_right_signal,      this, &FlowDiagramW::move_element_right_signal);

    // menus
//    duplicateElementA = new QAction("Duplicate");
//    connect(duplicateElementA, &QAction::triggered, this, [=](){
////        m_flowSequence.elements
//    });
//    removeElementA = new QAction("Remove");
//    connect(removeElementA, &QAction::triggered, this, [=](){

//    });
//    moveLeftElementA = new QAction("Move left");
//    connect(moveLeftElementA, &QAction::triggered, this, [=](){

//    });
//    moveRightElementA = new QAction("Move right");
//    connect(moveRightElementA, &QAction::triggered, this, [=](){

//    });
}


void FlowDiagramW::contextMenuEvent(QContextMenuEvent *event) {


    auto exec_element_menu = [this, event](FlowElement *element) {

        bool mouseOnElement = false;
        const bool isNode    = element->type == Element::Type::Node;
        const bool isRoutine = element->type == Element::Type::Routine;
        const bool isIsi     = element->type == Element::Type::Isi;
        const bool isLoop    = element->type == Element::Type::Loop;



        if(isRoutine || isIsi || isNode){
            mouseOnElement = element->uiAreaRect.contains(event->pos());
        }else if(isLoop){
            auto loop = dynamic_cast<LoopFlowElement*>(element);
            mouseOnElement =
                    loop->uiElemRect.contains(event->pos()) ||
                    loop->startLoopNode->uiAreaRect.contains(event->pos()) ||
                    loop->endLoopNode->uiAreaRect.contains(event->pos());
        }

        if(mouseOnElement){

            QMenu menu;
            ElementKey key = ElementKey{element->key};

            if(element->is_selected()){
                auto unselectedElement = new QAction(QSL("Unselect"));
                connect(unselectedElement, &QAction::triggered, this, [=](){
                    emit unselect_element_signal();
                });
                menu.addAction(unselectedElement);

                if(!isNode){
                    menu.addSeparator();
                }
            }

            if(isRoutine || isIsi || isLoop){

                auto duplicateElementA = new QAction(QSL("Duplicate"));
                connect(duplicateElementA, &QAction::triggered, this, [=](){
                    emit duplicate_element_signal(key);
                });
                menu.addAction(duplicateElementA);
            }

            if(isRoutine || isIsi || isLoop){
                auto removeElementA = new QAction(QSL("Remove"));
                connect(removeElementA, &QAction::triggered, this, [=](){
                    emit remove_element_signal(key);
                });
                menu.addAction(removeElementA);
            }

            if(isRoutine){
                menu.addSeparator();

                auto cleanCurrentRoutineConditionA = new QAction(QSL("Clean current condition"));
                connect(cleanCurrentRoutineConditionA, &QAction::triggered, this, [=](){
                    emit clean_current_routine_condition_signal(key);
                });
                menu.addAction(cleanCurrentRoutineConditionA);

                auto cleanAllRoutineConditionsA = new QAction(QSL("Clean all conditions"));
                connect(cleanAllRoutineConditionsA, &QAction::triggered, this, [=](){
                    emit clean_all_routine_conditions_signal(key);
                });
                menu.addAction(cleanAllRoutineConditionsA);

                menu.addSeparator();

                auto setAllRoutineConditionsDurationA = new QAction(QSL("Set duration for all conditions"));
                connect(setAllRoutineConditionsDurationA, &QAction::triggered, this, [=](){
                    emit set_duration_for_all_routine_conditions_signal(key);
                });
                menu.addAction(setAllRoutineConditionsDurationA);
            }

            menu.exec(event->globalPos());
            return true;
        }
        return false;
    };


    for(const auto &element : m_flowSequence.elements){
        if(exec_element_menu(element.get())){
            return;
        }
    }
    for(const auto &loop : m_flowSequence.loopsElements){
        if(exec_element_menu(loop.get())){
            return;
        }
    }
}

void FlowDiagramW::update_from_experiment(Experiment *exp){

    m_flowSequence.update_from_experiment(exp);
    m_maximumDeepLevel = exp->states.maximumDeepLevel;
    update();
}

void FlowDiagramW::reset(){
    m_flowSequence.reset();
    m_maximumDeepLevel = 0;
}

void FlowDiagramW::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event)
}

void FlowDiagramW::paintEvent(QPaintEvent *event){

    Q_UNUSED(event)

    // setup rendering
    QPainter painter(this);

    painter.fillRect(geometry(),Qt::lightGray);
    painter.setRenderHint(QPainter::Antialiasing);

    // define font and metrics
    QFont font;
    font.setPointSizeF(FlowElement::sizeTxt*m_zoomLevel);
    QFontMetrics fm(font);
    painter.setFont(font);

    // compute elements sizes
    FlowElement::define_area_height(fm.boundingRect("O").height()*2.5);
    // # elements
    for(auto& element : m_flowSequence.elements){
        element->adapt_size_from_name(fm);
    }
    // # loops
    for(auto& loop : m_flowSequence.loopsElements){
        loop->adapt_size_from_name(fm);
    }

    // area sizes
    qreal allElementsWidth = 0.;
    for(const auto& element : m_flowSequence.elements){
        allElementsWidth += element->uiAreaRect.width();
    }

    // compute starting and ending point
    const QPointF startMainLine(20*m_zoomLevel, (m_maximumDeepLevel+1)*FlowElement::areaHeight + FlowElement::areaHeight*0.5);
    const QPointF endMainLine(allElementsWidth+2*startMainLine.x(), (m_maximumDeepLevel+1)*FlowElement::areaHeight + FlowElement::areaHeight*0.5);
    const QSizeF allElementsAreaSize(endMainLine.x(), endMainLine.y() + (m_maximumDeepLevel+1)*FlowElement::areaHeight + 2*FlowElement::areaHeight);

    // define widget size
    setMinimumSize(static_cast<int>(allElementsAreaSize.width()),static_cast<int>(allElementsAreaSize.height()));

    // draw main line
    QPen linePen;
    linePen.setColor(Qt::black);
    linePen.setWidthF(m_zoomLevel*1.5);
    painter.setPen(linePen);
    draw_arrow_line(painter, QLineF(startMainLine,endMainLine),m_zoomLevel);

    // compute elements positions
    qreal xoffset = startMainLine.x();   
    for(auto& element : m_flowSequence.elements){
        QPointF topLeft(xoffset, (1+m_maximumDeepLevel)*FlowElement::areaHeight);
        element->compute_position(topLeft, 1+m_maximumDeepLevel);
        xoffset += element->uiAreaRect.width();
    }

    // draw sequence
    m_flowSequence.draw(painter, m_zoomLevel);

}

void FlowDiagramW::auto_resize(){ // TODO: to improve

    QSize scrollAreaSize = qobject_cast<QWidget*>(this->parent()->parent())->size();
    qreal factorX = 1.*scrollAreaSize.width()/minimumSize().width();
    qreal factorY = 1.*scrollAreaSize.height()/minimumSize().height();

    m_zoomLevel *= std::min(factorX, factorY);
    if(m_zoomLevel < m_minZoomLevel){
        m_zoomLevel = m_minZoomLevel;
    }else if(m_zoomLevel > m_maxZoomLevel){
        m_zoomLevel = m_maxZoomLevel;
    }
    m_zoomLevel*=0.9;
    update();
}

void FlowDiagramW::zoom(int value){
    m_zoomLevel = 0.1*value;
    update();
}

void FlowDiagramW::update_stretch(qreal stretch){
    FlowElement::define_stretch(stretch);
    update();
}

void FlowDiagramW::update_size_text(qreal sizeTxt){
    FlowElement::sizeTxt = sizeTxt;
    update();
}

void FlowDiagramW::draw_arrow_line(QPainter &painter, const QLineF &line, qreal zoomLevel){

    QSizeF sizeArrow(7.*zoomLevel, 7.*zoomLevel);

    QLineF lineArrow = line;
    lineArrow.setP2(lineArrow.p2() - QPointF(sizeArrow.width(),0.));

    QPen pen;
    pen.setWidthF(1.* zoomLevel);
    pen.setBrush(QBrush(Qt::black, Qt::SolidPattern));
    painter.setPen(pen);

    painter.drawLine(lineArrow.p1(),lineArrow.p2()- QPointF(sizeArrow.width() ,0.));

    QPolygonF arrow;
    arrow << lineArrow.p2();
    arrow << QPointF(lineArrow.p2().x()- sizeArrow.width(), lineArrow.p2().y() + sizeArrow.height()*0.5);
    arrow << QPointF(lineArrow.p2().x()- sizeArrow.width(), lineArrow.p2().y() - sizeArrow.height()*0.5);

    painter.drawPolygon(arrow);

    QPainterPath path;
    path.addPolygon(arrow);
    painter.fillPath(path,QBrush(Qt::black, Qt::SolidPattern));

    painter.drawText(lineArrow.p2() + QPointF(0.,sizeArrow.height()), "t");
}

void FlowDiagramW::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        m_flowSequence.check_click_on_elements(event->pos());
    }
}

void FlowDiagramW::mouseMoveEvent(QMouseEvent *event){
    if(auto element = m_flowSequence.mouse_on_element(event->pos()); element != nullptr){
        if(element->informations.length() == 0){
            return;
        }
        if(element->type == Element::Type::Routine || element->type == Element::Type::Isi){
            QToolTip::showText(event->globalPos(), element->informations, this, element->uiElemRect.toRect());
        }
    }

    if(auto loop = m_flowSequence.mouse_on_loop(event->pos()); loop != nullptr){
        if(loop->informations.length() == 0){
            return;
        }
        QToolTip::showText(event->globalPos(), loop->informations, this, loop->uiElemRect.toRect());
    }
}

void FlowDiagramW::keyPressEvent(QKeyEvent *event){

    QWidget::keyPressEvent(event);
    if(event->key() == Qt::Key_Delete){
        emit remove_selected_element_signal();
    }
}
#include "moc_flow_diagram_widget.cpp"
