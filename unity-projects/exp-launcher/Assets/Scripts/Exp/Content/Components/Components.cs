﻿
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
using System.Collections.Generic;

// unity
using UnityEngine;

using static Ex.ExComponent;


namespace Ex{

    //public class ComponentInfo{
    //    public ComponentInfo(Category category, Priority priority, Reserved reserved, bool catchExceptions) {
    //        this.category = category;
    //        this.priority = priority;
    //        this.reserved = reserved;
    //        this.catchExceptions = catchExceptions;
    //    }
    //    public Category category;
    //    public Priority priority;
    //    public Reserved reserved;
    //    public bool catchExceptions;
    //}

    public class Components : MonoBehaviour{
        
        public static readonly Dictionary<Category, string> Category2Transform = new Dictionary<Category, string> {
            [Category.Audio] = "[C:Audio]",
            [Category.Resource] = "[C:Resource]",
            [Category.Flow] = "[C:Flow]",
            [Category.InputDevice] = "[C:InputDevice]",
            [Category.Network] = "[C:Network]",
            [Category.Output] = "[C:Output]",
            [Category.Scene] = "[C:Scene]",
            [Category.Script] = "[C:Script]",
            [Category.Environment] = "[C:Environment]",
            [Category.UserInterface] = "[C:UserInterface]",
            [Category.Video] = "[C:Video]",
            [Category.AcquisitionDevice] = "[C:AcquisitionDevice]",
            [Category.Text] = "[C:Text]",
            [Category.Model] = "[C:Model]",
            [Category.Avatar] = "[C:Avatar]",
            [Category.Interaction] = "[C:Interaction]",
            [Category.Camera] = "[C:Camera]",
            [Category.Settings] = "[C:Settings]",
            [Category.Viewer] = "[C:Viewer]"
        };

        // sort by priority
        [SerializeField]
        private List<ExComponent> sortedComponents = new List<ExComponent>();
        private List<ExComponent> reverseSortedComponents = new List<ExComponent>();
        public Dictionary<Type, List<ExComponent>> componentsPerType = new Dictionary<Type, List<ExComponent>>();
        public Dictionary<Category, List<ExComponent>> componentsPerCategory = new Dictionary<Category, List<ExComponent>>();
        public Dictionary<int, ExComponent> componentPerKey = new Dictionary<int, ExComponent>();
        public Dictionary<string, ExComponent> componentPerName = new Dictionary<string, ExComponent>();

        public bool generate(XML.Components xmlComponents) {

            // create gameobjects and setup components
            List<ExComponent> components = new List<ExComponent>();

            // remove every previous category
            foreach (Transform child in transform) {
                DestroyImmediate(child.gameObject);
            }

            foreach (XML.Component xmlComponent in xmlComponents.Component) {
                
                Type typeComponent = Type.GetType(string.Format("Ex.{0}Component", xmlComponent.Type));
                if (typeComponent == null) {
                    ExVR.Log().error(string.Format("Error when creating component: {0}", xmlComponent.Type));
                    return false;
                }

                var component = (ExComponent)GO.generate_empty_object(xmlComponent.Name, null, true).AddComponent(typeComponent);
                if (!component.setup_component_object(xmlComponent)) {
                    return false;
                }
                components.Add(component);
                
            }

            // fill global components
            Condition.sortedGlobalComponents = new Dictionary<int, ExComponent>();
            foreach (var component in components) {
                if (component.is_global() && component.priority == Priority.Hight) {
                    Condition.sortedGlobalComponents[component.key] = component;
                }
            }
            foreach (var component in components) {
                if (component.is_global() && component.priority == Priority.Medium) {
                    Condition.sortedGlobalComponents[component.key] = component;
                }
            }
            foreach (var component in components) {
                if (component.is_global() && component.priority == Priority.Low) {
                    Condition.sortedGlobalComponents[component.key] = component;
                }
            }

            // sort components by priority
            sortedComponents.Clear();
            reverseSortedComponents.Clear();            

            foreach (var component in components) {
                if(component.priority == Priority.Hight) {
                    sortedComponents.Add(component);
                } else if (component.priority == Priority.Low) {
                    reverseSortedComponents.Add(component);
                }
            }
            foreach (var component in components) {
                if (component.priority == Priority.Medium) {
                    sortedComponents.Add(component);
                    reverseSortedComponents.Add(component);
                } 
            }
            foreach (var component in components) {
                if (component.priority == Priority.Low) {
                    sortedComponents.Add(component);
                } else if (component.priority == Priority.Hight) {
                    reverseSortedComponents.Add(component);
                }
            }

            // fill dictionnaries
            componentsPerType.Clear();
            componentsPerCategory.Clear();
            componentPerKey.Clear();
            componentPerName.Clear();
            foreach (var component in sortedComponents) {

                var typeComponent = component.GetType();
                if (!componentsPerType.ContainsKey(typeComponent)) {
                    componentsPerType[typeComponent] = new List<ExComponent>();
                }
                componentsPerType[typeComponent].Add(component);

                if (!componentsPerCategory.ContainsKey(component.category)) {
                    componentsPerCategory[component.category] = new List<ExComponent>();
                }
                componentsPerCategory[component.category].Add(component);

                componentPerKey[component.key] = component;
                componentPerName[component.name] = component;
            }

            return true;
        }

        public bool initialize() {

            // initialize components
            ExVR.ExpLog().components(Function.initialize, true);
            foreach (var component in sortedComponents) {

                if (component.is_function_defined(Function.initialize)) {
                    ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.initialize, true, true);
                }

                if (!component.base_initialize()) {
                    // if one component not initialized, we stop the experiment generation
                    return false;
                }

            }
            ExVR.ExpLog().components(Function.initialize, false);

            return true;
        }

        public void action_from_gui(int componentKey, int configKey, string actionName) {

            ExComponent component = get_from_key(componentKey);
            if (component == null) {
                return;                
            }

            if (component.is_function_defined(Function.action_from_gui)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.action_from_gui, true);
            }

            component.base_action_from_gui(configKey, actionName);

            if (component.is_function_defined(Function.action_from_gui)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.action_from_gui, false);
            }
        }

        public void update_parameter_from_gui_xml(int componentKey, int configKey, XML.Arg xmlArg) {

            ExComponent component = get_from_key(componentKey);
            if(component == null) {
                return;
            }

            var config = component.get_config(configKey, false);
            if(config == null) {
                return;
            }

            // update config arg
            if (!config.update_from_xml(xmlArg)) {
                ExVR.Log().error(string.Format("{0} updating parameter [{1}] with value [{2}] from config [{3}] failed.",
                    component.verbose_name(),
                    xmlArg.Name,
                    xmlArg.Value,
                    config.name
                ));
                return;
            }
            
            if(component.currentC == null) {
                return;
            }

            // continue only if updated parameter is from component current config
            if (component.currentC.key != configKey) {
                return;
            }

            if (component.is_function_defined(Function.update_parameter_from_gui)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.update_parameter_from_gui, true);
            }
    
            component.base_update_parameter_from_gui(xmlArg.Name);

            if (component.is_function_defined(Function.update_parameter_from_gui)) {
                ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.update_parameter_from_gui, false);
            }
        }


        public void start_experiment() {
                        
            ExVR.ExpLog().components(Function.start_experiment, true);

            foreach (var component in sortedComponents) {

                // reset states
                component.currentCondition  = null;
                component.currentRoutine    = null;
                component.currentTimeline   = null;

                if (component.is_function_defined(Function.start_experiment)) {
                    ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.start_experiment, true, true);
                }
                component.base_start_experiment();

                if (component.is_function_defined(Function.start_experiment)) {
                    ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.start_experiment, false, true);
                }
            }

            ExVR.ExpLog().components(Function.start_experiment, false);
        }

        public void post_start_experiment() {

            ExVR.ExpLog().components(Function.post_start_experiment, true);

            foreach (var component in sortedComponents) {

                if (component.is_function_defined(Function.post_start_experiment)) {
                    ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.post_start_experiment, true, true);
                }
                component.base_post_start_experiment();

                if (component.is_function_defined(Function.post_start_experiment)) {
                    ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.post_start_experiment, false, true);
                }
            }

            ExVR.ExpLog().components(Function.post_start_experiment, false);
        }


        public void pre_stop_experiment() {

            ExVR.ExpLog().components(Function.pre_stop_experiment, true);
            foreach (var component in reverseSortedComponents) {

                if (component.is_function_defined(Function.pre_stop_experiment)) {
                    ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.pre_stop_experiment, true, true, true, true);
                }

                component.base_pre_stop_experiment();

                if (component.is_function_defined(Function.pre_stop_experiment)) {
                    ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.pre_stop_experiment, false, true, true, true);
                }
            }
            ExVR.ExpLog().components(Function.pre_stop_experiment, false);

        }

        public void stop_experiment() {

            ExVR.ExpLog().components(Function.stop_experiment, true);
            foreach (var component in reverseSortedComponents) {

                if (component.is_function_defined(Function.stop_experiment)) {
                    ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.stop_experiment, true, true, true, true);
                }

                component.base_stop_experiment();

                if (component.is_function_defined(Function.stop_experiment)) {
                    ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.stop_experiment, false, true, true, true);
                }

                // reset states
                component.currentCondition  = null;
                component.currentRoutine    = null;
                component.currentTimeline   = null;
            }
            ExVR.ExpLog().components(Function.stop_experiment, false);
        }

        public void call_global_update() {
            foreach(var component in sortedComponents) {                
                component.global_update();
            }
        }

        public void close(ExComponent componentToClose) {

            if (!componentToClose.is_closed()) {
                //ExVR.Log().message(string.Format("Close component {0} with key {1}.", componentToClose.name, componentToClose.key));
                set_component_states(componentToClose, false, false);
                componentToClose.set_closed_flag(true);                
            } else {
                ExVR.Log().error(string.Format("Component {0} with key {1} already closed.", componentToClose.name, componentToClose.key));
            }            
        }

        public void clean() {
            
            ExVR.ExpLog().components(Function.clean, true);
            foreach (var component in reverseSortedComponents) {

                if (component.is_function_defined(Function.clean)) {
                    ExVR.ExpLog().component(component, Function.clean, "", "", false, false);                    
                }
                component.base_clean();

                // destroy gameobject
                if (component.gameObject != null) {
                    Destroy(component.gameObject);
                }
            }
            ExVR.ExpLog().components(Function.clean, false);

            // clean container
            sortedComponents.Clear();
            componentsPerType.Clear();
            componentsPerCategory.Clear();
        }

        public void update_states_from_time(Condition condition, double time) {

            //if(time > condition.duration()) {
            //    ExVR.Log().error("DURATION 0 CONDITION " + time +" " + condition.duration());
            //}

            // component inside condition
            HashSet<int> componentInsideCondition = new HashSet<int>();
            foreach (var action in condition.actions) {

                var component = action.component();
                componentInsideCondition.Add(component.key);

                bool doUpdate   = action.timeline().inside_update_intervals(time);
                bool visibility = action.timeline().inside_visibility_intervals(time);
                set_component_states(component, doUpdate, visibility);
            }

            // component not inside condition
            foreach(var component in reverseSortedComponents) {
                if (!componentInsideCondition.Contains(component.key)) {
                    set_component_states(component, false, false);
                }                
            }
        }

        public void set_every_component_states_to_false() {
            // disable/hide all components if necessary with login
            foreach(var component in reverseSortedComponents) {
                set_component_states(component, false, false);
            }
        }

        public void disable() {

            // force disable/hide all component without logging
            foreach (var component in reverseSortedComponents) {
                if (component.is_updating()) {
                    component.base_set_update_state(false);
                }
                if (component.is_visible()) {
                    component.base_set_visibility(false);
                }
            }
        }        


        public void set_component_states(ExComponent component, bool doUpdate, bool visibility, bool log = true) {

            if (component.is_closed()) {
                return;
            }

            if (doUpdate != component.is_updating()) {

                if (component.is_started() && log) {
                    ExVR.ExpLog().component(component, Function.set_update_state, component.currentC.name, " -> U=" + doUpdate, true, true);
                }

                if (log && component.is_function_defined(Function.set_update_state)) {
                    ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.set_update_state, true);
                }

                component.base_set_update_state(doUpdate);

                if (log && component.is_function_defined(Function.set_update_state)) {
                    ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.set_update_state, false);
                }                
            }
            
            if (visibility != component.is_visible()) {
       
                if (component.is_started() && log) {
                    ExVR.ExpLog().component(component, Function.set_visibility, component.currentC.name, " -> U=" + visibility, true, true);
                }

                if (log && component.is_function_defined(Function.set_visibility)) {
                    ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.set_visibility, true);
                }

                component.base_set_visibility(visibility);

                if (log && component.is_function_defined(Function.set_visibility)) {
                    ExVR.ExpLog().log_and_add_to_stacktrace(component, Function.set_visibility, false);
                }
            }
        }

        public int count() {
            return sortedComponents.Count;
        }

        // get
        public ExComponent get_from_key(int componentKey) {
            if (componentPerKey.ContainsKey(componentKey)) {
                return componentPerKey[componentKey];
            }
            return null;
        }
        public T get_from_key<T>(int componentKey) where T : ExComponent {
            return (T)get_from_key(componentKey);
        }

        public ExComponent get_from_name(string componentName) {

            if (componentPerName.ContainsKey(componentName)) {
                return componentPerName[componentName];
            }
            return null;
        }
        public T get_from_name<T>(string componentName) where T : ExComponent {
            return (T)get_from_name(componentName);
        }

        public T get_first<T>() where T : ExComponent {
            if (componentsPerType.ContainsKey(typeof(T))) {
                if (componentsPerType[typeof(T)].Count > 0) {
                    return (T)componentsPerType[typeof(T)][0];
                }
            }
            return null;
        }

        // set
        public void set_first<T>(ref T component) where T : ExComponent {
            if (componentsPerType.ContainsKey(typeof(T))){
                if (componentsPerType[typeof(T)].Count > 0) {
                    component = (T)componentsPerType[typeof(T)][0];
                }
            }
        }

        // get all
        public List<ExComponent> get_all() {

            List<ExComponent> componentsList = new List<ExComponent>(count());
            foreach (var component in sortedComponents) {
                componentsList.Add(component);
            }
            return componentsList;
        }

        public List<ExComponent> get_all_from_category(ExComponent.Category category) {
            List<ExComponent> componentsWithCategory = new List<ExComponent>();
            foreach (var component in componentsPerCategory[category]) {
                componentsWithCategory.Add(component);
            }
            return componentsWithCategory;
        }

        public List<ExComponent> get_all_from_category(string categoryStr) {
            Category category;
            if (System.Enum.TryParse(categoryStr, false, out category)) {
                return get_all_from_category(category);
            }
            return new List<ExComponent>();
        }


        public List<ExComponent> get_all_from_type(Type type) {
            List<ExComponent> componentsWithType = new List<ExComponent>();
            foreach (var component in componentsPerType[type]) {
                componentsWithType.Add(component);
            }
            return componentsWithType;
        }

        public List<T> get_all_from_type<T>() where T : ExComponent {
            List<T> components = new List<T>();
            var type = typeof(T);
            foreach (var component in componentsPerType[type]) {
                components.Add((T)component);
            }
            return components;
        }

        // others
        public T get_csharp_script<T>(string name) where T : BaseCompiledCSharpComponent { 
            var component = get_from_name(name);
            if (component != null) {
                if (component.GetType().ToString() == "Ex.CSharpScriptComponent") {
                    return (T) (((CSharpScriptComponent) component).compiled());
                } else {
                    ExVR.Log().error(string.Format("Component {0} is not a CSharp script.", name));
                }
            }
            return null;
        }

    }
}


//#if UNITY_EDITOR
//using UnityEditor;
//namespace Ex{

//    [CustomEditor(typeof(Components))]
//    public class ComponentsEditor : Editor{

//        public override bool RequiresConstantRepaint() {
//            return true;
//        }

//        public override void OnInspectorGUI() {

//            base.OnInspectorGUI();

//            if (!Application.isPlaying) {
//                return;
//            }

//            //EditorGUILayout.LabelField("Current element:");
//            //var currentElementInfo = ExVR.Schreduler().current_element_info();
//            //if (currentElementInfo != null) {
//            //    if (currentElementInfo.type() == FlowElement.FlowElementType.Routine) {

//            //        var routineInfo = (RoutineInfo)currentElementInfo;
//            //        var routine = (Routine)routineInfo.element;
//            //        if (routine != null) {
//            //            EditorGUILayout.LabelField("[Routine] with id " + (currentElementInfo.key()).ToString());
//            //            EditorGUILayout.ObjectField(routine, typeof(Routine), true);

//            //            var condition = routine.current_condition();
//            //            if (condition != null) {
//            //                EditorGUILayout.LabelField("With condition:");
//            //                EditorGUILayout.ObjectField(condition, typeof(Condition), true);
//            //            } else {
//            //                EditorGUILayout.LabelField("...");
//            //            }

//            //        } else {
//            //            EditorGUILayout.LabelField("...");
//            //        }
//            //    } else {

//            //        var isiInfo = (ISIInfo)currentElementInfo;
//            //        var isi = (ISI)isiInfo.element;
//            //        if (isi != null) {
//            //            EditorGUILayout.LabelField("[ISI] with id " + (currentElementInfo.key()).ToString());
//            //            EditorGUILayout.ObjectField(isi, typeof(ISI), true);
//            //        } else {
//            //            EditorGUILayout.LabelField("...");
//            //        }
//            //        //currentCondition = isi.current_condition();
//            //    }
//            //} else {
//            //    EditorGUILayout.LabelField("...");
//            //}
//        }
//    }
//}
//#endif