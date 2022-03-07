
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

        public static readonly Dictionary<RawKey, KeyCode> RawCodesCorrespondence = new Dictionary<RawKey, KeyCode>() {
            [RawKey.Back] = KeyCode.Backspace,
            [RawKey.Tab] = KeyCode.Tab,
            [RawKey.Clear] = KeyCode.Clear,
            [RawKey.Return] = KeyCode.Return,
            [RawKey.Menu] = KeyCode.Menu,
            [RawKey.Pause] = KeyCode.Pause,
            [RawKey.CapsLock] = KeyCode.CapsLock,
            [RawKey.Escape] = KeyCode.Escape,
            [RawKey.Space] = KeyCode.Space,
            [RawKey.End] = KeyCode.End,
            [RawKey.Home] = KeyCode.Home,
            [RawKey.Left] = KeyCode.LeftArrow,
            [RawKey.Up] = KeyCode.UpArrow,
            [RawKey.Right] = KeyCode.RightArrow,
            [RawKey.Down] = KeyCode.DownArrow,
            [RawKey.Print] = KeyCode.Print,
            [RawKey.Insert] = KeyCode.Insert,
            [RawKey.Delete] = KeyCode.Delete,
            [RawKey.Help] = KeyCode.Help,
            [RawKey.A] = KeyCode.A,
            [RawKey.B] = KeyCode.B,
            [RawKey.C] = KeyCode.C,
            [RawKey.D] = KeyCode.D,
            [RawKey.E] = KeyCode.E,
            [RawKey.F] = KeyCode.F,
            [RawKey.G] = KeyCode.G,
            [RawKey.H] = KeyCode.H,
            [RawKey.I] = KeyCode.I,
            [RawKey.J] = KeyCode.J,
            [RawKey.K] = KeyCode.K,
            [RawKey.L] = KeyCode.L,
            [RawKey.M] = KeyCode.M,
            [RawKey.N] = KeyCode.N,
            [RawKey.O] = KeyCode.O,
            [RawKey.P] = KeyCode.P,
            [RawKey.Q] = KeyCode.Q,
            [RawKey.R] = KeyCode.R,
            [RawKey.S] = KeyCode.S,
            [RawKey.T] = KeyCode.T,
            [RawKey.U] = KeyCode.U,
            [RawKey.V] = KeyCode.V,
            [RawKey.W] = KeyCode.W,
            [RawKey.X] = KeyCode.X,
            [RawKey.Y] = KeyCode.Y,
            [RawKey.Z] = KeyCode.Z,
            [RawKey.LeftWindows] = KeyCode.LeftWindows,
            [RawKey.RightWindows] = KeyCode.RightWindows,
            [RawKey.Numpad0] = KeyCode.Keypad0,
            [RawKey.Numpad1] = KeyCode.Keypad1,
            [RawKey.Numpad2] = KeyCode.Keypad2,
            [RawKey.Numpad3] = KeyCode.Keypad3,
            [RawKey.Numpad4] = KeyCode.Keypad4,
            [RawKey.Numpad5] = KeyCode.Keypad5,
            [RawKey.Numpad6] = KeyCode.Keypad6,
            [RawKey.Numpad7] = KeyCode.Keypad7,
            [RawKey.Numpad8] = KeyCode.Keypad8,
            [RawKey.Numpad9] = KeyCode.Keypad9,
            [RawKey.Multiply] = KeyCode.KeypadMultiply,
            [RawKey.Add] = KeyCode.KeypadPlus,
            [RawKey.Subtract] = KeyCode.KeypadMinus,
            [RawKey.Decimal] = KeyCode.Period,
            [RawKey.Divide] = KeyCode.KeypadDivide,
            [RawKey.F1] = KeyCode.F1,
            [RawKey.F2] = KeyCode.F2,
            [RawKey.F3] = KeyCode.F3,
            [RawKey.F4] = KeyCode.F4,
            [RawKey.F5] = KeyCode.F5,
            [RawKey.F6] = KeyCode.F6,
            [RawKey.F7] = KeyCode.F7,
            [RawKey.F8] = KeyCode.F8,
            [RawKey.F9] = KeyCode.F9,
            [RawKey.F10] = KeyCode.F10,
            [RawKey.F11] = KeyCode.F11,
            [RawKey.F12] = KeyCode.F12,
            [RawKey.F13] = KeyCode.F13,
            [RawKey.F14] = KeyCode.F14,
            [RawKey.F15] = KeyCode.F15,
            [RawKey.LeftShift] = KeyCode.LeftShift,
            [RawKey.RightShift] = KeyCode.RightShift,
            [RawKey.LeftControl] = KeyCode.LeftControl,
            [RawKey.RightControl] = KeyCode.RightControl,
            [RawKey.NumLock] = KeyCode.Numlock,
            [RawKey.ScrollLock] = KeyCode.ScrollLock,
            [RawKey.Prior] = KeyCode.UpArrow,
            [RawKey.Next] = KeyCode.PageDown,
            [RawKey.Snapshot] = KeyCode.SysReq,
            [RawKey.N0] = KeyCode.Alpha0,
            [RawKey.N1] = KeyCode.Alpha1,
            [RawKey.N2] = KeyCode.Alpha2,
            [RawKey.N3] = KeyCode.Alpha3,
            [RawKey.N4] = KeyCode.Alpha4,
            [RawKey.N5] = KeyCode.Alpha5,
            [RawKey.N6] = KeyCode.Alpha6,
            [RawKey.N7] = KeyCode.Alpha7,
            [RawKey.N8] = KeyCode.Alpha8,
            [RawKey.N9] = KeyCode.Alpha9,
            [RawKey.OEM1] = KeyCode.Semicolon,
            [RawKey.OEMPlus] = KeyCode.Equals,
            [RawKey.OEMComma] = KeyCode.Comma,
            [RawKey.OEMPeriod] = KeyCode.Period,
            [RawKey.OEM2] = KeyCode.Slash,
            [RawKey.OEM3] = KeyCode.BackQuote,
            [RawKey.OEM4] = KeyCode.LeftBracket,
            [RawKey.OEM5] = KeyCode.Backslash,
            [RawKey.OEM6] = KeyCode.RightBracket,
            [RawKey.OEM7] = KeyCode.Quote,
            [RawKey.OEM8] = KeyCode.BackQuote,
            [RawKey.OEM102] = KeyCode.Backslash,
            [RawKey.LeftMenu] = KeyCode.LeftAlt,
            [RawKey.RightMenu] = KeyCode.RightAlt,
        };
    }

    public class KeyboardButtonState {

        public KeyCode code;
        public double lastTimeDown = 0.0;
        public int nbTimesPressed = 0;

        public KeyboardButtonState(KeyCode code) {
            this.code = code;
        }

        public void update(bool pressed, double currentTime) {
            if (pressed && !is_pressed()) {
                ++nbTimesPressed;
                lastTimeDown = currentTime;
            } else if (!pressed) {
                lastTimeDown = -1.0;
            }
        }

        public bool is_pressed() {
            return lastTimeDown > 0.0;
        }

        public double current_time_pressed() {
            if(is_pressed()) {
                return ExVR.Time().ellapsed_exp_ms() - lastTimeDown;
            }            
            return 0.0;
        }
    }

    public class KeyboardButtonEvent {

        public KeyboardButtonEvent(KeyCode code) {
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

        public KeyCode code;
        public Button.State state;
        public double triggeredExperimentTime;
    }
}