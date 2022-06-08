
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

// unity
using UnityEngine;

namespace Ex
{

    [System.Serializable]
    public class SubParameterToSet
    {
        public string name = "";
        public string type = "";
        public string fullAssemblyName = "";
        public string strValue = "";

        public MonoBehaviour mono = null;
        public GameObject gameObject = null;
        public Component component = null;

        public bool isDeletedComponent = false;
        public string deletedComponentTypeName = "";
    }

    [System.Serializable]
    public class ParameterToSet
    {

        public string name = "";
        public string type = "";
        public string fullAssemblyName = "";
        public string strValue = "";

        public MonoBehaviour mono = null;
        public GameObject gameObject = null;
        public Component component = null;

        public bool isDeletedComponent = false;
        public string deletedComponentTypeName = "";

        public bool isArray = false;
        public bool isList = false;
        public List<SubParameterToSet> subParameters = null;

        public SubParameterToSet to_sub() {
            SubParameterToSet sub = new SubParameterToSet();
            sub.name = name;
            sub.type = type;
            sub.fullAssemblyName = fullAssemblyName;
            sub.strValue = strValue;
            sub.mono = mono;
            sub.gameObject = gameObject;
            sub.component = component;
            sub.isDeletedComponent = isDeletedComponent;
            sub.deletedComponentTypeName = deletedComponentTypeName;
            return sub;
        }
    }

    [System.Serializable]
    public class ComponentParametersToSet
    {
        public string name;
        public string assemblyFullName;
        public string args;
        public List<ParameterToSet> parameters = new List<ParameterToSet>();
    }

    [System.Serializable]
    public class ComponentsToInstantiate : MonoBehaviour
    {
        public List<ComponentParametersToSet> components = new List<ComponentParametersToSet>();
    }
}