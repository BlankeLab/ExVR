
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

// qt-utility
#include "qt_str.hpp"

using namespace tool;
using namespace tool::ex;


void ComponentsManager::clean_components(){

    components.clear();
    m_componentsPerCategory.clear();
    m_componentsPerType.clear();
    m_componentsPerKey.clear();
    m_componentsPerName.clear();

    for(auto category : Component::all_categories()){
        m_componentsPerCategory[category] = {};
    }

    for(auto type : Component::all_components_types()){
        m_componentsPerType[type] = {};
    }
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

void ComponentsManager::add_component(std::unique_ptr<Component> component){
    add_component_to_map(component.get());
    components.push_back(std::move(component));
}

void ComponentsManager::duplicate_component(ComponentKey componentKey){

    if(const auto compoInfo = get_component_and_position(componentKey); compoInfo.second != nullptr){
        if(Component::get_unicity(compoInfo.second->type)){
            QtLogger::error(QSL("[ComponentsManager::duplicate_component] You can only have one component of type [") % from_view(Component::get_type_name(compoInfo.second->type)) % QSL("] in the experiment."));
        }else{

            auto component = Component::copy_with_new_element_id(compoInfo.second, compoInfo.second->name() % QSL("(copy)"));

            add_component_to_map(component.get());

            components.insert(
                components.begin() + static_cast<std::vector<std::unique_ptr<Component>>::difference_type>(compoInfo.first + 1),
                std::move(component)
            );                        
        }
    }
}

void ComponentsManager::remove_component(ComponentKey componentKey){

    auto componentToRemove = get_component(componentKey);
    if(componentToRemove == nullptr){
        return;
    }

    for(size_t id = 0; id < components.size(); ++id){
        if(components[id]->key() == componentToRemove->key()){

            remove_component_from_map(components[id].get());

            QtLogger::message(QSL("Remove component ") % components[id]->to_string());
            components.erase(components.begin() + static_cast<int>(id));            
            break;
        }
    }
}

void ComponentsManager::update_component_position(ComponentKey componentKey, RowId id){

    if(const auto compoInfo = get_component_and_position(componentKey); compoInfo.second != nullptr){
        auto compoToMove = std::move(components[compoInfo.first]);
        components.erase(components.begin() + static_cast<std::vector<std::unique_ptr<Component>>::difference_type>(compoInfo.first));
        components.insert(components.begin() + id.v, std::move(compoToMove));
    }else{
        QtLogger::error(QSL("[ComponentsManager::update_component_position] Cannot update component position."));
    }
}

Component *ComponentsManager::get_component(RowId id, bool displayError) const{

    if(id.v < static_cast<int>(components.size())){
        return components[id.v].get();
    }
    if(displayError){
        QtLogger::error(QSL("[ComponentsManager::get_component] Component from row [") % QString::number(id.v) % QSL("] not found."));
    }

    return nullptr;
}


Component *ComponentsManager::get_component(ComponentKey componentKey, bool displayError) const{

    if(m_componentsPerKey.contains(componentKey)){
        return m_componentsPerKey.at(componentKey);
    }

    if(displayError){
        QtLogger::error(QSL("[ComponentsManager::get_component] Component with key [") % QString::number(componentKey.v) % QSL("] not found."));
    }

    return nullptr;
}

Component *ComponentsManager::get_component(const QString &name) const{

    if(m_componentsPerName.contains(name)){
        return m_componentsPerName.at(name);
    }

    QtLogger::error(QSL("[ComponentsManager::get_component] Component with name [") % name % QSL("] not found."));
    return  nullptr;
}


std::pair<size_t, Component *> ComponentsManager::get_component_and_position(ComponentKey componentKey) const{   

    for(size_t ii = 0; ii < components.size(); ++ii){
        if(components[ii]->key() == componentKey.v){
            return {ii, components[ii].get()};
        }
    }
    QtLogger::error(QSL("[ComponentsManager::get_component_and_position] Component with key [") % QString::number(componentKey.v) % QSL("] and its position not found."));
    return {0, nullptr};
}

int ComponentsManager::get_position(ComponentKey componentKey) const{
    for(size_t ii = 0; ii < components.size(); ++ii){
        if(components[ii]->key() == componentKey.v){
            return static_cast<int>(ii);
        }
    }
    QtLogger::error(QSL("[ComponentsManager::get_position] Component with key [") % QString::number(componentKey.v) % QSL("] cannot be found."));
    return -1;
}

void ComponentsManager::insert_copy_of_component(Component *component, std::vector<ConfigKey> configKeys, RowId id){

    if(Component::get_unicity(component->type) && count(component->type) > 0){
        QtLogger::error(QSL("[ComponentsManager::insert_copy_of_component] Unique component already inside experiment."));
        return;
    }

    size_t offset = 1;
    QString name;
    bool isInside = false;
    do{
        name = component->name() %  ((offset != 1) ? (QSL(" ") % QString::number(offset)) : QSL(""));
        isInside = false;
        for(auto &component : components){
            if(component->name() == name){
                isInside = true;
                break;
            }
        }
        ++offset;
    }while(isInside);

    auto nComponent = Component::copy_with_new_element_id(component, name % QSL("(imported)"), std::move(configKeys));

    add_component_to_map(nComponent.get());

    components.insert(
        std::begin(components) + id.v,
        std::move(nComponent)
    );
}

bool ComponentsManager::insert_new_component(Component::Type type, RowId id){

    if(Component::get_unicity(type) && count(type) > 0){
        QtLogger::error(QSL("[ComponentsManager::insert_new_component] Component of type [") % from_view(Component::get_type_name(type)) % QSL("] can only be included once per experiment."));
        return false;
    }

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


    auto component =
        std::make_unique<Component>(
        type, ComponentKey{-1}, name,
        std::make_unique<Config>("standard", ConfigKey{-1})
    );
    component->add_config(std::make_unique<Config>("standard", ConfigKey{-1}));

    add_component_to_map(component.get());

    components.insert(
        std::begin(components) + id.v,
        std::move(component)
    );
    return true;
}

bool ComponentsManager::update_component_name(ComponentKey componentKey, QString newName){

    auto component = get_component(componentKey);
    if(!component){
        return false;
    }

    if(newName.length() == 0){
        QtLogger::error(QSL("[ComponentsManager::update_component_name] Component name must not be empty."));
        return false;
    }

    if(m_componentsPerName.contains(newName)){
        QtLogger::error(QSL("[ComponentsManager::update_component_name] Component name already used."));
        return false;
    }

    auto node = m_componentsPerName.extract(component->name());
    node.key() = newName;
    m_componentsPerName.insert(std::move(node));

    component->set_name(newName);

    return true;
}

std::vector<Component *> ComponentsManager::get_components() const{

    std::vector<Component*> componentsPtr;
    componentsPtr.reserve(components.size());
    for(auto &component : components){
        componentsPtr.push_back(component.get());
    }
    return componentsPtr;
}

std::vector<Component *> ComponentsManager::get_components(Component::Type type) const{

    std::vector<Component*> componentsPtr;
    componentsPtr.reserve(count(type));
    for(auto component : m_componentsPerType.at(type)){
        componentsPtr.push_back(component.second);
    }
    return componentsPtr;
}

std::vector<Component *> ComponentsManager::get_components(Component::Category category, bool canBeLogged) const{
    std::vector<Component*> componentsPtr;
    componentsPtr.reserve(count(category));
    for(auto component : m_componentsPerCategory.at(category)){
        if(canBeLogged){
            if(Component::has_frame_logging(component.second->type) || Component::has_trigger_logging(component.second->type)){
                componentsPtr.push_back(component.second);
            }
        }else{
            componentsPtr.push_back(component.second);
        }
    }
    return componentsPtr;
}

bool ComponentsManager::is_key_used(ComponentKey key) const noexcept{
    return m_componentsPerKey.contains(key);
}

bool ComponentsManager::is_name_used(const QString &name) const noexcept{
    return m_componentsPerName.contains(name);
}

void ComponentsManager::fix_colors(){
    for(auto &component : components){
        for(auto &arg : component->initConfig->args){
            if(arg.second.associated_ui_type() == UiType::Color_pick){
                qDebug() << "arg" << arg.first << arg.second.value();
                auto split = arg.second.value().split(" ");
                QStringList nV;
                nV << split[1];
                nV << split[2];
                nV << split[3];
                nV << split[0];
                arg.second.set_value(nV.join(" "));
                qDebug() << "->" << arg.first << arg.second.value();
            }
        }

        for(auto &config : component->configs){
            for(auto &arg : config->args){
                if(arg.second.associated_ui_type() == UiType::Color_pick){
                    qDebug() << "arg" << arg.first << arg.second.value();
                    auto split = arg.second.value().split(" ");
                    QStringList nV;
                    nV << split[1];
                    nV << split[2];
                    nV << split[3];
                    nV << split[0];
                    arg.second.set_value(nV.join(" "));
                    qDebug() << "->" << arg.first << arg.second.value();
                }
            }
        }
    }
}

void ComponentsManager::add_component_to_map(Component *component){

    auto category = Component::get_category(component->type);
    m_componentsPerCategory[category][component->name()] = component;
    m_componentsPerType[component->type][component->name()] = component;
    m_componentsPerKey[component->c_key()] = component;
    m_componentsPerName[component->name()] = component;
}

void ComponentsManager::remove_component_from_map(Component *component){

    auto category = Component::get_category(component->type);
    m_componentsPerCategory[category].erase(component->name());
    m_componentsPerType[component->type].erase(component->name());
    m_componentsPerKey.erase(component->c_key());
    m_componentsPerName.erase(component->name());
}


