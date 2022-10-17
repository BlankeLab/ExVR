
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

    public class MouseComponent : ExComponent{

        public string buttonsInfos = "";
        public string axisInfos = "";

        // signals
        private static readonly string buttonOnGuiSignal    = "button";
        private static readonly string axisOnGuiSignal      = "axis";
        private static readonly string positionOnGuiSignal  = "position";
        // infos
        private static readonly string buttonsInfoSignal    = "buttons_state_info";
        private static readonly string axisInfoSignal       = "axis_state_info";
        private static readonly string positionInfoSignal   = "position_state_info";

        // events
        // # axis
        public Dictionary<Input.MouseAxis.Code, Input.MouseAxisEvent> axisEvent = new Dictionary<Input.MouseAxis.Code, Input.MouseAxisEvent>();
        // # buttons
        public Dictionary<KeyCode, Input.MouseButtonEvent> buttonsEvent = new Dictionary<KeyCode, Input.MouseButtonEvent>();
        // # position
        public Vector2 currentMousePosition = Vector2.zero;
        public Vector2 previousMousePosition = Vector2.zero;

        // triggers and infos
        private List<KeyCode> buttonsCodeInfoToSend = null;
        private List<Input.MouseButtonEvent> buttonsTriggersEvents = null;
        private List<Input.MouseAxis.Code> axisCodeInfoToSend = null;
        private List<Input.MouseAxisEvent> axisTriggersEvents = null;
        

        #region ex_functions

        protected override bool initialize() {

            add_signal(buttonOnGuiSignal);
            add_signal(axisOnGuiSignal);            
            add_signal(positionOnGuiSignal);

            foreach (var code in Input.MouseAxis.Codes) {
                axisEvent[code] = new Input.MouseAxisEvent(code);
            }

            foreach (var code in Input.MouseButton.Codes) {
                buttonsEvent[code] = new Input.MouseButtonEvent(code);
            }

            return true;
        }

        protected override void mouse_event(Event mEvent) {

            if (mEvent.type == EventType.MouseDown || mEvent.type == EventType.MouseUp) { 
                process_mouse_buttons(); // several times per frame
            }
        }

        protected override void pre_update() {

            process_mouse_axis(); // once per frame
            
            // send trggers
            if (is_updating()) {

                // buttons events
                if(buttonsTriggersEvents != null) {
                    foreach(var bEvent in buttonsTriggersEvents) {
                        invoke_signal(buttonOnGuiSignal, bEvent);
                    }
                }

                // axis events
                if (axisTriggersEvents != null) {
                    foreach (var aEvent in axisTriggersEvents) {
                        invoke_signal(axisOnGuiSignal, aEvent);
                    }
                }

                // mouse position event
                invoke_signal(positionOnGuiSignal, currentMousePosition);
            }

            // send infos
            // # buttons
            if (buttonsCodeInfoToSend != null) {
                StringBuilder infos = new StringBuilder();
                int currentCode = 0;
                foreach (var code in buttonsCodeInfoToSend) {
                    if (currentCode != buttonsCodeInfoToSend.Count - 1) {
                        infos.AppendFormat("{0},", ((int)code).ToString());
                    } else {
                        infos.Append(((int)code).ToString());
                    }
                    ++currentCode;
                }
                send_infos_to_gui_init_config(buttonsInfoSignal, buttonsInfos = infos.ToString());
            }
            // # axis
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
                send_infos_to_gui_init_config(axisInfoSignal, axisInfos = infos.ToString());
            }
            // # mouse position
            if (previousMousePosition != currentMousePosition) {
                send_infos_to_gui_init_config(positionInfoSignal, Converter.to_string(currentMousePosition));
            }

            // reset infos and triggers
            buttonsCodeInfoToSend = null;
            buttonsTriggersEvents = null;

            axisCodeInfoToSend    = null;
            axisTriggersEvents    = null;
        }

        #endregion

        #region private_functions

        private void process_mouse_buttons() {

            // get times
            var currentExpTime = time().ellapsed_exp_ms();
            var currentElemTime = time().ellapsed_element_ms();

            // update events
            foreach (var bEvent in buttonsEvent) {
                bEvent.Value.update(UnityEngine.Input.GetKey(bEvent.Key), currentExpTime, currentElemTime);
            }

            foreach (var bEvent in buttonsEvent) {

                // trigger signals
                if (bEvent.Value.triggerSignals && is_updating()) {
                    if(buttonsTriggersEvents == null) {
                        buttonsTriggersEvents = new List<Input.MouseButtonEvent>();
                    }
                    buttonsTriggersEvents.Add(bEvent.Value.copy());

                }
                // store key code of info to send
                if (bEvent.Value.sendInfos) {
                    if(buttonsCodeInfoToSend == null) {
                        buttonsCodeInfoToSend = new List<KeyCode>();
                    }
                    buttonsCodeInfoToSend.Add(bEvent.Key);
                }
            }
        }

        private void process_mouse_axis() {

            // get times
            var currentExpTime = time().ellapsed_exp_ms();
            var currentElemTime = time().ellapsed_element_ms();

            foreach (var aEvent in axisEvent) {
                aEvent.Value.update(UnityEngine.Input.GetAxis(Input.MouseAxis.CodesNames[aEvent.Key]), currentExpTime, currentElemTime);
            }

            previousMousePosition = currentMousePosition;
            currentMousePosition = UnityEngine.Input.mousePosition;

            foreach (var aEvent in axisEvent) {
                // trigger signals
                if (aEvent.Value.triggerSignals && is_updating()) {
                    if(axisTriggersEvents == null) {
                        axisTriggersEvents = new List<Input.MouseAxisEvent>();
                    }
                    axisTriggersEvents.Add(aEvent.Value.copy());
                }
                // store key code of info to send
                if (aEvent.Value.sendInfos) {
                    if(axisCodeInfoToSend == null) {
                        axisCodeInfoToSend = new List<Input.MouseAxis.Code>();
                    }
                    axisCodeInfoToSend.Add(aEvent.Key);
                }
            }         
        }

        #endregion
    }
}