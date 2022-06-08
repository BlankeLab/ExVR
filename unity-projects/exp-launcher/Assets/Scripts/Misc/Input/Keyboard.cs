
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
using UnityRawInput;

namespace Ex.Input {

    static public class Keyboard {

        public static readonly List<KeyCode> Codes = new List<KeyCode>(new KeyCode[] {
            KeyCode.Backspace, KeyCode.Delete, KeyCode.Tab, KeyCode.Clear,
            KeyCode.Return, KeyCode.Pause, KeyCode.Escape, KeyCode.Space,
            KeyCode.Keypad0, KeyCode.Keypad1, KeyCode.Keypad2, KeyCode.Keypad3,
            KeyCode.Keypad4, KeyCode.Keypad5, KeyCode.Keypad6, KeyCode.Keypad7,
            KeyCode.Keypad8, KeyCode.Keypad9, KeyCode.KeypadPeriod, KeyCode.KeypadDivide,
            KeyCode.KeypadMultiply, KeyCode.KeypadMinus, KeyCode.KeypadPlus, KeyCode.KeypadEnter,
            KeyCode.KeypadEquals, KeyCode.UpArrow, KeyCode.DownArrow, KeyCode.RightArrow,
            KeyCode.LeftArrow, KeyCode.Insert, KeyCode.Home, KeyCode.End,
            KeyCode.PageUp, KeyCode.PageDown,
            KeyCode.F1, KeyCode.F2, KeyCode.F3, KeyCode.F4, KeyCode.F5, KeyCode.F6, KeyCode.F7, KeyCode.F8, KeyCode.F9, KeyCode.F10, KeyCode.F11, KeyCode.F12, KeyCode.F13, KeyCode.F14, KeyCode.F15,
            KeyCode.Alpha0, KeyCode.Alpha1, KeyCode.Alpha2, KeyCode.Alpha3, KeyCode.Alpha4, KeyCode.Alpha5, KeyCode.Alpha6, KeyCode.Alpha7, KeyCode.Alpha8, KeyCode.Alpha9,
            KeyCode.Exclaim, KeyCode.DoubleQuote, KeyCode.Hash, KeyCode.Dollar, KeyCode.Percent, KeyCode.Ampersand, KeyCode.Quote, KeyCode.LeftParen, KeyCode.RightParen, KeyCode.Asterisk,
            KeyCode.Plus, KeyCode.Comma, KeyCode.Minus, KeyCode.Period, KeyCode.Slash, KeyCode.Colon, KeyCode.Semicolon, KeyCode.Less, KeyCode.Equals, KeyCode.Greater,
            KeyCode.Question, KeyCode.At, KeyCode.LeftBracket, KeyCode.Backslash, KeyCode.RightBracket, KeyCode.Underscore, KeyCode.BackQuote,
            KeyCode.A, KeyCode.B, KeyCode.C, KeyCode.D, KeyCode.E, KeyCode.F, KeyCode.G, KeyCode.H, KeyCode.I, KeyCode.J, KeyCode.K, KeyCode.L, KeyCode.M, KeyCode.N, KeyCode.O, KeyCode.P, KeyCode.Q, KeyCode.R,
            KeyCode.S, KeyCode.T, KeyCode.U, KeyCode.V, KeyCode.W, KeyCode.X, KeyCode.Y, KeyCode.Z,
            KeyCode.LeftCurlyBracket, KeyCode.Pipe, KeyCode.RightCurlyBracket, KeyCode.Tilde, KeyCode.Numlock, KeyCode.CapsLock, KeyCode.ScrollLock,
            KeyCode.RightShift, KeyCode.LeftShift, KeyCode.RightControl, KeyCode.LeftControl,
            KeyCode.RightAlt, KeyCode.LeftAlt, KeyCode.LeftCommand, KeyCode.LeftApple, KeyCode.LeftWindows, KeyCode.RightCommand, KeyCode.RightApple, KeyCode.RightWindows, KeyCode.AltGr, KeyCode.Help, KeyCode.Print,
            KeyCode.SysReq, KeyCode.Break, KeyCode.Menu
        });

        public static readonly Dictionary<VirtualButton, KeyCode> vrCodeCorrespondence = new Dictionary<VirtualButton, KeyCode>() { 
            [VirtualButton.VK_BACK]         = KeyCode.Backspace,
            [VirtualButton.VK_TAB]          = KeyCode.Tab,
            [VirtualButton.VK_CLEAR]        = KeyCode.Clear,
            [VirtualButton.VK_RETURN]       = KeyCode.Return,
            [VirtualButton.VK_MENU]         = KeyCode.Menu,
            [VirtualButton.VK_PAUSE]        = KeyCode.Pause,
            [VirtualButton.VK_CAPITAL]      = KeyCode.CapsLock,
            [VirtualButton.VK_ESCAPE]       = KeyCode.Escape,
            [VirtualButton.VK_SPACE]        = KeyCode.Space,
            [VirtualButton.VK_END]          = KeyCode.End,
            [VirtualButton.VK_HOME]         = KeyCode.Home,
            [VirtualButton.VK_LEFT]         = KeyCode.LeftArrow,
            [VirtualButton.VK_UP]           = KeyCode.UpArrow,
            [VirtualButton.VK_RIGHT]        = KeyCode.RightArrow,
            [VirtualButton.VK_DOWN]         = KeyCode.DownArrow,
            [VirtualButton.VK_PRINT]        = KeyCode.Print,
            [VirtualButton.VK_INSERT]       = KeyCode.Insert,
            [VirtualButton.VK_DELETE]       = KeyCode.Delete,
            [VirtualButton.VK_HELP]         = KeyCode.Help,
            [VirtualButton.VK_A]            = KeyCode.A,
            [VirtualButton.VK_B]            = KeyCode.B,
            [VirtualButton.VK_C]            = KeyCode.C,
            [VirtualButton.VK_D]            = KeyCode.D,
            [VirtualButton.VK_E]            = KeyCode.E,
            [VirtualButton.VK_F]            = KeyCode.F,
            [VirtualButton.VK_G]            = KeyCode.G,
            [VirtualButton.VK_H]            = KeyCode.H,
            [VirtualButton.VK_I]            = KeyCode.I,
            [VirtualButton.VK_J]            = KeyCode.J,
            [VirtualButton.VK_K]            = KeyCode.K,
            [VirtualButton.VK_L]            = KeyCode.L,
            [VirtualButton.VK_M]            = KeyCode.M,
            [VirtualButton.VK_N]            = KeyCode.N,
            [VirtualButton.VK_O]            = KeyCode.O,
            [VirtualButton.VK_P]            = KeyCode.P,
            [VirtualButton.VK_Q]            = KeyCode.Q,
            [VirtualButton.VK_R]            = KeyCode.R,
            [VirtualButton.VK_S]            = KeyCode.S,
            [VirtualButton.VK_T]            = KeyCode.T,
            [VirtualButton.VK_U]            = KeyCode.U,
            [VirtualButton.VK_V]            = KeyCode.V,
            [VirtualButton.VK_W]            = KeyCode.W,
            [VirtualButton.VK_X]            = KeyCode.X,
            [VirtualButton.VK_Y]            = KeyCode.Y,
            [VirtualButton.VK_Z]            = KeyCode.Z,
            [VirtualButton.VK_LWIN]         = KeyCode.LeftWindows,
            [VirtualButton.VK_RWIN]         = KeyCode.RightWindows,
            [VirtualButton.VK_NUMPAD0]      = KeyCode.Keypad0,
            [VirtualButton.VK_NUMPAD1]      = KeyCode.Keypad1,
            [VirtualButton.VK_NUMPAD2]      = KeyCode.Keypad2,
            [VirtualButton.VK_NUMPAD3]      = KeyCode.Keypad3,
            [VirtualButton.VK_NUMPAD4]      = KeyCode.Keypad4,
            [VirtualButton.VK_NUMPAD5]      = KeyCode.Keypad5,
            [VirtualButton.VK_NUMPAD6]      = KeyCode.Keypad6,
            [VirtualButton.VK_NUMPAD7]      = KeyCode.Keypad7,
            [VirtualButton.VK_NUMPAD8]      = KeyCode.Keypad8,
            [VirtualButton.VK_NUMPAD9]      = KeyCode.Keypad9,
            [VirtualButton.VK_MULTIPLY]     = KeyCode.KeypadMultiply,
            [VirtualButton.VK_ADD]          = KeyCode.KeypadPlus,
            [VirtualButton.VK_SUBTRACT]     = KeyCode.KeypadMinus,
            [VirtualButton.VK_DECIMAL]      = KeyCode.Period,
            [VirtualButton.VK_DIVIDE]       = KeyCode.KeypadDivide,
            [VirtualButton.VK_F1]           = KeyCode.F1,
            [VirtualButton.VK_F2]           = KeyCode.F2,
            [VirtualButton.VK_F3]           = KeyCode.F3,
            [VirtualButton.VK_F4]           = KeyCode.F4,
            [VirtualButton.VK_F5]           = KeyCode.F5,
            [VirtualButton.VK_F6]           = KeyCode.F6,
            [VirtualButton.VK_F7]           = KeyCode.F7,
            [VirtualButton.VK_F8]           = KeyCode.F8,
            [VirtualButton.VK_F9]           = KeyCode.F9,
            [VirtualButton.VK_F10]          = KeyCode.F10,
            [VirtualButton.VK_F11]          = KeyCode.F11,
            [VirtualButton.VK_F12]          = KeyCode.F12,
            [VirtualButton.VK_F13]          = KeyCode.F13,
            [VirtualButton.VK_F14]          = KeyCode.F14,
            [VirtualButton.VK_F15]          = KeyCode.F15,
            [VirtualButton.VK_LSHIFT]       = KeyCode.LeftShift,
            [VirtualButton.VK_RSHIFT]       = KeyCode.RightShift,
            [VirtualButton.VK_LCONTROL]     = KeyCode.LeftControl,
            [VirtualButton.VK_RCONTROL]     = KeyCode.RightControl,
            [VirtualButton.VK_NUMLOCK]      = KeyCode.Numlock,
            [VirtualButton.VK_SCROLL]       = KeyCode.ScrollLock,
            [VirtualButton.VK_PRIOR]        = KeyCode.UpArrow,
            [VirtualButton.VK_NEXT]         = KeyCode.PageDown,
            [VirtualButton.VK_SNAPSHOT]     = KeyCode.SysReq,
            [VirtualButton.VK_0]            = KeyCode.Alpha0,
            [VirtualButton.VK_1]            = KeyCode.Alpha1,
            [VirtualButton.VK_2]            = KeyCode.Alpha2,
            [VirtualButton.VK_3]            = KeyCode.Alpha3,
            [VirtualButton.VK_4]            = KeyCode.Alpha4,
            [VirtualButton.VK_5]            = KeyCode.Alpha5,
            [VirtualButton.VK_6]            = KeyCode.Alpha6,
            [VirtualButton.VK_7]            = KeyCode.Alpha7,
            [VirtualButton.VK_8]            = KeyCode.Alpha8,
            [VirtualButton.VK_9]            = KeyCode.Alpha9,
            [VirtualButton.VK_OEM_1]        = KeyCode.Semicolon,
            //[VirtualButton.VK_OEM_        = KeyCode.Equals,
            [VirtualButton.VK_OEM_COMMA]    = KeyCode.Comma,
            [VirtualButton.VK_OEM_PERIOD]   = KeyCode.Period,
            [VirtualButton.VK_OEM_2]        = KeyCode.Slash,
            [VirtualButton.VK_OEM_7]        = KeyCode.BackQuote,
            [VirtualButton.VK_OEM_4]        = KeyCode.LeftBracket,
            [VirtualButton.VK_OEM_5]        = KeyCode.Backslash,
            [VirtualButton.VK_OEM_6]        = KeyCode.RightBracket,      
            [VirtualButton.VK_LMENU]        = KeyCode.LeftAlt,
            [VirtualButton.VK_RMENU]        = KeyCode.RightAlt
        };    
    }

    public class KeyboardButtonEvent {

        public KeyboardButtonEvent(KeyCode code, Button.State state = Button.State.None, double triggeredExperimentTime = 0.0, double triggeredElementTime = 0.0, double lastTimeDown = -1.0) {
            this.code                       = code;
            this.state                      = state;
            this.triggeredExperimentTime    = triggeredExperimentTime;
            this.triggeredElementTime       = triggeredElementTime;
            this.lastTimeDown               = lastTimeDown;
        }

        public void update(bool pressed, double currentExpTime, double currentElementTime) {

            if (state == Button.State.None) {
                if (pressed) {
                    state        = Button.State.Down;
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
                triggeredElementTime    = currentElementTime;
            }
        }

        public KeyboardButtonEvent copy() {
            return new KeyboardButtonEvent(code, state, triggeredExperimentTime, triggeredElementTime, lastTimeDown);
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
        public double triggeredExperimentTime;
        public double triggeredElementTime;
        public double lastTimeDown = 0.0;
    }
}