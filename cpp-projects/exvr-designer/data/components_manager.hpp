
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

// std
#include <unordered_map>
#include <set>

// Qt
#include <QStringList>
#include <QDebug>

// base
#include "utility/vector.hpp"

// qt-utility
#include "qt_logger.hpp"

// local
#include "component.hpp"

namespace tool::ex {

class ComponentsManager;
using ComponentsManagerUP = std::unique_ptr<ComponentsManager>;

class ComponentsManager{

public:

    static void init();
    static ComponentsManager *get();

    void clean_components();
    void sort_by_category();
    void sort_by_type();
    void sort_by_name();

    Component *get_component(ComponentKey componentKey, bool displayError= true) const;
    Component *get_component(Component::Type type, const QString &name) const;

    std::pair<size_t, Component*> get_component_and_position(ComponentKey componentKey) const;

    void insert_new_component(Component::Type type, RowId id);
    bool update_component_name(ComponentKey componentKey, QString newName);

    std_v1<Component*> get_components(Component::Type type) const;    

    std_v1<ComponentUP> components;


private:

    static inline ComponentsManagerUP m_componentsManager = nullptr;

    // TODO: add map (for quick access to types)
};
}
