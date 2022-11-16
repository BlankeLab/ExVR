
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
using System.Text;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class Routines : MonoBehaviour{

        private List<Routine> m_routines = new List<Routine>();
        private Dictionary<string, Routine> m_routinesPerName = new Dictionary<string, Routine>();
        private Dictionary<int, Routine> m_routinesPerKey = new Dictionary<int, Routine>();

        private Routine m_currentRoutine = null;

        public Routine current() {            
            return m_currentRoutine;
        }

        public int count() {
            return m_routines.Count;
        }

        public List<Routine> get_all() {
            return new List<Routine>(m_routines);
        }

        public Routine get(string routineName, bool displayError = true) {
            if (m_routinesPerName.ContainsKey(routineName)) {
                return m_routinesPerName[routineName];
            }
            if (displayError) {
                ExVR.Log().error(string.Format("Cannot find routine with name {0}.", routineName));
            }
            return null;
        }

        public Routine get(int routineKey) {
            if (m_routinesPerKey.ContainsKey(routineKey)) {
                return m_routinesPerKey[routineKey];
            }
            ExVR.Log().error(string.Format("Cannot find routine with key {0}.", Converter.to_string(routineKey)));
            return null;
        }

        public Routine get_from_id(int id) {
            if (id < m_routines.Count) {
                return m_routines[id];
            }
            ExVR.Log().error(string.Format("Cannot find routine with id {0}.", Converter.to_string(id)));
            return null;
        }

        public void display_last_info() {

            // display last routine info
            StringBuilder builder = new StringBuilder();
            builder.Append(" [DISABLE] -> ");
            if (m_currentRoutine != null) {
                builder.AppendFormat(" last routine [{0}]", m_currentRoutine.name);
                var currentCond = m_currentRoutine.current_condition();
                if (currentCond != null) {
                    builder.AppendFormat(" with condition [{0}]", currentCond.name);
                    if (currentCond.currentConnector != null) {
                        builder.AppendFormat(" with last connector [{0}] and function [{1}]", currentCond.currentConnector.name, currentCond.currentConnector.currentFunction.ToString());
                    } else {
                        builder.Append(" and no last connector");
                    }
                } else {
                    builder.Append(" with no condition");
                }
            } else {
                builder.Append(" no last routine");
            }
            ExVR.ExpLog().routine_message(builder.ToString());
        }



        public void generate_from_xml(XML.Routines xmlRoutines) {

            ExVR.ExpLog().routines_message("Generate from XML.");

            m_routines.Clear();
            m_routinesPerName.Clear();
            m_routinesPerKey.Clear();
            foreach (XML.Routine xmlRoutine in xmlRoutines.Routine) {
                var routine = GO.generate_empty_object(xmlRoutine.Name, ExVR.GO().Routines.transform).AddComponent<Routine>();
                routine.setup_element_object(xmlRoutine);
                m_routines.Add(routine);
                m_routinesPerName[routine.name] = routine;
                m_routinesPerKey[routine.key()] = routine;
            }            
        }

        public bool initialize() {

            ExVR.ExpLog().routines_message("Initialize.");

            foreach (var routine in m_routines) {
                if (!routine.initialize()) {
                    ExVR.Log().error(string.Format("Cannot initialize routine with name {0} and key {1}", routine.name, routine.key_str()));
                    return false;
                }
            }
            return true;
        }



        public void start_routine(RoutineInfo info) {

            // set current routine
            m_currentRoutine = (Routine)info.element();

            // start it
            ExVR.ExpLog().push_to_strackTrace(new RoutinesManagerTrace(m_currentRoutine, "Start", true));
            m_currentRoutine.start(info);
            ExVR.ExpLog().push_to_strackTrace(new RoutinesManagerTrace(m_currentRoutine, "Started", false));
        }

        public void on_gui() {
            if (m_currentRoutine != null) {
                m_currentRoutine.on_gui();
            }
        }

        public void end_of_frame() {
            if (m_currentRoutine != null) {
                m_currentRoutine.end_of_frame();
            }
        }

        public void update_current_routine() {
            
            if (m_currentRoutine != null) {
                m_currentRoutine.update();
            }
        }

        public void trigger_update_signals(RoutineInfo info) {

            Routine routine = get(info.key());
            if (routine) {
                foreach (Condition condition in routine.get_conditions()) {

                    // retrieve current condition
                    if (condition.name == info.condition().name) {
                        condition.trigger_update_connector_signals();
                        return;
                    }
                }
            }
        }



        public void play() {
            if (m_currentRoutine != null) {
                m_currentRoutine.play();
            }
        }

        public void pause() {
            if (m_currentRoutine != null) {                
                m_currentRoutine.pause();
            }
        }

        public void stop_current_routine() {
            if(m_currentRoutine != null) {
                ExVR.ExpLog().push_to_strackTrace(new RoutinesManagerTrace(m_currentRoutine, "Stop", true));
                m_currentRoutine.stop();
                ExVR.ExpLog().push_to_strackTrace(new RoutinesManagerTrace(m_currentRoutine, "Stopped", false));
                m_currentRoutine = null;
            }
        }

        public void start_experiment() {
            ExVR.ExpLog().routines_message("Start experiment.");
            ExVR.Components().start_experiment();
            ExVR.Components().post_start_experiment();            
        }

        public void stop_experiment() {

            ExVR.ExpLog().routines_message("Stop experiment.");

            ExVR.Components().disable();

            // display info of last current routine
            display_last_info();
            stop_current_routine();

            foreach (var routine in m_routines) {
                routine.stop_experiment();
            }

            // call stop experiment function for all components
            ExVR.Components().pre_stop_experiment();
            ExVR.Components().stop_experiment();
        }

        public void clean() {

            ExVR.ExpLog().routines_message("Clean.");

            // destroy components
            ExVR.Components().clean();

            foreach (var routine in m_routines) {
                routine.clean();
                Destroy(routine.gameObject);
            }
            m_routines.Clear();
            m_routinesPerName.Clear();
            m_routinesPerKey.Clear();
        }

        public void update_components_states(RoutineInfo info) {
            ExVR.Components().update_states_from_time(info.condition(), ExVR.Time().ellapsed_element_s());
        }

        public bool modify_action_config(string routineName, string conditionName, string componentName, string newConfigName) {

            var component = ExVR.Components().get_from_name(componentName);
            if (component == null) {
                return false;
            }

            ComponentConfig newConfig = component.get_config(newConfigName);
            if (newConfig == null) {
                return false;
            }

            var routine = get(routineName);
            if(routine == null) {
                return false;
            }

            var condition = routine.get_condition_from_name(conditionName);
            if(condition == null) {
                return false;
            }

            var action = condition.get_action_from_component_name(componentName);
            if(action == null) {
                return false;
            }

            ExVR.ExpLog().push_to_strackTrace(new RoutinesManagerTrace(routine, "modify_action_config", true));
            action.set_config(newConfig);
            ExVR.ExpLog().push_to_strackTrace(new RoutinesManagerTrace(routine, "modify_action_config", false));

            return true;
        }
    }
}