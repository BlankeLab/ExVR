
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

#include "experiment.hpp"

// std
#include <algorithm>

// Qt
#include <QDebug>
#include <QMessageBox>
#include <QStringBuilder>
#include <QInputDialog>

// qt-utility
#include "qt_str.hpp"

// local
#include "data/config.hpp"
#include "data/node_flow.hpp"
#include "utility/path_utility.hpp"

using namespace tool;
using namespace tool::ex;

Experiment::Experiment(QString nVersion) : states(nVersion), randomizer(Randomizer(states.randomizationSeed)) {

    ResourcesManager::init();
    m_resM = ResourcesManager::get();

    ComponentsManager::init();
    m_compM = ComponentsManager::get();

    new_experiment();
}

void Experiment::update_randomization_seed(unsigned int seed){
    QtLogger::message(QSL("[EXP] Set randomizer with seed ") % QString::number(seed));
    states.randomizationSeed = seed;
    randomizer = Randomizer(states.randomizationSeed);
}

void Experiment::unselect_all_elements(bool updateSignal) noexcept{

    selectedElement = nullptr;

    for(auto &elem : elements){
        elem->set_selected(false);
    }

    for(auto &loop : loops){
        loop->set_selected(false);
    }

    if(updateSignal){
        add_to_update_flag(UpdateFlow | UpdateSelection);
    }
}

void Experiment::select_element(ElementKey elementKey, bool updateSignal){

    if(auto elem = get_element(elementKey); elem != nullptr){
        if(elem->is_selected()){
            return;
        }

        bool select = !elem->is_selected();
        unselect_all_elements(false); // unselect everything

        if(elem->type == Element::Type::LoopStart || elem->type == Element::Type::LoopEnd){
            LoopNode *loopNode = dynamic_cast<LoopNode*>(elem);
            loopNode->loop->set_selected(select);
            loopNode->loop->start->set_selected(select);
            loopNode->loop->end->set_selected(select);
            selectedElement = loopNode->loop;
        }else if(elem->type == Element::Type::Loop){
            Loop *loop = dynamic_cast<Loop*>(elem);
            loop->set_selected(select);
            loop->start->set_selected(select);
            loop->end->set_selected(select);
            selectedElement = loop;
        }else{

            if(elem->type == Element::Type::Routine){
                lastRoutineSelected = dynamic_cast<Routine*>(elem);                
                if(lastRoutineSelected->conditions.size() > 0){
                    lastRoutineSelected->select_condition(ConditionKey{lastRoutineSelected->conditions[0]->key()});
                }
            }else if(elem->type == Element::Type::Isi){
                lastIsiSelected = dynamic_cast<Isi*>(elem);
            }

            if(select){ // current element not selected
                selectedElement = elem;
                elem->set_selected(select);
            }
        }

        if(updateSignal){
            add_to_update_flag(UpdateFlow | UpdateSelection | UpdateRoutines);
        }
    }
}


size_t Experiment::get_element_position(Element *element) const{

    auto elementFound = std::find_if(elements.begin(), elements.end(), [element](const ElementUP & currElem){
        return currElem.get() == element;
    });
    if(elementFound != elements.end()){
        return static_cast<size_t>(std::distance(elements.begin(), elementFound));
    }

    QtLogger::error(QSL("[EXP] Element with key ") % QString::number(element->key()) % QSL(" not found."));
    return 0;
}

void Experiment::add_element(Element::Type type, size_t index){


    auto typeElements = get_elements_from_type(type);
    switch (type) {{
    case Element::Type::Routine:

            QString name;
            size_t offset = 1;
            bool isInside = false;
            do{
                name = QSL("Routine ") % QString::number(offset);
                isInside = false;
                for(auto element : typeElements){
                    if(element->name() == name){
                        isInside = true;
                        break;
                    }
                }
                ++offset;
            }while(isInside);

            auto routine = std::make_unique<Routine>(name, -1);
            auto routinePtr = routine.get();

            elements.insert(elements.begin() + static_cast<std_v1<ElementUP>::difference_type>(index + 1), std::move(routine));
            elements.insert(elements.begin() + static_cast<std_v1<ElementUP>::difference_type>(index + 2), std::make_unique<NodeFlow>());
            select_element(ElementKey{routinePtr->key()}, false);
    }break;{
    case Element::Type::Isi:

            QString name;
            size_t offset = 1;
            bool isInside = false;
            do{
                name = QSL("Isi ") % QString::number(offset);
                isInside = false;
                for(auto element : typeElements){
                    if(element->name() == name){
                        isInside = true;
                        break;
                    }
                }
                ++offset;
            }while(isInside);


            auto isi = std::make_unique<Isi>(name, ElementKey{-1});
            auto isiPtr = isi.get();
            elements.insert(elements.begin() + static_cast<std_v1<ElementUP>::difference_type>(index + 1), std::move(isi));
            elements.insert(elements.begin() + static_cast<std_v1<ElementUP>::difference_type>(index + 2), std::make_unique<NodeFlow>());
            select_element(ElementKey{isiPtr->key()}, false);
    }break;{
    case Element::Type::Loop:

            QString name;
            size_t offset = 1;
            bool isInside = false;
            do{
                name = QSL("Loop ") % QString::number(offset);
                isInside = false;
                for(const auto &loop : loops){
                    if(loop->name() == name){
                        isInside = true;
                        break;
                    }
                }
                ++offset;
            }while(isInside);

            auto loop = std::make_unique<Loop>(name, ElementKey{-1});
            auto loopPtr = loop.get();
            loops.emplace_back(std::move(loop));

            auto start = std::make_unique<LoopNode>(loopPtr, true);
            auto end   = std::make_unique<LoopNode>(loopPtr, false);
            loopPtr->set_nodes(start.get(),end.get());

            elements.insert(elements.begin() + static_cast<std_v1<ElementUP>::difference_type>(index + 1), std::move(start));
            elements.insert(elements.begin() + static_cast<std_v1<ElementUP>::difference_type>(index + 2), std::make_unique<NodeFlow>());
            elements.insert(elements.begin() + static_cast<std_v1<ElementUP>::difference_type>(index + 3), std::move(end));
            elements.insert(elements.begin() + static_cast<std_v1<ElementUP>::difference_type>(index + 4), std::make_unique<NodeFlow>());
            select_element(ElementKey{loopPtr->key()}, false);
    }break;
    default:
        return;
    }

    compute_loops_levels();
    update_conditions();    

    add_to_update_flag(UpdateFlow | UpdateSelection | UpdateRoutines);
}

void Experiment::remove_element(Element *elemToDelete){

    if(elemToDelete->type == Element::Type::LoopStart || elemToDelete->type == Element::Type::LoopEnd){
        elemToDelete = dynamic_cast<LoopNode*>(elemToDelete)->loop;
    }

    if(elemToDelete->type == Element::Type::Routine || elemToDelete->type == Element::Type::Isi){

        auto elementFound = std::find_if(elements.begin(), elements.end(), [elemToDelete](const ElementUP & currElem){
            return currElem.get() == elemToDelete;
        });
        elements.erase(elementFound-1, elementFound+1);

        remove_elements_not_in_flow();

    }else if(elemToDelete->type == Element::Type::Loop){

        LoopNode *startL = dynamic_cast<Loop*>(elemToDelete)->start;
        LoopNode *endL   = dynamic_cast<Loop*>(elemToDelete)->end;

        auto startP = std::find_if(elements.begin(), elements.end(), [startL](const ElementUP & currElem){
            return currElem.get() == startL;
        });
        auto endP = std::find_if(elements.begin(), elements.end(), [endL](const ElementUP & currElem){
            return currElem.get() == endL;
        });

        elements.erase(endP-1, endP+1);
        elements.erase(startP-1, startP+1);
        remove_elements_not_in_flow();
    }

    compute_loops_levels();
    update_conditions();
    selectedElement = nullptr;

    add_to_update_flag(UpdateFlow | UpdateSelection | UpdateRoutines);
}

void Experiment::remove_element_of_key(ElementKey elementKey){

    if(auto element = get_element(elementKey); element != nullptr){
        remove_element(element);
    }
}

void Experiment::duplicate_element(ElementKey elementKey){

    for(size_t ii = 0; ii < elements.size(); ++ii){
        if(elements[ii]->key() == elementKey.v){

            ElementUP newElement = nullptr;
            if(elements[ii]->type == Element::Type::Routine){

                newElement = Routine::copy_with_new_element_id(*dynamic_cast<Routine*>(elements[ii].get()), elements[ii]->name() % QSL("(copy)"));
                auto elementPtr = newElement.get();
                elements.insert(std::begin(elements) + static_cast<std_v1<ElementUP>::difference_type>(ii + 1), std::make_unique<NodeFlow>());
                elements.insert(std::begin(elements) + static_cast<std_v1<ElementUP>::difference_type>(ii + 2), std::move(newElement));
                select_element(ElementKey{elementPtr->key()}, false);

            }else if(elements[ii]->type == Element::Type::Isi){

                newElement = Isi::copy_with_new_element_id(*dynamic_cast<Isi*>(elements[ii].get()), elements[ii]->name() % QSL("(copy)"));
                auto elementPtr = newElement.get();
                elements.insert(std::begin(elements) + static_cast<std_v1<ElementUP>::difference_type>(ii + 1), std::make_unique<NodeFlow>());
                elements.insert(std::begin(elements) + static_cast<std_v1<ElementUP>::difference_type>(ii + 2), std::move(newElement));
                select_element(ElementKey{elementPtr->key()}, false);


            }else if(elements[ii]->type == Element::Type::LoopStart){

                auto loopStart = dynamic_cast<LoopNode*>(elements[ii].get());
                auto loopEnd = loopStart->loop->end;

                auto endPosition = get_element_position(loopEnd);

                auto loop = Loop::copy_with_new_element_id(*loopStart->loop, loopStart->loop->name() % QSL("(copy)"));
                auto loopPtr = loop.get();
                loops.emplace_back(std::move(loop));

                auto start = std::make_unique<LoopNode>(loopPtr, true);
                auto end   = std::make_unique<LoopNode>(loopPtr, false);
                loopPtr->set_nodes(start.get(),end.get());

                elements.insert(elements.begin() + static_cast<std_v1<ElementUP>::difference_type>(endPosition + 1), std::make_unique<NodeFlow>());
                elements.insert(elements.begin() + static_cast<std_v1<ElementUP>::difference_type>(endPosition + 2), std::move(start));
                elements.insert(elements.begin() + static_cast<std_v1<ElementUP>::difference_type>(endPosition + 3), std::make_unique<NodeFlow>());
                elements.insert(elements.begin() + static_cast<std_v1<ElementUP>::difference_type>(endPosition + 4), std::move(end));
                select_element(ElementKey{loopPtr->key()}, false);

                return;
            }else if(elements[ii]->type == Element::Type::LoopEnd){
                return;
            }else{
                return;
            }


            break;
        }
    }

    compute_loops_levels();
    update_conditions();

    add_to_update_flag(UpdateFlow | UpdateRoutines | UpdateSelection);
}

void Experiment::clean_current_routine_condition(ElementKey routineKey){

    if(auto routine = get_routine(routineKey); routine != nullptr){
        for(auto &condition : routine->conditions){
            if(condition->selected){
                condition->connectors.clear();
                condition->connections.clear();
                condition->actions.clear();
                break;
            }
        }
        add_to_update_flag(UpdateRoutines);
    }
}

void Experiment::clean_all_routine_conditions(ElementKey routineKey){

    if(auto routine = get_routine(routineKey); routine != nullptr){
        for(auto &condition : routine->conditions){
            condition->connectors.clear();
            condition->connections.clear();
            condition->actions.clear();
        }
        add_to_update_flag(UpdateRoutines);
    }
}

void Experiment::set_duration_for_all_routine_conditions(ElementKey routineKey){

    if(auto routine = get_routine(routineKey); routine != nullptr){

        double maxDuration = 0.0;
        for(auto &condition : routine->conditions){
            if(maxDuration < condition->duration.v ){
                maxDuration = condition->duration.v;
            }
        }

        bool ok;
        double newDuration = QInputDialog::getDouble(
            nullptr, tr("All routine conditions duration"), tr("Enter duration:"),
            maxDuration, 0., 1000000., 3, &ok
        );
        if(ok){
            for(auto &condition : routine->conditions){
                condition->update_max_length(SecondsTS{newDuration});
            }
            add_to_update_flag(UpdateRoutines);
        }
    }
}

void Experiment::remove_selected_element(){
    if(selectedElement){
        remove_element(selectedElement);
    }
}

void Experiment::move_left(size_t id){
    auto current  = elements[id].get();
    auto previous = elements[id-2].get();

    if(current->type == Element::Type::LoopStart && previous->type == Element::Type::LoopEnd){
        for(auto &elem : elements){
            if(elem->type == Element::Type::LoopStart && elem->key() == previous->key()){

                size_t idS = get_element_position(elem.get());
                auto e = std::move(elements[id]);
                elements.erase(elements.begin() + static_cast<std::vector<ElementUP>::difference_type>(id));
                elements.insert(elements.begin()+ static_cast<std::vector<ElementUP>::difference_type>(idS), std::move(e));

                e = std::move(elements[id]);
                elements.erase(elements.begin() + static_cast<std::vector<ElementUP>::difference_type>(id));
                elements.insert(elements.begin()+ static_cast<std::vector<ElementUP>::difference_type>(idS+1), std::move(e));

                compute_loops_levels();
                update_conditions();                
                add_to_update_flag(UpdateFlow | UpdateRoutines | UpdateSelection);
                return;
            }
        }
    }else if(current->type == Element::Type::LoopEnd && previous->type == Element::Type::LoopEnd){

        for(auto &elem : elements){
            if(elem->type == Element::Type::LoopStart && elem->key() == previous->key()){

                size_t idS = get_element_position(elem.get());

                auto e = std::move(elements[id]);
                erase_row(elements, id);
                elements.insert(elements.begin()+ static_cast<std::vector<ElementUP>::difference_type>(idS), std::move(e));

                e = std::move(elements[id]);
                erase_row(elements, id);
                elements.insert(elements.begin()+ static_cast<std::vector<ElementUP>::difference_type>(idS+1), std::move(e));

                compute_loops_levels();
                update_conditions();
                add_to_update_flag(UpdateFlow | UpdateRoutines | UpdateSelection);
                return;
            }
        }
    }else{
        std::swap(elements[id], elements[id-2]);
    }

    compute_loops_levels();
    update_conditions();

    add_to_update_flag(UpdateFlow | UpdateRoutines | UpdateSelection);
}

void Experiment::move_right(size_t id){

    auto current  = elements[id].get();
    auto next = elements[id+2].get();

    if(current->type == Element::Type::LoopStart && next->type == Element::Type::LoopStart){
        for(auto &elem : elements){
            if(elem->type == Element::Type::LoopEnd && elem->key() == next->key()){

                size_t idS = get_element_position(elem.get());
                auto e = std::move(elements[id]);
                elements.erase(elements.begin() + static_cast<std::vector<ElementUP>::difference_type>(id));
                elements.insert(elements.begin()+ static_cast<std::vector<ElementUP>::difference_type>(idS+1), std::move(e));

                e = std::move(elements[id]);
                elements.erase(elements.begin() + static_cast<std::vector<ElementUP>::difference_type>(id));
                elements.insert(elements.begin()+ static_cast<std::vector<ElementUP>::difference_type>(idS+1), std::move(e));

                compute_loops_levels();
                update_conditions();
                add_to_update_flag(UpdateFlow | UpdateRoutines | UpdateSelection);
                return;
            }
        }
    }else if(current->type == Element::Type::LoopEnd && next->type == Element::Type::LoopStart){

        for(auto &elem : elements){
            if(elem->type == Element::Type::LoopEnd && elem->key() == next->key()){

                size_t idS = get_element_position(elem.get());

                auto e = std::move(elements[id]);
                elements.erase(elements.begin() + static_cast<std::vector<ElementUP>::difference_type>(id));
                elements.insert(elements.begin()+ static_cast<std::vector<ElementUP>::difference_type>(idS+1), std::move(e));

                e = std::move(elements[id]);
                elements.erase(elements.begin() + static_cast<std::vector<ElementUP>::difference_type>(id));
                elements.insert(elements.begin()+ static_cast<std::vector<ElementUP>::difference_type>(idS+1), std::move(e));

                compute_loops_levels();
                update_conditions();
                add_to_update_flag(UpdateFlow | UpdateRoutines | UpdateSelection);
                return;
            }
        }
    }

    std::swap(elements[id], elements[id+2]);

    compute_loops_levels();
    update_conditions();
    add_to_update_flag(UpdateFlow | UpdateRoutines | UpdateSelection);
}

void Experiment::update_element_name(ElementKey elementKey, QString elemName){

    if(auto element = get_element(elementKey); element != nullptr){
        selectedElement->update_name(elemName);
        add_to_update_flag(UpdateComponents | UpdateFlow  | UpdateRoutines | UpdateSelection);
    }    
}

Element *Experiment::get_element(ElementKey elementKey) const{

    auto elementFound = std::find_if(elements.begin(), elements.end(), [elementKey](const ElementUP &element){
        return element->key() == elementKey.v;
    });
    if(elementFound != elements.end()){
        return elementFound->get();
    }

    QtLogger::error(QSL("[EXP] Element with key ") % QString::number(elementKey.v) % QSL(" not found."));
    return nullptr;
}

void Experiment::select_routine_condition(ElementKey routineKey, RowId conditionTabId){

    if(auto routine = get_routine(routineKey); routine != nullptr){
        select_element(routineKey, false);
        routine->select_condition(ConditionKey{routine->conditions[conditionTabId.v]->key()});
    }

    add_to_update_flag(UpdateRoutines | UpdateSelection);
}

void Experiment::move_routine_condition_down(ElementKey routineKey, RowId id){

    if(auto routine = get_routine(routineKey); routine != nullptr){
        routine->move_condition_down(id);
        add_to_update_flag(UpdateRoutines);
    }
}

void Experiment::move_routine_condition_up(ElementKey routineKey, RowId id){

    if(auto routine = get_routine(routineKey); routine != nullptr){
        routine->move_condition_up(id);
        add_to_update_flag(UpdateRoutines);
    }
}

void Experiment::set_routine_as_randomizer(ElementKey routineKey, bool isARandomizer){
    if(auto routine = get_routine(routineKey); routine != nullptr){
        routine->set_as_randomizer(isARandomizer);
        add_to_update_flag(UpdateRoutines | UpdateSelection | UpdateFlow);
    }
}

void Experiment::update_element_informations(ElementKey elementKey, QString informations){
    if(auto element = get_element(elementKey); element != nullptr){
        selectedElement->informations = std::move(informations);
        add_to_update_flag(UpdateFlow);
    }
}

void Experiment::update_condition_timeline(ElementKey routineKey, ConditionKey conditionKey, double duration, double scale, double uiFactorSize){

    if(auto condition = get_condition(routineKey, conditionKey); condition != nullptr){
        condition->update_max_length(SecondsTS{duration});
        condition->scale = scale;
        condition->uiFactorSize = uiFactorSize;
        add_to_update_flag(UpdateRoutines);
    }
}

Isi *Experiment::get_isi(ElementKey isiKey) const{


    auto elementFound = std::find_if(elements.begin(), elements.end(), [isiKey](const ElementUP &element){
        return element->key() == isiKey.v && element->is_isi();
    });
    if(elementFound != elements.end()){
        return dynamic_cast<Isi*>(elementFound->get());
    }

    QtLogger::error(QSL("[EXP] Isi with key ") % QString::number(isiKey.v) % QSL(" not found."));
    return nullptr;
}

Loop *Experiment::get_loop(ElementKey loopKey) const{

    auto loopFound = std::find_if(loops.begin(), loops.end(), [loopKey](const LoopUP &loop){
        return loop->key() == loopKey.v;
    });

    if(loopFound != loops.end()){
        return loopFound->get();
    }

    QtLogger::error(QSL("[EXP] Loop with key ") % QString::number(loopKey.v) % QSL(" not found."));
    return nullptr;
}

Action *Experiment::get_action(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey) const{

    if(auto condition = get_condition(routineKey, conditionKey); condition != nullptr){
        return condition->get_action_from_key(actionKey);
    }
    return nullptr;
}

void Experiment::check_integrity(){

    QtLogger::status(QSL("[EXP] Check experiment validity."), 2000);

    // check validity
    // # components
    auto cm = ComponentsManager::get();
    std::unordered_map<int, Component*> checkComponents;
    for(auto &component : cm->components){
        if(checkComponents.count(component->key()) != 0){
            QtLogger::error(QSL("[EXP] ") % component->to_string() % QSL(" already exists."));
        }else{
            checkComponents[component->key()] = component.get();
        }
    }

    // # elements
    std::unordered_map<int, Routine*> checkRoutines;
    std::unordered_map<int, Loop*> checkLoops;
    std::unordered_map<int, Isi*> checkIsi;
    std::unordered_map<int, Condition*> checkConditions;
    std::unordered_map<int, Connection*> checkConnection;
    std::unordered_map<int, Connector*> checkConnectors;
    std::unordered_map<int, Action*> checkActions;

    for(auto &elem : elements){
        if(elem->type == Element::Type::Routine){

            if(checkRoutines.count(elem->key()) != 0){
                QtLogger::error(QSL("[EXP] ") % dynamic_cast<Routine*>(elem.get())->to_string() % QSL(" already exists."));
            }else{
                checkRoutines[elem->key()] = dynamic_cast<Routine*>(elem.get());
            }

            for(auto &cond : checkRoutines[elem->key()]->conditions){
                if(checkConditions.count(cond->key()) != 0){
                    QtLogger::error(QSL("[EXP] ") % cond->to_string() % QSL(" already exists."));
                }else{
                    checkConditions[cond->key()] = dynamic_cast<Condition*>(cond.get());

                    for(auto &action : cond->actions){
                        if(checkActions.count(action->key()) != 0){
                            QtLogger::error(QSL("[EXP] ") % action->to_string() % QSL(" already exists."));
                        }else{
                            checkActions[action->key()] = dynamic_cast<Action*>(action.get());
                        }
                    }
                    for(auto &connector : cond->connectors){
                        if(checkConnectors.count(connector->key()) != 0){
                            QtLogger::error(QSL("[EXP] ") % connector->to_string() % QSL(" already exists."));
                        }else{
                            checkConnectors[connector->key()] = dynamic_cast<Connector*>(connector.get());
                        }
                    }
                    for(auto &connection : cond->connections){
                        if(checkConnection.count(connection->key()) != 0){
                            QtLogger::error(QSL("[EXP] ") % connection->to_string() % QSL(" already exists."));
                        }else{
                            checkConnection[connection->key()] = dynamic_cast<Connection*>(connection.get());
                        }
                    }
                }
            }
        }else if(elem->type == Element::Type::Loop){
            if(checkLoops.count(elem->key()) != 0){
                QtLogger::error(QSL("[EXP] ") % dynamic_cast<Loop*>(elem.get())->to_string() % QSL(" already exists."));
            }else{
                checkLoops[elem->key()] = dynamic_cast<Loop*>(elem.get());
            }
        }else if(elem->type == Element::Type::Isi){
            if(checkIsi.count(elem->key()) != 0){
                QtLogger::error(QSL("[EXP] ") % dynamic_cast<Isi*>(elem.get())->to_string() % QSL(" already exists."));
            }else{
                checkIsi[elem->key()] = dynamic_cast<Isi*>(elem.get());
            }
        }
    }


}

Condition *Experiment::get_condition(ConditionKey conditionKey) const{

    for(const auto &routine : get_elements_from_type<Routine>()){
        for(const auto &condition : routine->conditions){
            if(condition->key() == conditionKey.v){
                return condition.get();
            }
        }
    }
    return nullptr;
}

Condition *Experiment::get_condition(ElementKey routineKey, ConditionKey conditionKey) const{
    if(auto routine = get_routine(routineKey); routine != nullptr){
        return routine->get_condition(conditionKey);
    }
    return nullptr;
}

Routine *Experiment::get_routine(ElementKey routineKey) const{

    auto elementFound = std::find_if(elements.begin(), elements.end(), [routineKey](const ElementUP &element){
        return element->key() == routineKey.v && element->is_routine();
    });
    if(elementFound != elements.end()){
        return dynamic_cast<Routine*>(elementFound->get());
    }

    // QtLogger::error(QSL("[EXP] Routine with key ") % QString::number(routineKey.v) % QSL(" not found."));
    return nullptr;
}

void Experiment::copy_to_conditions(ElementKey routineKey, ConditionKey conditionKey, std_v1<std::pair<ElementKey,ConditionKey>> conditionsToBeEcrased, bool copyActions, bool copyConnections){

    if(auto conditionToCopy = get_condition(routineKey, conditionKey); conditionToCopy != nullptr){
        for(const auto &conditionToBeEcrased : conditionsToBeEcrased){
            if(auto cond = get_condition(conditionToBeEcrased.first, conditionToBeEcrased.second); cond != nullptr){
                cond->apply_condition(conditionToCopy, copyActions, copyConnections);
            }
        }
        add_to_update_flag(UpdateAll);
    }
}

void Experiment::delete_actions_from_condition(ElementKey routineKey, ConditionKey conditionKey){

    if(auto routine = get_routine(routineKey); routine != nullptr){
        routine->delete_actions_from_condition(conditionKey);
    }
    add_to_update_flag(UpdateRoutines);
}

void Experiment::fill_actions_from_condition(ElementKey routineKey, ConditionKey conditionKey){

    if(auto routine = get_routine(routineKey); routine != nullptr){
        routine->fill_actions_from_condition(conditionKey);
    }
    add_to_update_flag(UpdateRoutines);
}

void Experiment::clean_actions_from_condition(ElementKey routineKey, ConditionKey conditionKey){

    if(auto routine = get_routine(routineKey); routine != nullptr){
        routine->clean_actions_from_condition(conditionKey);
    }
    add_to_update_flag(UpdateRoutines);
}

void Experiment::create_connection(ElementKey routineKey, ConditionKey conditionKey, Connection *connection){

    if(auto routine = get_routine(routineKey); routine != nullptr){
        routine->create_nodes_connection(conditionKey, connection);
    }
    // no gui update, connection is created directly by the GUI
}

void Experiment::delete_connections(ElementKey routineKey, ConditionKey conditionKey){

    if(auto condition = get_condition(routineKey, conditionKey); condition != nullptr){
        condition->connectors.clear();
        condition->connections.clear();
        for(auto &action : condition->actions){
            action->nodeUsed = false;
        }
        add_to_update_flag(UpdateRoutines);
    }
}

void Experiment::create_connector_node(ElementKey routineKey, ConditionKey conditionKey, Connector *connector){

    if(auto routine = get_routine(routineKey); routine != nullptr){
        routine->create_connector_node(conditionKey, connector);
        add_to_update_flag(UpdateRoutines);
    }
}

void Experiment::modify_connector_node(ElementKey routineKey, ConditionKey conditionKey, ConnectorKey connectorKey, QString name, Arg arg){

    if(auto routine = get_routine(routineKey); routine != nullptr){
        routine->modify_connector(conditionKey, connectorKey, name, arg);
    }
}

void Experiment::move_connector_node(ElementKey routineKey, ConditionKey conditionKey, ConnectorKey connectorKey, QPointF pos){

    if(auto routine = get_routine(routineKey); routine != nullptr){
        routine->move_connector_node(conditionKey, connectorKey, pos);
    }
}

void Experiment::duplicate_connector_node(ElementKey routineKey, ConditionKey conditionKey, ConnectorKey connectorKey){

    if(auto routine = get_routine(routineKey); routine != nullptr){
        routine->duplicate_connector_node(conditionKey, connectorKey);
        add_to_update_flag(UpdateRoutines);
    }
}

void Experiment::set_connector_input_connection_validity(ElementKey routineKey, ConditionKey conditionKey, ConnectorKey connectorKey, bool state){

    if(auto routine = get_routine(routineKey); routine != nullptr){
        routine->set_connector_input_connection_validity(conditionKey, connectorKey, state);
    }
}

void Experiment::delete_selected_nodes(ElementKey routineKey, ConditionKey conditionKey){

    if(auto condition = get_condition(routineKey,conditionKey); condition != nullptr){

        std::vector<ConnectionKey> connectionsKey;
        for(auto &connection : condition->connections){
            if(connection->selected){
                connectionsKey.push_back({connection->key()});
            }
        }

        std::vector<ComponentKey> componentsKey;
        for(auto &action : condition->actions){
            if(action->nodeSelected){
                componentsKey.push_back({action->component->key()});
            }
        }

        std::vector<ConnectorKey> connectorsKey;
        for(auto &connector : condition->connectors){
            if(connector->selected){
                connectorsKey.push_back({connector->key()});
            }
        }

        // remove connections
        for(const auto &connectionKey : connectionsKey){
            condition->remove_connection(connectionKey);
        }

        // remove connectors
        for(const auto &connectorKey : connectorsKey){
            condition->remove_connector(connectorKey);
        }

        // remove components
        for(const auto &componentKey : componentsKey){
            condition->remove_component_node(componentKey);
        }

        add_to_update_flag(UpdateRoutines);
    }
}

void Experiment::create_component_node(ElementKey routineKey, ConditionKey conditionKey, ComponentKey componentKey, QPointF pos){

    if(auto condition = get_condition(routineKey, conditionKey); condition != nullptr){
        for(auto &action : condition->actions){
            if(action->component->key() == componentKey.v){
                action->nodeUsed     = true;
                action->nodePosition = pos;
                add_to_update_flag(UpdateRoutines);
                return;
            }
        }
    }
}

void Experiment::move_component_node(ElementKey routineKey, ConditionKey conditionKey, ComponentKey componentKey, QPointF pos){

    if(auto condition = get_condition(routineKey,conditionKey); condition != nullptr){
        condition->move_component(componentKey,pos);
    }
}

void Experiment::unselect_nodes_and_connections(ElementKey routineKey, ConditionKey conditionKey, bool doUpdate){
    if(auto condition = get_condition(routineKey,conditionKey); condition != nullptr){

        for(auto &connector : condition->connectors){
            connector->selected = false;
        }
        for(auto &connection : condition->connections){
            connection->selected = false;
        }
        for(auto &action : condition->actions){
            action->nodeSelected = false;
        }

        if(doUpdate){
            add_to_update_flag(UpdateRoutines);
        }
    }
}

void Experiment::delete_nodes_and_connections(ElementKey routineKey, ConditionKey conditionKey, std_v1<ConnectorKey> connectorsKey,
    std_v1<ComponentKey> componentsKey, std_v1<ConnectionKey> connectionsKey, bool doUpdate){

    if(auto condition = get_condition(routineKey,conditionKey); condition != nullptr){

        // remove connections
        for(const auto &connectionKey : connectionsKey){
            condition->remove_connection(connectionKey);
        }

        // remove connectors
        for(const auto &connectorKey : connectorsKey){
            condition->remove_connector(connectorKey);
        }

        // remove components
        for(const auto &componentKey : componentsKey){
            condition->remove_component_node(componentKey);
        }

        if(doUpdate){
            add_to_update_flag(UpdateRoutines);
        }
    }
}

void Experiment::select_nodes_and_connections(ElementKey routineKey, ConditionKey conditionKey,
    std_v1<ConnectorKey> connectorsKey, std_v1<ComponentKey> componentsKey, std_v1<ConnectionKey> connectionsKey, bool doUpdate){

    unselect_nodes_and_connections(routineKey, conditionKey, false);

    if(auto condition = get_condition(routineKey,conditionKey); condition != nullptr){

        for(const auto &connectorKey : connectorsKey){
            if(auto connector = condition->get_connector_from_key(connectorKey); connector != nullptr){
                connector->selected = true;
            }
        }

        for(const auto &componentKey : componentsKey){
            if(auto action = condition->get_action_from_component_key(componentKey); action != nullptr){
                action->nodeSelected = true;
            }
        }

        for(const auto &connectionKey : connectionsKey){
            if(auto connection = condition->get_connection_from_key(connectionKey); connection != nullptr){
                connection->selected = true;
            }
        }

        if(doUpdate){
            add_to_update_flag(UpdateRoutines);
        }
    }
}


void Experiment::add_action(ElementKey routineKey, ConditionKey conditionKey, ComponentKey componentKey,
    std::optional<ConfigKey> configKey, bool fillUpdateTimeline, bool fillVisibilityTimeline){

    auto component = m_compM->get_component(componentKey);
    if(component == nullptr){
        return;
    }

    if(auto routine = get_routine(routineKey); routine != nullptr){

        if(routine->isARandomizer){
            return;
        }

        if(auto condition = routine->get_condition(conditionKey); condition != nullptr){

            if(auto action = condition->get_action_from_component_key(componentKey, false); action == nullptr){
                condition->actions.emplace_back(Action::generate_component_action(
                    component, condition->duration, configKey, fillUpdateTimeline, fillVisibilityTimeline));

                add_to_update_flag(UpdateRoutines);
            }else{
                QtLogger::message(QSL("[EXP] Component already added to timeline."));
            }
        }
    }


}

void Experiment::add_action_to_all_conditions(ElementKey routineKey, ComponentKey componentKey,
    std::optional<ConfigKey> configKey, bool fillUpdateTimeline, bool fillVisibilityTimeline){

    auto component = m_compM->get_component(componentKey);
    if(component == nullptr){
        return;
    }

    if(auto routine = get_routine(routineKey); routine != nullptr){

        if(routine->isARandomizer){
            return;
        }

        for(auto &condition : routine->conditions){

            if(auto action = condition->get_action_from_component_key(componentKey, false); action == nullptr){

                condition->actions.emplace_back(Action::generate_component_action(
                    component, condition->duration, configKey, fillUpdateTimeline, fillVisibilityTimeline));
            }
        }
    }

    add_to_update_flag(UpdateRoutines);
}

void Experiment::add_action_to_all_routines_conditions(ComponentKey componentKey,
    std::optional<ConfigKey> configKey, bool fillUpdateTimeline, bool fillVisibilityTimeline){

    auto component = m_compM->get_component(componentKey);
    if(component == nullptr){
        return;
    }

    for(auto &routine : get_elements_from_type<Routine>()){

        if(routine->isARandomizer){
            continue;
        }

        for(auto &condition : routine->conditions){

            if(auto action = condition->get_action_from_component_key(componentKey, false); action == nullptr){

                condition->actions.emplace_back(Action::generate_component_action(
                    component, condition->duration, configKey, fillUpdateTimeline, fillVisibilityTimeline));
            }
        }
    }

    add_to_update_flag(UpdateRoutines);
}

void Experiment::modify_action(ElementKey routineKey, ConditionKey conditionKey, ComponentKey componentKey,
    bool changeConfig, bool changeUpdateTimeline, bool changeVisibilityTimeline, ConfigKey configKey, bool fillUpdateTimeline, bool fillVisibilityTimeline){

    auto component = m_compM->get_component(componentKey);
    if(component == nullptr){
        return;
    }

    if(auto routine = get_routine(routineKey); routine != nullptr){

        if(routine->isARandomizer){
            return;
        }

        if(auto condition = routine->get_condition(conditionKey); condition != nullptr){

            if(auto action = condition->get_action_from_component_key(componentKey, false); action != nullptr){

                if(changeConfig){
                    if(auto config = action->component->get_config(configKey); config != nullptr){
                        action->config = config;
                    }
                }

                if(changeUpdateTimeline){
                    if(fillUpdateTimeline){
                        action->timelineUpdate->fill(condition->duration);
                    }else{
                        action->timelineUpdate->clean();
                    }
                }

                if(changeVisibilityTimeline){
                    if(fillVisibilityTimeline){
                        action->timelineVisibility->fill(condition->duration);
                    }else{
                        action->timelineVisibility->clean();
                    }
                }
                add_to_update_flag(UpdateRoutines);
            }
        }
    }

}

void Experiment::modify_action_to_all_conditions(ElementKey routineKey, ComponentKey componentKey,
    bool changeConfig, bool changeUpdateTimeline, bool changeVisibilityTimeline, ConfigKey configKey, bool fillUpdateTimeline, bool fillVisibilityTimeline){


    auto component = m_compM->get_component(componentKey);
    if(component == nullptr){
        return;
    }

    if(auto routine = get_routine(routineKey); routine != nullptr){

        if(routine->isARandomizer){
            return;
        }

        for(auto &condition : routine->conditions){

            if(auto action = condition->get_action_from_component_key(componentKey, false); action != nullptr){

                if(changeConfig){
                    if(auto config = action->component->get_config(configKey); config != nullptr){
                        action->config = config;
                    }
                }

                if(changeUpdateTimeline){
                    if(fillUpdateTimeline){
                        action->timelineUpdate->fill(condition->duration);
                    }else{
                        action->timelineUpdate->clean();
                    }
                }

                if(changeVisibilityTimeline){
                    if(fillVisibilityTimeline){
                        action->timelineVisibility->fill(condition->duration);
                    }else{
                        action->timelineVisibility->clean();
                    }
                }
            }
        }
    }

    add_to_update_flag(UpdateRoutines);
}

void Experiment::modify_action_to_all_routines_conditions(ComponentKey componentKey,
    bool changeConfig, bool changeUpdateTimeline, bool changeVisibilityTimeline, ConfigKey configKey, bool fillUpdateTimeline, bool fillVisibilityTimeline){

    auto component = m_compM->get_component(componentKey);
    if(component == nullptr){
        return;
    }

    for(auto &routine : get_elements_from_type<Routine>()){

        if(routine->isARandomizer){
            continue;
        }

        for(auto &condition : routine->conditions){

            if(auto action = condition->get_action_from_component_key(componentKey, false); action != nullptr){

                if(changeConfig){
                    if(auto config = action->component->get_config(configKey); config != nullptr){
                        action->config = config;
                    }
                }

                if(changeUpdateTimeline){
                    if(fillUpdateTimeline){
                        action->timelineUpdate->fill(condition->duration);
                    }else{
                        action->timelineUpdate->clean();
                    }
                }

                if(changeVisibilityTimeline){
                    if(fillVisibilityTimeline){
                        action->timelineVisibility->fill(condition->duration);
                    }else{
                        action->timelineVisibility->clean();
                    }
                }
            }
        }
    }

    add_to_update_flag(UpdateRoutines);
}

void Experiment::remove_action_from_all_selected_routine_conditions(ElementKey routineKey, ComponentKey componentKey){

    if(auto routine = get_routine(routineKey); routine != nullptr){
        for(auto &condition : routine->conditions){

            for(const auto &action : condition->actions){
                if(action->component->key() == componentKey.v){
                    condition->remove_action(ActionKey{action->key()});
                    break;
                }
            }
        }

        add_to_update_flag(UpdateRoutines);
    }
}

void Experiment::remove_action_from_all_routines_conditions(ComponentKey componentKey){

    for(auto &routine : get_elements_from_type<Routine>()){
        for(auto &condition : routine->conditions){

            for(const auto &action : condition->actions){
                if(action->component->key() == componentKey.v){
                    condition->remove_action(ActionKey{action->key()});
                    break;
                }
            }
        }
    }

    add_to_update_flag(UpdateRoutines);
}

void Experiment::fill_action(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey, bool update, bool visibility){

    if(auto condition = get_condition(routineKey, conditionKey); condition != nullptr){
        if(auto action = condition->get_action_from_key(actionKey); action != nullptr){
            if(update){
                action->timelineUpdate->fill(condition->duration);
            }
            if(visibility){
                action->timelineVisibility->fill(condition->duration);
            }
            add_to_update_flag(UpdateRoutines);
        }
    }
}

void Experiment::clean_action(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey, bool update, bool visibility){

    if(auto condition = get_condition(routineKey, conditionKey); condition != nullptr){
        if(auto action = condition->get_action_from_key(actionKey); action != nullptr){
            if(update){
                action->timelineUpdate->clean();
            }
            if(visibility){
                action->timelineVisibility->clean();
            }
            add_to_update_flag(UpdateRoutines);
        }
    }
}

void Experiment::remove_action_from_condition(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey, bool update){

    if(auto condition = get_condition(routineKey, conditionKey); condition != nullptr){
        condition->remove_action(actionKey);

        if(update){
            add_to_update_flag(UpdateRoutines);
        }
    }
}

void Experiment::move_action_up(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey){

    if(auto condition = get_condition(routineKey, conditionKey); condition != nullptr){
        condition->move_action_up(actionKey);
        add_to_update_flag(UpdateRoutines);
    }
}

void Experiment::move_action_down(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey){

    if(auto condition = get_condition(routineKey, conditionKey); condition != nullptr){
        condition->move_action_down(actionKey);
        add_to_update_flag(UpdateRoutines);
    }
}

void Experiment::select_action_config(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey, RowId  configTabId){

    if(auto action = get_action(routineKey, conditionKey, actionKey); action != nullptr){
        action->select_config(configTabId);
        add_to_update_flag(UpdateRoutines);
    }
}

void Experiment::select_loop_set(ElementKey loopKey, QString setName){

    if(auto loop = get_loop(loopKey); loop != nullptr){
        if(setName != loop->currentSetName){
            loop->currentSetName = setName;
            add_to_update_flag(UpdateSelection | UpdateFlow | UpdateRoutines);
        }
    }
}

void Experiment::add_loop_sets(ElementKey loopKey, QString sets, RowId id){

    if(auto loop = get_loop(loopKey); loop != nullptr){
        if(loop->is_file_mode()){
            QtLogger::error(QSL("[EXP] Cannot add new set when file mode is used."));
        }else{

            if(loop->is_default()){
                loop->set_sets(sets.split("\n"));
            }else{
                loop->add_sets(sets.split("\n"), id);
//                int startId = id.v;
//                int ii = 0;
//                for(const auto &setName : sets.split("\n")){

//                    if(setName.length() == 0){
//                        continue;
//                    }

//                    if(loop->add_set(setName, RowId{startId+ii})){
//                        ++ii;
//                    }
//                }
            }

            update_conditions();
            add_to_update_flag(UpdateSelection | UpdateFlow | UpdateRoutines);
        }
    }
}

void Experiment::modify_loop_set_name(ElementKey loopKey, QString setName, RowId id){

    if(auto loop = get_loop(loopKey); loop != nullptr){        
        if(loop->modify_set_name(setName,id)){
            update_conditions();
        }
        add_to_update_flag(UpdateSelection | UpdateFlow | UpdateRoutines);
    }
}

void Experiment::modify_loop_set_occurrencies_nb(ElementKey loopKey, int setOccuranciesNb, RowId id){

    if(auto loop = get_loop(loopKey); loop != nullptr){
        if(loop->is_file_mode()){
            QtLogger::error(QSL("[EXP] Cannot modify loop set when file mode is used."));
        }else{
            loop->modify_set_occurencies_nb(setOccuranciesNb, id);
            add_to_update_flag(UpdateSelection);
        }
    }
}

void Experiment::modify_loop_type(ElementKey loopKey, Loop::Mode mode){

    if(auto loop = get_loop(loopKey); loop != nullptr){
        loop->set_loop_type(mode);
        update_conditions();
        add_to_update_flag(UpdateSelection);
    }
}

void Experiment::modify_loop_nb_reps(ElementKey loopKey, int nbReps){

    if(auto loop = get_loop(loopKey); loop != nullptr){
        loop->set_nb_reps(to_unsigned(nbReps));
        update_conditions();
        //add_to_update_flag(UpdateSelection);
    }
}

void Experiment::modify_loop_N(ElementKey loopKey, int N){

    if(auto loop = get_loop(loopKey); loop != nullptr){
        loop->set_N(to_unsigned(N));
        update_conditions();
        //add_to_update_flag(UpdateSelection);
    }
}

void Experiment::remove_set(ElementKey loopKey, RowId id){

    if(auto loop = get_loop(loopKey); loop != nullptr){
        if(loop->is_file_mode()){
            QtLogger::error(QSL("[EXP] Cannot remove loop set when file mode is used."));
        }else{

            loop->remove_set(id);
            update_conditions();
        }
        add_to_update_flag(UpdateRoutines | UpdateSelection | UpdateFlow);
    }
}

void Experiment::sort_loop_sets_lexico(ElementKey loopKey){

    if(auto loop = get_loop(loopKey); loop != nullptr){
        if(loop->is_file_mode()){
            QtLogger::error(QSL("[EXP] Cannot sort loop set when file mode is used."));
        }else{
            loop->sort_sets_lexico();
            update_conditions();
            add_to_update_flag(UpdateRoutines | UpdateSelection | UpdateFlow);
        }
    }
}

void Experiment::sort_loop_sets_num(ElementKey loopKey){
    if(auto loop = get_loop(loopKey); loop != nullptr){
        if(loop->is_file_mode()){
            QtLogger::error(QSL("[EXP] Cannot sort loop set when file mode is used."));
        }else{
            loop->sort_sets_num();
            update_conditions();
            add_to_update_flag(UpdateRoutines | UpdateSelection | UpdateFlow);
        }
    }
}

void Experiment::move_loop_set_up(ElementKey loopKey, RowId id){

    if(auto loop = get_loop(loopKey); loop != nullptr){
        if(loop->is_file_mode()){
            QtLogger::error(QSL("[EXP] Cannot move loop set when file mode is used."));
        }else{
            loop->move_set_up(id);
            update_conditions();
            add_to_update_flag(UpdateRoutines | UpdateSelection | UpdateFlow);
        }
    }
}

void Experiment::move_loop_set_down(ElementKey loopKey, RowId id){

    if(auto loop = get_loop(loopKey); loop != nullptr){
        if(loop->is_file_mode()){
            QtLogger::error(QSL("[EXP] Cannot move loop set when file mode is used."));
        }else{
            loop->move_set_down(id);
            update_conditions();
            add_to_update_flag(UpdateRoutines | UpdateSelection | UpdateFlow);
        }
    }
}

void Experiment::load_loop_sets_file(ElementKey loopKey, QString path){

    if(auto loop = get_loop(loopKey); loop != nullptr){
        if(loop->load_file(path)){
            update_conditions();
            add_to_update_flag(UpdateRoutines | UpdateSelection | UpdateFlow);
        }else{
            QtLogger::error(QSL("[EXP] Cannot load loops set file with path: ") % path);
        }
    }
}

void Experiment::reload_loop_sets_file(ElementKey loopKey){

    if(auto loop = get_loop(loopKey); loop != nullptr){
        if(loop->filePath.length() == 0){
            return;
        }
        if(loop->load_file(loop->filePath)){
            update_conditions();
            add_to_update_flag(UpdateRoutines | UpdateSelection | UpdateFlow);
        }else{
            QtLogger::error(QSL("[EXP] Cannot reload loops set file with path: ") % loop->filePath);
        }
    }
}

void Experiment::add_timeline_interval(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey, bool updateTimeline, TimelineKey timelineKey, Interval interval){

    Q_UNUSED(timelineKey)
    if(auto action = get_action(routineKey, conditionKey, actionKey); action != nullptr){

        if(updateTimeline){            
            if(action->timelineUpdate->add_interval(interval)){
                add_to_update_flag(UpdateRoutines);
            }
        }else{
            if(action->timelineVisibility->add_interval(interval)){
                add_to_update_flag(UpdateRoutines);
            }
        }        
    }
}

void Experiment::remove_timeline_interval(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey, bool updateTimeline, TimelineKey timelineKey, Interval interval){

    Q_UNUSED(timelineKey)
    if(auto action = get_action(routineKey, conditionKey, actionKey); action != nullptr){
        if(updateTimeline){
            if(action->timelineUpdate->remove_interval(interval)){
                add_to_update_flag(UpdateRoutines);
            }
        }else{
            if(action->timelineVisibility->remove_interval(interval)){
                add_to_update_flag(UpdateRoutines);
            }
        }        
    }
}

void Experiment::add_isi_interval(ElementKey isiKey, qreal value, RowId id){
    if(auto isi = get_isi(isiKey); isi != nullptr){
        isi->add_interval(value, id);
        add_to_update_flag(UpdateFlow | UpdateSelection);
    }
}

void Experiment::remove_isi_interval(ElementKey isiKey, RowId id){
    if(auto isi = get_isi(isiKey); isi != nullptr){
        isi->remove_interval(id);
        add_to_update_flag(UpdateFlow | UpdateSelection);
    }    
}

void Experiment::set_isi_randomize(ElementKey isiKey, bool randomize){
    if(auto isi = get_isi(isiKey); isi != nullptr){
        isi->set_randomize(randomize);
        add_to_update_flag(UpdateFlow | UpdateSelection);
    }    
}

void Experiment::modify_isi_interval(ElementKey isiKey, qreal value, RowId id){
    if(auto isi = get_isi(isiKey); isi != nullptr){
        isi->modify_interval(value, id);
        add_to_update_flag(UpdateFlow | UpdateSelection);
    }    
}

void Experiment::move_isi_interval_up(ElementKey isiKey, RowId id){
    if(auto isi = get_isi(isiKey); isi != nullptr){
        isi->move_interval_up(id);
        add_to_update_flag(UpdateFlow | UpdateSelection);
    }    
}

void Experiment::move_isi_interval_down(ElementKey isiKey, RowId id){
    if(auto isi = get_isi(isiKey); isi != nullptr){
        isi->move_interval_down(id);
        add_to_update_flag(UpdateFlow | UpdateSelection);
    }    
}

void Experiment::compute_loops_levels(){

    // retrieve id of loops inside each element
    std_v1<int> idLoops;
    states.maximumDeepLevel = -1;
    for(auto& elem : elements){
        if(elem->type == Element::Type::LoopStart){
            auto node = dynamic_cast<LoopNode*>(elem.get());
            node->insideLoopsID       = idLoops;
            node->loop->insideLoopsID = idLoops;
            idLoops.emplace_back(node->key());
         }else if(elem->type ==Element::Type::LoopEnd){
            auto node = dynamic_cast<LoopNode*>(elem.get());            
            idLoops.erase(std::find(idLoops.begin(),idLoops.end(), node->key()));
            node->insideLoopsID = idLoops;
        }else{
            elem->insideLoopsID = idLoops;
        }

        if(states.maximumDeepLevel < to_signed(idLoops.size())){
            states.maximumDeepLevel = to_signed(idLoops.size());
        }
    }

    // retrieve loops outside each element
    for(auto& elem : elements){

        elem->insideLoops.clear();
        for(auto id : elem->insideLoopsID){
            for(auto &loop : loops){
                if(loop->key() == id){
                    elem->insideLoops.emplace_back(loop.get());
                    break;
                }
            }
        }

        if(elem->insideLoops.size() != elem->insideLoopsID.size()){
            QtLogger::error(QSL("[EXP] Error during loop levels computing with element ") % elem->name());
        }
    }

    // retrieve loops outside each loop
    for(auto& loop : loops){

        loop->insideLoops.clear();
        for(auto id : loop->insideLoopsID){
            for(auto &iLoop : loops){
                if(iLoop->key() == id){
                    loop->insideLoops.emplace_back(iLoop.get());
                    break;
                }
            }
        }

        if(loop->insideLoops.size() != loop->insideLoopsID.size()){
            QtLogger::error(QSL("[EXP] Error during loop levels computing with loop ") % loop->name());
        }
    }
}

std_v1<QString> mix(const std_v1<QString> &l1, const std_v1<QString> &l2){

    std_v1<QString> m;
    m.reserve(l1.size()*l2.size());
    for(size_t ii = 0; ii < l1.size(); ++ii){
        for(size_t jj = 0; jj < l2.size(); ++jj){
            m.emplace_back(l1[ii] % QSL("-") % l2[jj]);
        }
    }
    return m;
}

void Experiment::update_conditions(){

    // parse every routine
    for(auto &routine : get_elements_from_type<Routine>()){

        // if routine not inside any loop, set only one "default" condition
        if(routine->insideLoops.size() == 0){

            if(routine->conditions.size() == 0){
                routine->conditions.emplace_back(Condition::generate_new_default());
            }else{

                // log deleted conditions
                if(routine->conditions.size() > 1){
                    for(size_t ii = 1; ii < routine->conditions.size(); ++ii){
                        QtLogger::message(
                            QSL("[EXP] Condition ") % routine->conditions[ii]->name %
                            QSL(" from routine ") % routine->name() % QSL("(") %
                            QString::number(routine->key()) % QSL(") doesn't exist anymore and has been removed.")
                        );
                    }
                }

                routine->conditions.resize(1);
                routine->conditions[0]->name = QSL("default");
                routine->conditions[0]->selected = true;
            }
            continue;
        }

        // merge every set key from every loop containing the routine
        std_v1<std_v1<QString>> setsId;
        for(const auto& insideLoop : routine->insideLoops){

            auto loop = dynamic_cast<Loop*>(insideLoop);
            if(loop->is_file_mode()){

                std_v1<QString> loopFileSetsId;
                loopFileSetsId.reserve(loop->fileSets.size());
                for(const auto &s : loop->fileSets){
                    loopFileSetsId.emplace_back(QString::number(s.key()));
                }
                setsId.emplace_back(std::move(loopFileSetsId));
            }else{

                std_v1<QString> loopSetsId;
                loopSetsId.reserve(loop->sets.size());
                for(const auto &s : loop->sets){
                    loopSetsId.emplace_back(QString::number(s.key()));
                }
                setsId.emplace_back(std::move(loopSetsId));
            }
        }

        // mix new conditions keys
        std_v1<QString> newConditionsKeysStr;
        while(setsId.size() > 1){
            setsId[setsId.size()-2] = mix(setsId[setsId.size()-2],setsId[setsId.size()-1]);
            setsId.erase(std::end(setsId)-1);
        }
        if(setsId.size() > 0){
            newConditionsKeysStr = std::move(setsId[0]);
        }

        //  create all current conditions names
        std_v1<std_v1<int>> newConditionsKeys;
        std_v1<QString> newConditionsNames;
        for(const auto &conditionKeysStr : newConditionsKeysStr){

            std_v1<int> conditionKeys;
            QStringList conditionName;

            for(const auto &key : conditionKeysStr.split("-")){

                bool found = false;
                for(const auto& insideLoop : routine->insideLoops){

                    auto loop = dynamic_cast<Loop*>(insideLoop);
                    if(auto set = loop->get_set(SetKey{key.toInt()}); set != nullptr){
                        conditionName << set->name;
                        conditionKeys.emplace_back(set->key());
                        found = true;
                        break;
                    }
                }

                if(!found){
                    QtLogger::error(QSL("[EXP] Cannot retrieve set with key ") % key % QSL(" from any loop."));
                }
            }
            newConditionsKeys.emplace_back(std::move(conditionKeys));
            newConditionsNames.emplace_back(conditionName.join("-"));
        }


        // one old/new condition, just rename it
        if(newConditionsKeysStr.size() == 1 && routine->conditions.size() == 1){
            routine->conditions[0]->name = newConditionsNames[0];
            continue;
        }
        // only one new condition, keep only one old and rename it
        if(newConditionsKeysStr.size() == 1 && routine->conditions.size() > 1){

            // log deleted conditions
            for(size_t ii = 1; ii < routine->conditions.size(); ++ii){
                QtLogger::message(
                    QSL("[EXP] Condition ") % routine->conditions[ii]->name %
                    QSL(" from routine ") % routine->name() % QSL("(") %
                    QString::number(routine->key()) % QSL(") doesn't exist anymore and has been removed.")
                );
            }

            routine->conditions.resize(1);
            routine->conditions[0]->name = newConditionsNames[0];
            continue;
        }




        // rebuild routine conditions
        std_v1<ConditionUP> newConditions;
        for(size_t ii = 0; ii < newConditionsKeys.size(); ++ii){

            bool found = false;
            for(size_t jj = 0; jj < routine->conditions.size(); ++jj){

                if(routine->conditions[jj]->contains_same_set_keys(newConditionsKeys[ii])){
                    // all keys found
                    routine->conditions[jj]->name = newConditionsNames[ii];
                    newConditions.emplace_back(std::move(routine->conditions[jj]));
                    routine->conditions.erase(std::begin(routine->conditions) + static_cast<int>(jj));
                    found = true;
                    break;
                }
            }

            if(!found){ // not found, create new condition
                auto condition = Condition::generate_new(newConditionsNames[ii]);
                condition->setsKeys = std::move(newConditionsKeys[ii]);
                newConditions.emplace_back(std::move(condition));
            }
        }

        // log deleted conditions
        for(const auto &conditionToBeDeleted : routine->conditions){

            QtLogger::message(QSL("[EXP] Condition ") % conditionToBeDeleted->name % QSL(" from routine ") % routine->name() % QSL("(") %
                            QString::number(routine->key()) % QSL(") doesn't exist anymore and has been removed."));
        }

        routine->conditions = std::move(newConditions);

        bool foundSelected = false;
        for(auto &condition : routine->conditions){
            if(condition->selected){
                foundSelected = true;
                break;
            }
        }
        if(!foundSelected){
            routine->conditions[0]->selected = true;
        }
    }
}

void Experiment::update_component_position(ComponentKey componentKey, RowId id){

    if(const auto compoInfo = m_compM->get_component_and_position(componentKey); compoInfo.second != nullptr){
        auto compoToMove = std::move(m_compM->components[compoInfo.first]);
        m_compM->components.erase(m_compM->components.begin() + static_cast<std_v1<ComponentUP>::difference_type>(compoInfo.first));
        m_compM->components.insert(m_compM->components.begin() + id.v, std::move(compoToMove));
        add_to_update_flag(UpdateComponents);
    }
}

void Experiment::remove_component(ComponentKey componentKey){

    if(auto componentToRemove = get_component(componentKey); componentToRemove != nullptr){

        // remove action which use this component
        for(auto routine : get_elements_from_type<Routine>()){

            // remove action containing component from conditions
            for(auto &condition : routine->conditions){
                if(auto action = condition->get_action_from_component_key(componentKey, false); action != nullptr){
                    QtLogger::message(QSL("[EXP] Remove ") % action->to_string() % QSL(" using component ") % action->component->name() % QSL(" from ") %  condition->to_string() % QSL(" from ") % routine->to_string());
                    condition->remove_action(ActionKey{action->key()});
                }
            }

//            // remove action containing component from ghost conditions
//            for(auto &condition : routine->ghostsConditions){
//                if(auto action = condition->get_action_from_component_key(componentKey, false); action != nullptr){
//                    condition->remove_action(ActionKey{action->key()});
//                }
//            }
        }

        for(size_t id = 0; id < m_compM->components.size(); ++id){
            if(m_compM->components[id]->key() == componentToRemove->key()){
                QtLogger::message(QSL("[EXP] Remove ") % m_compM->components[id]->to_string());
                m_compM->components.erase(m_compM->components.begin() + static_cast<int>(id));
                break;
            }
        }

        add_to_update_flag(UpdateComponents | UpdateRoutines);
    }
}

void Experiment::duplicate_component(ComponentKey componentKey){

    if(const auto compoInfo = m_compM->get_component_and_position(componentKey); compoInfo.second != nullptr){
        if(Component::get_unicity(compoInfo.second->type)){
            QtLogger::error(QSL("[EXP] You can only have one component of type [") % from_view(Component::get_type_name(compoInfo.second->type)) % QSL("] in the experiment."));
        }else{
            m_compM->components.insert(
                m_compM->components.begin() + static_cast<std_v1<ComponentUP>::difference_type>(compoInfo.first + 1),
                Component::copy_with_new_element_id(*compoInfo.second, compoInfo.second->name() % QSL("(copy)"))
            );
            add_to_update_flag(UpdateComponents | UpdateRoutines);
        }
    }
}

void Experiment::add_component(Component::Type type, RowId id){
    m_compM->insert_new_component(type, id);
    add_to_update_flag(UpdateComponents | UpdateRoutines);
}

void Experiment::update_component_name(ComponentKey componentKey, QString name){

    if(m_compM->update_component_name(componentKey, name)){
        add_to_update_flag(UpdateComponents | UpdateRoutines);
    }
}

void Experiment::sort_components_by_category(){
    m_compM->sort_by_category();
    add_to_update_flag(UpdateComponents);
}

void Experiment::sort_components_by_type(){
    m_compM->sort_by_type();
    add_to_update_flag(UpdateComponents);
}

void Experiment::sort_components_by_name(){
    m_compM->sort_by_name();
    add_to_update_flag(UpdateComponents);
}

void Experiment::select_config_in_component(ComponentKey componentKey, RowId id){
    if(auto component = get_component(componentKey); component != nullptr){
        component->selectedConfigId = id;
        add_to_update_flag(UpdateComponents | UpdateRoutines);
    }
}

void Experiment::insert_config_in_component(ComponentKey componentKey, RowId id, QString configName){

    if(auto component = get_component(componentKey); component != nullptr){
        for(const auto &config : component->configs){
            if(config->name == configName){
                QtLogger::error(QSL("[EXP] ") % config->to_string() % QSL(" already exist"));
                return;
            }
        }

        component->configs.insert(component->configs.begin() + id.v + 1, std::make_unique<Config>(configName, ConfigKey{-1}));
        component->selectedConfigId = {id.v + 1};
        add_to_update_flag(UpdateComponents | UpdateRoutines);
    }
}

void Experiment::copy_config_from_component(ComponentKey componentKey, RowId id, QString configName){

    if(auto component = get_component(componentKey); component != nullptr){
        for(const auto &config : component->configs){
            if(config->name == configName){
                QtLogger::error(QSL("[EXP] ") % config->to_string() % QSL(" already exist"));
                return;
            }
        }

        component->configs.insert(component->configs.begin() + id.v + 1,
            Config::copy_with_new_element_id(*component->configs[id.v].get(), configName));
        component->selectedConfigId = {id.v + 1};

        add_to_update_flag(UpdateComponents | UpdateRoutines);
    }
}

void Experiment::remove_config_from_component(ComponentKey componentKey, RowId id){

    if(auto component = get_component(componentKey); component != nullptr){

        if(id.v < to_signed(component->configs.size())){

            auto config = component->configs[id.v].get();
            ConfigKey configKey = ConfigKey{config->key()};

            for(auto routine : get_elements_from_type<Routine>()){

                for(auto &condition : routine->conditions){
                    std_v1<ActionKey> actionsToRemoved;
                    for(auto &action : condition->actions){
                        if(action->config->key() == configKey.v){
                            QtLogger::message(QSL("[EXP] Remove ") % action->to_string() % QSL(" from ") % condition->to_string() %
                                            QSL(" from ") % routine->to_string());
                            actionsToRemoved.emplace_back(ActionKey{action->key()});
                        }
                    }

                    for(auto actionToRemove : actionsToRemoved){
                        condition->remove_action(actionToRemove);
                    }
                }
            }

            QtLogger::message(QSL("[EXP] Remove ") % config->to_string() % QSL(" from ") % component->to_string());
            component->configs.erase(component->configs.begin() + id.v);
            component->selectedConfigId = {id.v-1};
            if(component->selectedConfigId.v < 0){
                component->selectedConfigId.v = 0;
            }
        }

        add_to_update_flag(UpdateComponents | UpdateRoutines);
    }
}

void Experiment::move_config_in_component(ComponentKey componentKey, RowId from, RowId to){
    if(auto component = get_component(componentKey); component != nullptr){
        auto config = std::move(component->configs[from.v]);
        component->configs.erase(component->configs.begin() + from.v);
        component->configs.insert(component->configs.begin() + to.v, std::move(config));
        component->selectedConfigId = to;
        add_to_update_flag(UpdateComponents |UpdateRoutines);                
    }
}

void Experiment::rename_config_in_component(ComponentKey componentKey, RowId id, QString configName){

    if(auto component = get_component(componentKey); component != nullptr){
        if(id.v < to_signed(component->configs.size())){
            component->configs[id.v]->name = configName;
            add_to_update_flag(UpdateComponents | UpdateRoutines);
        }else{
            QtLogger::error(QSL("[EXP] Cannot rename config in component ") % QString::number(componentKey.v) % QSL(" -> wrong id for configs ") %
                          QString::number(id.v) % QSL(" (size=") % QString::number(component->configs.size()) % QSL(")"));
        }
    }
}

void Experiment::new_arg(ComponentKey componentKey, ConfigKey configKey, Arg arg, bool initConfig){

    if(auto component = get_component(componentKey); component != nullptr){

        if(initConfig){
            component->initConfig->add_arg(std::move(arg));
        }else{
            component->get_config(configKey)->add_arg(std::move(arg));
        }
    }
}


void Experiment::arg_updated(ComponentKey componentKey, ConfigKey configKey, Arg arg, bool initConfig){

    if(auto component = get_component(componentKey); component != nullptr){
        if(initConfig){
            component->initConfig->update_arg(std::move(arg));
        }else{
            component->get_config(configKey)->update_arg(std::move(arg));
        }
    }
}

void Experiment::arg_removed(ComponentKey componentKey, ConfigKey configKey, QStringView argName, bool initConfig){

    if(auto component = get_component(componentKey); component != nullptr){
        if(initConfig){
            component->initConfig->remove_arg(argName);
        }else{
            component->get_config(configKey)->remove_arg(argName);
        }
    }
}

void Experiment::swap_arg(ComponentKey componentKey, ConfigKey configKey, QStringView argName1, QStringView argName2, bool initConfig){
    if(auto component = get_component(componentKey); component != nullptr){
        if(initConfig){
            component->initConfig->swap_arg(argName1, argName2);
        }else{
            component->get_config(configKey)->swap_arg(argName1, argName2);
        }
    }
}



void Experiment::add_resources(Resource::Type type, QStringList filesPath){
    m_resM->add_resources(type, filesPath);
    add_to_update_flag(UpdateResources | UpdateComponents | UpdateRoutines);
}

void Experiment::update_resource_path(QString currentPath, QString newPath){
    m_resM->update_resource_path(currentPath, newPath);
    add_to_update_flag(UpdateResources);
}

void Experiment::update_resource_alias(QString currentAlias, QString newAlias){
    m_resM->update_resource_alias(currentAlias, newAlias);
    add_to_update_flag(UpdateResources | UpdateComponents | UpdateRoutines);
}

void Experiment::select_resource(Resource::Type type, size_t index){
    m_resM->select_resource(type, index);
    add_to_update_flag(UpdateResources);
}

void Experiment::remove_resource(Resource::Type type, size_t index){
    m_resM->remove_resource(type, index);
    add_to_update_flag(UpdateResources | UpdateComponents | UpdateRoutines);
}

void Experiment::clean_resources(Resource::Type type){
    m_resM->clean_resources(type);
    add_to_update_flag(UpdateResources | UpdateComponents | UpdateRoutines);
}

void Experiment::update_reload_resource_code(int reloadCode){
    m_resM->set_reload_code(reloadCode);
}

Component *Experiment::get_component(ComponentKey componentKey) const{
    return m_compM->get_component(componentKey);
}

Config *Experiment::get_config(ComponentKey componentKey, ConfigKey configKey) const{
    if(auto component = get_component(componentKey); component != nullptr){
        return component->get_config(configKey);
    }
    return nullptr;
}

void Experiment::check_elements(){

//    size_t countBefore = elements.size();
//    std::sort(elements.begin(), elements.end());
//    elements.erase(std::unique(elements.begin(), elements.end()), elements.end());

//    size_t countAfter = elements.size();
//    if(countBefore > countAfter){
//        QtLogger::warning(
//            QSL("Remove ") % QString::number(countBefore - countAfter) % QSL(" duplicated elements.")
//        );
//    }

    for(auto &element : elements){
        qDebug() << "check " << element->name();
        element->check_integrity();
    }
}

void Experiment::check_legacy_conditions(){

    // check for empty condition sets keys
    for(auto &routine : get_elements_from_type<Routine>()){

        for(auto &condition : routine->conditions){

            if(routine->insideLoops.size() == 0){
                continue;
            }

            if(condition->setsKeys.size() == 0){

                QtLogger::error("[XML] OUTDATED SETS SYSTEM, WILL TRY TO UPGRADE, MAY FAIL IF LOOPS HAVE SAME SETS NAMES -> from routine " %
                                routine->name() % QSL(" with condition ") % condition->name);

                size_t countMatched = 0;
                auto split = condition->name.split("-");
                for(auto set : split){

                    for(auto &loop : routine->insideLoops){

                        bool found = false;
                        for(const auto &loopSet : dynamic_cast<Loop*>(loop)->sets){
                            if(loopSet.name == set){
                                found = true;
                                ++countMatched;
                                condition->setsKeys.emplace_back(loopSet.key());

                                break;
                            }
                        }

                        if(found){
                            break;
                        }
                    }
                }
                QtLogger::error(QSL("[XML] FOUND ") % QString::number(countMatched) % QSL(" OUT OF ") % QString::number(split.size()) % " SETS MATCHING. PLEASE CHECK IT.");
            }
        }
    }
}



void Experiment::new_experiment(){

    clean_experiment();

    QtLogger::message(QSL("[EXP] New experiment"));
    states.currentName = QSL("new_experiment");
    elements.emplace_back(std::make_unique<NodeFlow>()); // init with one node
    compute_loops_levels();
    update_conditions();
    add_to_update_flag(UpdateAll | ResetUI);
}

void Experiment::clean_experiment(){

    QtLogger::message(QSL("[EXP] Clean experiment"));

    // remove resources
    m_resM->clean_resources();

    // remove elements
    elements.clear();
    loops.clear();

    // remove components
    m_compM->clean_components();

    // reset settings
    m_settings.reset();

    // clean id
    IdKey::reset();

    // no selected element
    selectedElement = nullptr;

    // reset states
    states.reset();
}

void Experiment::update_exp_launcher_state(ExpLauncherState state, QStringView infos){
    Q_UNUSED(infos)
    states.explauncherState = state;
    add_to_update_flag(UpdateUI);
}

void Experiment::update_exp_state(ExpState state, QStringView infos){

    states.expState = state;
    if(states.expState == ExpState::Loaded){
        states.neverLoaded = false;
        qDebug() << "EXP LOADED";
    }


    bool updateCurrentElement = false;
    bool updateCurrentCondition = false;

    if(infos.length() > 0){

        int idSep = infos.indexOf('|');
        states.currentElementTimeS = infos.left(idSep).toDouble()*0.001;
        infos = infos.mid(idSep+1);

        idSep = infos.indexOf('|');
        states.experimentTimeS = infos.left(idSep).toDouble()*0.001;
        infos = infos.mid(idSep+1);

        idSep = infos.indexOf('|');
        states.currentIntervalEndTimeS = infos.left(idSep).toDouble()*0.001;
        infos = infos.mid(idSep+1);

        idSep = infos.indexOf('|');
        states.currentOrder = infos.left(idSep).toString();
        infos = infos.mid(idSep+1);

        idSep = infos.indexOf('|');
        auto elementKey = ElementKey{infos.left(idSep).toInt()};
        infos = infos.mid(idSep+1);

        idSep = infos.indexOf('|');
        states.nbCalls = infos.left(idSep).toString();
        infos = infos.mid(idSep+1);

        idSep = infos.indexOf('|');
        QChar elementType = infos.left(idSep)[0];
        infos = infos.mid(idSep+1);

        if(elementType == 'R'){

            idSep = infos.indexOf('|');
            auto conditionKey = ConditionKey{infos.left(idSep).toInt()};
            infos = infos.mid(idSep+1);

            states.currentElementType = Element::Type::Routine;
            if(auto routine = get_routine(elementKey); routine != nullptr){

                states.currentElementName = routine->name();

                if(states.currentElementKey != routine->key()){
                    states.currentElementKey = routine->key();
                    updateCurrentElement = true;
                }

                if(auto condition = routine->get_condition(conditionKey); condition != nullptr){
                    states.currentTypeSpecificInfo = condition->name;
                    if(condition->key() != states.currentConditionKey){
                        states.currentConditionKey = condition->key();
                        updateCurrentCondition = true;
                    }
                }
            }
        }else if(elementType == 'I'){

            idSep = infos.indexOf('|');
            auto specificInfo = infos.left(idSep);
            infos = infos.mid(idSep+1);

            states.currentElementType = Element::Type::Isi;

            if(auto isi = get_isi(elementKey); isi != nullptr){

                states.currentElementName = isi->name();

                if(states.currentElementKey != isi->key()){
                    updateCurrentElement = true;
                    states.currentElementKey = isi->key();
                }
                states.currentTypeSpecificInfo = specificInfo.toString();
            }
        }
    }

    add_to_update_flag(UpdateUI);

    if(updateCurrentElement && updateCurrentCondition && states.followsCurrentCondition){
        if(auto routine = get_routine(ElementKey{states.currentElementKey}); routine != nullptr){
            select_element(ElementKey{states.currentElementKey}, false);
            routine->select_condition(ConditionKey{states.currentConditionKey});            
            add_to_update_flag(UpdateSelection | UpdateRoutines);
        }      
    }else if(updateCurrentCondition && states.followsCurrentCondition){
        if(auto routine = get_routine(ElementKey{states.currentElementKey}); routine != nullptr){
            routine->select_condition(ConditionKey{states.currentConditionKey});
        }
        select_element(ElementKey{states.currentElementKey}, false);
        add_to_update_flag(UpdateSelection | UpdateRoutines);
    }else if(updateCurrentElement && states.followsCurrentCondition){
        select_element(ElementKey{states.currentElementKey}, false);
        add_to_update_flag(UpdateSelection | UpdateRoutines);
    }
}

void Experiment::reset_settings(){
    QtLogger::message(QSL("[EXP] Reset settings"));
    m_settings.reset();
    add_to_update_flag(UpdateSettings);
}

void Experiment::update_settings(Settings settings){
    this->m_settings = std::move(settings);
    add_to_update_flag(UpdateSettings);
}

void Experiment::toggle_actions_connections_separation(){
    m_gui.toggleActionsConnectionsSep = !m_gui.toggleActionsConnectionsSep;
    add_to_update_flag(UpdateRoutines);
}

void Experiment::toggle_design_mode(){

    if(states.currentMode == "designer"){
        states.currentMode = "experiment";
    }else{
        states.currentMode = "designer";
    }
    add_to_update_flag(UpdateUI);
}

void Experiment::toggle_follow_condition_mode(){
    states.followsCurrentCondition = !states.followsCurrentCondition;
    add_to_update_flag(UpdateUI);
}

void Experiment::update_connector_dialog_with_info(ElementKey elementKey, ConditionKey conditionKey, ConnectorKey connectorKey, QStringView id, QStringView value){
    connectorsInfo[elementKey.v][conditionKey.v][connectorKey.v][id] = value;
}

void Experiment::update_component_dialog_with_info(ComponentKey componentKey, ConfigKey configKey, QStringView id, QStringView value){
    componentsInfo[componentKey.v][configKey.v][id] = value;
}

void Experiment::set_instance_name(QString instanceName){
    states.currentInstanceName = instanceName;
    add_to_update_flag(UpdateUI);
}

void Experiment::remove_elements_not_in_flow(){

    QVector<int> idsLoopsRemaining;
    for(const auto &startLoop : get_elements_from_type<LoopNode>()){
        if(startLoop->type == Element::Type::LoopStart){
            idsLoopsRemaining << startLoop->key();
        }
    }

    for(int ii = to_signed(loops.size()-1); ii >= 0; --ii){
        if(idsLoopsRemaining.indexOf(loops[to_unsigned(ii)]->key()) == -1){
            loops.erase(loops.begin()+ii);
        }
    }
}

std_v1<Loop *> Experiment::get_loops() const{
    std_v1<Loop*> l;
    l.reserve(loops.size());
    for(const auto &loop : loops){
        l.emplace_back(loop.get());
    }
    return l;
}

std_v1<Element *> Experiment::get_elements() const{
    std_v1<Element*> children;
    children.reserve(elements.size());
    for(const auto &elem : elements){
        children.emplace_back(elem.get());
    }
    return children;
}

std_v1<Element *> Experiment::get_elements_from_type(Element::Type type) const{
    std_v1<Element*> children;
    for(const auto &elem : elements){
        if(elem->type == type){
            children.emplace_back(elem.get());
        }
    }
    return children;
}

#include "moc_experiment.cpp"
