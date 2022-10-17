
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

#include "action_widget.hpp"

// Qt
#include <QDebug>
#include <QMimeData>
#include <QDrag>

// base
#include "utility/types.hpp"

// qt-utility
#include "gui/widgets/custom_combobox_w.hpp"

// local
#include "data/component.hpp"
#include "data/config.hpp"
#include "experiment/global_signals.hpp"

using namespace tool;
using namespace tool::ex;

ActionW::ActionW(ElementKey routineKey, ConditionKey conditionKey, Action *action) :
      m_routineKey(routineKey), m_conditionKey(conditionKey), m_actionKey(ActionKey{action->key()}){

    m_ui.setupUi(this);

    auto icon = QIcon(Component::get_icon_path(action->component->type));
    m_ui.tbIcon->setIconSize(QSize(70,70));
    m_ui.tbIcon->setIcon(icon);

    m_ui.pbMinimizedIcon->setIconSize(QSize(20,20));
    m_ui.pbMinimizedIcon->setIcon(icon);

    m_ui.cbConfig->setFocusPolicy(Qt::StrongFocus);
    delete m_ui.cbConfig;
    m_ui.cbConfig = new ui::NoScrollFocusWidget<QComboBox>(nullptr);
    m_ui.hlTop->insertWidget(6, m_ui.cbConfig);



    setObjectName(QSL("action"));
    setStyleSheet(QSL("QWidget[objectName=\"action\"] {background-color:white;}"));

    // generate widgets
    auto opt = Component::get_timeline_opt(action->component->type);
    if(opt == Component::TimelineO::Update || opt == Component::TimelineO::Both){
        m_timelineUpdateW = new TimelineW(routine_key(), condition_key(), action_key(), action->timelineUpdate.get(), opt == Component::TimelineO::Update, true);
        m_ui.vlTimeline->addWidget(m_timelineUpdateW);
    }    

    if(opt == Component::TimelineO::Visibility || opt == Component::TimelineO::Both){
        m_timelineVisibilityW = new TimelineW(routine_key(), condition_key(), action_key(), action->timelineVisibility.get(), true, false);
        m_ui.vlTimeline->addWidget(m_timelineVisibilityW);
    }

    if(opt == Component::TimelineO::None){
        m_ui.pbFill->setEnabled(false);
        m_ui.pbClean->setEnabled(false);
        m_ui.cbVisibility->setEnabled(false);
        m_ui.cbUpdate->setEnabled(false);
        m_ui.laApplyTo->setEnabled(false);
    }

    // others connections
    connect(m_ui.pbUp, &QPushButton::clicked, this, [&](){
        emit GSignals::get()->move_action_up_signal(routine_key(), condition_key(),action_key());
    });
    connect(m_ui.pbDown, &QPushButton::clicked, this, [&](){
        emit GSignals::get()->move_action_down_signal(routine_key(), condition_key(),action_key());
    });
    connect(m_ui.pbUpTop, &QPushButton::clicked, this, [&](){
        emit GSignals::get()->move_action_up_signal(routine_key(), condition_key(),action_key());
    });
    connect(m_ui.pbDownTop, &QPushButton::clicked, this, [&](){
        emit GSignals::get()->move_action_down_signal(routine_key(), condition_key(),action_key());
    });
    connect(m_ui.cbConfig, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        emit GSignals::get()->select_action_config_signal(routine_key(), condition_key(), action_key(), RowId{index});
    });
    connect(m_ui.pbDelete, &QPushButton::clicked, this, [=]{
        emit GSignals::get()->remove_action_signal(routine_key(), condition_key(),action_key());
    });
    connect(m_ui.pbFill, &QPushButton::clicked, this, [=]{

        bool update = false, visibility = false;
        if(m_ui.cbUpdate->isChecked()     && (opt == Component::TimelineO::Update || opt == Component::TimelineO::Both)){
            update = true;
        }
        if(m_ui.cbVisibility->isChecked() && (opt == Component::TimelineO::Visibility || opt == Component::TimelineO::Both)){
            visibility = true;
        }
        emit GSignals::get()->fill_action_signal(routine_key(), condition_key(),action_key(), update, visibility);
    });
    connect(m_ui.pbClean, &QPushButton::clicked, this, [=]{
        bool update = false, visibility = false;
        if(m_ui.cbUpdate->isChecked()     && (opt == Component::TimelineO::Update || opt == Component::TimelineO::Both)){
            update = true;
        }
        if(m_ui.cbVisibility->isChecked() && (opt == Component::TimelineO::Visibility || opt == Component::TimelineO::Both)){
            visibility = true;
        }
        emit GSignals::get()->clean_action_signal(routine_key(), condition_key(),action_key(), update, visibility);
    });
    connect(m_ui.pbMinimizeExtend, &QPushButton::clicked, this, [=]{
        set_minimized_state(!m_minimized);
    });
    m_ui.pbMinimizedIcon->setVisible(false);
    m_ui.pbUpTop->setVisible(false);
    m_ui.pbDownTop->setVisible(false);

    // set stretch
    m_ui.hlTop->setStretch(0, 1);
    m_ui.hlTop->setStretch(1, 1);
    m_ui.hlTop->setStretch(2, 1);
    m_ui.hlTop->setStretch(3, 1);
    m_ui.hlTop->setStretch(4, 1);
    m_ui.hlTop->setStretch(5, 1);
    m_ui.hlTop->setStretch(6, 80);
    m_ui.hlTop->setStretch(7, 80);
    m_ui.hlTop->setStretch(8, 1);
    m_ui.hlTop->setStretch(9, 1);
    m_ui.hlTop->setStretch(10, 1);
    m_ui.hlTop->setStretch(11, 1);
    m_ui.hlTop->setStretch(12, 1);
    m_ui.hlTop->setStretch(13, 1);
    m_ui.hlTop->setStretch(14, 1);
    m_ui.hlTop->setStretch(15, 1);


//    1,1,1,1,5,20,1,20,1,1,1,1,1,1,1,1

}

void ActionW::mousePressEvent(QMouseEvent *event){

    // left click
    if(event->button()== Qt::MouseButton::LeftButton){
        QPixmap pixmap = m_ui.tbIcon->icon().pixmap(QSize(22, 22), QIcon::Normal, QIcon::On);
        QMimeData *mimeData = new QMimeData();
        mimeData->setText(QSL("action_") % QString::number(action_key().v));
        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap);
        drag->setHotSpot(QPoint(drag->pixmap().width()/2,drag->pixmap().height()/2));
        drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
    }
}

void ActionW::set_minimized_state(bool state){
    m_minimized = state;
    m_ui.pbMinimizeExtend->setText(m_minimized ? QSL("+") : QSL("-"));
    m_ui.wBottom->setVisible(!m_minimized);
    m_ui.pbUpTop->setVisible(m_minimized);
    m_ui.pbDownTop->setVisible(m_minimized);
    m_ui.pbMinimizedIcon->setVisible(m_minimized);
    m_ui.pbFill->setVisible(!m_minimized);
    m_ui.pbClean->setVisible(!m_minimized);
    m_ui.cbUpdate->setVisible(!m_minimized);
    m_ui.cbVisibility->setVisible(!m_minimized);
    m_ui.laApplyTo->setVisible(!m_minimized);
}

void ActionW::update_from_action(int id, Action *action, qreal scale, qreal factorSize, SecondsTS max){

    m_ui.laId->setText(QString::number(id) % QSL("#"));

    // init config combobox
    m_ui.cbConfig->clear();
    m_ui.cbConfig->addItems(action->component->get_configs_name());

    for(size_t ii = 0; ii < action->component->configs.size(); ++ii){
        if(action->config->key() == action->component->configs[ii]->key()){
            m_ui.cbConfig->setCurrentIndex(static_cast<int>(ii));
            break;
        }
    }

    m_ui.laName->setText(QSL("<b>") %  action->component->name() % QSL("</b>"));

    // update timelines
    auto opt = Component::get_timeline_opt(action->component->type);
    if(opt == Component::TimelineO::Update || opt == Component::TimelineO::Both){
        m_timelineUpdateW->update_from_timeline(action->timelineUpdate.get(), scale, factorSize, max);
    }
    if(opt == Component::TimelineO::Visibility || opt == Component::TimelineO::Both){
        m_timelineVisibilityW->update_from_timeline(action->timelineVisibility.get(), scale, factorSize, max);
    }

}
#include "moc_action_widget.cpp"
