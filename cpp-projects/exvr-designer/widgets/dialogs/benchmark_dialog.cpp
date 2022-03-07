
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

#include "benchmark_dialog.hpp"

// Qt
#include <QDoubleSpinBox>

// base
#include "utility/benchmark.hpp"

// qt-utility
#include "qt_ui.hpp"

using namespace tool;
using namespace tool::ex;


BenchmarkDialog::BenchmarkDialog(){

    setWindowTitle("Benchmark:");
    setLayout(new QVBoxLayout());
    setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setModal(false);

    view = new QTableView();
    model = new Table();
    view->setModel(model);

    QString txt = "Select element to reach:";
    layout()->addWidget(new QLabel(txt));

    cbSort  = new QCheckBox("Sort");
    auto unit = new QComboBox();
    auto minT = new QDoubleSpinBox();
    pbClear = new QPushButton("Clear");
    layout()->addWidget(ui::F::gen(ui::L::HB(), {cbSort, ui::W::txt("Unit:"), unit, ui::W::txt("Min T:"), minT, pbClear}, LStretch{true}, LMargins{true}, QFrame::NoFrame));
    layout()->addWidget(view);

    connect(pbClear, &QPushButton::clicked, this, [=]{
        qDebug() << "clear";
        Bench::clear();
        model->elements.clear();
        model->elementsRow.clear();
    });
}

void BenchmarkDialog::update(){
    model->update(cbSort->isChecked());
    view->viewport()->update();
}

int Table::rowCount(const QModelIndex &) const{
    return elements.size();
}

int Table::columnCount(const QModelIndex &) const{
    return 3;
}

QVariant Table::data(const QModelIndex &index, int role) const{

    if (role == Qt::DisplayRole){
        auto id = index.row();
        if(index.column() == 0){
            return std::get<1>(elements[id]);
        }else if (index.column() == 1){
            return std::get<2>(elements[id]);
        }else if(index.column() == 2){
            return std::get<4>(elements[id]);
        }
    }else if (role == Qt::BackgroundRole){
         if(index.column() == 0){
            return QColor(Qt::lightGray);
         }else if (index.column() == 1){
            return QColor(Qt::black);
         }else if (index.column() == 1){
             return QColor(Qt::lightGray);
         }
    }else if (role == Qt::ForegroundRole){
        if(index.column() == 0){
            return QColor(Qt::black);
        }else if (index.column() == 1){
            auto id = index.row();
            return std::get<3>(elements.at(id)) ? QColor(Qt::green) : QColor(Qt::darkYellow);
        }else if (index.column() == 2){
            return QColor(Qt::black);
        }
    }else if (role == Qt::TextAlignmentRole){
        if(index.column() == 0){
            return Qt::AlignLeft;
        }else if(index.column() == 1){
            return Qt::AlignCenter;
        }else if(index.column() == 1){
            return Qt::AlignCenter;
        }
    }

    return QVariant();
}

void Table::update(bool sort){

    beginResetModel();

    // reset enabled state
    for(auto &element : elements){
        std::get<3>(element) = false;
    }

    size_t rowAdded = 0;
    for(auto &timeInfo : Bench::all_total_times(BenchUnit::microseconds, -1, false)){

        std::string_view nameView = std::get<0>(timeInfo);
        const auto time  = std::get<1>(timeInfo);
        const auto count = std::get<2>(timeInfo);

        if(!elementsRow.contains(nameView)){
            elementsRow[nameView] = elementsRow.size();
            elements.emplace_back(std::make_tuple(nameView, from_view(nameView), time, true, count));
            ++rowAdded;
        }else{
            auto &element = elements[elementsRow[nameView]];
            std::get<2>(element) = time;
            std::get<3>(element) = true;
            std::get<4>(element) = count;
        }
    }

    // name view / time / enabled / count
    using T = std::tuple<std::string_view, QString, std::int64_t, bool, size_t>;
    if(sort){
        std::sort(std::begin(elements), std::end(elements), [&](const T& l, const T& r){
            if(std::get<3>(l) && !std::get<3>(r)){
                return true;
            }
            if(!std::get<3>(l) && std::get<3>(r)){
                return false;
            }
            return std::get<2>(l) > std::get<2>(r);
        });
    }

    for(size_t ii = 0; ii < elements.size(); ++ii){
        elementsRow[std::get<0>(elements[ii])] = ii;
    }

    endResetModel();
}





//class TorrentViewDelegate : public QItemDelegate
//{
//    Q_OBJECT
//public:
//    inline TorrentViewDelegate(MainWindow *mainWindow) : QItemDelegate(mainWindow) {}

//    inline void paint(QPainter *painter, const QStyleOptionViewItem &option,
//                      const QModelIndex &index ) const
//    {
//        if (index.column() != 2) {
//            QItemDelegate::paint(painter, option, index);
//            return;
//        }

//        // Set up a QStyleOptionProgressBar to precisely mimic the
//        // environment of a progress bar.
//        QStyleOptionProgressBar progressBarOption;
//        progressBarOption.state = QStyle::State_Enabled;
//        progressBarOption.direction = QApplication::layoutDirection();
//        progressBarOption.rect = option.rect;
//        progressBarOption.fontMetrics = QApplication::fontMetrics();
//        progressBarOption.minimum = 0;
//        progressBarOption.maximum = 100;
//        progressBarOption.textAlignment = Qt::AlignCenter;
//        progressBarOption.textVisible = true;

//        // Set the progress and text values of the style option.
//        int progress = qobject_cast<MainWindow *>(parent())->clientForRow(index.row())->progress();
//        progressBarOption.progress = progress < 0 ? 0 : progress;
//        progressBarOption.text = QString().sprintf("%d%%", progressBarOption.progress);

//        // Draw the progress bar onto the view.
//        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
//    }
//};

//void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
//                     const QModelIndex &index) const
//{
//    QStyleOptionViewItem opt(option);
//    initStyleOption(&opt, index);

//    const QPalette &palette(opt.palette);
//    const QRect &rect(opt.rect);
//    const QRect &contentRect(rect.adjusted(m_ptr->margins.left(),
//                                           m_ptr->margins.top(),
//                                           -m_ptr->margins.right(),
//                                           -m_ptr->margins.bottom()));
//    const bool lastIndex = (index.model()->rowCount() - 1) == index.row();
//    const bool hasIcon = !opt.icon.isNull();
//    const int bottomEdge = rect.bottom();
//    QFont f(opt.font);

//    f.setPointSize(m_ptr->timestampFontPointSize(opt.font));

//    painter->save();
//    painter->setClipping(true);
//    painter->setClipRect(rect);
//    painter->setFont(opt.font);

//    // Draw background
//    painter->fillRect(rect, opt.state & QStyle::State_Selected ?
//                                                               palette.highlight().color() :
//                                                               palette.light().color());

//    // Draw bottom line
//    painter->setPen(lastIndex ? palette.dark().color()
//                              : palette.mid().color());
//    painter->drawLine(lastIndex ? rect.left() : m_ptr->margins.left(),
//                      bottomEdge, rect.right(), bottomEdge);

//    // Draw message icon
//    if (hasIcon)
//        painter->drawPixmap(contentRect.left(), contentRect.top(),
//                            opt.icon.pixmap(m_ptr->iconSize));

//    // Draw timestamp
//    QRect timeStampRect(m_ptr->timestampBox(opt, index));

//    timeStampRect.moveTo(m_ptr->margins.left() + m_ptr->iconSize.width()
//                             + m_ptr->spacingHorizontal, contentRect.top());

//    painter->setFont(f);
//    painter->setPen(palette.text().color());
//    painter->drawText(timeStampRect, Qt::TextSingleLine,
//                      index.data(Qt::UserRole).toString());

//    // Draw message text
//    QRect messageRect(m_ptr->messageBox(opt));

//    messageRect.moveTo(timeStampRect.left(), timeStampRect.bottom()
//                                                 + m_ptr->spacingVertical);

//    painter->setFont(opt.font);
//    painter->setPen(palette.windowText().color());
//    painter->drawText(messageRect, Qt::TextSingleLine, opt.text);

//    painter->restore();
//}
