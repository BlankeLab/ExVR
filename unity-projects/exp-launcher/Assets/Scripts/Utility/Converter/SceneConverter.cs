
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
using System.Reflection;
using System.Collections.Generic;

// unity
using UnityEngine;
#if UNITY_EDITOR
using UnityEditor;

namespace Ex
{

    [CustomEditor(typeof(SceneConverter)), CanEditMultipleObjects]
    public class SceneConverterEditor : Editor
    {        
        SceneConverter t;
        SerializedObject m_targetObject;

        void OnEnable() {
            t = (SceneConverter)target;
            m_targetObject = new SerializedObject(t);
        }

        public void ClearLog() {
            var assembly = Assembly.GetAssembly(typeof(UnityEditor.Editor));
            var type = assembly.GetType("UnityEditor.LogEntries");
            var method = type.GetMethod("Clear");
            method.Invoke(new object(), null);
        }

        public override void OnInspectorGUI() {

            // Update our list
            m_targetObject.Update();

            // Resize our list
            EditorGUILayout.Space();

            t.destroyConvertor = GUILayout.Toggle(t.destroyConvertor, "Destroy convertor after");

            if (GUILayout.Button("CONVERT SCENE!")) {
                if (EditorUtility.DisplayDialog("Scene convertor",
                    "You should duplicate your current scene before using the feature.\n\n" +
                    "Every script from the list will be replaced by a mock one with corresponding parameters in the whole project.\n\n" +
                    "If ExVR load an asset bundle containing a mock script, it will replace it by recreating the original one and set the parameters" +
                    "before Awake call, the scripts need to be added in the resource manager in order to be compiled and instanced.", "Proceed", "Cancel")) {

                    ClearLog();
                    try {
                        t.convert(t.destroyConvertor);
                    } catch (Exception ex) {
                        Debug.LogError("Ex: " + ex.Message);
                    }
                    return;
                }
            }
            if (GUILayout.Button("CONVERT BACK!")) {
                try {
                    t.convert_back();
            } catch (Exception ex) {
                Debug.LogError("Ex: " + ex.Message);
            }
        }

            if (GUILayout.Button("DISPLAY HIERARCHY")) {

                HashSet<string> ignore = new HashSet<string>();
                ignore.Add("Ex.SceneConverter");

                HashSet<string> parsed = new HashSet<string>();

                Debug.Log("List of custom scripts found to be converted:");
                foreach (var component in t.gameObject.GetComponentsInChildren<Component>()) {
                    var type = component.GetType();
                    var t = type.ToString();

                    if (!ignore.Contains(t) &&
                    !t.Contains("UnityEngine.") &&
                    !t.Contains("TMPRO.") &&
                    !parsed.Contains(t)) {
                        parsed.Add(t);
                        Debug.Log(t);
                    }
                }
            }

            //Apply the changes to our list
            m_targetObject.ApplyModifiedProperties();
        }
    }
}
#endif

namespace Ex{


    [System.Serializable]
    public class SceneConverter : MonoBehaviour{

        public bool destroyConvertor = true;
        private HashSet<Type> types;

        public static object deserialize(ParameterToSet parameter, Assembly assembly = null) {
            return deserialize(parameter.to_sub(), assembly);
        }

        public static object deserialize(SubParameterToSet parameter, Assembly assembly = null) {

            // add reference to a generated component
            if (parameter.isDeletedComponent) {

                var type = Type.GetType(parameter.deletedComponentTypeName + ", " + parameter.fullAssemblyName);                
                if (type == null) {
                    type = Type.GetType(parameter.deletedComponentTypeName);
                    if (type == null) {
                        type = assembly.GetType(parameter.deletedComponentTypeName);
                        if (type == null) {
                            Debug.LogError("Type " + parameter.deletedComponentTypeName + " with assembly " + parameter.fullAssemblyName + " doesn't exists.");
                            return null;
                        }
                    }
                }

                var component = parameter.gameObject.GetComponent(type);
                if (component == null) {
                    Debug.LogError("GameObject " + parameter.gameObject.name + " doesn't have a component of type " + parameter.deletedComponentTypeName);
                    return null;
                }

                return component;
            }

            // add rerence to a non-generated component
            if (parameter.component != null) {
                return parameter.component; // MonoBehaviour
            }

            Type vType = Type.GetType(parameter.type + ", " + parameter.fullAssemblyName);
            if (vType == null) {
                vType = Type.GetType(parameter.type);
                if (vType == null) {
                    vType = assembly.GetType(parameter.type);
                    if (vType == null) {
                        Debug.LogError("Type " + parameter.type + " with assembly " + parameter.fullAssemblyName + " doesn't exist.");
                        return null;
                    }                    
                }                                
            }

            if (vType == typeof(GameObject)) {
                return parameter.gameObject;
            } else if (vType.IsSubclassOf(typeof(Component))) {
                return parameter.component;
            }

            // primitives
            if (vType == typeof(bool)) {
                return Converter.to_bool(parameter.strValue);
            } else if (vType == typeof(char)) {
                return Converter.to_char(parameter.strValue);
            } else if (vType == typeof(byte)) {
                return Converter.to_byte(parameter.strValue);
            } else if (vType == typeof(int)) {
                return Converter.to_int(parameter.strValue);
            } else if (vType == typeof(long)) {
                return Converter.to_long(parameter.strValue);
            } else if (vType == typeof(float)) {
                return Converter.to_float(parameter.strValue);
            } else if (vType == typeof(double)) {
                return Converter.to_double(parameter.strValue);
            } else if (vType == typeof(string)) {
                return parameter.strValue;
            } else if (vType.IsEnum) {
                return Enum.Parse(vType, parameter.strValue, true);
            } else {
                return Serializer.deserialize(parameter.strValue, vType);
            }
        }

        public ParameterToSet serialize_field(string name, Type vType, object value) {

            ParameterToSet parameter = new ParameterToSet();
            parameter.name = name;
            parameter.type = vType.ToString();
            parameter.fullAssemblyName = vType.Assembly.FullName;

            // convert primitives types
            if (vType == typeof(bool)) {
                parameter.strValue = Converter.to_string((bool)value);
            } else if (vType == typeof(char)) {
                parameter.strValue = Converter.to_string((char)value);
            } else if (vType == typeof(byte)) {
                parameter.strValue = Converter.to_string((byte)value);
            } else if (vType == typeof(int)) {
                parameter.strValue = Converter.to_string((int)value);
            } else if (vType == typeof(long)) {
                parameter.strValue = Converter.to_string((long)value);
            } else if (vType == typeof(float)) {
                parameter.strValue = Converter.to_string((float)value);
            } else if (vType == typeof(double)) {
                parameter.strValue = Converter.to_string((double)value);
            } else if (vType == typeof(string)) {
                parameter.strValue = (string)value;
            } else if (vType == typeof(GameObject)) {

                if ((GameObject)value == null) {
                    return null;
                }
                parameter.gameObject = (GameObject)value;
                parameter.strValue = parameter.gameObject.name;

            } else if (value is MonoBehaviour) {

                // check if component to be deleted                
                parameter.gameObject = ((MonoBehaviour)value).gameObject;
                foreach (var typeToDelete in types) { // check if type is from components we want to remove

                    if (parameter.type.ToString() == typeToDelete.ToString()) {
                        parameter.isDeletedComponent = true;
                        parameter.deletedComponentTypeName = parameter.type.ToString();
                        break;
                    }
                }

                if (!parameter.isDeletedComponent) {
                    parameter.mono = (MonoBehaviour)parameter.gameObject.GetComponent(parameter.type.ToString());
                }

            } else if (vType.IsEnum) {
                parameter.strValue = Convert.ToString(value);
            } else if (vType.IsSubclassOf(typeof(Component))) {
                parameter.component = (Component)value;
            } else {
                parameter.strValue = Serializer.serialize(value);
            }
            
            return parameter;
        }

        public void convert(bool destroyConvertor) {

            List<Component> componentsToReplace = new List<Component>();
            types = new HashSet<Type>();
            foreach (var childComponent in gameObject.GetComponentsInChildren<Component>(true)) {

                if (childComponent.gameObject == gameObject) {
                    continue;
                }
                var type = childComponent.GetType();
                if (type.ToString().Contains("UnityEngine.")) {
                    continue;
                }
                if (type.ToString().Contains("TMPro.")) {
                    continue;
                }
                if (type.ToString().Contains("Ex.SceneConverter")) {
                    continue;
                }
                if (!type.IsSubclassOf(typeof(MonoBehaviour))) {
                    continue;
                }
                componentsToReplace.Add(childComponent);
                types.Add(type);
            }

            const BindingFlags flags = BindingFlags.Public | BindingFlags.Instance;
            List<Component> componentsToDestroy = new List<Component>();

            // parse all components
            foreach (var component in componentsToReplace) {

                var type = component.GetType();

                // add or retrieve intantiate component
                var toInstantiate = component.GetComponent<ComponentsToInstantiate>();
                if (toInstantiate == null) {
                    toInstantiate = component.gameObject.AddComponent<ComponentsToInstantiate>();
                }

                ComponentParametersToSet componentParameters = new ComponentParametersToSet();
                componentParameters.name = type.ToString();
                componentParameters.assemblyFullName = type.Assembly.FullName;
                toInstantiate.components.Add(componentParameters);

                var fields = type.GetFields(flags);
                var c = component.gameObject.GetComponent(type);
                componentParameters.args = JsonUtility.ToJson(c);

                componentsToDestroy.Add(c);

                foreach (var field in fields) {

                    var value = field.GetValue(component);
                    if (value == null) {
                        Debug.Log("Cannot get value for field: " + field.Name + " in component " + componentParameters.name);
                        continue;
                    }

                    var vType = value.GetType();

                    // list
                    if (vType.IsGenericType && (vType.GetGenericTypeDefinition() == typeof(List<>))) {

                        System.Collections.ICollection list = field.GetValue(component) as System.Collections.ICollection;

                        ParameterToSet listParameter = new ParameterToSet();
                        listParameter.type = vType.ToString();
                        listParameter.name = field.Name;                        
                        listParameter.isList = true;
                        listParameter.subParameters = new List<SubParameterToSet>();

                        Type itemType = vType.GetGenericArguments()[0];
                        if(itemType != null) {
                            listParameter.fullAssemblyName = itemType.Assembly.FullName;
                        } else {
                            Debug.LogError("cannot get item type for list");
                        }

                        foreach (var v in list) {
                            var element = serialize_field("", v.GetType(), v);
                            if (element != null) {
                                listParameter.subParameters.Add(element.to_sub());
                            } else {
                                Debug.LogError("Cant add element to list " + field.Name);
                            }
                        }
                        componentParameters.parameters.Add(listParameter);
                        continue;
                    } else if (vType.IsArray) {

                        ParameterToSet arrayParameter = new ParameterToSet();
                        arrayParameter.type = vType.ToString();
                        arrayParameter.name = field.Name;
                        arrayParameter.isArray = true;
                        arrayParameter.subParameters = new List<SubParameterToSet>();
                        arrayParameter.fullAssemblyName = vType.Assembly.FullName;

                        Array array = (Array)value;
                        foreach (var v in array) {
                            var element = serialize_field("", v.GetType(), v);
                            if (element != null) {
                                arrayParameter.subParameters.Add(element.to_sub());
                            } else {
                                Debug.LogError("Cant add element to array " + field.Name);
                            }
                        }
                        componentParameters.parameters.Add(arrayParameter);
                        continue;
                    }

                    ParameterToSet parameter = serialize_field(field.Name, vType, value);
                    if (parameter != null) {
                        componentParameters.parameters.Add(parameter);
                    }
                }
            }

            foreach (var component in componentsToDestroy) {
                DestroyImmediate(component);
            }

            Debug.Log("End converting. You can create or update your prefab and create an asset bundle from it now. " +
                "AssetBundleComponent will be able to instantiate every custom component if they are added as C# script resources to the resource manager");

            if (destroyConvertor) {
                DestroyImmediate(gameObject.GetComponent<SceneConverter>());
            }
        }


        public void convert_back() {

            List<GameObject> gameObjectsToEnable = new List<GameObject>();
            var componentsToInstantiate = gameObject.GetComponentsInChildren<ComponentsToInstantiate>();
            List<Tuple<Type, ComponentParametersToSet, Component>> allComponents = new List<Tuple<Type, ComponentParametersToSet, Component>>();

            // generate components
            foreach (var componentToInstantiate in componentsToInstantiate) {

                var tr = componentToInstantiate.transform;

                // disable gameobject for avoiding Awake call just after component creation
                tr.gameObject.SetActive(false);
                gameObjectsToEnable.Add(tr.gameObject);

                // generate all components                
                foreach (var componentParameters in componentToInstantiate.components) {

                    // retrieve type
                    Type typeComponent = Type.GetType(componentParameters.name + "," + componentParameters.assemblyFullName);
                    if (typeComponent == null) {
                        Debug.LogError("Cannot instantiate type " + componentParameters.name);
                        continue;
                    }

                    // generate component                    
                    allComponents.Add(new Tuple<Type, ComponentParametersToSet, Component>(typeComponent, componentParameters, tr.gameObject.AddComponent(typeComponent)));
                }
            }


            //generate parameters for each component
            foreach (var component in allComponents) {

                var typeComponent = component.Item1;
                var parametersToSet = component.Item2;
                var generatedComponent = component.Item3;
                //JsonUtility.FromJsonOverwrite(parametersToSet.args, generatedComponent);

                // set parameters
                foreach (var parameter in parametersToSet.parameters) {

                    var varField = typeComponent.GetField(parameter.name);
                    if (varField == null) {
                        Debug.LogError("Variable " + parameter.name + " doesn't exist in component " + generatedComponent.name + " from GameObject " + generatedComponent.gameObject.name);
                        continue;
                    }

                    if (parameter.isArray) {
                                                
                        Type arrayType = Type.GetType(parameter.type + "," + parameter.fullAssemblyName);
                        Type elementType = arrayType.GetElementType();
                        if (elementType != null) {

                            var listType = typeof(List<>);
                            var constructedListType = listType.MakeGenericType(elementType);
                            var instance = Activator.CreateInstance(constructedListType);

                            var list = (System.Collections.IList)instance;
                            foreach (var subP in parameter.subParameters) {
                                var value = deserialize(subP);
                                if (value != null) {
                                    list.Add(value);
                                }
                            }

                            Array y = Array.CreateInstance(elementType, list.Count);
                            list.CopyTo(y, 0);
                            varField.SetValue(generatedComponent, y);
                        } else {
                            Debug.LogError("INVALID TYPE: " + parameter.type + " " + parameter.fullAssemblyName);
                        }

                    } else if (parameter.isList) {

                        
                        int startIndex = parameter.type.IndexOf('[') + 1;
                        int endIndex = parameter.type.LastIndexOf(']');
                        int length = endIndex - startIndex;
                        parameter.type = parameter.type.Substring(startIndex, length);

                        Type elementType = Type.GetType(parameter.type + ", " + parameter.fullAssemblyName);
                        if (elementType != null) {

                            var listType = typeof(List<>);
                            var constructedListType = listType.MakeGenericType(elementType);
                            var instance = Activator.CreateInstance(constructedListType);

                            var list = (System.Collections.IList)instance;
                            foreach (var subP in parameter.subParameters) {
                                var value = deserialize(subP);
                                if (value != null) {
                                    list.Add(value);
                                }
                            }
                            varField.SetValue(generatedComponent, list);
                        } else {
                            Debug.LogError("INVALID TYPE: " + parameter.type +" " + parameter.fullAssemblyName);
                        }

                    } else {
                        object value = deserialize(parameter);
                        if (value != null) {                            
                            varField.SetValue(generatedComponent, value);
                        } else {
                            Debug.LogError("CANNOT SET");
                        }
                    }
                }
            }


            // enable again associated gameobject to allow Awake call
            foreach (var go in gameObjectsToEnable) {
                DestroyImmediate(go.GetComponent<ComponentsToInstantiate>());
                go.SetActive(true);
            }
        }
    }
}

// https://answers.unity.com/questions/8633/how-do-i-programmatically-assign-a-gameobject-to-a.html