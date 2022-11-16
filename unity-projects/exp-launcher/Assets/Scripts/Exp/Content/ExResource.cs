
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
using System.IO;
using System.Runtime.CompilerServices;

// unity
using UnityEngine;

namespace Ex {

    public class ExResource : MonoBehaviour {

        public int key;
        public string alias;
        public string path;
        public bool doNotRemove = false;

        public virtual void create(int key, string alias, string path) {
            this.key = key;
            this.alias = alias;
            this.path = path;
        }

        public virtual bool read_data() {
            return true;
        }

        public virtual bool initialize() {
            return true;
        }

        public virtual void clean() {
        }

        public string verbose_name() {
            return string.Format(" [From resource of type {0} with alias {1}]", this.GetType().ToString(), alias);
        }

        public void log_message(object message, bool verbose = false, bool extra = false,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0) {
            if (verbose) {
                ExVR.Log().message(string.Concat(Converter.to_string(message), verbose_name()), extra, memberName, sourceFilePath, sourceLineNumber);
            } else {
                ExVR.Log().message(message, extra, memberName, sourceFilePath, sourceLineNumber);
            }
        }

        public void log_warning(object warning, bool verbose = true, bool extra = false,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0) {
            if (verbose) {
                ExVR.Log().warning(string.Concat(Converter.to_string(warning), verbose_name()), extra, memberName, sourceFilePath, sourceLineNumber);
            } else {
                ExVR.Log().warning(warning, extra, memberName, sourceFilePath, sourceLineNumber);
            }
        }

        public void log_error(object error, bool verbose = true, bool extra = true,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0) {
            if (verbose) {
                ExVR.Log().error(string.Concat(Converter.to_string(error), verbose_name()), extra, memberName, sourceFilePath, sourceLineNumber);
            } else {
                ExVR.Log().error(error, extra, memberName, sourceFilePath, sourceLineNumber);
            }
        }
    }
    public class ExResourceFile : ExResource {

        public string extension;

        public override void create(int key, string alias, string path) {
            base.create(key, alias, path);
            this.extension = Path.GetExtension(path);
        }
    }
}