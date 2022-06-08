
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

// local
#include "ui_base_component_config_widget.h"
#include "config_widget.hpp"

namespace tool::ex {

class ComponentConfigDialog;
using ComponentsConfigDialogUP = std::unique_ptr<ComponentConfigDialog>;



class ComponentConfigDialog : public QDialog{

    Q_OBJECT

public:

    ComponentConfigDialog(QWidget *parent, Component *component);

    void set_connections();

    void update_from_component(Component *component);
    void update_with_info(ConfigKey configKey, QStringView id, QStringView value);    

private :


    inline ConfigW* get_init_config_widget(){
         return qobject_cast<ConfigW*>(m_ui.vlInitConfig->itemAt(0)->widget());
    }

    inline ConfigW* get_config_widget(int index){
        return qobject_cast<ConfigW*>(m_ui.tabConfigs->widget(index));
    }

    ConfigParametersW *generate_parameters(Component::Type type, bool initConfig);

private slots:

    void show_insert_new_config_dialog();
    void show_duplicate_config_dialog();
    void show_rename_config_dialog();
    void show_reset_config_dialog();

public:

    ComponentKey componentKey;
    Component::Type componentType;

private:

    ConfigW *m_initConfigW = nullptr;

    // dialog
    std::unique_ptr<QDialog> m_modalDialog = nullptr;
    Ui::BaseComponentConfigWidget m_ui;

    Qt::WindowFlags defaultFlags;
    bool pinned = false;
    QWidget *m_parent = nullptr;

    bool m_isUpdating = false;
};


}
