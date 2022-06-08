
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

#include "gen_ui_item_dialog.hpp"

// Qt
#include <QDialogButtonBox>

// qt-utility
#include "qt_ui.hpp"

// local
#include "data/component.hpp"
#include "resources/resource.hpp"


using namespace tool::ex;


GenUIItemDialog::GenUIItemDialog(QString title){

    setWindowTitle(title);
    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Name parameter"), m_leName = new QLineEdit()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    m_layout->addWidget(ui::F::gen(ui::L::HB(), {m_laValid = ui::W::txt("")}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
}

GenUIItemDialog::GenUIItemDialog(UiType type) : m_type(type){

    setWindowTitle(QSL("Add ") % from_view(get_name(m_type)));
    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Name parameter"), m_leName = new QLineEdit()}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    m_layout->addWidget(ui::F::gen(ui::L::HB(), {m_laValid = ui::W::txt("")}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
}

bool GenUIItemDialog::show_dialog(QStringList othersParameters){

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    m_layout->addWidget(buttonBox);

    m_laValid->setText(m_notEmptyStr);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &GenUIItemDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &GenUIItemDialog::reject);
    connect(m_leName,   &QLineEdit::textChanged, this, [&, buttonBox]{

        const bool alreadyUsed = othersParameters.contains(m_leName->text());
        const bool empty = m_leName->text().isEmpty();

        bool isValid = true;
        std::optional<QString> errorMessage;
        if(m_genW != nullptr){
            isValid      = m_genW->isValid;
            errorMessage = m_genW->errorMessage;
        }

        if(isValid){
            buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!alreadyUsed && !empty);
            m_laValid->setText(empty ? m_notEmptyStr : (alreadyUsed ? m_aldreayUsedStr : ""));
        }else{
            buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
            m_laValid->setText(errorMessage.value());
        }
    });

    if(m_genW != nullptr){
        connect(m_genW, &BaseGenW::updated_signal, this, [&, buttonBox]{

            const bool alreadyUsed = othersParameters.contains(m_leName->text());
            const bool empty = m_leName->text().isEmpty();

            bool isValid = true;
            std::optional<QString> errorMessage;
            if(m_genW != nullptr){
                isValid      = m_genW->isValid;
                errorMessage = m_genW->errorMessage;
            }

            if(isValid){
                buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!alreadyUsed && !empty);
                m_laValid->setText(empty ? m_notEmptyStr : (alreadyUsed ? m_aldreayUsedStr : ""));
            }else{
                buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
                m_laValid->setText(errorMessage.value());
            }
        });
    }

    return exec() == QDialog::Accepted;
}

void GenUIItemDialog::add_row(QString name, QWidget *w){
    m_layout->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt(name),w}, LStretch{true}, LMargins{true}, QFrame::Box));
}

void GenUIItemDialog::add_gen_widget(BaseGenW *w){
    m_layout->addWidget(ui::F::gen(ui::L::HB(), {m_genW = w}, LStretch{true}, LMargins{true}, QFrame::Box));
}

Arg GenUIItemDialog::generate_arg(int order) const{

    auto arg = Arg::generate_init_ui_arg(m_type, name(), order);
    if(m_genW != nullptr){
        m_genW->update_arg(arg);
    }
    return arg;
}

QString GenUIItemDialog::name() const{
    return m_leName->text();
}


GenSpinboxW::GenSpinboxW(QString text){

    auto l = new QVBoxLayout();
    l->setContentsMargins(0,0,0,0);
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt(text)}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Min"),    &min}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Value"),  &value}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Max"),    &max}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Step"),   &step}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addStretch();
    setLayout(l);

    min.setMinimumWidth(50);
    max.setMinimumWidth(50);
    value.setMinimumWidth(50);
    step.setMinimumWidth(50);

    ui::W::init(&min,   MinV<int>{std::numeric_limits<int>::min()},V<int>{0},MaxV<int>{std::numeric_limits<int>::max()},StepV<int>{1});
    ui::W::init(&max,   MinV<int>{std::numeric_limits<int>::min()},V<int>{100},MaxV<int>{std::numeric_limits<int>::max()},StepV<int>{1});
    ui::W::init(&value, MinV<int>{std::numeric_limits<int>::min()},V<int>{50},MaxV<int>{std::numeric_limits<int>::max()},StepV<int>{1});
    ui::W::init(&step,  MinV<int>{0},V<int>{1},MaxV<int>{std::numeric_limits<int>::max()},StepV<int>{1});

    connect(&min, QOverload<int>::of(&QSpinBox::valueChanged), this, [&]{
        if(value.value() < min.value()){
            value.setValue(min.value());
        }
        if(max.value() < min.value()){
            max.setValue(min.value());
        }
    });
    connect(&max, QOverload<int>::of(&QSpinBox::valueChanged), this, [&]{
        if(value.value() > max.value()){
            value.setValue(max.value());
        }
        if(min.value() > max.value()){
            min.setValue(max.value());
        }
    });
    connect(&value, QOverload<int>::of(&QSpinBox::valueChanged), this, [&]{
        if(min.value() > value.value()){
            min.setValue(value.value());
        }
        if(max.value() < value.value()){
            max.setValue(value.value());
        }
    });
    connect(&step, QOverload<int>::of(&QSpinBox::valueChanged), this, [&]{
        min.setSingleStep(step.value());
        value.setSingleStep(step.value());
        max.setSingleStep(step.value());
    });

}

void GenSpinboxW::update_arg(Arg &arg) const {
    arg.set_value(QString::number(value.value()));
    arg.set_separator("");
    arg.generator->min  = QString::number(min.value());
    arg.generator->max  = QString::number(max.value());
    arg.generator->step = QString::number(step.value());
}

SpinboxFloatGenW::SpinboxFloatGenW(){

    auto l = new QVBoxLayout();
    l->setContentsMargins(0,0,0,0);
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Float spin box options")}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Min"),    &min}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Value"),  &value}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Max"),    &max}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Step"),   &step}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Nb decimals"),   &decimals}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addStretch();
    setLayout(l);

    min.setMinimumWidth(50);
    max.setMinimumWidth(50);
    value.setMinimumWidth(50);
    step.setMinimumWidth(50);
    decimals.setMinimumWidth(50);

    ui::W::init(&min,        MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{0.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&max,        MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{1.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&value,      MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{0.5},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&step,       MinV<qreal>{0.},V<qreal>{0.01},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&decimals,   MinV<int>{1},V<int>{2},MaxV<int>{10},StepV<int>{1});

    connect(&min, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(value.value() < min.value()){
            value.setValue(min.value());
        }
        if(max.value() < min.value()){
            max.setValue(min.value());
        }
    });
    connect(&max, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(value.value() > max.value()){
            value.setValue(max.value());
        }
        if(min.value() > max.value()){
            min.setValue(max.value());
        }
    });
    connect(&value, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(min.value() > value.value()){
            min.setValue(value.value());
        }
        if(max.value() < value.value()){
            max.setValue(value.value());
        }
    });
    connect(&step, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        min.setSingleStep(step.value());
        value.setSingleStep(step.value());
        max.setSingleStep(step.value());
    });

    connect(&decimals, QOverload<int>::of(&QSpinBox::valueChanged), this, [&]{
        min.setDecimals(decimals.value());
        max.setDecimals(decimals.value());
        value.setDecimals(decimals.value());
        step.setDecimals(decimals.value());
    });
}

void SpinboxFloatGenW::update_arg(Arg &arg) const {
    arg.set_value(QString::number(value.value()));
    arg.set_separator("");
    arg.generator->min        = QString::number(min.value());
    arg.generator->max        = QString::number(max.value());
    arg.generator->step       = QString::number(step.value());
    arg.generator->decimals   = QString::number(decimals.value());
}

SpinboxDoubleGenW::SpinboxDoubleGenW(QString text){

    auto l = new QVBoxLayout();
    l->setContentsMargins(0,0,0,0);
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt(text)}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Min"),    &min}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Value"),  &value}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Max"),    &max}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Step"),   &step}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Nb decimals"),   &decimals}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addStretch();
    setLayout(l);

    min.setMinimumWidth(50);
    max.setMinimumWidth(50);
    value.setMinimumWidth(50);
    step.setMinimumWidth(50);
    decimals.setMinimumWidth(50);

    ui::W::init(&min,        MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{0.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&max,        MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{1.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&value,      MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{0.5},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&step,       MinV<qreal>{0.},V<qreal>{0.01},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&decimals,   MinV<int>{1},V<int>{2},MaxV<int>{10},StepV<int>{1});

    connect(&min, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(value.value() < min.value()){
            value.setValue(min.value());
        }
        if(max.value() < min.value()){
            max.setValue(min.value());
        }
    });
    connect(&max, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(value.value() > max.value()){
            value.setValue(max.value());
        }
        if(min.value() > max.value()){
            min.setValue(max.value());
        }
    });
    connect(&value, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(min.value() > value.value()){
            min.setValue(value.value());
        }
        if(max.value() < value.value()){
            max.setValue(value.value());
        }
    });
    connect(&step, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        min.setSingleStep(step.value());
        value.setSingleStep(step.value());
        max.setSingleStep(step.value());
    });

    connect(&decimals, QOverload<int>::of(&QSpinBox::valueChanged), this, [&]{
        min.setDecimals(decimals.value());
        max.setDecimals(decimals.value());
        value.setDecimals(decimals.value());
        step.setDecimals(decimals.value());
    });
}

void SpinboxDoubleGenW::update_arg(Arg &arg) const {
    arg.set_value(QString::number(value.value()));
    arg.set_separator("");
    arg.generator->min        = QString::number(min.value());
    arg.generator->max        = QString::number(max.value());
    arg.generator->step       = QString::number(step.value());
    arg.generator->decimals   = QString::number(decimals.value());
}

Vector2dGenW::Vector2dGenW(){

    auto l = ui::L::VB();
    l->setContentsMargins(0,0,0,0);
    l->addWidget(ui::F::gen_frame(ui::L::HB(), {ui::W::txt("Vector 2D options")}, LStretchD{10, {}}));
    l->addWidget(ui::F::gen_frame(ui::L::HB(), {ui::W::txt("Min"), &min, ui::W::txt("Max"), &max}, LStretchD{10, {1,3,1,3}}));
    l->addWidget(ui::F::gen_frame(ui::L::HB(), {ui::W::txt("X"), &valueX, ui::W::txt("Y"), &valueY}, LStretchD{10, {1,3,1,3}}));
    l->addWidget(ui::F::gen_frame(ui::L::HB(), {ui::W::txt("Step"), &step, ui::W::txt("Nb decimals"), &decimals}, LStretchD{10, {1,3,1,2}}));
    l->addStretch();
    setLayout(l);

    min.setMinimumWidth(50);
    max.setMinimumWidth(50);
    valueX.setMinimumWidth(50);
    valueY.setMinimumWidth(50);
    step.setMinimumWidth(50);
    decimals.setMinimumWidth(50);

    ui::W::init(&min,        MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{0.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&max,        MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{1.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&valueX,     MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{0.5},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&valueY,     MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{0.5},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&step,       MinV<qreal>{0.},V<qreal>{0.01},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&decimals,   MinV<int>{1},V<int>{2},MaxV<int>{10},StepV<int>{1});

    connect(&min, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{

        if(valueX.value() < min.value()){
            valueX.setValue(min.value());
        }
        if(valueY.value() < min.value()){
            valueY.setValue(min.value());
        }
        if(max.value() < min.value()){
            max.setValue(min.value());
        }
    });
    connect(&max, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(valueX.value() > max.value()){
            valueX.setValue(max.value());
        }
        if(valueY.value() > max.value()){
            valueY.setValue(max.value());
        }
        if(min.value() > max.value()){
            min.setValue(max.value());
        }
    });
    connect(&valueX, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(min.value() > valueX.value()){
            min.setValue(valueX.value());
        }
        if(max.value() < valueX.value()){
            max.setValue(valueX.value());
        }
    });
    connect(&valueY, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(min.value() > valueY.value()){
            min.setValue(valueY.value());
        }
        if(max.value() < valueY.value()){
            max.setValue(valueY.value());
        }
    });

    connect(&step, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        min.setSingleStep(step.value());
        valueX.setSingleStep(step.value());
        valueY.setSingleStep(step.value());
        max.setSingleStep(step.value());
    });

    connect(&decimals, QOverload<int>::of(&QSpinBox::valueChanged), this, [&]{
        min.setDecimals(decimals.value());
        max.setDecimals(decimals.value());
        valueX.setDecimals(decimals.value());
        valueY.setDecimals(decimals.value());
        step.setDecimals(decimals.value());
    });
}

void Vector2dGenW::update_arg(Arg &arg) const {
    arg.set_value(QString::number(valueX.value()) + " " + QString::number(valueY.value()));
    arg.set_separator(" ");
    arg.generator->min        = QString::number(min.value());
    arg.generator->max        = QString::number(max.value());
    arg.generator->step       = QString::number(step.value());
    arg.generator->decimals   = QString::number(decimals.value());
}

Vector3dGenW::Vector3dGenW(){

    auto l = ui::L::VB();
    l->setContentsMargins(0,0,0,0);
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Vector 3D options")}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Min"), &min,ui::W::txt("Max"), &max}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("X"), &valueX}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Y"), &valueY}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Z"), &valueZ}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Step"), &step, ui::W::txt("Nb decimals"), &decimals}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addStretch();
    setLayout(l);

    min.setMinimumWidth(50);
    max.setMinimumWidth(50);
    valueX.setMinimumWidth(50);
    valueY.setMinimumWidth(50);
    valueZ.setMinimumWidth(50);
    step.setMinimumWidth(50);
    decimals.setMinimumWidth(50);

    ui::W::init(&min,        MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{0.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&max,        MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{1.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&valueX,     MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{0.5},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&valueY,     MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{0.5},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&valueZ,     MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{0.5},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&step,       MinV<qreal>{0.},V<qreal>{0.01},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&decimals,   MinV<int>{1},V<int>{2},MaxV<int>{10},StepV<int>{1});

    connect(&min, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{

        if(valueX.value() < min.value()){
            valueX.setValue(min.value());
        }
        if(valueY.value() < min.value()){
            valueY.setValue(min.value());
        }
        if(valueZ.value() < min.value()){
            valueZ.setValue(min.value());
        }

        if(max.value() < min.value()){
            max.setValue(min.value());
        }
    });
    connect(&max, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(valueX.value() > max.value()){
            valueX.setValue(max.value());
        }
        if(valueY.value() > max.value()){
            valueY.setValue(max.value());
        }
        if(valueZ.value() > max.value()){
            valueZ.setValue(max.value());
        }
        if(min.value() > max.value()){
            min.setValue(max.value());
        }
    });
    connect(&valueX, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(min.value() > valueX.value()){
            min.setValue(valueX.value());
        }
        if(max.value() < valueX.value()){
            max.setValue(valueX.value());
        }
    });
    connect(&valueY, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(min.value() > valueY.value()){
            min.setValue(valueY.value());
        }
        if(max.value() < valueY.value()){
            max.setValue(valueY.value());
        }
    });
    connect(&valueZ, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{

        if(min.value() > valueZ.value()){
            min.setValue(valueZ.value());
        }
        if(max.value() < valueZ.value()){
            max.setValue(valueZ.value());
        }
    });

    connect(&step, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        min.setSingleStep(step.value());
        valueX.setSingleStep(step.value());
        valueY.setSingleStep(step.value());
        valueZ.setSingleStep(step.value());
        max.setSingleStep(step.value());
    });

    connect(&decimals, QOverload<int>::of(&QSpinBox::valueChanged), this, [&]{
        min.setDecimals(decimals.value());
        max.setDecimals(decimals.value());
        valueX.setDecimals(decimals.value());
        valueY.setDecimals(decimals.value());
        valueZ.setDecimals(decimals.value());
        step.setDecimals(decimals.value());
    });
}

void Vector3dGenW::update_arg(Arg &arg) const {
    arg.set_value(QString::number(valueX.value()) + " " + QString::number(valueY.value()) + " " + QString::number(valueZ.value()));
    arg.set_separator(" ");
    arg.generator->min        = QString::number(min.value());
    arg.generator->max        = QString::number(max.value());
    arg.generator->step       = QString::number(step.value());
    arg.generator->decimals   = QString::number(decimals.value());
}

TransformGenW::TransformGenW(){

    auto l = new QGridLayout();
    setLayout(l);

    int minW = 50;
    trValue.setMinimumWidth(minW);
    trMin.setMinimumWidth(minW);
    trMax.setMinimumWidth(minW);
    trStep.setMinimumWidth(minW);
    trDec.setMinimumWidth(minW);

    rotValue.setMinimumWidth(minW);
    rotMin.setMinimumWidth(minW);
    rotMax.setMinimumWidth(minW);
    rotStep.setMinimumWidth(minW);
    rotDec.setMinimumWidth(minW);

    scValue.setMinimumWidth(minW);
    scMin.setMinimumWidth(minW);
    scMax.setMinimumWidth(minW);
    scStep.setMinimumWidth(minW);
    scDec.setMinimumWidth(minW);

    auto title = ui::W::txt("<b>Transform options</b>");
    title->setAlignment(Qt::AlignHCenter);
    l->addWidget(title, 0,0,1,10);

    l->addWidget(ui::W::txt("<b>Translation</b>"), 1,0,1,4);
    l->addWidget(ui::W::txt("Min"), 2, 0,1,1);
    l->addWidget(&trMin, 2, 1, 1,1);
    l->addWidget(ui::W::txt("Default value"), 2, 2,1,1);
    l->addWidget(&trValue, 2, 3, 1,1);
    l->addWidget(ui::W::txt("Max"), 2, 4,1,1);
    l->addWidget(&trMax, 2, 5, 1,1);
    l->addWidget(ui::W::txt("Step"), 2, 6,1,1);
    l->addWidget(&trStep, 2, 7, 1,1);
    l->addWidget(ui::W::txt("Dec"), 2, 8,1,1);
    l->addWidget(&trDec, 2, 9, 1,1);

    l->addWidget(ui::W::txt("<b>Rotation</b>"), 3,0,1,4);
    l->addWidget(ui::W::txt("Min"), 4, 0,1,1);
    l->addWidget(&rotMin, 4, 1, 1,1);
    l->addWidget(ui::W::txt("Default value"), 4, 2,1,1);
    l->addWidget(&rotValue, 4, 3, 1,1);
    l->addWidget(ui::W::txt("Max"), 4, 4,1,1);
    l->addWidget(&rotMax, 4, 5, 1,1);
    l->addWidget(ui::W::txt("Step"), 4, 6,1,1);
    l->addWidget(&rotStep, 4, 7, 1,1);
    l->addWidget(ui::W::txt("Dec"), 4, 8,1,1);
    l->addWidget(&rotDec, 4, 9, 1,1);

    l->addWidget(ui::W::txt("<b>Scale</b>"), 5,0,1,4);
    l->addWidget(ui::W::txt("Min"), 6, 0,1,1);
    l->addWidget(&scMin, 6, 1, 1,1);
    l->addWidget(ui::W::txt("Default value"), 6, 2,1,1);
    l->addWidget(&scValue, 6, 3, 1,1);
    l->addWidget(ui::W::txt("Max"), 6, 4,1,1);
    l->addWidget(&scMax, 6, 5, 1,1);
    l->addWidget(ui::W::txt("Step"), 6, 6,1,1);
    l->addWidget(&scStep, 6, 7, 1,1);
    l->addWidget(ui::W::txt("Dec"), 6, 8,1,1);
    l->addWidget(&scDec, 6, 9, 1,1);

    auto spacer = new QSpacerItem(20,20, QSizePolicy::Expanding, QSizePolicy::Expanding);
    l->addItem(spacer, 7, 0, 1, 1);

    ui::W::init(&trMin,     MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{-1000.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&rotMin,    MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{-1000.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&scMin,     MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{-1000.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&trMax,     MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{1000.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&rotMax,    MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{1000.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&scMax,     MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{1000.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&trValue,   MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{0.0},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&rotValue,  MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{0.0},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&scValue,   MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{1.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&trStep,    MinV<qreal>{0.},V<qreal>{0.01},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&rotStep,   MinV<qreal>{0.},V<qreal>{0.01},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&scStep,    MinV<qreal>{0.},V<qreal>{0.01},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&trDec,     MinV<int>{1},V<int>{2},MaxV<int>{10},StepV<int>{1});
    ui::W::init(&rotDec,    MinV<int>{1},V<int>{2},MaxV<int>{10},StepV<int>{1});
    ui::W::init(&scDec,     MinV<int>{1},V<int>{2},MaxV<int>{10},StepV<int>{1});

    connect(&trMin, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(trValue.value() < trMin.value()){
            trValue.setValue(trMin.value());
        }
        if(trMax.value() < trMin.value()){
            trMax.setValue(trMin.value());
        }
    });
    connect(&trMax, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(trValue.value() > trMax.value()){
            trValue.setValue(trMax.value());
        }
        if(trMin.value() > trMax.value()){
            trMin.setValue(trMax.value());
        }
    });
    connect(&trValue, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(trMin.value() > trValue.value()){
            trMin.setValue(trValue.value());
        }
        if(trMax.value() < trValue.value()){
            trMax.setValue(trValue.value());
        }
    });
    connect(&trStep, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        trMin.setSingleStep(trStep.value());
        trValue.setSingleStep(trStep.value());
        trMax.setSingleStep(trStep.value());
    });

    // rot
    connect(&rotMin, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(rotValue.value() < rotMin.value()){
            rotValue.setValue(rotMin.value());
        }
        if(rotMax.value() < rotMin.value()){
            rotMax.setValue(rotMin.value());
        }
    });
    connect(&rotMax, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(rotValue.value() > rotMax.value()){
            rotValue.setValue(rotMax.value());
        }
        if(rotMin.value() > rotMax.value()){
            rotMin.setValue(rotMax.value());
        }
    });
    connect(&rotValue, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(rotMin.value() > rotValue.value()){
            rotMin.setValue(rotValue.value());
        }
        if(rotMax.value() < rotValue.value()){
            rotMax.setValue(rotValue.value());
        }
    });
    connect(&rotStep, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        rotMin.setSingleStep(rotStep.value());
        rotValue.setSingleStep(rotStep.value());
        rotMax.setSingleStep(rotStep.value());
    });
    // sc
    connect(&scMin, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(scValue.value() < scMin.value()){
            scValue.setValue(scMin.value());
        }
        if(scMax.value() < scMin.value()){
            scMax.setValue(scMin.value());
        }
    });
    connect(&scMax, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(scValue.value() > scMax.value()){
            scValue.setValue(scMax.value());
        }
        if(scMin.value() > scMax.value()){
            scMin.setValue(scMax.value());
        }
    });
    connect(&scValue, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(scMin.value() > scValue.value()){
            scMin.setValue(scValue.value());
        }
        if(scMax.value() < scValue.value()){
            scMax.setValue(scValue.value());
        }
    });
    connect(&scStep, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        scMin.setSingleStep(scStep.value());
        scValue.setSingleStep(scStep.value());
        scMax.setSingleStep(scStep.value());
    });

}

void TransformGenW::update_arg(Arg &arg) const{

    auto trV    =  QString::number(trValue.value());
    auto rotV   =  QString::number(rotValue.value());
    auto scV    =  QString::number(scValue.value());
    arg.set_value(
        trV     % QSL(" ") % trV  % QSL(" ") % trV  % QSL(" ") +
        rotV    % QSL(" ") % rotV % QSL(" ") % rotV % QSL(" ") +
        scV     % QSL(" ") % scV  % QSL(" ") % scV
    );
    arg.set_separator(QSL(" "));
    arg.generator->min       = QString::number(trMin.value()) + " " + QString::number(rotMin.value()) + " " + QString::number(scMin.value());
    arg.generator->max       = QString::number(trMax.value()) + " " + QString::number(rotMax.value()) + " " + QString::number(scMax.value());
    arg.generator->step      = QString::number(trStep.value()) + " " + QString::number(rotStep.value()) + " " + QString::number(scStep.value());
    arg.generator->decimals  = QString::number(trDec.value()) + " " + QString::number(rotDec.value()) + " " + QString::number(scDec.value());

}


ResourceGenW::ResourceGenW(){

    auto l = ui::L::VB();
    l->setContentsMargins(0,0,0,0);
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Resources options")}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Type"), &resourcesTypes}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addStretch();
    setLayout(l);

    resourcesTypes.setMinimumWidth(200);

    QStringList types;
    for(const auto &n : Resource::get_names()){
        types << from_view(n);
    }
    ui::W::init(&resourcesTypes, types);
}

void ResourceGenW::update_arg(Arg &arg) const {
    arg.set_separator("");
    arg.set_value("");
    arg.generator->info = resourcesTypes.currentText();
}

ComponentGenW::ComponentGenW(){

    auto l = ui::L::VB();
    l->setContentsMargins(0,0,0,0);
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Components options")}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Type"), &componentsTypes}, LStretch{true}, LMargins{false}, QFrame::NoFrame));
    l->addStretch();
    setLayout(l);

    componentsTypes.setMinimumWidth(200);

    QStringList types;
    for(const auto &m : Component::components.data){
        types << from_view(Component::get_full_name(std::get<0>(m)));
    }
    ui::W::init(&componentsTypes, types);
}

void ComponentGenW::update_arg(Arg &arg) const{
    arg.set_separator("");
    arg.set_value("");
    arg.generator->info = componentsTypes.currentText();
}


TextGenW::TextGenW(QString text){
    auto l = new QVBoxLayout();
    l->setContentsMargins(0,0,0,0);
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt(text), &textEdit}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    l->addStretch();
    setLayout(l);
}

void TextGenW::update_arg(Arg &arg) const{
    arg.set_separator("");
    arg.set_value(textEdit.toPlainText());
}

CurveGen::CurveGen(){

//    QLineEdit curveTitle;
//    QLineEdit xTitle;
//    QLineEdit xTitle;
//    QDoubleSpinBox minX;
//    QDoubleSpinBox maxX;
//    QDoubleSpinBox minY;
//    QDoubleSpinBox maxY;

    auto l = new QVBoxLayout();
    l->setContentsMargins(0,0,0,0);
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Curve title"), &curveTitle}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("X axis title"), &xTitle}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Y axis title"), &yTitle}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("X min/max values"), &minX, &maxX}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Y min/max values"), &minY, &maxY}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    l->addStretch();
    setLayout(l);

    ui::W::init(&minX,        MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{0.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&maxX,        MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{1.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&minY,        MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{0.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&maxY,        MinV<qreal>{std::numeric_limits<qreal>::lowest()},V<qreal>{1.},MaxV<qreal>{std::numeric_limits<qreal>::max()},StepV<qreal>{0.01}, 2);
    ui::W::init(&curveTitle, "default");
    ui::W::init(&xTitle, "x");
    ui::W::init(&yTitle, "y");

    connect(&minX, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(maxX.value() < minX.value()){
            maxX.setValue(minX.value());
        }
    });
    connect(&maxX, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(minX.value() > maxX.value()){
            minX.setValue(maxX.value());
        }
    });
    connect(&minY, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(maxY.value() < minY.value()){
            maxY.setValue(minY.value());
        }
    });
    connect(&maxY, QOverload<qreal>::of(&QDoubleSpinBox::valueChanged), this, [&]{
        if(minY.value() > maxY.value()){
            minY.setValue(maxY.value());
        }
    });
}

void CurveGen::update_arg(Arg &arg) const{
    std::vector<double> x =  {minX.value(),maxX.value()};
    std::vector<double> y =  {minY.value(),maxY.value()};
    arg.init_from_curve(&x, &y, " ");
    arg.generator->min = QString::number(minX.value()) % QSL(" ") % QString::number(minY.value());
    arg.generator->max = QString::number(maxX.value()) % QSL(" ") % QString::number(maxY.value());
    arg.generator->info = curveTitle.text() % QSL("%%%") % xTitle.text() % QSL("%%%") % yTitle.text();
}

ComboTextGen::ComboTextGen(QString text){
    auto l = new QVBoxLayout();
    leText.setText(text);
    l->setContentsMargins(0,0,0,0);
    l->addWidget(ui::F::gen(ui::L::HB(), {ui::W::txt("Enter items (using \"|\" as separator)):"), &leText}, LStretch{false}, LMargins{false}, QFrame::NoFrame));
    l->addStretch();
    setLayout(l);

    connect(&leText, &QLineEdit::textChanged, this, [&]{

        if(leText.text().length() == 0){
            isValid = false;
            errorMessage = "No items";
            emit updated_signal();
            return;
        }

        auto split = leText.text().split("|");
        if(split.count() == 0){
            errorMessage = "Invalid items";
            isValid = false;
            emit updated_signal();
            return;
        }

        isValid = true;

        emit updated_signal();
    });
}

void ComboTextGen::update_arg(Arg &arg) const{

    if(leText.text().length() == 0){
        return;
    }

    auto split = leText.text().split("|");
    if(split.count() == 0){
        return;
    }

    arg.set_value(split[0]);
    arg.set_separator("");
    arg.generator->info = leText.text();
}

NotepadGen::NotepadGen(){}

void NotepadGen::update_arg(Arg &arg) const{}

CodeEditorGen::CodeEditorGen(){}

void CodeEditorGen::update_arg(Arg &) const{}

ColorPickGen::ColorPickGen(){}

void ColorPickGen::update_arg(Arg &) const{}

CheckBoxGen::CheckBoxGen(){}

void CheckBoxGen::update_arg(Arg &) const{}

