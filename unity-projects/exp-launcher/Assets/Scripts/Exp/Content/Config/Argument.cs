
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
using System.ComponentModel;


namespace Ex{

    public class Argument{

        // add static converters        
        public bool update_from_xml(XML.Arg xmlArg) {

            // get type
            xml  = xmlArg;
            type = Type.GetType(xmlArg.Type);
            if(type == null) {
                ExVR.Log().error(string.Format("Can't read type [{0}] from argument.", xmlArg.Type));
                return false;
            }

            TypeConverter converter = TypeDescriptor.GetConverter(type);
            if (!converter.CanConvertFrom(typeof(string))) {
                return false;
            }

            switch (xmlArg.Dim) {
                case 0: {// one value
                        value = converter.ConvertFrom(xmlArg.Value);
                        return true;
                    }
                case 1: { // 1D array
                        if (xmlArg.Value.Length == 0) { 
                            value = new List<object>();
                            return true;
                        }
                        value = Converter.to_list(xmlArg.Value, xmlArg.Separator, converter);
                        var l = (List<object>)value;
                        if(l.Count != Converter.to_int(xmlArg.Sizes.Split(' ')[0])) {
                            value = new List<object>();
                            ExVR.Log().error(string.Format("List size invalid for arg: {0} -> {1} {2}", xmlArg.Name, l.Count, xmlArg.Sizes[0]));
                        }
                        return true;
                    }
                case 2: { // 2D array
                        if (xmlArg.Value.Length == 0) {
                            return false;
                        }

                        string[] sizes = xmlArg.Sizes.Split(' ');
                        if(sizes.Length < 2) {
                            ExVR.Log().error(string.Format("List 2D sizes invalid for argument with name {0} and value {1}", xml.Name, xml.Value));
                            return false;
                        }

                        int rows = Converter.to_int(sizes[0]);
                        int cols = Converter.to_int(sizes[1]);
                        value = Converter.to_2D_list(xmlArg.Value, xmlArg.Separator, rows, cols, converter);
                        if(value == null) {
                            value = new List<List<object>>();
                            ExVR.Log().error(string.Format("List 2D sizes invalid for argument with name {0} and value {1}", xml.Name, xml.Value));
                            return false;
                        }
                                               
                        return true;
                    }
                default:
                    break;
            }

            return false;
        }

        public object value = null;
        public Type type    = null;
        public XML.Arg xml  = null;
    }
}