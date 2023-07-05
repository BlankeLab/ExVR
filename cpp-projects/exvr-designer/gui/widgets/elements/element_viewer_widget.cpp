
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

#include "element_viewer_widget.hpp"

// local
#include "experiment/global_signals.hpp"
#include "gui/settings/display.hpp"

// Qt
#include <QFileDialog>
#include <QSpinBox>
#include <QCollator>
#include <QMessageBox>
#include <QInputDialog>
#include <QTableWidget>
//#include <QRegExpValidator>

using namespace tool::ex;

ElementViewerW::ElementViewerW(QTabWidget *twCurrentElement) :  m_layout(new QHBoxLayout(this)),
    m_routineUI(std::make_unique<Ui::RoutineW>()), m_loopUI(std::make_unique<Ui::LoopW>()), m_isiUI(std::make_unique<Ui::IsiW>()), m_toolBox(twCurrentElement){

    // init ui and styles
    // # rouine
    m_routineUI->setupUi(&m_routineW);
    init_routine_ui();
    // # loop
    m_loopUI->setupUi(&m_loopW);
    init_loop_ui();
    // # ISI
    m_isiUI->setupUi(&m_isiW);
    init_isi_ui();
    // # no selection
    init_no_selection_ui();

    m_toolBox->removeTab(0);
    m_toolBox->insertTab(0, &m_noSelectionW, QSL("No selection"));
}

void ElementViewerW::reset(){
    // ...
}

void ElementViewerW::update_from_current_element(FlowElement *elem){


    if(elem == nullptr){
        m_toolBox->removeTab(0);
        update_no_selection_ui();
        m_toolBox->insertTab(0, &m_noSelectionW, QSL("No selection"));
        return;
    }

    if(elem->type() == FlowElement::Type::Node){
        m_toolBox->removeTab(0);
        m_toolBox->insertTab(0, &m_noSelectionW, QSL("No selection"));
        return;
    }

    m_currentElementId = ElementKey{elem->key()};


    m_toolBox->removeTab(0);
    if(elem->type() == FlowElement::Type::Loop){
        update_loop_ui(dynamic_cast<Loop*>(elem));
        m_toolBox->insertTab(0, &m_loopW, "Selected loop");
    }else if(elem->type() == FlowElement::Type::Routine){
        update_routine_ui(dynamic_cast<Routine*>(elem));
        m_toolBox->insertTab(0, &m_routineW, "Selected Routine");
    }else if(elem->type() == FlowElement::Type::Isi){
        update_isi_ui(dynamic_cast<Isi*>(elem));
        m_toolBox->insertTab(0, &m_isiW, "Selected Isi");
    }
}

void ElementViewerW::update_set_name(RowId id){

    QTableWidget *tw = m_loopUI->twSets;
    int row = id.v;

    auto button = qobject_cast<QPushButton*>(tw->cellWidget(row,0));
    QString currentSet = button->text();
    bool ok;
    QString newSetName = QInputDialog::getText(this,
       tr("Define selected Set name"),
       tr("Enter name:"), QLineEdit::Normal,
       currentSet, &ok);

    if(ok && newSetName.length() > 0){
        emit GSignals::get()->modify_loop_set_name_signal(m_currentElementId, newSetName.replace(' ', '_').replace('-', '_'), RowId{row});
    }
}

void ElementViewerW::update_set_occurencies(RowId id){

    QTableWidget *tw = m_loopUI->twSets;
    int row = id.v;

    auto button = qobject_cast<QPushButton*>(tw->cellWidget(row,0));
    QString currentNb = button->text();
    bool ok;
    int newOccurenciesNb = QInputDialog::getInt(this,
        tr("Define selected set number of occurencies"),
        tr("Enter number:"),
        currentNb.toInt(), 0, 100000, 1, &ok);
    if(ok ){
        emit GSignals::get()->modify_loop_set_occurrencies_nb_signal(m_currentElementId, newOccurenciesNb, RowId{row});
    }
}

void ElementViewerW::init_no_selection_ui(){

    QVBoxLayout *vbl = new QVBoxLayout();
    m_noSelectionW.setLayout(vbl);

    vbl->addWidget(m_allElementLa = new QLabel("Current flow elements (0): "));
    vbl->addWidget(m_noSelectionTW = new QTableWidget());

    m_noSelectionTW->verticalHeader()->setDefaultSectionSize(20);
    m_noSelectionTW->verticalHeader()->hide();
    m_noSelectionTW->setColumnCount(4);
    m_noSelectionTW->horizontalHeader()->setMinimumSectionSize(10);
    m_noSelectionTW->horizontalHeader()->setStretchLastSection(false);
    m_noSelectionTW->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Fixed);    
    m_noSelectionTW->setColumnWidth(0, 30); // id
    m_noSelectionTW->setColumnWidth(1, 20); // type
    m_noSelectionTW->setColumnWidth(2, 100); // name
    m_noSelectionTW->setColumnWidth(3, 80); // loo level
    m_noSelectionTW->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_noSelectionTW->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);

//    connect(m_allElementsLW, &QListWidget::itemSelectionChanged, this, [=]{
//        emit GSignals::get()->select_element_id_no_nodes_signal(RowId{m_allElementsLW->currentRow()}, true);
//    });
}

void ElementViewerW::init_routine_ui(){

    // routine
    Ui::RoutineW *ui = m_routineUI.get();
    QListWidget *lw = ui->lwConditions;

    connect(ui->pbSetName, &QPushButton::clicked, this, [&, ui]{

        bool ok;
        QString routineName = QInputDialog::getText(
            this, tr("Routine name"),
            tr("Enter new name:"), QLineEdit::Normal,
            ui->laName->text(), &ok
        );

        if(ok && !routineName.isEmpty()){
            ui->laName->setText(routineName);
            emit GSignals::get()->update_element_name_signal(m_currentElementId, routineName);
        }
    });
    connect(lw, &QListWidget::itemSelectionChanged, this, [=]{
        emit GSignals::get()->select_routine_condition_signal(m_currentElementId, RowId{lw->currentRow()});
    });
    connect(ui->pbUp, &QPushButton::clicked, this, [=]{
        emit GSignals::get()->move_routine_condition_up_signal(m_currentElementId, RowId{lw->currentRow()});
    });
    connect(ui->pbDown, &QPushButton::clicked, this, [=]{
        emit GSignals::get()->move_routine_condition_down_signal(m_currentElementId, RowId{lw->currentRow()});
    });

    connect(ui->cbIsARandomizer, &QCheckBox::clicked, this, [=]{

        bool isARandomizer = ui->cbIsARandomizer->isChecked();

        if(isARandomizer){
            QMessageBox validateBox;
            validateBox.setText("Set selected routine as a randomizer.");
            validateBox.setInformativeText("The content of the routine will be deleted, do you want to continue?");
            validateBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            validateBox.setDefaultButton(QMessageBox::Cancel);
            switch (validateBox.exec()) {
            case QMessageBox::Ok:
                break;
            case QMessageBox::Cancel:
                ui->cbIsARandomizer->blockSignals(true);
                ui->cbIsARandomizer->setChecked(false);
                ui->cbIsARandomizer->blockSignals(false);
                return;
            default:
                return;
            }
        }

        emit GSignals::get()->set_routine_as_randomizer_signal(m_currentElementId, isARandomizer);
    });

    auto teI = ui->teInformations;
    connect(teI, &QTextEdit::textChanged, this, [=]{
        emit GSignals::get()->update_element_informations_signal(m_currentElementId, teI->toPlainText());
    });
    ui->laName->setStyleSheet("QLabel{font: bold;}");
}


void ElementViewerW::init_loop_ui(){

    Ui::LoopW *ui = m_loopUI.get();

    QTableWidget *twS = ui->twSets;
    twS->setColumnWidth(0, 120);
    twS->setColumnWidth(1, 75);
    twS->setColumnWidth(2, 25);
    twS->setColumnWidth(3, 25);
    twS->setColumnWidth(4, 25);    
    twS->horizontalHeader()->setStretchLastSection(false);
    twS->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Fixed);
    twS->setEditTriggers(QAbstractItemView::NoEditTriggers);
    twS->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);

    // name
    connect(ui->pbSetName, &QPushButton::clicked, this, [&, ui]{
        bool ok;
        QString loopName = QInputDialog::getText(
            this, tr("Loop name"),
            tr("Enter new name:"), QLineEdit::Normal,
            ui->laName->text(), &ok);
        if(ok && !loopName.isEmpty()){
            ui->laName->setText(loopName);
            emit GSignals::get()->update_element_name_signal(m_currentElementId, loopName);
        }
    });

    // nb reps
    connect(ui->sbNbReps, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int value){
        emit GSignals::get()->modify_loop_nb_reps_signal(m_currentElementId, value);
    });
    // N
    connect(ui->sbN, QOverload<int>::of(&QSpinBox::valueChanged), this, [&](int value){
        emit GSignals::get()->modify_loop_n_signal(m_currentElementId, value);
    });
    // following values
    connect(ui->cbNoFollowingValues, &QCheckBox::clicked, this, [&](bool checked){
        emit GSignals::get()->modify_loop_no_following_value_signal(m_currentElementId, checked);
    });

    // style
    connect(ui->cbLoopStyle,QOverload<int>::of( &QComboBox::currentIndexChanged),[=](int index){
        emit GSignals::get()->modify_loop_type_signal(m_currentElementId, static_cast<Loop::Mode>(index));
    });

    // sets
    connect(ui->teAdd, &QTextEdit::textChanged, this, [=]{
        ui->pbAdd->setEnabled(ui->teAdd->toPlainText().size() > 0);
    });

    connect(ui->pbAdd, &QPushButton::clicked, this, [=]{
        QString txt = ui->teAdd->toPlainText().replace(' ', '_').replace('-', '_');
        ui->teAdd->blockSignals(true);
        ui->teAdd->setText(txt);
        ui->teAdd->blockSignals(false);
        emit GSignals::get()->add_loop_sets_signal(m_currentElementId, txt, RowId{ui->sbRowId->value()});
    });
    connect(ui->pbInitFromFile, &QPushButton::clicked, this, [=]{
        QString filePath = QFileDialog::getOpenFileName(nullptr, "Sets file to load",QCoreApplication::applicationDirPath() + "/data", "sets (*.txt)", nullptr);
        if(filePath.size() == 0){
            return;
        }
        emit GSignals::get()->load_loop_sets_file_signal(m_currentElementId, filePath);
    });

    auto teI = ui->teInformations;
    connect(teI, &QTextEdit::textChanged, this, [=]{
        emit GSignals::get()->update_element_informations_signal(m_currentElementId, teI->toPlainText());
    });

    ui->laName->setStyleSheet("QLabel{font: bold;}");
}

void ElementViewerW::init_isi_ui(){

    Ui::IsiW *ui = m_isiUI.get();
    QListWidget *lw = ui->lwIsi;

    // name
    connect(ui->pbSetName, &QPushButton::clicked, this, [&, ui]{
        bool ok;
        QString isiName = QInputDialog::getText(
            this, tr("ISI name"),
            tr("Enter new name:"), QLineEdit::Normal,
            ui->laName->text(), &ok);
        if(ok && !isiName.isEmpty()){
            ui->laName->setText(isiName);
            emit GSignals::get()->update_element_name_signal(m_currentElementId, isiName);
        }
    });

    // checkbox
    connect(ui->cbRandomize, &QCheckBox::clicked, [=](bool checked){
        emit GSignals::get()->set_isi_randomize_signal(m_currentElementId, checked);
    });

    // sets
    connect(lw, &QListWidget::itemSelectionChanged, this, [=]{
        ui->pbRemove->setEnabled(lw->currentRow()>= 0 && lw->count() > 1);
        ui->pbUp->setEnabled(lw->currentRow() > 0);
        ui->pbDown->setEnabled(lw->currentRow() < lw->count()-1 && lw->count() > 0);
    });
    connect(lw, &QListWidget::itemChanged, this, [=](QListWidgetItem * item){
        emit GSignals::get()->modify_isi_interval_signal(m_currentElementId, item->text().toDouble(), RowId{lw->row(item)});
    });

    connect(ui->pbAdd, &QPushButton::clicked, this, [=]{
        int id = lw->currentRow()+1;
        emit GSignals::get()->add_isi_interval_signal(m_currentElementId, ui->dsbNewIsi->value(), RowId{id});
    });

    auto teI = ui->teInformations;
    connect(teI, &QTextEdit::textChanged, this, [=]{
        emit GSignals::get()->update_element_informations_signal(m_currentElementId, teI->toPlainText());
    });
}


void ElementViewerW::update_no_selection_ui(){


    auto exp = ExperimentManager::get()->current();
    m_allElementLa->setText(QSL("Current flow elements (") % QString::number(exp->nb_elements_no_nodes()) % QSL(")"));

    m_noSelectionTW->blockSignals(true);
    m_noSelectionTW->clear();

    m_noSelectionTW->setRowCount(static_cast<int>(exp->nb_elements_no_nodes()));
    m_noSelectionTW->setHorizontalHeaderLabels({"Id","T","Name","Loop level"});

    QImage rImage = QImage(20,20, QImage::Format_BGR888);
    rImage.fill(Qt::darkGreen);
    QPixmap rIcon = QPixmap::fromImage(rImage);

    QImage lImage = QImage(20,20, QImage::Format_BGR888);
    lImage.fill(Qt::darkBlue);
    QPixmap lIcon = QPixmap::fromImage(lImage);


    size_t row = 0;
    for(const auto &element : exp->elements){

        using enum FlowElement::Type;
        if(element->type() == Node){
            continue;
        }

        auto laId = new QLabel(QString::number(row));
        laId->setStyleSheet(display::Styles::qtable_qlabel());
        laId->setAlignment(Qt::AlignCenter);
        m_noSelectionTW->setCellWidget(row, 0, laId);

        QString extra;

        auto ic = new QLabel();
        switch(element->type()){
            case Routine:{
                ic->setPixmap(rIcon);
                break;
            }case LoopStart:{
                ic->setPixmap(lIcon);
                extra =" (start)";
                break;
            }case LoopEnd:{
                ic->setPixmap(lIcon);
                extra =" (end)";
                break;
            }default:
                break;
        }

        m_noSelectionTW->setCellWidget(row, 1, ic);
        auto laName =new QLabel(element->name()%extra);
        laName->setStyleSheet(display::Styles::qtable_qlabel());
        m_noSelectionTW->setCellWidget(row, 2, laName);
        auto laIL = new QLabel(QString::number(element->insideLoops.size()));
        laIL->setStyleSheet(display::Styles::qtable_qlabel());
        laIL->setAlignment(Qt::AlignCenter);
        m_noSelectionTW->setCellWidget(row, 3, laIL);

        ++row;
    }

    m_noSelectionTW->blockSignals(false);
}


void ElementViewerW::update_loop_ui(Loop *loop){

    Ui::LoopW *ui = m_loopUI.get();
    QTableWidget *twS = ui->twSets;

    std::vector<std::unique_ptr<Set>> *sets = &loop->sets;
    ui->sbRowId->setMaximum(static_cast<int>(sets->size()));

    int idRow = 0;
    twS->blockSignals(true);
    twS->clear();
    twS->setRowCount(static_cast<int>(sets->size()));
    twS->setHorizontalHeaderLabels({"Name","Occurencies","","",""});

    for(const auto &set : *sets){

        auto *item1 = new QPushButton(set->name);
        twS->setCellWidget(idRow,0,item1);
        connect(item1, &QPushButton::clicked, this, [&,idRow]{
            update_set_name(RowId{idRow});
        });

        auto *item2 = new QPushButton(QString::number(set->occurencies));
        twS->setCellWidget(idRow,1,item2);
        connect(item2, &QPushButton::clicked, this, [&,idRow]{
            update_set_occurencies(RowId{idRow});
        });

        auto *item3 = new QPushButton("🡅");
        item3->setEnabled(idRow > 0);
        twS->setCellWidget(idRow,2,item3);
        connect(item3, &QPushButton::clicked, this, [&,idRow]{
            emit GSignals::get()->move_loop_set_up_signal(m_currentElementId, RowId{idRow});
        });

        auto *item4 = new QPushButton("🡇");
        twS->setCellWidget(idRow,3,item4);
        item4->setEnabled(idRow < twS->rowCount()-1);
        connect(item4, &QPushButton::clicked, this, [&,idRow]{
            emit GSignals::get()->move_loop_set_down_signal(m_currentElementId, RowId{idRow});
        });

        auto *item5 = new QPushButton("✖");
        twS->setCellWidget(idRow,4,item5);
        item5->setEnabled(twS->rowCount() > 1);
        connect(item5, &QPushButton::clicked, this, [&,idRow]{
            emit GSignals::get()->remove_set_signal(m_currentElementId, RowId{idRow});            
        });

        idRow++;
    }


    twS->blockSignals(false);

    // spinboxes
    ui->sbNbReps->blockSignals(true);
    ui->sbNbReps->setValue(to_int(loop->nbReps));
    ui->sbNbReps->blockSignals(false);

    ui->cbNoFollowingValues->blockSignals(true);
    ui->cbNoFollowingValues->setChecked(loop->noFollowingValues);
    ui->cbNoFollowingValues->blockSignals(false);

    ui->sbN->setEnabled(loop->mode == Loop::Mode::RandomEveryNInstances || loop->mode == Loop::Mode::ShuffleEveryNInstances);
    ui->sbN->blockSignals(true);
    ui->sbN->setValue(loop->N);
    ui->sbN->blockSignals(false);

    // comboboxes
    ui->cbLoopStyle->blockSignals(true);
    ui->cbLoopStyle->setCurrentIndex(static_cast<int>(loop->mode));    
    ui->cbLoopStyle->blockSignals(false);
    m_currentLoopIndex = ui->cbLoopStyle->currentIndex();

    // line edit
    ui->laName->setText(loop->name());

    // labels
    ui->laSetList->setText(QSL("Set List (") % QString::number(twS->rowCount()) % QSL(" elements)"));

    // informations
    ui->teInformations->setText(loop->informations);
}

void ElementViewerW::update_routine_ui(Routine *routine){

    Ui::RoutineW *ui = m_routineUI.get();
    QListWidget *lw = ui->lwConditions;


    // name
    ui->laName->setText(routine->name());

    ui->cbIsARandomizer->blockSignals(true);
    ui->cbIsARandomizer->setChecked(routine->isARandomizer);
    ui->cbIsARandomizer->blockSignals(false);

    // list
    lw->blockSignals(true);
    lw->clear();

//    QCollator collator;
//    std::sort(currRoutine->conditions.begin(), currRoutine->conditions.end(), [&collator](const Condition& c1, const Condition& c2){
//        return collator.compare(c1.name, c2.name) < 0;
//    });

    for(const auto & condition : routine->conditions){
        lw->addItem(condition->name);
    }

    // selection
    for(size_t ii = 0; ii < routine->conditions.size(); ++ii){
        if(routine->conditions[ii]->selected){
            lw->setCurrentRow(to_int(ii));
            break;
        }
    }

    ui->pbUp->setEnabled(lw->currentRow() > 0);
    ui->pbDown->setEnabled(lw->currentRow() >= 0 && lw->currentRow() < lw->count()-1 && lw->count() > 0);

    // labels
    ui->laConditions->setText(QSL("Conditions (") % QString::number(lw->count()) % QSL(" elements)"));
    lw->blockSignals(false);

    // informations
    ui->teInformations->setText(routine->informations);
}

void ElementViewerW::update_isi_ui(Isi *isi){

    Ui::IsiW *ui = m_isiUI.get();
    QListWidget *lw = ui->lwIsi;


    // list widget
    lw->blockSignals(true);
    lw->clear();

    for(const auto &interval : isi->intervals){
        QListWidgetItem *item = new QListWidgetItem(QString::number(interval), lw);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }

    if(to_int(isi->currentIntervalId) < lw->count()){
        lw->setCurrentRow(to_int(isi->currentIntervalId));
    }

//    int previousId = lw->currentRow();

//    int newCount = lw->count();
//    if(previousId < newCount){
//        lw->setCurrentRow(previousId);
//    }else{
//        lw->setCurrentRow(newCount-1);
//    }
    lw->blockSignals(false);

    // button
    ui->pbRemove->setEnabled(lw->currentRow()>= 0 && lw->count() > 1);
    ui->pbUp->setEnabled(lw->currentRow() > 0);
    ui->pbDown->setEnabled(lw->currentRow() >= 0 && lw->currentRow() < lw->count()-1 && lw->count() > 0);

    // checkbox
    ui->cbRandomize->blockSignals(true);
    ui->cbRandomize->setChecked(isi->randomized);
    ui->cbRandomize->blockSignals(false);

    // line edit
    ui->laName->setText(isi->name());

    // labels
    ui->laIsi->setText("Interstimulus state interval list (" + QString::number(lw->count()) + " elements)");

    // informations
    ui->teInformations->setText(isi->informations);
}

#include "moc_element_viewer_widget.cpp"
