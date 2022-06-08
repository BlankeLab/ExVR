using System;
using System.Collections.Generic;

namespace UnityRawInput{

    public static class RawKeyInput{
        /// <summary>
        /// Event invoked when user presses a key.
        /// </summary>
        public static event Action<RawKey, double> OnKeyDown;
        /// <summary>
        /// Event invoked when user releases a key.
        /// </summary>
        public static event Action<RawKey, double> OnKeyUp;

        /// <summary>
        /// Whether the service is running and input messages are being processed.
        /// </summary>
        public static bool IsRunning => hookPtr != IntPtr.Zero;

        /// <summary>
        /// Whether input messages should be handled when the application is not in focus.
        /// </summary>
        public static bool WorkInBackground { get; private set; }
        /// <summary>
        /// Whether handled input messages should not be propagated further.
        /// </summary>
        public static bool InterceptMessages { get; set; }

        private static IntPtr hookPtr = IntPtr.Zero;
        private static bool m_saveEvents = false;

        public static List<Tuple<RawKey, bool, double, double>> keyEvents = null;

        public static void save_events_state(bool state) {
            m_saveEvents = state;
        }
        

        /// <summary>
        /// Initializes the service and starts processing input messages.
        /// </summary>
        /// <param name="workInBackround">Whether input messages should be handled when the application is not in focus.</param>
        /// <returns>Whether the service started successfully.</returns>
        public static bool Start (bool workInBackround){

            if (IsRunning) {
                return false;
            }
            keyEvents = new List<Tuple<RawKey, bool, double, double>>();

            WorkInBackground = workInBackround;
            return set_hook();
        }

        /// <summary>
        /// Terminates the service and stops processing input messages.
        /// </summary>
        public static void Stop (){

            remove_hook();
            keyEvents = null;
        }


        private static bool set_hook (){

            if (hookPtr == IntPtr.Zero){
                if (WorkInBackground) {
                    hookPtr = Win32API.SetWindowsHookEx(HookType.WH_KEYBOARD_LL, HandleLowLevelHookProc, IntPtr.Zero, 0);
                } else {
                    hookPtr = Win32API.SetWindowsHookEx(HookType.WH_KEYBOARD, HandleHookProc, IntPtr.Zero, (int)Win32API.GetCurrentThreadId());
                }
            }
            return hookPtr != IntPtr.Zero;
        }

        private static void remove_hook (){

            if (hookPtr != IntPtr.Zero){
                Win32API.UnhookWindowsHookEx(hookPtr);
                hookPtr = IntPtr.Zero;
            }
        }

        [AOT.MonoPInvokeCallback(typeof(Win32API.HookProc))]
        private static int HandleHookProc (int code, IntPtr wParam, IntPtr lParam){

            if (code < 0) {
                return Win32API.CallNextHookEx(hookPtr, code, wParam, lParam);
            }

            var isKeyDown   = ((int)lParam & (1 << 31)) == 0;
            var key         = (RawKey)wParam;

            var expTime     = Ex.ExVR.Time().ellapsed_exp_ms();
            var elementTime = Ex.ExVR.Time().ellapsed_element_ms();

            if (isKeyDown) {
                HandleKeyDown(key, expTime, elementTime);
            } else {
                HandleKeyUp(key, expTime, elementTime);
            }

            return InterceptMessages ? 1 : Win32API.CallNextHookEx(hookPtr, 0, wParam, lParam);
        }

        [AOT.MonoPInvokeCallback(typeof(Win32API.HookProc))]
        private static int HandleLowLevelHookProc (int code, IntPtr wParam, IntPtr lParam){

            if (code < 0) {
                return Win32API.CallNextHookEx(hookPtr, code, wParam, lParam);
            }

            var kbd      = KBDLLHOOKSTRUCT.CreateFromPtr(lParam);
            var keyState = (RawKeyState)wParam;
            var key      = (RawKey)kbd.vkCode;

            var expTime     = Ex.ExVR.Time().ellapsed_exp_ms();
            var elementTime = Ex.ExVR.Time().ellapsed_element_ms();

            if (keyState == RawKeyState.KeyDown || keyState == RawKeyState.SysKeyDown) {
                HandleKeyDown(key, expTime, elementTime);
            } else {
                HandleKeyUp(key, expTime, elementTime);
            }

            return InterceptMessages ? 1 : Win32API.CallNextHookEx(hookPtr, 0, wParam, lParam);
        }

        private static void HandleKeyDown (RawKey key, double expTime, double elementTime){

            if (m_saveEvents) {
                keyEvents.Add(new Tuple<RawKey, bool, double, double>(key, true, expTime, elementTime));
            }

            if (OnKeyDown != null) {
                OnKeyDown.Invoke(key, expTime);
            } 
        }

        private static void HandleKeyUp (RawKey key, double expTime, double elementTime) {

            if (m_saveEvents) {
                keyEvents.Add(new Tuple<RawKey, bool, double, double>(key, false, expTime, elementTime));
            }

            if (OnKeyUp != null) {
                OnKeyUp.Invoke(key, expTime);
            }
        }
    }
}
