
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
using UnityEngine.UI;

// TMPro
using TMPro;

namespace Ex {

    public class ButtonsUIComponent : CanvasWorldSpaceComponent {

        private GameObject m_buttonsUiGO = null;
        private TextMeshProUGUI m_topText    = null;
        private TextMeshProUGUI m_bottomText = null;
        private TextMeshProUGUI m_leftText   = null;
        private TextMeshProUGUI m_rightText  = null;
        private Transform m_buttonsParent = null;
        private List<GameObject> m_buttonsLine = null;

        private Dictionary<int, Dictionary<int, Button>> m_buttons = null;
        private Dictionary<int, Dictionary<int, TextMeshProUGUI>> m_buttonsText = null;
        private List<System.Tuple<Button, TextMeshProUGUI>> m_buttonsL = null;
        private List<int> m_buttonsNb = null;

        private Material m_buttonsMat = null;

        public int selectedButtonId = -1;

        #region ex_functions

        protected override bool initialize() {

            // signals
            add_signal("validated id");
            add_signal("validated text");
            // slots            
            add_slot("set id", (buttonId) => {
                select_button((int)buttonId);
            });
            add_slot("next id", (nullArg) => {
                select_next_button();
            });
            add_slot("previous id", (nullArg) => {
                select_previous_button();
            });
            add_slot("validate", (nullArg) => {
                validate_button();
            });

            m_buttonsUiGO = ExVR.GlobalResources().instantiate_prebab("Components/ButtonsUI", transform);            
            m_buttonsUiGO.name = "Buttons UI";
            m_buttonsUiGO.GetComponent<UnityEngine.UI.Image>().material = ExVR.GlobalResources().instantiate_default_transparent_mat();
            m_buttonsMat = ExVR.GlobalResources().instantiate_default_ui_mat();

            m_topText       = m_buttonsUiGO.transform.Find("Top text").GetComponent<TextMeshProUGUI>();
            m_bottomText = m_buttonsUiGO.transform.Find("Bottom text").GetComponent<TextMeshProUGUI>();
            m_leftText = m_buttonsUiGO.transform.Find("Middle/Left text").GetComponent<TextMeshProUGUI>();       
            m_rightText = m_buttonsUiGO.transform.Find("Middle/Right text").GetComponent<TextMeshProUGUI>();
            m_buttonsParent = m_buttonsUiGO.transform.Find("Middle/Buttons");


            return true;
        }



        protected override void set_visibility(bool visibility) {
            m_buttonsUiGO.SetActive(visibility);
        }

        public override void update_from_current_config() {
            resize_container();
            update_ui();
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
            if (updatedArgName == "id") {
                if (currentC.get<bool>("update_id")) {
                    select_button(currentC.get<int>("id"));
                }
            }
        }

        protected override void start_routine() {
            if (currentC.get<bool>("update_id")){
                select_button(currentC.get<int>("id"));
            } else if (currentC.get<bool>("randomize_id")) {
                select_button(Random.Range(0, m_buttonsL.Count));
            }
        }

        protected override void post_update() {
            resize_container();
        }


        #endregion

        #region private_functions
        private void resize_container() {

            m_buttonsUiGO.transform.position = Vector3.zero;
            m_buttonsUiGO.transform.rotation = Quaternion.identity;

            var rTr = m_buttonsUiGO.GetComponent<RectTransform>();
            rTr.pivot = new Vector2(0.5f, 0.5f);

            if (currentC.get<bool>("use_eye_camera")) {

                // move to head
                Transform camTr = ExVR.Display().cameras().get_eye_camera_transform();
                m_buttonsUiGO.transform.position = camTr.position + camTr.forward * currentC.get<float>("distance");
                m_buttonsUiGO.transform.rotation = camTr.rotation;
                m_buttonsUiGO.transform.eulerAngles += currentC.get_vector3("rotation");
                rTr.pivot = currentC.get_vector2("pivot");

            } else {
                rTr.localPosition = currentC.get_vector3("position");
                rTr.localEulerAngles = currentC.get_vector3("rotation");
            }

            rTr.sizeDelta =
                new Vector2(currentC.get<int>("width"), currentC.get<int>("height")
            );

            var sf = currentC.get<float>("scale_factor") * 0.01f;
            rTr.localScale = new Vector3(
                sf, sf, sf
            );
        }

        private void update_ui() {

            // container
            // # background            
            m_buttonsUiGO.GetComponent<UnityEngine.UI.Image>().material.color = currentC.get_color("background_color");

            // textes                        
            currentC.update_text("left_t", m_leftText);
            currentC.update_text("right_t", m_rightText);
            currentC.update_text("top_t", m_topText);
            currentC.update_text("bottom_t", m_bottomText);

            m_leftText.gameObject.SetActive(m_leftText.text.Length > 0);
            m_rightText.gameObject.SetActive(m_rightText.text.Length > 0);
            m_topText.gameObject.SetActive(m_topText.text.Length > 0);
            m_bottomText.gameObject.SetActive(m_bottomText.text.Length > 0);


            // buttons
            string buttonsText = currentC.get<string>("buttons_text");
            var lines = buttonsText.Split(new char[] { '\n' }, System.StringSplitOptions.None);

            bool reset = false;
            if (m_buttonsNb == null) {
                reset = true;
            } else if (m_buttonsNb.Count != lines.Length) {
                reset = true;
            } else {

                for (int ii = 0; ii < lines.Length; ++ii) {
                    if (lines[ii].Length != m_buttonsNb[ii]) {
                        reset = true;
                        break;
                    }
                }
            }
            
            // reconstruct buttons           
            if (reset) {

                if (m_buttonsLine != null) {
                    foreach(var go in m_buttonsLine) {
                        Destroy(go);
                    }
                }

                m_buttons     = new Dictionary<int, Dictionary<int, Button>>(lines.Length);
                m_buttonsText = new Dictionary<int, Dictionary<int, TextMeshProUGUI>>(lines.Length);
                m_buttonsLine = new List<GameObject>(lines.Length);

                m_buttonsL    = new List<System.Tuple<Button, TextMeshProUGUI>>();
                m_buttonsNb   = new List<int>(lines.Length);
                selectedButtonId = -1;

                for (int ii = 0; ii < lines.Length; ++ii) {

                    var lineGO = GO.generate_empty_scene_object(string.Format("Line{0}", ii), m_buttonsParent, true);
                    lineGO.transform.localPosition = new Vector3(0, 0, -0.01f);
                    lineGO.transform.localRotation = Quaternion.identity;
                    lineGO.transform.localScale = new Vector3(1, 1, 1);
                    var le = lineGO.AddComponent<LayoutElement>();
                    le.flexibleWidth = 1f;
                    le.flexibleHeight = 1f;

                    var hlg = lineGO.AddComponent<HorizontalLayoutGroup>();
                    hlg.childControlWidth = true;
                    hlg.childControlHeight = true;
                    hlg.childScaleWidth = false;
                    hlg.childScaleHeight = false;
                    hlg.childForceExpandWidth = true;
                    hlg.childForceExpandHeight = true;

                    m_buttonsLine.Add(lineGO);

                    if (lines[ii].Length == 0) {
                        m_buttonsNb.Add(0);
                    } else {

                        var buttonsName = lines[ii].Split(new char[] { '_' }, System.StringSplitOptions.None); ;
                        var lineButtons = new Dictionary<int, Button>(buttonsName.Length);
                        var lineButtonsText = new Dictionary<int, TextMeshProUGUI>(buttonsName.Length);
                        for (int jj = 0; jj < buttonsName.Length; ++jj) {

                            if (buttonsName[jj].Length == 0) {
                                continue;
                            }

                            var buttonGO = ExVR.GlobalResources().instantiate_prebab("Common/Button", lineGO.transform, "Button");
                            
                            buttonGO.transform.localPosition = Vector3.zero;
                            buttonGO.transform.localScale = new Vector3(1, 1, 1);
                            buttonGO.transform.localRotation = Quaternion.identity;

                            var button = buttonGO.GetComponent<Button>();
                            button.GetComponent<UnityEngine.UI.Image>().material = m_buttonsMat;
                            button.interactable = false;
                            buttonGO.SetActive(true);

                            lineButtons.Add(jj, button);
                            var textGUI = buttonGO.transform.Find("Text (TMP)").gameObject.GetComponent<TextMeshProUGUI>();
                            lineButtonsText.Add(jj, textGUI);

                            m_buttonsL.Add(new System.Tuple<Button, TextMeshProUGUI>(button, textGUI));
                            if (selectedButtonId == -1) {
                                selectedButtonId = 0;
                            }

                        }
                        m_buttons.Add(ii, lineButtons);
                        m_buttonsText.Add(ii, lineButtonsText);
                        m_buttonsNb.Add(lineButtons.Count);
                    }
                }
            }

            // update text
            for (int ii = 0; ii < lines.Length; ++ii) {
                var buttonsName = lines[ii].Split(new char[] { '_' }, System.StringSplitOptions.None);
                for (int jj = 0; jj < buttonsName.Length; ++jj) {
                    if (buttonsName[jj].Length > 0) {
                        currentC.update_text("buttons_ts", m_buttonsText[ii][jj]);
                        m_buttonsText[ii][jj].text = buttonsName[jj];
                    }
                }
            }
            m_buttonsMat.color = currentC.get_color("buttons_color");

            // select button
            select_button(selectedButtonId);
        }

        #endregion

        #region public_functions

        public void select_next_button() {

            if (m_buttonsL.Count == 0) {
                return;
            }

            if (selectedButtonId == -1) {
                selectedButtonId = 0;
            }

            if(selectedButtonId == m_buttonsL.Count - 1) {
                select_button(0);
            } else {
                select_button(selectedButtonId + 1);
            }
        }

        public void select_previous_button() {

            if(m_buttonsL.Count == 0) {
                return;
            }

            if (selectedButtonId == -1) {
                selectedButtonId = 0;
            }

            if (selectedButtonId == 0) {
                select_button(m_buttonsL.Count - 1);
            } else {
                select_button(selectedButtonId - 1);
            }
        }

        public void select_button(int id) {

            if (m_buttonsL.Count == 0) {
                return;
            }
            if(id == selectedButtonId) {
                return;
            }

            for (int ii = 0; ii < m_buttonsL.Count; ++ii) {
                m_buttonsL[ii].Item1.interactable = (ii == id);
            }
            selectedButtonId = id;
        }

        public void validate_button() {
            if (selectedButtonId >= 0 && selectedButtonId < m_buttonsL.Count) {                
                invoke_signal("validated id", selectedButtonId);
                invoke_signal("validated text", m_buttonsL[selectedButtonId].Item2.text);
            }
        }

        #endregion
    }
}