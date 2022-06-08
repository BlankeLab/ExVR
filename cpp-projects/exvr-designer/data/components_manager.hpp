
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

class ComponentsManager{

public:

    using Name  = QString;
    using NameV = QStringView;
    using name_const_iterator = std::map<QStringView, Component*>::const_iterator;
    using key_const_iterator  = std::map<ComponentKey, Component*>::const_iterator;

    // sort
    void sort_by_category();
    void sort_by_type();
    void sort_by_name();

    // add
    void add_component(std::unique_ptr<Component> component);
    void insert_copy_of_component(Component *component, std::vector<ConfigKey> configKeys, RowId id);
    bool insert_new_component(Component::Type type, RowId id);
    void duplicate_component(ComponentKey componentKey);

    // remove
    void remove_component(ComponentKey componentKey);
    void clean_components();

    // update
    void update_component_position(ComponentKey componentKey, RowId id);
    bool update_component_name(ComponentKey componentKey, QString newName);

    // get
    Component *get_component(RowId id, bool displayError= true) const;
    Component *get_component(ComponentKey componentKey, bool displayError= true) const;
    Component *get_component(const Name &name) const;
    std::vector<Component*> get_components() const;
    std::vector<Component*> get_components(Component::Type type) const;


    std::pair<size_t, Component*> get_component_and_position(ComponentKey componentKey) const;
    int get_position(ComponentKey componentKey) const;

    inline size_t count() const {return components.size();}
    inline size_t count(Component::Type type) const {return m_componentsPerType.at(type).size();}
    inline size_t count(Component::Category category) const {return m_componentsPerCategory.at(category).size();}

    bool is_key_used(ComponentKey key) const noexcept;
    bool is_name_used(const Name &name) const noexcept;

    name_const_iterator category_begin(Component::Category category) const { return m_componentsPerCategory.at(category).cbegin(); }
    name_const_iterator category_end(Component::Category category) const { return m_componentsPerCategory.at(category).cend(); }
    name_const_iterator type_begin(Component::Type type) const { return m_componentsPerType.at(type).cbegin(); }
    name_const_iterator type_end(Component::Type type) const { return m_componentsPerType.at(type).cend(); }

    name_const_iterator name_begin() const { return m_componentsPerName.cbegin(); }
    name_const_iterator name_end() const { return m_componentsPerName.cend(); }

private:

    void add_component_to_map(Component *component);
    void remove_component_from_map(Component *component);

    std::vector<std::unique_ptr<Component>> components;
    std::map<Component::Category, std::map<NameV, Component*>> m_componentsPerCategory;
    std::map<Component::Type, std::map<NameV, Component*>> m_componentsPerType;
    std::map<ComponentKey, Component*> m_componentsPerKey;
    std::map<NameV, Component*> m_componentsPerName;

};
}
