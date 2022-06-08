
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

#include "component_widget.hpp"
#include "qt_str.hpp"

// Qt
#include <QHBoxLayout>
#include <QMimeData>
#include <QPainter>
#include <QPushButton>
#include <QMouseEvent>
#include <QDrag>
#include <QMenu>
#include <QWidget>

// qt-utility
#include "qt_ui.hpp"

// local
#include "experiment/global_signals.hpp"

using namespace tool;
using namespace tool::ex;

ComponentW::ComponentW(Component *component) : key(component->c_key()), type(component->type), category(component->category){

    setMouseTracking(true);

    // frame icon
    // setMinimumHeight(45);

    icon = QIcon(from_view(Component::get_icon_path(type)));

    m_iconButton = new QPushButton(icon,"");
    m_iconButton->setIconSize(QSize(15,15));
    m_iconButton->setObjectName(QSL("e_") % QString::number(key.v));
    connect(m_iconButton, &QPushButton::clicked, this, [&]{
        emit GSignals::get()->toggle_component_parameters_signal(key);
    });

    // title
    m_titleLabel = new QLabel(component->name(), this);
    m_titleLabel->setObjectName(QSL("t_") % from_view(Component::get_full_name(type)));

    // layout
    auto hLayout = ui::L::HB();
    hLayout->setContentsMargins(0,1,1,0);
    hLayout->addWidget(m_iconButton);
    hLayout->addWidget(m_titleLabel);
    hLayout->addStretch();
    auto mW = new QWidget();
    mW->setLayout(hLayout);

    auto vLayout = ui::L::VB();
    vLayout->setContentsMargins(2,0,2,0);
    vLayout->setSpacing(1);
    vLayout->addWidget(mW);

    QWidget *horizontalLineWidget = new QWidget;
    horizontalLineWidget->setFixedHeight(2);
    horizontalLineWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    horizontalLineWidget->setStyleSheet(QSL("background-color: #c0c0c0;"));
    vLayout->addWidget(horizontalLineWidget);
    setLayout(vLayout);

    setStyleSheet(componentNeutralStyle);
    currentStyle = ComponentStyle::neutral;
}

void ComponentW::update_from_component(Component *component){
    type     = component->type;
    category = component->category;
    m_titleLabel->setText(component->name());
}

void ComponentW::update_style(){

    if(mouseHovering){
        if(showWindow){
            if(currentStyle != ComponentStyle::hoverShow){
                setStyleSheet(hoverComponentShowStyle);
                currentStyle = ComponentStyle::hoverShow;
            }
        }else{
            if(currentStyle != ComponentStyle::hover){
                setStyleSheet(hoverComponentStyle);
                currentStyle = ComponentStyle::hover;
            }
        }
    }else{
        if(showWindow){
            if(currentStyle != ComponentStyle::show){
                setStyleSheet(componentShowStyle);
                currentStyle = ComponentStyle::show;
            }
        }else{
            if(currentStyle != ComponentStyle::neutral){
                setStyleSheet(componentNeutralStyle);
                currentStyle = ComponentStyle::neutral;
            }
        }
    }
}

void ComponentW::dropEvent(QDropEvent *event){
    Q_UNUSED(event)
}

void ComponentW::mousePressEvent(QMouseEvent *event){

    // right click
    if(event->button()== Qt::MouseButton::RightButton){
        emit GSignals::get()->show_component_custom_menu_signal(event->globalPos(),key);
        return;
    }

    // left click
    if(!isStatic){

        QPixmap pixmap = m_iconButton->icon().pixmap(QSize(50,50));
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(m_iconButton->objectName());

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap);
        drag->setHotSpot(QPoint(drag->pixmap().width()/2,
                                drag->pixmap().height()/2));

        if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
            m_iconButton->close();
        } else {
            m_iconButton->show();
        }
    }

    emit GSignals::get()->toggle_selection_component_signal(key);
}

void ComponentW::mouseDoubleClickEvent(QMouseEvent *event){

    if(event->button() == Qt::MouseButton::LeftButton){
        emit GSignals::get()->toggle_component_parameters_signal(key);
    }
    if(event->button() == Qt::MouseButton::MiddleButton){
        emit GSignals::get()->remove_component_signal(key);
    }
}

void ComponentW::enterEvent(QEvent *event){
    Q_UNUSED(event)
    mouseHovering = true;
    emit GSignals::get()->enter_component_signal(key);
}

void ComponentW::leaveEvent(QEvent *event){
    Q_UNUSED(event)
    mouseHovering = false;
    emit GSignals::get()->leave_component_signal(key);
}

#include "moc_component_widget.cpp"


