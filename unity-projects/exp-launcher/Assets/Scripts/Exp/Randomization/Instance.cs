
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

namespace Ex{

    public class Instance{

        public int idInstance = 0;

        [SerializeField]
        private List<FlowElementInfo> elementsOrder = new List<FlowElementInfo>();
        [SerializeField]
        private List<FlowElementInfo> randomizerElementsOrder = new List<FlowElementInfo>();

        public bool generate(XML.ExperimentFlow experimentFlow) {

            // clean
            elementsOrder.Clear();
            randomizerElementsOrder.Clear();

            idInstance = experimentFlow.IdInstance;

            var routinesManager = ExVR.Routines();
            var isisManager = ExVR.ISIs();

            //experimentFlow.IdInstance;

            // create flow experiment from instance xml
            foreach (XML.Element element in experimentFlow.Elements) {

                if (element.Type == "routine") { // Routine

                    // retrieve routine
                    var routine = routinesManager.get(element.Key);
                    if (!routine) {
                        ExVR.Log().error(string.Format("Routine with id {0} from instance not found in experiment.", element.Key));
                        return false;
                    }

                    // retrieve the end routine time
                    Condition currentCondition = routine.get_condition_from_name(element.Cond);
                    if (!currentCondition) {
                        ExVR.Log().error(string.Format("Condition {0} from instance not found in experiment.", element.Cond));
                        return false;
                    }

                    // add element info                                        
                    if (routine.is_a_randomizer()) {
                        randomizerElementsOrder.Add(new RoutineInfo(
                            routine,
                            currentCondition,
                            new Interval(0, 0),
                            randomizerElementsOrder.Count,
                            element.ElemIteration,
                            element.ConditionIteration
                        ));

                    } else {
                        elementsOrder.Add(new RoutineInfo(
                            routine,
                            currentCondition,
                            new Interval(0, currentCondition.duration()),
                            elementsOrder.Count,
                            element.ElemIteration,
                            element.ConditionIteration
                        ));
                    }

                } else { // ISI

                    // retrieve ISI
                    var isi = isisManager.get(element.Key);
                    if (!isi) {
                        ExVR.Log().error(string.Format("IIS with id {0} from instance not found in experiment.", element.Key));
                        return false;
                    }

                    // add element info
                    elementsOrder.Add(new ISIInfo(
                        isi,
                        element.Cond,
                        new Interval(0, Converter.to_double(element.Cond)),
                        elementsOrder.Count,
                        element.ElemIteration,
                        element.ConditionIteration
                    ));
                }
            }


            return true;
        }

        public FlowElementInfo element_order(int id) {
            return elementsOrder[id];
        }

        public int total_number_of_elements() {
            return elementsOrder.Count;
        }

        public List<FlowElementInfo> get_elements_info_order(bool isARandomizer) {
            if (isARandomizer) {
                return new List<FlowElementInfo>(randomizerElementsOrder);
            } else {
                return new List<FlowElementInfo>(elementsOrder);
            }
        }

        public List<RoutineInfo> get_routine_infos_order(Routine routine) {

            List<RoutineInfo> infos = new List<RoutineInfo>();
            var elements = routine.is_a_randomizer() ? randomizerElementsOrder : elementsOrder;
            foreach (var info in elements) {
                if (info.key() == routine.key()) {
                    infos.Add((RoutineInfo)info);
                }
            }
            return infos;
        }

        public Condition get_routine_condition_order(Routine routine, int id) {
            var elements = routine.is_a_randomizer() ? randomizerElementsOrder : elementsOrder;
            if (id < elements.Count && id >= 0) {
                return ((RoutineInfo)elements[id]).condition();
            }
            return null;
        }

        public List<Condition> get_routine_conditions_order(Routine routine) {

            List<Condition> conditions = new List<Condition>();
            var elements = routine.is_a_randomizer() ? randomizerElementsOrder : elementsOrder;
            foreach (var info in elements) {
                if (info.key() == routine.key()) {
                    conditions.Add(((RoutineInfo)info).condition());
                }
            }
            return conditions;
        }

        public List<string> get_routine_conditions_names_order(Routine routine) {

            List<string> conditionsName = new List<string>();
            var elements = routine.is_a_randomizer() ? randomizerElementsOrder : elementsOrder;
            foreach (var info in elements) {
                if (info.key() == routine.key()) {
                    conditionsName.Add(((RoutineInfo)info).condition().name);
                }
            }
            return conditionsName;
        }
    }
}