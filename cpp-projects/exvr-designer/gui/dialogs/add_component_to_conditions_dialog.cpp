
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

#include "add_component_to_conditions_dialog.hpp"

// local
#include "experiment/experiment.hpp"
#include "experiment/global_signals.hpp"

using namespace tool;
using namespace tool::ex;

AddComponentToConditionsDialog::AddComponentToConditionsDialog(ComponentKey componentKey){

    // setup ui
    condSelUi.setupUi(this);
    setWindowTitle("Specify details for adding component:");
    setModal(true);
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);

    // retrieve exp and selected component
    auto exp = ExperimentManager::get()->current();
    auto component = exp->get_component(componentKey);
    auto configsName = component->get_configs_name();

    // init top
    auto topL = new QVBoxLayout();
    condSelUi.wTop->setLayout(topL);
    topL->setContentsMargins(2,2,2,2);
    topL->addWidget(laApplyTo = new QLabel(QSL("Component <b>") % component->name() % QSL("</b> of type <b>[") % from_view(Component::get_type_name(component->type)) % QSL("]</b>:")));

    topL->addWidget(ui::W::horizontal_line());

    cbConfigs = new QComboBox();
    cbConfigs->addItems(configsName);
    topL->addWidget(ui::F::gen(ui::L::HB(),
        {ui::W::txt("Parameters to apply: ")}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    topL->addWidget(ui::F::gen(ui::L::HB(),
        {ui::W::txt("Config:"), cbConfigs, ui::W::txt(" Timelines:"), cbFillU = new QCheckBox("update"), cbFillV = new QCheckBox("visibility")}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    auto to = Component::get_timeline_opt(component->type);
    cbFillU->setEnabled(to == Component::TimelineO::Both || to == Component::TimelineO::Update);
    cbFillU->setChecked(cbFillU->isEnabled());
    cbFillV->setEnabled(to == Component::TimelineO::Both || to == Component::TimelineO::Visibility);
    cbFillV->setChecked(cbFillV->isEnabled());

    topL->addWidget(ui::W::horizontal_line());

    // init bottom
    auto botL = new QVBoxLayout();
    condSelUi.wBot->setLayout(botL);
    botL->setContentsMargins(2,2,2,2);
    botL->addWidget(ui::W::horizontal_line());
    botL->addWidget(laApplyTo = new QLabel());

    // update middle
    condSelUi.twRoutines->addTab(new QWidget(), QSL("Routines:"));
    condSelUi.twRoutines->setTabEnabled(0, false);

    QStringList routinesName;
    for(const auto &routine : exp->get_elements_from_type<Routine>()){

        if(routine->isARandomizer){
            continue;
        }

        QStringView conditionStr = routine->conditions.size() > 1 ? QSL("conditions") : QSL("condition");
        routinesName << routine->name() % QSL(" (") % QString::number(routine->conditions.size()) % QSL(" ") % conditionStr % QSL(")");

        auto lwConditions = std::make_unique<ui::ListWidget>();
        condSelUi.twRoutines->addTab(lwConditions.get(), routine->name());
        for(const auto &condition : routine->conditions){

            auto condSelectionCb = new QCheckBox(condition->name);
            condSelectionCb->setObjectName(QSL("Check ") % condition->name);

            auto configSelectionCb = new QComboBox();
            configSelectionCb->addItems(configsName);

            auto fillUpdateSelectionCb     = new QCheckBox("update");
            fillUpdateSelectionCb->setEnabled(cbFillU->isEnabled());
            auto fillVisibilitySelectionCb = new QCheckBox("visibility");
            fillVisibilitySelectionCb->setEnabled(cbFillV->isEnabled());

            if(condition->get_component_from_key(componentKey, false) != nullptr){
                condSelectionCb->setEnabled(false);
                condSelectionCb->setText(condSelectionCb->text() % QSL(" (already inside)"));
            }

            QStringView actionStr = condition->actions.size()    > 1 ? QSL("actions") : QSL("action");
            QStringView nodeStr   = condition->connectors.size() > 1 ? QSL("nodes")   : QSL("node");

            lwConditions->add_widget(ui::F::gen(ui::L::HB(), {
                condSelectionCb, ui::W::txt("Config:", Qt::AlignRight), configSelectionCb, ui::W::txt("Fill timelines:"), fillUpdateSelectionCb, fillVisibilitySelectionCb, ui::W::txt(QSL("(") % QString::number(condition->actions.size()) % QSL(" ") % actionStr % QSL(" ") %
                QString::number(condition->connectors.size()) % QSL(" ") % nodeStr % QSL(")"))},LStretch{false}, LMargins{false}, QFrame::NoFrame));

            connect(condSelectionCb, &QCheckBox::clicked, this, [&]{
                update_selection_number();
            });
        }
        lwConditions->set_margins(2,2,2,2,2);
        conditionsPerRoutinesW.emplace_back(std::make_pair(ElementKey{routine->key()}, std::move(lwConditions)));
    }

    condSelUi.twRoutines->setCurrentIndex(1);
    condSelUi.twRoutines->update();

    condSelUi.lwRoutinesNames->clear();
    condSelUi.lwRoutinesNames->addItems(routinesName);
    condSelUi.lwRoutinesNames->setCurrentRow(0);

    connect(condSelUi.pbCheckAllConds, &QPushButton::clicked, this, [&](){

        if(condSelUi.twRoutines->count() == 1){
            return;
        }

        for(int ii = 1; ii < condSelUi.twRoutines->count(); ++ii){
            auto lwConditions = conditionsPerRoutinesW[ii-1].second.get();
            for(int jj = 0; jj < lwConditions->count(); ++jj){
                auto condSelectionCb = qobject_cast<QCheckBox*>(lwConditions->widget_at(jj)->layout()->itemAt(0)->widget());
                if(condSelectionCb->isEnabled()){
                    condSelectionCb->setChecked(true);
                }
            }
        }
        update_selection_number();
    });
    connect(condSelUi.pbUncheckAllConds, &QPushButton::clicked, this, [&](){

        if(condSelUi.twRoutines->count() == 1){
            return;
        }

        for(int ii = 1; ii < condSelUi.twRoutines->count(); ++ii){
            auto lwConditions = conditionsPerRoutinesW[ii-1].second.get();
            for(int jj = 0; jj < lwConditions->count(); ++jj){
                auto condSelectionCb = qobject_cast<QCheckBox*>(lwConditions->widget_at(jj)->layout()->itemAt(0)->widget());
                if(condSelectionCb->isEnabled()){
                    condSelectionCb->setChecked(false);
                }
            }
        }
        update_selection_number();
    });
    connect(condSelUi.pbCheckRoutineConds, &QPushButton::clicked, this, [&](){

        if(condSelUi.twRoutines->count() == 1){
            return;
        }

        auto id = to_size_t(condSelUi.twRoutines->currentIndex());
        auto lwConditions = conditionsPerRoutinesW[id-1].second.get();
        for(int ii = 0; ii < lwConditions->count(); ++ii){
            auto condSelectionCb = qobject_cast<QCheckBox*>(lwConditions->widget_at(ii)->layout()->itemAt(0)->widget());
            if(condSelectionCb->isEnabled()){
                condSelectionCb->setChecked(true);
            }
        }
        update_selection_number();
    });
    connect(condSelUi.pbUncheckRoutineConds, &QPushButton::clicked, this, [&](){

        if(condSelUi.twRoutines->count() == 1){
            return;
        }

        auto id = to_size_t(condSelUi.twRoutines->currentIndex());
        auto lwConditions = conditionsPerRoutinesW[id-1].second.get();
        for(int ii = 0; ii < lwConditions->count(); ++ii){
            auto condSelectionCb = qobject_cast<QCheckBox*>(lwConditions->widget_at(ii)->layout()->itemAt(0)->widget());
            if(condSelectionCb->isEnabled()){
                condSelectionCb->setChecked(false);
            }
        }
        update_selection_number();
    });

    connect(condSelUi.lwRoutinesNames, &QListWidget::currentRowChanged, this, [&](int row){
        condSelUi.twRoutines->blockSignals(true);
        condSelUi.twRoutines->setCurrentIndex(row+1);
        condSelUi.twRoutines->blockSignals(false);
    });
    connect(condSelUi.twRoutines->tabBar(), &QTabBar::currentChanged, this, [&](int row){
        condSelUi.lwRoutinesNames->blockSignals(true);
        condSelUi.lwRoutinesNames->setCurrentRow(row-1);
        condSelUi.lwRoutinesNames->blockSignals(false);
    });

    connect(condSelUi.pbApplyToSelection, &QPushButton::clicked, this, [&]{

        if(condSelUi.twRoutines->count() == 1){
            return;
        }

        for(int ii = 1; ii < condSelUi.twRoutines->count(); ++ii){
            auto lwConditions = conditionsPerRoutinesW[ii-1].second.get();
            for(int jj = 0; jj < lwConditions->count(); ++jj){

                auto routineL = lwConditions->widget_at(jj)->layout();

                auto condSelectionCb = qobject_cast<QCheckBox*>(routineL->itemAt(0)->widget());
                if(condSelectionCb->isChecked()){
                    auto configSelCb = qobject_cast<QComboBox*>(routineL->itemAt(2)->widget());
                    configSelCb->setCurrentIndex(cbConfigs->currentIndex());
                    auto fillUCb     = qobject_cast<QCheckBox*>(routineL->itemAt(4)->widget());
                    fillUCb->setChecked(cbFillU->isChecked());
                    auto fillVCb     = qobject_cast<QCheckBox*>(routineL->itemAt(5)->widget());
                    fillVCb->setChecked(cbFillV->isChecked());
                }
            }
        }
    });

    connect(condSelUi.validateButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(condSelUi.validateButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(condSelUi.validateButtonBox, &QDialogButtonBox::accepted, this, [&,component]{

        std::vector<std::tuple<ElementKey,ConditionKey,ConfigKey,bool, bool>> addComponentToConditions;

        for(size_t ii = 0; ii < conditionsPerRoutinesW.size(); ++ii){

            auto routineKey   = conditionsPerRoutinesW[ii].first;
            auto routine      = ExperimentManager::get()->current()->get_routine(routineKey);
            auto lwConditions = conditionsPerRoutinesW[ii].second.get();

            for(int jj = 0; jj < lwConditions->count(); ++jj){

                auto routineL = lwConditions->widget_at(jj)->layout();
                auto condSelectionCb = qobject_cast<QCheckBox*>(routineL->itemAt(0)->widget());
                if(condSelectionCb->isChecked()){
                    auto configSelCb = qobject_cast<QComboBox*>(routineL->itemAt(2)->widget());
                    auto fillUCb     = qobject_cast<QCheckBox*>(routineL->itemAt(4)->widget());
                    auto fillVCb     = qobject_cast<QCheckBox*>(routineL->itemAt(5)->widget());

                    addComponentToConditions.emplace_back(std::make_tuple(
                        routineKey,
                        ConditionKey{routine->conditions[to_size_t(jj)]->key()},
                        ConfigKey{component->configs[configSelCb->currentIndex()]->key()},
                        fillUCb->isChecked(),
                        fillVCb->isChecked()
                    ));
                }
            }
        }

        emit GSignals::get()->insert_action_to_signal(
            {component->key()},
            std::move(addComponentToConditions)
        );

    });

    update_selection_number();
}

void AddComponentToConditionsDialog::update_selection_number(){

    size_t countR = 0;
    size_t totalCountC = 0;
    for(size_t ii = 0; ii < conditionsPerRoutinesW.size(); ++ii){
        size_t countC = 0;
        auto lw = conditionsPerRoutinesW[ii].second.get();
        for(int ii = 0; ii < lw->count(); ++ii){
            auto condSelectionCb = lw->widget_at(ii)->layout()->itemAt(0)->widget();
            if(qobject_cast<QCheckBox*>(condSelectionCb)->isChecked()){
                ++countC;
            }
        }
        if(countC > 0){
            ++countR;
        }
        totalCountC += countC;
    }

    QStringView r = countR > 1 ? QSL("routines") : QSL("routine");
    QStringView c = totalCountC > 1 ? QSL("conditions") : QSL("condition");

    laApplyTo->setText(QSL("Component will be added to <b>") % QString::number(totalCountC) %
                       QSL("</b> ") % c % QSL(" from <b>") % QString::number(countR) % QSL("</b> ") % r % QSL("."));
    condSelUi.validateButtonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(totalCountC > 0);
}
