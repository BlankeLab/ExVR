
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
#include <QWidget>

// std
#include <memory>

// local
#include "data/component.hpp"
#include "config_parameters/config_pw.hpp"


namespace tool::ex {

class ConfigW : public QWidget{

    Q_OBJECT // necessary for a cast in another class

public:

    ConfigW(Config *config, Component *component, bool initConfig, std::map<QStringView,Arg> &args);
    void update_from_config(Config *config);


    void update_with_info(QStringView id, QStringView value);
    void reset_args();

    ConfigKey configKey;
    ComponentKey componentKey;
    QString name;

private:

    ConfigParametersW *p = nullptr;
    ConfigParametersW *generate_parameters(Component::Type type, bool initConfig);

    template<typename T1, typename T2>
    ConfigParametersW *gen_params_w(bool initConfig){
        if(initConfig){
            return new T1();
        }else{
            return new T2();
        }
    }
};
}
