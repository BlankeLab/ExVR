
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
using System.Diagnostics;

// unity
using UnityEngine;

namespace Ex{

    public class Routine : FlowElement{

        // states
        [SerializeField]
        private bool m_isARandomizer = false;
        [SerializeField]
        private int m_conditionIteration = 0;

        // condition
        [SerializeField]
        private List<Condition> m_conditions = null;
        private Dictionary<string, Condition> m_conditionsPerName = null;
        private Dictionary<int, Condition> m_conditionsPerKey = null;

        [SerializeField]
        private Condition m_currentCondition = null;

        // timers
        [SerializeField]
        private Stopwatch m_startTimer = new Stopwatch();
        [SerializeField]
        private Stopwatch m_stopTimer = new Stopwatch();

        // states
        public bool is_a_randomizer() {return m_isARandomizer;}
        public int condition_iteration() { return m_conditionIteration; }

        // condition
        public Condition current_condition() {return m_currentCondition;}
        public List<Condition> get_conditions() { return m_conditions; }
        public int conditions_count() {return m_conditions.Count;}

        public Condition get_condition_from_id(int id) {
            if (id < conditions_count() && id >= 0) {
                return m_conditions[id];
            }
            ExVR.Log().error(string.Format("Condition with id [{0}] invalid.", Converter.to_string(id)));
            return null;
        }

        public Condition get_condition_from_key(int conditionKey) {
            if (m_conditionsPerKey.ContainsKey(conditionKey)) {
                return m_conditionsPerKey[conditionKey];
            }
            ExVR.Log().error(string.Format("Condition with key [{0}] not found.", Converter.to_string(conditionKey)));
            return null;
        }

        public Condition get_condition_from_name(string conditionName) {
            if (m_conditionsPerName.ContainsKey(conditionName)) {
                return m_conditionsPerName[conditionName];
            }
            ExVR.Log().error(string.Format("Condition with name [{0}] not found.", conditionName));
            return null;
        }

        // time
        public double start_timer_duration_ms() {return m_startTimer.Elapsed.TotalMilliseconds;}
        public double stop_timer_duration_ms() {return m_stopTimer.Elapsed.TotalMilliseconds;}

        // functions
        public void setup_element_object(XML.Routine routine) {

            m_key = routine.Key;
            m_keyStr = Converter.to_string(routine.Key);
            m_type = Type.Routine;
            m_isARandomizer = routine.Randomizer;

            // generate conditions
            m_conditions = new List<Condition>(routine.Conditions.Count);
            m_conditionsPerName = new Dictionary<string, Condition>(routine.Conditions.Count);
            m_conditionsPerKey = new Dictionary<int, Condition>(routine.Conditions.Count);

            foreach (XML.Condition xmlCondition in routine.Conditions) {
                var conditionGO = GO.generate_empty_object(xmlCondition.Name, transform, true);
                var condition = conditionGO.AddComponent<Condition>();
                condition.setup_condition_object(xmlCondition);
                //condition.initialize(xmlCondition);
                m_conditions.Add(condition);
                m_conditionsPerName[condition.name] = condition;
                m_conditionsPerKey[condition.key()] = condition;
            }

        }
        public bool initialize() {

            foreach (var condition in m_conditions) {
                if (!condition.initialize()) {
                    ExVR.Log().error(string.Format("Cannot initialize condition with name {0} and key {1}", condition.name, condition.key_str()));
                    return false; 
                }
            }

            return true;
        }

        public void start(RoutineInfo info) {

            ExVR.ExpLog().enable_routine(info);

            // update routine with info
            m_currentCondition = info.condition();
            m_elementIteration = info.element_iteration();
            m_conditionIteration = info.condition_iteration();

            // enable it
            gameObject.SetActive(true);

            m_startTimer.Restart();
            m_currentCondition.start_routine();
            m_startTimer.Stop();
            ExVR.ExpLog().routine_message(string.Format("Started in {0} ms", m_startTimer.ElapsedMilliseconds));            
        }

        public void on_gui() {m_currentCondition.on_gui();}
        public void update() {m_currentCondition.update();}
        public void play() {
            ExVR.ExpLog().routine_message("Play.");
            m_currentCondition.play();
        }
        public void pause() {
            ExVR.ExpLog().routine_message("Pause.");
            m_currentCondition.pause();
        }

        public void stop() {  

            m_stopTimer.Restart();
            m_currentCondition.stop_routine();
            m_stopTimer.Stop();
            ExVR.ExpLog().routine_message(string.Format("Stopped in {0} ms", m_stopTimer.ElapsedMilliseconds));

            // increment nb of calls
            m_callsNb++;

            // disable gameobject
            gameObject.SetActive(false);
        }

        public void stop_experiment() {

            m_callsNb = 0;
            foreach (var condition in m_conditions) {
                condition.stop_experiment();
            }
        }
        public void clean() {
            foreach (var condition in m_conditions) {
                foreach (var exCo in condition.connectors) {
                    Destroy(exCo.gameObject);
                }
            }
        }
    }
}