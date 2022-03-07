
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
using System.ComponentModel;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public static class Serializer{

        public static string serialize(object value) {
            return JsonUtility.ToJson(value);
        }
        public static object deserialize(string strValue, Type type) {
            return JsonUtility.FromJson(strValue, type);
        }
    }


    public static class Converter {

        //private const string p1 = "0.0";
        //private const string p2 = "0.00";
        //private const string p3 = "0.000";
        //private const string p4 = "0.0000";

        private const string g4  = "G4";
        private const string g7  = "G7";
        private const string g15 = "G15";

        private static readonly string vector2StrF = "{0}, {1}";
        private static readonly string vector3StrF = "{0}, {1}, {2}";
        private static readonly string transformValueStrF = "{0}\n{1}\n{2}";
        private static readonly string colorStrF = "{0}, {1}, {2}";


        // pitch yaw roll (x y z)
        // yaw roll pitch (y z x)
        public enum AxisOrder {
            PitchYawRoll, YawRollPitch
        };

        private static void log_error(string error) {
            ExVR.Log().error(string.Format("[CONVERTER] {0}", error));
        }

        // to decimal value
        public static DecimalValue to_decimal(int value) {
            return new DecimalValue(value);
        }
        public static DecimalValue to_decimal(float value) {
            return new DecimalValue(value);
        }
        public static DecimalValue to_decimal(double value) {
            return new DecimalValue(value);
        }

        public static DecimalValue to_decimal(object value) {

            if (value is int) {
                return new DecimalValue((int)value);
            } else if (value is float) {
                return new DecimalValue((float)value);
            } else if (value is double) {
                return new DecimalValue((double)value);
            } else if (value is bool) {
                return new DecimalValue(Converter.to_int((bool)value));
            } else if (value is DecimalValue) {
                return (DecimalValue)value;
            }
            return new DecimalValue(0);
        }

        public static DecimalValue to_decimal(string decimalStr) {
            
            var split = decimalStr.Split(':');
            int id = to_int(split[0]);
            if(id == 0) { // char
                return to_decimal(to_char(split[1]));
            }else if(id == 1) { // integer
                return to_decimal(to_int(split[1]));
            } else if (id == 2) { // float
                return to_decimal(to_float(split[1]));
            }
            // double
            return new DecimalValue(to_double(split[1]));
        }

        public static List<DecimalValue> to_decimal_list(DecimalValue value) {
            var list = new List<DecimalValue>(1);
            list.Add(value);
            return list;
        }


        // to string
        public static string to_string(object value) {

            if (value is string) {
                return (string)value;
            } else if (value is bool) {
                return to_string((bool)value);
            } else if (value is char) {
                return to_string((char)value);
            } else if (value is byte) {
                return to_string((byte)value);
            } else if (value is int) {
                return to_string((int)value);
            } else if (value is float) {
                return to_string((float)value);
            } else if (value is long) {
                return to_string((long)value);
            } else if (value is double) {
                return to_string((double)value);
            } else if (value is List<double>) {
                return to_string((List<double>)value);
            } else if (value is Vector2) {
                return to_string((Vector2)value);
            } else if (value is Vector3) {
                return to_string((Vector3)value);
            } else if (value is Color) {
                return to_string((Color)value);
            } else if (value is TransformValue) {
                return to_string((TransformValue)value);
            } else if (value is DecimalValue) {
                return to_string((DecimalValue)value);
            }

            return null;
        }

        public static string to_string(bool value, bool useLetters = true) {
            if (useLetters) {
                return value.ToString().ToLower();
            } else {
                return value ? "1" : "0";
            }
        }

        public static string to_string(char value) {
            return value.ToString();
        }
        public static string to_string(byte value) {
            return value.ToString();
        }
        public static string to_string(int value) {
            return value.ToString();
        }
        public static string to_string(long value) {            
            return value.ToString();
        }
        public static string to_string(float value, string format = g7) {
            return value.ToString(format);
        }
        public static string to_string(double value, string format = g7) {
            return value.ToString(format);
        }

        public static string to_string(List<double> values, string format = g7) {

            List<string> valuesStr = new List<string>(values.Count);
            foreach(var value in values) {
                valuesStr.Add(to_string(value, format));
            }
            return to_string(valuesStr, ",");
        }

        public static string to_string(Vector2 value, string format = g7) {
            return string.Format(vector2StrF, to_string(value.x, format), to_string(value.y, format));
        }
        public static string to_string(Vector3 value, string format = g7) {
            return string.Format(vector3StrF, to_string(value.x, format), to_string(value.y, format), to_string(value.z, format));
        }

        public static string to_string(TransformValue value) {
            return string.Format(transformValueStrF, to_string(value.position), to_string(value.rotation.eulerAngles), to_string(value.scale));
        }

        public static string to_string(Color value) {
            return string.Format(colorStrF, to_string(value.r, g4), to_string(value.g, g4), to_string(value.b, g4));
        }

        public static string to_string(DecimalValue value, string format = "G7") {
            if (value.has_int()) {
                return to_string(value.to_int());
            } else if (value.has_float()) {
                return to_string(value.to_float(), format);
            } else if (value.has_double()) {
                return to_string(value.to_double(), format);
            }
            return to_string(value.to_bool());
        }

        public static string to_string(List<string> value, string sep = " ") {
            return String.Join(sep, value.ToArray());
        }

        // to string list
        public static List<string> to_string_list(string value) {
            return new List<string>() { value};
        }

        public static List<string> to_string_list(object value) {
            if(value is List<string>) {
                return (List<string>)value;
            }
            return new List<string>();
        }

        // to short
        public static short to_short(object value) {

            if (value is int) {
                return to_short((int)value);
            } else if (value is short) {
                return (short)value;
            } else if (value is bool) {
                return to_short((bool)value);
            } else if (value is byte) {
                return to_short((byte)value);
            } else if (value is char) {
                return to_short((char)value);
            } else if (value is float) {
                return to_short((float)value);
            } else if (value is long) {
                return to_short((long)value);
            } else if (value is double) {
                return to_short((double)value);
            } else if (value is string) {
                return to_short((string)value);
            }

            log_error("Conversion to \"short\" not supported with current input object value.");
            return (byte)0;
        }

        public static short to_short(bool value) {
            return Convert.ToInt16(value);
        }
        public static short to_short(char value) {
            return Convert.ToInt16(value);
        }
        public static short to_short(long value) {
            return Convert.ToInt16(value);
        }
        public static short to_short(float value) {
            return Convert.ToInt16(value);
        }
        public static short to_short(double value) {
            return Convert.ToInt16(value);
        }
        public static short to_short(string strValue) {
            Int16.TryParse(strValue, out short result);
            return result;
        }

        // to byte
        public static byte to_byte(object value) {

            if (value is int) {
                return to_byte((int)value);
            } else if (value is bool) {
                return to_byte((bool)value);
            } else if (value is byte) {
                return (byte)value;
            } else if (value is char) {
                return to_byte((char)value);
            } else if (value is float) {
                return to_byte((float)value);
            } else if (value is long) {
                return to_byte((long)value);
            } else if (value is double) {
                return to_byte((double)value);
            } else if (value is string) {
                return to_byte((string)value);
            }

            log_error("Conversion to \"byte\" not supported with current input object value.");
            return (byte)0;
        }

        public static byte to_byte(bool value) {
            return Convert.ToByte(value);
        }
        public static byte to_byte(char value) {
            return Convert.ToByte(value);
        }
        public static byte to_byte(int value) {
            return Convert.ToByte(value);
        }
        public static byte to_byte(long value) {
            return Convert.ToByte(value);
        }
        public static byte to_byte(float value) {
            return Convert.ToByte(value);
        }
        public static byte to_byte(double value) {
            return Convert.ToByte(value);
        }
        public static byte to_byte(string strValue) {
            Byte.TryParse(strValue, out byte result);
            return result;
        }

        // to char
        public static char to_char(object value) {

            if (value is int) {
                return to_char((int)value);
            } else if (value is bool) {
                return to_char((bool)value);
            } else if (value is byte) {
                return to_char((byte)value);
            } else if (value is char) {
                return (char)value;
            } else if (value is float) {
                return to_char((float)value);
            } else if (value is long) {
                return to_char((long)value);
            } else if (value is double) {
                return to_char((double)value);
            } else if (value is string) {
                return to_char((string)value);
            }

            log_error("Conversion to \"char\" not supported with current input object value.");
            return (char)0;
        }

        public static char to_char(bool value) {
            return Convert.ToChar(value);
        }
        public static char to_char(byte value) {
            return Convert.ToChar(value);
        }
        public static char to_char(int value) {
            return Convert.ToChar(value);
        }
        public static char to_char(long value) {
            return Convert.ToChar(value);
        }
        public static char to_char(float value) {
            return Convert.ToChar(value);
        }
        public static char to_char(double value) {
            return Convert.ToChar(value);
        }
        public static char to_char(string strValue) {
            Char.TryParse(strValue, out char result);
            return result;            
        }

        // to bool
        public static bool to_bool(object value) {

            if (value is bool) {
                return (bool)value;
            } else if (value is byte) {
                return to_bool((byte)value);
            } else if (value is char) {
                return to_bool((char)value);
            } else if (value is int) {
                return to_bool((int)value);
            } else if (value is float) {
                return to_bool((float)value);
            } else if (value is long) {
                return to_bool((long)value);
            } else if (value is double) {
                return to_bool((double)value);
            } else if (value is string) {
                return to_bool((string)value);
            } else if (value is DecimalValue) {
                var dv = (DecimalValue)value;
                if (dv.has_int()) {
                    return to_bool(dv.to_int());
                } else if (dv.has_float()) {
                    return to_bool(dv.to_float());
                } else if (dv.has_double()) {
                    return to_bool(dv.to_double());
                } else if (dv.has_bool()) {
                    return dv.to_bool();
                }
                log_error("Conversion to \"bool\" impossible, decimal value is empty.");
                return false;
            }
            log_error("Conversion to \"bool\" not supported with current input object value.");
            return false;
        }
        public static bool to_bool(int value) {
            return Convert.ToBoolean(value);
        }
        public static bool to_bool(byte value) {
            return Convert.ToBoolean(value);
        }
        public static bool to_bool(char value) {
            return to_bool((int)value); // ToBoolean(Char) not supported
        }
        public static bool to_bool(float value) {
            return Convert.ToBoolean(value);
        }
        public static bool to_bool(long value) {
            return Convert.ToBoolean(value);
        }
        public static bool to_bool(double value) {
            return Convert.ToBoolean(value);
        }
        public static bool to_bool(string strValue) {
            if (strValue.Length == 1) {
                if (strValue[0] == '0') {
                    return false;
                }
                return true;
            }
            if(!Boolean.TryParse(strValue, out bool result)) {
                log_error(string.Format("Cannot convert {0} to bool.", strValue));
                return false;
            }
            return result;
        }

        // to long
        public static long to_long(object value) {

            if (value is int) {
                return to_long((int)value);
            } else if (value is bool) {
                return to_long((bool)value);
            } else if (value is byte) {
                return to_long((byte)value);
            } else if (value is char) {
                return to_long((char)value);
            } else if (value is float) {
                return to_long((float)value);
            } else if (value is long) {
                return (long)value;
            } else if (value is double) {
                return to_long((double)value);
            } else if (value is string) {
                return to_long((string)value);
            }

            log_error("Conversion to \"int\" not supported with current input object value.");
            return 0;
        }

        public static long to_long(bool value) {
            return Convert.ToInt64(value);
        }
        public static long to_long(byte value) {
            return Convert.ToInt64(value);
        }
        public static long to_long(char value) {
            return Convert.ToInt64(value);
        }

        public static long to_long(float value) {
            try {
                return Convert.ToInt64(value);
            } catch (OverflowException e) {
                log_error(e.Message);
            }
            return 0;
        }
        public static long to_long(long value) {
            try {
                return Convert.ToInt64(value);
            } catch (OverflowException e) {
                log_error(e.Message);
            }
            return 0;
        }

        public static long to_long(double value) {
            try {
                return Convert.ToInt64(value);
            } catch (OverflowException e) {
                log_error(e.Message);
            }
            return 0;
        }

        public static long to_long(string strValue) {
            Int64.TryParse(strValue, out long result);
            return result;
        }

        // to int
        public static int to_int(object value) {

            if (value is int) {
                return (int)value;
            } else if (value is bool) {
                return to_int((bool)value);
            } else if (value is byte) {
                return to_int((byte)value);
            } else if (value is char) {
                return to_int((char)value);
            } else if (value is float) {
                return to_int((float)value);
            } else if (value is long) {
                return to_int((long)value);
            } else if (value is double) {
                return to_int((double)value);
            } else if (value is string) {
                return to_int((string)value);
            }

            log_error("Conversion to \"int\" not supported with current input object value.");
            return 0;
        }
        public static int to_int(bool value) {
            return Convert.ToInt32(value);
        }
        public static int to_int(byte value) {
            return Convert.ToInt32(value);
        }
        public static int to_int(char value) {
            return Convert.ToInt32(value);
        }
        public static int to_int(float value) {
            try {
                return Convert.ToInt32(value);
            } catch (OverflowException e) {
                log_error(e.Message);
            }
            return 0;
        }
        public static int to_int(long value) {
            try {
                return Convert.ToInt32(value);
            }catch(OverflowException e) {
                log_error(e.Message);
            }
            return 0;
        }

        public static int to_int(double value) {
            try {
                return Convert.ToInt32(value);
            } catch (OverflowException e) {
                log_error(e.Message);
            }
            return 0;
        }
        public static int to_int(DecimalValue value) {
            return value.to_int();
        }

        public static int to_int(string strValue) {
            Int32.TryParse(strValue, out int result);
            return result;
        }

        // to float
        public static float to_float(object value) {
  
            if (value is float) {
                return (float)value;
            } else if (value is bool) {
                return to_float((bool)value);
            } else if (value is byte) {
                return to_float((byte)value);
            } else if (value is char) {
                return to_float((char)value);
            } else if (value is int) {
                return to_float((int)value);
            } else if (value is long) {
                return to_float((long)value);
            } else if (value is double) {
                return to_float((double)value);
            } else if (value is string) {
                return to_float((string)value);
            }

            log_error("Conversion to \"float\" not supported with current input object value. " + value.GetType().ToString());
            return 0f;
        }
        public static float to_float(bool value) {
            return Convert.ToSingle(value);
        }
        public static float to_float(byte value) {
            return Convert.ToSingle(value);
        }
        public static float to_float(char value) {
            return to_float((int)value); // ToSingle(Char) not supported
        }
        public static float to_float(int value) {
            return Convert.ToSingle(value);
        }
        public static float to_float(long value) {
            return Convert.ToSingle(value);
        }
        public static float to_float(double value) {
            return Convert.ToSingle(value);
        }
        public static float to_float(DecimalValue value) {
            return value.to_float();
        }

        public static float to_float(string strValue) {
            if(!Single.TryParse(strValue, out float result)) {                
                if(!Single.TryParse(strValue.Replace(",","."), out result)) {
                    log_error(string.Format("Float parse error: {0} -> {1}", strValue, result));
                }
            }
            return result;
        }

        // to double
        public static double to_double(object value) {

            if (value is double) {
                return (double)value;
            } else if (value is bool) {
                return to_double((bool)value);
            } else if (value is int) {
                return to_double((int)value);
            } else if (value is char) {
                return to_double((char)value);
            } else if (value is byte) {
                return to_double((byte)value);
            } else if (value is long) {
                return to_double((long)value);
            } else if (value is float) {
                return to_double((float)value);
            } else if (value is string) {
                return to_double((string)value);
            }

            return 0.0;
        }
        public static double to_double(bool value) {
            return Convert.ToDouble(value);
        }
        public static double to_double(byte value) {
            return Convert.ToDouble(value);
        }
        public static double to_double(char value) {
            return to_double((int)value); // ToDouble(Char) not supported
        }
        public static double to_double(int value) {
            return Convert.ToDouble(value);
        }
        public static double to_double(long value) {
            return Convert.ToDouble(value);
        }
        public static double to_double(float value) {
            return Convert.ToDouble(value);
        }
        public static double to_double(DecimalValue value) {
            return value.to_double();
        }
        public static double to_double(string strValue) {
            double.TryParse(strValue, out double result);
            return result;
        }


        // to color 
        public static Color to_color(string colorStr) {
            var split = colorStr.Split(' ');
            return new Color(to_float(split[1]), to_float(split[2]), to_float(split[3]), to_float(split[0]));
        }

        public static Color to_color(List<object> colorList) {
            return new Color((float)colorList[1], (float)colorList[2], (float)colorList[3], (float)colorList[0]);
        }

        // to vector2
        public static Vector2 to_vector2(object value) {
            if (value is Vector2) {
                return (Vector2)value;
            }
            return new Vector2();
        }

        public static Vector2 to_vector2(List<object> vector2List) {
            return new Vector2((float)vector2List[0], (float)vector2List[1]);
        }
        public static Vector2 to_vector2(string vector2Str) {
            var split = vector2Str.Split(' ');
            return new Vector2(to_float(split[0]), to_float(split[1]));
        }

        // to vector3
        public static Vector3 to_vector3(object value) {
            if (value is Vector3) {
                return (Vector3)value;
            }
            return new Vector3();
        }
        public static Vector3 to_vector3(List<object> vector3List) {
            return new Vector3((float)vector3List[0], (float)vector3List[1], (float)vector3List[2]);
        }

        public static Vector3 to_vector3(List<object> vector3List, AxisOrder order) {
            if(order == AxisOrder.PitchYawRoll) {
                return new Vector3((float)vector3List[0], (float)vector3List[1], (float)vector3List[2]);
            } else { // OrderRotation.YawRollPitch
                return new Vector3((float)vector3List[1], (float)vector3List[2], (float)vector3List[1]);
            }            
        }
        
        public static Vector3 to_vector3(string vector3Str) {
            var split = vector3Str.Split(' ');
            return new Vector3(to_float(split[0]), to_float(split[1]), to_float(split[2]));
        }

        // to id any
        public static IdAny to_id_any(object value) {
            if (value is IdAny) {
                return (IdAny)value;
            }
            return new IdAny();
        }

        // to string any
        public static StringAny to_string_any(object value) {
            if (value is StringAny) {
                return (StringAny)value;
            }
            return new StringAny();
        }

        // to double list
        public static List<double> to_double_list(object value) {

            if (value is float) {
                return to_double_list((float)value);
            } else if (value is int) {
                return to_double_list((int)value);
            } else if (value is double) {
                return to_double_list((double)value);
            } else if (value is string) {
                return to_double_list((string)value);
            } else if (value is DecimalValue) {
                return to_double_list((DecimalValue)value);
            } else if(value is List<double>) {
                return (List<double>)value;
            }
            return new List<double>();
        }

        public static List<double> to_double_list(int value) {
            List<double> list = new List<double>(1);
            list.Add(to_double(value));
            return list;
        }
        public static List<double> to_double_list(float value) {
            List<double> list = new List<double>(1);
            list.Add(to_double(value));
            return list;
        }
        public static List<double> to_double_list(double value) {
            List<double> list = new List<double>(1);
            list.Add(value);
            return list;
        }

        public static List<double> to_double_list(DecimalValue value) {
            List<double> list = new List<double>(1);
            list.Add(value.to_double());
            return list;
        }

        public static List<double> to_double_list(string values) {

            var split = values.Split(',');
            List<double> list = new List<double>(split.Length);
            foreach(var strValue in split) {
                list.Add(to_double(strValue));
            }            
            return list;
        }


        // to list
        public static List<T> to_list<T>(object value) {

            List<object> elements = (List<object>)value;
            List<T> array = new List<T>(elements.Count);
            foreach (object elem in elements) {
                array.Add((T)elem);
            }
            return array;
        }

        public static List<object> to_list(Vector2 value) {
            return new List<object> { value.x, value.y };
        }

        public static List<object> to_list(Vector3 value) {
            return new List<object> { value.x, value.y, value.z };
        }

        public static List<object> to_list(Color value) {
            return new List<object> { value.a, value.r, value.g, value.b};
        }

        public static List<object> to_list(Transform value, bool local) {
            return new List<object> {
                local ? value.localPosition.x    : value.position.x,
                local ? value.localPosition.y    : value.position.y,
                local ? value.localPosition.z    : value.position.z,
                local ? value.localEulerAngles.x : value.eulerAngles.x,
                local ? value.localEulerAngles.y : value.eulerAngles.y,
                local ? value.localEulerAngles.z : value.eulerAngles.z,
                value.localScale.x,
                value.localScale.y,
                value.localScale.z
            };
        }

        public static List<object> to_list(TransformValue value) {
            var eulerAngles = value.rotation.eulerAngles;
            return new List<object> {
                value.position.x,
                value.position.y,
                value.position.z,
                eulerAngles.x,
                eulerAngles.y,
                eulerAngles.z,
                value.scale.x,
                value.scale.y,
                value.scale.z
            };
        }

        public static List<object> to_list(List<Vector3> value) {

            if (value.Count != 3) {
                log_error("Invalid list size.");
                return new List<object> { 0, 0, 0, 0, 0, 0, 1, 1, 1 };
            }

            return new List<object> {
                value[0].x, value[0].y, value[0].z,
                value[1].x, value[1].y, value[1].z,
                value[2].x, value[2].y, value[2].z
            };
        }

        public static List<object> to_list(string listStr, string separator, TypeConverter converter) {
            string[] splitArg = listStr.Split(new string[] { separator }, StringSplitOptions.None);
            List<object> argList1D = new List<object>(splitArg.Length);
            foreach (string split in splitArg) {
                argList1D.Add(converter.ConvertFrom(split));
            }
            return argList1D;
        }

        // to 2D list
        public static List<List<object>> to_2D_list(string list2dStr, string separator, int rows, int cols, TypeConverter converter) {

            string[] splitArg = list2dStr.Split(new string[] { separator }, StringSplitOptions.None);
            List<List<object>> argList2D = new List<List<object>>(rows);
            if (splitArg.Length != rows * cols) {
                return null;
            }

            int id = 0;
            for (int ii = 0; ii < rows; ++ii) {
                argList2D.Add(new List<object>(cols));
                for (int jj = 0; jj < cols; ++jj) {
                    argList2D[ii].Add(converter.ConvertFrom(splitArg[id++]));
                }
            }

            return argList2D;
        }


        // to transform
        public static TransformValue to_transform(object value, AxisOrder order = AxisOrder.PitchYawRoll) {

            if(value is TransformValue trValue) {
                return trValue;
            } else {
                var values = to_list<float>(value);
                if (values.Count == 0) {
                    return new TransformValue();
                }

                var tr = new TransformValue();
                tr.position = new Vector3(values[0], values[1], values[2]);
                if (order == AxisOrder.PitchYawRoll) {
                    // pitch yaw roll (x y z)
                    
                    tr.rotation = Quaternion.Euler(values[3], values[4], values[5]);                    
                } else if( order == AxisOrder.YawRollPitch){

                    // yaw roll pitch (y z x)
                    //tr.position = new Vector3(values[1], values[2], values[0]);
                    tr.rotation = Quaternion.Euler(values[4], values[5], values[3]);
                }
                tr.scale = new Vector3(values[6], values[7], values[8]);
                return tr;
            }
        }

        public static void apply_to_transform(TransformValue value, Transform transform, bool local, bool position, bool rotation, bool scale) {

            if (scale) {
                transform.localScale = value.scale;
            }

            if (local) {
                if (position) {
                    transform.localPosition = value.position;
                }
                if (rotation) {
                    transform.localRotation = value.rotation;
                }
            } else {
                if (position) {
                    transform.position = value.position;
                }
                if (rotation) {
                    transform.rotation = value.rotation;                }
            }
        }

        public static void apply_to_transform(object value, Transform transform, bool local, bool position, bool rotation, bool scale) {

            if(value is TransformValue) {
                apply_to_transform((TransformValue)value, transform, local, position, rotation, scale);
                return;
            }

            var values = to_list<float>(value);
            if (scale) {
                transform.localScale = new Vector3(values[6], values[7], values[8]);
            }

            if (local) {
                if (position) {
                    transform.localPosition = new Vector3(values[0], values[1], values[2]);
                }
                if (rotation) {
                    //transform.localEulerAngles = new Vector3(values[3], values[4], values[5]);
                    transform.localRotation = 
                        Quaternion.AngleAxis(values[4], Vector3.up) *
                        Quaternion.AngleAxis(values[3], Vector3.right) *
                        Quaternion.AngleAxis(values[5], Vector3.forward);
                }
            } else {
                if (position) {
                    transform.position = new Vector3(values[0], values[1], values[2]);
                }
                if (rotation) {
                    //transform.eulerAngles = new Vector3(values[3], values[4], values[5]);
                    transform.rotation = 
                        Quaternion.AngleAxis(values[4], Vector3.up) *
                        Quaternion.AngleAxis(values[3], Vector3.right) *
                        Quaternion.AngleAxis(values[5], Vector3.forward);
                }
            }
        }

        // to animation curve
        public static AnimationCurve to_curve(string curveStr) {

            string[] splitArg = curveStr.Split(new string[] {" " }, StringSplitOptions.None);
            List<float> values = new List<float>(splitArg.Length);
            foreach(var v in splitArg) {
                values.Add(to_float(v));
            }
            return to_curve(values);
        }

        public static AnimationCurve to_curve(List<float> values) {

            AnimationCurve curve = new AnimationCurve();
            if (values.Count % 2 == 0) {
                for (int ii = 0; ii < values.Count / 2; ++ii) {
                    curve.AddKey(values[ii * 2], values[ii * 2 + 1]);
                }
            } else {
                log_error("Invalid values for curve.");
            }

            return curve;
        }
    }
}
