
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

#include "flow_sequence.hpp"

using namespace tool::ex;

void FlowSequence::reset(){
    nodesElements.clear();
    routinesElements.clear();
    ISIsElements.clear();
    loopsStart.clear();
    loopsEnd.clear();
    loopsElements.clear();
    elements.clear();
}

void FlowSequence::update_from_experiment(Experiment *exp){

    std_v1<NodeFlowElement*>        newNodesElements;
    std_v1<RoutineFlowElement*>     newRoutinesElements;
    std_v1<IsiFlowElement*>         newISIsElements;
    std_v1<LoopNodeFlowElement*>    newLoopsStart;
    std_v1<LoopNodeFlowElement*>    newLoopsEnd;
    std_v1<LoopFlowElementUP>       newLoopsElements;   // not in elements
    std_v1<FlowElementUP>           newElements;        // all elements

    for(auto &newElement : exp->elements){

        bool found = false;
        FlowElementUP oldE = nullptr;

        for(auto &oldElement : elements){
            if(oldElement == nullptr){
                continue;
            }

            if(newElement->key() == oldElement->key.v){
                found = true;
                oldE = std::move(oldElement);
                oldElement = nullptr;
                break;
            }
        }

        if(found){ // move from previous array
            // update
            oldE->update(newElement.get());
            switch (newElement->type) {
                case Element::Type::Node:{
                    newNodesElements.emplace_back(dynamic_cast<NodeFlowElement*>(oldE.get()));
                    newElements.emplace_back(std::move(oldE));
                }break;{
                case Element::Type::Routine:
                    newRoutinesElements.emplace_back(dynamic_cast<RoutineFlowElement*>(oldE.get()));
                    newElements.emplace_back(std::move(oldE));
                }break;{
                case Element::Type::Isi:
                    newISIsElements.emplace_back(dynamic_cast<IsiFlowElement*>(oldE.get()));
                    newElements.emplace_back(std::move(oldE));
                }break;{
                case Element::Type::LoopStart:
                    newLoopsStart.emplace_back(dynamic_cast<LoopNodeFlowElement*>(oldE.get()));
                    newElements.emplace_back(std::move(oldE));
                }break;{
                case Element::Type::LoopEnd:
                    newLoopsEnd.emplace_back(dynamic_cast<LoopNodeFlowElement*>(oldE.get()));
                    newElements.emplace_back(std::move(oldE));
                }break;
                default:
                break;
            }
        }else{ // create new
            switch (newElement->type) {{
                case Element::Type::Node:
                    auto nodeElement = std::make_unique<NodeFlowElement>(dynamic_cast<NodeFlow*>(newElement.get()));
                    newNodesElements.emplace_back(nodeElement.get());
                    newElements.emplace_back(std::move(nodeElement));
                }break;{
                case Element::Type::Routine:
                    auto routineElement = std::make_unique<RoutineFlowElement>(dynamic_cast<Routine*>(newElement.get()));
                    newRoutinesElements.emplace_back(routineElement.get());
                    newElements.emplace_back(std::move(routineElement));
                }break;{
                case Element::Type::Isi:
                    auto isiElement = std::make_unique<IsiFlowElement>(dynamic_cast<Isi*>(newElement.get()));
                    newISIsElements.emplace_back(isiElement.get());
                    newElements.emplace_back(std::move(isiElement));
                }break;{
                case Element::Type::LoopStart:
                    auto loopStart = std::make_unique<LoopNodeFlowElement>(dynamic_cast<LoopNode*>(newElement.get()));
                    newLoopsStart.emplace_back(loopStart.get());
                    newElements.emplace_back(std::move(loopStart));
                }break;{
                case Element::Type::LoopEnd:
                    auto loopEnd = std::make_unique<LoopNodeFlowElement>(dynamic_cast<LoopNode*>(newElement.get()));
                    newLoopsEnd.emplace_back(loopEnd.get());
                    newElements.emplace_back(std::move(loopEnd));
                }break;
                default:
                break;
            }
        }
    }

    nodesElements   = std::move(newNodesElements);
    routinesElements= std::move(newRoutinesElements);
    ISIsElements    = std::move(newISIsElements);
    loopsStart      = std::move(newLoopsStart);
    loopsEnd        = std::move(newLoopsEnd);
    elements        = std::move(newElements);

    // associate loops
    for(auto &newLoop : exp->loops){

        bool found = false;
        LoopFlowElementUP oldL = nullptr;

        for(auto &oldLoop : loopsElements){
            if(oldLoop == nullptr){
                continue;
            }

            if(newLoop->key() == oldLoop->key.v){
                found = true;
                oldL = std::move(oldLoop);
                oldLoop = nullptr;
                break;
            }
        }

        if(found){
            oldL->update(newLoop.get());
            auto loopElementPtr = oldL.get();
            auto start = dynamic_cast<LoopNodeFlowElement*>(elements[exp->get_element_position(newLoop->start)].get());
            auto end   = dynamic_cast<LoopNodeFlowElement*>(elements[exp->get_element_position(newLoop->end)].get());
            loopElementPtr->startLoopNode = start;
            loopElementPtr->endLoopNode   = end;
            newLoopsElements.emplace_back(std::move(oldL));
        }else{
            auto loopElement = std::make_unique<LoopFlowElement>(newLoop.get());
            auto loopElementPtr = loopElement.get();
            auto start = dynamic_cast<LoopNodeFlowElement*>(elements[exp->get_element_position(newLoop->start)].get());
            auto end   = dynamic_cast<LoopNodeFlowElement*>(elements[exp->get_element_position(newLoop->end)].get());
            loopElementPtr->startLoopNode = start;
            loopElementPtr->endLoopNode   = end;
            start->loopElement = loopElementPtr;
            end->loopElement   = loopElementPtr;
            newLoopsElements.emplace_back(std::move(loopElement));
        }
    }

    loopsElements = std::move(newLoopsElements);

    // active move elements
    for(auto &element : elements){
        if(element->type == Element::Type::Node){
            continue;
        }

        auto moveElem = dynamic_cast<MovableFlowElement*>(element.get());
        moveElem->canMoveToLeft = true;
        moveElem->canMoveToRight = true;

        size_t id = flow_position(element.get());
        moveElem->canMoveToLeft  &= id > 1;
        moveElem->canMoveToRight &= id < elements.size()-2;

        auto previousElem = moveElem->canMoveToLeft  ? dynamic_cast<MovableFlowElement*>(elements[id-2].get()) : nullptr;
        auto nextElem     = moveElem->canMoveToRight ? dynamic_cast<MovableFlowElement*>(elements[id+2].get()) : nullptr;

        if(previousElem){
            if(previousElem->type == Element::Type::LoopStart && moveElem->type == Element::Type::LoopEnd){
                moveElem->canMoveToLeft = false;
            }
            if(previousElem->type == Element::Type::LoopStart && moveElem->type == Element::Type::LoopStart){
                moveElem->canMoveToLeft = false;
            }
        }
        if(nextElem){
            if(nextElem->type == Element::Type::LoopEnd && moveElem->type == Element::Type::LoopStart){
                moveElem->canMoveToRight = false;
            }
            if(nextElem->type == Element::Type::LoopEnd && moveElem->type == Element::Type::LoopEnd){
                moveElem->canMoveToRight = false;
            }
        }
    }
}

FlowElement *FlowSequence::mouse_on_element(const QPoint &mousePos){

    for(auto& elem : elements){
        if(elem->uiElemRect.contains(mousePos)){
            return elem.get();
        }
    }
    return nullptr;
}

LoopFlowElement *FlowSequence::mouse_on_loop(const QPoint &mousePos){

    for(auto& elem : loopsElements){
        if(elem->uiElemRect.contains(mousePos)){
            return elem.get();
        }
    }
    return nullptr;
}

void FlowSequence::check_click_on_elements(QPoint clickPos) noexcept{

    // check in elements
    if(auto selectedElement = mouse_on_element(clickPos); selectedElement != nullptr){
        qDebug() << "element";
        emit select_element_signal(selectedElement->key);
        return;
    }

    // check in loops
    if(auto selectedLoop = mouse_on_loop(clickPos); selectedLoop != nullptr){
        qDebug() << "loop";
        emit select_element_signal(selectedLoop->key);
        return;
    }

    // check in add elements
    for(auto& node : nodesElements){

        if(node == nullptr){
            return;
        }

        if(node->is_selected()){

            if(node->addRoutine->uiElemRect.contains(clickPos)){
                emit add_element_signal(Element::Type::Routine,   flow_position(node));
                return;
            }else if(node->addIsi->uiElemRect.contains(clickPos)){
                emit add_element_signal(Element::Type::Isi,       flow_position(node));
                return;
            }else if(node->addLoop->uiElemRect.contains(clickPos)){
                emit add_element_signal(Element::Type::Loop,      flow_position(node));
                return;
            }
        }
    }

    // check in remove and move elements
    for(const auto& element : elements){

        if(element->type == Element::Type::Node || !element->is_selected()){
            continue;
        }
        const auto moveElem =  dynamic_cast<MovableFlowElement*>(element.get());
        if(moveElem->removeElement->uiElemRect.contains(clickPos)){
            emit remove_selected_element_signal();
            return;
        }

        int id = to_signed(flow_position(moveElem));
        if(moveElem->moveLeftElement->uiElemRect.contains(clickPos) && id > 1){
            emit move_element_left_signal(to_unsigned(id));
            return;
        }

        if(moveElem->moveRightElement->uiElemRect.contains(clickPos) && to_unsigned(id) < elements.size()-2){
            emit move_element_right_signal(to_unsigned(id));
            return;
        }
    }

    emit unselect_element_signal();
}

void FlowSequence::draw(QPainter &painter, qreal zoomLevel){

    for(auto& element : elements){
        element->draw(painter, zoomLevel);
    }

    for(auto& loop : loopsElements){
        loop->draw(painter, zoomLevel);
    }
}

FlowElement *FlowSequence::current_selection() const{

    for(const auto &element : elements){
        if(element->is_selected()){
            return element.get();
        }
    }
    for(const auto &loop : loopsElements){
        if(loop->is_selected()){
            return loop.get();
        }
    }
    return nullptr;
}

size_t FlowSequence::flow_position(FlowElement *elem) const{
    for(size_t ii = 0; ii < elements.size(); ++ii){
        if(elements[ii].get() == elem){
            return ii;
        }
    }
    qCritical() << "Cannot find flow element " << elem->key.v;
    return 0;
}

#include "moc_flow_sequence.cpp"
