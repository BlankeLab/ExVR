
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
using System.Text;
using System.Collections.Generic;
using System.Reflection;

namespace Ex {

    public class CSharpFunctionComponent : ExComponent {

        private static readonly string[] charsToRemove = new string[] { "@", ",", ".", ";", "'", "{", "}", "(", ")", "[", "]", " ", "-" };
        private Dictionary<string, MethodInfo> m_methods = null;
        private MethodInfo m_currentMethod = null;

        #region ex_functions
        protected override bool initialize() {

            // connections
            connections().add_slot("input", (input) => {
                invoke_signal("output", call_function(input));
            });
            connections().add_signal("output");


            string className = name;
            foreach (var c in charsToRemove) {
                className = className.Replace(c, "_");
            }            

            // retrieve type
            var assembly = CSharpScriptResource.get_compiled_assembly();
            if (assembly == null) {
                log_error(string.Concat("No assembly available, cannot instantiate class Ex.CSharpFunctions.", className));
                return false;
            }


            var flagPublic = System.Reflection.BindingFlags.Static | System.Reflection.BindingFlags.Public;
            m_methods = new Dictionary<string, MethodInfo>();
            foreach (var config in configs) {

                string configName = config.name;
                foreach (var c in charsToRemove) {
                    configName = configName.Replace(c, "_");
                }

                // get nested class using '+'
                var configClassName = string.Format("Ex.CSharpFunctions.{0}+{1}", className, configName);
                var runtimeType = assembly.GetType(configClassName);
                if (runtimeType == null) {
                    log_error(string.Concat("Cannot instantiate class ", configClassName));
                    return false;
                }

                var method = runtimeType.GetMethod("function", flagPublic);
                m_methods[config.name] = method;
            }

            return true;
        }

        protected override void start_routine() {
            m_currentMethod = m_methods[currentC.name];
        }

        protected override void stop_routine() {
            m_currentMethod = null;
        }

        #endregion

        #region private_functions

        private static string generate_code(string className, string initConfigExtraContent, List<string> configsName, List<string> condConfigsFunctionContent, List<string> condConfigsExtraContent) {
            

            StringBuilder b = new StringBuilder();
            b.Append("using System;\nusing System.Collections.Generic;\nusing UnityEngine;\n");
            b.Append(" namespace Ex.CSharpFunctions { public class ");
            b.Append(className);
            b.Append(" { \nstatic int countErrors = 0;");
            b.Append(initConfigExtraContent);
            for (int ii = 0; ii < configsName.Count; ++ii) {
                b.Append("public class ");
                b.Append(configsName[ii]);
                b.Append("{\n");
                b.Append(condConfigsExtraContent[ii]);
                b.Append("\n public static object function(object input) {\nobject output = null;\ntry{");
                b.Append(condConfigsFunctionContent[ii]);
                b.Append("\n}catch(System.Exception ex){\nCSharpFunctionComponent.display_static_exception(countErrors++,ex);}\nreturn output;\n}\n}");
            }
            b.Append("\n }\n}");
            return b.ToString();
        }

        private static List<string> generate_temp_files_from_source_batch(string baseName, List<string> sources) {

            List<string> fileNames = new List<string>(sources.Count);
            for (int ii = 0; ii < sources.Count; ++ii) {
                fileNames.Add(ExVR.Paths().expLauncherTempGeneratedDir + "/" + baseName + "_" + ii + ".cs");
                if (File.Exists(fileNames[ii])) {
                    File.Delete(fileNames[ii]);
                }
                FileStream f = new FileStream(fileNames[ii], FileMode.CreateNew);
                using (StreamWriter s = new StreamWriter(f, Encoding.UTF8)) {
                    s.Write(sources[ii]);
                    s.Close();
                }
                f.Close();
            }

            return fileNames;
        }

        public static List<string> generate_files_from_scripts_functions(XML.Components componentsXML) {

            List<string> csharpFunctionsComponentsCode = new List<string>();
            foreach (var component in componentsXML.Component) {
                if (component.Type == "CSharpFunction") {

                    string initConfigExtraContent = "";
                    List<string> configs = new List<string>();
                    List<string> condConfigsFuncContent = new List<string>();
                    List<string> condConfigsExtraContent = new List<string>();

                    foreach (var arg in component.InitConfig.Arg) {
                        if (arg.Name == "extra") {
                            initConfigExtraContent = arg.Value;
                            break;
                        }
                    }

                    foreach (var config in component.Configs.Config) {

                        string configName = config.Name;
                        foreach (var c in charsToRemove) {
                            configName = configName.Replace(c, "_");
                        }

                        configs.Add(configName);
                        foreach (var arg in config.Arg) {
                            if (arg.Name == "function") {
                                condConfigsFuncContent.Add(arg.Value);
                                continue;
                            }
                            if (arg.Name == "extra") {
                                condConfigsExtraContent.Add(arg.Value);
                                continue;
                            }
                        }
                    }

                    var componentName = component.Name;
                    foreach (var c in charsToRemove) {
                        componentName = componentName.Replace(c, "_");
                    }

                    csharpFunctionsComponentsCode.Add(generate_code(componentName, initConfigExtraContent, configs, condConfigsFuncContent, condConfigsExtraContent)); ;
                }
            }

            return generate_temp_files_from_source_batch("csharpFunctions", csharpFunctionsComponentsCode);
        }

        #endregion

        #region public_functions
        public static void display_static_exception(int countErrors, System.Exception e,
            [System.Runtime.CompilerServices.CallerMemberName] string memberName = "",
            [System.Runtime.CompilerServices.CallerFilePath] string sourceFilePath = "",
            [System.Runtime.CompilerServices.CallerLineNumber] int sourceLineNumber = 0) {

            if (countErrors <= 10) {
                ExVR.Log().error(string.Format("[error-exception] "), true, memberName, sourceFilePath, sourceLineNumber);
                ExVR.Log().error(string.Format("[message] {0}", e.Message), false);
                if(countErrors == 10) {
                    ExVR.Log().error("Too many errors, exception display disabled. Please fix issues and reload experiment.");
                }
            }
        }

        public object call_function(object value) {
            if (m_currentMethod != null) {
                return m_currentMethod.Invoke(null, new object[1] { value });
            }
            return null;
        }

        #endregion
    }
}

