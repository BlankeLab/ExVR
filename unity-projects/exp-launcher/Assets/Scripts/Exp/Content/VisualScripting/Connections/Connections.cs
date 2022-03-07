
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
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex.Events{

    [System.Serializable]
    public class ConnectionInfo {
        public int startIndex;
        public int endIndex;
        public GameObject start;
        public GameObject end;
    }

    public class Connections{

        private Dictionary<string, Slot>   slots    = new Dictionary<string, Slot>();
        private Dictionary<string, Signal> signals  = new Dictionary<string, Signal>();

        private string nameParent;

        public void log_error(string error) {
            ExVR.Log().error(string.Format("[CONNECTION] {0} : {1}", nameParent, error));
        }

        public Connections(string nameParent) {
            this.nameParent = nameParent;
        }

        public int nb_slots() {
            return slots.Count;
        }

        public int nb_signals() {
            return signals.Count;
        }

        public Signal get_signal(string name) {

            if (signals.ContainsKey(name)) {
                return signals[name];
            }
            log_error(String.Format("Signal [{0}] doesn't exist.", name));
            return null;
        }

        public void add_connectors_signals(int nb) {
            for(int ii = 0; ii < nb; ++ii) {
                var signalName = String.Format("output_value_{0}", ii.ToString());
                signals[signalName] = new Signal(String.Format("{0} | {1}", nameParent, signalName));
            }
        }

        public void add_signal(string name){

            if (signals.ContainsKey(name)) {
                log_error(String.Format("Signal [{0}] already exists.", name));
                return;
            } else {
                signals[name] = new Signal(String.Format("{0} | {1}", nameParent, name));//, nbArgs);
            } 
        }

        public void invoke_signal(string name, object arg = null) {
            get_signal(name)?.invoke(arg);
        }

        public Slot get_slot(string name) {
            if (slots.ContainsKey(name)) {
                return slots[name];
            }
            log_error(String.Format("Slot [{0}] doesn't exist.", name));
            return null;
        }

        public void add_slot(string name, System.Action<object> action) {
            if (slots.ContainsKey(name)) {
                log_error(String.Format("Slot [{0}] already exists.", name));
                return;
            }
            slots[name] = new Slot(nameParent + " | " + name, action);
        }

        public static bool connect(Signal signal, Slot slot) {

            if(slot == null) {
                ExVR.Log().error("Cannot connect, slot is null.");
                return false;
            }
            if (signal == null) {
                ExVR.Log().error("Cannot connect, signal is null.");
                return false;
            }

            slot.connect(signal);
            return true;
        }

        public static bool connect(Signal signal, Slot slot, Func<object, object> convertor) {

            if (slot == null) {
                ExVR.Log().error("Cannot connect, slot is null.");
                return false;
            }
            if (signal == null) {
                ExVR.Log().error("Cannot connect, signal is null.");
                return false;
            }

            slot.connect(signal, convertor);
            return true;
        }


        public void clean() {
            foreach(var signalP in signals) {
                signalP.Value.remove_listeners();
            }
        }
    }
}

