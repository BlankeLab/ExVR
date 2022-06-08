
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

namespace Ex.Input {

    static public class JoypadButton {
        public enum Code {
            // P1
            A_P1 = 0, B_P1, X_P1, Y_P1,
            RightBumper_P1, LeftBumper_P1, Start_P1, Back_P1,
            LeftStickClick_P1, RightStickClick_P1,
            // P2
            A_P2, B_P2, X_P2, Y_P2,
            RightBumper_P2, LeftBumper_P2, Start_P2, Back_P2,
            LeftStickClick_P2, RightStickClick_P2
        }

        public static readonly List<Code> Codes = new List<Code>(new Code[] {
            // P1
            Code.A_P1, Code.B_P1, Code.X_P1, Code.Y_P1,
            Code.RightBumper_P1, Code.LeftBumper_P1, Code.Start_P1, Code.Back_P1,
            Code.LeftStickClick_P1, Code.RightStickClick_P1,
            // P2
            Code.A_P2, Code.B_P2, Code.X_P2, Code.Y_P2,
            Code.RightBumper_P2, Code.LeftBumper_P2, Code.Start_P2, Code.Back_P2,
            Code.LeftStickClick_P2, Code.RightStickClick_P2,
        });

        public static readonly Dictionary<Code, string> CodesNames = new Dictionary<Code, string> {
            // P1
            [Code.A_P1] = Code.A_P1.ToString(),
            [Code.B_P1] = Code.B_P1.ToString(),
            [Code.X_P1] = Code.X_P1.ToString(),
            [Code.Y_P1] = Code.Y_P1.ToString(),
            [Code.RightBumper_P1] = Code.RightBumper_P1.ToString(),
            [Code.LeftBumper_P1] = Code.LeftBumper_P1.ToString(),
            [Code.Start_P1] = Code.Start_P1.ToString(),
            [Code.Back_P1] = Code.Back_P1.ToString(),
            [Code.LeftStickClick_P1] = Code.LeftStickClick_P1.ToString(),
            [Code.RightStickClick_P1] = Code.RightStickClick_P1.ToString(),
            // P2
            [Code.A_P2] = Code.A_P2.ToString(),
            [Code.B_P2] = Code.B_P2.ToString(),
            [Code.X_P2] = Code.X_P2.ToString(),
            [Code.Y_P2] = Code.Y_P2.ToString(),
            [Code.RightBumper_P2] = Code.RightBumper_P2.ToString(),
            [Code.LeftBumper_P2] = Code.LeftBumper_P2.ToString(),
            [Code.Start_P2] = Code.Start_P2.ToString(),
            [Code.Back_P2] = Code.Back_P2.ToString(),
            [Code.LeftStickClick_P2] = Code.LeftStickClick_P2.ToString(),
            [Code.RightStickClick_P2] = Code.RightStickClick_P2.ToString(),
        };
    }


    public class JoypadButtonState {

        public JoypadButton.Code code;
        public double lastTimeDown = 0.0;
        public int nbTimesPressed = 0;

        public JoypadButtonState(JoypadButton.Code code) {
            this.code = code;
        }
        public void update(bool pressed, double currentTime) {
            if (pressed && !is_pressed()) {
                ++nbTimesPressed;
                lastTimeDown = currentTime;
            } else if(!pressed){
                lastTimeDown = -1.0;
            }
        }

        public bool is_pressed() {
            return lastTimeDown > 0.0;
        }

        public double current_time_pressed() {
            if (is_pressed()) {
                return ExVR.Time().ellapsed_exp_ms() - lastTimeDown;
            }
            return 0.0;
        }
    }

    public class JoypadButtonEvent {

        public JoypadButtonEvent(JoypadButton.Code code) {
            this.code = code;
            state = Button.State.None;
            triggeredExperimentTime = 0.0;
        }

        public void update(bool pressed, double currentTime) {

            if (state == Input.Button.State.None) {
                if (pressed) {
                    state = Input.Button.State.Down;
                }
            } else if (state == Input.Button.State.Down) {
                if (pressed) {
                    state = Input.Button.State.Pressed;
                } else {
                    state = Input.Button.State.Up;
                }

            } else if (state == Input.Button.State.Pressed) {
                if (!pressed) {
                    state = Input.Button.State.Up;
                }
            } else if (state == Input.Button.State.Up) {
                if (pressed) {
                    state = Input.Button.State.Down;
                } else {
                    state = Input.Button.State.None;
                }
            }

            if (state != Input.Button.State.None) {
                triggeredExperimentTime = currentTime;
            }
        }

        public JoypadButton.Code code;
        public Button.State state;
        public double triggeredExperimentTime;
    }

    static public class JoypadAxis {

        public enum Code {
            // P1
            LeftJoystickX_P1 = 100, RightJoystickX_P1,
            LeftJoystickY_P1, RightJoystickY_P1,
            LeftTrigger_P1, RightTrigger_P1,
            DPadX_P1, DPadY_P1,
            // P2
            LeftJoystickX_P2, RightJoystickX_P2,
            LeftJoystickY_P2, RightJoystickY_P2,
            LeftTrigger_P2, RightTrigger_P2,
            DPadX_P2, DPadY_P2,
        }

        public static readonly List<Code> Codes = new List<Code>(new Code[] {
            // P1
            Code.LeftJoystickX_P1, Code.RightJoystickX_P1, Code.LeftJoystickY_P1, Code.RightJoystickY_P1,
            Code.LeftTrigger_P1, Code.RightTrigger_P1, Code.DPadX_P1, Code.DPadY_P1,
            // P2
            Code.LeftJoystickX_P2, Code.RightJoystickX_P2, Code.LeftJoystickY_P2, Code.RightJoystickY_P2,
            Code.LeftTrigger_P2, Code.RightTrigger_P2, Code.DPadX_P2, Code.DPadY_P2,
        });

        public static readonly Dictionary<Code, string> CodesNames = new Dictionary<Code, string> {
            // P1
            [Code.LeftJoystickX_P1] = Code.LeftJoystickX_P1.ToString(),
            [Code.RightJoystickX_P1] = Code.RightJoystickX_P1.ToString(),
            [Code.LeftJoystickY_P1] = Code.LeftJoystickY_P1.ToString(),
            [Code.RightJoystickY_P1] = Code.RightJoystickY_P1.ToString(),
            [Code.LeftTrigger_P1] = Code.LeftTrigger_P1.ToString(),
            [Code.RightTrigger_P1] = Code.RightTrigger_P1.ToString(),
            [Code.DPadX_P1] = Code.DPadX_P1.ToString(),
            [Code.DPadY_P1] = Code.DPadY_P1.ToString(),
            // P2
            [Code.LeftJoystickX_P2] = Code.LeftJoystickX_P2.ToString(),
            [Code.RightJoystickX_P2] = Code.RightJoystickX_P2.ToString(),
            [Code.LeftJoystickY_P2] = Code.LeftJoystickY_P2.ToString(),
            [Code.RightJoystickY_P2] = Code.RightJoystickY_P2.ToString(),
            [Code.LeftTrigger_P2] = Code.LeftTrigger_P2.ToString(),
            [Code.RightTrigger_P2] = Code.RightTrigger_P2.ToString(),
            [Code.DPadX_P2] = Code.DPadX_P2.ToString(),
            [Code.DPadY_P2] = Code.DPadY_P2.ToString(),
        };
    }

    public class JoypadAxisState {

        public JoypadAxis.Code code;
        public double lastTimeDown = 0.0;
        public float value = 0f;

        public JoypadAxisState(JoypadAxis.Code code) {
            this.code = code;
        }
        public void update(float value, double currentTime) {
            this.value = value;
            if (value != 0f) {
                lastTimeDown = currentTime;
            } else {
                lastTimeDown = -1.0;
            }
        }

        public bool is_pressed() {
            return lastTimeDown > 0.0;
        }

        public double current_time_pressed() {
            if (is_pressed()) {
                return ExVR.Time().ellapsed_exp_ms() - lastTimeDown;
            }
            return 0.0;
        }
    }

    public class JoypadAxisEvent {

        public JoypadAxisEvent(JoypadAxis.Code code) {
            this.code = code;
            triggeredExperimentTime = 0.0;
        }

        public void update(float value, double currentTime) {
            this.value = value;
            if (value != 0f) {
                triggeredExperimentTime = currentTime;
            }
        }

        public JoypadAxis.Code code;
        public float value = 0f;
        public double triggeredExperimentTime;
    }
}