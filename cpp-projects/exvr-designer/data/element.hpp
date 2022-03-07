
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
#include <memory>
#include <optional>

// Qt
#include <QString>
#include <QList>
#include <QVector>

// base
#include "utility/vector.hpp"
#include "utility/tuple_array.hpp"

// qt-utility
#include "data/id_key.hpp"

namespace tool::ex {

using namespace std::literals::string_view_literals;


struct Element;
using ElementUP = std::unique_ptr<Element>;

struct Element {

public:

    enum class Type : int {
        Node,LoopStart,LoopEnd,Routine,Isi,Loop,AddElement,RemoveElement,MoveElement,Default,
        SizeEnum};

    using Name     = std::string_view;
    using IsButton = bool;
    using TElement = std::tuple<Type, Name, IsButton>;
    static constexpr TupleArray<Type::SizeEnum,TElement> elements ={{
        TElement
        {Type::Node,         "Node"sv,          false},
        {Type::LoopStart,    "LoopStart"sv,     false},
        {Type::LoopEnd,      "LoopEnd"sv,       false},
        {Type::Routine,      "Routine"sv,       false},
        {Type::Isi,          "Isi"sv,           false},
        {Type::Loop,         "Loop"sv,          false},
        {Type::AddElement,   "AddElement"sv,    true},
        {Type::RemoveElement,"RemoveElement"sv, true},
        {Type::MoveElement,  "MoveElement"sv,   true},
        {Type::Default,      "Default"sv,       false}
    }};

    static Type get_type(Name n) {
        return elements.at<1,0>(n);
    }

    static Name get_type_name(Type t) {
        return elements.at<0,1>(t);
    }

    static bool is_button(Type t) {
        return elements.at<0,2>(t);
    }

    Element() = delete;
    Element(Type t, QString n, int id = -1, QString infos = "") :
          key(is_button(t) ? IdKey::Type::ButtonElement :
            IdKey::Type::Element, id), type(t), informations(infos), m_name(n){
    }

    virtual ~Element() = default;

    inline void set_selected(bool select) noexcept {
        m_isSelected = select;
    }

    inline void update_name(QString name)noexcept{
        m_name = name;
    }

    inline QString name()const noexcept{
        return m_name;
    }

    constexpr bool is_selected() const noexcept {
        return m_isSelected;
    }

    constexpr bool is_routine()const{
        return type == Type::Routine;
    }

    constexpr bool is_isi()const{
        return type == Type::Isi;
    }

    virtual void check_integrity(){}

    IdKey key;
    Type type = Type::Default;

    std_v1<int> insideLoopsID;       /**< all loop id containing the element */
    std_v1<Element*> insideLoops;
    QString informations;

private:

    QString m_name = "";
    bool m_isSelected = false;  /**< is the element selected ? */
};

static bool operator<(const ElementUP &l, const ElementUP &r){
    if(l->key() == r->key()){
        return false;
    }
    return true;
}

static bool operator==(const ElementUP &l, const ElementUP &r){
    return !(l < r) && !(r < l);
}
}
