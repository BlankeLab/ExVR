
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
#include "data/id_key.hpp"

// local
#include "forward.hpp"
#include "data/timeline.hpp"
#include "data/connection.hpp"

namespace tool::ex {

struct Action;
using ActionUP = std::unique_ptr<Action>;

struct Action{

    Action() = delete;
    Action(Component *component, Config *config, ActionKey id);

    void select_config(RowId  configTabId);
    void update_intervals_with_max_length(SecondsTS maxLength);

    inline QString to_string() const{return QSL("Action(") % QString::number(key()) % QSL(")");}

    static ActionUP generate_component_action(Component *component, SecondsTS duration,
        std::optional<ConfigKey> configKey, bool fillUpdateTimeline, bool fillVisibilityTimeline);
    static ActionUP copy_with_new_element_id(const Action &actionToCopy);

    void check_integrity();

    IdKey key;

    Config    *config               = nullptr;
    Component *component            = nullptr;

    TimelineUP timelineUpdate       = nullptr;
    TimelineUP timelineVisibility   = nullptr;

    // ui
    // # graph
    QPointF nodePosition;
    //QSize nodeSize;
    bool nodeUsed = false;
    bool nodeSelected = false;
};

static bool operator<(const ActionUP &l, const ActionUP &r){
    if(l->key() == r->key()){
        return false;
    }

    if(l->component->key() == r->component->key()){
        return false;
    }

    return true;
}

static bool operator==(const ActionUP &l, const ActionUP &r){
    return !(l < r) && !(r < l);
}

}
