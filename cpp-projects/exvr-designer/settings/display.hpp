
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
#include <unordered_map>

// Qt
#include <QColor>

// local
#include "data/element.hpp"

namespace tool::ex::display {

struct Colors{

    struct FlowElement{

        QColor selectedTextColor;
        QColor unselectedTextColor;
        QColor selectedLineBoxColor;
        QColor unselectedLineBoxColor;
        QColor selectedFillBoxColor;
        QColor unselectedFillBoxColor;
    };

    static inline QColor inused = Qt::black;

    using QC = QColor;

    static inline const std::unordered_map<Element::Type, FlowElement> flowElements = {
                                         // sel_txt     unsel_txt       sel_line        unsel_line      sel_box         unsel_box
        {Element::Type::Node,           {QC("#000000"), QC("#000000"),  QC("#000000"),  QC("#000000"),  QC("#000000"),  QC("#000000")}},
        {Element::Type::LoopStart,      {QC("#FFFFFF"), QC("#FFFFFF"),  QC("#427b89"),  QC("#000000"),  QC("#427b89"),  QC("#427b89")}},
        {Element::Type::LoopEnd,        {QC("#FFFFFF"), QC("#FFFFFF"),  QC("#427b89"),  QC("#000000"),  QC("#427b89"),  QC("#427b89")}},
        {Element::Type::Routine,        {QC("#FFFFFF"), QC("#FFFFFF"),  QC("#279340"),  QC("#000000"),  QC("#279340"),  QC("#279340")}},
        {Element::Type::Isi,            {QC("#FFFFFF"), QC("#FFFFFF"),  QC("#ff8c00"),  QC("#000000"),  QC("#ff8c00"),  QC("#ff8c00")}},
        {Element::Type::Loop,           {QC("#FFFFFF"), QC("#FFFFFF"),  QC("#427b89"),  QC("#000000"),  QC("#427b89"),  QC("#427b89")}},
        {Element::Type::AddElement,     {QC("#FFFFFF"), inused,         QC("#000000"),  inused,         QC("#FFFFFF"),  inused}},
        {Element::Type::RemoveElement,  {QC("#FFFFFF"), inused,         QC("#000000"),  inused,         QC("#000000"),  inused}},
        {Element::Type::MoveElement,    {QC("#000000"), inused,         QC("#000000"),  inused,         QC("#FFFFFF"),  inused}},
        {Element::Type::MoveElement,    {QC("#000000"), inused,         QC("#000000"),  inused,         QC("#FFFFFF"),  inused}},
        {Element::Type::Default,        {inused,        inused,         inused,         inused,         inused,         inused}},
    };

};
}
