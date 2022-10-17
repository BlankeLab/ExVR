
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
using System.Runtime.InteropServices;

namespace Ex{

    //public static class ObjectHandleExtensions{

    //    public static IntPtr ToIntPtr(this object target) {
    //        return GCHandle.Alloc(target).ToIntPtr();
    //    }

    //    public static GCHandle ToGcHandle(this object target) {
    //        return GCHandle.Alloc(target);
    //    }

    //    public static IntPtr ToIntPtr(this GCHandle target) {
    //        return GCHandle.ToIntPtr(target);
    //    }
    //}

    //public class GCHandleProvider : IDisposable{

    //    public GCHandleProvider(object target) {
    //        Handle = target.ToGcHandle();
    //    }

    //    public IntPtr Pointer => Handle.ToIntPtr();

    //    public GCHandle Handle { get; }

    //    private void ReleaseUnmanagedResources() {
    //        if (Handle.IsAllocated) {
    //            Handle.Free();
    //        }
    //    }

    //    public void Dispose() {
    //        ReleaseUnmanagedResources();
    //        GC.SuppressFinalize(this);
    //    }

    //    ~GCHandleProvider() {
    //        ReleaseUnmanagedResources();
    //    }
    //}


    /// <summary>
    /// Base class for creating C++ Dll import classes
    /// </summary>
    public abstract class DLLCppImport : IDisposable{

        #region members

        /// <summary>
        /// pointer to C+ dll class
        /// </summary>
        protected HandleRef _handle;

        // Track whether Dispose has been called.
        private bool disposed = false;

        #endregion members

        #region functions

        /// <summary>
        /// Allocate DLL memory
        /// </summary>
        abstract protected void create_DLL_class();

        /// <summary>
        /// Clean DLL memory
        /// </summary>
        abstract protected void delete_DLL_class();

        /// <summary>
        /// CppDLLImportBase default constructor
        /// </summary>
        public DLLCppImport() {
            create_DLL_class();
        }

        /// <summary>
        /// CppDLLImportBase constructor with an already allocated dll class
        /// </summary>
        /// <param name="ptr"></param>
        public DLLCppImport(IntPtr ptr) {
            _handle = new HandleRef(this, ptr);
        }

        // Use C# destructor syntax for finalization code.
        // This destructor will run only if the Dispose method
        // does not get called.
        // It gives your base class the opportunity to finalize.
        // Do not provide destructors in types derived from this class
        ~DLLCppImport() {
            // Do not re-create Dispose clean-up code here.
            // Calling Dispose(false) is optimal in terms of
            // readability and maintainability.
            Dispose(false);
        }

        // Implement IDisposable.
        // Do not make this method virtual.
        // A derived class should not be able to override this method.
        public void Dispose() {

            Dispose(true);
            // This object will be cleaned up by the Dispose method.
            // Therefore, you should call GC.SupressFinalize to
            // take this object off the finalization queue
            // and prevent finalization code for this object
            // from executing a second time.
            GC.SuppressFinalize(this);
        }

        // Dispose(bool disposing) executes in two distinct scenarios.
        // If disposing equals true, the method has been called directly
        // or indirectly by a user's code. Managed and unmanaged resources
        // can be disposed.
        // If disposing equals false, the method has been called by the
        // runtime from inside the finalizer and you should not reference
        // other objects. Only unmanaged resources can be disposed.
        protected virtual void Dispose(bool disposing) {

            // Check to see if Dispose has already been called.
            if (!this.disposed) {

                // If disposing equals true, dispose all managed
                // and unmanaged resources.
                if (disposing) {
                    // Dispose managed resources.
                    // ...
                }

                // cleanup unmanaged resources
                delete_DLL_class();
                _handle = new HandleRef(this, IntPtr.Zero);

                // Note disposing has been done.
                disposed = true;
            }
        }


        /// <summary>
        /// Return pointer to C++ DLL
        /// </summary>
        /// <returns></returns>
        public HandleRef getHandle() {
            return _handle;
        }

        #endregion functions
    }
}
