
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

namespace Ex{

    public class Duration{

        private int m_callsNb = 0;

        public int calls_nb() {
            return m_callsNb;
        }
    }

    public class ISI : FlowElement{
        
        public List<double> m_durations = null;

        private bool m_randomized = false;
        public void initialize(XML.Isi isi) {

            m_key        = isi.Key;
            m_keyStr     = Converter.to_string(isi.Key);
            m_type       = FlowElementType.Isi;
            m_randomized = isi.Randomized;

            string[] splits = isi.Set.Split(' ');
            m_durations = new List<double>(splits.Length);
            foreach (string split in splits) {
                m_durations.Add(Converter.to_double(split));
            }
        }

        public void start(FlowElementInfo elementInfo) {
            gameObject.SetActive(true);
        }

        public void stop() {
            gameObject.SetActive(false);
        }
    }
}