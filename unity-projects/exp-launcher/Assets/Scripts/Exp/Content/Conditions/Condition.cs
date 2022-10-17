
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
using UnityEngine.Profiling;

namespace Ex{



    public class Condition : MonoBehaviour{

        // condition info
        private int m_key;
        private string keyStr;
        private double m_durationS = 0.0;
        private int m_callsNb = 0;
        private XML.Condition m_xmlCondition = null;

        // visual scripting
        private List<XML.Connection> m_connectionsXML = null;
        // # connections
        public List<Events.ConnectionInfo> connections = new List<Events.ConnectionInfo>();
        // # connectors
        public List<ExConnector> connectors = null;
        public Dictionary<int, ExConnector> connectorsPerKey = null;
        public ExConnector currentConnector = null;

        // actions
        public List<Action> actions = null;
        public List<Action> reverseOrderActions = null;

        // global components (always call update functions even if not in current condition)
        static public Dictionary<int, ExComponent> sortedGlobalComponents = null;

        public Dictionary<ExComponent.Category, List<Action>> actionsPerComponentCategory = null;
        public Dictionary<Type, List<Action>> actionsPerComponentType   = null;        
        public Dictionary<string, Action> actionPerComponentName = null;
        public Dictionary<int, Action> actionPerComponentKey = null; 


        public int key() {
            return m_key;
        }

        public string key_str() {
            return keyStr;
        }

        public double duration() {
            return m_durationS;
        }

        public int calls_nb() {
            return m_callsNb;
        }

        private void set_current(ExConnector connector) {
            currentConnector = connector;
        }

        public ExConnector get_connector(int connnectorKey) {
            if (connectorsPerKey.ContainsKey(connnectorKey)) {
                return connectorsPerKey[connnectorKey];
            }
            return null;
        }

        public Action get_action_from_component_key(int componentKey) {

            if (actionPerComponentKey.ContainsKey(componentKey)) {
                return actionPerComponentKey[componentKey];
            }
            return null;
        }

        public Action get_action_from_component_name(string componentName) {

            if (actionPerComponentName.ContainsKey(componentName)) {
                return actionPerComponentName[componentName];
            }
            return null;
        }

        public List<ExComponent> get_all_components() {
            List<ExComponent> components = new List<ExComponent>();
            foreach (var action in actions) {
                components.Add(action.component());
            }
            return components;
        }

        public List<ExComponent> get_all_components(ExComponent.Category category) {
            List<ExComponent> componentsWithCategory = new List<ExComponent>();
            foreach (var action in actionsPerComponentCategory[category]) {
                componentsWithCategory.Add(action.component());
            }
            return componentsWithCategory;
        }

        public List<ExComponent> get_all_components(Type type) {
            List<ExComponent> componentsWithType = new List<ExComponent>();
            foreach (var action in actionsPerComponentType[type]) {
                componentsWithType.Add(action.component());
            }
            return componentsWithType;
        }

        public List<T> get_all_components<T>() where T : ExComponent {
            List<T> components = new List<T>();
            var type = typeof(T);
            foreach (var action in actionsPerComponentType[type]) {
                components.Add((T)action.component());
            }
            return components;
        }


        public Routine parent_routine() {
            return transform.parent.GetComponent<Routine>();
        }

        public void setup_condition_object(XML.Condition xmlCondition) {

            m_xmlCondition = xmlCondition;

            // save key
            m_key = m_xmlCondition.Key;
            keyStr = Converter.to_string(m_xmlCondition.Key);

            // save connections
            m_connectionsXML = m_xmlCondition.Connections;

            // generate actions            
            var unsortedActions = new List<Action>(m_xmlCondition.Actions.Count);
            foreach (XML.Action actionXml in m_xmlCondition.Actions) {
                unsortedActions.Add(new Action(actionXml));
            }

            // store actions by priority
            actions = new List<Action>(m_xmlCondition.Actions.Count);
            reverseOrderActions = new List<Action>(m_xmlCondition.Actions.Count);
            foreach (var action in unsortedActions) {
                if (action.component().priority == ExComponent.Priority.Hight) {
                    actions.Add(action);
                } else if (action.component().priority == ExComponent.Priority.Low) {
                    reverseOrderActions.Add(action);
                }
            }
            foreach (var action in unsortedActions) {
                if (action.component().priority == ExComponent.Priority.Medium) {
                    actions.Add(action);
                    reverseOrderActions.Add(action);
                }
            }
            foreach (var action in unsortedActions) {
                if (action.component().priority == ExComponent.Priority.Low) {
                    actions.Add(action);
                } else if (action.component().priority == ExComponent.Priority.Hight) {
                    reverseOrderActions.Add(action);
                }
            }

            // store actions in dictionnary
            actionsPerComponentCategory = new Dictionary<ExComponent.Category, List<Action>>();
            actionsPerComponentType = new Dictionary<Type, List<Action>>();
            actionPerComponentName = new Dictionary<string, Action>();
            actionPerComponentKey = new Dictionary<int, Action>();
            foreach (var action in actions) {

                var typeComponent = action.component().GetType();
                if (!actionsPerComponentType.ContainsKey(typeComponent)) {
                    actionsPerComponentType[typeComponent] = new List<Action>();
                }
                actionsPerComponentType[typeComponent].Add(action);

                var category = action.component().category;
                if (!actionsPerComponentCategory.ContainsKey(category)) {
                    actionsPerComponentCategory[category] = new List<Action>();
                }
                actionsPerComponentCategory[category].Add(action);


                actionPerComponentName[action.component().name] = action;
                actionPerComponentKey[action.component().key] = action;
            }


            // find duration of the condition
            m_durationS = m_xmlCondition.Duration;

            // generate connectors
            connectors = new List<ExConnector>(m_xmlCondition.Connectors.Count);
            connectorsPerKey = new Dictionary<int, ExConnector>(m_xmlCondition.Connectors.Count);
            foreach (XML.Connector xmlConnector in m_xmlCondition.Connectors) {

                // generate type
                Type typeConnector = Type.GetType(string.Format("Ex.{0}Connector", xmlConnector.Name));
                if (typeConnector == null) {
                    ExVR.Log().error(string.Format("Error when creating connector: {0}", xmlConnector.Name));
                    continue;
                }

                // generate gameobject
                var go = new GameObject(String.Format("{0} {1}", xmlConnector.Name, xmlConnector.Key.ToString()));
                GO.init_local_transform(go, Vector3.zero, Vector3.zero, Vector3.one);
                go.transform.SetParent(ExVR.GO().Connectors.transform);

                // generate connector
                var connector = (ExConnector)go.AddComponent(typeConnector);
                if (connector == null) {
                    ExVR.Log().error(String.Format("Cannot generate connector from type: {0}", typeConnector.ToString()));
                    continue;
                }
                connector.setup_connector_object(xmlConnector);

                connector.associatedCondition = this;
                connector.associatedRoutine = parent_routine();
                connectors.Add(connector);
                connectorsPerKey[connector.key] = connector;         
            }
        }

        public bool initialize() {

            foreach (var connector in connectors) {
                if (!connector.base_initialize()) {
                    ExVR.Log().error(string.Format("Cannot initialize connector [{0}] with key [{1}].", connector.name, connector.keyStr));
                    return false;
                }
            }

            return true;
        }

        public void set_connections() {

            connections = new List<Events.ConnectionInfo>(m_connectionsXML.Count);
            foreach (var connectionXML in m_connectionsXML) {

                var connection = new Events.ConnectionInfo();
                connection.startIndex = connectionXML.StartIndex;
                connection.endIndex   = connectionXML.EndIndex;

                bool componentStartType = connectionXML.StartType == "component";
                bool componentEndType   = connectionXML.EndType   == "component";

                // retrieve output event
                Events.Connections outE = null;
                if(componentStartType) {

                    var action = get_action_from_component_key(connectionXML.StartKey);
                    if (action == null) {
                        ExVR.Log().error(string.Format("Connection start key {0} not found in components.", connectionXML.StartKey));
                        continue;
                    }
                    outE = action.component().connections();
                    connection.start = action.component().gameObject;

                } else {

                    var connector = get_connector(connectionXML.StartKey);
                    if (connector == null) {
                        ExVR.Log().error(string.Format("Connection start key {0} not found in connectors.", connectionXML.StartKey));
                        continue;
                    }
                    
                    outE = connector.connections();
                    connection.start = connector.gameObject;
                }

                // retrieve input event
                Events.Connections inE = null;
                if (componentEndType) {

                    var action = get_action_from_component_key(connectionXML.EndKey);
                    if (action == null) {
                        ExVR.Log().error(string.Format("Connection end key {0} not found in components.", connectionXML.EndKey));
                        continue;
                    }
                    inE = action.component().connections();
                    connection.end = action.component().gameObject;

                } else {

                    var connector = get_connector(connectionXML.EndKey);
                    if (connector == null) {
                        ExVR.Log().error(string.Format("Connection end key {0} not found in connectors.", connectionXML.EndKey));
                        continue;
                    }
                    inE = connector.connections();
                    connection.end = connector.gameObject;                    
                }


                // add input/output to connectors
                if(!componentStartType && componentEndType){

                    connection.start.GetComponent<ExConnector>().outputGO.Add(connection.end);
                    connection.start.GetComponent<ExConnector>().outputConnections.Add(connection);

                } else if (componentStartType && !componentEndType){
                    connection.end.GetComponent<ExConnector>().inputGO.Add(connection.start);
                    connection.end.GetComponent<ExConnector>().inputConnections.Add(connection);

                } else if (!componentStartType && !componentEndType){

                    connection.start.GetComponent<ExConnector>().outputGO.Add(connection.end);
                    connection.start.GetComponent<ExConnector>().outputConnections.Add(connection);

                    connection.end.GetComponent<ExConnector>().inputGO.Add(connection.start);
                    connection.end.GetComponent<ExConnector>().inputConnections.Add(connection);
                }

                var signal = outE.get_signal(connectionXML.Signal);
                if(signal == null) {
                    continue;
                }
                var slot   = inE.get_slot(connectionXML.Slot);
                if(slot == null) {
                    continue;
                }

                // generate the connection
                if (connectionXML.StartDataType == connectionXML.EndDataType) {
                    if(!Events.Connections.connect(signal, slot)) {
                        //connectionXML.StartKey
                        ExVR.Log().error(string.Format("Connection error with signal {0} and slot {1}.", connectionXML.Signal, connectionXML.Slot));
                        continue;
                    }
                } else {
                    // if start data and end data are differents, retrieve the corresponding data convertor
                    string code = string.Format("{0}:{1}", connectionXML.StartDataType, connectionXML.EndDataType);
                    var convertor = ConnectionsConvertors.get(code);
                    if(convertor == null) {
                        continue;
                    }
                    if(!Events.Connections.connect(signal, slot, convertor)) {
                        ExVR.Log().error(string.Format("Connection error with signal {0} and slot {1}.", connectionXML.Signal, connectionXML.Slot));
                        continue;
                    }                    
                }

                connections.Add(connection);
            }
        }

        public void start_routine() {

            // set connections between components and connectors
            set_connections();

            // reset components current routine/condition/timeline/config
            foreach (var component in ExVR.Components().componentPerName) {
                component.Value.currentRoutine   = null;
                component.Value.currentCondition = null;
                component.Value.currentTimeline  = null;
                component.Value.currentC         = null;
            }

            // actions
            {
                // set components config
                foreach (var action in actions) {
                    action.set_current_config(this);                    
                }

                foreach (var action in actions) {
                    action.update_from_current_config();
                }                

                // pre start routine
                ExVR.ExpLog().action(ExComponent.Function.pre_start_routine, true);
                foreach (var action in actions) {
                    action.pre_start_routine();
                }
                ExVR.ExpLog().action(ExComponent.Function.pre_start_routine, false);

                // start routine
                ExVR.ExpLog().action(ExComponent.Function.start_routine, true);
                foreach (var action in actions) {
                    action.start_routine();
                }
                ExVR.ExpLog().action(ExComponent.Function.start_routine, false);

                // post start routine
                ExVR.ExpLog().action(ExComponent.Function.post_start_routine, true);
                foreach (var action in actions) {
                    action.post_start_routine();
                }
                ExVR.ExpLog().action(ExComponent.Function.post_start_routine, false);
            }

            // connectors
            {                
                // pre start routine
                ExVR.ExpLog().connector(ExConnector.Function.pre_start_routine, true);
                foreach (var connector in connectors) {
                    set_current(connector);
                    connector.base_pre_start_routine();
                }
                ExVR.ExpLog().connector(ExConnector.Function.pre_start_routine, false);

                // start routine
                ExVR.ExpLog().connector(ExConnector.Function.start_routine, true);
                foreach (var connector in connectors) {
                    set_current(connector);
                    connector.base_start_routine();
                }
                ExVR.ExpLog().connector(ExConnector.Function.start_routine, false);
            }
        }

        public void on_gui() {

            Profiler.BeginSample(string.Format("[ExVR][Condition][{0}] on_gui", name));

            // call on_gui for global components
            foreach (var component in sortedGlobalComponents) {
                Profiler.BeginSample(string.Format("[ExVR][Component][{0}] on_gui", component.Value.name));
                Action.on_gui(component.Value);
                Profiler.EndSample();
            }

            // call on_gui for actions
            foreach (var action in actions) {                
                if (!action.component().is_global()) {
                    Profiler.BeginSample(string.Format("[ExVR][Component][{0}] on_gui", action.component().name));
                    action.on_gui();
                    Profiler.EndSample();
                }
            }
            Profiler.EndSample();
        }

        public void end_of_frame() {

            Profiler.BeginSample(string.Format("[ExVR][Condition][{0}] end_of_frame", name));

            // call end_of_frame for global components
            foreach (var component in sortedGlobalComponents) {
                Profiler.BeginSample(string.Format("[ExVR][Component][{0}] end_of_frame", component.Value.name));
                Action.end_of_frame(component.Value);
                Profiler.EndSample();
            }

            // call end_of_frame for actions
            foreach (var action in actions) {
                if (!action.component().is_global()) {
                    Profiler.BeginSample(string.Format("[ExVR][Component][{0}] end_of_frame", action.component().name));
                    action.end_of_frame();
                    Profiler.EndSample();
                }
            }

            Profiler.EndSample();
        }

        public void update() {

            Profiler.BeginSample(string.Format("[ExVR][Condition][{0}] pre_update", name));

            // call pre_update for global component
            foreach (var component in sortedGlobalComponents) {
                Profiler.BeginSample(string.Format("[ExVR][Component][{0}] pre_update", component.Value.name));
                Action.pre_update(component.Value);
                Profiler.EndSample();
            }

            // call pre_update for actions       
            foreach (var action in actions) {
                if (!action.component().is_global()) {
                    Profiler.BeginSample(string.Format("[ExVR][Component][{0}] pre_update", action.component().name));
                    action.pre_update();
                    Profiler.EndSample();
                }
            }
            Profiler.EndSample();


            Profiler.BeginSample(string.Format("[ExVR][Condition][{0}] update", name));

            // call update for global components
            foreach (var component in sortedGlobalComponents) {
                Profiler.BeginSample(string.Format("[ExVR][Component][{0}] update", component.Value.name));
                Action.update(component.Value);
                Profiler.EndSample();
            }

            // call update for actions
            foreach (var action in actions) {
                if (!sortedGlobalComponents.ContainsKey(action.component().key)) {
                    Profiler.BeginSample(string.Format("[ExVR][Component][{0}] update", action.component().name));
                    action.update();
                    Profiler.EndSample();
                }
            }
            Profiler.EndSample();

            Profiler.BeginSample(string.Format("[ExVR][Condition][{0}] post_update", name));

            // call post_update for global components
            foreach (var component in sortedGlobalComponents) {
                Profiler.BeginSample(string.Format("[ExVR][Component][{0}] post_update", component.Value.name));
                Action.post_update(component.Value);
                Profiler.EndSample();
            }

            // call post_update for actions
            foreach (var action in actions) {
                if (!sortedGlobalComponents.ContainsKey(action.component().key)) {
                    Profiler.BeginSample(string.Format("[ExVR][Component][{0}] post_update", action.component().name));
                    action.post_update();
                    Profiler.EndSample();
                }
            }
            Profiler.EndSample();
        }

        public void play() {
            ExVR.ExpLog().action(ExComponent.Function.play, true);
            foreach (var action in actions) {
                action.play();
            }
            ExVR.ExpLog().action(ExComponent.Function.play, false);
        }

        public void pause() {
            ExVR.ExpLog().action(ExComponent.Function.pause, true);
            foreach (var action in actions) {
                action.pause();
            }
            ExVR.ExpLog().action(ExComponent.Function.pause, false);
        }
        public void stop_routine() {

            // connectors
            {
                ExVR.ExpLog().connector(ExConnector.Function.stop_routine, true);
                foreach (var connector in connectors) {
                    set_current(connector);
                    connector.base_stop_routine();
                }
                ExVR.ExpLog().connector(ExConnector.Function.stop_routine, false);
            }

            // actions
            {
                ExVR.ExpLog().action(ExComponent.Function.stop_routine, true);
                foreach (var action in reverseOrderActions) {
                    action.stop_routine();
                }
                ExVR.ExpLog().action(ExComponent.Function.stop_routine, false);
            }
            
            // remove connections between components and connectors
            remove_connections();

            // increment nb of calls
            m_callsNb++;
        }

        public void stop_experiment() {
            m_callsNb = 0;
        }

        public void trigger_update_connector_signals() {

            foreach (var connector in connectors) {
                set_current(connector);
                connector.base_pre_update();
            }

            foreach (var connector in connectors) {
                set_current(connector);
                connector.base_update();
            }

            foreach (var connector in connectors) {
                set_current(connector);
                connector.base_post_update();
            }
        }

        public void remove_connections() {

            foreach (var action in actions) {                
                action.component().connections().clean();
            }
            foreach (var connector in connectors) {
                connector.connections().clean();
            }
        }
    }
}

