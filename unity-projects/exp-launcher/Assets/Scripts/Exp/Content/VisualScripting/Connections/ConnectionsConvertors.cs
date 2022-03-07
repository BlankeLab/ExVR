
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

// unity
using UnityEngine;

namespace Ex{

    public class ConnectionsConvertors {

        private static Dictionary<string, Func<object, object>> convertors = new Dictionary<string, Func<object, object>> {
            // # from any to
            ["any:id_any"] = input => { return Converter.to_id_any(input); },
            ["any:string_any"] = input => { return Converter.to_string_any(input); },
            ["any:bool"] = input => { return Converter.to_bool(input); },
            ["any:integer"] = input => { return Converter.to_int(input); },
            ["any:float"] = input => { return Converter.to_float(input); },
            ["any:real"] = input => { return Converter.to_double(input); },
            ["any:string"] = input => { return Converter.to_string(input); },
            ["any:vector2"] = input => { return Converter.to_vector2(input); },
            ["any:vector3"] = input => { return Converter.to_vector3(input); },
            ["any:transform"] = input => { return Converter.to_transform(input); },
            ["any:decimal"] = input => { return Converter.to_decimal(input); },
            ["any:list_string"] = input => { return Converter.to_string_list(input); },
            ["any:lm_frame"] = input => { return (Leap.Frame)input; },
            ["any:lm_hands_frame"] = input => { return (LeapMotionFrame)input; },
            ["any:kinect_body"] = input => { return (KinectBodyData)input; },
            ["any:image"] = input => { return (ImageContainer)input; },
            ["any:real_list"] = input => { return Converter.to_double_list(input); },
            ["any:keyboard_button"] = input => { return (Input.KeyboardButtonEvent)input; },
            ["any:joypad_button"] = input => { return (Input.JoypadButtonEvent)input; },
            ["any:joypad_axis"] = input => { return (Input.JoypadAxisEvent)input; },
            ["any:mouse_button"] = input => { return (Input.MouseButtonEvent)input; },
            ["any:trigger"] = input => { return null; },
            // # from trigger
            ["trigger:any"] = input => { return 0; },
            // # from bool to 
            ["bool:integer"] = input => { return Converter.to_int((bool)input); },
            ["bool:float"] = input => { return Converter.to_float((bool)input); },
            ["bool:real"] = input => { return Converter.to_double((bool)input); },
            ["bool:decimal"] = input => { return Converter.to_decimal((bool)input); },
            ["bool:string"] = input => { return Converter.to_string((bool)input); },
            ["bool:any"] = input => { return input; },
            ["bool:trigger"] = input => { return null; },
            // # from int to
            ["integer:bool"] = input => { return Converter.to_bool((int)input); },
            ["integer:float"] = input => { return Converter.to_float((int)input); },
            ["integer:real"] = input => { return Converter.to_double((int)input); },
            ["integer:real_list"] = input => { return Converter.to_double_list((int)input); },
            ["integer:decimal"] = input => { return Converter.to_decimal((int)input); },
            ["integer:string"] = input => { return Converter.to_string((int)input); },
            ["integer:any"] = input => { return input; },
            ["integer:trigger"] = input => { return null; },
            // # from float to 
            ["float:bool"] = input => { return Converter.to_bool((float)input); },
            ["float:integer"] = input => { return Converter.to_int((float)input); },
            ["float:real"] = input => { return Converter.to_double((float)input); },
            ["float:real_list"] = input => { return Converter.to_double_list((float)input); },
            ["float:decimal"] = input => { return Converter.to_decimal((float)input); },
            ["float:string"] = input => { return Converter.to_string((float)input); },
            ["float:any"] = input => { return input; },
            ["float:trigger"] = input => { return null; },
            // # from real to 
            ["real:bool"] = input => { return Converter.to_bool((double)input); },
            ["real:integer"] = input => { return Converter.to_int((double)input); },
            ["real:float"] = input => { return Converter.to_float((double)input); },
            ["real:decimal"] = input => { return Converter.to_decimal((double)input); },
            ["real:real_list"] = input => { return Converter.to_double_list((double)input); },
            ["real:string"] = input => { return Converter.to_string((double)input); },
            ["real:any"] = input => { return input; },
            ["real:trigger"] = input => { return null; },
            // # from decimal to
            ["decimal:bool"] = input => { return Converter.to_bool(input); },
            ["decimal:integer"] = input => { return Converter.to_int((DecimalValue)input); },
            ["decimal:float"] = input => { return Converter.to_float((DecimalValue)input); },
            ["decimal:real"] = input => { return Converter.to_double((DecimalValue)input); },
            ["decimal:string"] = input => { return Converter.to_string((DecimalValue)input); },
            ["decimal:real_list"] = input => { return Converter.to_double_list((DecimalValue)input); },
            ["decimal:decimal_list"] = input => { return Converter.to_decimal_list((DecimalValue)input); },
            ["decimal:any"] = input => { return input; },
            ["decimal:trigger"] = input => { return null; },
            // # from string to            
            ["string:string_list"] = input => { return Converter.to_string_list((string)input); },
            ["string:any"] = input => { return input; },
            ["string:trigger"] = input => { return null; },
            ["string:bool"] = input => { return Converter.to_bool((string)input); },
            ["string:integer"] = input => { return Converter.to_int((string)input); },
            ["string:real"] = input => { return Converter.to_double((string)input); },
            // # from List<string> to                
            ["string_list:string"] = input => { return Converter.to_string((List<string>)input); },
            ["string_list:any"] = input => { return input; },
            ["string_list:trigger"] = input => { return null; },
            // # from List<real> to
            ["real_list:any"] = input => { return input; },
            ["real_list:trigger"] = input => { return null; },
            // # from List<decimal> to
            ["decimal_list:any"] = input => { return input; },
            ["decimal_list:trigger"] = input => { return null; },
            // # from leap motion frame
            ["lm_frame:any"] = input => { return input; },
            ["lm_frame:trigger"] = input => { return null; },
            // # from leap motion hands frame
            ["lm_hands_frame:any"] = input => { return input; },
            ["lm_hands_frame:trigger"] = input => { return null; },
            // from kinect body
            ["kinect_body:any"] = input => { return input; },
            ["kinect_body:trigger"] = input => { return null; },
            // # from image
            ["image:any"] = input => { return input; },
            ["image:trigger"] = input => { return null; },
            // # from plot
            ["plot:any"] = input => { return input; },
            ["plot:trigger"] = input => { return null; },
            // # from vector2
            ["vector2:any"] = input => { return input; },
            ["vector2:trigger"] = input => { return null; },
            ["vector2:string"] = input => { return Converter.to_string((Vector2)input); },
            // # from vector3
            ["vector3:any"] = input => { return input; },
            ["vector3:trigger"] = input => { return null; },
            ["vector3:string"] = input => { return Converter.to_string((Vector3)input); },
            // # from transform
            ["transform:any"] = input => { return input; },
            ["transform:trigger"] = input => { return null; },
            ["transform:string"] = input => { return Converter.to_string((TransformValue)input); },
            // # from id any
            ["id_any:any"] = input => { return input; },
            ["id_any:trigger"] = input => { return null; },
            // # from string any
            ["string_any:any"] = input => { return input; },
            ["string_any:trigger"] = input => { return null; },
            // # from keyboard button state
            ["keyboard_button:any"] = input => { return input; },
            ["keyboard_button:trigger"] = input => { return null; },
            // # from mouse button state
            ["mouse_button:any"] = input => { return input; },
            ["mouse_button:trigger"] = input => { return null; },
            // # from joypad button state
            ["joypad_button:any"] = input => { return input; },
            ["joypad_button:trigger"] = input => { return null; },
            // # from joypad axis state
            ["joypad_axis:any"] = input => { return input; },
            ["joypad_axis:trigger"] = input => { return null; },
            // # from GameObject list
            ["game_object_list:any"] = input => { return input; },
            ["game_object_list:trigger"] = input => { return null; }
        };

        public static Func<object, object> get(string convertorStr) {
            if (convertors.ContainsKey(convertorStr)) {
                return convertors[convertorStr];
            }
            ExVR.Log().error(string.Format("Convertor [{0}] doesn't exist.", convertorStr));
            return null;            
        }
    }
}
