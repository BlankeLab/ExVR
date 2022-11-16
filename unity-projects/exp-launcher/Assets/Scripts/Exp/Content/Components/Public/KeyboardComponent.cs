
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
using System.Runtime.InteropServices;
using System.Threading;
using System.Collections.Concurrent;
using System.Diagnostics;

// unity
using UnityEngine;
using UnityEngine.Profiling;

namespace Ex {

    public class VirtualKeyGetterThread : ThreadedJob {


        volatile public bool doLoop = false;
        volatile public bool process = false;
        public HashSet<KeyCode> keysToFIlter = null;
        public ConcurrentQueue<Input.KeyboardButtonEvent> newEvents = new ConcurrentQueue<Input.KeyboardButtonEvent>();
        private Dictionary<Input.VirtualButton, Input.KeyboardButtonEvent> m_keyEvents = new Dictionary<Input.VirtualButton, Input.KeyboardButtonEvent>();


        [DllImport("User32.dll")]
        public static extern short GetAsyncKeyState(int key);
        protected override void thread_function() {

            Thread.CurrentThread.Name = "VirtualKeyGetterThread";
            Profiler.BeginThreadProfiling("VirtualKeyGetterThread", "VirtualKeyGetterThread 1");
           
            // generate key events
            foreach (var corr in Input.Keyboard.vrCodeCorrespondence) {
                if (keysToFIlter != null) {
                    if (keysToFIlter.Contains(corr.Value)) {
                        m_keyEvents[corr.Key] = new Input.KeyboardButtonEvent(corr.Value);
                    }
                } else {
                    m_keyEvents[corr.Key] = new Input.KeyboardButtonEvent(corr.Value);
                }                    
            }

            Stopwatch sw = new Stopwatch();
            sw.Restart();
            //long totalMs = 0;
            //long totalTicsk = 0;
            //long nb = 0;
            //long superior1 = 0;
            //long superior2 = 0;
            //long superior5 = 0;

            // 1 tick is 100 nanoseconds
            // 1000 ticks is 10000 nanoseconds -> 0.1 ms
            //long ticks = 1;
            //var ts = new System.TimeSpan(ticks); // 1000
            //long maxT = -10;
            //long minT = 100000;
            long count = 0;
            var tm = ExVR.Time();

            long expTicks = 0;
            long elementTicks = 0;
            while (doLoop) {

                if (process) {
                    
                    expTicks     = tm.ellapsed_exp_ticks();
                    elementTicks = tm.ellapsed_element_ticks();

                    foreach (var keyEvent in m_keyEvents) {
                        keyEvent.Value.update(System.Convert.ToBoolean(GetAsyncKeyState((int)keyEvent.Key) & 0x8001), expTicks, elementTicks);
                        if(keyEvent.Value.state != Input.Button.State.None) {
                            newEvents.Enqueue(keyEvent.Value.copy());
                        }
                    }
                }


                //Thread.Sleep(ts);
                Thread.SpinWait(1);
                ++count;
                //var nbMs = sw.ElapsedMilliseconds;
                //if (nbMs != 0) {
                //    UnityEngine.Debug.Log("nbMs" + nbMs);
                //}
                //var nbTicks = sw.ElapsedTicks;
                //if(maxT < nbTicks) {
                //    maxT = nbTicks;
                //}
                //if(minT > nbTicks){
                //    minT = nbTicks;
                //}
                //totalMs += nbMs;
                //totalTicsk += nbTicks;
                //if(nbMs >= 0) {
                //    superior1++;
                //}
                //if (nbMs >= 1) {
                //    superior2++;
                //}
                //if (nbMs >= 5) {
                //    superior5++;
                //}
                //++nb;
            }
            //ExVR.Log().error("total: " + ticks + " " + (1.0 * totalMs / nb) + " " + (1.0 * totalTicsk / nb) +" " + superior1 +" " + superior2 + " "+ superior5);
            //ExVR.Log().error("-> average ms " + (1.0 * sw.ElapsedMilliseconds / count));

            Profiler.EndThreadProfiling();
        }
    }



    public class KeyboardComponent : ExComponent{
        
        // signals
        private static readonly string buttonOnGuiSignal  = "button";
        // infos
        private static readonly string infosSignal  = "buttons_state_info";
        // return values
        private Dictionary<KeyCode, bool> m_keyboardGetReturn = new Dictionary<KeyCode, bool>();

        // events
        // # all events states
        public Dictionary<KeyCode, Input.KeyboardButtonEvent> buttonsEvent  = new Dictionary<KeyCode, Input.KeyboardButtonEvent>();
        [System.Obsolete("Use buttonsEvent instead.")]
        public Dictionary<KeyCode, Input.KeyboardButtonEvent> buttonsState = null;
        // # triggers
        private List<Input.KeyboardButtonEvent> triggerEvents = null;

        // thread
        private VirtualKeyGetterThread m_keysGetterThread = null;

        #region ex_functions
        protected override bool initialize() {

            // add signal
            add_signal(buttonOnGuiSignal);

            // generate filter
            HashSet<KeyCode> keysToFIlter = null;
            if (initC.get<bool>("filter")) {
                keysToFIlter = new HashSet<KeyCode>();
                foreach (var line in Text.split_lines(initC.get<string>("keys_to_filter"), true)) {
                    KeyCode code;
                    var codeSstr = line.Contains("KeyCode.") ? line.Substring(8) : line;
                    if (Enum.TryParse(codeSstr, out code)) {
                        keysToFIlter.Add(code);                        
                    }
                }
            }

            // generate key events
            if (keysToFIlter == null) {
                foreach (var code in Input.Keyboard.Codes) {
                    buttonsEvent[code] = new Input.KeyboardButtonEvent(code);
                    m_keyboardGetReturn[code] = false;
                }
            } else {
                foreach (var code in keysToFIlter) {
                    buttonsEvent[code] = new Input.KeyboardButtonEvent(code);
                    m_keyboardGetReturn[code] = false;
                }
            }
            buttonsState = buttonsEvent;

            if (ExVR.GuiSettings().catchExternalKeyboardEvents) {
                // init thread
                m_keysGetterThread = new VirtualKeyGetterThread();
                m_keysGetterThread.keysToFIlter = keysToFIlter;
                m_keysGetterThread.doLoop = true;
                m_keysGetterThread.start(System.Threading.ThreadPriority.AboveNormal);
            }

            return true;
        }

        protected override void start_experiment() {

            if (!ExVR.GuiSettings().catchExternalKeyboardEvents) {
                return;
            }

            m_keysGetterThread.process = true;
        }
        
        protected override void stop_experiment() {

            if (!ExVR.GuiSettings().catchExternalKeyboardEvents) {
                return;
            }

            m_keysGetterThread.process = false;
        }

        protected override void clean() {

            if (!ExVR.GuiSettings().catchExternalKeyboardEvents) {
                return;
            }

            // stop thread
            m_keysGetterThread.process = false;
            m_keysGetterThread.doLoop = false;
            if (!m_keysGetterThread.join(500)) {
                log_error(string.Format("Stop acquisition thread timeout."));
            }
            m_keysGetterThread = null;
         
        }

        protected override void first_event(bool isCalledBeforeUpdate) {

            if (ExVR.GuiSettings().catchExternalKeyboardEvents) {
                return;
            }

            // reset triggers
            triggerEvents = null;
        }

        protected override void keyboard_event(Event kEvent) {

            if (ExVR.GuiSettings().catchExternalKeyboardEvents) {
                return;
            }

            // call several times per frame
            process_keyboard();
        }

        protected override void last_event() {

            if (ExVR.GuiSettings().catchExternalKeyboardEvents) {
                return;
            }

            // store pressed events only once
            foreach (var kEvent in buttonsEvent) {

                if (kEvent.Value.state == Input.Button.State.Pressed) {

                    if (triggerEvents == null) {
                        triggerEvents = new List<Input.KeyboardButtonEvent>();
                    }
                    triggerEvents.Add(kEvent.Value.copy());
                }
            }

            send_infos_and_signals();
        }

        protected override void pre_update() {

            if (!ExVR.GuiSettings().catchExternalKeyboardEvents) {
                return;
            }

            // reset triggers
            triggerEvents = null;

            // call once per frame
            process_external_keyboard();
            send_infos_and_signals();
        }


        public override string format_frame_data_for_global_logger(bool header) {

            if (header) {
                return "keys_pressed";
            }

            if (triggerEvents == null) {
                return "none";
            }

            List<Input.KeyboardButtonEvent> events = null;
            foreach (var kEvent in triggerEvents) {

                if (kEvent.state != Input.Button.State.Pressed) {
                    continue;
                }

                if (events == null) {
                    events = new List<Input.KeyboardButtonEvent>();
                }
                events.Add(kEvent);
            }

            if (events == null) {
                return "none";
            }

            StringBuilder sb = new StringBuilder();
            foreach (var bEvent in events) {
                sb.AppendFormat("{0} ", Enum.GetName(typeof(KeyCode), bEvent.code));
            }
            return sb.ToString();
        }

        public override List<Tuple<double, double, string>> format_trigger_data_for_global_logger() {

            if (triggerEvents == null) {
                return null;
            }

            List<Input.KeyboardButtonEvent> events = null;
            foreach (var kEvent in triggerEvents) {
                if (kEvent.state == Input.Button.State.Pressed) {
                    continue;
                }

                if (events == null) {
                    events = new List<Input.KeyboardButtonEvent>();
                }
                events.Add(kEvent);
            }

            if (events == null) {
                return null;
            }

            List<Tuple<double, double, string>> triggersStr = events.Count > 0 ? new List<Tuple<double, double, string>>(events.Count) : null;
            foreach (var bEvent in events) {
                triggersStr.Add(new Tuple<double, double, string>
                    (bEvent.triggeredExperimentTime,
                    bEvent.triggeredElementTime,
                    string.Format("{0} {1}", Enum.GetName(typeof(KeyCode), bEvent.code), Enum.GetName(typeof(Input.Button.State), bEvent.state))
                ));
            }
            return triggersStr;
        }

        #endregion

        #region private_functions

        private void process_external_keyboard() {

            Input.KeyboardButtonEvent kEvent;
            while (m_keysGetterThread.newEvents.TryDequeue(out kEvent)) {

                // update all events
                buttonsEvent[kEvent.code].update(kEvent);

                // store up and down events
                if (kEvent.state == Input.Button.State.Down || kEvent.state == Input.Button.State.Up) {
                    if(triggerEvents == null) {
                        triggerEvents = new List<Input.KeyboardButtonEvent>();
                    }
                    triggerEvents.Add(kEvent.copy());
                }
            }

            // store pressed events
            foreach(var bEvent in buttonsEvent) {
                if(bEvent.Value.state == Input.Button.State.Pressed) {
                    if (triggerEvents == null) {
                        triggerEvents = new List<Input.KeyboardButtonEvent>();
                    }
                    triggerEvents.Add(bEvent.Value.copy());
                }
            }
        }

        private void process_keyboard() {

            // retrieve current return value
            var currentExpTime = time().ellapsed_exp_ms();
            var currentElementTime = time().ellapsed_element_ms();
            foreach (var kEvent in buttonsEvent) {
                m_keyboardGetReturn[kEvent.Key] = UnityEngine.Input.GetKey(kEvent.Key);
            }

            // update events
            foreach (var kEvent in buttonsEvent) {

                // update all events
                kEvent.Value.update(m_keyboardGetReturn[kEvent.Key], currentExpTime, currentElementTime);

                // store down/up events
                if (kEvent.Value.state == Input.Button.State.Up || kEvent.Value.state == Input.Button.State.Down) {

                    if(triggerEvents == null) {
                        triggerEvents = new List<Input.KeyboardButtonEvent>();
                    }
                    triggerEvents.Add(kEvent.Value.copy());
                }
            }
        }

        private void send_infos_and_signals() {

            StringBuilder infos = null;

            // send signals
            if (triggerEvents != null) {
                foreach (var kEvent in triggerEvents) {

                    if (kEvent.state == Input.Button.State.None) {
                        continue;
                    }

                    if (is_updating()) {
                        invoke_signal(buttonOnGuiSignal, kEvent);
                    }

                    if (kEvent.state == Input.Button.State.Pressed) {
                        if (infos == null) {
                            infos = new StringBuilder();
                        }
                        infos.AppendFormat("{0} ", ((int)kEvent.code).ToString());
                    }
                }
            }

            // send infos
            if (infos != null) {
                if (infos.Length > 0) {
                    send_infos_to_gui_init_config(infosSignal, infos.ToString());
                }
            } else {
                send_infos_to_gui_init_config(infosSignal, "");
            }
        }

        #endregion

    }
}