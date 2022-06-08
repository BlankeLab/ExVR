
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

#include "action.hpp"

#include "component.hpp"
#include "timeline.hpp"

using namespace tool::ex;

Action::Action(Component *component, Config *config, ActionKey id) : m_key(IdKey::Type::Action, id.v){
    this->component    = component;
    this->config       = config;
}

std::unique_ptr<Action> Action::generate_component_action(Component *component, tool::SecondsTS duration,
        std::optional<ConfigKey> configKey, bool fillUpdateTimeline, bool fillVisibilityTimeline){

    auto action = std::make_unique<Action>(component,
        configKey.has_value() ? component->get_config(configKey.value()) : component->get_config(RowId{0}), ActionKey{-1});
    action->timelineUpdate     = std::make_unique<Timeline>(Timeline::Type::Update);
    if(fillUpdateTimeline){
        action->timelineUpdate->add_interval({SecondsTS{0}, duration});
    }
    action->timelineVisibility = std::make_unique<Timeline>(Timeline::Type::Visibility);
    if(fillVisibilityTimeline){
        action->timelineVisibility->add_interval({SecondsTS{0}, duration});
    }
    return action;
}

void Action::select_config(RowId configTabId){
    if(auto conf = component->get_config(configTabId); conf != nullptr){
        config = conf;
    }
}

std::unique_ptr<Action> Action::copy_with_new_element_id(const Action &actionToCopy){
    auto action                = std::make_unique<Action>(actionToCopy.component, actionToCopy.config, ActionKey{-1});
    action->timelineUpdate     = Timeline::copy_with_new_element_id(*actionToCopy.timelineUpdate);
    action->timelineVisibility = Timeline::copy_with_new_element_id(*actionToCopy.timelineVisibility);
    action->nodePosition       = actionToCopy.nodePosition;
    action->nodeUsed           = actionToCopy.nodeUsed;
    return action;
}

void Action::check_integrity(){

}

void Action::update_intervals_with_max_length(tool::SecondsTS maxLength){

    timelineUpdate->intervals.erase(std::remove_if(timelineUpdate->intervals.begin(), timelineUpdate->intervals.end(),[=](Interval& i) {
        return (i.start.v > maxLength.v);
    }), timelineUpdate->intervals.end());

    timelineVisibility->intervals.erase(std::remove_if(timelineVisibility->intervals.begin(), timelineVisibility->intervals.end(),[=](Interval& i) {
        return (i.start.v > maxLength.v);
    }), timelineVisibility->intervals.end());

    for(auto &interval : timelineUpdate->intervals){
        if(interval.inside(maxLength)){
            interval.end = maxLength;
        }
    }
    for(auto &interval : timelineVisibility->intervals){
        if(interval.inside(maxLength)){
            interval.end = maxLength;
        }
    }
}
