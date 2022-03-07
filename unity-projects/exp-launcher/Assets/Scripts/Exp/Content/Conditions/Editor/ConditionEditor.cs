
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

    [CustomEditor(typeof(Condition))]
    public class ConditionEditor : Editor
    {

        public override bool RequiresConstantRepaint() {
            return true;
        }

        public override void OnInspectorGUI() {

            
            if (!Application.isPlaying) {
                return;
            }

            Condition condition = (Condition)target;

            var win = Screen.width;
            var w1 = win * 0.1f;
            var w2 = win * 0.4f;
            var w3 = win * 0.4f;

            EditorGUILayout.LabelField("All connections: ");
            GUILayout.BeginHorizontal();
            GUILayout.Label("N°", GUILayout.Width(w1));
            GUILayout.Label("output", GUILayout.Width(w2));
            GUILayout.Label("input", GUILayout.Width(w3));
            GUILayout.EndHorizontal();

            for (int ii = 0; ii < condition.connections.Count; ++ii) {

                GUILayout.BeginHorizontal();
                GUILayout.Label("("+ ii +")", GUILayout.Width(w1));
                EditorGUILayout.ObjectField(condition.connections[ii].start, typeof(Routine), true, GUILayout.Width(w2));
                EditorGUILayout.ObjectField(condition.connections[ii].end, typeof(Routine), true, GUILayout.Width(w3));
                GUILayout.EndHorizontal();
            }

        }
    }
}
