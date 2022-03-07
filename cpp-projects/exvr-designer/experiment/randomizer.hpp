
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
#include <random>
#include <algorithm>
#include <deque>

// Qt
#include <QStringList>
#include <QVector>

// base
#include "utility/vector.hpp"

namespace tool::ex {

class Randomizer{

    std::unique_ptr<std::mt19937> gen = nullptr;

public :

    Randomizer(unsigned int seed){
        gen = std::make_unique<std::mt19937>(seed);
    }

    Randomizer( std::random_device &rd) {
        gen = std::make_unique<std::mt19937>(rd());
    }


    size_t randomize_start(size_t containerSize) const{
        std::uniform_int_distribution<> dist(0, static_cast<int>(containerSize)-1);
        return static_cast<size_t>(dist(*gen));
    }

    template<typename T>
    std_v1<T> shuffle(const std_v1<T> &values, size_t reps) const{

        std_v1<int> res;
        res.reserve(reps);
        std_v1<int> ids(values.size());

        while(res.size() < reps){

            std::iota(ids.begin(),ids.end(),0);
            std::shuffle(ids.begin(),ids.end(), *gen);

            for(auto & id : ids){
                if(res.size() == reps){
                    break;
                }
                res.emplace_back(id);
            }
        }

        std_v1<T> randomSet;
        randomSet.reserve(res.size());
        for(auto &id : res){
            randomSet.emplace_back(values[id]);
        }
        return randomSet;
    }

    template<typename T>
    std_v1<T> randomize(const std_v1<T> &values, size_t reps) const{

        std_v1<T> res;        
        if(values.size() == 0 || reps <= 0){
            return res;
        }
        res.reserve(reps);

        std::uniform_int_distribution<> dist(0,static_cast<int>(values.size())-1);
        for(size_t ii = 0; ii < reps; ++ii){
            res.emplace_back(values[dist(*gen)]);
        }

        return  res;
    }

};
}
