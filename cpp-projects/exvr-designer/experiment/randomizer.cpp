

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

#include "randomizer.hpp"

using namespace tool::ex;

Randomizer::Randomizer(unsigned int seed){
    gen = std::make_unique<std::mt19937>(seed);
}

Randomizer::Randomizer(std::random_device &rd) {
    gen = std::make_unique<std::mt19937>(rd());
}

size_t Randomizer::randomize_start(size_t containerSize) const{
    std::uniform_int_distribution<> dist(0, static_cast<int>(containerSize)-1);
    return static_cast<size_t>(dist(*gen));
}

std::vector<int> Randomizer::generate_shuffle_ids(size_t nbValues, size_t reps, bool noFollowingValue)const{

    std::vector<int> shuffleIds;

    if(!noFollowingValue){

        shuffleIds.reserve(reps);

        std::vector<int> ids(nbValues);
        while(shuffleIds.size() < reps){

            std::iota(ids.begin(),ids.end(),0);
            std::shuffle(ids.begin(),ids.end(), *gen);

            for(auto id : ids){
                if(shuffleIds.size() == reps){
                    break;
                }
                shuffleIds.push_back(id);
            }
        }

    }else{

        if(nbValues == 1){
            shuffleIds.resize(reps);
            std::fill(shuffleIds.begin(), shuffleIds.end(), 0);
        }else if (nbValues == 2){

            std::vector<int> ids = {0,1};
            std::shuffle(ids.begin(),ids.end(), *gen);

            shuffleIds.reserve(reps);
            for(size_t ii = 0; ii < reps; ++ii){
                shuffleIds.push_back(ii % 2 == 0 ? ids[0] : ids[1]);
            }

        }else{

            std::vector<int> ids(nbValues);
            int prevLast = -1;

            while(shuffleIds.size() < reps){

                std::iota(ids.begin(),ids.end(),0);
                std::shuffle(ids.begin(),ids.end(), *gen);

                while(ids[0] == prevLast){
                    std::shuffle(ids.begin(),ids.end(), *gen);
                }
                prevLast = ids[ids.size()-1];

                for(auto id : ids){
                    if(shuffleIds.size() == reps){
                        break;
                    }
                    shuffleIds.push_back(id);
                }
            }
        }
    }
    return shuffleIds;
}

std::vector<int> Randomizer::generate_random_ids(size_t nbValues, size_t reps, bool noFollowingValue)const{

    std::vector<int> randomIds;

    if(!noFollowingValue){
        randomIds.reserve(reps);
        std::uniform_int_distribution<> dist(0,static_cast<int>(nbValues)-1);
        for(size_t ii = 0; ii < reps; ++ii){
            randomIds.push_back(dist(*gen));
        }

    }else{
        if(nbValues == 1){
            randomIds.resize(reps);
            std::fill(randomIds.begin(), randomIds.end(), 0);
        }else if (nbValues == 2){

            std::vector<int> ids = {0,1};
            std::shuffle(ids.begin(),ids.end(), *gen);

            randomIds.reserve(reps);
            for(size_t ii = 0; ii < reps; ++ii){
                randomIds.push_back(ii % 2 == 0 ? ids[0] : ids[1]);
            }
        }else{

            randomIds.reserve(reps);
            std::uniform_int_distribution<> dist(0,static_cast<int>(nbValues)-1);

            int prevLast = -1;
            while(randomIds.size() < reps){

                int id = dist(*gen);
                while(id == prevLast){
                    id = dist(*gen);
                }
                prevLast = id;
                randomIds.push_back(id);
            }
        }
    }
    return randomIds;
}
