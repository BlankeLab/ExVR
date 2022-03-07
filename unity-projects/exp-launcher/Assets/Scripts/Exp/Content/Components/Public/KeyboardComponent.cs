
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
using UnityRawInput;

namespace Ex{

    public class KeyboardComponent : ExComponent{

        private bool sendInfos = false;

        // signals
        private static readonly string buttonOnGuiSignal  = "button";
        // infos
        private static readonly string infosSignal  = "buttons_state_info";

        // events
        // # buttons
        private Dictionary<KeyCode,  Input.KeyboardButtonEvent> buttonsEvent  = new Dictionary<KeyCode, Input.KeyboardButtonEvent>();
        // #raw buttons
        private Dictionary<RawKey, Input.KeyboardButtonEvent> rawButtonsEvent = new Dictionary<RawKey, Input.KeyboardButtonEvent>();

        // states 
        public Dictionary<KeyCode, Input.KeyboardButtonState> buttonsState = new Dictionary<KeyCode, Input.KeyboardButtonState>();


        protected override bool initialize() {

            add_signal(buttonOnGuiSignal);

            foreach (var code in Input.Keyboard.Codes) {
                buttonsEvent[code]  = new Input.KeyboardButtonEvent(code);
                buttonsState[code] = new Input.KeyboardButtonState(code);
            }

            foreach (var button in Input.Keyboard.RawCodesCorrespondence) {                
                rawButtonsEvent[button.Key] = new Input.KeyboardButtonEvent(button.Value);
            }

            return true;
        }


        protected override void update() {
            // reset states infos
            sendInfos = true;
        }

        protected override void set_update_state(bool doUpdate) {

            if (!doUpdate) {

                // reset states
                var currentTime = time().ellapsed_exp_ms();
                foreach (KeyCode keyCode in Input.Keyboard.Codes) {
                    var currentEvent = buttonsEvent[keyCode];
                    var currentState = buttonsState[keyCode];
                    currentEvent.update(false, currentTime);
                    currentState.update(false, currentTime);
                }

                foreach (var codePair in Input.Keyboard.RawCodesCorrespondence) {
                    var currentEvent = rawButtonsEvent[codePair.Key];
                    var currentState = buttonsState[codePair.Value];
                    currentEvent.update(false, currentTime);
                    currentState.update(false, currentTime);
                }
            }
        }

        protected override void on_gui() {

            var eventType = Event.current.type;
            if (eventType != EventType.KeyDown && eventType != EventType.KeyUp && eventType != EventType.Repaint) {
                return;
            }

            var currentTime  = time().ellapsed_exp_ms();

            // retrieve keys states
            int keyInputChanged = 0;
            if (!ExVR.GuiSettings().catchExternalKeyboardEvents) {

                foreach (KeyCode keyCode in Input.Keyboard.Codes) {

                    bool pressed = UnityEngine.Input.GetKey(keyCode);

                    // update event
                    var currentEvent = buttonsEvent[keyCode];
                    var currentState = buttonsState[keyCode];
                    var previousState = currentEvent.state;
                    currentEvent.update(pressed, currentTime);
                    currentState.update(pressed, currentTime);

                    if (previousState != currentEvent.state) {
                        ++keyInputChanged;
                    }

                    if (currentEvent.state != Input.Button.State.None) {
                        invoke_signal(buttonOnGuiSignal, currentEvent);
                    }                 
                }

                // send infos only once per frame
                if (keyInputChanged > 0 && sendInfos) {

                    StringBuilder infos = new StringBuilder();
                    int currentKey = 0;
                    foreach (KeyCode button in Input.Keyboard.Codes) {
                        var buttonState = buttonsEvent[button];
                        if (buttonState.state == Input.Button.State.Pressed || buttonState.state == Input.Button.State.Down) {
                            if (currentKey != keyInputChanged - 1) {
                                infos.AppendFormat("{0},", ((int)button).ToString());
                            } else {
                                infos.Append(((int)button).ToString());
                            }
                            ++currentKey;
                        }
                    }

                    send_infos_to_gui_init_config(infosSignal, infos.ToString());
                    sendInfos = false;
                }

            } else { 
            
                foreach (var codePair in Input.Keyboard.RawCodesCorrespondence) {

                    bool pressed = RawKeyInput.IsKeyDown(codePair.Key);

                    // update event
                    var currentEvent = rawButtonsEvent[codePair.Key];
                    var currentState = buttonsState[codePair.Value];
                    var previousState = currentEvent.state;

                    currentEvent.update(pressed, currentTime);                    
                    currentState.update(pressed, currentTime);

                    if (previousState != currentEvent.state) {
                        ++keyInputChanged;
                    }

                    if (currentEvent.state != Input.Button.State.None) {
                        invoke_signal(buttonOnGuiSignal, currentEvent);
                    }
                }

                // send infos only once per frame
                if (keyInputChanged > 0 && sendInfos) {
                    StringBuilder infos = new StringBuilder();
                    int currentKey = 0;

                    foreach(var codePair in Input.Keyboard.RawCodesCorrespondence) {
                        var buttonState = rawButtonsEvent[codePair.Key];
                        if (buttonState.state == Input.Button.State.Pressed || buttonState.state == Input.Button.State.Down) {
                            if (currentKey != keyInputChanged - 1) {
                                infos.AppendFormat("{0},", ((int)codePair.Value).ToString());
                            } else {
                                infos.Append(((int)codePair.Value).ToString());
                            }
                            ++currentKey;
                        }
                    }

                    send_infos_to_gui_init_config(infosSignal, infos.ToString());
                    sendInfos = false;
                }

            }
        }
    }
}