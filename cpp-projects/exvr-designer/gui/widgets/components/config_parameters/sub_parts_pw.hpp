
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
#include <QFileDialog>

// base
#include "geometry/point3.hpp"

// qt-utility
#include "gui/ex_widgets/ex_base_w.hpp"

namespace tool::ex {

class ConfigParametersSubPart : public QObject{
    Q_OBJECT
protected:
    void block_signals(bool state);
    void add_input_ui(ExBaseW *w);
    void add_inputs_ui(std::vector<ExBaseW *> widgets);
    virtual void create_connections(){}
    virtual void late_update_ui(){}
public:
    std::vector<std::pair<QStringView, ExBaseW*>> inputUiElements;
    QFrame *frame = nullptr;
};

class TransformSubPart : public ConfigParametersSubPart{
public :
    TransformSubPart(QString name = "transform");
    ~TransformSubPart();
    TransformSubPart *init_widget(QString title = "Transform");
    void set_enable_state(bool s1, bool s2, bool s3);
    void set_scale_values(geo::Pt3d values);
    void set_rotation_values(geo::Pt3<qreal> v);
    void set_trans_decimals(int d);
    void set_trans_steps(geo::Pt3<qreal> s);
private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};

class WordSpaceCameraCanvasSubPart :  public ConfigParametersSubPart{
public:
    WordSpaceCameraCanvasSubPart();
    ~WordSpaceCameraCanvasSubPart();
    WordSpaceCameraCanvasSubPart *init_widget();
    void set_wh_enable_state(bool w, bool h);
private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};

class WordSpaceCanvasSubPart :  public ConfigParametersSubPart{
public:
    WordSpaceCanvasSubPart();
    ~WordSpaceCanvasSubPart();
    WordSpaceCanvasSubPart *init_widget();
    void set_wh_enable_state(bool w, bool h);
    void set_position_enable_state(bool x, bool y, bool z);
    void set_background_color(QColor bgCol);
    void create_connections() override;
    void late_update_ui() override;
private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};

class EyeRendererSubPart : public ConfigParametersSubPart{
public:
    EyeRendererSubPart();
    ~EyeRendererSubPart();
    EyeRendererSubPart *init_widget();
private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};

class TextSubPart : public ConfigParametersSubPart{
public:
    TextSubPart(QString name, bool removeInputText = false);
    ~TextSubPart();
    TextSubPart *init_widget(QString defaultText = "Text...");

    QWidget *textW = nullptr;
    QWidget *settingsW = nullptr;

private:
    struct Impl;
    std::unique_ptr<Impl> m_p;
};

}
