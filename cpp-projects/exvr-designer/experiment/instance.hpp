
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

// qt-utility
#include "qt_str.hpp"

// local
#include "experiment.hpp"
#include "randomizer.hpp"

namespace tool::ex {

struct LoopInfo{
    int currentRep = 0;
    int idStart;
    int idEnd;
    QStringList set;
    Loop *l = nullptr;
};

struct InstanceElement{
    int elementIteration = 0;
    int conditionIteration = 0;
    Element *elem;
    QString condition;
};

struct Instance {

    Instance() = default;
    Instance(const Randomizer *randomizer, const std_v1<Element*> &elements, size_t idInstance);

    static std::unique_ptr<Instance> generate_from_full_experiment(const Randomizer *randomizer, const Experiment &experiment, size_t idInstance);
    static std::unique_ptr<Instance> generate_from_one_routine(Routine *routine);
    static std::unique_ptr<Instance> generate_from_element_to_the_end(const Randomizer *randomizer, const Experiment &experiment, size_t idInstance);
    static std::unique_ptr<Instance> generate_from_start_to_element(const Randomizer *randomizer, const Experiment &experiment, size_t idInstance);

    static inline std::unordered_map<int, std_v1<QString>> onlyOnceShuffleLoopSets = {};
    static inline std::unordered_map<int, std_v1<QString>> onlyOnceRandomLoopSets = {};
    static inline std::unordered_map<int, std_v1<QString>> everyNShuffleLoopSets = {};
    static inline std::unordered_map<int, std_v1<QString>> everyNRandomLoopSets = {};

    QString filePath = "";
    QString fileName = "debug-instance";
    size_t idInstance = 0;
    std_v1<InstanceElement> flow;
};

}
