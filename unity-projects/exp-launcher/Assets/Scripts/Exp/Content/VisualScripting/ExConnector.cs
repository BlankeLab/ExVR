
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
using System.Text;
using System.Runtime.CompilerServices;

// unity
using UnityEngine;

namespace Ex {

    public class ExConnector : MonoBehaviour {

        // enums
        public enum Function {
            initialize,
            update_from_gui,
            pre_start_routine,
            start_routine,
            stop_routine,
            pre_update,
            update,
            post_update,
            slot1,
            slot2,
            slot3,
            slot4,
            slot5,
            slot6,
            slot7,
            slot8,
            slot9,
            slot10,
            undefined
        }
        public enum Category {
            Generator,
            Operator,
            Function,
            Convertor,
            Action,
            Flow,
            Resource,
            Component,            
            Undefined
        };

        public enum Pritority { Low, Medium, Hight };

        // signals / slots
        protected static readonly string valueStr = "value";
        private static readonly string uiId = "v";

        protected static readonly List<string> slotsStr = new List<string>(new string[] {
            "input value 0",
            "input value 1",
            "input value 2",
            "input value 3",
            "input value 4",
            "input value 5",
            "input value 6",
            "input value 7",
        });

        protected static readonly List<string> signalsStr = new List<string>(new string[] {
            "output value 0",
            "output value 1",
            "output value 2",
            "output value 3",
            "output value 4",
            "output value 5",
            "output value 6",
            "output value 7",
        });

        // state
        public int key = -1;
        public string keyStr;
        public Category category = Category.Undefined;
        public Pritority priority = Pritority.Medium;
        public Function currentFunction = Function.undefined;
        private bool m_initialized = false;
        private bool m_catchExceptions = false;

        // associated to
        public Routine associatedRoutine = null;
        public Condition associatedCondition = null;
        protected Config m_config = null;

        // connections
        private Events.Connections m_connections = null;
        public List<GameObject> inputGO = new List<GameObject>();
        public List<GameObject> outputGO = new List<GameObject>();
        public List<Events.ConnectionInfo> inputConnections = new List<Events.ConnectionInfo>();
        public List<Events.ConnectionInfo> outputConnections = new List<Events.ConnectionInfo>();

        // access        
        public Components components() { return ExVR.Components(); }
        public Log log() { return ExVR.Log(); }
        public TimeManager time() { return ExVR.Time(); }
        public EventsManager events() { return ExVR.Events(); }
        public Events.Connections connections() { return m_connections; }
        public Events.Command command() { return events().command; }


        protected void send_connector_infos_to_gui(object value) {
            ExVR.Network().gui_ipc().send_connector_infos_to_GUI(
                associatedRoutine.key_str(),
                associatedCondition.key_str(),
                keyStr,
                uiId,
                Converter.to_string(value)
            );
        }

        protected void send_connector_infos_to_gui(string value) {
            ExVR.Network().gui_ipc().send_connector_infos_to_GUI(
                associatedRoutine.key_str(),
                associatedCondition.key_str(),
                keyStr,
                uiId,
                value
            );
        }

        public void add_slot(int id, System.Action<object> action) {

            if (id < 0 || id > 8) {
                log_error(string.Format("Invalid slod id {0}", id.ToString()));
                return;
            }

            m_connections.add_slot(slotsStr[id], action);
        }

        protected void add_signals(int count) {

            if (count <= 0 || count > 8) {
                log_error(string.Format("Invalid number of slots {0}", count.ToString()));
                return;
            }

            for (int ii = 0; ii < count; ++ii) {
                m_connections.add_signal(signalsStr[ii]);
            }
        }

        protected void invoke_signal(int id, object arg = null) {
            m_connections.invoke_signal(signalsStr[id], arg);
        }

        protected string verbose_name() {
            return String.Format(" [From connector with name [{0}] of type [{1}] and key [{2}] from function [{3}]]",
                name,
                this.GetType().ToString(),
                key,                
                currentFunction.ToString()
            );
        }



        // logs
        public void log_message(object message, bool verbose = false, bool extra = false,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0) {
            if (verbose) {
                log().message(string.Concat(Converter.to_string(message), verbose_name()), extra, memberName, sourceFilePath, sourceLineNumber);
            } else {
                log().message(message, extra, memberName, sourceFilePath, sourceLineNumber);
            }
        }

        public void log_warning(object warning, bool verbose = true, bool extra = false,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0) {
            if (verbose) {
                log().warning(string.Concat(Converter.to_string(warning), verbose_name()), extra, memberName, sourceFilePath, sourceLineNumber);
            } else {
                log().warning(warning, extra, memberName, sourceFilePath, sourceLineNumber);
            }
        }

        public void log_error(object error, bool verbose = true, bool extra = true,
            [CallerMemberName] string memberName = "",
            [CallerFilePath] string sourceFilePath = "",
            [CallerLineNumber] int sourceLineNumber = 0) {
            if (verbose) {
                log().error(string.Concat(Converter.to_string(error), verbose_name()), extra, memberName, sourceFilePath, sourceLineNumber);
            } else {
                log().error(error, extra, memberName, sourceFilePath, sourceLineNumber);
            }
        }

        protected void display_exception(Exception e) {

            log_error(string.Format("[ERROR-EXCEPTION] "), true, false);
            log_error(string.Format("[MESSAGE] {0}", e.Message), false, false);
            log_warning(string.Format("\t[SOURCE] {0}", e.Source), false, false);
            log_warning(string.Format("\t[TARGET] {0}", e.TargetSite.ToString()), false, false);
            log_warning(string.Format("\t[STACK]: {0}", e.StackTrace), false, false);
        }

        public void setup_connector_object(XML.Connector xmlConnector) {

            m_connections = new Events.Connections(gameObject.name);

            m_catchExceptions = ExVR.GuiSettings().catchComponentsExceptions;
            currentFunction = Function.initialize;

            key = xmlConnector.Key;
            keyStr = Converter.to_string(xmlConnector.Key);
            m_config = gameObject.AddComponent<Config>();
            m_config.init_from_xml(xmlConnector.Arg);
        }

        public bool base_initialize() {

            if (m_catchExceptions) {
                try {
                    m_initialized = initialize();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                m_initialized = initialize();
            }

            return m_initialized;
        }


        public void base_update_from_gui(XML.Arg arg) {

            m_config.init_from_xml(arg);
            currentFunction = Function.update_from_gui;

            if (m_catchExceptions) {
                try {
                    update_from_gui();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                update_from_gui();
            }
        }

        public void base_pre_start_routine() {

            currentFunction = Function.pre_start_routine;

            if (m_catchExceptions) {
                try {
                    pre_start_routine();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                pre_start_routine();
            }
        }

        public void base_start_routine() {

            currentFunction = Function.start_routine;

            if (m_catchExceptions) {
                try {
                    start_routine();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                start_routine();
            }
        }


        public void base_pre_update() {

            currentFunction = Function.pre_update;

            if (m_catchExceptions) {
                try {
                    pre_update();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                pre_update();
            }
        }
        public void base_update() {

            currentFunction = Function.update;

            if (m_catchExceptions) {
                try {
                    update();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                update();
            }
        }
        public void base_post_update() {

            currentFunction = Function.post_update;

            if (m_catchExceptions) {
                try {
                    post_update();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                post_update();
            }
        }

        public void base_stop_routine() {

            currentFunction = Function.stop_routine;

            if (m_catchExceptions) {
                try {
                    stop_routine();
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                stop_routine();
            }
        }

        protected void base_slot1(object arg) {

            currentFunction = Function.slot1;

            if (m_catchExceptions) {
                try {
                    slot1(arg);
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                slot1(arg);
            }
        }

        protected void base_slot2(object arg) {

            currentFunction = Function.slot2;

            if (m_catchExceptions) {
                try {
                    slot2(arg);
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                slot2(arg);
            }
        }

        protected void base_slot3(object arg) {

            currentFunction = Function.slot3;

            if (m_catchExceptions) {
                try {
                    slot3(arg);
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                slot3(arg);
            }
        }

        protected void base_slot4(object arg) {

            currentFunction = Function.slot4;

            if (m_catchExceptions) {
                try {
                    slot4(arg);
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                slot4(arg);
            }
        }

        protected void base_slot5(object arg) {

            currentFunction = Function.slot5;

            if (m_catchExceptions) {
                try {
                    slot5(arg);
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                slot5(arg);
            }
        }

        protected void base_slot6(object arg) {

            currentFunction = Function.slot6;

            if (m_catchExceptions) {
                try {
                    slot6(arg);
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                slot6(arg);
            }
        }

        protected void base_slot7(object arg) {

            currentFunction = Function.slot7;

            if (m_catchExceptions) {
                try {
                    slot7(arg);
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                slot7(arg);
            }
        }

        protected void base_slot8(object arg) {

            currentFunction = Function.slot8;

            if (m_catchExceptions) {
                try {
                    slot8(arg);
                } catch (Exception e) {
                    display_exception(e);
                }
            } else {
                slot8(arg);
            }
        }

        protected virtual bool initialize() { return true; }
        protected virtual void pre_start_routine() { }
        protected virtual void start_routine() { }
        protected virtual void update_from_gui() { }
        protected virtual void pre_update() { }
        protected virtual void update() { }
        protected virtual void post_update() { }
        protected virtual void stop_routine() { }

        protected virtual void slot1(object arg) { }
        protected virtual void slot2(object arg) { }
        protected virtual void slot3(object arg) { }
        protected virtual void slot4(object arg) { }
        protected virtual void slot5(object arg) { }
        protected virtual void slot6(object arg) { }
        protected virtual void slot7(object arg) { }
        protected virtual void slot8(object arg) { }
    }
}