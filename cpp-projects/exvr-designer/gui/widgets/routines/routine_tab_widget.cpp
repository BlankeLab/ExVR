
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

#include "routine_tab_widget.hpp"

// Qt
#include <QStringBuilder>

// base
#include "utility/benchmark.hpp"

// local
#include "experiment/global_signals.hpp"

using namespace tool::ex;

RoutineTabW::RoutineTabW(Routine *routine) : m_routineKey(ElementKey{routine->key()}) {

    setObjectName(QSL("Routine tab widget (") % QString::number(m_routineKey.v) % QSL(")"));
    setContentsMargins(2,2,2,2);

    tabBar()->setStyleSheet(
        "QTabBar::tab:selected:hover {\
            color: rgb(0,0,150);\
        }\
        QTabBar::tab:!selected:hover{\
            color: rgb(0,0,150);\
        }\
        QTabBar::tab:selected{\
            color: rgb(0,0,150);\
        }\
        QTabBar::tab:!selected{\
            color: rgb(0,0,0);\
        }\
        ");


    connect(this, &RoutineTabW::currentChanged, this, [&](int id){
        GSignals::get()->routine_condition_selected_signal(m_routineKey, RowId{id});
    });
}

ConditionW *RoutineTabW::current_condition_widget(){
    int id = currentIndex();
    if(id > -1){
        return qobject_cast<ConditionW*>(widget(id));
    }
    return nullptr;
}

void RoutineTabW::reset(){

    BlockSignalsGuard guard;

    while(count() > 0){
        qobject_cast<ConditionW*>(widget(0))->clean();
        delete widget(0);
    }
}

void RoutineTabW::close_all_windows(){
    for(int ii = 0; ii < count(); ++ii){
        qobject_cast<ConditionW*>(widget(ii))->close_all_connectors_windows();
    }
}

void RoutineTabW::update_from_routine(GUI *gui, Routine *routine){

    if(routine->isARandomizer){
        reset();
        setEnabled(false);
        return;
    }else{
        setEnabled(true);
    }

    bool display = false;
    Bench::start("RoutineTabW update_from_routine 1"sv, display);

    // find conditions to remove
    std::vector<size_t> idCondsToRemove;
    std::vector<bool> conditionsMask(routine->conditions.size(), true);
    for(int ii = count()-1; ii >= 0; --ii){

        auto currentTabConditionKey = qobject_cast<ConditionW*>(widget(ii))->condition_key();
        bool found = false;
        size_t idFound = 0;
        for(size_t jj = 0; jj < routine->conditions.size(); ++jj){
            if(currentTabConditionKey.v == routine->conditions[jj]->key()){
                found = true;
                idFound = jj;
                break;
            }
        }

        if(!found){
            idCondsToRemove.emplace_back(ii);
        }else{
            conditionsMask[idFound] = false;
        }
    }

    Bench::stop();
    Bench::start("RoutineTabW update_from_routine 2"sv, display);

    // remove unused conditions
    for(const auto id : idCondsToRemove){
        delete widget(static_cast<int>(id));
    }

    // add new conditions
    for(size_t ii = 0; ii < conditionsMask.size(); ++ii){
        if(!conditionsMask[ii]){
            continue;
        }
        addTab(new ConditionW(ElementKey{routine->key()}, routine->conditions[ii].get()), "");
    }

    Bench::stop();
    Bench::start("RoutineTabW update_from_routine 3"sv, display);

    // reorder
    for(int ii = 0; ii < to_int(routine->conditions.size()); ++ii){
        for(int jj = 0; jj < count(); ++jj){
            if(qobject_cast<ConditionW*>(widget(jj))->condition_key().v == routine->conditions[to_size_t(ii)]->key()){
                if(ii != jj){
                    tabBar()->moveTab(jj,ii);
                }
                break;
            }
        }
    }

    // select current
    for(size_t ii = 0; ii < routine->conditions.size(); ++ii){
        if(routine->conditions[ii]->selected){
            setCurrentIndex(to_int(ii));
            break;
        }
    }

    Bench::stop();
    Bench::start("RoutineTabW update_from_routine 4"sv, display);

    // update condition widgets
    for(int ii = 0; ii < count(); ++ii){
        auto condition = routine->conditions[to_size_t(ii)].get();
         QString txt = condition->name % QSL(" (") %  QString::number(condition->actions.size()) % QSL("/") % QString::number(condition->connectors.size()) % QSL(")");
        if(tabText(ii) != txt){
            setTabText(ii, std::move(txt));
        }
        if(ii == currentIndex()){
            qobject_cast<ConditionW*>(widget(ii))->update_from_condition(gui, condition);
        }
    }

    Bench::stop();
}

void RoutineTabW::update_from_connector_info(ConditionKey conditionKey, ConnectorKey connectorKey, QStringView id, QStringView value){

    for(int ii = 0; ii < count(); ++ii){
        auto cw = condition_widget(RowId{ii});
        if(cw->condition_key() == conditionKey){
            cw->update_from_connector_info(connectorKey, id, value);
            break;
        }
    }
}

ConditionW *RoutineTabW::condition_widget(RowId  tabId){
    return qobject_cast<ConditionW*>(widget(tabId.v));
}

#include "moc_routine_tab_widget.cpp"
