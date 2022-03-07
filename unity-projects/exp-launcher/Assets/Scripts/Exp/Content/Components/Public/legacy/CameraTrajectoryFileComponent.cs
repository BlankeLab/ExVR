
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
using Ex.XML;

// unity
using UnityEngine;

namespace Ex{

    public class CameraTrajectoryFileComponent : ExComponent{

        Dictionary<int, Trajectory> trajectories = null;

        Trajectory currentTraj = null;
        AnimationCurve speedCurve = null;

        private GameObject trajectoryGO = null;
        private GameObject linePosGO = null;
        private GameObject lineDirGO = null;
        private LineRenderer linePosRenderer = null;
        private LineRenderer lineDirRenderer = null;

        float lastForwardLineTimeS = 0f;

        protected override bool initialize() {

            // init trajectories from files
            trajectories = new Dictionary<int, Trajectory>();

            foreach (var config in configs) {

                var alias = config.get_resource_alias("trajectory_file");
                if (alias.Length == 0) {
                    log_error(string.Format("No trajectory defined for config {0}", config.name));
                    return false;
                }
                trajectories[config.key] = new Trajectory();                 
                if (!trajectories[config.key].init_from_text(ExVR.Resources().get_text_file_data(alias).content, config.get<bool>("inverse"))) {
                    return false;
                }
            }

            // init lines GO
            trajectoryGO = GO.generate_empty_scene_object("Trajectory lines", transform, true, Layers.Debug);
            linePosGO    = GO.generate_empty_scene_object("Line position", trajectoryGO.transform, true, Layers.Debug);
            // # line position
            linePosRenderer = linePosGO.AddComponent<LineRenderer>();
            linePosRenderer.material = new Material(Shader.Find("Sprites/Default"));
            linePosRenderer.positionCount = 0;
            linePosRenderer.widthMultiplier = 0.02f;
            linePosRenderer.useWorldSpace = true;
            // # line direction
            lineDirGO = GO.generate_empty_scene_object("Line direction", trajectoryGO.transform, true, Layers.Debug);
            lineDirRenderer = lineDirGO.AddComponent<LineRenderer>();
            lineDirRenderer.material = new Material(Shader.Find("Sprites/Default"));
            lineDirRenderer.positionCount = 0;
            lineDirRenderer.widthMultiplier = 0.02f;
            lineDirRenderer.useWorldSpace = true;

            return true;
        }
        protected override void clean() {

            // delete lines GO
            if (linePosGO != null) {                
                Destroy(linePosGO);
                Destroy(lineDirGO);
                Destroy(trajectoryGO);
            }
        }

        protected override void pre_start_routine() {

            // retrieve current trajectory
            currentTraj = trajectories[currentC.key];                        
      
            // reset lines
            linePosRenderer.positionCount = 0;
            linePosRenderer.SetPositions(new Vector3[] { });
            lineDirRenderer.positionCount = 0;
            lineDirRenderer.SetPositions(new Vector3[] { });

            trajectoryGO.SetActive(is_visible());
        }

        private void set_gradient(Color cStart, Color cEnd) {

            Gradient gradient = new Gradient();
            gradient.SetKeys(
                new GradientColorKey[] { new GradientColorKey(cStart, 0.0f), new GradientColorKey(cEnd, 1.0f) },
                new GradientAlphaKey[] { new GradientAlphaKey(1.0f, 0.0f), new GradientAlphaKey(1.0f, 1.0f) }
            );
            linePosRenderer.colorGradient = gradient;


            gradient.SetKeys(
                new GradientColorKey[] { new GradientColorKey(new Color(0.5f, 0.5f, 0f), 0.0f), new GradientColorKey(new Color(0.5f, 0.5f, 0f), 1.0f) },
                new GradientAlphaKey[] { new GradientAlphaKey(1.0f, 0.0f), new GradientAlphaKey(1.0f, 1.0f) }
            );
            lineDirRenderer.colorGradient = gradient;
        }

        protected override void update() {
            
            if(currentTraj == null) {
                return;
            }

            // compute time
            float currentTimeS = (float)(time().ellapsed_element_ms() * 0.001);
            bool addNewForwardLine = false;
            if (currentTimeS - lastForwardLineTimeS > 0.25) {
                lastForwardLineTimeS = currentTimeS;
                addNewForwardLine = true;
            }


            float duration = currentC.get<float>("duration");
            if(currentTimeS > duration) {
                return;
            }

            float speedFactor = speedCurve.Evaluate((float)(currentTimeS/ duration));
            float timeF = speedFactor * duration;

            // compute target
            bool sphericInterpolation = currentC.get<bool>("spherical_interpolation");
            var targetPosition = currentTraj.compute_position(timeF, duration, sphericInterpolation);
            var targetRotation = currentTraj.compute_rotation(timeF, duration, sphericInterpolation);
            log_message(string.Format("time: {0} duration {1} speedFactor {2}", timeF, duration, speedFactor));

            // move camera
            ExVR.Display().cameras().set_calibration_transform_by_modifying_camera_rig(
                targetPosition, targetRotation
            );

            // update lines 
            var currTr = ExVR.Display().cameras().get_calibration_transform();
            linePosRenderer.positionCount++;
            linePosRenderer.SetPosition(linePosRenderer.positionCount - 1, currTr.position);

            if (addNewForwardLine) {
                linePosRenderer.positionCount++;
                linePosRenderer.SetPosition(linePosRenderer.positionCount - 1, currTr.position + currTr.forward * 0.2f);

                linePosRenderer.positionCount++;
                linePosRenderer.SetPosition(linePosRenderer.positionCount - 1, currTr.position);
            }

            lineDirRenderer.positionCount++;
            lineDirRenderer.SetPosition(lineDirRenderer.positionCount-1, currTr.position + currTr.forward * 0.2f);
        }

        public override void update_from_current_config() {

            // update speed curve
            speedCurve = currentC.get_curve("speed");

            // set lines color
            set_gradient(currentC.get_color("start_color"), currentC.get_color("end_color"));
        }

        protected override void set_visibility(bool visible) {
            trajectoryGO.SetActive(visible);
        }
    }
}