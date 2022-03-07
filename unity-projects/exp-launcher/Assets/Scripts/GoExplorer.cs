
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

// unity
using UnityEngine;

namespace Ex{

    public class GoExplorer : MonoBehaviour{

        [Header("Settings")]
        [Rename("Gui")]
        public GameObject GuiSettings = null;
        [Rename("Network")]
        public GameObject NetworkSettings = null;
        [Rename("VR")]
        public GameObject VRSettings = null;
        [Rename("Events")]
        public GameObject EventsSettings = null;
        [Rename("Audio")]
        public GameObject AudioSettings = null;
        [Rename("Paths")]
        public GameObject PathsSettings = null;
        [Rename("Scripting")]
        public GameObject ScriptingSettings = null;
        [Rename("Log")]
        public GameObject LogSettings = null;

        [Header("Experiment content")]
        public GameObject Experiment = null;
        public GameObject Routines = null;
        public GameObject Components = null;
        public GameObject ISIs = null;
        public GameObject Connectors = null;

        [Header("Debug")]
        [Rename("Scene")]
        public GameObject DebugScene = null;
        [Rename("UI")]
        public GameObject DebugUI = null;
        [Rename("Editor camera")]
        public GameObject DebugEditorCamera = null;
        [Rename("Content")]
        public GameObject DebugContent = null;

        [Header("Global")]
        [Rename("Scene")]
        public GameObject GlobalScene = null;
        [Rename("Lights")]
        public GameObject GlobalLights = null;

        public GameObject instantiate(GameObject go, Transform parent = null, bool worldSpace = false) {
            return Instantiate(go, parent, worldSpace);
        }
    }
}