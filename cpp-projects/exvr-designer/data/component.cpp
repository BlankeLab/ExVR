
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

Component::Component(Component::Type t, ComponentKey id, QString name) : key(IdKey::Type::Component, id.v), type(t), category(get_category(type)){

    if(m_counter.count(type) == 0){
        m_counter[type] = 1;
    }else{
        m_counter[type]++;
    }

    if(name.length()==0){
        m_name = QString::fromStdString(std::string(get_full_name(t)));
    }else{
        m_name = name;
    }
}

Component::~Component(){
    if(m_counter.count(type) > 0){
        m_counter[type]--;
    }
}

ComponentUP Component::copy_with_new_element_id(const Component &componentToCopy, const QString &newName){

    ComponentUP component = std::make_unique<Component>(componentToCopy.type, ComponentKey{-1}, newName);
    component->set_init_config(ex::Config::copy_with_new_element_id(*componentToCopy.initConfig.get(), componentToCopy.initConfig->name));
    for(const auto &config : componentToCopy.configs){
        component->add_config(ex::Config::copy_with_new_element_id(*config.get(), config->name));
    }

    return component;
}

QStringList Component::get_configs_name() const{

    QStringList configsList;
    for(const auto &config : configs){
        configsList << config->name;
    }
    return configsList;
}

void Component::set_name(QString name){
    m_name = name;
}

tool::ex::Config *Component::get_config(ConfigKey configKey){

    auto configFound = std::find_if(configs.begin(), configs.end(), [configKey](const ConfigUP &config){
        return config->key() == configKey.v;
    });
    if(configFound != configs.end()){
        return configFound->get();
    }

    QtLogger::error(QSL("Config with key ") % QString::number(configKey.v) % QSL(" not found"));
    return nullptr;
}


