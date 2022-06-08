
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
using UnityEngine.Events;

// system;
using System.Collections;

namespace Ex{

    namespace Events {

        // base
        public class IntEvent : UnityEvent<int> { }
        public class StringEvent : UnityEvent<string> { }
        public class String4Event : UnityEvent<string, string, string, string> { }
        public class ExComponentMessageEvent : UnityEvent<ExComponent, string> { }


        public class Command {

            public UnityEvent NextElementEvent      = new UnityEvent();
            public UnityEvent PreviousElementEvent  = new UnityEvent();
            public UnityEvent StopExperimentEvent   = new UnityEvent();
            public UnityEvent PauseExperimentEvent  = new UnityEvent();

            public Events.IntEvent ScheduleNextEvent = new Events.IntEvent();

            public Events.StringEvent NextElementWithNameEvent          = new Events.StringEvent();
            public Events.StringEvent PreviousElementWithNameEvent      = new Events.StringEvent();
            public Events.StringEvent NextElementWithConditionEvent     = new Events.StringEvent();
            public Events.StringEvent PreviousElementWithConditionEvent = new Events.StringEvent();
            public Events.IntEvent GoToSpecificInstanceElementEvent     = new Events.IntEvent();
            public Events.String4Event ModifyRoutineActionConfigEvent   = new Events.String4Event();

            public void next() {
                NextElementEvent.Invoke();
            }

            IEnumerator next_in(int milliseconds) {
                yield return new WaitForSeconds(milliseconds * 0.001f);
                next();
            }

            public void schedule_next(int milliseconds) {
                ExVR.Coroutines().start(next_in(milliseconds));
            }

            public void previous() {
                PreviousElementEvent.Invoke();
            }

            public void pause_experiment() {
                PauseExperimentEvent.Invoke();
            }

            public void force_stop_experiment() {
                StopExperimentEvent.Invoke();
            }

            public void next_element_with_name(string elementName) {
                NextElementWithNameEvent.Invoke(elementName);
            }

            public void previous_element_with_name(string elementName) {
                PreviousElementWithNameEvent.Invoke(elementName);
            }

            public void next_element_with_condition(string elementName) {
                NextElementWithConditionEvent.Invoke(elementName);
            }

            public void previous_element_with_condition(string elementName) {
                PreviousElementWithConditionEvent.Invoke(elementName);
            }

            public void modify_routine_action_config(string routineName, string conditionName, string componentName, string newConfigName) {
                ModifyRoutineActionConfigEvent.Invoke(routineName, conditionName, componentName, newConfigName);
            }
        }

        public class Gui {
            public Events.StringEvent MessageFromGUI = new Events.StringEvent();
        }

        public class Log {

            public class MessageEvent : UnityEvent<string, bool> { }

            public MessageEvent Error = new MessageEvent();            
            public MessageEvent Warning = new MessageEvent();            
            public MessageEvent Message = new MessageEvent();
            public MessageEvent LoggerError = new MessageEvent();
            public MessageEvent LoggerWarning = new MessageEvent();
            public MessageEvent LoggerMessage = new MessageEvent();
        }

        public class Stacktrace {
            public Events.ExComponentMessageEvent ComponentTrace = new Events.ExComponentMessageEvent();
        }
    }

    public class EventsManager : MonoBehaviour{

        public Events.Log log = null;
        public Events.Gui gui = null;
        public Events.Command command = null;
        public Events.Stacktrace stacktrace = null;

        public void initialize() {
  
            log = new Events.Log();
            gui = new Events.Gui();
            command = new Events.Command();
            stacktrace = new Events.Stacktrace();

            // implement counters...
            // ...
        }
    }
}
