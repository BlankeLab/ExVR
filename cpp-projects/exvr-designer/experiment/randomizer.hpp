
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
#include <vector>

namespace tool::ex {

class Randomizer{

    std::unique_ptr<std::mt19937> gen = nullptr;

public :

    Randomizer(unsigned int seed);
    Randomizer( std::random_device &rd);

    std::vector<int> generate_shuffle_ids(size_t nbValues, size_t reps, bool noFollowingValue)const;
    std::vector<int> generate_random_ids(size_t nbValues, size_t reps, bool noFollowingValue)const;
    size_t randomize_start(size_t containerSize) const;

    template<typename T>
    std::vector<T> shuffle(const std::vector<T> &values, size_t reps, bool noFollowingValue) const{

        std::vector<T> shuffleSet;
        if(values.size() == 0 || reps <= 0){
            return shuffleSet;
        }
        shuffleSet.reserve(reps);

        for(auto id : generate_shuffle_ids(values.size(), reps, noFollowingValue)){
            shuffleSet.push_back(values[id]);
        }
        return shuffleSet;
    }


    template<typename T>
    std::vector<T> randomize(const std::vector<T> &values, size_t reps, bool noFollowingValue) const{

        std::vector<T> randomSet;
        if(values.size() == 0 || reps <= 0){
            return randomSet;
        }
        randomSet.reserve(reps);

        for(auto id : generate_random_ids(values.size(), reps, noFollowingValue)){
            randomSet.push_back(values[id]);
        }
        return  randomSet;
    }

};
}
