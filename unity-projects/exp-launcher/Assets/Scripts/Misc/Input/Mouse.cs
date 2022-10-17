
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

namespace Ex.Input {

    static public class MouseButton {

        public static readonly List<KeyCode> Codes = new List<KeyCode>(new KeyCode[] {
            KeyCode.Mouse0,
            KeyCode.Mouse1,
            KeyCode.Mouse2,
            KeyCode.Mouse3,
            KeyCode.Mouse4,
            KeyCode.Mouse5,
            KeyCode.Mouse6
        });

        public static readonly Dictionary<KeyCode, string> CodesNames = new Dictionary<KeyCode, string> {
            [KeyCode.Mouse0] = KeyCode.Mouse0.ToString(),
            [KeyCode.Mouse1] = KeyCode.Mouse1.ToString(),
            [KeyCode.Mouse2] = KeyCode.Mouse2.ToString(),
            [KeyCode.Mouse3] = KeyCode.Mouse3.ToString(),
            [KeyCode.Mouse4] = KeyCode.Mouse4.ToString(),
            [KeyCode.Mouse5] = KeyCode.Mouse5.ToString(),
            [KeyCode.Mouse6] = KeyCode.Mouse6.ToString(),
        };
    }

    public class MouseButtonEvent {
        public MouseButtonEvent(KeyCode code, Button.State state = Button.State.None, double triggeredExperimentTime = 0.0, double triggeredElementTime = 0.0, double lastTimeDown = -1.0) {
            this.code = code;
            this.state = state;
            this.triggeredExperimentTime = triggeredExperimentTime;
            this.triggeredElementTime = triggeredElementTime;
            this.lastTimeDown = lastTimeDown;
        }

        public void update(bool pressed, double currentExpTime, double currentElementTime) {

            previousState = state;

            if (state == Button.State.None) {
                if (pressed) {
                    state = Button.State.Down;
                    lastTimeDown = currentExpTime;
                }
            } else if (state == Button.State.Down) {
                if (pressed) {
                    state = Button.State.Pressed;
                } else {
                    state = Button.State.Up;
                    lastTimeDown = -1.0;
                }

            } else if (state == Button.State.Pressed) {
                if (!pressed) {
                    state = Button.State.Up;
                }
            } else if (state == Button.State.Up) {
                if (pressed) {
                    state = Button.State.Down;
                } else {
                    state = Button.State.None;
                }
            }

            if (state != Button.State.None) {
                triggeredExperimentTime = currentExpTime;
                triggeredElementTime = currentElementTime;
                sendInfos = previousState != state;
                triggerSignals = true;
            } else {
                sendInfos = false;
                triggerSignals = false;
            }
        }

        public MouseButtonEvent copy() {
            return new MouseButtonEvent(code, state, triggeredExperimentTime, triggeredElementTime, lastTimeDown);
        }

        public bool is_pressed() {
            return state == Button.State.Pressed || state == Button.State.Down;
        }

        public double current_time_pressed_ms() {
            if (is_pressed()) {
                return ExVR.Time().ellapsed_exp_ms() - lastTimeDown;
            }
            return 0.0;
        }

        public KeyCode code;
        public Button.State state;
        public Button.State previousState;
        public double triggeredExperimentTime;
        public double triggeredElementTime;
        public double lastTimeDown = 0.0;

        public bool triggerSignals = false;
        public bool sendInfos = false;
    }

    static public class MouseAxis {
        public enum Code {
            Horizontal, Vertical
        }

        public static readonly List<Code> Codes = new List<Code>(new Code[] {
            Code.Horizontal, Code.Vertical
        });

        public static readonly Dictionary<Code, string> CodesNames = new Dictionary<Code, string> {
            [Code.Horizontal] = "Mouse X",
            [Code.Vertical]   = "Mouse Y"
        };
    }

    public class MouseAxisEvent {

        public MouseAxisEvent(MouseAxis.Code code, float value = 0f, double triggeredExperimentTime = 0.0, double triggeredElementTime = 0.0, double lastTimeDown = -1.0) {
            this.code = code;
            this.value = value;
            this.triggeredExperimentTime = triggeredExperimentTime;
            this.triggeredElementTime = triggeredElementTime;
            this.lastTimeDown = lastTimeDown;
        }

        public void update(float newValue, double currentExpTime, double currentElementTime) {

            previousValue = value;
            if (newValue != 0f) {

                if (value == 0f) {
                    lastTimeDown = currentExpTime;
                }

                triggeredExperimentTime = currentExpTime;
                triggeredElementTime = currentElementTime;
                
                triggerSignals = true;
            } else {
                lastTimeDown = -1.0;
                triggerSignals = false;
            }

            sendInfos = previousValue != newValue;
            value = newValue;
        }
        public MouseAxisEvent copy() {
            return new MouseAxisEvent(code, value, triggeredExperimentTime, triggeredElementTime, lastTimeDown);
        }

        public bool is_moved() {
            return lastTimeDown > 0.0;
        }

        public double current_time_moving() {
            if (is_moved()) {
                return ExVR.Time().ellapsed_exp_ms() - lastTimeDown;
            }
            return 0.0;
        }

        public MouseAxis.Code code;
        public float value = 0f;
        public float previousValue = 0f;
        public double triggeredExperimentTime;
        public double triggeredElementTime;
        public double lastTimeDown = 0.0;

        public bool triggerSignals = false;
        public bool sendInfos = false;
    }
}