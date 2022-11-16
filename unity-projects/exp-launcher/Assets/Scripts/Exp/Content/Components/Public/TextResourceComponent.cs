
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

    public class TextResourceComponent : ExComponent{

        string m_text = "";
        string[] m_lines = null;
        int m_currentLine = 0;

        protected override bool initialize() {

            add_signal("full text");
            add_signal("alias");
            add_signal("path");
            add_signal("line");

            add_slot("read line", (nullArg) => {
                if(m_currentLine < m_lines.Length) {
                    invoke_signal("line", m_lines[m_currentLine++]);
                }                
            });

            m_currentLine = 0;

            return true;
        }


        public override void update_from_current_config() {
            load_text();
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        private void load_text() {            
            string textAlias = currentC.get_resource_alias("text");
            var textData = ExVR.Resources().get_text_file_data(textAlias);
            m_text = string.Copy(textData.content);
            m_lines =  m_text.Split('\n');
            m_currentLine = 0;
            
            invoke_signal("alias", textData.alias);
            invoke_signal("path", textData.path);
            invoke_signal("full text", m_text);
        }
    }
}