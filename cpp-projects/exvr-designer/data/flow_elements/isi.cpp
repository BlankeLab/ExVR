
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

#include "isi.hpp"

using namespace tool;
using namespace tool::ex;



IsiUP Isi::copy_with_new_element_id(const Isi &isiToCopy, const QString &newName){

    IsiUP isi = std::make_unique<Isi>(newName, ElementKey{-1}, isiToCopy.informations);
    isi->randomized = isiToCopy.randomized;
    isi->intervals = isiToCopy.intervals;
    isi->currentIntervalId = isiToCopy.currentIntervalId;
    return isi;
}

void Isi::set_randomize(bool randomize) {
    this->randomized = randomize;
}

void Isi::modify_interval(double value, RowId id){
    intervals[id.v] = value;
}

void Isi::remove_interval(RowId id){
    intervals.erase(intervals.begin()+id.v);
    if(id.v > 0){
        currentIntervalId = to_unsigned(id.v-1);
    }
}

void Isi::add_interval(double value, RowId id){
    intervals.insert(intervals.begin()+ id.v, value);
    currentIntervalId = id.v;
}

void Isi::move_interval_up(RowId id){
    std::swap(intervals[id.v], intervals[id.v-1]);
    currentIntervalId = id.v-1;
}

void Isi::move_interval_down(RowId id){
    std::swap(intervals[id.v], intervals[id.v + 1]);
    currentIntervalId = id.v + 1;
}

QString Isi::str_intervals() const{
    QString str;
    for(const auto& interval : intervals){
        str += QString::number(interval) % QSL(" ");
    }
    if(str.size() > 0){
        str.remove(str.size()-1,1);
    }
    return str;
}
