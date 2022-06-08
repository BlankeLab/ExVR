
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

#if UNITY_EDITOR
using UnityEditor;
namespace Ex{

    [CustomEditor(typeof(Scheduler))]
    public class SchredulerEditor : Editor{

        public override bool RequiresConstantRepaint() {
            return true;
        }

        public override void OnInspectorGUI() {

            base.OnInspectorGUI();

            if (!Application.isPlaying) {
                return;
            }

            EditorGUILayout.LabelField("Current element:");
            var currentElementInfo = ExVR.Scheduler().current_element_info();
            if (currentElementInfo != null) {
                if (currentElementInfo.type() == FlowElement.Type.Routine) {

                    var routineInfo = (RoutineInfo)currentElementInfo;
                    var routine = (Routine)routineInfo.element();
                    if (routine != null) {
                        EditorGUILayout.LabelField(string.Format("[Routine] with id {0}, called {1} times", currentElementInfo.key().ToString(), routine.calls_nb()));
                        EditorGUILayout.ObjectField(routine, typeof(Routine), true);

                        var condition = routine.current_condition();
                        if (condition != null) {
                            EditorGUILayout.LabelField("With condition:");
                            EditorGUILayout.ObjectField(condition, typeof(Condition), true);
                        } else {
                            EditorGUILayout.LabelField("...");
                        }

                    } else {
                        EditorGUILayout.LabelField("...");
                    }
                } else {

                    var isiInfo = (ISIInfo)currentElementInfo;
                    var isi = (ISI)isiInfo.element();
                    if (isi != null) {
                        EditorGUILayout.LabelField(string.Format("[ISI] with id {0}", currentElementInfo.key().ToString()));
                        EditorGUILayout.ObjectField(isi, typeof(ISI), true);
                    } else {
                        EditorGUILayout.LabelField("...");
                    }
                    //currentCondition = isi.current_condition();
                }
            } else {
                EditorGUILayout.LabelField("...");
            }

            //EditorGUILayout.BeginHorizontal();
            //EditorGUILayout.EndHorizontal();


            //foreach (var info in ExVR.Schreduler().get_elements_info_order()) {
            //    EditorGUILayout.LabelField("[" + (id++).ToString() + "] " + (info.type == Elements.FlowElementType.Routine ? "Routine" : "ISI") + " -> " + info.name, EditorStyles.label);
            //    EditorGUILayout.LabelField("   with condition " + info.conditionName + "(" + info.conditionKey + ") and interval [" + info.interval.tStart + " - " + info.interval.tEnd + "]", EditorStyles.label);
            //}

            //if(currentElementInfo != null) {
            //    EditorGUILayout.LabelField("Current: " + currentElementInfo.name, EditorStyles.boldLabel);
            //} else {
            //    EditorGUILayout.LabelField("Current: ...");
            //}
        }
    }
}
#endif


namespace Ex {

    public class Scheduler : MonoBehaviour{

        // flow elements
        [SerializeField]
        private int m_currentElementId = 0;

        [SerializeField]
        private FlowElementInfo m_currentElementInfo = null;
        
        [SerializeField]
        public Instance instance = new Instance();

        public int current_element_order() {
            return m_currentElementId;
        }

        public FlowElementInfo current_element_info() {
            return m_currentElementInfo;
        }

        public bool generate_instance(XML.ExperimentFlow experimentFlow) {
            return instance.generate(experimentFlow);
        }

        public bool previous_element() {

            if (m_currentElementId > 0) {  // enable previous element
                --m_currentElementId;
                start_current_flow_element();
                return true;
            }

            // no more element before current element
            return false;
        }

        public bool next_element() {

            ExVR.Time().onGuiWait = true;

            if (m_currentElementId < instance.total_number_of_elements() - 1) { // enable next element
                m_currentElementId++;
                start_current_flow_element();
                return true;
            }            
            return false;
        }

        public bool next_element_with_name(string elementName) {
            if (m_currentElementId < instance.total_number_of_elements() - 1) {
                for (int ii = m_currentElementId + 1; ii < instance.total_number_of_elements(); ++ii) {
                    if (instance.element_order(ii).name() == elementName) {
                        m_currentElementId = ii;
                        start_current_flow_element();
                        return true;
                    }
                }
            }
            return false;
        }

        public bool previous_element_with_name(string elementName) {
            if (m_currentElementId > 0) {
                for (int ii = m_currentElementId - 1; ii >= 0; --ii) {
                    if (instance.element_order(ii).name() == elementName) {
                        m_currentElementId = ii;
                        start_current_flow_element();
                        return true;
                    }
                }
            }
            return false;
        }

        public bool next_element_with_condition(string conditionName) {

            if (m_currentElementId < instance.total_number_of_elements() - 1) {
                for (int ii = m_currentElementId + 1; ii < instance.total_number_of_elements(); ++ii) {
                    var elementO = instance.element_order(ii);
                    if ((elementO.type() == FlowElement.Type.Routine ?
                        ((RoutineInfo)elementO).condition().name : ((ISIInfo)elementO).duration_str()) == conditionName) { 
                        m_currentElementId = ii;
                        start_current_flow_element();
                        return true;
                    }
                }
            }
            return false;
        }

        public bool previous_element_with_condition(string conditionName) {

            if (m_currentElementId > 0) {
                for (int ii = m_currentElementId - 1; ii >= 0; --ii) {
                    var elementO = instance.element_order(ii);
                    if ((elementO.type() == FlowElement.Type.Routine ?
                        ((RoutineInfo)elementO).condition().name : ((ISIInfo)elementO).duration_str()) == conditionName) {
                        m_currentElementId = ii;
                        start_current_flow_element();
                        return true;
                    }
                }
            }
            return false;
        }

        public bool go_to_specific_instance_element(int elementOrderId) {

            if (elementOrderId < instance.total_number_of_elements()) {
                m_currentElementId = elementOrderId;
                start_current_flow_element();
                return true;
            }
            return false;
        }

        public bool start_experiment() {

            if(instance.total_number_of_elements() == 0) {
                return false;
            }

            // reset id and info
            m_currentElementId = 0;
            m_currentElementInfo = instance.element_order(m_currentElementId);
         
            return true;
        }

        public void start_current_flow_element() {

            // stop previous element
            ExVR.Routines().stop_current_routine();
            ExVR.ISIs().stop_current_isi();

            // retrieve new info
            m_currentElementInfo = instance.element_order(m_currentElementId);

            // enable new current flow element
            ExVR.Time().start_element();
            if (m_currentElementInfo.type() == FlowElement.Type.Isi) {
                // new element is an ISI
                ExVR.ISIs().start_isi((ISIInfo)m_currentElementInfo);
            } else if (m_currentElementInfo.type() == FlowElement.Type.Routine) {
                // new element is a routine 
                ExVR.Routines().start_routine((RoutineInfo)m_currentElementInfo);
            }
        }

        public FlowElementInfo update_current_flow_element() {

            if (instance.total_number_of_elements() == 0) { // no flow element
                return null;
            }

            // check if still inside interval
            if (!current_interval().is_in_interval(ExVR.Time().ellapsed_element_s())) {

                // go  for next element
                if (!next_element()) {
                    // no elemen remaining, end of experiment
                    return null; 
                }
            }

            return m_currentElementInfo;
        }


        public void stop_experiment() {
            // nullify current element
            m_currentElementInfo = null;
        }

        public Interval current_interval() {

            if(m_currentElementId >= instance.total_number_of_elements()) {
                return null;
            }
            return instance.element_order(m_currentElementId).interval();
        }
    }
}