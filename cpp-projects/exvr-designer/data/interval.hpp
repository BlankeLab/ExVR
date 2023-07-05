
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
#include "utility/math.hpp"

// qt-utility
#include "qt_str.hpp"

namespace tool::ex {

struct Interval{

    Interval(SecondsTS tS, SecondsTS tE) : start(tS), end(tE){}
    Interval(const Interval &interval) = default;

    void merge_with(const Interval &i);

    bool inside(SecondsTS time) const;
    static bool inside(SecondsTS start, SecondsTS end, SecondsTS time);
    bool collide(const Interval &interval);

    Seconds length() const;

    inline QString to_string() const{return QSL("[") % QString::number(start.v) % QSL(", ") % QString::number(end.v) % QSL("]");}

    SecondsTS start{0.};
    SecondsTS end{0.};
};

[[maybe_unused]] static bool compare_intervals(const Interval &i1, const Interval &i2){
    if(almost_equal(i1.start.v,i2.start.v)){
        return i1.end.v < i2.end.v;
    }
    return i1.start.v < i2.start.v;
}

}
