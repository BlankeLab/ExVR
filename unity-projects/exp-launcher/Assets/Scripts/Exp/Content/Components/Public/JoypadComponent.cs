
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

namespace Ex{

    public class JoypadComponent : ExComponent{

        // signals
        private static readonly string axisOnGuiSignal    = "axis";
        private static readonly string buttonOnGuiSignal  = "button";
        // infos
        private static readonly string axisInfoSignal     = "axes_state_info";
        private static readonly string buttonsInfoSignal  = "buttons_state_info";
        private static readonly string joypadInfoSignal   = "joysticks_info";

        // dead zones
        private Dictionary<Input.JoypadAxis.Code, float> m_axisDeadZones = new Dictionary<Input.JoypadAxis.Code, float>();

        // events
        // # axis
        public Dictionary<Input.JoypadAxis.Code, Input.JoypadAxisEvent> axisEvent = new Dictionary<Input.JoypadAxis.Code, Input.JoypadAxisEvent>();

        [System.Obsolete("Use axisEvent instead.")]
        public Dictionary<Input.JoypadAxis.Code, Input.JoypadAxisEvent> axisState = null;
        // # buttons
        public Dictionary<Input.JoypadButton.Code, Input.JoypadButtonEvent> buttonsEvent = new Dictionary<Input.JoypadButton.Code, Input.JoypadButtonEvent>();

        public string[] joysticks = null;

        // triggers and infos
        private List<Input.JoypadButton.Code> buttonsCodeInfoToSend = null;
        private List<Input.JoypadAxis.Code> axisCodeInfoToSend = null;
        private List<Input.JoypadButtonEvent> buttonsTriggersEvents = null;

        #region ex_functions

        protected override bool initialize() {

            add_signal(axisOnGuiSignal);
            add_signal(buttonOnGuiSignal);

            foreach(var code in Input.JoypadAxis.Codes) {
                axisEvent[code] = new Input.JoypadAxisEvent(code);
            }
            axisState = axisEvent;

            foreach (var code in Input.JoypadButton.Codes) {
                buttonsEvent[code] = new Input.JoypadButtonEvent(code);
            }

            foreach (var code in Input.JoypadAxis.Codes) {
                m_axisDeadZones[code] = Converter.to_float(initC.get<double>(string.Format("dead_zone_{0}", Converter.to_string((int)code))));
            }

            send_joystick_info();

            return true;
        }

        protected override void start_experiment() {
            send_joystick_info();
        }

        protected override void pre_update() {

            // reset
            buttonsTriggersEvents = null;
            buttonsCodeInfoToSend = null;
            axisCodeInfoToSend = null;

            // process
            process_joypad(); // once per frame

            //invoke_signal(buttonOnGuiSignal, bEvent.Value);
        }


        public override string format_frame_data_for_global_logger(bool header) {

            // axis != 0 | buttons pressed 

            //if (triggerEvents == null) {
            //    return null;
            //}

            //List<Input.KeyboardButtonEvent> events = null;
            //foreach (var kEvent in triggerEvents) {

            //    if (kEvent.state != Input.Button.State.Pressed) {
            //        continue;
            //    }

            //    if (events == null) {
            //        events = new List<Input.KeyboardButtonEvent>();
            //    }
            //    events.Add(kEvent);
            //}

            //if (events == null) {
            //    return null;
            //}

            //StringBuilder sb = new StringBuilder();
            //foreach (var bEvent in events) {
            //    sb.AppendFormat("{0} ", Enum.GetName(typeof(KeyCode), bEvent.code));
            //}
            //return sb.ToString();
            return null;
        }

        public override List<Tuple<double, double, string>> format_trigger_data_for_global_logger() {

            // buttons triggers
            // 

            //if (triggerEvents == null) {
            //    return null;
            //}

            //List<Input.KeyboardButtonEvent> events = null;
            //foreach (var kEvent in triggerEvents) {
            //    if (kEvent.state == Input.Button.State.Pressed) {
            //        continue;
            //    }

            //    if (events == null) {
            //        events = new List<Input.KeyboardButtonEvent>();
            //    }
            //    events.Add(kEvent);
            //}

            //if (events == null) {
            //    return null;
            //}

            //UnityEngine.Debug.LogError("events -> " + events.Count);
            //List<Tuple<double, double, string>> triggersStr = events.Count > 0 ? new List<Tuple<double, double, string>>(events.Count) : null;
            //foreach (var bEvent in events) {
            //    UnityEngine.Debug.LogError("-> " + bEvent.state + " " + bEvent.code);
            //    triggersStr.Add(new Tuple<double, double, string>
            //        (bEvent.triggeredExperimentTime,
            //        bEvent.triggeredElementTime,
            //        string.Format("{0}, {1}", Enum.GetName(typeof(KeyCode), bEvent.code), Enum.GetName(typeof(Input.Button.State), bEvent.state))
            //    ));
            //}
            //return triggersStr;
            return null;
        }


        #endregion

        #region private_functions

        private void send_joystick_info() {

            StringBuilder joystickStr = new StringBuilder();
            int ii = 0;            
            foreach (string joystick in (joysticks = UnityEngine.Input.GetJoystickNames())) {
                if (joystick.Length > 0) {
                    log_message(string.Format("Joystick n°{0} detected: {1}", ii, joystick));
                    joystickStr.AppendFormat("{0}%", joystick);
                    ++ii;
                }
            }
            if(ii == 0) {
                log_warning("No joystick detected.", false);
            }
            send_infos_to_gui_init_config(joypadInfoSignal, joystickStr.ToString());

        }

        private void process_joypad() {

            // get times
            var currentExpTime = time().ellapsed_exp_ms();
            var currentElemTime = time().ellapsed_element_ms();

            // update events
            // # joypad button
            foreach (var bEvent in buttonsEvent) {

                // update
                bEvent.Value.update(UnityEngine.Input.GetButton(Input.JoypadButton.CodesNames[bEvent.Key]), currentExpTime, currentElemTime);

                // store events
                if (bEvent.Value.triggerSignals && is_updating()) {
                    if (buttonsTriggersEvents == null) {
                        buttonsTriggersEvents = new List<Input.JoypadButtonEvent>();
                    }
                    buttonsTriggersEvents.Add(bEvent.Value);                    
                }
            }
            // # joyad axis
            foreach (var aEvent in axisEvent) {
                float value = UnityEngine.Input.GetAxis(Input.JoypadAxis.CodesNames[aEvent.Key]);
                if (Math.Abs(value) < m_axisDeadZones[aEvent.Key]) {
                    value = 0f;
                }
                aEvent.Value.update(value, currentExpTime, currentElemTime);
            }

            // joypad button            
            foreach (var bEvent in buttonsEvent) {


                // store key code of info to send
                if (bEvent.Value.sendInfos) {
                    if(buttonsCodeInfoToSend == null) {
                        buttonsCodeInfoToSend = new List<Input.JoypadButton.Code>();
                    }
                    buttonsCodeInfoToSend.Add(bEvent.Key);
                }
            }

            // joyad axis            
            foreach (var aEvent in axisEvent) {

                // trigger signals
                if (aEvent.Value.triggerSignals && is_updating()) {
                    invoke_signal(axisOnGuiSignal, aEvent.Value);
                }
                // store key code of info to send
                if (aEvent.Value.sendInfos) {
                    if(axisCodeInfoToSend == null) {
                        axisCodeInfoToSend = new List<Input.JoypadAxis.Code>();
                    }
                    axisCodeInfoToSend.Add(aEvent.Key);
                }
            }

            // send infos
            // # joypad button
            if (buttonsCodeInfoToSend != null) {
                StringBuilder infos = new StringBuilder();
                int currentCode = 0;
                foreach (var code in buttonsCodeInfoToSend) {
                    if (currentCode != buttonsCodeInfoToSend.Count - 1) {
                        infos.AppendFormat("{0},{1}%", ((int)code).ToString(), buttonsEvent[code].is_pressed() ? "1" : "0");
                    } else {
                        infos.AppendFormat("{0},{1}", ((int)code).ToString(), buttonsEvent[code].is_pressed() ? "1" : "0");
                    }
                    ++currentCode;
                }
                send_infos_to_gui_init_config(buttonsInfoSignal, infos.ToString());
            }
            // # joypad axis
            if (axisCodeInfoToSend != null) {
                StringBuilder infos = new StringBuilder();
                int currentCode = 0;
                foreach (var code in axisCodeInfoToSend) {
                    var axeEvent = axisEvent[code];
                    if (currentCode != axisCodeInfoToSend.Count - 1) {
                        infos.AppendFormat("{0},{1}%", ((int)code).ToString(), Converter.to_string(axeEvent.value, "0.00"));
                    } else {
                        infos.AppendFormat("{0},{1}", ((int)code).ToString(), Converter.to_string(axeEvent.value, "0.00"));
                    }
                    ++currentCode;
                }
                send_infos_to_gui_init_config(axisInfoSignal, infos.ToString());
            }            
        }

        #endregion
    }
}