
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
using System.Runtime.InteropServices;
using System.Threading;
using System.Collections.Concurrent;

// unity
using UnityEngine;
using UnityRawInput;
using UnityEngine.Profiling;


namespace Ex {

    public class VirtualKeyGetterThread : ThreadedJob {



        volatile public bool doLoop = false;
        volatile public bool process = false;
        public ConcurrentQueue<Input.KeyboardButtonEvent> newEvents = new ConcurrentQueue<Input.KeyboardButtonEvent>();
        private Dictionary<Input.VirtualButton, Input.KeyboardButtonEvent> m_keyEvents = new Dictionary<Input.VirtualButton, Input.KeyboardButtonEvent>();


        [DllImport("User32.dll")]
        public static extern short GetAsyncKeyState(int key);
        protected override void thread_function() {

            Thread.CurrentThread.Name = "VirtualKeyGetterThread";
            Profiler.BeginThreadProfiling("VirtualKeyGetterThread", "VirtualKeyGetterThread 1");
           
            foreach (var corr in Input.Keyboard.vrCodeCorrespondence) {
                m_keyEvents[corr.Key] = new Input.KeyboardButtonEvent(corr.Value);
            }

            // 1 tick is 100 nanoseconds
            // 1000 ticks is 10000 nanoseconds -> 0.1 ms
            var ts = new System.TimeSpan(3000); // 1000
            while (doLoop) {

                if (process) {
                    
                    var expTime     = ExVR.Time().ellapsed_exp_ms();
                    var elementTime = ExVR.Time().ellapsed_element_ms();

                    foreach (var keyEvent in m_keyEvents) {
                        bool keyPressed = Converter.to_bool(GetAsyncKeyState((int)keyEvent.Key) & 0x8001);
                        keyEvent.Value.update(keyPressed, expTime, elementTime);

                        if(keyEvent.Value.state != Input.Button.State.None) {
                            newEvents.Enqueue(keyEvent.Value.copy());
                        }
                    }
                }

                
                Thread.Sleep(ts);
                //Thread.Sleep(10);
            }

            Profiler.EndThreadProfiling();
        }
    }



    public class KeyboardComponent : ExComponent{

        private bool sendInfos = false;

        // signals
        private static readonly string buttonOnGuiSignal  = "button";
        // infos
        private static readonly string infosSignal  = "buttons_state_info";

        // events
        // # buttons
        private Dictionary<KeyCode, bool> m_keyboardGetReturn = new Dictionary<KeyCode, bool>();
        private Dictionary<KeyCode, Input.KeyboardButtonEvent> m_buttonsEvent  = new Dictionary<KeyCode, Input.KeyboardButtonEvent>();

        //private Dictionary<RawKey, Input.KeyboardButtonEvent> m_rawButtonsEvent = new Dictionary<RawKey, Input.KeyboardButtonEvent>();

        VirtualKeyGetterThread thread = null;
        protected override bool initialize() {

            add_signal(buttonOnGuiSignal);


            if (!ExVR.GuiSettings().catchExternalKeyboardEvents) {

                foreach (var code in Input.Keyboard.Codes) {
                    m_buttonsEvent[code]      = new Input.KeyboardButtonEvent(code);
                    m_keyboardGetReturn[code] = false;
                }

            } else {

                //foreach (var button in Input.Keyboard.RawCodesCorrespondence) {
                //    m_rawButtonsEvent[button.Key] = new Input.KeyboardButtonEvent(button.Value);
                //}

                thread = new VirtualKeyGetterThread();
                thread.doLoop = true;
                thread.start();
            }


            return true;
        }


        protected override void update() {
            // reset states infos
            sendInfos = true;
        }

        protected override void set_update_state(bool doUpdate) {

            if (!ExVR.GuiSettings().catchExternalKeyboardEvents && !doUpdate) {

                // reset states
                var currentExpTime     = time().ellapsed_exp_ms();
                var currentElementTime = time().ellapsed_element_ms();
                foreach (KeyCode keyCode in Input.Keyboard.Codes) {
                    m_buttonsEvent[keyCode].update(false, currentExpTime, currentElementTime);
                }

            } else if(ExVR.GuiSettings().catchExternalKeyboardEvents) {

                //RawKeyInput.save_events_state(doUpdate);
                thread.process = doUpdate;
            }
        }

        protected override void on_gui() {

            var eventType = Event.current.type;
            if (eventType != EventType.KeyDown && eventType != EventType.KeyUp && eventType != EventType.Repaint) {
                return;
            }


            if (!ExVR.GuiSettings().catchExternalKeyboardEvents) {

                // retrieve current return value
                var currentExpTime     = time().ellapsed_exp_ms();
                var currentElementTime = time().ellapsed_element_ms();
                foreach (KeyCode keyCode in Input.Keyboard.Codes) {
                    m_keyboardGetReturn[keyCode] = UnityEngine.Input.GetKey(keyCode);
                }

                // update events
                foreach (KeyCode keyCode in Input.Keyboard.Codes) {                    
                    m_buttonsEvent[keyCode].update(m_keyboardGetReturn[keyCode], currentExpTime, currentElementTime);   
                }

                // send triggers
                foreach (KeyCode keyCode in Input.Keyboard.Codes) {
                    var currentEvent = m_buttonsEvent[keyCode];
                    if (currentEvent.state != Input.Button.State.None) {
                        invoke_signal(buttonOnGuiSignal, currentEvent);
                    }
                }


                // send infos only once per frame
                if (sendInfos) {

                    StringBuilder infos = new StringBuilder();
                    foreach (KeyCode button in Input.Keyboard.Codes) {
                        var buttonState = m_buttonsEvent[button];
                        if (buttonState.state == Input.Button.State.Pressed || buttonState.state == Input.Button.State.Down) {
                            infos.AppendFormat("{0} ", ((int)button).ToString());
                        }
                    }
                    
                    if (infos.Length > 0) {
                        send_infos_to_gui_init_config(infosSignal, infos.ToString());
                    }
                    sendInfos = false;
                }

            } else if (ExVR.GuiSettings().catchExternalKeyboardEvents) {


                // retrieve events
                List<Input.KeyboardButtonEvent> kDownEvents = null;
                List<Input.KeyboardButtonEvent> kUpEvents   = null;
                Dictionary<KeyCode, Input.KeyboardButtonEvent> KPressedEvents = null;
                {
                    Input.KeyboardButtonEvent kEvent;
                    while (thread.newEvents.TryDequeue(out kEvent)) {
        
                        if (kEvent.state == Input.Button.State.Pressed) { // keep only the last pressed event every frame

                            if (KPressedEvents == null) {
                                KPressedEvents = new Dictionary<KeyCode, Input.KeyboardButtonEvent>();
                            }
                            KPressedEvents[kEvent.code] = kEvent;

                        } else if (kEvent.state == Input.Button.State.Down) { // keep all down events

                            if (kDownEvents == null) {
                                kDownEvents = new List<Input.KeyboardButtonEvent>();
                            }
                            kDownEvents.Add(kEvent);

                        } else if(kEvent.state == Input.Button.State.Up){ // keep all up events

                            if(kUpEvents == null) {
                                kUpEvents = new List<Input.KeyboardButtonEvent>();
                            }
                            kUpEvents.Add(kEvent);                            
                        }
                    }
                }

                StringBuilder infos = null;
                if (sendInfos) {
                    infos = new StringBuilder();
                }

                // send signals
                if (kDownEvents != null) {
                    foreach (var kEvent in kDownEvents) {
                        invoke_signal(buttonOnGuiSignal, kEvent);

                        if (infos != null) {
                            infos.AppendFormat("{0} ", ((int)kEvent.code).ToString());
                        }
                    }
                }
                if (KPressedEvents != null) {
                    foreach (var kEvent in KPressedEvents) {
                        invoke_signal(buttonOnGuiSignal, kEvent.Value);

                        if (infos != null) {
                            infos.AppendFormat("{0} ", ((int)kEvent.Key).ToString());
                        }
                    }
                }
                if (kUpEvents != null) {
                    foreach (var kEvent in kUpEvents) {
                        invoke_signal(buttonOnGuiSignal, kEvent);
                    }
                }

                if (sendInfos) {
                    if (infos.Length > 0) {
                        send_infos_to_gui_init_config(infosSignal, infos.ToString());
                    }
                    sendInfos = false;
                }
            }
        }

        protected override void clean() {

            if (ExVR.GuiSettings().catchExternalKeyboardEvents) {
                // stop threads
                thread.process = false;
                thread.doLoop = false;

                if (!thread.join(500)) {
                    log_error(string.Format("Stop acquisition thread timeout."));
                }
                thread = null;
            }
        }
    }
}