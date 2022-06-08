
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

        public object value = null;
        public Type type = null;
        public XML.Arg xml = null;

        // add static converters        

        private object convert(TypeConverter converter, string strValue) {            
            try {
                return converter.ConvertFrom(strValue);
            } catch (NotSupportedException ex) {
                ExVR.Log().error(string.Format("Cannot perform convertion of argument with name [{0}] of type [{1}] with value [{2}], error with message: [{3}]",
                    xml.Type, xml.Name, strValue, ex.Message));
                return null;
            }
        }

        private bool convert_to_value(TypeConverter converter) {
            value = convert(converter, xml.Value);
            return value != null;
        }

        private bool convert_to_1d_array(TypeConverter converter) {

            if (xml.Value.Length != 0) {

                string[] splitArg = Ex.Text.split(xml.Value, xml.Separator, false);

                List<object> argList1D = new List<object>(splitArg.Length);
                foreach (string split in splitArg) {

                    var arrayValue = convert(converter, split);
                    if(arrayValue != null) {
                        argList1D.Add(arrayValue);
                    } else {
                        ExVR.Log().error(string.Format("Invalid 1D array value for from arg with name [{0}] and type [{1}]",
                            xml.Name, xml.Type));
                        return false;
                    }
                }

                string[] sizes = Ex.Text.split(xml.Sizes, ' ');
                if (argList1D.Count == Converter.to_int(sizes[0])) {
                    value = argList1D;
                } else {
                    value = new List<object>();
                    ExVR.Log().error(string.Format("List size inconsistency for arg with name [{0}] -> [{1} {2}]",
                        xml.Name, argList1D.Count, sizes[0]));
                    return false;                    
                }

            } else {
                value = new List<object>();
            }
            return true;
        }

        private bool convert_to_2d_array(TypeConverter converter) {

            string[] sizes = Ex.Text.split(xml.Sizes, ' ');
            if (sizes.Length < 2) {
                ExVR.Log().error(string.Format("List 2D sizes invalid for argument with name [{0}] and value [{1}]",
                    xml.Name, xml.Value));
                return false;
            }

            if (xml.Value.Length != 0) {

                int rows = Converter.to_int(sizes[0]);
                int cols = Converter.to_int(sizes[1]);

                string[] splitArg = Ex.Text.split(xml.Value, xml.Separator, false);
                List<List<object>> argList2D = new List<List<object>>(rows);
                if (splitArg.Length != rows * cols) {
                    ExVR.Log().error(string.Format("List 2D sizes invalid for argument with name {0} and value {1}", xml.Name, xml.Value));
                    return false;
                }

                int id = 0;
                for (int ii = 0; ii < rows; ++ii) {
                    argList2D.Add(new List<object>(cols));
                    for (int jj = 0; jj < cols; ++jj) {     

                        var arrayValue = converter.ConvertFrom(splitArg[id++]);
                        if (arrayValue != null) {
                            argList2D[ii].Add(arrayValue);
                        } else {
                            ExVR.Log().error(string.Format("Invalid 2D array value for from arg with name [{0}] and type [{1}]",
                                xml.Name, xml.Type));
                            return false;
                        }
                    }
                }

                value = argList2D;

            } else {
                value = new List<List<object>>();
            }

            return true;
        }

        public bool update_from_xml(XML.Arg xmlArg) {

            // get type
            xml  = xmlArg;
            type = Type.GetType(xmlArg.Type);
            if(type == null) {
                ExVR.Log().error(string.Format("Can't read type [{0}] from argument with name [{1}].", xmlArg.Type, xmlArg.Name));
                return false;
            }

            // get converter
            TypeConverter converter = TypeDescriptor.GetConverter(type);
            if (!converter.CanConvertFrom(typeof(string))) {
                return false;
            }

            if(xmlArg.Dim == 0) { // one value

                return convert_to_value(converter);

            } else if(xmlArg.Dim == 1) { // 1D array

                return convert_to_1d_array(converter);

            } else if(xmlArg.Dim == 2) { // 2D array

                return convert_to_2d_array(converter);

            } else {
                ExVR.Log().error(string.Format("Cannot convert arg with name [{0}] and value [{1}], nb of dimensions [{2}] not managed. ", xmlArg.Name, xmlArg.Value, xmlArg.Dim));
                return false;
            }
        }
    }
}
