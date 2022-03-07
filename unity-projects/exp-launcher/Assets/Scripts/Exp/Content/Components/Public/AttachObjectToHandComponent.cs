
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

namespace Ex{

    public class AttachObjectToHandComponent : ExComponent{

        //private GameObject m_spongeGO = null;
        //private LeapMotionArmsDisplayComponent m_leapMotion = null;

        #region ex_functions
        protected override bool initialize() {

            //// generate sponge
            //m_spongeGO = ExVR.GlobalResources().instantiate_prebab("Props/Sponge", transform);

            //// get leapmotion
            //string nameC = initC.get<string>("leap_motion_component");
            //if (nameC.Length > 0) {

            //    var component = get(nameC);
            //    if (component == null) {
            //        ExVR.Log().error("No component found with name " + nameC);
            //        return false;
            //    }
            //    m_leapMotion = component as LeapMotionArmsDisplayComponent;
            //    if (m_leapMotion == null) {
            //        ExVR.Log().error("No leap motion found");
            //        return false;
            //    }
            //}

            return true;
        }

        public override void update_from_current_config() {
            //if (!currentC.get<bool>("transform_do_not_apply")) {
                //currentC.update_transform("transform", m_spongeGO.transform);
            //}
        }

        protected override void set_visibility(bool visibility) {
            //m_spongeGO.SetActive(visibility );
        }

        protected override void update() {

            //if(m_leapMotion == null) {
            //    return;
            //}

            //var fingers = currentC.get<string>("arm") == "Left" ?  m_leapMotion.left_start_fingers() : m_leapMotion.right_start_fingers();
            //GameObject fingerToAttach = null;
            //foreach (var finger in fingers) {
            //    if (finger.name.Contains(currentC.get<string>("finger"))) {
            //        fingerToAttach = finger;
            //        break;
            //    }
            //}
            //if (fingerToAttach != null) {
            //    transform.localPosition = fingerToAttach.transform.position;
            //    transform.localRotation = fingerToAttach.transform.rotation;
            //}
        }

        #endregion
    }
}