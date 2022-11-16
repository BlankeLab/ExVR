
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

#include "loop.hpp"

// local
#include "qt_logger.hpp"

using namespace tool::ex;


Loop::Loop() : FlowElement(Type::Loop, QSL("loop")){
    sets.push_back(std::make_unique<Set>(QSL("default")));
}

Loop::Loop(QString n, ElementKey id, QString infos) : FlowElement(Type::Loop, n, id.v, infos){
    if(id.v == -1){
        sets.push_back(std::make_unique<Set>(QSL("default")));
    }
}

std::unique_ptr<Loop> Loop::copy_with_new_element_id(const Loop &loopToCopy, const QString &newName){

    auto loop = std::make_unique<Loop>(newName, ElementKey{-1}, loopToCopy.informations);
    loop->nbReps            = loopToCopy.nbReps;
    loop->mode              = loopToCopy.mode;
    loop->noFollowingValues = loopToCopy.noFollowingValues;

    loop->sets.reserve(loopToCopy.sets.size());
    for(const auto &setToCopy : loopToCopy.sets){
        loop->sets.push_back(Set::copy_with_new_element_id(setToCopy.get()));
    }

    return loop;
}

void Loop::set_nodes(LoopNode *start, LoopNode *end){
    this->start = start;
    this->end   = end;
}

void Loop::set_nb_reps(size_t nbReps) noexcept{
    this->nbReps = nbReps;
}

void Loop::set_N(int N) noexcept{
    this->N = N;
}

void Loop::set_loop_type(Loop::Mode mode) noexcept{
    this->mode = mode;
}

bool Loop::is_default() const{

    if(sets.size() == 1){
        if(sets[0]->name == QSL("default")){
            return true;
        }
    }
    return false;
}

void Loop::set_sets(QStringList setsName){

    std::set<QString> validNames;
    sets.clear();

    for(const auto &setName: setsName){

        // is empty?
        if(setName.length() == 0){
            continue;
        }
        // is using "default"?
        if(setName == QSL("default")){
            QtLogger::error(QSL("[LOOP] Cannot add \"default\" as set name"));
            continue;
        }
        // is duplicate?
        if(validNames.contains(setName)){
            QtLogger::error(QSL("[LOOP] Name \"") % setName % QSL("\" is inserted more than once in the list."));
            continue;
        }
        validNames.insert(setName);

        // add valid set
        sets.push_back(std::make_unique<Set>(setName, 1, SetKey{-1}));
    }
}

bool Loop::add_set(QString setName, RowId id) {

    // cannot add default as set name
    if(setName == QSL("default")){
        QtLogger::error(QSL("[LOOP] Cannot add \"default\" as set name"));
        return false;
    }

    if(is_default()){
        sets[0]->name = setName;
        return true;
    }

    // name already used
    for(const auto &set : sets){
        if(set->name == setName){
            QtLogger::error(QSL("[LOOP] Loop already contains a set named \"") % setName % QSL("\""));
            return false;
        }
    }

    sets.insert(std::begin(sets) + id.v, std::make_unique<Set>(setName));

    return true;
}

void Loop::add_sets(QStringList setsName, RowId id){

    std::set<QString> validNames;
    for(const auto &set : sets){
        validNames.insert(set->name);
    }

    int v = 0;
    for(const auto &setName : setsName){

        // is empty?
        if(setName.length() == 0){
            continue;
        }
        // is using "default"?
        if(setName == QSL("default")){
            QtLogger::error(QSL("[LOOP] Cannot add \"default\" as set name"));
            continue;
        }
        // is duplicate?
        if(validNames.contains(setName)){
            QtLogger::error(QSL("[LOOP] Name \"") % setName % QSL("\" is inserted more than once in the list."));
            continue;
        }
        validNames.insert(setName);

        // insert valid set
        sets.insert(std::begin(sets) + id.v + (v++), std::make_unique<Set>(setName));
    }
}

void Loop::remove_set(RowId id){
    sets.erase(std::begin(sets) + id.v);

    for(auto &set : sets){
        if(set->occurencies > 0){
            return;
        }
    }

    if(sets.size() > 0){
        sets[0]->occurencies = 1;
        QtLogger::warning(QSL("[LOOP] No remaining set has an occurencies nb > 0, first set occurencies nb has been set to 1."));
    }
}

bool Loop::modify_set_name(QString newSetName, RowId id){

    if(newSetName == QSL("default")){
        QtLogger::error(QSL("[LOOP] Cannot rename as set to \"default\" name"));
        return false;
    }

    // check if set doesn't already exist
    for(const auto &set : sets){
        if(set->name == newSetName){
            QtLogger::error(QSL("[LOOP] Set ") % newSetName % QSL(" already exists."));
            return false;
        }
    }

    sets[id.v]->name = newSetName;
    return true;
}

bool Loop::modify_set_occurencies_nb(int occurrencies, RowId id){

    auto currentNb = sets[id.v]->occurencies;
    sets[id.v]->occurencies = static_cast<size_t>(occurrencies);

    bool foundNotZero = false;
    for(const auto &set : sets){
        if(set->occurencies != 0){
            foundNotZero = true;
            break;
        }
    }

    if(!foundNotZero){
        sets[id.v]->occurencies = currentNb;
        QtLogger::error(QSL("[LOOP] We need to have at least one set with a non-zero occurency."));
        return false;
    }
    return true;
}

void Loop::move_set_up(RowId id){
    std::swap(sets[id.v], sets[id.v-1]);

}

void Loop::move_set_down(RowId id){
    std::swap(sets[id.v], sets[id.v+1]);
}


void Loop::sort_sets_lexico(){
//    std::sort(std::begin(sets), std::end(sets));
}

void Loop::sort_sets_num(){
//    std::sort(std::begin(sets), std::end(sets), [](const Set& lhs, const Set& rhs) {
//        return lhs.name.toInt() < rhs.name.toInt();
//    });
}

bool Loop::load_file(QString path){

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }

    QTextStream in(&file);
    std::vector<std::unique_ptr<Set>> filSets;
    std::set<QString> validNames;

    while (!in.atEnd()){
        auto line = in.readLine();
        auto split = line.split(" ");
        QString name;
        if(split.length() > 0){
            name = split[0];
        }
        if(name.length() == 0 || validNames.contains(name)){
            continue;
        }
        validNames.insert(name);

        int nbOcc = 1;
        if(split.length() > 1){
            bool ok;
            nbOcc = split[1].toInt(&ok);
            if(!ok){
                nbOcc = 1;
            }
        }
        filSets.push_back(std::make_unique<Set>(name,nbOcc, SetKey{-1}));
    }
    file.close();

    if(filSets.size() == 0){
        return false;
    }

    sets = std::move(filSets);

    return true;
}

QString Loop::merge_sets_names() const{

    QString str;
    for(const auto& set : sets){
        str += set->name % QSL(" ");
    }
    if(str.size() > 0){
        str.remove(str.size()-1,1);
    }
    return str;
}

QString Loop::merge_sets_occurencies() const{
    QString str;
    for(const auto& set : sets){
        str += QString::number(set->occurencies) % QSL(" ");
    }
    if(str.size() > 0){
        str.remove(str.size()-1,1);
    }
    return str;
}

Set *Loop::get_set(SetKey setKey) {

    for(auto &set : sets){
        if(set->key() == setKey.v){
            return set.get();
        }
    }

    return nullptr;
}

