
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
#include "gui/ex_widgets/ex_item_w.hpp"

namespace tool::ex {

class TabUiW : public QWidget{
Q_OBJECT
public:

    TabUiW();

    int current_tab_id();
    void insert_tab(int index, QWidget *w);
    void remove_tab(int index);
    void set_title(const QString &title);
    void set_tab_position(QTabWidget::TabPosition tabPosition);
    void set_max_tab_number(int maxTabNumber);

signals:

    void ask_new_tab_signal(int index);
    void ask_remove_tab_signal(int index);
    void tab_moved_signal(int from, int to);

private:

    void update_tab_names();

    QLabel *name = nullptr;
    QPushButton *pbAddTab = nullptr;
    QPushButton *pbRemoveTab = nullptr;
    QTabWidget *tab = nullptr;

    QString baseName = "Tab_";
    int maxTabNumber = 10;
};


template <typename T>
class ExTabW : public ExItemW<TabUiW>{
    static_assert(std::is_base_of<ExBaseW, T>::value, "T must derive from ExBaseW");
public:

    ExTabW(QString name ="") : ExItemW<TabUiW>(UiType::Tab, name){

        connect(w.get(), &TabUiW::ask_new_tab_signal,this, [&](int index){
            add_tab(index+1);
            trigger_ui_change();
        });
        connect(w.get(), &TabUiW::ask_remove_tab_signal, this, [&](int index){
            if(index != -1){
                w->remove_tab(index);
                widgets.erase(widgets.begin() + index);                
                trigger_ui_change();
            }
        });

        connect(w.get(), &TabUiW::tab_moved_signal, this, [&](int from, int to){
            std::swap(widgets[from], widgets[to]);
            trigger_ui_change();
        });
    }


    ExTabW *init_widget(QString tabTitle, std_v1<std::any> initParameters, int maxTabNumber, QTabWidget::TabPosition tabPosition = QTabWidget::TabPosition::North, bool enabled = true){

        w->set_title(tabTitle);
        w->set_max_tab_number(maxTabNumber);
        m_initParameters = std::move(initParameters);

        w->set_tab_position(tabPosition);
        w->setEnabled(enabled);

        return this;
    }

    virtual void update_from_arg(const Arg &arg) override{

        ExItemW::update_from_arg(arg);

        w->blockSignals(true);

        if(arg.value().length() != 0){
            for(auto value : arg.split_value(QSL("[#T#]"))){
                Arg arg;
                arg.init_from(std::move(value));
                add_tab(static_cast<int>(widgets.size()))->update_from_arg(arg);
            }
        }

        w->blockSignals(false);
    }

    virtual Arg convert_to_arg() const override{

        Arg arg = ExBaseW::convert_to_arg();

        std::vector<Arg> args;
        for(const auto& w : widgets){
            args.emplace_back(w->convert_to_arg());
        }
        arg.init_from_args(std::move(args), "[#T#]", UnityType::System_string);

        return arg;
    }

    virtual void set_as_generator() override{
        for(auto &w : widgets){
            w->set_as_generator();
        }
    }


private:


    ExBaseW *add_tab(int index){

        // create item
        widgets.insert(widgets.begin() + index, std::make_unique<T>(QSL("tab")));

        auto widget = dynamic_cast<T*>(widgets[index].get());

        // init sub connections
        auto baseW = dynamic_cast<ExBaseW*>(widget);
        connect(baseW, &ExBaseW::ui_change_signal, this, [&]{
            trigger_ui_change();
        });

        // init widget
        widget->init_widget_from_any_array(m_initParameters);

        // add tab
        QWidget *subW = new QWidget();
        auto l = ui::L::VB();
        subW->setLayout(l);
        l->addWidget(widget->w.get());
        l->addStretch();

        w->insert_tab(index, subW);

        return baseW;
    }

public:
    std_v1<std::unique_ptr<T>> widgets;

private :
    std_v1<std::any> m_initParameters;
};

}
