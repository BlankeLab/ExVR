
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

    [CustomEditor(typeof(Routine))]
    public class RoutineEditor : Editor{

        public override bool RequiresConstantRepaint() {
            return true;
        }

        public override void OnInspectorGUI() {

            base.OnInspectorGUI();
            if (!Application.isPlaying) {
                return;
            }

            Routine routine = (Routine)target;
            EditorGUILayout.LabelField("ID: ", routine.key().ToString());            
            EditorGUILayout.LabelField("Start time: " + routine.start_timer_duration_ms() + " ms");
            EditorGUILayout.LabelField("Stop time: " + routine.stop_timer_duration_ms() + " ms");
            EditorGUILayout.LabelField("Current condition: ");
            EditorGUILayout.ObjectField(routine.current_condition(), typeof(Condition), true);

            EditorGUILayout.LabelField("All conditions: ");
            foreach (var condition in routine.get_conditions()) {
                EditorGUILayout.ObjectField(condition, typeof(Condition), true);
            }            
        }
    }
}
