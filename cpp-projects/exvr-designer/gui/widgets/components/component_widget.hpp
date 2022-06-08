
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
#include <QFrame>
#include <QIcon>
#include <QLabel>
#include <QPushButton>

// local
// # data
#include "data/component.hpp"

namespace tool::ex {

class ComponentW;
using ComponentWUP = std::unique_ptr<ComponentW>;

enum class ComponentStyle{
    neutral,
    hover,
    hoverShow,
    show,
};

class ComponentW : public QFrame{
    Q_OBJECT

public :

    ComponentW(Component *component);

    void update_from_component(Component *component);
    void update_style();

public slots:

    void dropEvent(QDropEvent *event) override;

protected:

    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

public :

    // data
    QIcon icon;

    ComponentKey key = {-1};
    Component::Type type;
    Component::Category category;
    bool isStatic= false;

    bool mouseHovering  = false;
    bool showWindow     = false;

protected:

    QPushButton *m_iconButton = nullptr;
    QLabel *m_titleLabel = nullptr;

    ComponentStyle currentStyle = ComponentStyle::neutral;
    const static inline QString componentNeutralStyle   = QSL("QPushButton{background-color:white;}");
    const static inline QString componentShowStyle      = QSL("QPushButton{background-color:rgb(160, 211, 146);}");
    const static inline QString hoverComponentStyle     = QSL("QPushButton{background-color:lightgrey;}");
    const static inline QString hoverComponentShowStyle = QSL("QPushButton{background-color:rgb(211, 235, 205);}");
};

}
