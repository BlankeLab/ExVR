
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

// nodes
#include "nodes/NodeDataModel.hpp"
#include "nodes/DataModelRegistry.hpp"
#include "nodes/TypeConverter.hpp"

using QtNodes::NodeData;
using QtNodes::DataModelRegistry;
using QtNodes::TypeConverter;

// local
#include "nodes_data.hpp"

namespace tool::ex {

using DataSP = std::shared_ptr<QtNodes::NodeData>;

struct BaseConverter{
    virtual ~BaseConverter(){}
    virtual DataSP operator()(DataSP data) = 0;
protected:
    DataSP m_value;
};

// void
struct ToVoidConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
// any
struct ToAnyConverter : public BaseConverter{DataSP operator()(DataSP data) override;};



//template <typename tIN, typename tOUT>
struct UniversalConverter : public BaseConverter{

    UniversalConverter(ConnectionNode::Type in, ConnectionNode::Type out) : m_in(in), m_out(out){

    }

    ConnectionNode::Type m_in;
    ConnectionNode::Type m_out;


    DataSP operator()(DataSP data) override;
};



struct FromAnyConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
// any
//struct AnyToBoolConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
//struct AnyToDecimalConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
//struct AnyToIntegerConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
//struct AnyToFloatConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
//struct AnyToRealConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
//struct AnyToStringConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
//struct AnyToRealListConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
//struct AnyToStringListConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
// decimal
struct DecimalToBoolConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct DecimalToIntegerConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct DecimalToFloatConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct DecimalToRealConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct DecimalToStringConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct DecimalToRealListConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
// bool
struct BoolToIntegerConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct BoolToFloatConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct BoolToRealConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct BoolToDecimalConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct BoolToStringConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
// integer
struct IntegerToBoolConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct IntegerToFloatConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct IntegerToRealConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct IntegerToDecimalConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct IntegerToStringConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct IntegerToRealListConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
// float
struct FloatToBoolConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct FloatToIntegerConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct FloatToRealConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct FloatToDecimalConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct FloatToStringConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct FloatToRealListConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
// real
struct RealToBoolConverter  : public BaseConverter{DataSP operator()(DataSP data) override;};
struct RealToIntegerConverter  : public BaseConverter{DataSP operator()(DataSP data) override;};
struct RealToFloatConverter  : public BaseConverter{DataSP operator()(DataSP data) override;};
struct RealToDecimalConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct RealToStringConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct RealToRealListConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
// string
struct StringToBoolConverter  : public BaseConverter{DataSP operator()(DataSP data) override;};
struct StringToIntegerConverter  : public BaseConverter{DataSP operator()(DataSP data) override;};
struct StringToFloatConverter  : public BaseConverter{DataSP operator()(DataSP data) override;};
struct StringToRealConverter  : public BaseConverter{DataSP operator()(DataSP data) override;};
struct StringToDecimalConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
struct StringToStringListConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
// vector2
struct Vector2ToStringConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
// vector3
struct Vector3ToStringConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
// transform
struct TransformToStringConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
// string list
struct StringListToStringConverter : public BaseConverter{DataSP operator()(DataSP data) override;};
// keyboard button event
struct KeyboardButtonEventToStringConverter : public BaseConverter{DataSP operator()(DataSP data) override;};


// real list
// ...
// id any
// ...


}
