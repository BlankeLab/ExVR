
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
using System.Collections.Generic;

namespace Ex{

    public class PythonScriptComponent : CppComponent{


        protected override bool initialize() {

            // init DLL
            cppDll = new DLL.PythonScriptComponentDLL();
            cppDll.parent = this;

            // force catch exception for csharp scripts components
            catchExceptions = true;

            // slots
            add_slot("slot1", (arg) => {
                cppDll.call_slot(0, arg);
            });
            add_slot("slot2", (arg) => {
                cppDll.call_slot(1, arg);
            });
            add_slot("slot3", (arg) => {
                cppDll.call_slot(2, arg);
            });
            add_slot("slot4", (arg) => {
                cppDll.call_slot(3, arg);
            });
            // signals
            add_signal("signal1");
            add_signal("signal2");
            add_signal("signal3");
            add_signal("signal4");

            List<string> pathsPython = initC.get_list<string>("paths_python");
            // ...
     
            return cppDll.initialize();
        }
    }
}