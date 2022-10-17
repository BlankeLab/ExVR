
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
using UnityEngine.UI;

// TMPro
using TMPro;

namespace Ex{

    public class SliderUIComponent : CanvasWorldSpaceComponent{
        
        private GameObject m_sliderGO = null;
        
        private Slider m_slider = null;
        private TextMeshProUGUI m_descriptionText1 = null;
        private TextMeshProUGUI m_descriptionText2 = null;        
        private TextMeshProUGUI m_sliderText1 = null;
        private TextMeshProUGUI m_sliderText2 = null;
        private TextMeshProUGUI m_sliderValueText = null;


        private bool valueIntialized = false;


        #region ex_functions
        protected override bool initialize() {

            // signals
            add_signal("value updated");
            // slots            
            add_slot("set value", (value) => { set_value((float)value); });
            add_slot("increase", (amout) => { increase((float)amout); });
            add_slot("decrease", (amout) => { decrease((float)amout); });            

            // init gameObjects            
            if (initC.get<bool>("horizontal_slider")) {
                m_sliderGO = ExVR.GlobalResources().instantiate_prebab("Components/SliderUI/Horizontal", transform); 
                m_sliderGO.name = "Horizontal slider";
            } else {
                m_sliderGO = ExVR.GlobalResources().instantiate_prebab("Components/SliderUI/Vertical", transform);
                m_sliderGO.name = "Vertical slider";
            }

            // find sub go
            if (initC.get<bool>("horizontal_slider")) {
            
                m_descriptionText1 = m_sliderGO.transform.Find("TopText").GetComponent<TextMeshProUGUI>();
                m_descriptionText2 = m_sliderGO.transform.Find("BottomText").GetComponent<TextMeshProUGUI>();

                var sliderPanel = m_sliderGO.transform.Find("SliderPanel");
                m_slider = sliderPanel.Find("Slider").GetComponent<Slider>();
                m_sliderValueText = sliderPanel.Find("Slider").Find("SliderValueText").GetComponent<TextMeshProUGUI>();
                m_sliderText1 = sliderPanel.Find("LeftSliderText").GetComponent<TextMeshProUGUI>();
                m_sliderText2 = sliderPanel.Find("RightSliderText").GetComponent<TextMeshProUGUI>();

            } else {

                m_descriptionText1 = m_sliderGO.transform.Find("LeftText").GetComponent<TextMeshProUGUI>();
                m_descriptionText2 = m_sliderGO.transform.Find("RightText").GetComponent<TextMeshProUGUI>();

                var sliderPanel = m_sliderGO.transform.Find("SliderPanel");
                m_slider = sliderPanel.Find("Slider").GetComponent<Slider>();
                m_sliderValueText = sliderPanel.Find("Slider").Find("SliderValueText").GetComponent<TextMeshProUGUI>();
                m_sliderText1 = sliderPanel.Find("TopSliderText").GetComponent<TextMeshProUGUI>();
                m_sliderText2 = sliderPanel.Find("BottomSliderText").GetComponent<TextMeshProUGUI>();
            }
           
            return true;
        }

        protected override void start_experiment() {
            valueIntialized = false;
        }

        protected override void start_routine() {
            init_value();
        }

        protected override void update_parameter_from_gui(string updatedArgName) {


            if (updatedArgName == "normal_start" || updatedArgName == "random_start" || updatedArgName == "once_start" || updatedArgName == "once_random_start") {

                if (currentC.get<bool>(updatedArgName)) {
                    init_value();
                }

            } else if (updatedArgName == "value" || updatedArgName == "min" || updatedArgName == "max" || updatedArgName == "whole") {
                set_value(currentC.get<float>("value"));                
            }

            update_from_current_config();
        }

        public override void update_from_current_config() {

            // update text ui
            currentC.update_text("t1", m_descriptionText1);
            currentC.update_text("t2", m_descriptionText2);
            update_min_max_value_textes(m_slider.wholeNumbers, m_slider.minValue, m_slider.value, m_slider.maxValue);

            m_sliderGO.GetComponent<UnityEngine.UI.Image>().color = currentC.get_color("background_color");
            m_slider.transform.Find("Fill Area").Find("Fill").GetComponent<UnityEngine.UI.Image>().color = currentC.get_color("fill_area_color");
            m_slider.transform.Find("Handle Slide Area").Find("Handle").GetComponent<UnityEngine.UI.Image>().color = currentC.get_color("handle_color");
            m_slider.transform.Find("Background").GetComponent<UnityEngine.UI.Image>().color = currentC.get_color("rest_area_color");
        }

        protected override void pre_update() {
            resize_container();
        }

        protected override void set_visibility(bool visibility) {
            m_sliderGO.SetActive(visibility);
        }

        #endregion

        #region private_functions

        private void update_min_max_value_textes(bool whole, float min, float value, float max) {

            string minValueText = null, maxValueText = null, currentSliderValueText = null;
            if (currentC.get<bool>("display_min_max")) {

                minValueText = (!whole ? min : (int)min).ToString();
                maxValueText = (!whole ? max : (int)max).ToString();
                currentSliderValueText = "";

            } else if (currentC.get<bool>("display_min_max_value")) {

                minValueText = (!whole ? min : (int)min).ToString();
                maxValueText = (!whole ? max : (int)max).ToString();

                if (whole) {
                    currentSliderValueText = ((int)value).ToString();
                } else {
                    currentSliderValueText = value.ToString("G4");
                }

            } else if (currentC.get<bool>("display_slider_textes_text")) {

                minValueText = currentC.get<string>("slider_text1");
                maxValueText = currentC.get<string>("slider_text2");
                currentSliderValueText = "";
            }else { // nothing
                minValueText = "";
                maxValueText = "";
                currentSliderValueText = "";
            }
            // # min
            currentC.update_text("tmin", m_sliderText1, minValueText);
            // # max
            currentC.update_text("tmax", m_sliderText2, maxValueText);
            // # value
            currentC.update_text("tvalue", m_sliderValueText, currentSliderValueText);
        }

        private void init_value() {

            if (currentC.get<bool>("normal_start")) {

                set_value(currentC.get<float>("value"));

            } else if (currentC.get<bool>("once_start")) {

                if (!valueIntialized) {
                    set_value(currentC.get<float>("value"));
                }

            } else if (currentC.get<bool>("random_start")) {

                if (currentC.get<bool>("whole")) {
                    set_value((float)Random.Range((int)currentC.get<float>("min"), (int)currentC.get<float>("max") + 1));
                } else {
                    set_value(Random.Range(currentC.get<float>("min"), currentC.get<float>("max")));
                }

            } else if (currentC.get<bool>("once_random_start")){

                if (!valueIntialized) {

                    if (currentC.get<bool>("whole")) {
                        set_value((float)Random.Range((int)currentC.get<float>("min"), (int)currentC.get<float>("max") + 1));
                    } else {
                        set_value(Random.Range(currentC.get<float>("min"), currentC.get<float>("max")));
                    }
                }
            } else { // do_nothing
                // TODO: ...
            }
        }

        #endregion

        #region public_functions

        public void set_min(float min) {
            set_min_value_max(min, currentC.get<float>("value"), currentC.get<float>("max"));
        }

        public void set_max(float max) {
            set_min_value_max(currentC.get<float>("min"), currentC.get<float>("value"), max);
        }

        public void set_value(float value) {
            set_min_value_max(currentC.get<float>("min"), value, currentC.get<float>("max"));
        }
        public void set_min_value_max(float min, float value, float max) {

            bool whole = currentC.get<bool>("whole");

            // slider
            m_slider.wholeNumbers = whole;
            // # min
            m_slider.minValue = !whole ? min : (int)min;
            // # max            
            m_slider.maxValue = !whole ? max : (int)max;

            // # clamp value according to min/max
            if (value < min) {
                value = min;
            }
            if (value > max) {
                value = max;
            }
            // # value
            float previousValue = m_slider.value;
            m_slider.value = !whole ? value : (int)value;
            valueIntialized = true;

            // updat textes
            update_min_max_value_textes(whole, min, value, max);

            // send updated slider value
            if (previousValue != m_slider.value) {
                invoke_signal("value updated", m_slider.value);
            }
        }

        public float get_value() {
            return m_slider.value;
        }
        public void decrease(float amount) {
            set_value(get_value() - amount);
        }

        public void increase(float amount) {
            set_value(get_value() + amount);
        }

        public void resize_container() {

            m_sliderGO.transform.position = Vector3.zero;
            m_sliderGO.transform.rotation = Quaternion.identity;

            var rTr = m_sliderGO.GetComponent<RectTransform>();
            rTr.pivot = new Vector2(0.5f, 0.5f);

            if (currentC.get<bool>("use_eye_camera")) {

                // move to head
                Transform camTr = ExVR.Display().cameras().get_eye_camera_transform();
                m_sliderGO.transform.position = camTr.position + camTr.forward * currentC.get<float>("distance");
                m_sliderGO.transform.rotation = camTr.rotation;
                m_sliderGO.transform.eulerAngles += currentC.get_vector3("rotation");
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


        #endregion


    }
}