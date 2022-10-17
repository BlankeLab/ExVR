
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

// base
#include "utility/tuple_array.hpp"

namespace tool::ex{

using namespace std::literals::string_view_literals;

struct ConnectionNode{

    enum class Type : int{
        boolean_t, integer_t,float_t, real_t, decimal_t, string_t,
        vector2_t, vector3_t, transform_t, color_t,
        string_list_t, decimal_list_t, real_list_t, gameobject_list_t,
        any_t, void_t,
        id_any_t, string_any_t, time_any_t,
        lm_frame_t, lm_hands_frame_t, image_t, plot_t, variant_t, kinect_body_t, component_out_data_t,
        keyboard_button_event_t, joypad_button_event_t, joypad_axis_event_t, mouse_button_event_t, mouse_axis_event_t,
        SizeEnum};

    using T = Type;
    using TConnectionNode = std::tuple<T,std::string_view,std::string_view>;
    static constexpr TupleArray<T::SizeEnum,TConnectionNode> connectionsNodes= {{
        // type                         // id                      // name
        TConnectionNode
        {T::boolean_t,                  "bool"sv,                  "bool"sv },
        {T::integer_t,                  "integer"sv,               "int"sv },
        {T::float_t,                    "float"sv,                 "float"sv },
        {T::real_t,                     "real"sv,                  "real"sv },
        {T::decimal_t,                  "decimal"sv,               "dec"sv },
        {T::string_t,                   "string"sv,                "str"sv },
        {T::vector2_t,                  "vector2"sv,               "vec2"sv },
        {T::vector3_t,                  "vector3"sv,               "vec3"sv },
        {T::color_t,                    "color"sv,                 "color"sv },
        {T::transform_t,                "transform"sv,             "transfo"sv },
        {T::string_list_t,              "string_list"sv,           "str list"sv },
        {T::decimal_list_t,             "decimal_list"sv,          "dec list"sv },
        {T::real_list_t,                "real_list"sv,             "real list"sv },
        {T::gameobject_list_t,          "game_object_list"sv,      "GO list"sv },
        {T::any_t,                      "any"sv,                   "any"sv },
        {T::void_t,                     "trigger"sv,               "trigger"sv },
        {T::lm_frame_t,                 "lm_frame"sv,              "LM frame"sv },
        {T::lm_hands_frame_t,           "lm_hands_frame"sv,        "LM hands frame"sv },
        {T::kinect_body_t,              "kinect_body"sv,           "Kinect body"sv },
        {T::image_t,                    "image"sv,                 "image"sv },
        {T::plot_t,                     "plot"sv,                  "plot"sv },
        {T::variant_t,                  "variant"sv,               "variant"sv},
        {T::id_any_t,                   "id_any"sv,                "id any"sv},
        {T::string_any_t,               "string_any"sv,            "str any"sv},
        {T::time_any_t,                 "time_any"sv,              "time any"sv},
        {T::keyboard_button_event_t,    "keyboard_button"sv,       "k. button"sv},
        {T::joypad_button_event_t,      "joypad_button"sv,         "j. button."sv},
        {T::joypad_axis_event_t,        "joypad_axis"sv,           "j. axis."sv},
        {T::mouse_button_event_t,       "mouse_button"sv,          "m. button."sv},
        {T::mouse_axis_event_t,         "mouse_axis"sv,            "m. axis."sv},
    }};

    static Type get_type_from_name(std::string_view name) {
        return connectionsNodes.at<2,0>(name);
    }

    static std::string_view get_id(Type type) {
        return connectionsNodes.at<0,1>(type);
    }

    static std::string_view get_name(Type type) {
        return connectionsNodes.at<0,2>(type);
    }
};


}
