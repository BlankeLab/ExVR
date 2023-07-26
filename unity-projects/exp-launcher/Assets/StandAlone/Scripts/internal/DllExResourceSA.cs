
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
using System.Runtime.InteropServices;

namespace SA {


    public abstract class DLLExResourceSA : DLLExElementSA {

        // parent component
        public CppExResourceFileSA parent = null;

        // dll management
        abstract protected override void create_DLL_class();
        protected override void delete_DLL_class() { delete_ex_resource(_handle); }

        // once per loading
        public virtual bool initialize() {
            
            set_exp_ex_element(_handle, DLLExperimentSA.global.getHandle());
            set(ParametersSA.Container.Global, "resource_key", parent.key);
            return initialize_ex_resource(_handle) == 1 ? true : false;
        }

        public virtual void clean() {
            clean_ex_resource(_handle);
            Dispose();
        }

        [DllImport("exvr-export", EntryPoint = "delete_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern void delete_ex_resource(HandleRef exResource);

        [DllImport("exvr-export", EntryPoint = "initialize_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern int initialize_ex_resource(HandleRef exResource);

        [DllImport("exvr-export", EntryPoint = "clean_ex_resource", CallingConvention = CallingConvention.Cdecl)]
        static public extern void clean_ex_resource(HandleRef exResource);

    }
}
