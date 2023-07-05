
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

// base
#include "utility/unordered_map.hpp"

// Qt
#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QDialog>
#include <QFileDialog>

// base
#include "utility/vector.hpp"

// local
#include "resources/resources_manager.hpp"
#include "ui_resources_manager_dialog.h"
#include "ui_resource_type.h"


namespace tool::ex {

class VerticalTextTabWidgetStyle : public QProxyStyle{
    public:
    QSize sizeFromContents(ContentsType type, const QStyleOption* option,const QSize& size, const QWidget* widget) const;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
};

class ResourcesManagerDialog : public QDialog{
Q_OBJECT
public:

    ResourcesManagerDialog();
    void update_from_resources_manager(const ResourcesManager *resM);

public slots:

    void update_resources_to_reload();
    void generate_csharp_script();
    void show_section(Resource::Type type);

signals:

    void add_resources_signal(Resource::Type type, QStringList path);
    void update_resource_path_signal(QString currentPath, QString newPath);
    void update_resource_alias_signal(QString currentPath, QString newAlias);

    void resource_selected_signal(Resource::Type type, size_t index);
    void clean_resources_signal(Resource::Type type);
    void remove_resource_signal(Resource::Type type, size_t index);

    void update_reload_resource_code_signal(int code);
private:

    Ui::ResourcesManagerD m_ui;
    umap<Resource::Type, std::tuple<Ui::ResourceTypeW,QWidget*>> m_typesW;
    umap<Resource::Type, int> m_tabsIdW;
};


}
