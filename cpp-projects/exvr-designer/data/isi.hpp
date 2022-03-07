
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
#include <deque>

// base
#include "utility/types.hpp"

// qt-utility
#include "qt_str.hpp"

// local
#include "element.hpp"

namespace tool::ex {

struct Isi;
using IsiUP = std::unique_ptr<Isi>;

// TODO
struct Duration{
    double value = 1.0;
};

struct Isi : public Element  {

    Isi() : Element(Type::Isi, "isi"){
    }

    Isi(QString n, ElementKey id, QString infos ="") : Element(Type::Isi, n, id.v, infos){
    }

    inline QString to_string() const{return QSL("Isi(") % name() % QSL("|") % QString::number(key()) % QSL(")");}

    static IsiUP copy_with_new_element_id(const Isi &isiToCopy, const QString &newName);


    void set_randomize(bool randomize);

    void add_interval(double value, RowId id);
    void modify_interval(double value, RowId id);
    void remove_interval(RowId id);
    void move_interval_up(RowId id);
    void move_interval_down(RowId id);

    QString str_intervals()const;

    bool randomized;
    std_v1<double> intervals = {1.0};
    size_t currentIntervalId = 0;
};
}
