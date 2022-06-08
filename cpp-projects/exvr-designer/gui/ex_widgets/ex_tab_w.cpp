
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

#include "ex_tab_w.hpp"

using namespace tool::ex;


TabUiW::TabUiW(){

    auto vb = ui::L::VB();
    setLayout(vb);
     vb->addWidget(ui::F::gen(ui::L::VB(), {
        name = ui::W::txt("Default"),
        ui::F::gen(ui::L::HB(),{pbAddTab = new QPushButton("Insert tab"), pbRemoveTab = new QPushButton("Remove current tab")}, LStretch{true}, LMargins{false},QFrame::NoFrame),
        tab = new QTabWidget()},
        LStretch{true}, LMargins{true}, QFrame::Box
    ));

    connect(pbAddTab,    &QPushButton::clicked, this, [&]{
        emit ask_new_tab_signal(tab->currentIndex());
    });
    connect(pbRemoveTab,    &QPushButton::clicked, this, [&]{
        emit ask_remove_tab_signal(tab->currentIndex());
    });
    connect(tab->tabBar(), &QTabBar::tabMoved, this, [&](int from, int to){
        update_tab_names();
        emit tab_moved_signal(from, to);
    });

    tab->setMovable(true);
    pbRemoveTab->setEnabled(false);
}

int TabUiW::current_tab_id(){
    return tab->currentIndex();
}

void TabUiW::insert_tab(int index, QWidget *w){
    tab->insertTab(index, w, "");
    pbRemoveTab->setEnabled(true);
    pbAddTab->setEnabled(tab->count() < maxTabNumber);
    update_tab_names();
}

void TabUiW::remove_tab(int index){
    tab->removeTab(index);
    pbRemoveTab->setEnabled(tab->count() > 0);
    pbAddTab->setEnabled(tab->count() < maxTabNumber);
    update_tab_names();
}

void TabUiW::set_title(const QString &title){
    name->setText(title);
}

void TabUiW::set_tab_position(QTabWidget::TabPosition tabPosition){
    tab->setTabPosition(tabPosition);
}

void TabUiW::set_max_tab_number(int maxTabNumber){
    this->maxTabNumber = maxTabNumber;
}

void TabUiW::update_tab_names(){
    for(int ii = 0; ii < tab->count(); ++ii){
        tab->setTabText(ii, QString::number(ii+1));
    }
}
