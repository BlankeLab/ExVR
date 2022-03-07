
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

// Qt
#include <QFileDialog>
#include <QSpinBox>
#include <QCollator>
#include <QMessageBox>
#include <QInputDialog>
#include <QTableWidget>
#include <QItemDelegate>
#include <QRegExpValidator>

using namespace tool::ex;

ElementViewerW::ElementViewerW(QTabWidget *twCurrentElement) :  m_layout(new QHBoxLayout(this)),
    m_routineUI(std::make_unique<Ui::RoutineW>()), m_loopUI(std::make_unique<Ui::LoopW>()), m_isiUI(std::make_unique<Ui::IsiW>()), m_toolBox(twCurrentElement){

    // init ui
    m_routineUI->setupUi(&m_routineW);
    m_loopUI->setupUi(&m_loopW);
    m_isiUI->setupUi(&m_isiW);

    init_routine_ui();
    init_loop_ui();
    init_isi_ui();

    m_toolBox->removeTab(0);
    m_toolBox->insertTab(0, &m_noSelectionW, QSL("No selection"));
}

void ElementViewerW::reset(){

//    m_toolBox->setVisible(false);
}

void ElementViewerW::update_from_current_element(Element *elem){


    if(elem == nullptr){
        m_toolBox->removeTab(0);
        m_toolBox->insertTab(0, &m_noSelectionW, QSL("No selection"));
        return;
    }

    if(elem->type == Element::Type::Node){
        m_toolBox->removeTab(0);
        m_toolBox->insertTab(0, &m_noSelectionW, QSL("No selection"));
        return;
    }

    m_currentElementId = ElementKey{elem->key()};


    m_toolBox->removeTab(0);
    if(elem->type == Element::Type::Loop){
        update_loop_ui(dynamic_cast<Loop*>(elem));
        m_toolBox->insertTab(0, &m_loopW, "Selected loop");
    }else if(elem->type == Element::Type::Routine){
        update_routine_ui(dynamic_cast<Routine*>(elem));
        m_toolBox->insertTab(0, &m_routineW, "Selected Routine");
    }else if(elem->type == Element::Type::Isi){
        update_isi_ui(dynamic_cast<Isi*>(elem));
        m_toolBox->insertTab(0, &m_isiW, "Selected Isi");
    }
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
}

class Delegate : public QItemDelegate {
public:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex & index) const{

        QLineEdit *lineEdit = new QLineEdit(parent);
        if(index.column() == 0){
//            lineEdit->setValidator(new QRegExpValidator(QRegExp("[^-\n\r]")));
//            lineEdit->setValidator(new QRegExpValidator(QRegExp("-{0}")));
        }else if(index.column() == 1){
            lineEdit->setValidator(new QIntValidator(0, 10000));
        }
        return lineEdit;
    }
};

void ElementViewerW::init_loop_ui(){

    Ui::LoopW *ui = m_loopUI.get();

    QTableWidget *tw = ui->twSets;
    tw->setItemDelegate(new Delegate);
    tw->setColumnWidth(0, 100);
    tw->setColumnWidth(1, 75);

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

    // style
    connect(ui->cbLoopStyle,QOverload<int>::of( &QComboBox::currentIndexChanged),[=](int index){

        if(m_currentLoopIndex == 3){ // previous mode was file mode
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Question);
            msgBox.setText("If you switch to an other mode, current loop conditions will be deleted. Do you want to continue?");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            msgBox.setModal(true);

            if(msgBox.exec() == QMessageBox::Ok){
                m_currentLoopIndex = index;
            }else{
                ui->cbLoopStyle->blockSignals(true);
                ui->cbLoopStyle->setCurrentIndex(m_currentLoopIndex);
                ui->cbLoopStyle->blockSignals(false);
                return;
            }
        }

        if(index == 3){ // file mode
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Question);
            msgBox.setText("If you switch to file mode, current loop conditions will be deleted. Do you want to continue?");
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            msgBox.setModal(true);

            if(msgBox.exec() == QMessageBox::Ok){
                m_currentLoopIndex = index;
            }else{
                ui->cbLoopStyle->blockSignals(true);
                ui->cbLoopStyle->setCurrentIndex(m_currentLoopIndex);
                ui->cbLoopStyle->blockSignals(false);
                return;
            }
        }
        emit GSignals::get()->modify_loop_type_signal(m_currentElementId, static_cast<Loop::Mode>(index));
    });

    // sets
    connect(ui->teAdd, &QTextEdit::textChanged, this, [=]{
        ui->pbAdd->setEnabled(ui->teAdd->toPlainText().size() > 0);
    });

    connect(tw, &QTableWidget::itemSelectionChanged, this, [=]{
        int row = tw->currentRow();
        emit GSignals::get()->select_loop_set_signal(m_currentElementId, tw->item(row,0)->text());
    });
    connect(tw, &QTableWidget::itemChanged, this, [=](QTableWidgetItem *item){
        int row = item->row();
        if(item->column() == 0){
            auto txt = tw->item(row,0)->text().replace(' ', '_').replace('-', '_');
            tw->blockSignals(true);
            tw->item(row,0)->setText(txt);
            tw->blockSignals(false);
            emit GSignals::get()->modify_loop_set_name_signal(m_currentElementId, txt, RowId{row});
        }else{
            emit GSignals::get()->modify_loop_set_occurrencies_nb_signal(m_currentElementId, tw->item(row,1)->text().toInt(), RowId{row});
        }
    });

    connect(ui->pbAdd, &QPushButton::clicked, this, [=]{
        QString txt = ui->teAdd->toPlainText().replace(' ', '_').replace('-', '_');
        ui->teAdd->blockSignals(true);
        ui->teAdd->setText(txt);
        ui->teAdd->blockSignals(false);
        emit GSignals::get()->add_loop_sets_signal(m_currentElementId, txt, RowId{tw->currentRow()+1});
    });
    connect(ui->pbRemove, &QPushButton::clicked, this, [=]{
        int id = tw->currentRow();
        if(id > -1){
            emit GSignals::get()->remove_set_signal(m_currentElementId, RowId{id});
        }
    });
    connect(ui->pbUp, &QPushButton::clicked, this, [=]{
        int id = tw->currentRow();
        if(id > -1){
            emit GSignals::get()->move_loop_set_up_signal(m_currentElementId, RowId{id});
        }
    });
    connect(ui->pbDown, &QPushButton::clicked, this, [=]{
        int id = tw->currentRow();
        if(id > -1){
            emit GSignals::get()->move_loop_set_down_signal(m_currentElementId, RowId{id});
        }
    });
    connect(ui->pbChooseFile, &QPushButton::clicked, this, [=]{
        QString filePath = QFileDialog::getOpenFileName(nullptr, "Sets file to load",QCoreApplication::applicationDirPath() + "/data", "sets (*.txt)", nullptr);
        if(filePath.size() == 0){
            return;
        }
        emit GSignals::get()->load_loop_sets_file_signal(m_currentElementId, filePath);
    });
    connect(ui->pbReload, &QPushButton::clicked, this, [=]{
        emit GSignals::get()->reload_loop_sets_file_signal(m_currentElementId);
    });

    auto teI = ui->teInformations;
    connect(teI, &QTextEdit::textChanged, this, [=]{
        emit GSignals::get()->update_element_informations_signal(m_currentElementId, teI->toPlainText());
    });
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
    connect(ui->pbRemove, &QPushButton::clicked, this, [=]{
        int id = lw->currentRow();
        emit GSignals::get()->remove_isi_interval_signal(m_currentElementId, RowId{id});
    });
    connect(ui->pbUp, &QPushButton::clicked, this, [=]{
        int id = lw->currentRow();
        emit GSignals::get()->move_isi_interval_up_signal(m_currentElementId, RowId{id});
    });
    connect(ui->pbDown, &QPushButton::clicked, this, [=]{
        int id = lw->currentRow();
        emit GSignals::get()->move_isi_interval_down_signal(m_currentElementId, RowId{id});
    });

    auto teI = ui->teInformations;
    connect(teI, &QTextEdit::textChanged, this, [=]{
        emit GSignals::get()->update_element_informations_signal(m_currentElementId, teI->toPlainText());
    });
}


void ElementViewerW::update_loop_ui(Loop *loop){

    Ui::LoopW *ui = m_loopUI.get();
    QTableWidget *tw = ui->twSets;

    int currentCol = tw->currentColumn();
    tw->blockSignals(true);
    tw->clear();

    ui->wChooseFile->setHidden(true);
    ui->wAddSet->setHidden(false);
    ui->fMove->setHidden(false);

    std_v1<Set> *sets = nullptr;
    if(loop->mode != Loop::Mode::File){
        sets = &loop->sets;
    }else{
        ui->laCurrentFile->blockSignals(true);
        ui->laCurrentFile->setText(loop->filePath);
        ui->laCurrentFile->blockSignals(false);
        ui->wChooseFile->setHidden(false);
        ui->wAddSet->setHidden(true);
        ui->fMove->setHidden(true);
        ui->pbReload->setEnabled(loop->filePath.size() > 0);
        ui->laCurrentFile->setText(loop->filePath);

        sets = &loop->fileSets;
    }

    int idRow = 0;
    tw->setRowCount(static_cast<int>(sets->size()));       
    tw->setCurrentCell(0,0);

    for(const auto &set : *sets){

        auto *item1 = new QTableWidgetItem(set.name);
        tw->setItem(idRow,0,item1);

        auto *item2 = new QTableWidgetItem(QString::number(set.occurencies));
        tw->setItem(idRow++,1,item2);

        if(loop->mode != Loop::Mode::File){
            item1->setFlags(item1->flags() | Qt::ItemIsEditable);
            item2->setFlags(item2->flags() | Qt::ItemIsEditable);
        }
    }
    tw->setHorizontalHeaderLabels({"Name","Occurencies"});


    for(int ii = 0; ii < tw->rowCount(); ++ii){
        if(tw->item(ii,0)->text() == loop->currentSetName){
            tw->setCurrentCell(ii,currentCol);
            break;
        }
    }

    tw->blockSignals(false);

    // button    
    int currentRow = tw->currentRow();
    ui->pbRemove->setEnabled(currentRow >= 0 && tw->rowCount() > 1);
    ui->pbUp->setEnabled(currentRow > 0);
    ui->pbDown->setEnabled(currentRow < tw->rowCount()-1 && tw->rowCount() > 1);
    ui->fMove->setEnabled(loop->mode != Loop::Mode::File);

    // spinboxes
    ui->sbNbReps->blockSignals(true);
    ui->sbNbReps->setValue(to_signed(loop->nbReps));
    ui->sbNbReps->blockSignals(false);

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
    ui->laSetList->setText(QSL("Set List (") % QString::number(tw->rowCount()) % QSL(" elements)"));

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
            lw->setCurrentRow(to_signed(ii));
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

    if(to_signed(isi->currentIntervalId) < lw->count()){
        lw->setCurrentRow(to_signed(isi->currentIntervalId));
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
