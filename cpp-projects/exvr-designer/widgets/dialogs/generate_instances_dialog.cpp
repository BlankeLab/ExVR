
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

#include "generate_instances_dialog.hpp"

// Qt
#include <QFileDialog>

tool::ex::GenerateInstancesDialog::GenerateInstancesDialog(){

    setModal(false);

    m_ui.setupUi(this);
    m_ui.pbAdd->setEnabled(m_ui.teAdd->toPlainText().size() > 0);
    m_ui.twNames->setColumnWidth(0, 200);

    m_ui.twNamesOptions->setCurrentIndex(0);
    m_ui.twNamesOptions->setTabEnabled(0, true);
    m_ui.twNamesOptions->setTabEnabled(1, false);

    connect(m_ui.rbBasename, &QRadioButton::clicked, this, [&]{
        m_ui.twNamesOptions->setCurrentIndex(0);
        m_ui.twNamesOptions->setTabEnabled(0, true);
        m_ui.twNamesOptions->setTabEnabled(1, false);
    });
    connect(m_ui.rbManual, &QRadioButton::clicked, this, [&]{
        m_ui.twNamesOptions->setCurrentIndex(1);
        m_ui.twNamesOptions->setTabEnabled(0, false);
        m_ui.twNamesOptions->setTabEnabled(1, true);
    });

    connect(m_ui.teAdd, &QTextEdit::textChanged, this, [=]{
        m_ui.pbAdd->setEnabled(m_ui.teAdd->toPlainText().size() > 0);
    });

    connect(m_ui.pbAdd, &QPushButton::clicked, this,[&]{

        const auto split = m_ui.teAdd->toPlainText().split("\n", Qt::SkipEmptyParts);
        auto id = m_ui.twNames->currentRow();
        if(id == -1){
            id = 0;
        }

        for(int ii = 0; ii < split.size(); ++ii){
            m_ui.twNames->insertRow(id + ii);
            m_ui.twNames->setItem(id + ii, 0, new QTableWidgetItem(split[ii]));
        }

        m_ui.laInstancesNamesList->setText("Instances names list (" + QString::number(m_ui.twNames->rowCount()) + ")");
    });
    connect(m_ui.pbDelete, &QPushButton::clicked, this, [=]{
        m_ui.twNames->removeRow(m_ui.twNames->currentRow());
        m_ui.laInstancesNamesList->setText("Instances names list (" + QString::number(m_ui.twNames->rowCount()) + ")");
    });
    connect(m_ui.pbMoveUp, &QPushButton::clicked, this, [=]{
        auto row = m_ui.twNames->currentRow();
        if(row > 0){
            auto rowUp      = m_ui.twNames->item(row-1, 0);
            auto currentRow = m_ui.twNames->item(row, 0);
            auto t1 = rowUp->text();
            auto t2 = currentRow->text();
            rowUp->setText(t2);
            currentRow->setText(t1);
            m_ui.twNames->setCurrentCell(row-1,0);
        }
    });
    connect(m_ui.pbMoveDown, &QPushButton::clicked, this, [=]{
        auto row = m_ui.twNames->currentRow();
        if(row < m_ui.twNames->rowCount()-1){
            auto rowDown      = m_ui.twNames->item(row+1, 0);
            auto currentRow = m_ui.twNames->item(row, 0);
            auto t1 = rowDown->text();
            auto t2 = currentRow->text();
            rowDown->setText(t2);
            currentRow->setText(t1);
            m_ui.twNames->setCurrentCell(row+1,0);
        }
    });


    connect(m_ui.pbCancel, &QPushButton::clicked, this, &GenerateInstancesDialog::close);
    connect(m_ui.pbGenerateFiles, &QPushButton::clicked, this, [&]{

        directoryPath = QFileDialog::getExistingDirectory(nullptr, "Select parent directory for the generated instances");
        if(directoryPath.size() == 0){
            return;
        }

        nbInstances = m_ui.sbNbInstancesFiles->value();
        randomSeed  = static_cast<unsigned int>(m_ui.sbSeed->value());
        baseName    = m_ui.leBaseName->text();
        startId     = m_ui.sbStartId->value();
        useBaseName = m_ui.rbBasename->isChecked();
        useManual   = m_ui.rbManual->isChecked();

        manualNames.clear();
        for(int ii = 0; ii < m_ui.twNames->rowCount(); ++ii){
            manualNames << m_ui.twNames->item(ii, 0)->text();
        }

        close();
    });
}

void tool::ex::GenerateInstancesDialog::show_dialog(){
    show();
    raise();
}
