
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

namespace Ex{

    public class DelayConnector : ExConnector{

        int delay = 500;
        int maxNb = 10000;
        Queue<Tuple<double, object>> elements = new Queue<Tuple<double, object>>();

        protected override bool initialize() {
            update_from_gui();

            add_signals(1);
            add_slot(0, (arg) => { base_slot1(arg); });
            return true;
        }

        protected override void slot1(object arg) {
            elements.Enqueue(new Tuple<double, object>(time().ellapsed_element_ms(), arg));
            if (elements.Count > maxNb) {
                elements.Dequeue();
            }
        }

        protected override void update_from_gui() {
            delay = m_config.get<int>(valueStr);
        }

        protected override void update() {

            var currTime = time().ellapsed_element_ms();

            List<object> items = new List<object>();
            while (elements.Count > 0) {

                if(elements.Peek().Item1 + delay < currTime) {
                    items.Add(elements.Dequeue().Item2);
                } else {
                    break;
                }
            }

            foreach(var item in items) {
                invoke_signal(0, item);
            }
        }

        protected override void stop_routine() {
            elements.Clear();
        }
    }
}