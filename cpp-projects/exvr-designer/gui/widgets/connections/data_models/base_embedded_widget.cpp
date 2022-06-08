
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

#include "base_embedded_widget.hpp"

// base
#include "utility/benchmark.hpp"
// qt-utility
#include "qt_logger.hpp"

using namespace tool::ex;

const QString BaseNodeContainerW::buttonStyle1 =
    "QPushButton#settingButton {\
        background-color: rgb(0, 128, 192);\
        border-style: outset;\
        border-width: 2px;\
        border-color: black;\
        border-radius: 5px;\
    }\
    QPushButton#settingButton:pressed {\
        background-color: rgb(72, 196, 255);\
        border-style: inset;\
    }\
    QPushButton#settingButton:hover:!pressed {\
        background-color: rgb(155, 222, 255);\
        border-style: inset;\
    }";

const QString BaseNodeContainerW::buttonStyle2 =
    "QPushButton#settingButton {\
        background-color: rgb(192, 128, 0);\
        border-style: outset;\
        border-width: 2px;\
        border-color: black;\
        border-radius: 5px;\
    }\
    QPushButton#settingButton:pressed {\
        background-color: rgb(255, 196, 72);\
        border-style: inset;\
    }\
    QPushButton#settingButton:hover:!pressed {\
        background-color: rgb(255, 222, 155);\
        border-style: inset;\
    }";

NodeDataDisplayDialog::NodeDataDisplayDialog(const QString &caption, bool popup) : m_popup(popup){

    layout = new QVBoxLayout(this);
    setWindowTitle(caption);

    if(m_popup){
        setWindowFlags(Qt::Popup);
    }else{
        setWindowFlag(Qt::Window);
    }
}

void NodeDataDisplayDialog::add_row_in_dialog(QString name, QWidget *w, bool horizontalLayout){

    // store it
    if(name.length() > 0){
        auto formW = new QWidget();

        if(horizontalLayout){
            auto hl = ui::L::HB();
            hl->setContentsMargins(0,0,0,0);
            hl->setStretch(0, 1);
            hl->setStretch(1, 20);
            hl->addWidget(new QLabel(name));
            hl->addWidget(w);
            formW->setLayout(hl);
        }else{
            auto vl = ui::L::VB();
            vl->setContentsMargins(0,0,0,0);
            vl->setStretch(0, 1);
            vl->setStretch(1, 20);
            vl->addWidget(new QLabel(name));
            vl->addWidget(w);
            formW->setLayout(vl);
        }
        layout->addWidget(formW);

    }else{
        layout->addWidget(w);
    }
}

bool NodeDataDisplayDialog::node_settings_execute(QPoint pos){

    move(pos);
    if(m_popup){
        return exec() == QDialog::Accepted;
    }else{
        show();
        raise();
        setFocus();
        return true;
    }
}


void BaseNodeContainerW::initialize_container(const QString &caption, bool popup, Qt::TextFormat textFormat){

    setContentsMargins(0,0,0,0);
    setObjectName("parent-widget");
    setStyleSheet("#parent-widget {background-color: rgba(0,0,0,0)}");

    m_dataDisplayD = std::make_unique<NodeDataDisplayDialog>(caption, popup);
    m_valueText.setTextFormat(textFormat);

    // layout
    auto l = new QHBoxLayout();
    setLayout(l);
    l->addWidget(&m_openDialogButton);
    l->addWidget(&m_valueText);
    l->setContentsMargins(0,0,0,0);
//    l->addStretch();

    m_openDialogButton.setText("");
    m_helpButton.setText("");

    QSize sizeB(20,20);
    m_openDialogButton.setMaximumSize(sizeB);
    m_helpButton.setMaximumSize(sizeB);

    // m_valueText.setFixedSize(100,20);

    m_openDialogButton.setObjectName("settingButton");
    m_openDialogButton.setStyleSheet(buttonStyle1);
    connect(&m_openDialogButton, &QPushButton::clicked, this, [&]{
        m_dataDisplayD->node_settings_execute(QCursor::pos()+ QPoint(0,10));
    });
}


BaseNodeContainerW::~BaseNodeContainerW(){
    close();
}

void BaseNodeContainerW::close(){
    m_dataDisplayD->close();
    m_dataDisplayD->hide();
}


void BaseNodeContainerW::add_row_in_dialog(QString name, QWidget *w, bool horizontalLayout){
    m_dataDisplayD->add_row_in_dialog(name,w, horizontalLayout);
}

bool BaseNodeContainerW::set_text_value(const QString &value){

//    bool askForResize = m_valueText.text().size() != value.size();
//    m_valueText.blockSignals(true);

    Bench::start("BaseNodeContainerW::set_text_value");
    m_valueText.setText(value);
    Bench::stop();
//    m_valueText.blockSignals(false);
    return true;
//    return askForResize;
}

void BaseNodeContainerW::disable(){
    setEnabled(false);
    m_openDialogButton.setStyleSheet(buttonStyle2);
}

void BaseNodeContainerW::enable(){
    setEnabled(true);
    m_openDialogButton.setStyleSheet(buttonStyle1);
}

void BaseNodeContainerW::set_title(QString title){
    m_dialogTitle = title;
    m_dataDisplayD->setWindowTitle(title);
}

bool BaseNodeContainerW::update_with_info(QStringView value){
    return set_text_value(value.toString());
}

void BaseNodeContainerW::paintEvent(QPaintEvent *event){
    tool::Bench::start("BaseNodeContainerW::paintEvent");
    QWidget::paintEvent(event);
    tool::Bench::stop();
}

#include "moc_base_embedded_widget.cpp"

