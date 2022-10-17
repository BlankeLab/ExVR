
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
using System.Text;

// unity
using UnityEngine;

// TMPro
using TMPro;

namespace Ex {

    public class ExpInfos : MonoBehaviour {

        public TextMeshProUGUI m_cameraMode = null;
        public TextMeshProUGUI m_tRoutine = null;
        public TextMeshProUGUI m_tExp = null;
        public TextMeshProUGUI m_status = null;
        public TextMeshProUGUI m_mode = null;
        public TextMeshProUGUI m_routine = null;
        public TextMeshProUGUI m_condition = null;
        public TextMeshProUGUI m_exp = null;
        public TextMeshProUGUI m_instance = null;

        void Update() {
            var scheduler = ExVR.Scheduler();
            var time = ExVR.Time();
            
            string routineTime = "-";
            string duration = "-";
            if(scheduler.current_interval() != null) {
                routineTime = Converter.to_string(time.ellapsed_element_s(), "0.00");
                duration = Converter.to_string(scheduler.current_interval().duration(), "0.00");
            }

            m_tRoutine.text = string.Format("Routine: {0} / {1}", routineTime, duration);
            m_tExp.text = string.Format("Total: {0}" , Converter.to_string(time.ellapsed_exp_s(), "0.00"));

            if (ExVR.Experiment().is_loaded()) {
                if (time.is_experiment_started()) {

                    if (ExVR.Time().is_paused()) {
                        m_status.text = "Paused";
                        m_status.faceColor = Color.yellow;
                        m_status.outlineColor = Color.yellow;
                    } else {
                        m_status.text = "Running";
                        m_status.faceColor = Color.green;
                        m_status.outlineColor = Color.green;
                    }
                } else {
                    m_status.text = "Idle";
                    m_status.faceColor = Color.white;
                    m_status.outlineColor = Color.white;
                }
            } else {
                m_status.text = "Not loaded";
                m_status.faceColor = Color.red;
                m_status.outlineColor = Color.red;
            }

            switch (GetComponent<ButtonsActionsExp>().debugCamState) {
                case ButtonsActionsExp.DebugCameraState.expanded:
                    m_cameraMode.text = "Full";
                    break;
                case ButtonsActionsExp.DebugCameraState.hidden:
                    m_cameraMode.text = "Off";
                    break;
                case ButtonsActionsExp.DebugCameraState.minimized:
                    m_cameraMode.text = "Min";
                    break;
            }

            m_mode.text = ExVR.Display().current_mode().ToString();

            var info = scheduler.current_element_info();
            if(info != null) {
                m_routine.text = info.name();
                m_condition.text = ((RoutineInfo)info).condition().name;

                m_routine.faceColor = Color.green;
                m_routine.outlineColor = Color.green;

                m_condition.faceColor = Color.green;
                m_condition.outlineColor = Color.green;

            } else {
                m_routine.text = "...";
                m_condition.text = "...";

                m_routine.faceColor = Color.red;
                m_routine.outlineColor = Color.red;

                m_condition.faceColor = Color.red;
                m_condition.outlineColor = Color.red;
            }

            if (ExVR.Experiment().is_loaded()) {
                m_exp.text = ExVR.Experiment().experimentName;
                m_exp.faceColor = Color.green;
                m_exp.outlineColor = Color.green;

                m_instance.text = ExVR.Experiment().instanceName;
                m_instance.faceColor = Color.green;
                m_instance.outlineColor = Color.green;
            } else {
                m_exp.text = "...";
                m_exp.faceColor = Color.red;
                m_exp.outlineColor = Color.red;

                m_instance.text = "...";
                m_instance.faceColor = Color.red;
                m_instance.outlineColor = Color.red;
            }
        }
    }
}