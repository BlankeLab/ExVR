
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

// qt-utility
#include "qt_str.hpp"

// local
#include "data/argument.hpp"
#include "widgets/list_widget.hpp"
#include "ex_widgets/ex_item_w.hpp"

// Qt
#include <QScrollArea>
#include <QScrollBar>

namespace tool::ex {


class ExParametersGeneratorWidgetW : public ExItemW<QWidget>{

    Q_OBJECT

public :

    ExParametersGeneratorWidgetW(QString name ="");
    ~ExParametersGeneratorWidgetW() override;

    ExParametersGeneratorWidgetW *init_widget(std::unordered_map<QStringView, ExBaseW*> *inputUiElements, bool enabled = true);

    void update_from_arg(const Arg &arg) override;
    Arg convert_to_arg() const override;
    void update_from_resources() override;
    void update_from_components() override;

private:

    std::pair<QWidget*,ExBaseW*> gen_ui_element(UiType uiType);
    void add_ui_element_from_dialog(UiType uiType, int order);
    void add_ui_element_from_arg(Arg arg);

private:

    QComboBox m_cbTypeParam;
    QPushButton m_pbAddParam;
    QPushButton m_pbMoveUp;
    QPushButton m_pbMoveDown;
    QPushButton m_pbRemoveParam;
    ui::ListWidget m_lwParameters;

//    int currentId = 0;
    QVector<QStringView> elementsOrder;
    std::unordered_map<QStringView, ExBaseW*> generatorElements;
    std::unordered_map<QStringView, ExBaseW*> *m_inputUiElements = nullptr;

signals:

    void add_ui_signal(Arg arg);
    void remove_ui_signal(QStringView argName);
    void swap_ui_signal(QStringView arg1Name, QStringView arg2Name);
};

}
