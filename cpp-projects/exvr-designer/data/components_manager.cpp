
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

#include "components_manager.hpp"

// Qt
#include <QStringBuilder>

using namespace tool;
using namespace tool::ex;


void ComponentsManager::init(){
    if(m_componentsManager == nullptr){
        m_componentsManager = std::make_unique<ComponentsManager>();
    }
}

ComponentsManager *ComponentsManager::get(){
    if(m_componentsManager != nullptr){
        return m_componentsManager.get();
    }
    return nullptr;
}

void ComponentsManager::clean_components(){
    components.clear();
}

void ComponentsManager::sort_by_category(){

    std::sort(std::begin(components), std::end(components), [&](const std::unique_ptr<Component> &c1,  const std::unique_ptr<Component> &c2){
        if(c1->category != c2->category){
            return c1->category < c2->category;
        }
        if(c1->type != c2->type){
            return c1->type < c2->type;
        }
        return c1->name().toUpper() < c2->name().toUpper();
    });
}

void ComponentsManager::sort_by_type(){
    std::sort(std::begin(components), std::end(components), [&](const std::unique_ptr<Component> &c1,  const std::unique_ptr<Component> &c2){
        if(c1->type != c2->type){
            return c1->type < c2->type;
        }
        return c1->name().toUpper() < c2->name().toUpper();
    });
}

void ComponentsManager::sort_by_name(){
    std::sort(std::begin(components), std::end(components), [&](const std::unique_ptr<Component> &c1,  const std::unique_ptr<Component> &c2){
        return c1->name().toUpper() < c2->name().toUpper();
    });
}


Component *ComponentsManager::get_component(ComponentKey componentKey, bool displayError) const{

    auto componentFound = std::find_if(std::begin(components), std::end(components), [componentKey](const ComponentUP &component){
        return component->key() == componentKey.v;
    });

    if(componentFound != std::end(components)){
        return componentFound->get();
    }

    if(displayError){
        QtLogger::error(QSL("Component with key ") % QString::number(componentKey.v) % QSL(" not found."));
    }

    return nullptr;
}

Component *ComponentsManager::get_component(Component::Type type, const QString &name) const{
    for(auto &component : get_components(type)){
        if(component->name() == name){
            return component;
        }
    }
    QtLogger::error(QSL("Component with name ") % name % QSL(" not found."));
    return  nullptr;
}

std::pair<size_t, Component *> ComponentsManager::get_component_and_position(ComponentKey componentKey) const{

    for(size_t ii = 0; ii < components.size(); ++ii){
        if(components[ii]->key() == componentKey.v){
            return {ii, components[ii].get()};
        }
    }
    QtLogger::error(QSL("Component with key ") % QString::number(componentKey.v) % QSL(" not found."));
    return {0, nullptr};
}

void ComponentsManager::insert_new_component(Component::Type type, RowId id){

    const QString baseName = QString::fromStdString(std::string(Component::get_full_name(type)));
    size_t offset = 1;
    QString name;
    bool isInside = false;
    do{
        name = baseName %  ((offset != 1) ? (QSL(" ") % QString::number(offset)) : QSL(""));
        isInside = false;
        for(auto &component : components){
            if(component->name() == name){
                isInside = true;
                break;
            }
        }
        ++offset;
    }while(isInside);

    auto component = std::make_unique<Component>(type, ComponentKey{-1}, name);
    component->set_init_config(std::make_unique<Config>(QSL("standard"), ConfigKey{-1}));
    component->add_config(std::make_unique<Config>(QSL("standard"),ConfigKey{-1}));
    components.insert(std::begin(components) + id.v, std::move(component));
}

bool ComponentsManager::update_component_name(ComponentKey componentKey, QString newName){

    if(newName.length() == 0){
        QtLogger::error(QSL("Component name must not be empty."));
        return false;
    }

    for(auto &component : components){
        if(component->name() == newName){
            QtLogger::error(QSL("Component name already used."));
            return false;
        }
    }

    if(auto component = get_component(componentKey); component != nullptr){
        component->set_name(newName);
        return true;
    }
    return false;
}

std_v1<Component*> ComponentsManager::get_components(Component::Type type) const{
    std_v1<Component*> componentsOfType;
    for(auto &component : components){
        if(component->type == type){
            componentsOfType.emplace_back(component.get());
        }
    }
    return componentsOfType;
}

