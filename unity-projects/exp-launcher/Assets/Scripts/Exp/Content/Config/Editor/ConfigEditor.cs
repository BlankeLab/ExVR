
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

// unity
using UnityEngine;


using UnityEditor;
namespace Ex{

    [CustomEditor(typeof(Config),true)]
    public class ConfigEditor : Editor{

        public override bool RequiresConstantRepaint() {
            return true;
        }

        public override void OnInspectorGUI() {

            if (!Application.isPlaying) {
                return;
            }

            Config config = (Config)target;

            var win = Screen.width;
            var w1 = win * 0.25f;
            var w2 = win * 0.25f;
            var w3 = win * 0.25f;
            var w4 = win * 0.25f;

            EditorGUILayout.LabelField("All args: ");
            GUILayout.BeginHorizontal();
            GUILayout.Label("Name", GUILayout.Width(w1));
            GUILayout.Label("Type", GUILayout.Width(w2));
            GUILayout.Label("Value", GUILayout.Width(w3));
            GUILayout.Label("Array", GUILayout.Width(w4));
            GUILayout.EndHorizontal();

            foreach (var pair in config.args) {

                GUILayout.BeginHorizontal();
                if (pair.Value.xml == null) {
                    GUILayout.Label("(-) " + pair.Key, GUILayout.Width(w1));
                    GUILayout.Label(pair.Value.type.ToString(), GUILayout.Width(w2));
                    GUILayout.Label(Converter.to_string(pair.Value.value), GUILayout.Width(w3));

                } else { 
                    
                    GUILayout.Label(pair.Key, GUILayout.Width(w1));
                    GUILayout.Label(pair.Value.type.ToString(), GUILayout.Width(w2));
                    GUILayout.Label(Converter.to_string(pair.Value.value), GUILayout.Width(w3));
                    if (pair.Value.xml.Dim > 0) {
                        GUILayout.Label("[d=" + pair.Value.xml.Dim + "][s=" + pair.Value.xml.Sizes + "][s=" + pair.Value.xml.Separator + "]", GUILayout.Width(w4));
                    }                    
                }

                GUILayout.EndHorizontal();
            }

        }
    }
}
