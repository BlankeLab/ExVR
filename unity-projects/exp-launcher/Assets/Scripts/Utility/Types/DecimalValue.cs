
/***********************************************************************************
** exvr-exp                                                                       **
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

namespace Ex {
    public class DecimalValue{

        enum Type{
            boolT,intT,floatT,doubleT
        }

        private object boolValue = null;
        private object intValue     = null;
        private object floatValue   = null;
        private object doubleValue  = null;
        
        public DecimalValue() {
        }
        public DecimalValue(bool value) {
            boolValue = value;
        }
        public DecimalValue(int value) {
            intValue = value;
        }
        public DecimalValue(float value) {
            floatValue = value;
        }
        public DecimalValue(double value) {
            doubleValue = value;
        }

        // has
        public bool has_bool() {
            return boolValue != null;
        }
        public bool has_int() {
            return intValue != null;
        }
        public bool has_float() {
            return floatValue != null;
        }
        public bool has_double() {
            return doubleValue != null;
        }

        public bool to_bool() {
            if (has_bool()) {
                return (bool)boolValue;
            }
            if (has_int()) {
                return Converter.to_bool((int)intValue);
            }
            if (has_float()) {
                return Converter.to_bool((float)floatValue);
            }
            if (has_double()) {
                return Converter.to_bool((double)doubleValue);
            }
            return false;
        }

        public int to_int() {
            if (has_int()) {
                return (int)intValue;
            }
            if (has_float()) {
                return Converter.to_int((float)floatValue);
            }
            if (has_double()) {
                return Converter.to_int((double)doubleValue);
            }
            if (has_bool()) {
                return Converter.to_int((bool)boolValue);
            }
            return 0;
        }

        public float to_float() {
            if (has_float()) {
                return (float)floatValue;
            }
            if (has_double()) {
                return Converter.to_float((double)doubleValue);
            }
            if (has_int()) {
                return Converter.to_float((int)intValue);
            }
            if (has_bool()) {
                return Converter.to_float((bool)boolValue);
            }
            return 0f;
        }

        public double to_double() {
            if (has_double()) {
                return (double)doubleValue;
            }
            if (has_float()) {
                return Converter.to_double((float)floatValue);
            }
            if (has_int()) {
                return Converter.to_double((int)intValue);
            }
            if (has_bool()) {
                return Converter.to_double((bool)boolValue);
            }
            return 0f;
        }

        static Type get_type(DecimalValue v1, DecimalValue v2) {
            if(v1.has_double() || v2.has_double()) {
                return Type.doubleT;
            }
            if (v1.has_float() || v2.has_float()) {
                return Type.floatT;
            }
            if (v1.has_int() || v2.has_int()) {
                return Type.intT;
            }
            return Type.boolT;
        }

        public void add(DecimalValue value) {
            switch (get_type(this, value)) {
                case Type.doubleT:
                    doubleValue = to_double() + value.to_double();
                    boolValue = null;
                    intValue = null;
                    floatValue = null;
                    break;
                case Type.floatT:
                    floatValue = to_float() + value.to_float();
                    boolValue = null;
                    intValue = null;
                    doubleValue = null;
                    break;
                case Type.intT:
                    intValue = to_int() + value.to_int();
                    boolValue = null;
                    floatValue = null;
                    doubleValue = null;
                    break;
                case Type.boolT:
                    boolValue = null;
                    intValue    = to_int() + value.to_int();
                    floatValue  = null;
                    doubleValue = null;
                    break;
            }
        }

        public void sub(DecimalValue value) {
            switch (get_type(this, value)) {
                case Type.doubleT:
                    doubleValue = to_double() - value.to_double();
                    intValue    = null;
                    floatValue  = null;
                    break;
                case Type.floatT:
                    floatValue  = to_float() - value.to_float();
                    intValue    = null;
                    doubleValue = null;
                    break;
                case Type.intT:
                    intValue    = to_int() - value.to_int();
                    floatValue  = null;
                    doubleValue = null;
                    break;
                case Type.boolT:
                    boolValue   = null;
                    intValue    = to_int() - value.to_int();
                    floatValue  = null;
                    doubleValue = null;
                    break;
            }
        }

        public void multiply(DecimalValue value) {
            switch (get_type(this, value)) {
                case Type.doubleT:
                    doubleValue = to_double() * value.to_double();
                    intValue    = null;
                    floatValue  = null;
                    break;
                case Type.floatT:
                    floatValue  = to_float() * value.to_float();
                    intValue    = null;
                    doubleValue = null;
                    break;
                case Type.intT:
                    intValue    = to_int() * value.to_int();
                    floatValue  = null;
                    doubleValue = null;
                    break;
                case Type.boolT:
                    boolValue   = null;
                    intValue    = to_int() * value.to_int();
                    floatValue  = null;
                    doubleValue = null;
                    break;
            }
        }

        public static void apply_function(DecimalValue d1, DecimalValue d2, DecimalValue outV, ConnectorsFunctions.DecimalOperator o) {

            double value1   = d1.to_double();
            double value2   = d2.to_double();
            
            var output      = ConnectorsFunctions.get(o).Invoke(value1, value2);

            if(output.GetType() == typeof(double)) { // double
                outV.doubleValue    = output;
                outV.boolValue      = null;
                outV.intValue       = null;
                outV.floatValue     = null;                
            } else { // bool
                outV.boolValue      = output;
                outV.intValue       = null;
                outV.floatValue     = null;
                outV.doubleValue    = null;
            }
        }

        public static void apply_function(DecimalValue d, DecimalValue outV, ConnectorsFunctions.TrigonometryFunction t) {

            var output = ConnectorsFunctions.get(t).Invoke(d.to_double());
            outV.doubleValue    = output;
            outV.boolValue      = null;
            outV.intValue       = null;
            outV.floatValue     = null;
        }
    }
}