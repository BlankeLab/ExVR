
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

// unity
using UnityEngine;

namespace Ex{

    public class PathsManager : MonoBehaviour{

        public string expLauncherExeFile;
        public string expLauncherMainDir;
        public string expLauncherDataDir;
        public string expLauncherMonoDir;
        public string expLauncherTempGeneratedDir;

        public string designerMainDir;
        public string designerDataTempDir;
        public string designerTempExpFile;
        public string designerDefaultInstanceFile;
        public string designerLogDir;


        public string currentExperimentFile = "";
        public string currentInstanceFile   = "";

        public void initialize() {

            expLauncherDataDir = Application.dataPath;
            var platform = Application.platform;
            if (platform == RuntimePlatform.WindowsPlayer || platform == RuntimePlatform.WindowsEditor) {
                expLauncherMainDir = expLauncherDataDir + "/..";
                expLauncherExeFile = expLauncherMainDir + "/ExVR-exp.exe";
            } else if (platform == RuntimePlatform.LinuxPlayer) {
                //expLauncherMainDir = ...; 
                //expLauncherExeFile = ...;
            }
            else if (platform == RuntimePlatform.OSXPlayer) {
                //expLauncherMainDir = ...;
                //expLauncherExeFile = ...;
            }
            expLauncherMonoDir = expLauncherMainDir + "/mono-scripting";
            expLauncherTempGeneratedDir = expLauncherMainDir + "/temp_generated";
            if (!Directory.Exists(expLauncherTempGeneratedDir)) {
                Directory.CreateDirectory(expLauncherTempGeneratedDir);
            }

#if UNITY_EDITOR
            update_designer_path(string.Format("{0}/../../cpp-projects/_build/bin/exvr-designer", expLauncherMainDir));
#else
            update_designer_path(expLauncherMainDir + "/..");
#endif
        }

        public void update_designer_path(string designerMainDir) {

            this.designerMainDir            = designerMainDir;
            designerDataTempDir             = string.Format("{0}/data/temp", designerMainDir);
            designerTempExpFile             = string.Format("{0}/data/temp/exp.xml", designerMainDir);
            designerLogDir                  = string.Format("{0}/logs", designerMainDir);
            designerDefaultInstanceFile     = string.Format("{0}/data/temp/debug-instance.xml", designerMainDir);

            if (!File.Exists(designerTempExpFile)) {
                designerTempExpFile = expLauncherMainDir + "/exp.xml";
                designerDefaultInstanceFile = expLauncherMainDir + "/debug-instance.xml";
            }
        }
    }
}