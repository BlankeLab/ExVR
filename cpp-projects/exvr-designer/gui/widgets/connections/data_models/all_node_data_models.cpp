
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

#include "all_node_data_models.hpp"

// base
#include "utility/benchmark.hpp"

// nodes
#include "nodes/FlowViewStyle.hpp"
#include "nodes/ConnectionStyle.hpp"

// local
// # converters
#include "data/nodes_data_converters.hpp"
// # connectors
#include "connectors/boolean_ndm.hpp"
#include "connectors/integer_ndm.hpp"
#include "connectors/real_ndm.hpp"
#include "connectors/string_ndm.hpp"
#include "connectors/vector3_ndm.hpp"
#include "connectors/transform_ndm.hpp"
#include "connectors/string_ndm.hpp"
#include "connectors/id_any_ndm.hpp"
#include "connectors/string_any_ndm.hpp"
#include "connectors/time_ndm.hpp"
#include "connectors/flow_routine_ndm.hpp"
#include "connectors/binary_operation_ndm.hpp"
#include "connectors/string_operation_ndm.hpp"
#include "connectors/decimal_operation_ndm.hpp"
#include "connectors/check_joypad_ndm.hpp"
#include "connectors/keyboard_ndm.hpp"
#include "connectors/check_mouse_ndm.hpp"
#include "connectors/pass_value_trigger_ndm.hpp"
#include "connectors/conditional_trigger_ndm.hpp"
#include "connectors/pass_values_ndm.hpp"
#include "connectors/conditional_gate_ndm.hpp"
#include "connectors/check_id_ndm.hpp"
#include "connectors/check_str_ndm.hpp"
#include "connectors/decimal_trigo_ndm.hpp"
#include "connectors/delay_ndm.hpp"
#include "connectors/variable_delay_ndm.hpp"
#include "connectors/decimal_counter_ndm.hpp"
#include "connectors/logger_ndm.hpp"
#include "connectors/curve_x_ndm.hpp"
#include "connectors/reals_to_vector2_ndm.hpp"
#include "connectors/reals_to_vector3_ndm.hpp"
#include "connectors/vector2_to_reals_ndm.hpp"
#include "connectors/vector3_to_reals_ndm.hpp"
#include "connectors/string_list_to_id_any_ndm.hpp"
#include "connectors/transform_to_vectors_ndm.hpp"
#include "connectors/vectors_to_transform_ndm.hpp"
#include "connectors/basic_ndm.hpp"
#include "connectors/resources_ndm.hpp"
#include "connectors/from_time_any_ndm.hpp"

using namespace tool;
using namespace tool::ex;

using QtNodes::NodeStyle;
using QtNodes::FlowViewStyle;
using QtNodes::ConnectionStyle;


void DataNodeModels::initialize(){

    registry = std::make_shared<DataModelRegistry>();

    using TC = TypeConverter;
    auto r = registry;
    // ### decimal

    // TEST
//    r->registerTypeConverter(std::make_pair(BoolData().type(),IntData().type()),UniversalConverter(BoolData().type_data(), IntData().type_data()));
    r->registerTypeConverter(std::make_pair(DecimalData().type(), BoolData().type()),TC{DecimalToBoolConverter()});
    r->registerTypeConverter(std::make_pair(DecimalData().type(), IntData().type()),TC{DecimalToIntegerConverter()});
    r->registerTypeConverter(std::make_pair(DecimalData().type(), FloatData().type()),TC{DecimalToFloatConverter()});
    r->registerTypeConverter(std::make_pair(DecimalData().type(), RealData().type()),TC{DecimalToRealConverter()});
    r->registerTypeConverter(std::make_pair(DecimalData().type(), StringData().type()),TC{DecimalToStringConverter()});
    r->registerTypeConverter(std::make_pair(DecimalData().type(), AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(DecimalData().type(), VoidData().type()),TC{ToVoidConverter()});
    r->registerTypeConverter(std::make_pair(DecimalData().type(), RealListData().type()),TC{DecimalToRealListConverter()});
    // ### bool    
    r->registerTypeConverter(std::make_pair(BoolData().type(),DecimalData().type()),TC{BoolToDecimalConverter()});
    r->registerTypeConverter(std::make_pair(BoolData().type(),IntData().type()),TC{BoolToIntegerConverter()});
    r->registerTypeConverter(std::make_pair(BoolData().type(),FloatData().type()),TC{BoolToFloatConverter()});
    r->registerTypeConverter(std::make_pair(BoolData().type(),RealData().type()),TC{BoolToRealConverter()});
    r->registerTypeConverter(std::make_pair(BoolData().type(),StringData().type()),TC{BoolToStringConverter()});
    r->registerTypeConverter(std::make_pair(BoolData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(BoolData().type(),VoidData().type()),TC{ToVoidConverter()});
    // ### int
    r->registerTypeConverter(std::make_pair(IntData().type(),DecimalData().type()),TC{IntegerToDecimalConverter()});
    r->registerTypeConverter(std::make_pair(IntData().type(),BoolData().type()),TC{IntegerToBoolConverter()});
    r->registerTypeConverter(std::make_pair(IntData().type(),FloatData().type()),TC{IntegerToFloatConverter()});
    r->registerTypeConverter(std::make_pair(IntData().type(),RealData().type()),TC{IntegerToRealConverter()});
    r->registerTypeConverter(std::make_pair(IntData().type(),StringData().type()),TC{IntegerToStringConverter()});
    r->registerTypeConverter(std::make_pair(IntData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(IntData().type(),VoidData().type()),TC{ToVoidConverter()});
    r->registerTypeConverter(std::make_pair(IntData().type(),RealListData().type()),TC{IntegerToRealListConverter()});
    // ### float
    r->registerTypeConverter(std::make_pair(FloatData().type(),DecimalData().type()),TC{FloatToDecimalConverter()});
    r->registerTypeConverter(std::make_pair(FloatData().type(),BoolData().type()),TC{FloatToBoolConverter()});
    r->registerTypeConverter(std::make_pair(FloatData().type(),IntData().type()),TC{FloatToIntegerConverter()});
    r->registerTypeConverter(std::make_pair(FloatData().type(),RealData().type()),TC{FloatToRealConverter()});
    r->registerTypeConverter(std::make_pair(FloatData().type(),StringData().type()),TC{FloatToStringConverter()});
    r->registerTypeConverter(std::make_pair(FloatData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(FloatData().type(),VoidData().type()),TC{ToVoidConverter()});
    r->registerTypeConverter(std::make_pair(FloatData().type(),RealListData().type()),TC{FloatToRealListConverter()});
    // ### real
    r->registerTypeConverter(std::make_pair(RealData().type(),DecimalData().type()),TC{RealToDecimalConverter()});
    r->registerTypeConverter(std::make_pair(RealData().type(),BoolData().type()),TC{RealToBoolConverter()});
    r->registerTypeConverter(std::make_pair(RealData().type(),IntData().type()),TC{RealToIntegerConverter()});
    r->registerTypeConverter(std::make_pair(RealData().type(),FloatData().type()),TC{RealToFloatConverter()});
    r->registerTypeConverter(std::make_pair(RealData().type(),StringData().type()),TC{RealToStringConverter()});
    r->registerTypeConverter(std::make_pair(RealData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(RealData().type(),VoidData().type()),TC{ToVoidConverter()});
    r->registerTypeConverter(std::make_pair(RealData().type(),RealListData().type()),TC{RealToRealListConverter()});
    // ### string
    r->registerTypeConverter(std::make_pair(StringData().type(),DecimalData().type()),TC{StringToDecimalConverter()});
    r->registerTypeConverter(std::make_pair(StringData().type(),BoolData().type()),TC{StringToBoolConverter()});
    r->registerTypeConverter(std::make_pair(StringData().type(),IntData().type()),TC{StringToIntegerConverter()});
    r->registerTypeConverter(std::make_pair(StringData().type(),FloatData().type()),TC{StringToFloatConverter()});
    r->registerTypeConverter(std::make_pair(StringData().type(),RealData().type()),TC{StringToRealConverter()});
    r->registerTypeConverter(std::make_pair(StringData().type(),StringListData().type()),TC{StringToStringListConverter()});
    r->registerTypeConverter(std::make_pair(StringData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(StringData().type(),VoidData().type()),TC{ToVoidConverter()});
    // ### string list
    r->registerTypeConverter(std::make_pair(StringListData().type(),StringData().type()),TC{StringListToStringConverter()});
    r->registerTypeConverter(std::make_pair(StringListData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(StringListData().type(),VoidData().type()),TC{ToVoidConverter()});
    // ### double list
    r->registerTypeConverter(std::make_pair(RealListData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(RealListData().type(),VoidData().type()),TC{ToVoidConverter()});
    // ### any
    r->registerTypeConverter(std::make_pair(AnyData().type(),BoolData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),IntData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),FloatData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),RealData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),StringData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),Vector2Data().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),Vector3Data().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),TransformData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),DecimalData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),VoidData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),StringListData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),RealListData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),DecimalListData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),LeapMotionFrameData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),LeapMotionHandsFrameData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),ImageData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),KinectBodyData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),IdAnyData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),StringAnyData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),TimeAnyData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),PlotData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),KeyboardButtonEventData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),MouseButtonEventData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),JoypadButtonEventData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),JoypadAxisEventData().type()),TC{FromAnyConverter()});
    r->registerTypeConverter(std::make_pair(AnyData().type(),GameObjectListData().type()),TC{FromAnyConverter()});
    // ### void
    r->registerTypeConverter(std::make_pair(VoidData().type(),AnyData().type()),TC{ToAnyConverter()});
    // ### leap motion frame
    r->registerTypeConverter(std::make_pair(LeapMotionFrameData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(LeapMotionFrameData().type(),VoidData().type()),TC{ToVoidConverter()});
    // ### keyboard button state
    r->registerTypeConverter(std::make_pair(KeyboardButtonEventData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(KeyboardButtonEventData().type(),VoidData().type()),TC{ToVoidConverter()});
    r->registerTypeConverter(std::make_pair(KeyboardButtonEventData().type(),StringData().type()),TC{KeyboardButtonEventToStringConverter()});
    // ### mouse button state
    r->registerTypeConverter(std::make_pair(MouseButtonEventData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(MouseButtonEventData().type(),VoidData().type()),TC{ToVoidConverter()});
    // ### joypad button state
    r->registerTypeConverter(std::make_pair(JoypadButtonEventData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(JoypadButtonEventData().type(),VoidData().type()),TC{ToVoidConverter()});
    // ### joypad axis state
    r->registerTypeConverter(std::make_pair(JoypadAxisEventData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(JoypadAxisEventData().type(),VoidData().type()),TC{ToVoidConverter()});
    // ### leap motion hands frame
    r->registerTypeConverter(std::make_pair(LeapMotionHandsFrameData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(LeapMotionHandsFrameData().type(),VoidData().type()),TC{ToVoidConverter()});
    // ### kinect body
    r->registerTypeConverter(std::make_pair(KinectBodyData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(KinectBodyData().type(),VoidData().type()),TC{ToVoidConverter()});
    // ### image
    r->registerTypeConverter(std::make_pair(ImageData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(ImageData().type(),VoidData().type()),TC{ToVoidConverter()});
    // ### plot
    r->registerTypeConverter(std::make_pair(PlotData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(PlotData().type(),VoidData().type()),TC{ToVoidConverter()});
    // ### vector2
    r->registerTypeConverter(std::make_pair(Vector2Data().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(Vector2Data().type(),VoidData().type()),TC{ToVoidConverter()});
    r->registerTypeConverter(std::make_pair(Vector2Data().type(),StringData().type()),TC{Vector2ToStringConverter()});
    // ### vector3
    r->registerTypeConverter(std::make_pair(Vector3Data().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(Vector3Data().type(),VoidData().type()),TC{ToVoidConverter()});
    r->registerTypeConverter(std::make_pair(Vector3Data().type(),StringData().type()),TC{Vector3ToStringConverter()});
    // ### transform
    r->registerTypeConverter(std::make_pair(TransformData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(TransformData().type(),VoidData().type()),TC{ToVoidConverter()});
    r->registerTypeConverter(std::make_pair(TransformData().type(),StringData().type()),TC{TransformToStringConverter()});
    // ### id any
    r->registerTypeConverter(std::make_pair(IdAnyData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(IdAnyData().type(),VoidData().type()),TC{ToVoidConverter()});
    // ### string any
    r->registerTypeConverter(std::make_pair(StringAnyData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(StringAnyData().type(),VoidData().type()),TC{ToVoidConverter()});
    // ### gameobject list
    r->registerTypeConverter(std::make_pair(GameObjectListData().type(),AnyData().type()),TC{ToAnyConverter()});
    r->registerTypeConverter(std::make_pair(GameObjectListData().type(),VoidData().type()),TC{ToVoidConverter()});

//    // ### component out data
//    r->registerTypeConverter(std::make_pair(ComponentOutData().type(),IntData().type()),TC{FromComponentConverter()});

    FlowViewStyleStr =
    R"({
        "FlowViewStyle": {
            "BackgroundColor": [255, 255, 240],
            "FineGridColor": [245, 245, 230],
            "CoarseGridColor": [235, 235, 220]
        }
    }
    )";

    ConnectionStyleStr =
    R"({
        "ConnectionStyle": {
            "ConstructionColor": "gray",
            "NormalColor": "black",
            "SelectedColor": "gray",
            "SelectedHaloColor": "deepskyblue",
            "HoveredColor": "deepskyblue",

            "LineWidth": 3.0,
            "ConstructionLineWidth": 2.0,
            "PointDiameter": 10.0,

            "UseDataDefinedColors": true
        }
    }
    )";
    NodeStyleStr =
    R"({
        "NodeStyle": {
            "NormalBoundaryColor": "darkgray",
            "SelectedBoundaryColor": "deepskyblue",
            "GradientColor0": "mintcream",
            "GradientColor1": "mintcream",
            "GradientColor2": "mintcream",
            "GradientColor3": "mintcream",
            "WarningColor": [255, 201, 14],
            "ErrorColor": [200, 0, 0],
            "ShadowColor": [200, 200, 200],
            "FontColor": [10, 10, 10],
            "FontColorFaded": [100, 100, 100],
            "ConnectionPointColor": "white",
            "PenWidth": 2.0,
            "HoveredPenWidth": 2.5,
            "ConnectionPointDiameter": 10.0,
            "Opacity": 1.0
        }
    }
    )";

    // init styles
    FlowViewStyle::setStyle(FlowViewStyleStr);
    ConnectionStyle::setConnectionStyle(ConnectionStyleStr);
    NodeStyle::setNodeStyle(NodeStyleStr);

    generatorStyle.NormalBoundaryColor = Qt::darkGray;
    generatorStyle.SelectedBoundaryColor= Qt::blue;
    generatorStyle.GradientColor0  = Qt::white;
    generatorStyle.GradientColor1  = Qt::white;
    generatorStyle.GradientColor2  = Qt::white;
    generatorStyle.GradientColor3  = Qt::white;
    generatorStyle.WarningColor    = QColor(255, 201, 14);
    generatorStyle.ErrorColor      = QColor(200, 0, 0);
    generatorStyle.ShadowColor     = QColor(200, 200, 200);
    generatorStyle.FontColor       = QColor(10, 10, 10);
    generatorStyle.FontColorFaded  = QColor(100, 100, 100);
    generatorStyle.ConnectionPointColor  = Qt::white;
    generatorStyle.PenWidth  = 2.0;
    generatorStyle.HoveredPenWidth  = 2.5;
    generatorStyle.ConnectionPointDiameter  =  10.0;
    generatorStyle.Opacity  = 1.0;

    componentStyle = generatorStyle;
    const auto compColor = QColor(213,240,247);
    componentStyle.GradientColor0  = compColor;
    componentStyle.GradientColor1  = compColor;
    componentStyle.GradientColor2  = compColor;
    componentStyle.GradientColor3  = compColor;
    componentStyle.FontColorFaded  = Qt::black;

    functionStyle = generatorStyle;
    const auto funColor = QColor(232,253,196);
    functionStyle.GradientColor0  = funColor;
    functionStyle.GradientColor1  = funColor;
    functionStyle.GradientColor2  = funColor;
    functionStyle.GradientColor3  = funColor;
    functionStyle.FontColorFaded  = Qt::black;

    convertorStyle = generatorStyle;
    const auto convColor = QColor(255,228,202);
    convertorStyle.GradientColor0  = convColor;
    convertorStyle.GradientColor1  = convColor;
    convertorStyle.GradientColor2  = convColor;
    convertorStyle.GradientColor3  = convColor;
    convertorStyle.FontColorFaded  = Qt::black;

    operatorStyle = generatorStyle;
    const auto opeColor = QColor(255,223,239);
    operatorStyle.GradientColor0  = opeColor;
    operatorStyle.GradientColor1  = opeColor;
    operatorStyle.GradientColor2  = opeColor;
    operatorStyle.GradientColor3  = opeColor;
    operatorStyle.FontColorFaded  = Qt::black;

    actionStyle = generatorStyle;
    const auto actColor = QColor(224,193,255);
    actionStyle.GradientColor0  = actColor;
    actionStyle.GradientColor1  = actColor;
    actionStyle.GradientColor2  = actColor;
    actionStyle.GradientColor3  = actColor;
    actionStyle.FontColorFaded  = Qt::black;

    displayStyle = generatorStyle;
    const auto disColor = QColor(255, 247, 209);//QColor(206,206,255);
    displayStyle.GradientColor0  = disColor;
    displayStyle.GradientColor1  = disColor;
    displayStyle.GradientColor2  = disColor;
    displayStyle.GradientColor3  = disColor;
    displayStyle.FontColorFaded  = Qt::black;

    eventStyle = generatorStyle;
    const auto evColor = QColor(228,228,201);
    eventStyle.GradientColor0  = evColor;
    eventStyle.GradientColor1  = evColor;
    eventStyle.GradientColor2  = evColor;
    eventStyle.GradientColor3  = evColor;
    eventStyle.FontColorFaded  = Qt::black;

    flowStyle = generatorStyle;
    const auto flowColor = QColor(214,171,171);
    flowStyle.GradientColor0  = flowColor;
    flowStyle.GradientColor1  = flowColor;
    flowStyle.GradientColor2  = flowColor;
    flowStyle.GradientColor3  = flowColor;
    flowStyle.FontColorFaded  = Qt::black;

    linkStyle = generatorStyle;
    const auto linkColor = QColor(155,230,215);
    linkStyle.GradientColor0  = linkColor;
    linkStyle.GradientColor1  = linkColor;
    linkStyle.GradientColor2  = linkColor;
    linkStyle.GradientColor3  = linkColor;
    linkStyle.FontColorFaded  = Qt::black;

    resourceStyle = generatorStyle;
    const auto resourceColor = QColor(200,180,100);
    resourceStyle.GradientColor0  = resourceColor;
    resourceStyle.GradientColor1  = resourceColor;
    resourceStyle.GradientColor2  = resourceColor;
    resourceStyle.GradientColor3  = resourceColor;
    resourceStyle.FontColorFaded  = Qt::black;
}

std::unique_ptr<ConnectorNodeDataModel> DataNodeModels::generate_connector_data_model(Connector *connector){

    std::unique_ptr<ConnectorNodeDataModel> connectorDataModel = nullptr;
    using T = Connector::Type;
    switch (connector->type) {
    case T::Vectors_to_transform:
        connectorDataModel = std::make_unique<VectorsToTransformNodeDataModel>();
        break;
    case T::Transform_to_vectors:
        connectorDataModel = std::make_unique<TransformToVectorsNodeDataModel>();
        break;
    case T::Check_str:
        connectorDataModel = std::make_unique<CheckStrNodeDataModel>();
        break;
    case T::Transform:
        connectorDataModel = std::make_unique<TransformNodeDataModel>();
        break;
    case T::Check_id:
        connectorDataModel = std::make_unique<CheckIdNodeDataModel>();
        break;
    case T::From_time_any:
        connectorDataModel = std::make_unique<FromTimeAnyNodeDataModel>();
        break;
    case T::String_list_to_id_any:
        connectorDataModel = std::make_unique<StringListToIdAnyNodeDataModel>();
        break;
    case T::Pass_values:
        connectorDataModel = std::make_unique<PassValuesNodeDataModel>();
        break;
    case T::Id_any:
        connectorDataModel = std::make_unique<IdAnyNodeDataModel>();
        break;
    case T::String_any:
        connectorDataModel = std::make_unique<StringAnyNodeDataModel>();
        break;
    case T::Real:
        connectorDataModel = std::make_unique<RealNodeDataModel>();
        break;
    case T::Time:
        connectorDataModel = std::make_unique<TimeNodeDataModel>();
        break;
    case T::Boolean:
        connectorDataModel = std::make_unique<BooleanNodeDataModel>();
        break;
    case T::Integer:
        connectorDataModel = std::make_unique<IntegerNodeDataModel>();
        break;
    case T::String:
        connectorDataModel = std::make_unique<StringNodeDataModel>();
        break;
    case T::Vector3:
        connectorDataModel = std::make_unique<Vector3NodeDataModel>();
        break;
    case T::Random_real:
        connectorDataModel = std::make_unique<RandomRealNodeDataModel>();
        break;
    case T::Reals_to_vec2:
        connectorDataModel = std::make_unique<RealsToVector2NodeDataModel>();
        break;
    case T::Reals_to_vec3:
        connectorDataModel = std::make_unique<RealsToVector3NodeDataModel>();
        break;
    case T::Vec2_to_reals:
        connectorDataModel = std::make_unique<Vector2ToRealsNodeDataModel>();
        break;
    case T::Vec3_to_reals:
        connectorDataModel = std::make_unique<Vector3ToRealsNodeDataModel>();
        break;
    case T::Decimal_trigonometry:
        connectorDataModel = std::make_unique<DecimalTrigoNodeDataModel>();
        break;
    case T::Delay:
        connectorDataModel = std::make_unique<DelayNodeDataModel>();
        break;
    case T::Variable_delay:
        connectorDataModel = std::make_unique<VariableDelayNodeDataModel>();
        break;
    case T::Binary_operation:
        connectorDataModel = std::make_unique<BinaryOperationNodeDataModel>();
        break;
    case T::Decimal_operation:
        connectorDataModel = std::make_unique<DecimalOperationNodeDataModel>();
        break;
    case T::String_operation:
        connectorDataModel = std::make_unique<StringOperationNodeDataModel>();
        break;
    case T::Pass_value_trigger:
        connectorDataModel = std::make_unique<PassValueTriggerNodeDataModel>();
        break;
    case T::Conditional_trigger:
        connectorDataModel = std::make_unique<ConditionalTriggerNodeDataModel>();
        break;
    case T::Next:
        connectorDataModel = std::make_unique<NextNodeDataModel>();
        break;
    case T::Previous:
        connectorDataModel = std::make_unique<PreviousNodeDataModel>();
        break;
    case T::Stop:
        connectorDataModel = std::make_unique<StopNodeDataModel>();
        break;
    case T::Pause:
        connectorDataModel = std::make_unique<PauseNodeDataModel>();
        break;
    case T::Next_with_name:
        connectorDataModel = std::make_unique<NextWithNameNodeDataModel>();
        break;
    case T::Previous_with_name:
        connectorDataModel = std::make_unique<PreviousWithNameNodeDataModel>();
        break;
    case T::Next_with_cond:
        connectorDataModel = std::make_unique<NextWithCondNodeDataModel>();
        break;
    case T::Previous_with_cond:
        connectorDataModel = std::make_unique<PreviousWithCondNodeDataModel>();
        break;
    case T::Filter_keyboard_button:
        connectorDataModel = std::make_unique<FilterKeyboardButtonNodeDataModel>();
        break;
    case T::Check_joypad_button:
        connectorDataModel = std::make_unique<CheckJoypadButtonNodeDataModel>();
        break;
    case T::Check_joypad_axis:
        connectorDataModel = std::make_unique<CheckJoypadAxisNodeDataModel>();
        break;
    case T::Check_keyboard_button:
        connectorDataModel = std::make_unique<CheckKeyboardButtonNodeDataModel>();
        break;
    case T::Check_mouse_button:
        connectorDataModel = std::make_unique<CheckMouseButtonNodeDataModel>();
        break;
    case T::Start_routine:
        connectorDataModel = std::make_unique<StartRoutineNodeDataModel>();
        break;
    case T::Stop_routine:
        connectorDataModel = std::make_unique<StopRoutineNodeDataModel>();
        break;
    case T::Pre_update_routine:
        connectorDataModel = std::make_unique<PreUpdateRoutineNodeDataModel>();
        break;
    case T::Update_routine:
        connectorDataModel = std::make_unique<UpdateRoutineNodeDataModel>();
        break;
    case T::Post_update_routine:
        connectorDataModel = std::make_unique<PostUpdateRoutineNodeDataModel>();
        break;
    case T::Routine_condition:
        connectorDataModel = std::make_unique<RoutineConditionNodeDataModel>();
        break;
    case T::Force_component_config:
        connectorDataModel = std::make_unique<ForceComponentConfigNodeDataModel>();
        break;
    case T::Decimal_counter:
        connectorDataModel = std::make_unique<DecimalCounterConfigNodeDataModel>();
        break;
    case T::Conditional_gate:
        connectorDataModel = std::make_unique<ConditionalGateNodeDataModel>();
        break;
    // # Display
    case T::Logger:
        connectorDataModel = std::make_unique<LoggerNodeDataModel>();
        break;
    case T::Curve_x:
        connectorDataModel = std::make_unique<CurveXNodeDataModel>();
        break;
    case T::Post_it:
        connectorDataModel = std::make_unique<PostItNodeDataModel>();
        break;
    // # Resource
    case T::Image_resource:
        connectorDataModel = std::make_unique<ImageResourceNodeDataModel>();
        break;
    case T::Text_resource:
        connectorDataModel = std::make_unique<TextResourceNodeDataModel>();
        break;
    // # Component
    case T::Component:
        // error
        return nullptr;
    case T::SizeEnum:
        // error
        return nullptr;
    }

    // retrieve style
    QtNodes::NodeStyle style;
    switch(Connector::get_category(connector->type)){
        case Connector::Category::Function:
            style = functionStyle;
        break;
        case Connector::Category::Operator:
            style = operatorStyle;
        break;
        case Connector::Category::Convertor:
            style = convertorStyle;
        break;
        case Connector::Category::Generator:
            style = generatorStyle;
        break;
        case Connector::Category::Action:
            style = actionStyle;
        break;
        case Connector::Category::Display:
            style = displayStyle;
        break;
        case Connector::Category::Event:
            style = eventStyle;
        break;
        case Connector::Category::Flow:
            style = flowStyle;
        break;
        case Connector::Category::Link:
            style = linkStyle;
            break;
        case Connector::Category::Resource:
            style = resourceStyle;
            break;
        default:
        break;
    }

    // initialize connector node
    connectorDataModel->initialize(style, ConnectorKey{connector->key()});

    // update
    if(connector->arg.value().isEmpty()){
        connector->arg = connectorDataModel->convert_to_arg();
    }else{
        connectorDataModel->update_from_connector(*connector);
    }

//    connectorDataModel->compute(); // force one call to compute

    return connectorDataModel;
}

std::unique_ptr<ComponentNodeDataModel> DataNodeModels::generate_component_data_model(Component *component){
    auto componentDataModel = std::make_unique<ComponentNodeDataModel>(component->key(), component->type);
    componentDataModel->set_node_style(componentStyle);
    componentDataModel->update_name(component->name());
    return componentDataModel;
}




