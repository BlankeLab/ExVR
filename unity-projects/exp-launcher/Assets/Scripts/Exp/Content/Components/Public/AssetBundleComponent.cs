
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
using System.Text;
using System.Collections.Generic;
using System.Reflection;
// unity
using UnityEngine;


namespace Ex{

    public class AssetBundleComponent : ExComponent {

        public GameObject bundle = null;
        private List<string> assembliesNames = null;

        #region ex_functions
        protected override bool initialize() {

            // slots
            add_slot("visibility", (visibility) => {set_visibility((bool)visibility);});
            add_slot("position", (position) => { set_position((Vector3)position); });
            add_slot("rotation", (rotation) => { set_rotation((Vector3)rotation); });
            add_slot("scale", (scale) => { set_scale((Vector3)scale); });
            add_slot("transform", (value) => {
                var transformV = (TransformValue)value;
                set_position(transformV.position);
                set_rotation(transformV.rotation);
                set_scale(transformV.scale);
            });
            // signals
            add_signal("visibility changed");

            string aliasAssetBundle = initC.get_resource_alias("asset_bundle");
            string nameSubObject    = initC.get<string>("name_sub_object");

            if(aliasAssetBundle.Length == 0) {
                log_error("No asset bundle alias defined.");
                return false;
            }

            bundle = ExVR.Resources().instantiate_asset_bundle(aliasAssetBundle, nameSubObject, null);
            if(bundle == null) {
                return false;
            }

            if (initC.get<bool>("display_hierarchy")) {
                StringBuilder sb = new StringBuilder();
                foreach (var tr in bundle.GetComponentsInChildren<Transform>()) {
                    int depth = GO.get_depth_level(tr);
                    sb.AppendFormat("{0}/{1} {2}\n", new String(' ', depth*5), tr.name, components_info(tr));
                }
                send_infos_to_gui_init_config("hierarchy", sb.ToString());
            } else {
                send_infos_to_gui_init_config("hierarchy", "");
            }

            bundle.transform.SetParent(transform);  

            instantiate_sub_components();

            return true;
        }

        protected override void start_experiment() {
            if (!initC.get<bool>("init_transform_do_not_apply")) {
                initC.update_transform("init_transform", transform, true);
            }
        }

        protected override void set_visibility(bool visibility) {
            bundle.SetActive(visibility);
            invoke_signal("visibility changed", visibility);
        }

        public override void update_from_current_config() {
            reset_config_transform();
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }


        #endregion
        #region private_fuctions

        private void instantiate_sub_components() {

            // keep deleted components references to be added after when every component would have been regenerated
            List<GameObject> gameObjectsToEnable = new List<GameObject>();
            List<Tuple<UnityEngine.Component, FieldInfo, GameObject, string>> deletedComponentsToAdd = new List<Tuple<UnityEngine.Component, FieldInfo, GameObject, string>>();

            // init assemblies paths
            assembliesNames = new List<string>();
            assembliesNames.Add("");
            assembliesNames.Add(", UnityEngine");
            assembliesNames.Add(", UnityEngine.UI");
            assembliesNames.Add(", UnityEngine.Image");
            assembliesNames.Add(", System");
            assembliesNames.Add(", System.Collections");
            assembliesNames.Add(", System.Collections.Generic");

            List<Tuple<Type, ComponentParametersToSet, Component>> allComponents = new List<Tuple<Type, ComponentParametersToSet, Component>>();
            foreach (var componentToInstantiate in bundle.GetComponentsInChildren<ComponentsToInstantiate>()) {

                var tr = componentToInstantiate.transform;

                // disable gameobject for avoiding Awake call just after component creation
                tr.gameObject.SetActive(false);
                gameObjectsToEnable.Add(tr.gameObject);

                // generate all components                
                foreach (var componentParameters in componentToInstantiate.components) {

                    Type typeComponent = Type.GetType(componentParameters.name);
                    if (typeComponent == null) {
                        typeComponent = Type.GetType(componentParameters.name + "," + componentParameters.assemblyFullName);
                        if (typeComponent == null) {

                            if (CSharpScriptResource.get_compiled_assembly() == null) {
                                ExVR.Log().error("No compiled assembly.");
                                continue;
                            }
                            typeComponent = CSharpScriptResource.get_type_from_compiled_assembly(componentParameters.name);
                            if (typeComponent == null) {
                                ExVR.Log().error(string.Format("Cannot instantiate type [{0}]", componentParameters.name));
                                continue;
                            }
                        }
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

                // set parameters
                foreach (var parameter in parametersToSet.parameters) {

                    var varField = typeComponent.GetField(parameter.name);
                    if (varField == null) {
                        ExVR.Log().error(string.Format("Variable [{0}] doesn't exist in component [{1}] from GameObject [{2}]", 
                            parameter.name, generatedComponent.name, generatedComponent.gameObject.name));
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
                                var value = SceneConverter.deserialize(subP, CSharpScriptResource.get_compiled_assembly());
                                if (value != null) {
                                    list.Add(value);
                                }
                            }

                            Array y = Array.CreateInstance(elementType, list.Count);
                            list.CopyTo(y, 0);
                            varField.SetValue(generatedComponent, y);
                        } else {
                            ExVR.Log().error("INVALID TYPE: " + parameter.type + " " + parameter.fullAssemblyName);
                        }

                    } else if (parameter.isList) {


                        int startIndex = parameter.type.IndexOf('[') + 1;
                        int endIndex = parameter.type.LastIndexOf(']');
                        int length = endIndex - startIndex;
                        parameter.type = parameter.type.Substring(startIndex, length);

                        Type elementType = Type.GetType(string.Concat(parameter.type, ", ",parameter.fullAssemblyName));
                        if (elementType != null) {

                            var listType = typeof(List<>);
                            var constructedListType = listType.MakeGenericType(elementType);
                            var instance = Activator.CreateInstance(constructedListType);

                            var list = (System.Collections.IList)instance;
                            foreach (var subP in parameter.subParameters) {
                                var value = SceneConverter.deserialize(subP, CSharpScriptResource.get_compiled_assembly());
                                if (value != null) {
                                    list.Add(value);
                                }
                            }
                            varField.SetValue(generatedComponent, list);
                        } else {
                            ExVR.Log().error("INVALID TYPE: " + parameter.type + " " + parameter.fullAssemblyName);
                        }

                    } else {
                        object value = SceneConverter.deserialize(parameter, CSharpScriptResource.get_compiled_assembly());
                        if (value != null) {
                            varField.SetValue(generatedComponent, value);
                        } else {
                            ExVR.Log().error("CANNOT SET");
                        }
                    }
                }
            }

            // enable again associated gameobject to allow Awake call
            foreach (var go in gameObjectsToEnable) {
                Destroy(go.GetComponent<ComponentsToInstantiate>());
                go.SetActive(true);
            }
        }

        private string components_info(Transform tr) {

            StringBuilder sb = new StringBuilder();
            foreach (var component in tr.GetComponents<Component>()) {
                var type = component.GetType();
                if (component != this && type != typeof(Transform)) {
                    sb.AppendFormat("[{0}]", type.ToString().Replace("UnityEngine.", ""));
                }
            }
            return sb.ToString();
        }

        #endregion
    }

}




//private Type get_type_from_str(string strType) {
//    Type vType = null;
//    foreach (var aName in assembliesNames) {
//        vType = Type.GetType(strType + aName);
//        if (vType != null) {
//            break;
//        }
//    }

//    // check from compiled assembly
//    if (vType == null) {
//        vType = Type.GetType(strType + ", " + CSharpScriptData.get_compiled_assembly().FullName);
//    }

//    return vType;
//}

//private object deserialize(ParameterToSet parameter) {
//    return base_deserialize(parameter.to_sub());
//}

//private object deserialize(SubParameterToSet parameter) {
//    return base_deserialize(parameter);
//}

//private object base_deserialize(SubParameterToSet parameter) {


//    // add reference to a generated component
//    if (parameter.isDeletedComponent) {

//        var type = Type.GetType(parameter.deletedComponentTypeName + ", " + CSharpScriptData.get_compiled_assembly().FullName);
//        if (type == null) {
//            ExVR.Log().error("Type " + parameter.deletedComponentTypeName + " doesn't exists.");
//            return null;
//        }

//        var component = parameter.gameObject.GetComponent(type);
//        if (component == null) {
//            ExVR.Log().error("GameObject " + parameter.gameObject.name + " doesn't have a component of type " + parameter.deletedComponentTypeName);
//            return null;
//        }

//        return component;
//    }

//    // add rerence to a non-generated component
//    if (parameter.component != null) {
//        return parameter.component; // MonoBehaviour
//    }

//    Type vType = get_type_from_str(parameter.type);
//    if (vType == null) {
//        ExVR.Log().error("Type " + parameter.type + " doesn't exist.");
//        return null;
//    }

//    if (vType == typeof(GameObject)) {
//        return parameter.gameObject;
//    } else if (vType == typeof(Transform)) {
//        return parameter.transform;
//    }

//    // primitives
//    object value = null;
//    if (vType == typeof(bool)) {
//        value = Converter.to_bool(parameter.strValue);
//    } else if (vType == typeof(char)) {
//        value = Converter.to_char(parameter.strValue);
//    } else if (vType == typeof(byte)) {
//        //value = Converter.to_byte(parameter.strValue);
//    } else if (vType == typeof(int)) {
//        value = Converter.to_int(parameter.strValue);
//    } else if (vType == typeof(long)) {
//        //value = Converter.to_long(parameter.strValue);
//    } else if (vType == typeof(float)) {                
//        value = Converter.to_float(parameter.strValue);
//        Debug.Log("parameter.strValue to float: " + parameter.strValue + " " + value);
//    } else if (vType == typeof(double)) {
//        value = Converter.to_double(parameter.strValue);
//    } else if (vType == typeof(string)) {
//        value = parameter.strValue;
//    } else if (vType.IsEnum) {
//        value = Enum.Parse(vType, parameter.strValue, true);
//    } else {
//        value = Serializer.deserialize(parameter.strValue, vType);
//    }

//    if (value == null) {
//        ExVR.Log().error("Type " + vType.ToString() + " not managed.");
//    }

//    return value;
//}

