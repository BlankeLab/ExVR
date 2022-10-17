
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
#include <QDialog>
#include <QPushButton>
#include <QFormLayout>
#include <QPainter>
#include <QFontMetrics>
#include <QPlainTextEdit>

// base
#include "utility/benchmark.hpp"

// qt-utility
#include "qt_str.hpp"
#include "data/argument.hpp"
#include "gui/ex_widgets/ex_label_w.hpp"

// local
#include "gui/widgets/connections/data_models/data/nodes_data.hpp"

namespace tool::ex{

class NodeDataDisplayDialog : public QDialog {

Q_OBJECT

public:

    NodeDataDisplayDialog(const QString &caption, bool popup);

    void add_row_in_dialog(QString name, QWidget *w, bool horizontalLayout);
    bool node_settings_execute(QPoint pos);

    QVBoxLayout *layout = nullptr;

private:
    bool m_popup = true;
};


class BaseNodeContainerW : public QWidget{

Q_OBJECT
public:

    ~BaseNodeContainerW();

    void initialize_container(const QString &caption, bool popup, Qt::TextFormat textFormat = Qt::PlainText);

    void close();
    void add_row_in_dialog(QString name, QWidget *w, bool horizontalLayout = true);

    virtual bool set_text_value(const QString &value);
    virtual bool set_pixmap_value(const QPixmap &value);
    void disable();
    void enable();
    void set_title(QString title);

    virtual bool update_with_info(QStringView value);
    virtual void update_from_arg(const Arg &arg) = 0;
    virtual void update_from_components(){}
    virtual Arg convert_to_arg() = 0;

    virtual void trigger_signals() = 0;

    virtual void paintEvent(QPaintEvent *event) override;

signals:

    void update_internal_data_signal(std_v1<size_t> indexes, std_v1<std::shared_ptr<NodeData>> nodes);
    void compute_data_signal();
    void ask_node_to_update();

protected:

//    QPlainTextEdit m_valueText;
    QLabel m_valueText;
    QPushButton m_openDialogButton;
    QPushButton m_helpButton;

    std::optional<QString> m_dialogTitle;
    std_v1<std::tuple<QString, QWidget*, bool>> m_dialogWidgetsRows;
    std::unique_ptr<NodeDataDisplayDialog> m_dataDisplayD = nullptr;


    static const QString buttonStyle1;
    static const QString buttonStyle2;
};

template <typename W>
class NodeContainerW : public BaseNodeContainerW{

public:
    NodeContainerW(){
        w = std::make_unique<W>("value");
    }    

    virtual void initialize(){}

    virtual void update_from_arg(const Arg &arg) override{
        w->update_from_arg(arg);

    }
    virtual Arg convert_to_arg() override{
        return w->convert_to_arg();
    }

    void trigger_signals() override{
        emit w->trigger_ui_change();
    }

    std::unique_ptr<W> w = nullptr;
};


class PlaceHolderEmbeddedW : public NodeContainerW<ExLabelW>{

Q_OBJECT
public:

    void initialize() override{

        // init widget
        w->init_widget("");

        // set widget connections
        connect(w.get(), &ExLabelW::ui_change_signal, this, [=]{
            emit compute_data_signal();
        });
    }
};

}

