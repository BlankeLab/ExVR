
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

#include "copy_to_conditions_dialog.hpp"

// Qt
#include <QStringList>

using namespace tool;
using namespace tool::ex;

CopyToConditionDialog::CopyToConditionDialog(){

    ui.setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    setModal(true);   

    connect(ui.buttonBox, &QDialogButtonBox::accepted, this, [&]{

        std_v1<std::pair<ElementKey,ConditionKey>> conditionsToBeEcrased;
        for(size_t ii = 0; ii < conditionsPerRoutines.size(); ++ii){
            auto routine = conditionsPerRoutines[ii].first;
            auto lw = conditionsPerRoutines[ii].second.get();
            for(int ii = 0; ii < lw->count(); ++ii){                
                auto w = lw->widget_at(ii)->layout()->itemAt(0)->widget();
                auto cb = qobject_cast<QCheckBox*>(w);
                if(cb->isChecked()){
                    conditionsToBeEcrased.emplace_back(std::make_pair(
                        ElementKey{routine->key()}, ConditionKey{routine->conditions[to_unsigned(ii)]->key()}));
                }
            }
        }

        const auto copyActions     = ui.rbCopyBoth->isChecked() || ui.rbCopyActions->isChecked();
        const auto copyConnections = ui.rbCopyBoth->isChecked() || ui.rbCopyConnections->isChecked();
        emit copy_to_conditions_signal(m_currentRoutine, m_currentCondition, std::move(conditionsToBeEcrased), copyActions, copyConnections);
    });

    connect(ui.pbCheckAllConds, &QPushButton::clicked, this, [&](){

        auto id = to_unsigned(ui.twRoutines->currentIndex());
        auto lw = conditionsPerRoutines[id-1].second.get();
        for(int ii = 0; ii < lw->count(); ++ii){
            auto w = lw->widget_at(ii)->layout()->itemAt(0)->widget();
            auto cb = qobject_cast<QCheckBox*>(w);
            if(cb->isEnabled()){
                cb->setChecked(true);
            }
        }
        update_ui_from_conditions_checkboxes();
    });

    connect(ui.pbUncheckAllConds, &QPushButton::clicked, this, [&](){

        auto id = to_unsigned(ui.twRoutines->currentIndex());
        auto lw = conditionsPerRoutines[id-1].second.get();
        for(int ii = 0; ii < lw->count(); ++ii){

            auto w = lw->widget_at(ii)->layout()->itemAt(0)->widget();
            auto cb = qobject_cast<QCheckBox*>(w);
            if(cb->isEnabled()){
                cb->setChecked(false);
            }
        }
        update_ui_from_conditions_checkboxes();
    });

    connect(ui.lwRoutinesNames, &QListWidget::currentRowChanged, this, [&](int row){
        ui.twRoutines->blockSignals(true);
        ui.twRoutines->setCurrentIndex(row+1);
        ui.twRoutines->blockSignals(false);
    });
    connect(ui.twRoutines->tabBar(), &QTabBar::currentChanged, this, [&](int row){
        ui.lwRoutinesNames->blockSignals(true);
        ui.lwRoutinesNames->setCurrentRow(row-1);
        ui.lwRoutinesNames->blockSignals(false);
    });

    update_ui_from_conditions_checkboxes();
}

void CopyToConditionDialog::update_ui_from_conditions_checkboxes(){

    size_t count = 0;
    for(size_t ii = 0; ii < conditionsPerRoutines.size(); ++ii){
        auto lw = conditionsPerRoutines[ii].second.get();
        for(int ii = 0; ii < lw->count(); ++ii){
            auto cb = lw->widget_at(ii)->layout()->itemAt(0)->widget();
            if(qobject_cast<QCheckBox*>(cb)->isChecked()){
                ++count;
            }
        }
    }

    if(count > 1){
        ui.laConditionsCount->setText(QSL("Copy will be applied on ") % QString::number(count) % QSL(" conditions."));
    }else{
        ui.laConditionsCount->setText(QSL("Copy will be applied on ") % QString::number(count) % QSL(" condition."));
    }
    ui.buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(count > 0);
}


void CopyToConditionDialog::update_from_data(ElementKey currentRoutineKey, ConditionKey currentConditionKey, std_v1<Routine *> routines){

//    ui.twRoutines->tabBar()->blockSignals(true);
    ui.lwRoutinesNames->blockSignals(true);

    m_currentRoutine    = currentRoutineKey;
    m_currentCondition  = currentConditionKey;

    // clean
    conditionsPerRoutines.clear();
    while(ui.twRoutines->count() > 0){
        ui.twRoutines->removeTab(0);
    }

    // fill conditions
    ui.twRoutines->addTab(new QWidget(), QSL("Routines:"));
    ui.twRoutines->setTabEnabled(0, false);

    QStringList routinesName;
    for(const auto &routine : routines){

        routinesName << routine->name();

        auto lw = std::make_unique<ui::ListWidget>();
        ui.twRoutines->addTab(lw.get(), routine->name());
        for(const auto &condition : routine->conditions){

            auto cb = new QCheckBox(condition->name);
            cb->setObjectName(QSL("Check ") % condition->name);
            if(condition->key() == m_currentCondition.v){
                cb->setEnabled(false);
                cb->setText(cb->text() % QSL(" (condition to copy)"));
            }

            lw->add_widget(ui::F::gen(ui::L::HB(), {
                cb, ui::W::txt(QSL("(A: ") % QString::number(condition->actions.size()) % QSL(", C: ") %
                    QString::number(condition->connectors.size()) %QSL(")"))},LStretch{false}, LMargins{false}, QFrame::NoFrame));

            connect(cb, &QCheckBox::clicked, this, &CopyToConditionDialog::update_ui_from_conditions_checkboxes);
        }
        lw->set_margins(2,2,2,2,2);
        conditionsPerRoutines.emplace_back(std::make_pair(routine, std::move(lw)));
    }

    ui.twRoutines->setCurrentIndex(1);
    ui.twRoutines->update();

    ui.lwRoutinesNames->clear();
    ui.lwRoutinesNames->addItems(routinesName);
    ui.lwRoutinesNames->setCurrentRow(0);

    ui.lwRoutinesNames->blockSignals(false);
//    ui.twRoutines->tabBar()->blockSignals(false);
}

