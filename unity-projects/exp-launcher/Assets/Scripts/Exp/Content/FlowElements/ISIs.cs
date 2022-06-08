
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

namespace Ex{

    public class ISIs : MonoBehaviour{

        private List<ISI> m_ISIs = new List<ISI>();
        private ISI m_currentIsi = null;

        public void generate_from_xml(XML.ISIs xmlISIs) {

            foreach (XML.Isi xmlIsi in xmlISIs.Isi) {
                var isi = GO.generate_empty_object(xmlIsi.Name, ExVR.GO().ISIs.transform).AddComponent<ISI>();
                isi.initialize(xmlIsi);
                m_ISIs.Add(isi);
            }
        }

        public ISI current() {
            return m_currentIsi;
        }

        public List<ISI> get_all() {
            return new List<ISI>(m_ISIs);
        }

        public ISI get(int isiKey) {

            foreach(var isi in m_ISIs) {
                if(isi.key() == isiKey) {
                    return isi;
                }
            }
            return null;
        }

        public void start_isi(ISIInfo info) {

            ExVR.ExpLog().enable_isi(info);

            // set current isi
            m_currentIsi = (ISI)info.element();            
            m_currentIsi.start(info);
        }

        public void stop_current_isi() {

            if(m_currentIsi != null) {
                m_currentIsi.stop();
                m_currentIsi = null;
            }
        }

        public void clean() {

            foreach (var isi in m_ISIs) {
                Destroy(isi.gameObject);
            }
            m_ISIs.Clear();
        }


    }
}


