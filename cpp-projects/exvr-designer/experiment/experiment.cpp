
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
#include "utility/path_utility.hpp"

using namespace tool;
using namespace tool::ex;


Experiment::Experiment(QString nVersion) : states(nVersion), randomizer(Randomizer(states.randomizationSeed)) {
    new_experiment();
}

auto Experiment::nb_elements() const noexcept -> size_t{
    return elements.size();
}

auto Experiment::nb_elements_no_nodes() const noexcept -> size_t{
    return nb_elements() / 2;
}

auto Experiment::get_element_position(FlowElement *element) const -> RowId{

    if(auto elementFound = get_element_iterator(element); elementFound != elements.end()){
        return RowId{static_cast<int>(std::distance(elements.begin(), elementFound))};
    }

    QtLogger::error(QSL("[EXP] Element with key ") % QString::number(element->key()) % QSL(" not found."));
    return RowId{-1};
}

auto Experiment::get_element_position_no_nodes(FlowElement *element) const -> RowId{

    if(auto row = get_element_position(element); row.v != -1){
        return {row.v / 2};
    }
    return {-1};
}

auto Experiment::get_element(RowId id) const -> FlowElement*{
    if(id.v < static_cast<int>(nb_elements())){
        return elements[id.v].get();
    }
    return nullptr;
}

auto Experiment::get_element_no_nodes(RowId id) const -> FlowElement*{
    return get_element(RowId{id.v * 2 + 1});
}

auto Experiment::get_element(ElementKey elementKey, bool showError) const -> FlowElement* {

    if(auto elementFound = get_element_iterator(elementKey); elementFound != elements.end()){
        return elementFound->get();
    }
    if(showError){
        QtLogger::error(QSL("[EXP] Element with key ") % QString::number(elementKey.v) % QSL(" not found."));
    }
    return nullptr;
}

auto Experiment::get_elements() const -> std::vector<FlowElement*>{

    std::vector<FlowElement*> children;
    children.reserve(elements.size());
    for(const auto &elem : elements){
        children.push_back(elem.get());
    }
    return children;
}

auto Experiment::get_elements_of_type(FlowElement::Type type) const{
    return elements | std::ranges::views::filter([type](const auto &element) {
        return element->type() == type;
    });
}



auto Experiment::get_routine(ElementKey routineKey) const -> Routine*{
    if(auto elementFound = std::find_if(elements.begin(), elements.end(), [routineKey](const auto &element){
            return element->key() == routineKey.v && element->is_routine();
        }); elementFound != elements.end()){
        return dynamic_cast<Routine*>(elementFound->get());
    }
    return nullptr;
}

auto Experiment::get_routine(RowId id) const -> Routine*{
    auto routines = get_elements_of_type(FlowElement::Type::Routine);
    if(id.v < std::ranges::distance(routines)){
        auto routinesIt = routines.begin();
        std::ranges::advance(routinesIt, id.v);
        return dynamic_cast<Routine*>(routinesIt->get());
    }
    return nullptr;
}

auto Experiment::get_routines_name() const -> std::vector<QStringView>{

    auto routinesV = get_elements_of_type(FlowElement::Type::Routine);
    std::vector<QStringView> names;
    names.reserve(std::ranges::distance(routinesV));
    for(auto it = routinesV.begin(); it != routinesV.end(); ++it){
        names.push_back(it->get()->name());
    }
    return names;
}


auto Experiment::get_condition(ElementKey routineKey, ConditionKey conditionKey) const -> Condition*{
    if(auto routine = get_routine(routineKey); routine != nullptr){
        return routine->get_condition(conditionKey);
    }
    return nullptr;
}

auto Experiment::get_condition(ConditionKey conditionKey) const -> Condition *{

    for(const auto &routine : get_elements_from_type<Routine>()){

        if(auto condFound = std::find_if(routine->conditions.begin(), routine->conditions.end(), [conditionKey](const auto &condition){
                return condition->key() == conditionKey.v;
            }); condFound != routine->conditions.end()){
            return condFound->get();
        }
    }
    QtLogger::error(QSL("[EXP] Condition with key ") % QString::number(conditionKey.v) % QSL(" not found."));
    return nullptr;
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

void Experiment::select_element_id(RowId elementId, bool updateSignal){
    if(elementId.v < static_cast<int>(elements.size())){
        select_element_from_ptr(elements[elementId.v].get(), updateSignal);
    }else{
        QtLogger::error(QSL("Cannot select element with id [") % QString::number(elementId.v) % QSL("], it doesn't exist."));
    }
}

void Experiment::select_element_id_no_nodes(RowId elementId, bool updateSignal){
    select_element_id(RowId{elementId.v * 2 + 1}, updateSignal);
}

void Experiment::select_element(ElementKey elementKey, bool updateSignal){

    if(auto elem = get_element(elementKey); elem != nullptr){
        select_element_from_ptr(elem, updateSignal);
    }else{
        QtLogger::error(QSL("Cannot select element with key [") % QString::number(elementKey.v) % QSL("], it doesn't exist."));
    }
}

void Experiment::select_element_from_ptr(FlowElement *element, bool updateSignal){


    if(element->is_selected()){
        return;
    }

    bool select = !element->is_selected();
    unselect_all_elements(false); // unselect everything

    if(element->type() == FlowElement::Type::LoopStart || element->type() == FlowElement::Type::LoopEnd){
        LoopNode *loopNode = dynamic_cast<LoopNode*>(element);
        loopNode->loop->set_selected(select);
        loopNode->loop->start->set_selected(select);
        loopNode->loop->end->set_selected(select);
        selectedElement = loopNode->loop;
    }else if(element->type() == FlowElement::Type::Loop){
        Loop *loop = dynamic_cast<Loop*>(element);
        loop->set_selected(select);
        loop->start->set_selected(select);
        loop->end->set_selected(select);
        selectedElement = loop;
    }else{

        if(element->type() == FlowElement::Type::Routine){
            lastRoutineSelected = dynamic_cast<Routine*>(element);
            if(lastRoutineSelected->selected_condition() == nullptr){
                lastRoutineSelected->select_condition(RowId{0});
            }
        }else if(element->type() == FlowElement::Type::Isi){
            lastIsiSelected = dynamic_cast<Isi*>(element);
        }

        if(select){ // current element not selected
            selectedElement = element;
            element->set_selected(select);
        }
    }

    if(updateSignal){
        add_to_update_flag(UpdateFlow | UpdateSelection | UpdateRoutines);
    }
}




void Experiment::add_element(FlowElement::Type type, size_t index){


    auto typeElements = get_elements_of_type(type);
    switch (type) {
    case FlowElement::Type::Routine:{

            QString name;
            size_t offset = 1;
            bool isInside = false;
            do{
                name = QSL("Routine ") % QString::number(offset);
                isInside = false;
                for(const auto &element : typeElements){
                    if(element->name() == name){
                        isInside = true;
                        break;
                    }
                }
                ++offset;
            }while(isInside);

            auto routine = std::make_unique<Routine>(name, -1);
            auto routinePtr = routine.get();

            elements.insert(elements.begin() + static_cast<std_v1<std::unique_ptr<FlowElement>>::difference_type>(index + 1), std::move(routine));
            elements.insert(elements.begin() + static_cast<std_v1<std::unique_ptr<FlowElement>>::difference_type>(index + 2), std::make_unique<NodeFlow>());
            select_element(routinePtr->e_key(), false);
    }break;
    case FlowElement::Type::Isi:{

            QString name;
            size_t offset = 1;
            bool isInside = false;
            do{
                name = QSL("Isi ") % QString::number(offset);
                isInside = false;
                for(const auto &element : typeElements){
                    if(element->name() == name){
                        isInside = true;
                        break;
                    }
                }
                ++offset;
            }while(isInside);


            auto isi = std::make_unique<Isi>(name, ElementKey{-1});
            auto isiPtr = isi.get();
            elements.insert(elements.begin() + static_cast<std_v1<std::unique_ptr<FlowElement>>::difference_type>(index + 1), std::move(isi));
            elements.insert(elements.begin() + static_cast<std_v1<std::unique_ptr<FlowElement>>::difference_type>(index + 2), std::make_unique<NodeFlow>());
            select_element(isiPtr->e_key(), false);
    }break;
    case FlowElement::Type::Loop:{

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
            loops.push_back(std::move(loop));

            auto start = std::make_unique<LoopNode>(loopPtr, true);
            auto end   = std::make_unique<LoopNode>(loopPtr, false);
            loopPtr->set_nodes(start.get(),end.get());

            elements.insert(elements.begin() + static_cast<std_v1<std::unique_ptr<FlowElement>>::difference_type>(index + 1), std::move(start));
            elements.insert(elements.begin() + static_cast<std_v1<std::unique_ptr<FlowElement>>::difference_type>(index + 2), std::make_unique<NodeFlow>());
            elements.insert(elements.begin() + static_cast<std_v1<std::unique_ptr<FlowElement>>::difference_type>(index + 3), std::move(end));
            elements.insert(elements.begin() + static_cast<std_v1<std::unique_ptr<FlowElement>>::difference_type>(index + 4), std::make_unique<NodeFlow>());
            select_element(loopPtr->e_key(), false);
    }break;
    default:
        return;
    }

    compute_loops_levels();
    update_conditions();    

    add_to_update_flag(UpdateFlow | UpdateSelection | UpdateRoutines);
}

void Experiment::remove_element(FlowElement *elemToDelete, bool updateConditions){

    if(selectedElement != nullptr){
        if(selectedElement->key() == elemToDelete->key()){
            selectedElement = nullptr;
        }
    }

    if(elemToDelete->type() == FlowElement::Type::LoopStart || elemToDelete->type() == FlowElement::Type::LoopEnd){
        elemToDelete = dynamic_cast<LoopNode*>(elemToDelete)->loop;
    }

    if(elemToDelete->type() == FlowElement::Type::Routine || elemToDelete->type() == FlowElement::Type::Isi){

        auto elementFound = std::find_if(elements.begin(), elements.end(), [elemToDelete](const std::unique_ptr<FlowElement> & currElem){
            return currElem.get() == elemToDelete;
        });
        elements.erase(elementFound-1, elementFound+1);

        remove_elements_not_in_flow();

    }else if(elemToDelete->type() == FlowElement::Type::Loop){

        LoopNode *startL = dynamic_cast<Loop*>(elemToDelete)->start;
        LoopNode *endL   = dynamic_cast<Loop*>(elemToDelete)->end;

        auto startP = std::find_if(elements.begin(), elements.end(), [startL](const std::unique_ptr<FlowElement> & currElem){
            return currElem.get() == startL;
        });
        auto endP = std::find_if(elements.begin(), elements.end(), [endL](const std::unique_ptr<FlowElement> & currElem){
            return currElem.get() == endL;
        });

        elements.erase(endP-1, endP+1);
        elements.erase(startP-1, startP+1);
        remove_elements_not_in_flow();
    }

    if(updateConditions){
        compute_loops_levels();
        update_conditions();
    }

    add_to_update_flag(UpdateFlow | UpdateSelection | UpdateRoutines);
}

void Experiment::remove_element_of_key(ElementKey elementKey){
    if(auto element = get_element(elementKey); element != nullptr){
        remove_element(element);
    }
}

void Experiment::remove_everything_before_element_of_key(ElementKey elementKey){

    if(auto firstNewElement = get_element(elementKey); firstNewElement != nullptr){

        std::vector<ElementKey> elementsToRemove;
        std::vector<ElementKey> loopsToRemove;
        for(const auto &element : elements){

            if(element->type() == FlowElement::Type::Node){
                continue;
            }

            if(element->key() != firstNewElement->key()){

                if(element->type() == FlowElement::Type::Routine || element->type() == FlowElement::Type::Isi){
                    elementsToRemove.push_back(element->e_key());
                }else if(element->type() == FlowElement::Type::LoopStart || element->type() == FlowElement::Type::LoopEnd){
                    loopsToRemove.push_back(element->e_key());
                }
            }else{
                break;
            }
        }

        if(elementsToRemove.size() == 0){
            return;
        }

        QMessageBox validateBox;
        validateBox.setWindowTitle("Remove everything before selected element");
        validateBox.setText(QString("This action will remove several elements from the flow, do you want to continue?"));
        validateBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        validateBox.setDefaultButton(QMessageBox::Cancel);
        switch (validateBox.exec()) {
        case QMessageBox::Ok:
            for (ElementKey eKey : std::ranges::reverse_view(elementsToRemove)){
                if(auto element = get_element(eKey, false); element != nullptr){
                    remove_element(element, false);
                }
            }
            for (ElementKey eKey : std::ranges::reverse_view(loopsToRemove)){
                if(auto element = get_element(eKey, false); element != nullptr){
                    remove_element(element, false);
                }
            }
            compute_loops_levels();
            update_conditions();
            add_to_update_flag(UpdateFlow | UpdateSelection | UpdateRoutines);
            return;
        default:
            return;
        }
    }
}

void Experiment::remove_everything_after_element_of_key(ElementKey elementKey){

    if(auto firstNewElement = get_element(elementKey); firstNewElement != nullptr){

        std::vector<ElementKey> elementsToRemove;
        std::vector<ElementKey> loopsToRemove;
        bool found = false;
        for(const auto &element : elements){

            if(element->type() == FlowElement::Type::Node){
                continue;
            }

            if(element->key() == firstNewElement->key()){
                found = true;
                continue;
            }

            if(found){
                if(element->type() == FlowElement::Type::Routine || element->type() == FlowElement::Type::Isi){
                    elementsToRemove.push_back(element->e_key());
                }else if(element->type() == FlowElement::Type::LoopStart || element->type() == FlowElement::Type::LoopEnd){
                    loopsToRemove.push_back(element->e_key());
                }
            }
        }

        if(elementsToRemove.size() == 0){
            return;
        }

        QMessageBox validateBox;
        validateBox.setWindowTitle("Remove everything after selected element");
        validateBox.setText(QString("This action will remove several elements from the flow, do you want to continue?"));
        validateBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        validateBox.setDefaultButton(QMessageBox::Cancel);
        switch (validateBox.exec()) {
        case QMessageBox::Ok:
            for (ElementKey eKey : std::ranges::reverse_view(elementsToRemove)){
                if(auto element = get_element(eKey, false); element != nullptr){
                    remove_element(element, false);
                }
            }
            for (ElementKey eKey : std::ranges::reverse_view(loopsToRemove)){
                if(auto element = get_element(eKey, false); element != nullptr){
                    remove_element(element, false);
                }
            }
            compute_loops_levels();
            update_conditions();
            add_to_update_flag(UpdateFlow | UpdateSelection | UpdateRoutines);
            return;
        default:
            return;
        }
    }
}

void Experiment::duplicate_element(ElementKey elementKey){

    for(size_t ii = 0; ii < elements.size(); ++ii){
        if(elements[ii]->key() == elementKey.v){

            std::unique_ptr<FlowElement> newElement = nullptr;
            if(elements[ii]->type() == FlowElement::Type::Routine){

                newElement = Routine::copy_with_new_element_id(*dynamic_cast<Routine*>(elements[ii].get()), elements[ii]->name() % QSL("(copy)"));
                auto elementPtr = newElement.get();
                elements.insert(std::begin(elements) + static_cast<std_v1<std::unique_ptr<FlowElement>>::difference_type>(ii + 1), std::make_unique<NodeFlow>());
                elements.insert(std::begin(elements) + static_cast<std_v1<std::unique_ptr<FlowElement>>::difference_type>(ii + 2), std::move(newElement));
                select_element(ElementKey{elementPtr->key()}, false);

            }else if(elements[ii]->type() == FlowElement::Type::Isi){

                newElement = Isi::copy_with_new_element_id(*dynamic_cast<Isi*>(elements[ii].get()), elements[ii]->name() % QSL("(copy)"));
                auto elementPtr = newElement.get();
                elements.insert(std::begin(elements) + static_cast<std_v1<std::unique_ptr<FlowElement>>::difference_type>(ii + 1), std::make_unique<NodeFlow>());
                elements.insert(std::begin(elements) + static_cast<std_v1<std::unique_ptr<FlowElement>>::difference_type>(ii + 2), std::move(newElement));
                select_element(ElementKey{elementPtr->key()}, false);

            }else if(elements[ii]->type() == FlowElement::Type::LoopStart){

                auto loopStart = dynamic_cast<LoopNode*>(elements[ii].get());
                auto loopEnd = loopStart->loop->end;

                auto endPosition = get_element_position(loopEnd).v;

                auto loop = Loop::copy_with_new_element_id(*loopStart->loop, loopStart->loop->name() % QSL("(copy)"));
                auto loopPtr = loop.get();
                loops.push_back(std::move(loop));

                auto start = std::make_unique<LoopNode>(loopPtr, true);
                auto end   = std::make_unique<LoopNode>(loopPtr, false);
                loopPtr->set_nodes(start.get(),end.get());

                elements.insert(elements.begin() + static_cast<std_v1<std::unique_ptr<FlowElement>>::difference_type>(endPosition + 1), std::make_unique<NodeFlow>());
                elements.insert(elements.begin() + static_cast<std_v1<std::unique_ptr<FlowElement>>::difference_type>(endPosition + 2), std::move(start));
                elements.insert(elements.begin() + static_cast<std_v1<std::unique_ptr<FlowElement>>::difference_type>(endPosition + 3), std::make_unique<NodeFlow>());
                elements.insert(elements.begin() + static_cast<std_v1<std::unique_ptr<FlowElement>>::difference_type>(endPosition + 4), std::move(end));
                select_element(ElementKey{loopPtr->key()}, false);

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

    if(current->type() == FlowElement::Type::LoopStart && previous->type() == FlowElement::Type::LoopEnd){
        for(auto &elem : elements){
            if(elem->type() == FlowElement::Type::LoopStart && elem->key() == previous->key()){

                int idS = get_element_position(elem.get()).v;
                auto e = std::move(elements[id]);
                elements.erase(elements.begin() + static_cast<std::vector<std::unique_ptr<FlowElement>>::difference_type>(id));
                elements.insert(elements.begin()+ static_cast<std::vector<std::unique_ptr<FlowElement>>::difference_type>(idS), std::move(e));

                e = std::move(elements[id]);
                elements.erase(elements.begin() + static_cast<std::vector<std::unique_ptr<FlowElement>>::difference_type>(id));
                elements.insert(elements.begin()+ static_cast<std::vector<std::unique_ptr<FlowElement>>::difference_type>(idS+1), std::move(e));

                compute_loops_levels();
                update_conditions();                
                add_to_update_flag(UpdateFlow | UpdateRoutines | UpdateSelection);
                return;
            }
        }
    }else if(current->type() == FlowElement::Type::LoopEnd && previous->type() == FlowElement::Type::LoopEnd){

        for(auto &elem : elements){
            if(elem->type() == FlowElement::Type::LoopStart && elem->key() == previous->key()){

                int idS = get_element_position(elem.get()).v;

                auto e = std::move(elements[id]);
                erase_row(elements, id);
                elements.insert(elements.begin()+ static_cast<std::vector<std::unique_ptr<FlowElement>>::difference_type>(idS), std::move(e));

                e = std::move(elements[id]);
                erase_row(elements, id);
                elements.insert(elements.begin()+ static_cast<std::vector<std::unique_ptr<FlowElement>>::difference_type>(idS+1), std::move(e));

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

    if(current->type() == FlowElement::Type::LoopStart && next->type() == FlowElement::Type::LoopStart){
        for(auto &elem : elements){
            if(elem->type() == FlowElement::Type::LoopEnd && elem->key() == next->key()){

                int idS = get_element_position(elem.get()).v;
                auto e = std::move(elements[id]);
                elements.erase(elements.begin() + static_cast<std::vector<std::unique_ptr<FlowElement>>::difference_type>(id));
                elements.insert(elements.begin()+ static_cast<std::vector<std::unique_ptr<FlowElement>>::difference_type>(idS+1), std::move(e));

                e = std::move(elements[id]);
                elements.erase(elements.begin() + static_cast<std::vector<std::unique_ptr<FlowElement>>::difference_type>(id));
                elements.insert(elements.begin()+ static_cast<std::vector<std::unique_ptr<FlowElement>>::difference_type>(idS+1), std::move(e));

                compute_loops_levels();
                update_conditions();
                add_to_update_flag(UpdateFlow | UpdateRoutines | UpdateSelection);
                return;
            }
        }
    }else if(current->type() == FlowElement::Type::LoopEnd && next->type() == FlowElement::Type::LoopStart){

        for(auto &elem : elements){
            if(elem->type() == FlowElement::Type::LoopEnd && elem->key() == next->key()){

                int idS = get_element_position(elem.get()).v;

                auto e = std::move(elements[id]);
                elements.erase(elements.begin() + static_cast<std::vector<std::unique_ptr<FlowElement>>::difference_type>(id));
                elements.insert(elements.begin()+ static_cast<std::vector<std::unique_ptr<FlowElement>>::difference_type>(idS+1), std::move(e));

                e = std::move(elements[id]);
                elements.erase(elements.begin() + static_cast<std::vector<std::unique_ptr<FlowElement>>::difference_type>(id));
                elements.insert(elements.begin()+ static_cast<std::vector<std::unique_ptr<FlowElement>>::difference_type>(idS+1), std::move(e));

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

void Experiment::move_element(ElementKey elementKey){

    if(auto element = get_element(elementKey); element != nullptr){

        auto currentNoNodePosition =  get_element_position_no_nodes(element);
        bool ok;
        auto newNoNodePosition = RowId{QInputDialog::getInt(nullptr,
            QSL("Move element from position [") % QString::number(currentNoNodePosition.v) % QSL("] to"), "Enter new id position", currentNoNodePosition.v, 0,
            static_cast<int>(nb_elements_no_nodes()-1),1, &ok)};

        if(ok){

            auto currentPos = get_element_position(get_element_no_nodes(currentNoNodePosition));
            auto newPos     = get_element_position(get_element_no_nodes(newNoNodePosition));
            if(currentPos.v == newPos.v){
                return;
            }

            auto node    = std::move(elements[currentPos.v-1]);
            auto element = std::move(elements[currentPos.v]);

            // remove element
            elements.erase(elements.begin() + currentPos.v);

            // remove node
            elements.erase(elements.begin() + currentPos.v-1);

            if(newPos.v > currentPos.v){
                elements.insert(elements.begin() + newPos.v - 1, std::move(element));
                elements.insert(elements.begin() + newPos.v - 1, std::move(node));
            }else{
                elements.insert(elements.begin() + newPos.v, std::move(node));
                elements.insert(elements.begin() + newPos.v, std::move(element));
            }

            compute_loops_levels();
            update_conditions();
            add_to_update_flag(UpdateRoutines | UpdateFlow | UpdateSelection);
        }
    }
}

void Experiment::update_element_name(ElementKey elementKey, QString elemName){

    if(auto element = get_element(elementKey); element != nullptr){
        selectedElement->update_name(elemName);
        add_to_update_flag(UpdateComponents | UpdateFlow  | UpdateRoutines | UpdateSelection);
    }    
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
        add_to_update_flag(UpdateRoutines | UpdateSelection);
    }
}

void Experiment::move_routine_condition_up(ElementKey routineKey, RowId id){

    if(auto routine = get_routine(routineKey); routine != nullptr){
        routine->move_condition_up(id);
        add_to_update_flag(UpdateRoutines | UpdateSelection);
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


    auto elementFound = std::find_if(elements.begin(), elements.end(), [isiKey](const std::unique_ptr<FlowElement> &element){
        return element->key() == isiKey.v && element->is_isi();
    });
    if(elementFound != elements.end()){
        return dynamic_cast<Isi*>(elementFound->get());
    }

    QtLogger::error(QSL("[EXP] Isi with key ") % QString::number(isiKey.v) % QSL(" not found."));
    return nullptr;
}

Loop *Experiment::get_loop(ElementKey loopKey) const{

    auto loopFound = std::find_if(loops.begin(), loops.end(), [loopKey](const std::unique_ptr<Loop> &loop){
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
    std::unordered_map<int, Component*> checkComponents;
    std::unordered_map<int, Config*> checkConfigs;
    for(auto component : compM.get_components()){
        if(checkComponents.count(component->key()) != 0){
            QtLogger::error(QSL("[EXP] ") % component->to_string() % QSL(" already exists."));
        }else{
            checkComponents[component->key()] = component;
        }

//        for(const auto &config : component->configs){
//            if(checkConfigs.count(config->key()) != 0){
//                QtLogger::error(QSL("[EXP] ") % config->to_string() % QSL(" already exists."));
//            }else{
//                checkConfigs[config->key()] = config.get();
//            }
//        }
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
        if(elem->type() == FlowElement::Type::Routine){

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
        }else if(elem->type() == FlowElement::Type::Loop){
            if(checkLoops.count(elem->key()) != 0){
                QtLogger::error(QSL("[EXP] ") % dynamic_cast<Loop*>(elem.get())->to_string() % QSL(" already exists."));
            }else{
                checkLoops[elem->key()] = dynamic_cast<Loop*>(elem.get());
            }
        }else if(elem->type() == FlowElement::Type::Isi){
            if(checkIsi.count(elem->key()) != 0){
                QtLogger::error(QSL("[EXP] ") % dynamic_cast<Isi*>(elem.get())->to_string() % QSL(" already exists."));
            }else{
                checkIsi[elem->key()] = dynamic_cast<Isi*>(elem.get());
            }
        }
    }


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
            if(auto connection = condition->get_connection_from_key(connectionKey, false); connection != nullptr){
                connection->selected = true;
            }
        }

        if(doUpdate){
            add_to_update_flag(UpdateRoutines);
        }
    }
}

void Experiment::display_exp_infos(){

    QtLogger::message("### ELEMENTS ###");
    for(const auto &element : elements){
        QtLogger::message(QSL("->") % QString::number(element->key()) % QSL(" ") %element->name() %  QSL(" ") % from_view(FlowElement::get_type_name(element->type())));
    }
}


void Experiment::add_action(ElementKey routineKey, ConditionKey conditionKey, ComponentKey componentKey,
    std::optional<ConfigKey> configKey, bool fillUpdateTimeline, bool fillVisibilityTimeline){

    auto component = compM.get_component(componentKey);
    if(component == nullptr){
        QtLogger::error(QSL("Cannot add action to condition with key [") % QString::number(conditionKey.v) % QSL("] from routine with key [")
            % QString::number(routineKey.v) % QSL("]"));
        return;
    }

    if(auto routine = get_routine(routineKey); routine != nullptr){

        if(routine->isARandomizer){
            return;
        }

        if(auto condition = routine->get_condition(conditionKey); condition != nullptr){

            if(auto action = condition->get_action_from_component_key(componentKey, false); action == nullptr){
                condition->actions.push_back(Action::generate_component_action(
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

    auto component = compM.get_component(componentKey);
    if(component == nullptr){
        QtLogger::error(QSL("[Experiment::add_action_to_all_conditions] Cannot add action.)"));
        return;
    }

    if(auto routine = get_routine(routineKey); routine != nullptr){

        if(routine->isARandomizer){
            return;
        }

        for(auto &condition : routine->conditions){

            if(auto action = condition->get_action_from_component_key(componentKey, false); action == nullptr){

                condition->actions.push_back(Action::generate_component_action(
                    component, condition->duration, configKey, fillUpdateTimeline, fillVisibilityTimeline));
            }
        }
    }

    add_to_update_flag(UpdateRoutines);
}

void Experiment::add_action_to_all_routines_conditions(ComponentKey componentKey,
    std::optional<ConfigKey> configKey, bool fillUpdateTimeline, bool fillVisibilityTimeline){

    auto component = compM.get_component(componentKey);
    if(component == nullptr){
        QtLogger::error(QSL("[Experiment::add_action_to_all_routines_conditions] Cannot add action to all routines conditions.)"));
        return;
    }

    for(auto &routine : get_elements_from_type<Routine>()){

        if(routine->isARandomizer){
            continue;
        }

        for(auto &condition : routine->conditions){

            if(auto action = condition->get_action_from_component_key(componentKey, false); action == nullptr){

                condition->actions.push_back(Action::generate_component_action(
                    component, condition->duration, configKey, fillUpdateTimeline, fillVisibilityTimeline));
            }
        }
    }

    add_to_update_flag(UpdateRoutines);
}

void Experiment::insert_action_to(ComponentKey componentKey, std::vector<std::tuple<ElementKey,ConditionKey,ConfigKey,bool, bool>> details){

    auto component = compM.get_component(componentKey);
    if(component == nullptr){
        QtLogger::error(QSL("Cannot insert action.)"));
        return;
    }

    for(auto &detail : details){
        if(auto routine = get_element_from_type_and_id<Routine>(std::get<0>(detail)); routine != nullptr){
            if(auto condition = routine->get_condition(std::get<1>(detail)); condition != nullptr) {
                if(auto config = component->get_config(std::get<2>(detail)); config != nullptr){
                    if(auto action = condition->get_action_from_component_key(componentKey, false); action == nullptr){
                        condition->actions.push_back(Action::generate_component_action(
                            component,
                            condition->duration,
                            ConfigKey{config->key()},
                            std::get<3>(detail),
                            std::get<4>(detail))
                        );
                    }
                }
            }
        }
    }

    add_to_update_flag(UpdateRoutines);
}

void Experiment::modify_action(ElementKey routineKey, ConditionKey conditionKey, ComponentKey componentKey,
    bool changeConfig, bool changeUpdateTimeline, bool changeVisibilityTimeline, ConfigKey configKey, bool fillUpdateTimeline, bool fillVisibilityTimeline){

    auto component = compM.get_component(componentKey);
    if(component == nullptr){
        QtLogger::error(QSL("Cannot modify action.)"));
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


    auto component = compM.get_component(componentKey);
    if(component == nullptr){
        QtLogger::error(QSL("Cannot modify action.)"));
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

    auto component = compM.get_component(componentKey);
    if(component == nullptr){
        QtLogger::error(QSL("Cannot modify action.)"));
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

void Experiment::add_loop_sets(ElementKey loopKey, QString sets, RowId id){

    if(auto loop = get_loop(loopKey); loop != nullptr){

        if(loop->is_default()){
            loop->set_sets(sets.split("\n"));
        }else{
            loop->add_sets(sets.split("\n"), id);
        }

        update_conditions();
        add_to_update_flag(UpdateSelection | UpdateFlow | UpdateRoutines);
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
        loop->modify_set_occurencies_nb(setOccuranciesNb, id);
        add_to_update_flag(UpdateSelection);
    }else{
        QtLogger::error(QSL("[Experiment::modify_loop_set_occurrencies_nb] Cannot get loop from key [") % QString::number(loopKey.v) % QSL("]"));
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
    }
}

void Experiment::modify_loop_N(ElementKey loopKey, int N){

    if(auto loop = get_loop(loopKey); loop != nullptr){
        loop->set_N(N);
    }
}

void Experiment::modify_loop_no_following_value(ElementKey loopKey, bool state){
    if(auto loop = get_loop(loopKey); loop != nullptr){
        loop->noFollowingValues = state;
    }
}

void Experiment::remove_set(ElementKey loopKey, RowId id){
    if(auto loop = get_loop(loopKey); loop != nullptr){
        loop->remove_set(id);
        update_conditions();
        add_to_update_flag(UpdateRoutines | UpdateSelection | UpdateFlow);
    }
}

void Experiment::sort_loop_sets_lexico(ElementKey loopKey){
    if(auto loop = get_loop(loopKey); loop != nullptr){
        loop->sort_sets_lexico();
        update_conditions();
        add_to_update_flag(UpdateRoutines | UpdateSelection | UpdateFlow);
    }
}

void Experiment::sort_loop_sets_num(ElementKey loopKey){
    if(auto loop = get_loop(loopKey); loop != nullptr){
        loop->sort_sets_num();
        update_conditions();
        add_to_update_flag(UpdateRoutines | UpdateSelection | UpdateFlow);
    }
}

void Experiment::move_loop_set_up(ElementKey loopKey, RowId id){

    if(auto loop = get_loop(loopKey); loop != nullptr){
        loop->move_set_up(id);
        update_conditions();
        add_to_update_flag(UpdateRoutines | UpdateSelection | UpdateFlow);
    }
}

void Experiment::move_loop_set_down(ElementKey loopKey, RowId id){

    if(auto loop = get_loop(loopKey); loop != nullptr){
        loop->move_set_down(id);
        update_conditions();
        add_to_update_flag(UpdateRoutines | UpdateSelection | UpdateFlow);
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

void Experiment::add_timeline_interval(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey, bool updateTimeline, Interval interval){

    if(auto action = get_action(routineKey, conditionKey, actionKey); action != nullptr){

        if(updateTimeline){
            if(action->timelineUpdate->add_interval(std::move(interval))){
                add_to_update_flag(UpdateRoutines);
            }
        }else{
            if(action->timelineVisibility->add_interval(std::move(interval))){
                add_to_update_flag(UpdateRoutines);
            }
        }        
    }
}

void Experiment::remove_timeline_interval(ElementKey routineKey, ConditionKey conditionKey, ActionKey actionKey, bool updateTimeline, Interval interval){

    if(auto action = get_action(routineKey, conditionKey, actionKey); action != nullptr){
        if(updateTimeline){
            if(action->timelineUpdate->remove_interval(std::move(interval))){
                add_to_update_flag(UpdateRoutines);
            }
        }else{
            if(action->timelineVisibility->remove_interval(std::move(interval))){
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
    std::vector<ElementKey> idLoops;
    states.maximumDeepLevel = -1;
    for(auto& elem : elements){
        if(elem->type() == FlowElement::Type::LoopStart){
            auto node = dynamic_cast<LoopNode*>(elem.get());
            node->insideLoopsID       = idLoops;
            node->loop->insideLoopsID = idLoops;
            idLoops.push_back(ElementKey{node->key()});
         }else if(elem->type() ==FlowElement::Type::LoopEnd){
            auto node = dynamic_cast<LoopNode*>(elem.get());
            idLoops.erase(std::find(idLoops.begin(),idLoops.end(), ElementKey{node->key()}));
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
                if(loop->key() == id.v){
                    elem->insideLoops.push_back(loop.get());
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
                if(iLoop->key() == id.v){
                    loop->insideLoops.push_back(iLoop.get());
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
            m.push_back(l1[ii] % QSL("-") % l2[jj]);
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
                routine->conditions.push_back(Condition::generate_new_default());
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
        std::vector<std::vector<QString>> setsId;
        for(const auto& insideLoop : routine->insideLoops){

            auto loop = dynamic_cast<Loop*>(insideLoop);

            std::vector<QString> loopSetsId;
            loopSetsId.reserve(loop->sets.size());
            for(const auto &s : loop->sets){
                loopSetsId.push_back(QString::number(s->key()));
            }
            setsId.push_back(std::move(loopSetsId));

        }

        // mix new conditions keys
        std::vector<QString> newConditionsKeysStr;
        while(setsId.size() > 1){
            setsId[setsId.size()-2] = mix(setsId[setsId.size()-2],setsId[setsId.size()-1]);
            setsId.erase(std::end(setsId)-1);
        }
        if(setsId.size() > 0){
            newConditionsKeysStr = std::move(setsId[0]);
        }

        //  create all current conditions names
        std::vector<std::vector<SetKey>> newConditionsKeys;
        std::vector<QString> newConditionsNames;
        for(const auto &conditionKeysStr : newConditionsKeysStr){

            std::vector<SetKey> conditionKeys;
            QStringList conditionName;

            for(const auto &key : conditionKeysStr.split("-")){

                bool found = false;
                for(const auto& insideLoop : routine->insideLoops){

                    auto loop = dynamic_cast<Loop*>(insideLoop);
                    if(auto set = loop->get_set(SetKey{key.toInt()}); set != nullptr){
                        conditionName << set->name;
                        conditionKeys.push_back(set->s_key());
                        found = true;
                        break;
                    }
                }

                if(!found){
                    QtLogger::error(QSL("[EXP] Cannot retrieve set with key ") % key % QSL(" from any loop."));
                }
            }
            newConditionsKeys.push_back(std::move(conditionKeys));
            newConditionsNames.push_back(conditionName.join("-"));
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
        std_v1<std::unique_ptr<Condition>> newConditions;
        for(size_t ii = 0; ii < newConditionsKeys.size(); ++ii){

            bool found = false;
            for(size_t jj = 0; jj < routine->conditions.size(); ++jj){

                if(routine->conditions[jj]->contains_same_set_keys(newConditionsKeys[ii])){
                    // all keys found
                    routine->conditions[jj]->name = newConditionsNames[ii];
                    newConditions.push_back(std::move(routine->conditions[jj]));
                    routine->conditions.erase(std::begin(routine->conditions) + static_cast<int>(jj));
                    found = true;
                    break;
                }
            }

            if(!found){ // not found, create new condition
                auto condition = Condition::generate_new(newConditionsNames[ii]);
                condition->setsKeys = std::move(newConditionsKeys[ii]);
                newConditions.push_back(std::move(condition));
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
    compM.update_component_position(componentKey, id);
    add_to_update_flag(UpdateComponents);
}

void Experiment::remove_component(ComponentKey componentKey){

    // remove action which use this component
    for(auto routine : get_elements_from_type<Routine>()){

        // remove action containing component from conditions
        for(auto &condition : routine->conditions){
            if(auto action = condition->get_action_from_component_key(componentKey, false); action != nullptr){
                QtLogger::message(QSL("[EXP] Remove ") % action->to_string() % QSL(" using component ") % action->component->name() % QSL(" from ") %  condition->to_string() % QSL(" from ") % routine->to_string());
                condition->remove_action(ActionKey{action->key()});
            }
        }
    }

    compM.remove_component(componentKey);
    add_to_update_flag(UpdateComponents | UpdateRoutines);
}

void Experiment::duplicate_component(ComponentKey componentKey){
    compM.duplicate_component(componentKey);
    add_to_update_flag(UpdateComponents);
}

void Experiment::add_new_component(Component::Type type, RowId id){
    if(compM.insert_new_component(type, id)){
        add_to_update_flag(UpdateComponents);
    }
}

void Experiment::copy_component(Component *component, std::vector<ConfigKey> configKeys, RowId id){
    compM.insert_copy_of_component(component, std::move(configKeys), id);
    add_to_update_flag(UpdateComponents | UpdateRoutines);
}

void Experiment::update_component_name(ComponentKey componentKey, QString name){

    if(compM.update_component_name(componentKey, name)){
        add_to_update_flag(UpdateComponents | UpdateRoutines);
    }
}

void Experiment::sort_components_by_category(){
    compM.sort_by_category();
    add_to_update_flag(UpdateComponents);
}

void Experiment::sort_components_by_type(){
    compM.sort_by_type();
    add_to_update_flag(UpdateComponents);
}

void Experiment::sort_components_by_name(){
    compM.sort_by_name();
    add_to_update_flag(UpdateComponents);
}

void Experiment::delete_unused_components(){

//    QtLogger::message("### CHECK DUPLICATE NAMES###");
//    std::set<QString> cNames;
//    for(auto &component : compM.components){

//        auto n = component->name();
//        if(cNames.contains(n)){
//            size_t iter = 1;
//            while(cNames.contains(n)){
//                n = component->name() % QSL(" (") % QString::number(iter++) % QSL(")");
//            }
//            QtLogger::message(QSL("Rename component [") % component->name() % QSL("] into [") % n % QSL("]"));
//            component->set_name(n);
//        }
//    }


//    QtLogger::message("### REMOVE ACTIONS###");
//    std::vector<std::tuple<ElementKey, ConditionKey, ActionKey>> actionsToRemove;
//    for(const auto &routine : get_elements_from_type<Routine>()){
//        for(const auto &condition : routine->conditions){
//            for(const auto &action : condition->actions){
//                auto timeOpt = Component::get_timeline_opt(action->component->type);
//                bool removeAction = false;
//                if(timeOpt == Component::TimelineO::Both){
//                    if(action->timelineUpdate->intervals.size() == 0 && action->timelineVisibility->intervals.size() == 0){
//                        removeAction = true;
//                    }
//                }else if(timeOpt == Component::TimelineO::Update){
//                    if(action->timelineUpdate->intervals.size() == 0){
//                        removeAction = true;
//                    }
//                }else if(timeOpt == Component::TimelineO::Visibility){
//                    if(action->timelineVisibility->intervals.size() == 0){
//                        removeAction = true;
//                    }
//                }

//                if(removeAction){
//                    QtLogger::message(QSL(" - remove component [") % action->component->name() % QSL("] from [") % condition->name % QSL("] from routine [") % routine->name() % QSL("]"));
//                    actionsToRemove.push_back(std::make_tuple(ElementKey{routine->key()}, ConditionKey{condition->key()}, ActionKey{action->key()}));
//                }
//            }
//        }
//    }

//    for(const auto &action : actionsToRemove){
//        remove_action_from_condition(std::get<0>(action), std::get<1>(action), std::get<2>(action), false);
//    }

    QtLogger::message("### REMOVE COMPONENTS###");
    std::unordered_set<int> keys;
    for(const auto &routine : get_elements_from_type<Routine>()){
        for(const auto &condition : routine->conditions){
            for(const auto &action : condition->actions){
                keys.emplace(action->component->key());
            }
        }
    }

    std::vector<Component*> componentsToRemove;
    for(auto component : compM.get_components()){
        if(!keys.contains(component->key())){
            componentsToRemove.push_back(component);
        }
    }

    for(const auto &c : componentsToRemove){
        remove_component(c->c_key());
    }

    add_to_update_flag(UpdateComponents | UpdateRoutines);
}

void Experiment::select_config_in_component(ComponentKey componentKey, RowId id){
    if(auto component = get_component(componentKey); component != nullptr){
        if(component->select_config(id)){
            add_to_update_flag(UpdateComponents | UpdateRoutines);
        }
    }
}

void Experiment::insert_config_in_component(ComponentKey componentKey, RowId id, QString configName){

    if(auto component = get_component(componentKey); component != nullptr){
        if(component->insert_config(id, configName)){
            add_to_update_flag(UpdateComponents | UpdateRoutines);
        }
    }
}

void Experiment::copy_config_from_component(ComponentKey componentKey, RowId id, QString configName){

    if(auto component = get_component(componentKey); component != nullptr){
        if(component->copy_config(id, configName)){
            add_to_update_flag(UpdateComponents | UpdateRoutines);
        }
    }
}

void Experiment::remove_config_from_component(ComponentKey componentKey, RowId id){

    if(auto component = get_component(componentKey); component != nullptr){

        auto config = component->get_config(id);
        if(config == nullptr){
            return;
        }

        auto configKey = ConfigKey{config->key()};
        for(auto routine : get_elements_from_type<Routine>()){

            for(auto &condition : routine->conditions){

                std_v1<ActionKey> actionsToRemoved;
                for(auto &action : condition->actions){
                    if(action->config->key() == configKey.v){
                        QtLogger::message(QSL("[EXP] Remove ") % action->to_string() % QSL(" from ") % condition->to_string() %
                                          QSL(" from ") % routine->to_string());
                        actionsToRemoved.push_back(ActionKey{action->key()});
                    }
                }

                for(auto actionToRemove : actionsToRemoved){
                    condition->remove_action(actionToRemove);
                }
            }
        }

        component->remove_config(id);
        add_to_update_flag(UpdateComponents | UpdateRoutines);
    }
}

void Experiment::move_config_in_component(ComponentKey componentKey, RowId from, RowId to){
    if(auto component = get_component(componentKey); component != nullptr){        
        if(component->move_config(from, to)){
            add_to_update_flag(UpdateComponents | UpdateRoutines);
        }
    }
}

void Experiment::rename_config_in_component(ComponentKey componentKey, RowId id, QString configName){

    if(auto component = get_component(componentKey); component != nullptr){
        if(component->rename_config(id, configName)){
            add_to_update_flag(UpdateComponents | UpdateRoutines);
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
    resM.add_resources(type, filesPath);
    add_to_update_flag(UpdateResources | UpdateComponents | UpdateRoutines);
}

void Experiment::update_resource_path(QString currentPath, QString newPath){
    resM.update_resource_path(currentPath, newPath);
    add_to_update_flag(UpdateResources);
}

void Experiment::update_resource_alias(QString currentAlias, QString newAlias){
    resM.update_resource_alias(currentAlias, newAlias);
    add_to_update_flag(UpdateResources | UpdateComponents | UpdateRoutines);
}

void Experiment::select_resource(Resource::Type type, size_t index){
    resM.select_resource(type, index);
    add_to_update_flag(UpdateResources);
}

void Experiment::remove_resource(Resource::Type type, size_t index){
    resM.remove_resource(type, index);
    add_to_update_flag(UpdateResources | UpdateComponents | UpdateRoutines);
}

void Experiment::clean_resources(Resource::Type type){
    resM.clean_resources(type);
    add_to_update_flag(UpdateResources | UpdateComponents | UpdateRoutines);
}

void Experiment::update_reload_resource_code(int reloadCode){
    resM.set_reload_code(reloadCode);
}

void Experiment::copy_resource(Resource *resource){
    resM.copy_resource(resource);
    add_to_update_flag(UpdateResources | UpdateComponents | UpdateRoutines);
}

Component *Experiment::get_component(ComponentKey componentKey) const{
    return compM.get_component(componentKey);
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
                            if(loopSet->name == set){
                                found = true;
                                ++countMatched;
                                condition->setsKeys.push_back(loopSet->s_key());
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
    elements.push_back(std::make_unique<NodeFlow>()); // init with one node
    compute_loops_levels();
    update_conditions();

    add_new_component(Component::Type::Global_logger, RowId{0});

    add_to_update_flag(UpdateAll | ResetUI);
}

void Experiment::clean_experiment(){

    QtLogger::message(QSL("[EXP] Clean experiment"));

    // remove resources
    resM.clean_resources();

    // remove elements
    elements.clear();
    loops.clear();

    // remove components
    compM.clean_components();

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

            states.currentElementType = FlowElement::Type::Routine;
            if(auto routine = get_routine(elementKey); routine != nullptr){

                states.currentElementName = routine->name();

                if(states.currentElementKey != routine->key()){
                    states.currentElementKey = routine->key();
                    updateCurrentElement = true;
                }

                if(auto condition = routine->get_condition(conditionKey); condition != nullptr){
                    states.currentTypeSpecificInfo = clamp(condition->name, 15);
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

            states.currentElementType = FlowElement::Type::Isi;

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
        if(startLoop->type() == FlowElement::Type::LoopStart){
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
        l.push_back(loop.get());
    }
    return l;
}


auto Experiment::get_element_iterator(ElementKey elementKey) const -> std::vector<std::unique_ptr<FlowElement>>::const_iterator{
    return std::find_if(elements.begin(), elements.end(), [elementKey](const std::unique_ptr<FlowElement> &element){
        return element->key() == elementKey.v;
    });
}


auto Experiment::get_element_iterator(FlowElement *element) const -> std::vector<std::unique_ptr<FlowElement>>::const_iterator{
    return std::find_if(elements.begin(), elements.end(), [element](const std::unique_ptr<FlowElement> & currElem){
        return currElem.get() == element;
    });
}


#include "moc_experiment.cpp"
