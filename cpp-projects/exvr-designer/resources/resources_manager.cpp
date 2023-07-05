
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

#include "resources_manager.hpp"

// Qt
#include <QFile>
#include <QDir>

// qt-utility
#include "qt_str.hpp"

using namespace tool;
using namespace tool::ex;

//void ResourcesManager::init(){
//    if(m_resourcesManager == nullptr){
//        m_resourcesManager = std::make_unique<ResourcesManager>();
//    }
//}

//ResourcesManager *ResourcesManager::get(){
//    if(m_resourcesManager != nullptr){
//        return m_resourcesManager.get();
//    }
//    return nullptr;
//}

std::vector<Resource *> ResourcesManager::get_resources(Resource::Type type) const{

    if(m_resourcesPerType.count(type) == 0){
        return {};
    }

    return m_resourcesPerType.at(type);
}

size_t ResourcesManager::get_type_selected_id(Resource::Type type) const{
    if(m_idSelectedPerType.count(type) == 0){
        return 0;
    }
    return m_idSelectedPerType.at(type);
}

Resource *ResourcesManager::get_resource(ResourceKey key) const{
    if(m_resources.count(key.v) == 0){
        return {};
    }
    return m_resources.at(key.v).get();
}

Resource *ResourcesManager::get_resource(Resource::Type type, ResourceKey key, bool errorIfNotFound) const{

    if(m_resourcesPerType.count(type) == 0 || key.v == -1){
        return {};
    }    

    for(const auto &resource : m_resourcesPerType.at(type)){
        if(resource->key() == key.v){
            return resource;
        }
    }

    if(errorIfNotFound){
        QtLogger::error(QSL("Resource of type : ") % from_view(Resource::get_name(type)) % QSL(" with key ") % QString::number(key.v) % QSL(" not found."));
    }

    return nullptr;
}

Resource *ResourcesManager::get_resource(Resource::Type type, const QString &alias, bool errorIfNotFound) const{

    if(m_resourcesPerType.count(type) == 0 || alias.length() == 0){
        return {};
    }

    for(const auto &resource : m_resourcesPerType.at(type)){
        if(resource->alias == alias){
            return resource;
        }
    }

    if(errorIfNotFound){
        QtLogger::error(QSL("Resource of type : ") % from_view(Resource::get_name(type)) % QSL(" with alias ") % alias % QSL(" not found."));
    }

    return nullptr;
}


void ResourcesManager::insert_resource(std::unique_ptr<Resource> resource){

    const auto type  = resource->type;
    const auto key   = resource->key();
    auto ptr         = resource.get();

    m_resourcesPerType[type].push_back(ptr);
    m_idSelectedPerType[type]                   = m_resourcesPerType[type].size()-1;
    m_paths[resource->path.toStdString()]       = ptr;
    m_aliases[resource->alias.toStdString()]    = ptr;

    m_resources[key] = std::move(resource);
}

void ResourcesManager::add_resource(std::unique_ptr<Resource> resourceToAdd){

    const auto path = resourceToAdd->path.toStdString();
    if(m_paths.count(path) != 0){
        QtLogger::error(QSL("Resource with path: ") % resourceToAdd->path % QSL(" already added."));
        return;
    }

    auto type = resourceToAdd->type;
    insert_resource(std::move(resourceToAdd));
    m_idSelectedPerType[type] = m_resourcesPerType[type].size()-1;
}


void ResourcesManager::add_resource(Resource::Type type, const QString &path){

    if(m_paths.count(path.toStdString()) != 0){
        QtLogger::error(QSL("Resource with path: ") % path % QSL(" already added."));
        return;
    }

    insert_resource(std::make_unique<Resource>(type,ResourceKey{-1}, path));
    m_idSelectedPerType[type] = m_resourcesPerType[type].size()-1;
}

void ResourcesManager::add_resources(Resource::Type type, const QStringList &filesPaths){

    for(const auto &filePath : filesPaths){
        if(m_paths.count(filePath.toStdString()) != 0){
            QtLogger::error(QSL("Resource with path: ") % filePath % QSL(" already added."));
            continue;
        }        
        insert_resource(std::make_unique<Resource>(type,ResourceKey{-1}, filePath));
    }

    // current selection for the type
    m_idSelectedPerType[type] = m_resourcesPerType[type].size()-1;
}

void ResourcesManager::copy_resource(Resource *resource){
    add_resource(Resource::copy_with_new_element_id(resource));
}

void ResourcesManager::update_resource_path(QString currentPath, QString newPath){

    if(m_paths.count(newPath.toStdString()) != 0){
        QtLogger::error(QSL("Resource with path: ") % newPath % QSL(" already added."));
        return;
    }

    if(m_paths.count(currentPath.toStdString()) == 0){
        QtLogger::error(QSL("Resource with path: ") % currentPath % QSL(" doesn't exist."));
        return;
    }

    auto resource = m_resources[m_paths[currentPath.toStdString()]->key()].get();
    m_paths.erase(resource->path.toStdString());

    resource->path = newPath;
    m_paths[resource->path.toStdString()] = resource;

}

void ResourcesManager::update_resource_alias(QString currentAlias, QString newAlias){

    if(m_aliases.count(newAlias.toStdString()) != 0){
        QtLogger::error(QSL("Resource with alias: ") % newAlias % QSL(" already added."));
        return;
    }

    if(m_aliases.count(currentAlias.toStdString()) == 0){
        QtLogger::error(QSL("Resource with alias: ") % currentAlias % QSL(" doesn't exist."));
        return;
    }

    auto resource = m_resources[m_aliases[currentAlias.toStdString()]->key()].get();
    m_aliases.erase(resource->alias.toStdString());

    resource->alias = newAlias;
    m_aliases[resource->alias.toStdString()] = resource;
}

void ResourcesManager::select_resource(Resource::Type type, size_t index){

    if(m_resourcesPerType.count(type) == 0){
        // error
        return;
    }

    if(index >= m_resourcesPerType[type].size()){
        // error
        return;
    }

    m_idSelectedPerType[type] = index;
}

void ResourcesManager::clean_resources(Resource::Type type){

    for(const auto &resource : m_resourcesPerType[type]){
        m_paths.erase(resource->path.toStdString());
        m_aliases.erase(resource->alias.toStdString());
        m_resources.erase(resource->key());
    }

    m_resourcesPerType[type].clear();
    m_idSelectedPerType[type] = 0;
}

void ResourcesManager::clean_resources(){

    m_resourcesPerType.clear();
    m_idSelectedPerType.clear();
    m_paths.clear();
    m_aliases.clear();
    m_resources.clear();
    m_reloadCode = reloadDefaultCode;
}

void ResourcesManager::remove_resource(Resource::Type type, size_t index){

    if(m_resourcesPerType.count(type) == 0){
        // error
        return;
    }

    if(index >= m_resourcesPerType[type].size()){
        // error
        return;
    }

    if(m_idSelectedPerType[type] >= index){
        if(m_idSelectedPerType[type] > 0){
            m_idSelectedPerType[type]--;
        }
    }

    auto resourceToRemove = m_resourcesPerType[type].at(index);

    m_paths.erase(resourceToRemove->path.toStdString());
    m_aliases.erase(resourceToRemove->alias.toStdString());

    m_resourcesPerType[type].erase(m_resourcesPerType[type].begin() + to_int(index));
    m_resources.erase(resourceToRemove->key());
}

void ResourcesManager::set_reload_code(int code){m_reloadCode = code;}

int ResourcesManager::reload_code() const {return m_reloadCode;}

void ResourcesManager::export_to(QString path){

    // create readme file
    // ...

    // create resources dir
    QDir resourcesDir(path % QSL("/resources"));
    if (!resourcesDir.exists()){
        resourcesDir.mkpath(".");
    }

    // create resource types subdir
    for(const auto &type : Resource::get_types()){
        if(m_resourcesPerType[type].size() > 0){
            QDir typeDir(path % QSL("/resources/") % from_view(Resource::get_name(type)));
            if (!typeDir.exists()){
                typeDir.mkpath(".");
            }
        }
    }

    // copy resources
    for(const auto &typeResources : m_resourcesPerType){
        QString basePath = path % QSL("/resources/") % from_view(Resource::get_name(typeResources.first)) % QSL("/");
        for(const auto &resource : typeResources.second){
            if(resource->type == Resource::Type::Directory){
                QDir dir(basePath + resource->path.split("/").last());
                if (!dir.exists()){
                    dir.mkdir(".");
                }

            }else{
                QFile::copy(resource->path, basePath + resource->path.split("/").last());
            }
        }
    }
}
