
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

#include "component.hpp"

// qt-utility
#include "qt_logger.hpp"

using namespace tool::ex;


Component::Component(Type t, ComponentKey id, QString name, std::unique_ptr<Config> initConfig) : type(t), category(get_category(type)), m_key(IdKey::Type::Component, id.v){

    if(name.length()==0){
        m_name = QString::fromStdString(std::string(get_full_name(t)));
    }else{
        m_name = name;
    }

    this->initConfig = std::move(initConfig);
}

std::unique_ptr<Component> Component::copy_with_new_element_id(Component *componentToCopy, const QString &newName, std::vector<ConfigKey> configKeys){

    auto component = std::make_unique<Component>(
        componentToCopy->type,
        ComponentKey{-1},
        newName,
        Config::copy_with_new_element_id(*componentToCopy->initConfig, QSL("standard")
    ));

    if(configKeys.size() == 0){
        for(auto &config : componentToCopy->configs){
            component->add_config(ex::Config::copy_with_new_element_id(*config, config->name));
        }
    }else{
        for(const auto &configKey : configKeys){
            if(auto config = componentToCopy->get_config(configKey); config != nullptr){
                component->add_config(ex::Config::copy_with_new_element_id(*config, config->name));
            }
        }
    }

    return component;
}


void Component::add_config(std::unique_ptr<Config> config){
    configs.push_back(std::move(config));
}

bool Component::insert_config(RowId id, QString configName){

    for(const auto &config : configs){
        if(config->name == configName){
            QtLogger::error(QSL("[Component::insert_config] Config ") % config->to_string() % QSL(" already exists in ") % to_string());
            return false;
        }
    }

    configs.insert(configs.begin() + id.v + 1, std::make_unique<Config>(std::move(configName), ConfigKey{-1}));
    selectedConfigId = {id.v + 1};

    return true;
}

bool Component::select_config(RowId id){

    if(id.v < static_cast<int>(configs.size())){
        selectedConfigId = id;
        return true;
    }
    QtLogger::error(QSL("[Component::select_config] Config with row id ") % QString::number(id.v) % QSL(" not found in ") % to_string());
    return false;
}

bool Component::remove_config(RowId id){

    if(id.v < static_cast<int>(configs.size())){

        QtLogger::message(QSL("[INFO] Remove ") % configs[id.v]->to_string() % QSL(" from ") % to_string());
        configs.erase(configs.begin() + id.v);

        selectedConfigId = {id.v-1};
        if(selectedConfigId.v < 0){
            selectedConfigId.v = 0;
        }

        return true;
    }
    QtLogger::error(QSL("[Component::remove_config] Config with row id [") % QString::number(id.v) % QSL("] not found in ") % to_string());
    return false;
}

bool Component::move_config(RowId from, RowId to){

    if(from.v >= static_cast<int>(configs.size())){
        QtLogger::error(QSL("[Component::move_config] Config with row id [") % QString::number(from.v) % QSL("] not found in ") % to_string());
        return false;
    }
    if(to.v >= static_cast<int>(configs.size())){
        QtLogger::error(QSL("[Component::move_config] Config with row id [") % QString::number(to.v) % QSL("] not found in ") % to_string());
        return false;
    }

    std::swap(configs[from.v], configs[to.v]);
    selectedConfigId = to;

    return false;
}

bool Component::rename_config(RowId id, QString configName){

    if(id.v < static_cast<int>(configs.size())){

        for(const auto &config : configs){
            if(config->name == configName){
                QtLogger::error(QSL("[Component::rename_config] Config with name [") % configName % QSL("] already exists in ") % to_string());
                return false;
            }
        }

        configs[id.v]->name = configName;
        return true;
    }
    QtLogger::error(QSL("[Component::rename_config] Config with row id [") % QString::number(id.v) % QSL("] not found in ") % to_string());
    return false;
}

bool Component::copy_config(RowId id, QString configName){

    if(id.v < static_cast<int>(configs.size())){

        for(const auto &config : configs){
            if(config->name == configName){
                QtLogger::error(QSL("[Component::copy_config] Config with name [") % configName % QSL("] already exists in ") % to_string());
                return false;
            }
        }

        configs.insert(
            configs.begin() + id.v + 1,
            Config::copy_with_new_element_id(*configs[id.v], configName)
        );
        selectedConfigId = {id.v + 1};

        return true;
    }

    QtLogger::error(QSL("[Component::copy_config] Config with row id [") % QString::number(id.v) % QSL("] not found in ") % to_string());
    return false;
}


Config *Component::get_config(RowId id) const {

    if(id.v < static_cast<int>(configs.size())){
        return configs[id.v].get();
    }
    QtLogger::error(QSL("[Component::get_config] Config with row id [") % QString::number(id.v) % QSL("] not found in ") % to_string());
    return nullptr;
}

tool::ex::Config *Component::get_config(ConfigKey configKey) const {

    auto configFound = std::find_if(configs.begin(), configs.end(), [configKey](const std::unique_ptr<Config> &config){
        return config->key() == configKey.v;
    });
    if(configFound != configs.end()){
        return (*configFound).get();
    }

    QtLogger::error(QSL("[Component::get_config] Config with key [") % QString::number(configKey.v) % QSL("] not found in ") % to_string());
    return nullptr;
}

QStringList Component::get_configs_name() const {

    QStringList configsList;
    configsList.reserve(configs.size());
    for(const auto &config : configs){
        configsList << config->name;
    }
    return configsList;
}

