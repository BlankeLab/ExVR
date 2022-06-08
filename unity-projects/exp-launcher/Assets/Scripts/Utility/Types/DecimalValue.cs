
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
            boolT,longT,doubleT
        }

        private object boolValue    = null;
        private object longValue    = null;
        private object doubleValue  = null;
        
        public DecimalValue() {
        }
        public DecimalValue(bool value) {
            boolValue = value;
        }
        public DecimalValue(byte value) {
            longValue = Converter.to_long(value);
        }
        public DecimalValue(char value) {
            longValue = Converter.to_long(value);
        }
        public DecimalValue(short value) {
            longValue = Converter.to_long(value);
        }
        public DecimalValue(int value) {
            longValue = Converter.to_long(value);
        }
        public DecimalValue(float value) {
            doubleValue = Converter.to_double(value);
        }
        public DecimalValue(double value) {
            doubleValue = value;
        }

        // has
        public bool has_bool_value() {
            return boolValue != null;
        }
        public bool has_whole_value() {
            return longValue != null;
        }
        public bool has_real_value() {
            return doubleValue != null;
        }

        public bool to_bool() {
            if (has_bool_value()) {
                return (bool)boolValue;
            }
            if (has_whole_value()) {
                return Converter.to_bool((long)longValue);
            }
            if (has_real_value()) {
                return Converter.to_bool((double)doubleValue);
            }
            return false;
        }

        public int to_int() {
            if (has_whole_value()) {
                return Converter.to_int((long)longValue);
            }
            if (has_real_value()) {
                return Converter.to_int((double)doubleValue);
            }
            if (has_bool_value()) {
                return Converter.to_int((bool)boolValue);
            }
            return 0;
        }

        public long to_long() {            
            if (has_whole_value()) {
                return (long)longValue;
            }
            if (has_real_value()) {
                return Converter.to_long((double)doubleValue);
            }
            if (has_bool_value()) {
                return Converter.to_long((bool)boolValue);
            }
            return 0;
        }

        public float to_float() {
            if (has_real_value()) {
                return Converter.to_float((double)doubleValue);
            }
            if (has_whole_value()) {
                return Converter.to_float((long)longValue);
            }
            if (has_bool_value()) {
                return Converter.to_float((bool)boolValue);
            }
            return 0f;
        }

        public double to_double() {
            if (has_real_value()) {
                return (double)doubleValue;
            }
            if (has_whole_value()) {
                return Converter.to_double((long)longValue);
            }
            if (has_bool_value()) {
                return Converter.to_double((bool)boolValue);
            }
            return 0f;
        }

        public string to_string(string format = Converter.g7) {
            if (has_bool_value()) {
                return Converter.to_string((bool)boolValue);
            }
            if (has_whole_value()) {
                return Converter.to_string((long)longValue);
            }
            if (has_real_value()) {
                return Converter.to_string((double)doubleValue, format);
            }
            return "";
        }

        static Type get_type(DecimalValue v1, DecimalValue v2) {

            if(v1.has_real_value() || v2.has_real_value()) {
                return Type.doubleT;
            }
            if (v1.has_whole_value() || v2.has_whole_value()) {
                return Type.longT;
            }
            return Type.boolT;
        }

        public void add(DecimalValue value) {
            switch (get_type(this, value)) {
                case Type.doubleT:                    
                    boolValue   = null;                    
                    longValue   = null;
                    doubleValue = to_double() + value.to_double();
                    break;
                case Type.longT:                    
                    boolValue   = null;
                    longValue   = to_long() + value.to_long();
                    doubleValue = null;
                    break;
                case Type.boolT:
                    boolValue   = null;
                    longValue   = to_long() + value.to_long();
                    doubleValue = null;
                    break;
            }
        }

        public void sub(DecimalValue value) {
            switch (get_type(this, value)) {
                case Type.doubleT:
                    boolValue   = null;
                    longValue   = null;
                    doubleValue = to_double() - value.to_double();                    
                    break;
                case Type.longT:
                    boolValue   = null;                        
                    longValue   = to_long() - value.to_long();
                    doubleValue = null;
                    break;
                case Type.boolT:
                    boolValue   = null;                    
                    longValue   = to_long() - value.to_long(); ;
                    doubleValue = null;
                    break;
            }
        }

        public void multiply(DecimalValue value) {
            switch (get_type(this, value)) {
                case Type.doubleT:
                    boolValue   = null;
                    longValue   = null;
                    doubleValue = to_double() * value.to_double();
                    break;
                case Type.longT:
                    boolValue   = null;
                    longValue   = to_long() * value.to_long();
                    doubleValue = null;                    
                    break;
                case Type.boolT:
                    boolValue   = null;
                    longValue   = to_long() * value.to_long();
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
            } else { // bool
                outV.boolValue      = output;
                outV.doubleValue    = null;
            }
        }

        public static void apply_function(DecimalValue d, DecimalValue outV, ConnectorsFunctions.TrigonometryFunction t) {

            var output = ConnectorsFunctions.get(t).Invoke(d.to_double());
            outV.doubleValue    = output;
            outV.boolValue      = null;
        }
    }
}