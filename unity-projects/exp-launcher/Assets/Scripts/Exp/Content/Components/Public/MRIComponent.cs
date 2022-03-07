
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
using System.Diagnostics;
using System.Collections;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class MRIComponent : ExComponent{

        public float currentDistance = 0f;
        public float currentFactor = 0f;

        private HumanoidControllerComponent m_controller = null;
        private KeyboardComponent m_keyboard = null;
        private List<LoggerComponent> m_loggers = null;
        private KeyCode triggerCode;

        private TransformValue initAvatarTr = null;
        private Vector3 initPos;
        private Vector3 initHeadOffsetPos;
        private Vector3 initHeadOffsetRot;

        public GameObject m_mriGO = null;
        public List<GameObject> m_roomElementsGO = null;
        public List<GameObject> m_magnetElementsGO = null;
        public List<Light> m_lights = null;
        public List<GameObject> m_emissiveLights = null;
        public GameObject m_movingTableGO = null;

        private bool processSolving = false;
        private bool processMoving = false;

        Vector3 initKneePosition = Vector3.zero;
        Vector3 currentKneePosition = Vector3.zero;

        #region ex_functions
        protected override bool initialize() {

            triggerCode = (KeyCode)System.Enum.Parse(typeof(KeyCode), initC.get<string>("trigger_key"));

            add_slot("keyboard button", (button) => {
                var b = (Input.KeyboardButtonEvent)button;
                if (b.code == triggerCode) {
                    string triggerLine = string.Format("TRIGGER,{0},{1},{2},{3}",
                        currentRoutine.name,
                        currentCondition.name,
                        Converter.to_string(time().ellapsed_exp_ms()),
                        Converter.to_string(time().ellapsed_element_ms())
                    );
                    foreach (var logger in m_loggers) {
                        logger.write(triggerLine, true);
                    }

                    if (current_config().get<bool>("trigger_go_next")) {
                        command().next();
                    }
                }
            });

            add_slot("init knee", (value) => {
                var strAny = (StringAny)value;
                if (strAny.str == "Knee") {
                    if (strAny.value is TransformValue transformV) {
                        initKneePosition = transformV.position;
                    }
                    if (strAny.value is Vector3 position) {
                        initKneePosition = position;
                    }
                }
            });
            add_slot("update knee", (value) => {
                var strAny = (StringAny)value;
                if (strAny.str == "Knee") {
                    if (strAny.value is TransformValue transformV) {
                        currentKneePosition = transformV.position;
                    }
                    if (strAny.value is Vector3 position) {
                        currentKneePosition = position;
                    }
                }
            });

            // instantiate MRI scene
            m_mriGO = ExVR.GlobalResources().instantiate_prebab("Components/MRI", transform);
            m_mriGO.name = "MRI_scene";

            // retrieve scene elements
            var tr = m_mriGO.transform;
            m_movingTableGO = tr.Find("moving_table").gameObject;

            var roomTr = tr.Find("room");
            // magnet
            m_magnetElementsGO = new List<GameObject>();
            m_magnetElementsGO.Add(roomTr.Find("MRI/Table").gameObject);
            //m_magnetElementsGO.Add(roomTr.Find("MRI/Magnet").gameObject);
            // room
            // # elements
            m_roomElementsGO = new List<GameObject>();
            var roomsElementsTr = roomTr.Find("RoomAndOthers");
            for (int ii = 0; ii < roomsElementsTr.childCount; ++ii) {
                m_roomElementsGO.Add(roomsElementsTr.GetChild(ii).gameObject);
            }
            // # cameras and bars
            var camerasTr = roomTr.Find("Qualysis cameras");
            var bars1 = camerasTr.Find("Bars1");
            for (int ii = 0; ii < bars1.childCount; ++ii) {
                m_roomElementsGO.Add(bars1.GetChild(ii).gameObject);
            }
            var bars2 = camerasTr.Find("Bars2");
            for (int ii = 0; ii < bars2.childCount; ++ii) {
                m_roomElementsGO.Add(bars2.GetChild(ii).gameObject);
            }
            var bars3 = camerasTr.Find("Bars3");
            for (int ii = 0; ii < bars3.childCount; ++ii) {
                m_roomElementsGO.Add(bars3.GetChild(ii).gameObject);
            }
            var bars4 = camerasTr.Find("Bars4");
            for (int ii = 0; ii < bars4.childCount; ++ii) {
                m_roomElementsGO.Add(bars4.GetChild(ii).gameObject);
            }

            //m_roomElementsGO

            // lights
            m_lights = new List<Light>();
            var lightsParent = roomTr.transform.Find("Lights");
            for (int ii = 0; ii < lightsParent.childCount; ++ii) {
                var lightTr = lightsParent.GetChild(ii);
                var lightC = lightTr.GetComponent<Light>();
                if (lightC != null) {
                    m_lights.Add(lightC);
                }
            }
            m_emissiveLights = new List<GameObject>();
            var emissiveLightsParent = roomTr.transform.Find("Lights/ObjectsCilynderLight");
            for (int ii = 0; ii < emissiveLightsParent.childCount; ++ii) {
                m_emissiveLights.Add(emissiveLightsParent.GetChild(ii).gameObject);
            }

            return true;
        }


        protected override void start_experiment() {

            // components
            m_controller = initC.get_component<HumanoidControllerComponent>("humanoid_controller");
            m_keyboard = initC.get_component<KeyboardComponent>("keyboard");
            m_loggers = initC.get_components_list<LoggerComponent>("loggers");

            foreach (var go in m_roomElementsGO) {
                go.GetComponent<Renderer>().material.SetFloat("_SliceAmount", 0);
                go.GetComponent<Renderer>().shadowCastingMode = UnityEngine.Rendering.ShadowCastingMode.On;
                go.GetComponent<Renderer>().receiveShadows = true;
                go.SetActive(true);
            }
            foreach (var go in m_magnetElementsGO) {
                go.GetComponent<Renderer>().material.SetFloat("_SliceAmount", 0);
                go.GetComponent<Renderer>().shadowCastingMode = UnityEngine.Rendering.ShadowCastingMode.On;
                go.GetComponent<Renderer>().receiveShadows = true;
                go.SetActive(true);
            }

            foreach (var light in m_lights) {
                light.gameObject.SetActive(true);
            }
            foreach (var emissiveLight in m_emissiveLights) {
                emissiveLight.SetActive(true);
            }
        }

        public override void update_from_current_config() {
            reset_config_transform();
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        protected override void start_routine() {

            if (m_keyboard != null) {
                // trigger event                
                var slot = connections().get_slot("keyboard button");
                var signal = m_keyboard.connections().get_signal("button");
                slot.connect(signal);
            }

            if (initAvatarTr == null) {
                if (m_controller != null) {
                    initAvatarTr = m_controller.current_config().get_transform("transform");
                    initPos = initAvatarTr.position;
                    initHeadOffsetPos = m_controller.current_config().get_vector3("head_tracking_offset_pos");
                    initHeadOffsetRot = m_controller.current_config().get_vector3("head_tracking_offset_rot");
                }
            }

            if (currentC.get<bool>("dissolve")) {

                List<GameObject> objectsGO = new List<GameObject>();

                if (currentC.get<bool>("process_room")) {
                    foreach (var go in m_roomElementsGO) {
                        objectsGO.Add(go);
                    }
                }
                if (currentC.get<bool>("process_magnet")) {
                    foreach (var go in m_magnetElementsGO) {
                        objectsGO.Add(go);
                    }
                }

                StartCoroutine(dissolve_process(objectsGO, (float)currentC.get<double>("duration")));

            } else if (currentC.get<bool>("solve")) {

                List<GameObject> objectsGO = new List<GameObject>();

                if (currentC.get<bool>("process_room")) {
                    foreach (var go in m_roomElementsGO) {
                        objectsGO.Add(go);
                    }
                }
                if (currentC.get<bool>("process_magnet")) {
                    foreach (var go in m_magnetElementsGO) {
                        objectsGO.Add(go);
                    }
                }

                StartCoroutine(solve_process(objectsGO, (float)currentC.get<double>("duration")));

            } else if (currentC.get<bool>("move_table_inside")) {
                StartCoroutine(move_table_inside((float)currentC.get<double>("duration")));
            } else if (currentC.get<bool>("move_table_outside")) {
                StartCoroutine(move_table_outside((float)currentC.get<double>("duration")));
            }
        }

        protected override void stop_routine() {
            processSolving = false;
            processMoving = false;
        }

        protected override void update() {

            if (currentC.get<bool>("move_table_qualisys")) {

                var maxDistance = (float)currentC.get<double>("distance");
                var currentDistance = (currentKneePosition - initKneePosition).magnitude;
                if (currentDistance > maxDistance) {
                    currentDistance = maxDistance;
                }
                move_table(currentDistance / maxDistance);

                //m_movingTableGO.transform.localPosition = new Vector3(0.382f, 0.982f, -0.88f + currentDistance);

                //if (m_controller != null) {
                //    initAvatarTr.position = initPos + new Vector3(0, 0, currentDistance);
                //    m_controller.current_config().set_transform("transform", initAvatarTr);

                //    float factor = currentDistance / maxDistance;

                //    m_controller.current_config().set_vector3("head_tracking_offset_pos",
                //        initHeadOffsetPos + factor * (currentC.get_vector3("target_head_tr_offset") - initHeadOffsetPos));
                //    m_controller.current_config().set_vector3("head_tracking_offset_rot",
                //        initHeadOffsetRot + factor * (currentC.get_vector3("target_head_rot_offset") - initHeadOffsetRot));
                //    m_controller.update_from_current_config();
                //}
            }
        }

        protected override void set_visibility(bool visibility) {
            m_mriGO.SetActive(visibility);
        }


        #endregion
        #region private_functions


        private IEnumerator dissolve_process(List<GameObject> objectsGO, float duration) {

            processSolving = true;
            float normalizedTime = 0;
            Stopwatch timer = new Stopwatch();
            timer.Start();
            while (normalizedTime <= 1f) {

                normalizedTime = timer.ElapsedMilliseconds * 0.001f / duration;

                foreach (var go in objectsGO) {
                    go.GetComponent<Renderer>().material.SetFloat("_SliceAmount", normalizedTime);
                }

                if (!processSolving) {
                    break;
                }

                yield return null;
            }

            foreach (var go in objectsGO) {
                go.GetComponent<Renderer>().material.SetFloat("_SliceAmount", 1);
                go.GetComponent<Renderer>().shadowCastingMode = UnityEngine.Rendering.ShadowCastingMode.Off;
                go.GetComponent<Renderer>().receiveShadows = false;
                go.SetActive(false);
            }

            foreach (var light in m_lights) {
                light.gameObject.SetActive(false);
            }
            foreach (var emissiveLight in m_emissiveLights) {
                emissiveLight.SetActive(false);
            }

            processSolving = false;
        }

        private IEnumerator solve_process(List<GameObject> objectsGO, float duration) {

            processSolving = true;
            float normalizedTime = 0;
            Stopwatch timer = new Stopwatch();
            timer.Start();
            while (normalizedTime <= 1f) {

                normalizedTime = timer.ElapsedMilliseconds * 0.001f / duration;

                foreach (var go in objectsGO) {
                    go.GetComponent<Renderer>().material.SetFloat("_SliceAmount", 1 - normalizedTime);
                    go.SetActive(true);
                }

                if (!processSolving) {
                    break;
                }

                yield return null;
            }

            foreach (var go in objectsGO) {
                go.GetComponent<Renderer>().material.SetFloat("_SliceAmount", 0);
                go.GetComponent<Renderer>().shadowCastingMode = UnityEngine.Rendering.ShadowCastingMode.On;
                go.GetComponent<Renderer>().receiveShadows = true;
            }

            foreach (var light in m_lights) {
                light.gameObject.SetActive(true);
            }
            foreach (var emissiveLight in m_emissiveLights) {
                emissiveLight.SetActive(true);
            }

            processSolving = false;
        }

        private IEnumerator move_table_inside(float duration) {

            processMoving = true;
            float normalizedTime = 0;
            Stopwatch timer = new Stopwatch();
            timer.Start();
            while (normalizedTime <= 1f) {

                normalizedTime = timer.ElapsedMilliseconds * 0.001f / duration;
                move_table(normalizedTime);

                if (!processMoving) {
                    break;
                }

                yield return null;
            }
        }

        private IEnumerator move_table_outside(float duration) {

            processMoving = true;
            float normalizedTime = 0;
            Stopwatch timer = new Stopwatch();
            timer.Start();
            while (normalizedTime <= 1f) {

                normalizedTime = timer.ElapsedMilliseconds * 0.001f / duration;
                move_table(1 - normalizedTime);

                if (!processMoving) {
                    break;
                }

                yield return null;
            }
        }


        #endregion
        #region public_functions

        public void move_table(float factor) {

            currentFactor = factor;
            currentDistance = (float)currentC.get<double>("distance") * currentFactor;

            var headTrOffset = currentC.get_vector3("target_head_tr_offset");
            var headRotOffset = currentC.get_vector3("target_head_rot_offset");
            m_movingTableGO.transform.localPosition = new Vector3(0.382f, 0.982f, -0.88f + currentDistance);

            if (m_controller != null) {

                initAvatarTr.position = initPos + new Vector3(0, 0, currentDistance);
                m_controller.current_config().set_transform("transform", initAvatarTr);

                m_controller.current_config().set_vector3("head_tracking_offset_pos",
                    initHeadOffsetPos + currentFactor * (headTrOffset - initHeadOffsetPos));
                m_controller.current_config().set_vector3("head_tracking_offset_rot",
                    initHeadOffsetRot + currentFactor * (headRotOffset - initHeadOffsetRot));
                m_controller.update_from_current_config();
            } 
        }

        #endregion


    }
}