
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
#include <QString>
#include <QVector>
#include <QDebug>

// qt-utility
#include "data/argument.hpp"

namespace tool::ex {


struct Config;
using ConfigUP = std::unique_ptr<Config>;

struct Config{

    Config() = delete;
    Config(const QString &n, ConfigKey id) : name(n), key(IdKey::Type::Config, id.v){}

    static ConfigUP copy_with_new_element_id(const Config &configToCopy, const QString &newName);

    void update_arg(Arg arg);
    void add_arg(Arg arg);
    void remove_arg(QStringView argName);
    void swap_arg(QStringView arg1Name, QStringView arg2Name);
    void fix();

    inline QString to_string() const{return QSL("Config(") % name % QSL("|") % QString::number(key()) % QSL(")");}

    QString name{QSL("standard")};
    IdKey key;

    std::map<QStringView, Arg> args;
};
}
