
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
using System.Linq;

namespace Ex {

    public static class Text {

        public static readonly string[] linesSep = { "\r\n", "\r", "\n" };
        public static readonly char[] tabSep = { '\t' };

        public static string new_line() {
            return linesSep[2];
        }

        public static string join(IEnumerable<string> text) {
            return string.Join("", text);
        }

        public static string join_with_space(IEnumerable<string> text) {
            return string.Join(" ", text);
        }

        public static string join_with_new_line(IEnumerable<string> text) {
            return string.Join("\n", text);
        }

        public static string[] split_lines(string text, bool removeEmptyLines = true) {
            return text.Split(linesSep, removeEmptyLines ? StringSplitOptions.RemoveEmptyEntries : StringSplitOptions.None);
        }

        public static string[] split_tabs(string text, bool removeEmptyLines = true) {
            return text.Split(tabSep, removeEmptyLines ? StringSplitOptions.RemoveEmptyEntries : StringSplitOptions.None);
        }

        public static string[] split(string text, char sep) {
            return text.Split(sep);
        }

        public static string[] split(string text, string sep, bool removeEmptyLines = true) {
            return text.Split(new string[] {sep}, removeEmptyLines ? StringSplitOptions.RemoveEmptyEntries : StringSplitOptions.None);
        }

        public static string[] split(string text, char[] sep, bool removeEmptyLines = true) {
            return text.Split(sep, removeEmptyLines ? StringSplitOptions.RemoveEmptyEntries : StringSplitOptions.None);
        }

        public static string[] split(string text, string[] sep, bool removeEmptyLines = true) {
            return text.Split(sep, removeEmptyLines ? StringSplitOptions.RemoveEmptyEntries : StringSplitOptions.None);
        }
    }
}