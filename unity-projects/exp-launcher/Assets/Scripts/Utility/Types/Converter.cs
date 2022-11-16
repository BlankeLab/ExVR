
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

    using SC = System.Convert;
    using SE = System.SystemException;
    public static class Converter {

        public const string g2 = "G2";
        public const string g4  = "G4";
        public const string g7  = "G7";
        public const string g15 = "G15";

        public static readonly string transformValueStrF   = "{0}\n{1}\n{2}";


        // pitch yaw roll (x y z)
        // yaw roll pitch (y z x)
        public enum AxisOrder {
            PitchYawRoll, YawRollPitch
        };

        private static void log_w(string warning) {
            ExVR.Log().warning(string.Format("[CONVERTER] {0}", warning));
        }

        private static void log_e(string error) {
            ExVR.Log().error(string.Format("[CONVERTER] {0}", error));
        }

        // types
        // # builtin
        private static readonly System.Type boolT       = typeof(bool);
        private static readonly System.Type byteT       = typeof(byte);
        private static readonly System.Type charT       = typeof(char);
        private static readonly System.Type shortT      = typeof(short);
        private static readonly System.Type intT        = typeof(int);
        private static readonly System.Type longT       = typeof(long);
        private static readonly System.Type floatT      = typeof(float);
        private static readonly System.Type doubleT     = typeof(double);
        private static readonly System.Type stringT     = typeof(string);
        // # custom
        private static readonly System.Type decValT     = typeof(DecimalValue);
        private static readonly System.Type idAnyT      = typeof(IdAny);
        private static readonly System.Type strAnyT     = typeof(StringAny);
        private static readonly System.Type timeAnyT    = typeof(TimeAny);        
        private static readonly System.Type trValT      = typeof(TransformValue);        
        // # unity
        private static readonly System.Type vec2T       = typeof(UnityEngine.Vector2);
        private static readonly System.Type vec3T       = typeof(UnityEngine.Vector3);
        private static readonly System.Type colT        = typeof(UnityEngine.Color);
        private static readonly System.Type aniCurveT   = typeof(UnityEngine.AnimationCurve);
        private static readonly System.Type trT         = typeof(UnityEngine.Transform);
        // # lists        
        private static readonly System.Type lIntT       = typeof(List<int>);
        private static readonly System.Type lFloatT     = typeof(List<float>);
        private static readonly System.Type lDoubleT    = typeof(List<double>);
        private static readonly System.Type lStringT    = typeof(List<string>);
        private static readonly System.Type lDecValT    = typeof(List<DecimalValue>);
        private static readonly System.Type lVec2T      = typeof(List<Vector2>);
        private static readonly System.Type lVec3T      = typeof(List<Vector3>);
        private static readonly System.Type lObjT       = typeof(List<object>);


        public static readonly Dictionary<System.Type, string> namesTypes = new Dictionary<Type, string> {
            // # builtin
            [boolT]     = "bool",
            [byteT]     = "byte",
            [charT]     = "char",
            [shortT]    = "short",
            [intT]      = "integer",
            [longT]     = "long",
            [floatT]    = "float",
            [doubleT]   = "double",
            [stringT]   = "string",
            // # custom
            [decValT]   = "decimal",
            [idAnyT]    = "id any",
            [strAnyT]   = "str any",
            [timeAnyT]  = "time any",
            [trValT]    = "tr value",
            // # unity
            [vec2T]     = "vec2",
            [vec3T]     = "vec3",
            [colT]      = "color",
            [aniCurveT] = "animation curve",
            [trT]       = "transform",
            // # lists
            [lIntT]     = "list int",
            [lFloatT]   = "list float",
            [lDoubleT]  = "list double",
            [lStringT]  = "list string",
            [lDecValT]  = "list decimal",
            [lVec2T]    = "list vec2",
            [lVec3T]    = "list vec3",
            [lObjT]     = "list object",
        };

        public static string get_type_name(System.Type type) {
            if (namesTypes.ContainsKey(type)) {
                return namesTypes[type];
            }
            return type.ToString();
        }

        public static T to<T>(object value) {

            if (value == null) {
                return default(T);
            }

            var toType = typeof(T);
            if (conv.ContainsKey(toType)) {
                return to<T>(conv[toType], value);
            } else {
                log_e(string.Format("No specific converter defined for destination type [{0}]. ", 
                    toType.ToString()));
                return default(T);
            }
        }
        public static T to<T>(Dictionary<System.Type, Func<object, object>> toConvF, object value) {

            if(value == null) {
                return default(T);
            }

            var fromType = value.GetType();
            if (toConvF.ContainsKey(fromType)) {
                return (T)(object)toConvF[fromType](value);
            } else {
                log_e(string.Format("No specific converter defined for source type [{0}] with destination type [{1}]. ", 
                    fromType.ToString(), typeof(T).ToString()));
                return default(T);
            }
        }

        private static object get_default(Type type) {
            if (type.IsValueType) {
                return Activator.CreateInstance(type);
            }
            return null;
        }

        public static object to_type(object value, Type destinationType, bool nullIfError = false, bool verbose = true) {

            if (conv.ContainsKey(destinationType)) {
                return to_type(conv[destinationType], value, destinationType, nullIfError, verbose);
            } else {
                if (verbose) {
                    log_e(string.Format("No specific converter defined for destination type [{0}]. ",
                        destinationType.ToString()));
                }
                return nullIfError ? null : get_default(destinationType);
            }
        }

        public static object to_type(Dictionary<System.Type, Func<object, object>> toConvF, object value, Type destinationType, bool nullIfError = false, bool verbose = true) {

            var fromType = value.GetType();
            if (toConvF.ContainsKey(fromType)) {
                return toConvF[fromType](value);
            } else {
                if (verbose) {
                    log_e(string.Format("No specific converter defined for source type [{0}] with destination type [{1}]. ",
                        fromType.ToString(), destinationType.ToString()));
                }
                return nullIfError ? null : get_default(destinationType);
            }
        }




        // specific to
        // # builtin
        public static bool to_bool(object value) {return to<bool>(conv[boolT], value);}
        public static byte to_byte(object value) {return to<byte>(conv[byteT], value);}
        public static char to_char(object value) {return to<char>(conv[charT], value);}
        public static short to_short(object value) {return to<short>(conv[shortT], value);}
        public static int to_int(object value) {return to<int>(conv[intT], value);}
        public static long to_long(object value) {return to<long>(conv[longT], value);}
        public static float to_float(object value) {return to<float>(conv[floatT], value);}
        public static double to_double(object value) {return to<double>(conv[doubleT], value);}
        public static string to_string(object value) {return to<string>(conv[stringT], value);}
        // # custom
        public static DecimalValue to_decimal(object value) {return to<DecimalValue>(conv[decValT], value);}
        public static IdAny to_id_any(object value) {return to<IdAny>(conv[idAnyT], value);}
        public static StringAny to_string_any(object value) {return to<StringAny>(conv[strAnyT], value);}
        public static TimeAny to_time_any(object value) { return to<TimeAny>(conv[timeAnyT], value); }
        public static TransformValue to_transform_value(object value, AxisOrder order = AxisOrder.PitchYawRoll) {

            var fromType = value.GetType();
            if (convTrVal.ContainsKey(fromType)) {
                return convTrVal[fromType](value, order);
            } else {
                log_e(string.Format("No specific converter defined for source type {0} with destination type {1}. ", fromType.ToString(), typeof(TransformValue).ToString()));
                return default(TransformValue);
            }
        }
        // # unity
        public static Vector2 to_vector2(object value) {return to<Vector2>(conv[vec2T], value);}
        public static Vector3 to_vector3(object value) {return to<Vector3>(conv[vec3T], value);}
        public static Color to_color(object value) {return to<Color>(conv[colT], value);}
        public static AnimationCurve to_animation_curve(object value) { return to<AnimationCurve>(conv[aniCurveT], value); }
        // # lists
        public static List<int> to_int_list(object value) { return to<List<int>>(conv[lIntT], value); }
        public static List<float> to_float_list(object value) { return to<List<float>>(conv[lFloatT], value); }
        public static List<double> to_double_list(object value) { return to<List<double>>(conv[lDoubleT], value); }
        public static List<string> to_string_list(object value) { return to<List<string>>(conv[lStringT], value); }
        public static List<DecimalValue> to_decimal_value_list(object value) { return to<List<DecimalValue>>(conv[lDecValT], value); }
        public static List<Vector2> to_vec2_list(object value) { return to<List<Vector2>>(conv[lVec2T], value); }
        public static List<Vector3> to_vec3_list(object value) { return to<List<Vector3>>(conv[lVec3T], value); }
        public static List<object> to_object_list(object value) { return to<List<object>>(conv[lObjT], value); }


        private static Dictionary<Type, Func<object, object>> types_to_bool = new Dictionary<Type, Func<object, object>>() {
            [boolT] = input => { return (bool)input; },
            [byteT] = input => { return SC.ToBoolean((byte)input); },
            [charT] = input => { return SC.ToBoolean(SC.ToInt32((char)input)); },
            [shortT] = input => { return SC.ToBoolean((short)input); },
            [intT] = input => { return SC.ToBoolean((int)input); },
            [longT] = input => { return SC.ToBoolean((long)input); },
            [floatT] = input => { return SC.ToBoolean((float)input); },
            [doubleT] = input => { return SC.ToBoolean((double)input); },
            [decValT] = input => { return ((DecimalValue)input).to_bool(); },
            [stringT] = input => {
                var strValue = (string)input;
                if (strValue.Length == 1) {
                    return strValue[0] != '0';
                }
                Boolean.TryParse(strValue, out bool result);
                return result;
            }
        };

        private static Dictionary<Type, Func<object, object>> types_to_byte = new Dictionary<Type, Func<object, object>>() {
            [boolT] = input => { return SC.ToByte((bool)input); },
            [byteT] = input => { return (byte)input; },
            [charT] = input => { return SC.ToByte((char)input); },
            [shortT] = input => { try { return SC.ToByte((short)input); } catch (SE e) { log_e(e.Message); } return default(byte); },
            [intT] = input => { try { return SC.ToByte((int)input); } catch (SE e) { log_e(e.Message); } return default(byte); },
            [longT] = input => { try { return SC.ToByte((long)input); } catch (SE e) { log_e(e.Message); } return default(byte); },
            [floatT] = input => { try { return SC.ToByte((float)input); } catch (SE e) { log_e(e.Message); } return default(byte); },
            [doubleT] = input => { try { return SC.ToByte((double)input); } catch (SE e) { log_e(e.Message); } return default(byte); },
            [decValT] = input => { try { return SC.ToByte(((DecimalValue)input).to_int()); } catch (SE e) { log_e(e.Message); } return default(byte); },
            [stringT] = input => { Byte.TryParse((string)input, out byte result); return result; }
        };

        private static Dictionary<Type, Func<object, object>> types_to_char = new Dictionary<Type, Func<object, object>>() {
            [boolT]   = input => { return SC.ToChar(to_int((bool)input));},
            [byteT]   = input => { return SC.ToChar((byte)input);},
            [charT]   = input => { return (char)input;},
            [shortT]  = input => { try { return SC.ToChar((short)input);}     catch (SE e) { log_e(e.Message);} return default(char);},
            [intT]    = input => { try { return SC.ToChar((int)input);}       catch (SE e) { log_e(e.Message);} return default(char);},
            [longT]   = input => { try { return SC.ToChar((long)input);}      catch (SE e) { log_e(e.Message);} return default(char);},
            [floatT]  = input => { try { return SC.ToChar(to_int((float)input));}   catch (SE e) { log_e(e.Message);} return default(char);},
            [doubleT] = input => { try { return SC.ToChar(to_int((double)input));}  catch (SE e) { log_e(e.Message);} return default(char);},
            [decValT] = input => { try { return SC.ToChar(((DecimalValue)input).to_int());} catch (SE e) { log_e(e.Message);} return default(char);},
            [stringT] = input => { Char.TryParse((string)input, out char result); return result;}
        };

        private static Dictionary<Type, Func<object, object>> types_to_short = new Dictionary<Type, Func<object, object>>() {
            [boolT] = input => { return SC.ToInt16((bool)input); },
            [byteT] = input => { return SC.ToInt16((byte)input); },
            [charT] = input => { return SC.ToInt16((char)input); },
            [shortT] = input => { return (short)input; },
            [intT] = input => { try { return SC.ToInt16((int)input); } catch (SE e) { log_e(e.Message); } return default(short); },
            [longT] = input => { try { return SC.ToInt16((long)input); } catch (SE e) { log_e(e.Message); } return default(short); },
            [floatT] = input => { try { return SC.ToInt16((float)input); } catch (SE e) { log_e(e.Message); } return default(short); },
            [doubleT] = input => { try { return SC.ToInt16((double)input); } catch (SE e) { log_e(e.Message); } return default(short); },
            [decValT] = input => { try { return SC.ToInt16(((DecimalValue)input).to_int()); } catch (SE e) { log_e(e.Message); } return default(short); },
            [stringT] = input => { Int16.TryParse((string)input, out short result); return result; }
        };

        private static Dictionary<Type, Func<object, object>> types_to_int = new Dictionary<Type, Func<object, object>>() {
            [boolT] = input => { return SC.ToInt32((bool)input); },
            [byteT] = input => { return SC.ToInt32((byte)input); },
            [charT] = input => { return SC.ToInt32((char)input); },
            [shortT] = input => { return SC.ToInt32((short)input); },
            [intT] = input => { return (int)input; },
            [longT] = input => { try { return SC.ToInt32((long)input); } catch (SE e) { log_e(e.Message); } return default(int); },
            [floatT] = input => { try { return SC.ToInt32((float)input); } catch (SE e) { log_e(e.Message); } return default(int); },
            [doubleT] = input => { try { return SC.ToInt32((double)input); } catch (SE e) { log_e(e.Message); } return default(int); },
            [decValT] = input => { return ((DecimalValue)input).to_int(); },
            [stringT] = input => { Int32.TryParse((string)input, out int result); return result; },
        };

        private static Dictionary<Type, Func<object, object>> types_to_long = new Dictionary<Type, Func<object, object>>() {
            [boolT] = input => { return SC.ToInt64((bool)input); },
            [byteT] = input => { return SC.ToInt64((byte)input); },
            [charT] = input => { return SC.ToInt64((char)input); },
            [shortT] = input => { return SC.ToInt64((short)input); },
            [intT] = input => { return SC.ToInt64((int)input); },
            [longT] = input => { return (long)input; },
            [floatT] = input => { try { return SC.ToInt64((float)input); } catch (SE e) { log_e(e.Message); } return default(long); },
            [doubleT] = input => { try { return SC.ToInt64((double)input); } catch (SE e) { log_e(e.Message); } return default(long); },
            [decValT] = input => { return ((DecimalValue)input).to_long(); },
            [stringT] = input => { Int64.TryParse((string)input, out long result); return result; },
        };

        private static Dictionary<Type, Func<object, object>> types_to_float = new Dictionary<Type, Func<object, object>>() {
            [boolT] = input => { return SC.ToSingle((bool)input); },
            [byteT] = input => { return SC.ToSingle((byte)input); },
            [charT] = input => { return SC.ToSingle(SC.ToInt32((char)input)); },
            [shortT] = input => { return SC.ToSingle((short)input); },
            [intT] = input => { return SC.ToSingle((int)input); },
            [longT] = input => { return SC.ToSingle((long)input); },
            [floatT] = input => { return (float)input; },
            [doubleT] = input => { try { return SC.ToSingle((double)input); } catch (SE e) { log_e(e.Message); } return default(float); },
            [decValT] = input => { return ((DecimalValue)input).to_float(); },
            [stringT] = input => {
                var strValue = (string)input;
                if (Single.TryParse(strValue, out float result1)) {
                    return result1;
                }
                if (!Single.TryParse(strValue.Replace(",", "."), out float result2)) {
                    log_e(string.Format("Float parse error: {0} -> {1}", strValue, result2));
                }
                return result2;
            }
        };

        private static Dictionary<Type, Func<object, object>> types_to_double = new Dictionary<Type, Func<object, object>>() {
            [boolT] = input => { return SC.ToDouble((bool)input); },
            [byteT] = input => { return SC.ToDouble((byte)input); },
            [charT] = input => { return SC.ToDouble(SC.ToInt32((char)input)); },
            [shortT] = input => { return SC.ToDouble((short)input); },
            [intT] = input => { return SC.ToDouble((int)input); },
            [longT] = input => { return SC.ToDouble((long)input); },
            [floatT] = input => { return SC.ToDouble((float)input); },
            [doubleT] = input => { return (double)input; },
            [decValT] = input => { return ((DecimalValue)input).to_double(); },
            [stringT] = input => {
                var strValue = (string)input;
                if (double.TryParse(strValue, out double result1)) {
                    return result1;
                }
                if (!double.TryParse(strValue.Replace(",", "."), out double result2)) {
                    log_e(string.Format("Double parse error: {0} -> {1}", strValue, result2));
                }
                return result2;
            }
        };

        private static Dictionary<Type, Func<object, object>> types_to_string = new Dictionary<Type, Func<object, object>>() {
            [boolT] = input => { return to_string((bool)input, true); },
            [byteT] = input => { return ((byte)input).ToString(); },
            [charT] = input => { return ((char)input).ToString(); },
            [shortT] = input => { return ((short)input).ToString(); },
            [intT] = input => { return ((int)input).ToString(); },
            [longT] = input => { return ((long)input).ToString(); },
            [floatT] = input => { return to_string((float)input, g7); },
            [doubleT] = input => { return to_string((double)input, g7); },
            [decValT] = input => { return ((DecimalValue)input).to_string(g7); },
            [stringT] = input => { return (string)input; },
            [vec2T] = input => { return to_string((Vector2)input, g7); },
            [vec3T] = input => { return to_string((Vector3)input, g7); },
            [colT] = input => { return to_string((Color)input, g4); },
            [trValT] = input => { return to_string((TransformValue)input, g7); },
            [idAnyT] = input => {
                var idA = (IdAny)input; return string.Format("Id:{0} Value:{1}", idA.value, to_string(idA.value));
            },
            [strAnyT] = input => {
                var idA = (StringAny)input; return string.Format("Id:{0} Value:{1}", idA.value, to_string(idA.value));
            },
            [lFloatT] = input => { return to_string((List<float>)input, g7, ","); },
            [lDoubleT] = input => { return to_string((List<double>)input, g7, ","); },
            [lStringT] = input => { return to_string((List<string>)input, " "); }
        };

        // to - from
        private static Dictionary<System.Type, Dictionary<System.Type, Func<object, object>>> conv = new Dictionary<Type, Dictionary<Type, Func<object, object>>> {
            #region builtin_types
            [boolT]   = types_to_bool,
            [byteT]   = types_to_byte,
            [charT]   = types_to_char,
            [shortT]  = types_to_short,
            [intT]    = types_to_int,
            [longT]   = types_to_long,
            [floatT]  = types_to_float,
            [doubleT] = types_to_double,
            [stringT] = types_to_string,
            #endregion
            #region custom_types
            [decValT] = new Dictionary<Type, Func<object, object>>() {
                [boolT] = input => { return new DecimalValue((bool)input);},
                [byteT] = input => { return new DecimalValue((byte)input);},
                [charT] = input => { return new DecimalValue((char)input);},
                [shortT] = input => { return new DecimalValue((short)input);},
                [intT] = input => { return new DecimalValue((int)input);},
                [longT] = input => { return  new DecimalValue((long)input);},
                [floatT] = input => { return  new DecimalValue((float)input);},
                [doubleT] = input => { return new DecimalValue((double)input);},
                [decValT] = input => { return ((DecimalValue)input);},
                [stringT] = input => {
                    var decimalStr = (string)input;
                    var split = decimalStr.Split(':');
                    if(split.Length != 2) {
                        log_e(string.Format("DecimalValue parse error with input: {0}", decimalStr));
                        return new DecimalValue();
                    }

                    int id = to_int(split[0]);
                    if (id == 0) { // char
                        return to_decimal(to_char(split[1]));
                    } else if (id == 1) { // integer
                        return to_decimal(to_int(split[1]));
                    } else if (id == 2) { // float
                        return to_decimal(to_float(split[1]));
                    }
                    // double
                    return new DecimalValue(to_double(split[1]));
                }
            },
            [idAnyT] = new Dictionary<Type, Func<object, object>>() {
                [idAnyT]    = input => { return (IdAny)input; },
                [intT]      = input => { return new IdAny((int)input, null); },
            },
            [strAnyT] = new Dictionary<Type, Func<object, object>>() {
                [strAnyT]   = input => { return (StringAny)input; },
                [stringT]   = input => { return new StringAny((string)input, null);},
            },
            [timeAnyT] = new Dictionary<Type, Func<object, object>>() {
                [timeAnyT] = input => { return (TimeAny)input; },
                [doubleT] = input => { return new TimeAny((double)input, -1.0, null); },
            },
            #endregion
            #region unity_types
            [vec2T] = new Dictionary<Type, Func<object, object>>() {
                [intT]      = input => { var vf = to_float((int)input);     return new Vector2(vf, vf); },
                [floatT]    = input => { var vf = (float)input;             return new Vector2(vf, vf);},
                [doubleT]   = input => { var vf = to_float((double)input);  return new Vector2(vf, vf); },
                [vec2T]     = input => {                                    return (Vector2)input; },
                [vec3T]     = input => { var v3 = ((Vector3)input);         return new Vector2(v3.x, v3.y); },
                [stringT]   = input => {
                    var vector2Str = (string)input;
                    var split = vector2Str.Split(' ');
                    if (split.Length >= 2) {
                        return new Vector2(to_float(split[0]), to_float(split[1]));
                    }
                    log_e(string.Format("Vector2 parse error with input: {0}", vector2Str));
                    return Vector2.zero;
                },
                [lObjT] = input => {
                    var listO = (List<object>)input;
                    if (listO.Count >= 2) {
                        return new Vector2(to_float(listO[0]), to_float(listO[1]));
                    }
                    log_e(string.Format("Vector2 parse error with input list of size: {0}", listO.Count));
                    return Vector2.zero;
                },
            },
            [vec3T] = new Dictionary<Type, Func<object, object>>() {
                [intT]      = input => { var vf = to_float((int)input); return new Vector3(vf, vf, vf); },
                [floatT]    = input => { var vf = (float)input; return new Vector3(vf, vf, vf); },
                [doubleT]   = input => { var vf = to_float((double)input); return new Vector3(vf, vf, vf); },
                [vec2T]     = input => { var v3 = ((Vector2)input); return new Vector3(v3.x, v3.y, 0); },
                [vec3T]     = input => { return (Vector3)input; },
                [colT]      = input => { var c = ((Color)input); return new Vector3(c.r,c.g,c.b); },
                [stringT] = input => {
                    var vector3Str = (string)input;
                    var split = vector3Str.Split(' ');
                    if (split.Length >= 3) {
                        return new Vector3(to_float(split[0]), to_float(split[1]), to_float(split[2]));
                    }
                    log_e(string.Format("Vector3 parse error with input: {0}", vector3Str));
                    return Vector3.zero;
                },
                [lObjT] = input => {
                    var listO = (List<object>)input;
                    if (listO.Count >= 3) {
                        return new Vector3(to_float(listO[0]), to_float(listO[1]), to_float(listO[2]));
                    }
                    log_e(string.Format("Vector3 parse error with input list of size: {0}", listO.Count));
                    return Vector3.zero;
                },
            },
            [colT] = new Dictionary<Type, Func<object, object>>() {
                [colT]      = input => { return (Color)input; },
                [stringT]   = input => {
                    var colorStr = (string)input; 
                    var split = colorStr.Split(' ');
                    if (split.Length == 4) {
                        return new Color(to_float(split[0]), to_float(split[1]), to_float(split[2]), to_float(split[3]));
                    }else if(split.Length == 3) {
                        return new Color(to_float(split[0]), to_float(split[1]), to_float(split[2]));
                    }
                    log_e(string.Format("Color parse error with input: {0}", colorStr));
                    return Color.black;
                },     
                [lObjT]     = input => {
                    var listO = (List<object>)input;
                    if (listO.Count >= 4) {
                        return new Color(to_float(listO[0]), to_float(listO[1]), to_float(listO[2]), to_float(listO[3]));
                    } else if (listO.Count >= 3) {
                        return new Color(to_float(listO[0]), to_float(listO[1]), to_float(listO[2]));
                    }
                    log_e(string.Format("Color parse error with input list of size: {0}", listO.Count));
                    return Color.black;
                },
                [vec3T]     = input => { var v3 = (Vector3)input; return new Color(v3.x, v3.y, v3.z); },
            },
            [aniCurveT] = new Dictionary<Type, Func<object, object>>() {
                [aniCurveT] = input => { return (AnimationCurve)input; },
                [lFloatT] = input => {
                    var values = (List<float>)input;
                    AnimationCurve curve = new AnimationCurve();
                    if (values.Count % 2 == 0) {
                        for (int ii = 0; ii < values.Count / 2; ++ii) {
                            curve.AddKey(values[ii * 2], values[ii * 2 + 1]);
                        }
                    } else {
                        log_e("Invalid values for curve.");
                    }
                    return curve;
                },
                [lDoubleT] = input => {
                    return to_animation_curve(to_float_list((List<double>)input));
                },
            },
            #endregion
            #region lists
            [lIntT] = new Dictionary<Type, Func<object, object>>() {
                [intT] = input => { return new List<int>() { (int)input }; },
                [floatT] = input => { return new List<int>() { to_int((float)input) }; },
                [doubleT] = input => { return new List<int>() { to_int((double)input) }; },
                [decValT] = input => { return new List<int>() { ((DecimalValue)input).to_int() }; },
                [vec2T] = input => { var v2 = (Vector2)input; return new List<int>() { to_int(v2.x), to_int(v2.y) }; },
                [vec3T] = input => { var v3 = (Vector3)input; return new List<int>() { to_int(v3.x), to_int(v3.y), to_int(v3.z) }; },
                [stringT] = input => {
                    var split = Ex.Text.split((string)input, ','); List<int> list = new List<int>(split.Length);
                    foreach (var strValue in split) { list.Add(to_int(strValue)); }
                    return list;
                },
                [lIntT] = input => { return (List<int>)input; },
                [lFloatT] = input => {
                    var lStr = (List<float>)input; var list = new List<int>(lStr.Count);
                    foreach (var value in lStr) { list.Add(to_int(value)); }
                    return list;
                },
                [lDoubleT] = input => {
                    var lStr = (List<double>)input; var list = new List<int>(lStr.Count);
                    foreach (var value in lStr) { list.Add(to_int(value)); }
                    return list;
                },
                [lStringT] = input => {
                    var lStr = (List<String>)input; var list = new List<int>(lStr.Count);
                    foreach (var value in lStr) { list.Add(to_int(value)); }
                    return list;
                },
                [lVec2T] = input => {
                    var lV2 = (List<Vector2>)input; var list = new List<int>(lV2.Count * 2);
                    foreach (var value in lV2) { list.Add(to_int(value.x)); list.Add(to_int(value.y)); }
                    return list;
                },
                [lVec3T] = input => {
                    var lV3 = (List<Vector3>)input; var list = new List<int>(lV3.Count * 3);
                    foreach (var value in lV3) { list.Add(to_int(value.x)); list.Add(to_int(value.y)); list.Add(to_int(value.z)); }
                    return list;
                },
                [lObjT] = input => {
                    var lObj = (List<object>)input; var list = new List<int>(lObj.Count);
                    foreach (var value in lObj) { list.Add(to_int(value)); }
                    return list;
                },
            },
            [lFloatT] = new Dictionary<Type, Func<object, object>>() {
                [intT]      = input => { return new List<float>() { to_float((int)input) }; },
                [floatT]    = input => { return new List<float>() {(float)input }; },
                [doubleT]   = input => { return new List<float>() { to_float((double)input) }; },
                [decValT]   = input => { return new List<float>() { ((DecimalValue)input).to_float() }; },
                [vec2T]     = input => { var v2 = (Vector2)input; return new List<float>() { v2.x, v2.y}; },
                [vec3T]     = input => { var v3 = (Vector3)input; return new List<float>() { v3.x, v3.y, v3.z }; },
                [stringT]   = input => {
                    var split = Ex.Text.split((string)input, ','); List<float> list = new List<float>(split.Length);
                    foreach (var strValue in split) { list.Add(to_float(strValue));} return list;
                },
                [lIntT] = input => {
                    var lStr = (List<int>)input; var list = new List<float>(lStr.Count);
                    foreach (var value in lStr) { list.Add(to_float(value)); }
                    return list;
                },
                [lFloatT] = input => { return (List<float>)input; },
                [lDoubleT]  = input => {
                    var lStr = (List<double>)input; var list = new List<float>(lStr.Count);
                    foreach (var value in lStr) { list.Add(to_float(value));} return list;
                },
                [lStringT]  = input => {
                    var lStr = (List<String>)input; var list = new List<float>(lStr.Count);
                    foreach (var value in lStr) { list.Add(to_float(value));} return list;
                },
                [lVec2T]    = input => {
                    var lV2 = (List<Vector2>)input; var list = new List<float>(lV2.Count * 2);
                    foreach (var value in lV2) { list.Add(value.x); list.Add(value.y);} return list;
                },
                [lVec3T]    = input => {
                    var lV3 = (List<Vector3>)input; var list = new List<float>(lV3.Count * 3);
                    foreach (var value in lV3) { list.Add(value.x); list.Add(value.y); list.Add(value.z);} return list;
                },
                [lObjT] = input => {
                    var lObj = (List<object>)input; var list = new List<float>(lObj.Count);
                    foreach (var value in lObj) { list.Add(to_float(value));}return list;
                },
            },
            [lDoubleT] = new Dictionary<Type, Func<object, object>>() {
                [intT]      = input => { return new List<double>() { to_double((int)input) }; },
                [floatT]    = input => { return new List<double>() { to_double((float)input) }; },
                [doubleT]   = input => { return new List<double>() { (double)input }; },                
                [decValT]   = input => { return new List<double>() { ((DecimalValue)input).to_double() }; },
                [vec2T]     = input => { var v2 = (Vector2)input; return new List<double>() { to_double(v2.x), to_double(v2.y) }; },
                [vec3T]     = input => { var v3 = (Vector3)input; return new List<double>() { to_double(v3.x), to_double(v3.y), to_double(v3.z) }; },
                [stringT]   = input => {
                    var split = Ex.Text.split((string)input, ','); var list = new List<double>(split.Length);
                    foreach (var strValue in split) { list.Add(to_double(strValue)); }
                    return list;
                },
                [lIntT] = input => {
                    var lStr = (List<int>)input; var list = new List<double>(lStr.Count);
                    foreach (var value in lStr) { list.Add(to_double(value)); }
                    return list;
                },
                [lFloatT] = input => {
                    var lStr = (List<float>)input; var list = new List<double>(lStr.Count);
                    foreach (var value in lStr) { list.Add(to_double(value)); }
                    return list;
                },
                [lDoubleT] = input => { return (List<double>)input; },
                [lStringT] = input => {
                    var lStr = (List<string>)input; var list = new List<double>(lStr.Count);
                    foreach (var value in lStr) { list.Add(to_double(value)); }
                    return list;
                },
                [lVec2T] = input => {
                    var lV2 = (List<Vector2>)input; var list = new List<double>(lV2.Count * 2);
                    foreach (var value in lV2) { list.Add(to_double(value.x)); list.Add(to_double(value.y)); }
                    return list;
                },
                [lVec3T] = input => {
                    var lV3 = (List<Vector3>)input; var list = new List<double>(lV3.Count * 3);
                    foreach (var value in lV3) { list.Add(to_double(value.x)); list.Add(to_double(value.y)); list.Add(to_double(value.z)); }
                    return list;
                },
                [lObjT] = input => {
                    var lObj = (List<object>)input; var list = new List<double>(lObj.Count);
                    foreach (var value in lObj) { list.Add(to_double(value)); }
                    return list;
                },
            },
            [lStringT] = new Dictionary<Type, Func<object, object>>() {
                [stringT]   = input => { return new List<string>() { (string)input }; },
                [vec2T]     = input => { var v2 = (Vector2)input; return new List<string>() { to_string(v2.x), to_string(v2.y) }; },
                [vec3T]     = input => { var v3 = (Vector3)input; return new List<string>() { to_string(v3.x), to_string(v3.y), to_string(v3.z) }; },
                [lStringT]  = input => { return (List<string>)input; },
            },
            [lObjT] = new Dictionary<Type, Func<object, object>>() {
                [lObjT] = input => { return (List<object>)input; },
                [vec2T] = input => { var v2 = (Vector2)input; return new List<object>() { v2.x, v2.y }; },
                [vec3T] = input => { var v3 = (Vector3)input; return new List<object>() { v3.x, v3.y, v3.z }; },
                [colT]  = input => { var col = (Color)input; return new List<object>() { col.a, col.r, col.g, col.b }; },
                [trValT] = input => {
                    var trVal = (TransformValue)input;
                    var eulerAngles = trVal.rotation.eulerAngles;
                    return new List<object>() {
                        trVal.position.x, trVal.position.y, trVal.position.z,
                        eulerAngles.x, eulerAngles.y, eulerAngles.z,
                        trVal.scale.x, trVal.scale.y, trVal.scale.z
                    };
                },
                [lVec3T] = input => {
                    var lVec = (List<Vector3>)input;
                    List<object> values = new List<object>(lVec.Count*3);
                    foreach (var vec in (List<Vector3>)input) {
                        values.Add(vec.x);
                        values.Add(vec.y);
                        values.Add(vec.z);
                    }
                    return values;
                },
            },
            [lDecValT] = new Dictionary<Type, Func<object, object>>() {
                [decValT] = input => { return new List<DecimalValue>() { (DecimalValue)input }; },
            },
            #endregion
        };

        #region to_list
        public static List<T> to_list<T>(object value) {
            if (value is List<object>) {
                return to_list<T>((List<object>)value);
            }
            return new List<T>() { (T)value };
        }

        public static List<T> to_list<T>(List<object> value) {
            List<T> array = new List<T>(value.Count);
            foreach (object elem in value) {
                array.Add((T)elem);
            }
            return array;
        }

        #endregion

        #region to_object_list

        public static List<object> to_object_list(Transform value, bool local) {
            return new List<object>(){
                local ? value.localPosition.x    : value.position.x,
                local ? value.localPosition.y    : value.position.y,
                local ? value.localPosition.z    : value.position.z,
                local ? value.localEulerAngles.x : value.eulerAngles.x,
                local ? value.localEulerAngles.y : value.eulerAngles.y,
                local ? value.localEulerAngles.z : value.eulerAngles.z,
                local ? value.localScale.x       : value.lossyScale.x,
                local ? value.localScale.y       : value.lossyScale.y,
                local ? value.localScale.z       : value.lossyScale.z
            };
        }

        #endregion

        #region to_string
        public static string to_string(bool value, bool useLetters) {
            return useLetters ? value.ToString().ToLower() : (value ? "1" : "0");
        }
        public static string to_string(float value, string format) {
            return value.ToString(format);
        }
        public static string to_string(double value, string format) {
            return value.ToString(format);
        }
        public static string to_string(Vector2 value, string format, string sep = ",") {
            return string.Join(sep, new string[] { to_string(value.x, format), to_string(value.y, format)});
        }
        public static string to_string(Vector3 value, string format, string sep = ",") {
            return string.Join(sep, new string[] { to_string(value.x, format), to_string(value.y, format), to_string(value.z, format) });
        }
        public static string to_string(TransformValue value, string format, string sep = "\n") {
            return string.Join(sep, new string[] { to_string(value.position, format), to_string(value.rotation.eulerAngles, format), to_string(value.scale, format) });
        }
        public static string to_string(Color value, string format, string sep = " ") {
            return string.Join(sep, new string[] { to_string(value.r, format), to_string(value.g, format), to_string(value.b, format), to_string(value.a, format) });
        }
        public static string to_string(DecimalValue value, string format) {
            return value.to_string(format);
        }
        public static string to_string(List<float> values, string format, string sep) {
            List<string> valuesStr = new List<string>(values.Count);
            foreach (var value in values) {valuesStr.Add(to_string(value, format));}
            return to_string(valuesStr, sep);
        }

        public static string to_string(List<double> values, string format, string sep) {
            List<string> valuesStr = new List<string>(values.Count);
            foreach (var value in values) {valuesStr.Add(to_string(value, format));}
            return to_string(valuesStr, sep);
        }

        public static string to_string(List<string> value, string sep) {
            return string.Join(sep, value.ToArray());
        }

        #endregion

        #region from_string
        public static List<float> to_float_list(string values, char sep) {
            var split = Ex.Text.split(values, sep); List<float> list = new List<float>(split.Length);
            foreach (var strValue in split) { list.Add(to_float(strValue)); }
            return list;
        }

        #endregion

        #region to_vector3
        public static Vector3 to_vector3(List<object> vector3List, AxisOrder order) {
            if (order == AxisOrder.PitchYawRoll) {
                return new Vector3(to_float(vector3List[0]), to_float(vector3List[1]), to_float(vector3List[2]));
            } else { // OrderRotation.YawRollPitch
                return new Vector3(to_float(vector3List[1]), to_float(vector3List[2]), to_float(vector3List[0]));
            }
        }

        #endregion

        #region to_transform_value


        private static Dictionary<System.Type, Func<object, Converter.AxisOrder, TransformValue>> convTrVal = new Dictionary<Type, Func<object, Converter.AxisOrder, TransformValue>>() {
            [trValT]    = (input, order) => { return (TransformValue)input; },
            [trT]       = (input, order) => { var tr = (Transform)input; return new TransformValue(tr.position, tr.rotation, tr.localScale); }, // TODO: lossy scale?
            [lDoubleT]  = (input, order) => { return convTrVal[lFloatT](to_float_list((List<double>)input), order); },            
            [stringT]   = (input, order) => { return convTrVal[lFloatT](to_float_list((string)input, ' '), order); },
            [lVec3T]    = (input, order) => { return convTrVal[lFloatT](to_float_list((List<Vector3>)input), order); },
            [lStringT]  = (input, order) => { return convTrVal[lFloatT](to_float_list((List<string>)input), order); },
            [lObjT]     = (input, order) => { return convTrVal[lFloatT](to_float_list((List<object>)input), order); },
            [lFloatT]   = (input, order) => {
                var values = (List<float>)input;
                TransformValue trV = new TransformValue();
                if (values.Count == 9) {
                    trV.position.x = values[0]; trV.position.y = values[1]; trV.position.z = values[2];
                    if (order == AxisOrder.PitchYawRoll) {
                        trV.rotation = Quaternion.Euler(values[3], values[4], values[5]);
                    } else {
                        trV.rotation = Quaternion.Euler(values[4], values[5], values[3]);
                    }
                    trV.scale.x = values[6]; trV.scale.y = values[7]; trV.scale.z = values[8];
                } else {
                    log_e("Invalid number of values for TransformValue.");
                }
                return trV;  
            },
        };



        #endregion

        public static void unit_tests() {

 

            //// # lists
            //private static readonly System.Type lFloatT = typeof(List<float>);
            //private static readonly System.Type lDoubleT = typeof(List<double>);
            //private static readonly System.Type lStringT = typeof(List<string>);
            //private static readonly System.Type lDecValT = typeof(List<DecimalValue>);
            //private static readonly System.Type lVec2T = typeof(List<Vector2>);
            //private static readonly System.Type lVec3T = typeof(List<Vector3>);
            //private static readonly System.Type lObjT = typeof(List<object>);

            bool vBool1 = false; bool vBool2 = true;
            byte vByte1 = 0, vByte2 = 25, vByte3 = 255;
            char vChar1 = 'e', vChar2 = (char)106, vChar3 = '\u006A';
            short vShort1 = -32768, vShort2 = 4154, vShort3 = 32767;
            int vInt = 866;
            long vLong = 468468484;
            float vFloat = 846.8f;
            double vDouble = 58453.07578;
            string vString = "test";

            Vector2 vVec2 = new Vector2(1, 2);
            Vector3 vVec3 = new Vector3(1, 2, 3);
            Color vCol = new Color(1, 0, 0, 1);
            AnimationCurve vAniCurve = new AnimationCurve();
            vAniCurve.AddKey(0, 0.0f);
            vAniCurve.AddKey(0.2f, 0.5f);
            vAniCurve.AddKey(1.0f, 1.0f);
            GameObject go = new GameObject();
            Transform vTr = go.transform;

            DecimalValue vDecVamA = new DecimalValue(true);
            DecimalValue vDecVamB = new DecimalValue(164);
            DecimalValue vDecVamC = new DecimalValue(21.045);
            IdAny vIdAnyA = new IdAny(5, 45);
            IdAny vIdAnyB = new IdAny(3, "785");
            IdAny vIdAnyC = new IdAny(2, 9489.07);
            StringAny vStrAnyA = new StringAny("a", 45);
            StringAny vStrAnyB = new StringAny("b", "785");
            StringAny vStrAnyC = new StringAny("c", 9489.07);
            TransformValue vTrVal = new TransformValue();

            List<object> valuesToTest = new List<object>() {
                // builtin
                vBool1, vBool2 ,vByte1, vByte2, vByte3, vChar1, vChar2, vChar3, 
                vShort1, vShort2, vShort3, vInt, vLong, vFloat, vDouble, vString,
                // unity
                vVec2, vVec3, vCol, vAniCurve, vTr,
                // custom
                vDecVamA, vDecVamB, vDecVamC, vIdAnyA, vIdAnyB, vIdAnyC, vStrAnyA, vStrAnyB, vStrAnyC, vTrVal                
            };
            test(valuesToTest);

            ExVR.Memory().destroy_go(go);
        }

        private static void test(List<object> valuesToTest) {
            foreach (var valueToConvert in valuesToTest) {
                foreach (var typeToConvert in valuesToTest) {
                    var type = typeToConvert.GetType();
                    var convertedValue = to_type(valueToConvert, type, true, false);
                   // if(convertedValue != null) {
                   //     Debug.Log(string.Format("[{0}] -> [{1}] : [{2}] -> [{3}]",
                   //         valueToConvert.GetType(), type, valueToConvert, convertedValue));
                   // } else {
                   //     Debug.LogError(string.Format("[{0}] -> [{1}] : [{2}] -> [FAILURE]",
                   //         valueToConvert.GetType(), type, valueToConvert));
                   //}
                }
            }
        }
    }
}
