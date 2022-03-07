
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

#include "nodes_data_converters.hpp"

// base
#include "utility/math.hpp"

using namespace tool::str;
using namespace tool::ex;

bool is_from_component(DataSP data){
    if (auto inputData = std::dynamic_pointer_cast<ComponentOutData>(data); inputData){
        return true;
    }
    return false;
}

void propagate_runtime(DataSP input, DataSP output){
    if(std::dynamic_pointer_cast<BaseNodeData>(input)->is_runtime()){
        std::dynamic_pointer_cast<BaseNodeData>(output)->set_runtime();
    }
}

DataSP BoolToIntegerConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<IntData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData = std::dynamic_pointer_cast<BoolData>(data); inputData){
        m_value = std::make_shared<IntData>(inputData->value() ? 1 : 0);        
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP BoolToFloatConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<FloatData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData = std::dynamic_pointer_cast<BoolData>(data); inputData){
        m_value = std::make_shared<FloatData>(inputData->value()? 1.f : 0.f);
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP BoolToRealConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<RealData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData = std::dynamic_pointer_cast<BoolData>(data); inputData){
        m_value = std::make_shared<RealData>(inputData->value()? 1. : 0.);
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP BoolToDecimalConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<DecimalData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData = std::dynamic_pointer_cast<BoolData>(data); inputData){
        m_value = std::make_shared<DecimalData>(Decimal{inputData->value()});
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}


DataSP IntegerToBoolConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<BoolData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData = std::dynamic_pointer_cast<IntData>(data); inputData){
        m_value = std::make_shared<BoolData>(inputData->value() != 0);
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

std::shared_ptr<NodeData> IntegerToFloatConverter::operator()(std::shared_ptr<NodeData> data){

    if(is_from_component(data)){
        auto v = std::make_shared<FloatData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData = std::dynamic_pointer_cast<IntData>(data); inputData){
        m_value = std::make_shared<FloatData>(static_cast<float>(inputData->value()));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP IntegerToRealConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<RealData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData = std::dynamic_pointer_cast<IntData>(data); inputData){
        m_value = std::make_shared<RealData>(static_cast<double>(inputData->value()));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}


DataSP FloatToBoolConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<BoolData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData = std::dynamic_pointer_cast<FloatData>(data); inputData){
        m_value = std::make_shared<BoolData>(!almost_equal<float>(inputData->value(), 0.f));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP FloatToIntegerConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<IntData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData = std::dynamic_pointer_cast<FloatData>(data); inputData){
        m_value = std::make_shared<IntData>(static_cast<int>(inputData->value()));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP FloatToRealConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<RealData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<FloatData>(data)){
        m_value = std::make_shared<RealData>(static_cast<double>(inputData->value()));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP DecimalToBoolConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<BoolData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<DecimalData>(data)){
        m_value = std::make_shared<BoolData>(inputData->value().to_int() != 0);
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP DecimalToIntegerConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<IntData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<DecimalData>(data)){
        m_value = std::make_shared<IntData>(inputData->value().to_int());
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP DecimalToFloatConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<FloatData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<DecimalData>(data)){
        m_value = std::make_shared<FloatData>(inputData->value().to_float());
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP DecimalToRealConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<RealData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<DecimalData>(data)){
        m_value = std::make_shared<RealData>(inputData->value().to_double());
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP IntegerToDecimalConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<DecimalData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<IntData>(data)){
        m_value = std::make_shared<DecimalData>(Decimal{inputData->value()});
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP FloatToDecimalConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<DecimalData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<FloatData>(data)){
        m_value = std::make_shared<DecimalData>(Decimal{inputData->value()});
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}


DataSP RealToBoolConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<BoolData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<RealData>(data)){
        m_value = std::make_shared<BoolData>(!almost_equal<double>(inputData->value(), 0.));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}


DataSP RealToIntegerConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<IntData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<RealData>(data)){
        m_value = std::make_shared<IntData>(static_cast<int>(inputData->value()));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP RealToFloatConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<FloatData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<RealData>(data)){
        m_value = std::make_shared<FloatData>(static_cast<float>(inputData->value()));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP RealToDecimalConverter::operator()(DataSP data){    

    if(is_from_component(data)){
        auto v = std::make_shared<DecimalData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<RealData>(data)){
        m_value = std::make_shared<DecimalData>(Decimal{inputData->value()});
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}


DataSP ToAnyConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<AnyData>(std::make_shared<IntData>(0));
        v->set_runtime();
        return m_value = v;;
    }

    if(data){
        m_value = std::make_shared<AnyData>(data);
        propagate_runtime(data, m_value);
    }else{
        m_value.reset();
    }

    return m_value;
}

DataSP FromAnyConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<AnyData>(std::make_shared<IntData>(0));
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData = std::dynamic_pointer_cast<AnyData>(data)){
        m_value = inputData->value();
        propagate_runtime(data, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP BoolToStringConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<StringData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<BoolData>(data)){
        m_value = std::make_shared<StringData>(inputData->value_as_text());
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP IntegerToStringConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<StringData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<IntData>(data)){
        m_value = std::make_shared<StringData>(inputData->value_as_text());
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP FloatToStringConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<StringData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<FloatData>(data)){
        m_value = std::make_shared<StringData>(inputData->value_as_text());
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP RealToStringConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<StringData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<RealData>(data)){
        m_value = std::make_shared<StringData>(inputData->value_as_text());
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP DecimalToStringConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<StringData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<DecimalData>(data)){
        m_value = std::make_shared<StringData>(inputData->value_as_text());
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}


DataSP Vector2ToStringConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<StringData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<Vector2Data>(data)){
        m_value = std::make_shared<StringData>(inputData->value_as_text());
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;

}

DataSP Vector3ToStringConverter::operator()(DataSP data){
    if(is_from_component(data)){
        auto v = std::make_shared<StringData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<Vector3Data>(data)){
        m_value = std::make_shared<StringData>(inputData->value_as_text());
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;

}

DataSP TransformToStringConverter::operator()(DataSP data){
    if(is_from_component(data)){
        auto v = std::make_shared<StringData>();
        v->set_runtime();
        return m_value = v;;
    }

    if (auto inputData =std::dynamic_pointer_cast<TransformData>(data)){
        m_value = std::make_shared<StringData>(inputData->value_as_text());
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}


DataSP StringToBoolConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<BoolData>();
        v->set_runtime();
        return m_value = v;;
    }

    if(auto inputData =std::dynamic_pointer_cast<StringData>(data)){
        m_value = std::make_shared<BoolData>(Convertor::to_bool(inputData->value()));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP StringToIntegerConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<IntData>();
        v->set_runtime();
        return m_value = v;;
    }

    if(auto inputData = std::dynamic_pointer_cast<StringData>(data)){
        m_value = std::make_shared<IntData>(Convertor::to_int(inputData->value()));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP StringToFloatConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<FloatData>();
        v->set_runtime();
        return m_value = v;;
    }

    if(auto inputData =std::dynamic_pointer_cast<StringData>(data)){
        m_value = std::make_shared<FloatData>(Convertor::to_float(inputData->value()));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP StringToRealConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<RealData>();
        v->set_runtime();
        return m_value = v;;
    }

    if(auto inputData =std::dynamic_pointer_cast<StringData>(data)){
        m_value = std::make_shared<RealData>(Convertor::to_double(inputData->value()));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP StringToDecimalConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<DecimalData>();
        v->set_runtime();
        return m_value = v;;
    }

    if(auto inputData =std::dynamic_pointer_cast<StringData>(data)){
        bool ok;
        auto value = inputData->value().toDouble(&ok);
        m_value = std::make_shared<DecimalData>(Decimal{ok ? value : 0.});
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP StringToStringListConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<StringListData>();
        v->set_runtime();
        return m_value = v;;
    }

    if(auto inputData =std::dynamic_pointer_cast<StringData>(data)){
        m_value = std::make_shared<StringListData>(QStringList() << inputData->value());
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP StringListToStringConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<StringData>();
        v->set_runtime();
        return m_value = v;;
    }

    if(auto inputData =std::dynamic_pointer_cast<StringListData>(data)){
        m_value = std::make_shared<StringData>(Convertor::to_str(inputData->value(), " "));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}


DataSP KeyboardButtonEventToStringConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<StringData>();
        v->set_runtime();
        return m_value = v;;
    }

    if(auto inputData =std::dynamic_pointer_cast<KeyboardButtonEventData>(data)){
        m_value = std::make_shared<StringData>(Convertor::to_str(inputData->value().code));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}


DataSP RealToRealListConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<RealListData>();
        v->set_runtime();
        return m_value = v;;
    }

    if(auto inputData =std::dynamic_pointer_cast<RealData>(data)){
        m_value = std::make_shared<RealListData>(Convertor::to_double_list(inputData->value()));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP FloatToRealListConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<RealListData>();
        v->set_runtime();
        return m_value = v;;
    }

    if(auto inputData =std::dynamic_pointer_cast<FloatData>(data)){
        m_value = std::make_shared<RealListData>(Convertor::to_double_list(Convertor::to_double(inputData->value())));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP IntegerToRealListConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<RealListData>();
        v->set_runtime();
        return m_value = v;;
    }

    if(auto inputData =std::dynamic_pointer_cast<IntData>(data)){
        m_value = std::make_shared<RealListData>(Convertor::to_double_list(Convertor::to_double(inputData->value())));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

DataSP DecimalToRealListConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<RealListData>();
        v->set_runtime();
        return m_value = v;;
    }

    if(auto inputData =std::dynamic_pointer_cast<DecimalData>(data)){
        m_value = std::make_shared<RealListData>(Convertor::to_double_list(inputData->value().to_double()));
        propagate_runtime(inputData, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}




DataSP ToVoidConverter::operator()(DataSP data){

    if(is_from_component(data)){
        auto v = std::make_shared<VoidData>();
        v->set_runtime();
        return m_value = v;;
    }

    if(data){
        m_value = std::make_shared<VoidData>();
        propagate_runtime(data, m_value);
    }else{
        m_value.reset();
    }
    return m_value;
}

//DataSP AnyToBoolConverter::operator()(DataSP data){

//    if(is_from_component(data)){
//        auto v = std::make_shared<BoolData>();
//        v->set_runtime();
//        return m_value = v;
//    }

//    if(auto inputData = std::dynamic_pointer_cast<AnyData>(data)){

//        auto node = std::dynamic_pointer_cast<BaseNodeData>(inputData->value());
//        switch(node->type_data()){
//            case ConnectionNode::Type::boolean_t:
//                m_value = node;
//                break;
//            case ConnectionNode::Type::integer_t:
//                m_value = std::make_shared<BoolData>(std::dynamic_pointer_cast<IntData>(node)->value() != 0);
//                break;
//        }

//        propagate_runtime(inputData, m_value);

//    }else{
//        m_value.reset();
//    }
//    return m_value;
//}

DataSP UniversalConverter::operator()(DataSP data){

    if(m_in == m_out){
        return data;
    }

    using T = ConnectionNode::Type;
    switch(m_in){
        case T::string_t:

            break;
        case T::boolean_t:

            break;
        case T::integer_t:

            break;
        case T::float_t:

            break;
        case T::real_t:

            break;
        case T::decimal_t:

            break;
        case T::vector2_t:

            break;
        case T::vector3_t:

            break;
        case T::transform_t:

            break;
        case T::string_list_t:

            break;
        case T::decimal_list_t:

            break;
        case T::real_list_t:

            break;
        case T::gameobject_list_t:

            break;
        case T::any_t:

            break;
        case T::void_t:

            break;
        case T::id_any_t:

            break;
        case T::string_any_t:

            break;
        case T::lm_frame_t:

            break;
        case T::lm_hands_frame_t:

            break;
        case T::image_t:

            break;
        case T::plot_t:

            break;
        case T::variant_t:

            break;
        case T::kinect_body_t:

            break;
        case T::component_out_data_t:

            break;
        case T::keyboard_button_event_t:

            break;
        case T::joypad_button_event_t:

            break;
        case T::joypad_axis_event_t:

            break;
        case T::mouse_button_event_t:

            break;
        case T::SizeEnum:
            break;
    }


    return nullptr;
}



