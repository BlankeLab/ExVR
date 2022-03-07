
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

// TMPro
using TMPro;

namespace Ex{

    public class TextViewerComponent : CanvasWorldSpaceComponent{


        private GameObject m_textGO = null;
        private TextMeshProUGUI m_text = null;

        protected override bool initialize() {

            // init slots
            add_slot("set text", (text) => {
                set_text((string)text);
            });

            // init gameObject
            m_textGO = ExVR.GlobalResources().instantiate_prebab("Components/TextViewer", transform);
            m_textGO.name = "TextRect";
            m_textGO.GetComponent<UnityEngine.UI.Image>().material = ExVR.GlobalResources().instantiate_unlit_transparent_color_mat();
            m_text    = m_textGO.transform.Find("Text (TMP)").GetComponent<TMPro.TextMeshProUGUI>();

            return m_textGO != null;
        }

        protected override void set_visibility(bool visibility) {
            m_textGO.SetActive(visibility);
        }

        public override void update_from_current_config() {
            resize_container();
            update_text();
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        // use_eye_camera
        protected override void post_update() {
            resize_container();
        }

        public void resize_container() {

            m_textGO.transform.position = Vector3.zero;
            m_textGO.transform.rotation = Quaternion.identity;

            var rTr = m_textGO.GetComponent<RectTransform>();
            rTr.pivot = new Vector2(0.5f, 0.5f);

            if (currentC.get<bool>("use_eye_camera")) {

                // move to head
                Transform camTr = ExVR.Display().cameras().get_eye_camera_transform();
                rTr.pivot = currentC.get_vector2("pivot");
                rTr.rotation = camTr.rotation * Quaternion.Euler(currentC.get_vector3("rotation"));
                rTr.position = camTr.position + camTr.forward * currentC.get<float>("distance");

            } else {
                rTr.localPosition = currentC.get_vector3("position");
                rTr.localEulerAngles = currentC.get_vector3("rotation");
            }

            rTr.sizeDelta = new Vector2(currentC.get<int>("width"), currentC.get<int>("height"));

            var sf = currentC.get<float>("scale_factor")*0.01f;
            rTr.localScale = new Vector3(
                sf, sf, sf
            );

            rTr = m_text.GetComponent<RectTransform>();
            rTr.sizeDelta = new Vector2(currentC.get<int>("width"), currentC.get<int>("height"));
        }

        public void set_text(string text) {
            currentC.set<string>("t_text", text);
            update_text();
        }

        public void update_text() {

            // background
            m_textGO.GetComponent<UnityEngine.UI.Image>().material.color = currentC.get_color("background_color");

            // text
            currentC.update_text("t", m_text);
        }
    }
}

//Arial Arial   Arial.ttf	7.00
//Arial Italic    Ariali.ttf	7.00
//Arial Bold  Arialbd.ttf	7.00
//Arial Bold Italic Arialbi.ttf 7.00
//Arial Black Arial Black Ariblk.ttf  5.23
//Bahnschrift Bahnschrift *	Bahnschrift.ttf 2.01
//Calibri Calibri Light Calibril.ttf    6.22

//Calibri Light Italic Calibrili.ttf   6.22

//Calibri Calibri.ttf 6.22

//Calibri Italic  Calibrii.ttf    6.22

//Calibri Bold    Calibrib.ttf    6.22

//Calibri Bold Italic Calibriz.ttf    6.22
//Cambria Cambria Cambria.ttc 6.99

//Cambria Italic  Cambriai.ttf    6.98

//Cambria Bold    Cambriab.ttf    6.98

//Cambria Bold Italic Cambriaz.ttf    6.98
//Cambria Math    Cambria Math    Cambria.ttc 6.99
//Candara Candara Light* Candaral.ttf    5.63

//Candara Light Italic* Candarali.ttf   5.62

//Candara Candara.ttf 5.62

//Candara Italic  Candarai.ttf    5.62

//Candara Bold    Candarab.ttf    5.62

//Candara Bold Italic Candaraz.ttf    5.62
//Comic Sans MS Comic Sans MS   Comic.ttf   5.14

//Comic Sans MS Italic    Comici.ttf  5.14

//Comic Sans MS Bold  Comicbd.ttf 5.14

//Comic Sans MS Bold Italic Comicz.ttf  5.14
//Consolas Consolas    Consola.ttf 7.00

//Consolas Italic Consolai.ttf    7.00

//Consolas Bold   Consolab.ttf    7.00

//Consolas Bold Italic Consolaz.ttf    7.00
//Constantia Constantia  Constan.ttf 5.93

//Constantia Italic   Constani.ttf    5.93

//Constantia Bold Constanb.ttf    5.93

//Constantia Bold Italic Constanz.ttf    5.93
//Corbel Corbel Light* Corbell.ttf 5.90

//Corbel Light Italic* Corbelli.ttf    5.90

//Corbel Corbel.ttf  5.90

//Corbel Italic   Corbeli.ttf 5.90

//Corbel Bold Corbelb.ttf 5.90

//Corbel Bold Italic Corbelz.ttf 5.90
//Courier New Courier New Cour.ttf    6.92

//Courier New Italic Couri.ttf   6.91

//Courier New Bold Courbd.ttf  6.92

//Courier New Bold Italic Courbi.ttf  6.91
//Ebrima Ebrima  Ebrima.ttf  5.12

//Ebrima Bold Ebrimabd.ttf    5.12
//Franklin Gothic Medium Franklin Gothic Medium  Framd.ttf   5.02

//Franklin Gothic Medium Italic   Framdit.ttf 5.01
//Gabriola Gabriola    Gabriola.ttf    5.93
//Gadugi Gadugi  Gadugi.ttf  1.12

//Gadugi Bold Gadugib.ttf 1.12
//Georgia Georgia Georgia.ttf 5.59

//Georgia Italic  Georgiai.ttf    5.59

//Georgia Bold    Georgiab.ttf    5.59

//Georgia Bold Italic Georgiaz.ttf    5.59
//HoloLens MDL2 Assets HoloLens MDL2 Assets *	Holomdl2.ttf    2.02
//Impact Impact  Impact.ttf  5.11
//Ink Free    Ink Free *	Inkfree.ttf 1.00
//Javanese Text   Javanese Text   Javatext.ttf    1.09
//Leelawadee UI   Leelawadee UI   Leelawui.ttf    5.05

//Leelawadee UI Semilight Leeluisl.ttf    5.05

//Leelawadee UI Bold Leelauib.ttf    5.05
//Lucida Console  Lucida Console  Lucon.ttf   5.01
//Lucida Sans Unicode Lucida Sans Unicode L_10646.ttf 5.01
//Malgun Gothic   Malgun Gothic   Malgun.ttf  6.68

//Malgun Gothic Bold Malgunbd.ttf    6.68

//Malgun Gothic Semilight* Malgunsl.ttf    6.68
//Marlett Marlett Marlett.ttf 5.01
//Microsoft Himalaya  Microsoft Himalaya  Himalaya.ttf    5.23
//Microsoft JhengHei  Microsoft JhengHei Light Msjhl.ttc   6.14

//Microsoft JhengHei  Msjh.ttc    6.14

//Microsoft JhengHei Bold MSJHBD.ttc  6.13

//Microsoft JhengHei UI Light Msjhl.ttc   6.14

//Microsoft JhengHei UI Msjh.ttc    6.14

//Microsoft JhengHei UI Bold  MSJHBD.ttc  6.13
//Microsoft New Tai Lue   Microsoft New Tai Lue   Ntailu.ttf  5.99

//Microsoft New Tai Lue Bold Ntailub.ttf 5.99
//Microsoft PhagsPa   Microsoft PhagsPa   Phagspa.ttf 6.00

//Microsoft PhagsPa Bold PhagsPaB.ttf    6.00
//Microsoft Sans Serif Microsoft Sans Serif    Micross.ttf 7.00
//Microsoft Tai Le Microsoft Tai Le    Taile.ttf   6.00

//Microsoft Tai Le Bold   TaiLeb.ttf  6.00
//Microsoft YaHei Microsoft YaHei Light Msyhl.ttc   6.23

//Microsoft YaHei Msyh.ttc    6.25

//Microsoft YaHei Bold Msyhbd.ttc  6.25

//Microsoft YaHei UI Light    Msyhl.ttc   6.23

//Microsoft YaHei UI Msyh.ttc    6.25

//Microsoft YaHei UI Bold Msyhbd.ttc  6.25
//Microsoft Yi Baiti Microsoft Yi Baiti  Msyi.ttf    6.00
//MingLiU-ExtB MingLiU-ExtB Mingliub.ttc    7.02

//PMingLiU-ExtB Mingliub.ttc    7.02

//MingLiU_HKSCS-ExtB Mingliub.ttc    7.02
//Mongolian Baiti Mongolian Baiti Monbaiti.ttf    5.53
//MS Gothic   MS Gothic   Msgothic.ttc    5.30

//MS PGothic  Msgothic.ttc    5.30

//MS UI Gothic Msgothic.ttc    5.30
//MV Boli MV Boli Mvboli.ttf  6.84
//Myanmar Text    Myanmar Text    Mmrtext.ttf 1.18

//Myanmar Text Bold Mmrtextb.ttf    1.18
//Nirmala UI  Nirmala UI Semilight Nirmalas.ttf    1.37

//Nirmala UI  Nirmala.ttf 1.37

//Nirmala UI Bold Nirmalab.ttf    1.37
//Palatino Linotype   Palatino Linotype   Pala.ttf    5.03

//Palatino Linotype Italic Palai.ttf   5.03

//Palatino Linotype Bold Palab.ttf   5.03

//Palatino Linotype Bold Italic   Palabi.ttf  5.03
//Segoe MDL2 Assets Segoe MDL2 Assets *	Segmdl2.ttf 1.68
//Segoe Print Segoe Print Segoepr.ttf 5.04

//Segoe Print Bold Segoeprb.ttf    5.04
//Segoe Script    Segoe Script    Segoesc.ttf 5.02

//Segoe Script Bold Segoescb.ttf    5.02
//Segoe UI    Segoe UI Light Segoeuil.ttf    5.60

//Segoe UI Light Italic   Seguili.ttf 5.30

//Segoe UI Semilight Segoeuisl.ttf   5.60

//Segoe UI Semilight Italic   Seguisli.ttf    5.30

//Segoe UI    Segoeui.ttf 5.60

//Segoe UI Italic Segoeuii.ttf    5.30

//Segoe UI Semibold Seguisb.ttf 5.60

//Segoe UI Semibold Italic    Seguisbi.ttf    5.30

//Segoe UI Bold Segoeuib.ttf    5.60

//Segoe UI Bold Italic    Segoeuiz.ttf    5.30

//Segoe UI Black Seguibl.ttf 2.01

//Segoe UI Black Italic   Seguibli.ttf    2.01
//Segoe UI Historic Segoe UI Historic *	Seguihis.ttf    1.03
//Segoe UI Emoji Segoe UI Emoji  Seguiemj.ttf    1.27
//Segoe UI Symbol Segoe UI Symbol Seguisym.ttf    6.23
//SimSun SimSun  Simsun.ttc  5.16

//NSimSun Simsun.ttc  5.16

//SimSun-ExtB Simsunb.ttf 5.03
//Sitka Sitka Small Sitka.ttc   1.12

//Sitka Small Italic SitkaI.ttc  1.12

//Sitka Small Bold SitkaB.ttc  1.12

//Sitka Small Bold Italic SitkaZ.ttc  1.12

//Sitka Text  Sitka.ttc   1.12

//Sitka Text Italic SitkaI.ttc  1.12

//Sitka Text Bold SitkaB.ttc  1.12

//Sitka Text Bold Italic  SitkaZ.ttc  1.12

//Sitka Subheading    Sitka.ttc   1.12

//Sitka Subheading Italic SitkaI.ttc  1.12

//Sitka Subheading Bold SitkaB.ttc  1.12

//Sitka Subheading Bold Italic    SitkaZ.ttc  1.12

//Sitka Heading   Sitka.ttc   1.12

//Sitka Heading Italic SitkaI.ttc  1.12

//Sitka Heading Bold SitkaB.ttc  1.12

//Sitka Heading Bold Italic   SitkaZ.ttc  1.12

//Sitka Display   Sitka.ttc   1.12

//Sitka Display Italic SitkaI.ttc  1.12

//Sitka Display Bold SitkaB.ttc  1.12

//Sitka Display Bold Italic   SitkaZ.ttc  1.12

//Sitka Banner    Sitka.ttc   1.12

//Sitka Banner Italic SitkaI.ttc  1.12

//Sitka Banner Bold SitkaB.ttc  1.12

//Sitka Banner Bold Italic    SitkaZ.ttc  1.12
//Sylfaen Sylfaen Sylfaen.ttf 5.06
//Symbol Symbol  Symbol.ttf  5.01
//Tahoma Tahoma  Tahoma.ttf  7.00

//Tahoma Bold Tahomabd.ttf    7.00
//Times New Roman Times New Roman Times.ttf   7.00

//Times New Roman Italic  Timesi.ttf  7.00

//Times New Roman Bold    Timesbd.ttf 7.00

//Times New Roman Bold Italic Timesbi.ttf 7.00
//Trebuchet MS    Trebuchet MS    Trebuc.ttf  5.15

//Trebuchet MS Italic Trebucit.ttf    5.15

//Trebuchet MS Bold Trebucbd.ttf    5.15

//Trebuchet MS Bold Italic    Trebucbi.ttf    5.15
//Verdana Verdana Verdana.ttf 5.33

//Verdana Italic  Verdanai.ttf    5.33

//Verdana Bold    Verdanab.ttf    5.33

//Verdana Bold Italic Verdanaz.ttf    5.33
//Webdings Webdings    Webdings.ttf    5.01
//Wingdings Wingdings   Wingding.ttf    5.01
//Yu Gothic   Yu Gothic Light YuGothL.ttc 1.90

//Yu Gothic Regular YuGothR.ttc 1.90

//Yu Gothic Medium* Yugothm.ttc 1.90

//Yu Gothic Bold YuGothB.ttc 1.90

//Yu Gothic UI Light *	YuGothL.ttc 1.90

//Yu Gothic UI Semilight *	YuGothR.ttc 1.90

//Yu Gothic UI Regular *	Yugothm.ttc 1.90

//Yu Gothic UI Semibold *	YuGothB.ttc 1.90

//Yu Gothic UI Bold *	YuGothB.ttc 1.90