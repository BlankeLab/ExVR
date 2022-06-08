
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

#include "keys_dialog.hpp"

// Qt
#include <QDoubleSpinBox>


// qt-utility
#include "qt_ui.hpp"

using namespace tool;
using namespace tool::ex;


KeysDialog::KeysDialog(){

    setWindowTitle("Keys:");
    setLayout(new QVBoxLayout());
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setModal(false);

    view = new QTableView();
    model = new KeysTable();
    view->setModel(model);

    QString txt = "...:";
    layout()->addWidget(new QLabel(txt));
    layout()->addWidget(view);
}

void KeysDialog::update(){
    model->update();
    view->viewport()->update();
}

void KeysDialog::show_dialog(){
    show();
    raise();
}

int KeysTable::rowCount(const QModelIndex &) const{
    return static_cast<int>(elements.size());
}

int KeysTable::columnCount(const QModelIndex &) const{
    return 1;
}

QVariant KeysTable::data(const QModelIndex &index, int role) const{

//    if (role == Qt::DisplayRole){
//        auto id = index.row();
//        if(index.column() == 0){
//            return std::get<1>(elements[id]);
//        }else if (index.column() == 1){
//            return std::get<2>(elements[id]);
//        }else if(index.column() == 2){
//            return std::get<4>(elements[id]);
//        }
//    }else if (role == Qt::BackgroundRole){
//        if(index.column() == 0){
//            return QColor(Qt::lightGray);
//        }else if (index.column() == 1){
//            return QColor(Qt::black);
//        }else if (index.column() == 1){
//            return QColor(Qt::lightGray);
//        }
//    }else if (role == Qt::ForegroundRole){
//        if(index.column() == 0){
//            return QColor(Qt::black);
//        }else if (index.column() == 1){
//            auto id = index.row();
//            return std::get<3>(elements.at(id)) ? QColor(Qt::green) : QColor(Qt::darkYellow);
//        }else if (index.column() == 2){
//            return QColor(Qt::black);
//        }
//    }else if (role == Qt::TextAlignmentRole){
//        if(index.column() == 0){
//            return Qt::AlignLeft;
//        }else if(index.column() == 1){
//            return Qt::AlignCenter;
//        }else if(index.column() == 1){
//            return Qt::AlignCenter;
//        }
//    }

    return QVariant();
}

void KeysTable::update(){

    beginResetModel();

//    // reset enabled state
//    for(auto &element : elements){
//        std::get<3>(element) = false;
//    }

//    size_t rowAdded = 0;
//    for(auto &timeInfo : Bench::all_total_times(BenchUnit::microseconds, -1, false)){

//        std::string_view nameView = std::get<0>(timeInfo);
//        const auto time  = std::get<1>(timeInfo);
//        const auto count = std::get<2>(timeInfo);

//        if(!elementsRow.contains(nameView)){
//            elementsRow[nameView] = elementsRow.size();
//            elements.emplace_back(std::make_tuple(nameView, from_view(nameView), time, true, count));
//            ++rowAdded;
//        }else{
//            auto &element = elements[elementsRow[nameView]];
//            std::get<2>(element) = time;
//            std::get<3>(element) = true;
//            std::get<4>(element) = count;
//        }
//    }

//    for(size_t ii = 0; ii < elements.size(); ++ii){
//        elementsRow[std::get<0>(elements[ii])] = ii;
//    }

    endResetModel();
}


