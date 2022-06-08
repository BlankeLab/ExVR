
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

// qt-utility
#include "data/id_key.hpp"

namespace tool::ex {

struct Set{

    Set() = delete;
    Set(QString name, size_t occurencies = 1, SetKey id = {-1}) : name(name), occurencies(occurencies), m_key(IdKey::Type::Set, id.v) {}
    Set(const Set&) = delete;
    Set& operator=(const Set&) = delete;

    static std::unique_ptr<Set> copy_with_new_element_id(Set *setToCopy){
        return std::make_unique<Set>(setToCopy->name, setToCopy->occurencies, SetKey{-1});
    }

    QString name;
    size_t occurencies = 1;

    constexpr int key() const noexcept{ return m_key();}
    constexpr SetKey s_key() const noexcept {return SetKey{key()};}

private :
    IdKey m_key;
};

[[maybe_unused]] static bool operator==(const Set &l, const Set &r){
    return  (l.name  == r.name);
}


}
