
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

// base
#include "utility/types.hpp"

// qt-utility
#include "qt_str.hpp"
#include "data/id_key.hpp"

namespace tool::ex {

struct Interval{

    Interval(): key(IdKey::Type::Interval, -1){}
    Interval(SecondsTS tS, SecondsTS tE, IntervalKey id) : key(IdKey::Type::Interval, id.v), start(tS), end(tE){}

    static Interval copy_with_new_element_id(const Interval &intervalToCopy){
        return Interval(intervalToCopy.start, intervalToCopy.end, IntervalKey{-1});
    }

    bool inside(SecondsTS time) const;
    bool collide(Interval interval);
    Seconds length() const;

    IdKey key;
    SecondsTS start{0.};
    SecondsTS end{0.};

    void merge(const Interval &i){
        start = SecondsTS{std::min(start.v, i.start.v)};
        end   = SecondsTS{std::max(end.v,   i.end.v)};
    }

    inline QString to_string() const{return QSL("[") % QString::number(start.v) % QSL(", ") % QString::number(end.v) % QSL("]");}
};

    [[maybe_unused]] static Interval merge_intervals(const Interval &i1, const Interval &i2){
        return Interval(SecondsTS{std::min(i1.start.v,i2.start.v)},
                        SecondsTS{std::max(i1.end.v, i2.end.v)}, IntervalKey{i1.key()});
    }

    [[maybe_unused]] static bool compare_intervals(const Interval &i1, const Interval &i2){
        if(almost_equal(i1.start.v,i2.start.v)){
            return i1.end.v < i2.end.v;
        }
        return i1.start.v < i2.start.v;
    }

}
