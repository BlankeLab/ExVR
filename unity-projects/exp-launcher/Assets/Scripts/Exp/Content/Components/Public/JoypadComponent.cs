
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

// unity
using UnityEngine;

namespace Ex{

    public class JoypadComponent : ExComponent{

        public string buttonsInfos = "";
        public string axisInfos = "";
        public string joystickInfos = "";
        public Dictionary<Input.JoypadAxis.Code, float> axisDeadZones = new Dictionary<Input.JoypadAxis.Code, float>();

        private bool sendInfos = false;        

        // signals
        private static readonly string axisOnGuiSignal = "axis";
        private static readonly string buttonOnGuiSignal = "button";
        // infos
        private static readonly string axisInfoSignal     = "axes_state_info";
        private static readonly string buttonsInfoSignal  = "buttons_state_info";
        private static readonly string joypadInfoSignal   = "joysticks_info";

        // events
        // # axis
        private Dictionary<Input.JoypadAxis.Code, Input.JoypadAxisEvent> axisEvent = new Dictionary<Input.JoypadAxis.Code, Input.JoypadAxisEvent>();
        // # buttons
        private Dictionary<Input.JoypadButton.Code, Input.JoypadButtonEvent> buttonsEvent = new Dictionary<Input.JoypadButton.Code, Input.JoypadButtonEvent>();

        // states
        public Dictionary<Input.JoypadAxis.Code, Input.JoypadAxisState> axisState = new Dictionary<Input.JoypadAxis.Code, Input.JoypadAxisState>();
        public Dictionary<Input.JoypadButton.Code, Input.JoypadButtonState> buttonsState = new Dictionary<Input.JoypadButton.Code, Input.JoypadButtonState>();
        

        protected override bool initialize() {

            add_signal(axisOnGuiSignal);
            add_signal(buttonOnGuiSignal);

            foreach(var code in Input.JoypadAxis.Codes) {
                axisEvent[code] = new Input.JoypadAxisEvent(code);
                axisState[code] = new Input.JoypadAxisState(code);
            }

            foreach(var code in Input.JoypadButton.Codes) {
                buttonsEvent[code] = new Input.JoypadButtonEvent(code);
                buttonsState[code] = new Input.JoypadButtonState(code);
            }

            foreach (var code in Input.JoypadAxis.Codes) {
                axisDeadZones[code] = Converter.to_float(initC.get<double>(string.Format("dead_zone_{0}", Converter.to_string((int)code))));
            }

            send_joystick_info();

            return true;
        }

        protected override void start_experiment() {
            send_joystick_info();
        }

        protected override void update() {
            // reset states infos
            sendInfos = true;
        }

        protected override void set_update_state(bool doUpdate) {

            if (!doUpdate) {

                // reset states
                var currentTime = time().ellapsed_exp_ms();
                foreach (Input.JoypadButton.Code buttonCode in Input.JoypadButton.Codes) {
                    var currentEvent = buttonsEvent[buttonCode];
                    var currentState = buttonsState[buttonCode];
                    currentEvent.update(false, currentTime);
                    currentState.update(false, currentTime);
                }

                foreach (Input.JoypadAxis.Code axisCode in Input.JoypadAxis.Codes) {
                    var currentEvent = axisEvent[axisCode];
                    var currentState = axisState[axisCode];
                    currentEvent.update(0f, currentTime);
                    currentState.update(0f, currentTime);
                }
            }
        }

        protected override void on_gui() {

            var eventType = Event.current.type;
            if (eventType != EventType.Repaint) {
                return;
            }
            var currentTime = time().ellapsed_exp_ms();


            var buttonsCodeInfoToSend = new List<Input.JoypadButton.Code>();
            foreach (Input.JoypadButton.Code buttonCode in Input.JoypadButton.Codes) {

                bool pressed = UnityEngine.Input.GetButton(Input.JoypadButton.CodesNames[buttonCode]);

                // update event
                var currentEvent = buttonsEvent[buttonCode];
                var currentState = buttonsState[buttonCode];
                var previousState = currentEvent.state;
                currentEvent.update(pressed, currentTime);
                currentState.update(pressed, currentTime);

                bool notNone = currentEvent.state != Input.Button.State.None;
                if (notNone || (previousState != currentEvent.state)) {
                    buttonsCodeInfoToSend.Add(buttonCode);
                }

                if (notNone) {
                    invoke_signal(buttonOnGuiSignal, currentEvent);
                }
            }

            var axisCodeInfoToSend = new List<Input.JoypadAxis.Code>();
            foreach (Input.JoypadAxis.Code axisCode in Input.JoypadAxis.Codes) {

                float value = UnityEngine.Input.GetAxis(Input.JoypadAxis.CodesNames[axisCode]);

                if (Math.Abs(value) < axisDeadZones[axisCode]) {
                    value = 0f;
                }
                
                var currentEvent = axisEvent[axisCode];
                var currentState = axisState[axisCode];
                var previousValue = currentEvent.value;
                currentEvent.update(value, currentTime);
                currentState.update(value, currentTime);

                bool notNone = currentEvent.value != 0f;
                if (notNone || (previousValue != currentEvent.value)) {
                    axisCodeInfoToSend.Add(axisCode);
                }

                if (notNone) { 
                    invoke_signal(axisOnGuiSignal, currentEvent);
                }
            }

            // send infos only once per frame
            if (sendInfos) {
                StringBuilder infos = new StringBuilder();
                int currentCode = 0;
                foreach (var code in buttonsCodeInfoToSend) {           
                    if (currentCode != buttonsCodeInfoToSend.Count-1) {
                        infos.AppendFormat("{0},", ((int)code).ToString());
                    } else {
                        infos.Append(((int)code).ToString());
                    }
                    ++currentCode;
                }
                send_infos_to_gui_init_config(buttonsInfoSignal, buttonsInfos = infos.ToString());
            }
            if(sendInfos) {
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
                send_infos_to_gui_init_config(axisInfoSignal, axisInfos = infos.ToString());
            }

            sendInfos = false;
        }

        void send_joystick_info() {

            StringBuilder joystickStr = new StringBuilder();
            int ii = 0;
            foreach (string joystick in UnityEngine.Input.GetJoystickNames()) {
                if (joystick.Length > 0) {
                    log_message(string.Format("Joystick n°{0} detected: {1}", ii, joystick));
                    joystickStr.AppendFormat("{0}%", joystick);
                    ++ii;
                }
            }
            if(ii == 0) {
                log_warning("No joystick detected.", false);
            }
            send_infos_to_gui_init_config(joypadInfoSignal, joystickInfos = joystickStr.ToString());

        }
    }
}