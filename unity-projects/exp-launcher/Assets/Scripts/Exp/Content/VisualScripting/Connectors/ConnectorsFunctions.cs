
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

// system
using System;
using System.Collections.Generic;

namespace Ex{
    public class ConnectorsFunctions {

        public enum TrigonometryFunction : int { cos, sin, tan, acos, asin, atan, cosh, sinh, tanh };
        public enum DecimalOperator : int { add, sub, multiply, divide, modulo, less, less_equal, superior, superior_equal, equal, different };
        public enum BinaryOperator : int { and, or, xor, not };
        public enum StringOperator : int { add, remove, contains, do_not_contains, equals, not_equals, starts_with, ends_with, splits_with };

        public static bool is_trigonometry_function(string name) {
            return strToTrigonometryFunction.ContainsKey(name);
        }
        public static bool is_decimal_operator(string name) {
            return strToDecimalOperator.ContainsKey(name);
        }
        public static bool is_string_operator(string name) {
            return strToStringOperators.ContainsKey(name);
        }

        public static TrigonometryFunction get_trigonometry_function(string name) {

            if (is_trigonometry_function(name)) {
                return strToTrigonometryFunction[name];
            }
            ExVR.Log().error(string.Format("Cannot convert {0} to trigonometry function.", name));
            return TrigonometryFunction.cos;

        }
        public static DecimalOperator get_decimal_operator(string name) {

            if (is_decimal_operator(name)) {
                return strToDecimalOperator[name];
            }
            ExVR.Log().error(string.Format("Cannot convert {0} to decimal operator.", name));
            return DecimalOperator.add;
        }

        public static BinaryOperator get_binary_function(string name) {

            if (strToBinaryOperator.ContainsKey(name)) {
                return strToBinaryOperator[name];
            }
            ExVR.Log().error(string.Format("Cannot convert {0} to binary function.", name));
            return BinaryOperator.and;
        }
        public static StringOperator get_string_operator(string name) {
            if (strToStringOperators.ContainsKey(name)) {
                return strToStringOperators[name];
            }
            ExVR.Log().error(string.Format("Cannot convert {0} to string operator.", name));
            return StringOperator.add;
        }

        public static Func<double, double> get(TrigonometryFunction t) {
            return trigonometryFunctions[t];
        }
        public static Func<double, double, object> get(DecimalOperator o) {
            return decimalOperators[o];
        }

        public static Func<bool, bool, bool> get(BinaryOperator b) {
            return binaryOperators[b];
        }
        public static Func<string, string, object> get(StringOperator o) {
            return stringOperators[o];
        }


        //public static Action<object> get_logger(Type type) {
        //    if (logger.ContainsKey(type)) {
        //        return logger[type];
        //    }
        //    return input => { ExVR.Log().message(input); };
        //}

        public static readonly Dictionary<string, TrigonometryFunction> strToTrigonometryFunction = new Dictionary<string, TrigonometryFunction> {
            ["Cos"] = TrigonometryFunction.cos,
            ["Sin"] = TrigonometryFunction.sin,
            ["Tan"] = TrigonometryFunction.tan,
            ["Acos"] = TrigonometryFunction.acos,
            ["Asin"] = TrigonometryFunction.asin,
            ["Atan"] = TrigonometryFunction.atan,
            ["Cosh"] = TrigonometryFunction.cosh,
            ["Sinh"] = TrigonometryFunction.sinh,
            ["Tanh"] = TrigonometryFunction.tanh
        };

        public static readonly Dictionary<string, DecimalOperator> strToDecimalOperator = new Dictionary<string, DecimalOperator> {
            ["  Add  "] = DecimalOperator.add,
            ["Add"]     = DecimalOperator.add,
            ["Substract"] = DecimalOperator.sub,
            ["Multiply"] = DecimalOperator.multiply,
            ["Divide"] = DecimalOperator.divide,
            ["Modulo"] = DecimalOperator.modulo,
            ["Inferior?"] = DecimalOperator.less,
            ["Inferior or equal?"] = DecimalOperator.less_equal,
            ["Superior?"] = DecimalOperator.superior,
            ["Superior or equal?"] = DecimalOperator.superior_equal,
            [" Equal? "] = DecimalOperator.equal,
            ["Not equal?"] = DecimalOperator.different
        };

        public static readonly Dictionary<string, BinaryOperator> strToBinaryOperator = new Dictionary<string, BinaryOperator> {
            ["AND"] = BinaryOperator.and,
            ["OR"] = BinaryOperator.or,
            ["XOR"] = BinaryOperator.xor,
            ["NOT"] = BinaryOperator.not
        };

        public static readonly Dictionary<string, StringOperator> strToStringOperators = new Dictionary<string, StringOperator> {
            ["Add"]             = StringOperator.add,
            ["Remove"]          = StringOperator.remove,
            ["Contains"]        = StringOperator.contains,
            ["Do not contains"] = StringOperator.do_not_contains,
            ["Equals"]          = StringOperator.equals,
            ["Do not equals"]   = StringOperator.not_equals,
            ["Starts with"]     = StringOperator.starts_with,
            ["Ends with"]       = StringOperator.ends_with,
            ["Splits with"]     = StringOperator.splits_with    
        };

        public static readonly Dictionary<TrigonometryFunction, Func<double, double>> trigonometryFunctions =new Dictionary<TrigonometryFunction, Func<double, double>> {
            [TrigonometryFunction.cos]  = input => { return Math.Cos(input);  },
            [TrigonometryFunction.sin]  = input => { return Math.Sin(input);  },
            [TrigonometryFunction.tan]  = input => { return Math.Tan(input);  },
            [TrigonometryFunction.acos] = input => { return Math.Acos(input); },
            [TrigonometryFunction.asin] = input => { return Math.Asin(input); },
            [TrigonometryFunction.atan] = input => { return Math.Atan(input); },
            [TrigonometryFunction.cosh] = input => { return Math.Cosh(input); },
            [TrigonometryFunction.sinh] = input => { return Math.Sinh(input); },
            [TrigonometryFunction.tanh] = input => { return Math.Tanh(input); }
        };

        public static readonly Dictionary<DecimalOperator, Func<double, double, object>> decimalOperators = new Dictionary<DecimalOperator, Func<double, double, object>> {
            [DecimalOperator.add]            = (input1, input2) => { return input1 + input2; },
            [DecimalOperator.sub]            = (input1, input2) => { return input1 - input2; },
            [DecimalOperator.multiply]       = (input1, input2) => { return input1 * input2;},
            [DecimalOperator.divide]         = (input1, input2) => { if (Math.Abs(input2) < 0.001) { return 0.0; } else { return input1 / input2; } },
            [DecimalOperator.modulo]         = (input1, input2) => { if (Math.Abs(input2) < 0.001) { return 0.0; } else { return input1 % input2; } },
            [DecimalOperator.less]           = (input1, input2) => { return input1 < input2; },
            [DecimalOperator.less_equal]     = (input1, input2) => { return (input1 < input2) || Maths.is_almost_equal(input1, input2); },
            [DecimalOperator.superior]       = (input1, input2) => { return input1 > input2; },
            [DecimalOperator.superior_equal] = (input1, input2) => { return (input1 > input2) || Maths.is_almost_equal(input1, input2); },
            [DecimalOperator.equal]          = (input1, input2) => { return Maths.is_almost_equal(input1, input2); },
            [DecimalOperator.different]      = (input1, input2) => { return !Maths.is_almost_equal(input1, input2); }
        };

        public static readonly Dictionary<BinaryOperator, Func<bool, bool, bool>> binaryOperators = new Dictionary<BinaryOperator, Func<bool, bool, bool>> {
            [BinaryOperator.and] = (input1, input2) => { return input1 && input2; },
            [BinaryOperator.or]  = (input1, input2) => { return input1 || input2; },
            [BinaryOperator.xor] = (input1, input2) => { return input1 != input2; },
            [BinaryOperator.not] = (input1, input2) => { return !input1; }
        };
        public static readonly Dictionary<StringOperator, Func<object, object, object>> stringOperators = new Dictionary<StringOperator, Func<object, object, object>> {
            [StringOperator.add]             = (input1, input2) => { return (string)input1 + (string)input2; },
            [StringOperator.remove]          = (input1, input2) => { return ((string)input1).Replace((string)input2, ""); },
            [StringOperator.contains]        = (input1, input2) => { return ((string)input1).Contains((string)input2); },
            [StringOperator.do_not_contains] = (input1, input2) => { return !((string)input1).Contains((string)input2); },
            [StringOperator.equals]          = (input1, input2) => { return (string)input1 == (string)input2; },
            [StringOperator.not_equals]      = (input1, input2) => { return (string)input1 != (string)input2; },
            [StringOperator.starts_with]     = (input1, input2) => { return ((string)input1).StartsWith((string)input2); },
            [StringOperator.ends_with]       = (input1, input2) => { return ((string)input1).EndsWith((string)input2); },
            [StringOperator.splits_with]     = (input1, input2) => { return new List<string>(((string)input1).Split(new string[] { (string)input2 }, StringSplitOptions.None)); }
        };

        //public static readonly Dictionary<Type, Action<object>> logger = new Dictionary<Type, Action<object>> {
        //    [typeof(int)] = input => { ExVR.Log().message(input); }
        //};
    }
}
