
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
//// system
//using System.Reflection;

//// unity
//using UnityEngine;

//namespace Expy{

//    namespace Events{

//        public class Foo2 : UnityEvent<int>
//        {
//            public Foo2() {
//                AddListener((value) => {
//                    m_slots.invoke_all_slots(value);
//                });
//            }

//            private SlotsEvent m_slots = new SlotsEvent();
//            public SlotsEvent slots() { return m_slots; }
//        }
//    }

//    public class SampleComponent : ExComponent
//    {

//        protected override bool initialize() {

//            // bool addDateToFileName
//            // string directoryPath;
//            // string baseFileName

//            return true;
//        }

//        protected override void start_routine() {
//            // ...
//        }

//        protected override void set_visibility(bool visibility) {
//            // not used
//        }

//        protected override void update() {
//            // ...
//        }

//        protected override void clean() {
//            // ...
//        }


//        public Events.Foo2 m_fooEvent = new Events.Foo2();

//        protected override void connect_signal_to_slot(string signal, MethodInfo slotMethod, object slotObject) {
//            if (signal == nameof(m_fooEvent)) {
//                m_fooEvent.slots().add_slot(slotMethod, slotObject);
//            }
//        }

//        protected override void remove_connections() {
//            m_fooEvent.slots().remove_slots();
//        }

//        protected override void update_parameter_from_gui(XML.Arg arg) {
//            // ...
//        }

//    }
//}