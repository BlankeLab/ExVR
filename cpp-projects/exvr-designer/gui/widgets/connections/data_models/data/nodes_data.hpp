
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
#include <variant>
#include <cmath>

// Qt
#include <QDebug>

// nodes
#include "nodes/NodeData.hpp"
using QtNodes::NodeData;
using QtNodes::NodeDataType;

// base
#include "input/keyboard.hpp"
#include "input/joypad.hpp"
#include "utility/math.hpp"
#include "utility/vector.hpp"
#include "geometry/point2.hpp"
#include "geometry/point4.hpp"

// qt-utility
#include "qt_convertors.hpp"

// local
#include "data/connection_node.hpp"

namespace tool::ex {

static QString get_id(ConnectionNode::Type t){
    return tool::from_view(ConnectionNode::get_id(t));
}

static QString get_name(ConnectionNode::Type t){
    return tool::from_view(ConnectionNode::get_name(t));
}


template<class T>
QString apply_str_convertion(const T &value){

    static_cast<void>(value); // for removing unused warning
    if constexpr(!std::is_same_v<T, std::shared_ptr<NodeData>>){        
        if constexpr(std::is_same_v<T, QStringList>){
            return str::Convertor::to_str(value, " ");
        }
        if constexpr(!std::is_same_v<T, QStringList>){
            return str::Convertor::to_str(value);
        }
    }
    return "";
}

class BaseNodeData : public NodeData{

public:

    BaseNodeData(ConnectionNode::Type type, QString text = "") :  m_type(type), m_text(text) {
        if(type == ConnectionNode::Type::component_out_data_t){
            m_runtime = true;
        }        
        m_nodeDatatype = NodeDataType {get_id(m_type), get_name(m_type)};
    }

    inline QString value_as_text() const{return m_text;}
    const NodeDataType& type() const override {return m_nodeDatatype;}
    ConnectionNode::Type type_data() const{return m_type;}
    inline const QString &id()   const {return m_nodeDatatype.id;}
    inline const QString &name() const {return m_nodeDatatype.name;}

    constexpr bool is_runtime() const noexcept{
        return m_runtime;
    }

    void set_runtime(){
        m_runtime = true;
    }

protected:

    ConnectionNode::Type m_type;
    QString m_text;
    bool m_runtime = false;
};


template<ConnectionNode::Type T, class V>
class TypeNodeData : public BaseNodeData{

public:

    TypeNodeData() : BaseNodeData(T){}
    TypeNodeData(V value)  :  BaseNodeData(T,  apply_str_convertion(value)), m_value(value){}

    inline V value() const{return m_value;}    

private:
    V m_value;  
};

struct IdAny{
    int id;
    std::shared_ptr<NodeData> data;
};

struct StringAny{
    QString str;
    std::shared_ptr<NodeData> data;
};
struct TimeAny{
    qreal expTime;
    qreal routineTime;
    std::shared_ptr<NodeData> data;
};

struct KeyboardButtonState{
    input::Keyboard::Button code;
    bool pressed;
    bool firstFrameDown;
    bool firstFrameUp;
};

struct MouseButtonState{
    input::Mouse::Button code;
    bool pressed;
    bool firstFrameDown;
    bool firstFrameUp;
};

struct MouseAxisState{
    input::Mouse::Axis code;
    float value;
};

struct JoypadButtonState{
    input::Joypad::Button code;
    bool pressed;
    bool firstFrameDown;
    bool firstFrameUp;
};

struct JoypadAxisState{
    input::Joypad::Axis code;
    float value;
};

template<>
class TypeNodeData<ConnectionNode::Type::id_any_t, IdAny> : public BaseNodeData{

public:
    TypeNodeData() : BaseNodeData(ConnectionNode::Type::id_any_t){}
    TypeNodeData(IdAny value)  :  BaseNodeData(ConnectionNode::Type::id_any_t, apply_str_convertion(value.id)), m_value(value){}

    inline IdAny value() const{return m_value;}

private:
    IdAny m_value;
};

template<>
class TypeNodeData<ConnectionNode::Type::string_any_t, StringAny> : public BaseNodeData{

public:
    TypeNodeData() : BaseNodeData(ConnectionNode::Type::string_any_t){}
    TypeNodeData(StringAny value)  :  BaseNodeData(ConnectionNode::Type::string_any_t, value.str), m_value(value){}

    inline StringAny value() const{return m_value;}

private:
    StringAny m_value;
};

using CNT                       = ConnectionNode::Type;
using IdAnyData                 = TypeNodeData<CNT::id_any_t, IdAny>;
using StringAnyData             = TypeNodeData<CNT::string_any_t, StringAny>;
using TimeAnyData               = TypeNodeData<CNT::time_any_t, TimeAny>;
using BoolData                  = TypeNodeData<CNT::boolean_t, bool>;
using IntData                   = TypeNodeData<CNT::integer_t, int>;
using FloatData                 = TypeNodeData<CNT::float_t, float>;
using RealData                  = TypeNodeData<CNT::real_t, double>;
using StringData                = TypeNodeData<CNT::string_t, QString>;
using DecimalListData           = TypeNodeData<CNT::decimal_list_t, DecimalList>;
using RealListData              = TypeNodeData<CNT::real_list_t, RealList>;
using Vector2Data               = TypeNodeData<CNT::vector2_t, geo::Vec2<float>>;
using Vector3Data               = TypeNodeData<CNT::vector3_t, geo::Vec3<float>>;
using ColorData                 = TypeNodeData<CNT::color_t, QColor>;
using TransformData             = TypeNodeData<CNT::transform_t, Transform>;
using KeyboardButtonEventData   = TypeNodeData<CNT::keyboard_button_event_t, KeyboardButtonState>;
using JoypadButtonEventData     = TypeNodeData<CNT::joypad_button_event_t, JoypadButtonState>;
using JoypadAxisEventData       = TypeNodeData<CNT::joypad_axis_event_t, JoypadAxisState>;
using MouseButtonEventData      = TypeNodeData<CNT::mouse_button_event_t, MouseButtonState>;
using MouseAxisEventData        = TypeNodeData<CNT::mouse_axis_event_t, MouseAxisState>;


using VoidData                  = TypeNodeData<CNT::void_t, VoidV>;
using LeapMotionFrameData       = TypeNodeData<CNT::lm_frame_t, UndefinedV>;
using LeapMotionHandsFrameData  = TypeNodeData<CNT::lm_hands_frame_t, UndefinedV>;
using KinectBodyData            = TypeNodeData<CNT::kinect_body_t, UndefinedV>;
using DecimalData               = TypeNodeData<CNT::decimal_t, Decimal>;
using ImageData                 = TypeNodeData<CNT::image_t, UndefinedV>;
using PlotData                  = TypeNodeData<CNT::plot_t, UndefinedV>;
using AnyData                   = TypeNodeData<CNT::any_t, std::shared_ptr<NodeData>>;
using StringListData            = TypeNodeData<CNT::string_list_t, QStringList>;
using GameObjectListData        = TypeNodeData<CNT::gameobject_list_t, UndefinedV>;
using ComponentOutData          = TypeNodeData<CNT::component_out_data_t, UndefinedV>;


[[maybe_unused]] static QtNodes::NodeDataType generate_node_data_type(ConnectionNode::Type type){
    const auto nodeType = get_id(type);
    const auto nodeName = get_name(type);
    if(nodeType.length() > 0 || nodeName.length() > 0){
        return NodeDataType {nodeType, nodeName};
    }
    qCritical() << "Cant generate node data type from type: " << nodeType << nodeName;
    return NodeDataType();
}

[[maybe_unused]] static QString convert_node_data_to_string(std::shared_ptr<QtNodes::NodeData> nodeData){
    return std::dynamic_pointer_cast<BaseNodeData>(nodeData)->value_as_text();
}


}
