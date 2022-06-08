
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

#pragma once

// Qt
#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>

// qt-utility
#include "data/argument.hpp"

namespace tool::ex {

class BaseGenW : public QWidget{
Q_OBJECT
public:
    virtual void update_arg(Arg &arg) const = 0;
public:
    bool isValid = true;
    QString errorMessage;
signals:

    void updated_signal();
};

class GenUIItemDialog : public QDialog{

public:

    GenUIItemDialog(QString title);
    GenUIItemDialog(UiType type);

    bool show_dialog(QStringList othersParameters);
    void add_row(QString name, QWidget *w);
    void add_gen_widget(BaseGenW *w);

    Arg generate_arg(int order) const;
    QString name() const;

private:

    UiType m_type;

    const QString m_notEmptyStr {"(Parameter name must not be empty)"};
    const QString m_aldreayUsedStr {"(Parameter name is already used)"};

    QVBoxLayout *m_layout = nullptr;
    QLabel *m_laValid = nullptr;
    QLineEdit *m_leName = nullptr;
    BaseGenW *m_genW = nullptr;
};


class GenSpinboxW : public BaseGenW{

public:

    GenSpinboxW(QString text = "Spin box options");

    QSpinBox min;
    QSpinBox max;
    QSpinBox value;
    QSpinBox step;

    void update_arg(Arg &arg) const override;
};

class SpinboxFloatGenW : public BaseGenW{

public:

    SpinboxFloatGenW();

    QDoubleSpinBox min;
    QDoubleSpinBox max;
    QDoubleSpinBox value;
    QDoubleSpinBox step;
    QSpinBox decimals;

    void update_arg(Arg &arg) const override;
};

class SpinboxDoubleGenW : public BaseGenW{

public:

    SpinboxDoubleGenW(QString text = "Double spin box options");

    QDoubleSpinBox min;
    QDoubleSpinBox max;
    QDoubleSpinBox value;
    QDoubleSpinBox step;
    QSpinBox decimals;

    void update_arg(Arg &arg) const override;
};

class Vector2dGenW : public BaseGenW{

public:

    Vector2dGenW();

    QDoubleSpinBox min;
    QDoubleSpinBox max;
    QDoubleSpinBox valueX;
    QDoubleSpinBox valueY;
    QDoubleSpinBox step;
    QSpinBox decimals;

    void update_arg(Arg &arg) const override;
};

class Vector3dGenW : public BaseGenW{

public:

    Vector3dGenW();

    QDoubleSpinBox min;
    QDoubleSpinBox max;
    QDoubleSpinBox valueX;
    QDoubleSpinBox valueY;
    QDoubleSpinBox valueZ;
    QDoubleSpinBox step;
    QSpinBox decimals;

    void update_arg(Arg &arg) const override;

};

class TransformGenW : public BaseGenW{

public:

    TransformGenW();

    QDoubleSpinBox trValue;
    QDoubleSpinBox trMin;
    QDoubleSpinBox trMax;
    QDoubleSpinBox trStep;
    QSpinBox       trDec;

    QDoubleSpinBox rotValue;
    QDoubleSpinBox rotMin;
    QDoubleSpinBox rotMax;
    QDoubleSpinBox rotStep;
    QSpinBox       rotDec;

    QDoubleSpinBox scValue;
    QDoubleSpinBox scMin;
    QDoubleSpinBox scMax;
    QDoubleSpinBox scStep;
    QSpinBox       scDec;

    void update_arg(Arg &arg) const override;

};

class TextGenW : public BaseGenW{

public:
    QTextEdit textEdit;

    TextGenW(QString text = "Text");

    void update_arg(Arg &arg) const override;
};

class CheckBoxGen : public BaseGenW{
public:
    CheckBoxGen();
    void update_arg(Arg &arg) const override;
};

class ComboTextGen : public BaseGenW{

public:

    QLineEdit leText;

    ComboTextGen(QString text = "A|B|C");
    void update_arg(Arg &arg) const override;
};

class ResourceGenW : public BaseGenW{
public:

    QComboBox resourcesTypes;

    ResourceGenW();
    void update_arg(Arg &arg) const override;
};

class ComponentGenW : public BaseGenW{
public:
    QComboBox componentsTypes;

    ComponentGenW();
    void update_arg(Arg &arg) const override;
};

class CurveGen : public BaseGenW{
public:

    QLineEdit curveTitle;
    QLineEdit xTitle;
    QLineEdit yTitle;
    QDoubleSpinBox minX;
    QDoubleSpinBox maxX;
    QDoubleSpinBox minY;
    QDoubleSpinBox maxY;

    CurveGen();
    void update_arg(Arg &arg) const override;
};

class NotepadGen : public BaseGenW{
public:
    NotepadGen();
    void update_arg(Arg &arg) const override;
};

class CodeEditorGen : public BaseGenW{
public:
    CodeEditorGen();
    void update_arg(Arg &arg) const override;
};

class ColorPickGen : public BaseGenW{
public:
    ColorPickGen();
    void update_arg(Arg &arg) const override;
};





}
