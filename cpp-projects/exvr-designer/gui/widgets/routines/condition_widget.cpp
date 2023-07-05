
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

#include "condition_widget.hpp"

// std
#include <map>

// Qt
#include <QMenuBar>

// base
#include "utility/benchmark.hpp"

// local
#include "experiment/global_signals.hpp"

using namespace tool::ex;

ScaleSpinBox::ScaleSpinBox(QWidget *parent) : ui::NoScrollFocusWidget<QDoubleSpinBox>(parent){
    Q_UNUSED(parent)
    setDecimals(2);
    setMinimum(0.01);
    setMaximum(1000.);
    setValue(1.);
}

void ScaleSpinBox::stepBy(int steps){
    switch(steps) {
    case 1 :    setValue(value()*10.);
        break;
    case -1 :   setValue(value()*0.1);
        break;
    default:    QDoubleSpinBox::stepBy(steps);
        break;
    }
}

ConditionW::ConditionW(ElementKey routineKey, Condition *condition) : m_routineKey(routineKey), m_conditionKey(ConditionKey{condition->key()}){

    setAcceptDrops(true);

    m_ui.setupUi(this);    


    lBoth = ui::L::HB();
    lBoth->setContentsMargins(2,2,2,2);
    m_ui.tBoth->setLayout(lBoth);
    m_ui.tBoth->setVisible(false);

    // add connections widget
    lConnections = ui::L::VB();
    lConnections->setContentsMargins(2,2,2,2);
    m_connectionsW = std::make_unique<ConnectionsW>(routine_key(), condition_key());
    lConnections->addWidget(m_connectionsW.get());

    m_ui.tConnections->setLayout(lConnections);

    // add actions list
    lActions = ui::L::VB();
    lActions->setContentsMargins(2,2,2,2);
    lActions->addWidget(&m_actionsListW);
    m_ui.tActions->setLayout(lActions);

    for(const auto &action : condition->actions){        

        for(const auto &interval : action->timelineUpdate->intervals){
            if(condition->duration.v < interval.end.v){
                condition->duration.v = interval.end.v;
            }
        }
        for(const auto &interval : action->timelineVisibility->intervals){
            if(condition->duration.v < interval.end.v){
                condition->duration.v = interval.end.v;
            }
        }
    }



    int idDuration =  m_ui.hlLeftOptions->indexOf(m_ui.dsbDuration);
    delete m_ui.hlLeftOptions->itemAt(idDuration)->widget();
    dsbDuration = new OnlyFocusScrollSpinBox();
    dsbDuration->setObjectName("duration");
    dsbDuration->setMinimumWidth(75);
    dsbDuration->setMinimumHeight(18);
    dsbDuration->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred));
    m_ui.hlLeftOptions->insertWidget(idDuration,dsbDuration);
    dsbDuration->setMinimum(0);
    dsbDuration->setMaximum(100000);
    if(dsbDuration->value() < condition->duration.v){
        dsbDuration->setValue(std::ceil(condition->duration.v));
    }

    int idScale =  m_ui.hlLeftOptions->indexOf(m_ui.dsbScale);
    delete m_ui.hlLeftOptions->itemAt(idScale)->widget();
    dsbScale = new ScaleSpinBox();
    dsbScale->setObjectName("scale");
    dsbScale->setMinimumWidth(75);
    dsbScale->setMinimumHeight(18);    
    dsbScale->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred));
    m_ui.hlLeftOptions->insertWidget(idScale,dsbScale);
    dsbScale->setMinimum(0.01);
    dsbScale->setMaximum(1000);
    dsbScale->setValue(condition->scale);
    dsbDuration->setSingleStep(dsbScale->value());

    int idUiFactorSize =  m_ui.hlLeftOptions->indexOf(m_ui.dsbUiFactorSize);
    delete m_ui.hlLeftOptions->itemAt(idUiFactorSize)->widget();
    dsbUiFactorSize = new OnlyFocusScrollSpinBox();
    dsbUiFactorSize->setObjectName("ui_factor_size");
    dsbUiFactorSize->setMinimumWidth(75);
    dsbUiFactorSize->setMinimumHeight(18);
    dsbUiFactorSize->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred));
    m_ui.hlLeftOptions->insertWidget(idUiFactorSize,dsbUiFactorSize);
    dsbUiFactorSize->setSingleStep(0.1);
    dsbUiFactorSize->setMinimum(0.5);
    dsbUiFactorSize->setMaximum(10);
    dsbUiFactorSize->setValue(condition->uiFactorSize);

    // connections
    connect(dsbScale, QOverload<double>::of(&ScaleSpinBox::valueChanged),     this, [&](double){
        if(m_isUpdating){
            return;
        }
        update_timeline();
    });
    connect(dsbUiFactorSize, QOverload<double>::of(&ScaleSpinBox::valueChanged),     this, [&](double){
        if(m_isUpdating){
            return;
        }
        update_timeline();
    });
    connect(dsbDuration, &ScaleSpinBox::editingFinished,     this, [&](){
        if(m_isUpdating){
            return;
        }
        update_timeline();
    });

    using CW  = ConditionW;
    connect(m_ui.tabCondition, &QTabWidget::currentChanged, this, &CW::update_ui_from_current_tab);
    connect(m_ui.pbFillActions,         &QPushButton::clicked, this, [&]{GSignals::get()->fill_actions_signal(routine_key(), condition_key());});
    connect(m_ui.pbCleanActions,        &QPushButton::clicked, this, [&]{GSignals::get()->clean_actions_signal(routine_key(), condition_key());});
    connect(m_ui.pbRemoveActions,       &QPushButton::clicked, this, [&]{GSignals::get()->delete_actions_signal(routine_key(), condition_key());});
    connect(m_ui.pbCopyCondTo,          &QPushButton::clicked, this, [&]{GSignals::get()->copy_condition_to_signal(routine_key(), condition_key());});
    connect(m_ui.pbRemoveConnections,   &QPushButton::clicked, this, [&]{GSignals::get()->delete_connections_signal(routine_key(), condition_key());});

    connect(m_ui.pbRemoveConnections,   &QPushButton::clicked, this, [&]{GSignals::get()->delete_connections_signal(routine_key(), condition_key());});
    connect(m_ui.pbMinimizeExtendAll,   &QPushButton::clicked, this, [&]{
        m_minimzed = !m_minimzed;
        for(int ii = 0; ii < m_actionsListW.count(); ++ii){
            auto actionW = qobject_cast<ActionW*>(m_actionsListW.widget_at(ii));
            actionW->set_minimized_state(m_minimzed);
        }
    });

    separate_panels(true);
}

void ConditionW::update_from_condition(GUI *gui, Condition *condition){

    m_isUpdating = true;

    bool display = false;
    Bench::start("ConditionW update_from_condition 1"sv, display);

        separate_panels(gui->toggleActionsConnectionsSep);

        // connections
        m_connectionsW->update_from_condition(condition);
    Bench::stop();

    Bench::start("ConditionW update_from_condition 2"sv, display);

        // actions
        // # update scale/length/size
        if(dsbScale->value() != condition->scale){
            dsbScale->setValue(condition->scale);
        }
        if(dsbUiFactorSize->value() != condition->uiFactorSize){
            dsbUiFactorSize->setValue(condition->uiFactorSize);
        }
        if(dsbDuration->value() != condition->duration.v){
            dsbDuration->setValue(condition->duration.v);
        }
        dsbDuration->setSingleStep(dsbScale->value());

        // # remove inused actions
        std::map<int,bool> mask;
        for(int ii = m_actionsListW.count()-1; ii >= 0; --ii){

            bool found = false;
            auto actionW = qobject_cast<ActionW*>(m_actionsListW.widget_at(ii));
            for(const auto &action : condition->actions){
                if(action->key() == actionW->action_key().v){
                    found = true;
                    break;
                }
            }
            mask[actionW->action_key().v] = found;
            if(!found){
                delete m_actionsListW.remove_at(ii);
            }
        }

        // # add new actions
        for(const auto &action : condition->actions){
            if(!mask[action->key()]){
                m_actionsListW.add_widget(new ActionW(routine_key(), condition_key(), action.get()));
            }
        }

        // reorder
        for(int ii = 0; ii < to_int(condition->actions.size()); ++ii){
            for(int jj = 0; jj < to_int(m_actionsListW.count()); ++jj){
                if(qobject_cast<ActionW*>(m_actionsListW.widget_at(jj))->action_key().v == condition->actions[to_size_t(ii)]->key()){
                    if(ii != jj){
                        m_actionsListW.move_from_to(jj,ii);
                    }
                    break;
                }
            }
        }

    Bench::stop();
    Bench::start("ConditionW update_from_condition 3"sv, display);

        // update actions
        for(int ii = 0; ii< m_actionsListW.count(); ++ii){
            qobject_cast<ActionW*>(m_actionsListW.widget_at(ii))->update_from_action(ii, condition->actions[to_size_t(ii)].get(), condition->scale, condition->uiFactorSize, condition->duration);
        }

        update_ui_from_current_tab(m_ui.tabCondition->currentIndex());
        Bench::stop();

    m_isUpdating = false;
}

void ConditionW::update_from_connector_info(ConnectorKey connectorKey, QStringView id, QStringView value){
    if(auto connector = connections_widget()->connector(connectorKey); connector != nullptr){
        connector->update_with_info(id, value);
    }
}

void ConditionW::clean(){
    BlockSignalsGuard guard;
    m_connectionsW = nullptr;
    m_actionsListW.remove_all();
}

void ConditionW::close_all_connectors_windows(){
    m_connectionsW->close_all_connectors_windows();
}

ConnectionsW *ConditionW::connections_widget(){
    return m_connectionsW.get();
}

void ConditionW::separate_panels(bool state){

    int count = m_ui.tabCondition->count();
    if(state && count == 1){
        return;
    }
    if(!state && count == 2){
        return;
    }

    while(m_ui.tabCondition->count() > 0){
        m_ui.tabCondition->removeTab(0);
    }

    if(state){

        lBoth->addWidget(&m_actionsListW);
        lBoth->addWidget(m_connectionsW.get());
        lBoth->setStretch(0,1);
        lBoth->setStretch(1,1);
        m_ui.tabCondition->addTab(m_ui.tBoth, QSL("Actions/Connections"));
    }else{
        lActions->addWidget(&m_actionsListW);
        lConnections->addWidget(m_connectionsW.get());
        m_ui.tabCondition->addTab(m_ui.tActions, QSL("Actions"));
        m_ui.tabCondition->addTab(m_ui.tConnections, QSL("Connections"));
    }
}

void ConditionW::dragEnterEvent(QDragEnterEvent *event){
    event->acceptProposedAction();
}

void ConditionW::dropEvent(QDropEvent *event){

    const QMimeData *data = event->mimeData();
    auto split = data->text().split("_");
    if(split[0] == QSL("e")){
        GSignals::get()->add_action_signal(routine_key(), condition_key(), ComponentKey{split[1].toInt()}, {}, true, true);
    }else if(split[0] == QSL("action")){
        // ...
    }
}

void ConditionW::mousePressEvent(QMouseEvent *event){
    Q_UNUSED(event)
}

void ConditionW::resizeEvent(QResizeEvent *event){
    Q_UNUSED(event)
}

void ConditionW::update_ui_from_current_tab(int index){

    if(index == 0){
        m_ui.pbFillActions->setVisible(true);
        m_ui.pbCleanActions->setVisible(true);
        m_ui.pbRemoveActions->setVisible(true);
        dsbDuration->setVisible(true);
        dsbUiFactorSize->setVisible(true);
        m_ui.laMax->setVisible(true);
        m_ui.laSize->setVisible(true);
        m_ui.laScale->setVisible(true);
        dsbScale->setVisible(true);
        m_ui.pbRemoveConnections->setVisible(false);
    }else{
        m_ui.pbFillActions->setVisible(false);
        m_ui.pbCleanActions->setVisible(false);
        m_ui.pbRemoveActions->setVisible(false);
        m_ui.laMax->setVisible(false);
        m_ui.laSize->setVisible(false);
        m_ui.laScale->setVisible(false);
        dsbDuration->setVisible(false);
        dsbUiFactorSize->setVisible(false);
        dsbScale->setVisible(false);
        m_ui.pbRemoveConnections->setVisible(true);
    }
}

void ConditionW::update_timeline(){

    dsbDuration->blockSignals(true);
    dsbScale->blockSignals(true);
    dsbUiFactorSize->blockSignals(true);

    auto duration       = dsbDuration->value();
    auto scale          = dsbScale->value();
    auto uiFactorSize   = dsbUiFactorSize->value();

    auto nbSteps = duration/scale;
    while(nbSteps > 1000.){
        scale *= 10.;
        nbSteps = duration/scale;
        QtLogger::message(QSL("Timeline scale increased (steps count > 1000)"));
    }

    dsbScale->setValue(scale);
    dsbDuration->setSingleStep(scale);

    dsbDuration->blockSignals(false);
    dsbScale->blockSignals(false);
    dsbUiFactorSize->blockSignals(false);

    emit GSignals::get()->update_timeline_signal(routine_key(), condition_key(), duration, scale, uiFactorSize);
}


#include "moc_condition_widget.cpp"
