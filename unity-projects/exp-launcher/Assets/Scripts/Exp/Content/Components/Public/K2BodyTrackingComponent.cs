

/*******************************************************************************
** exvr-exp                                                                   **
** No license (to be defined)                                                 **
** Copyright (c) [2018] [Florian Lance][EPFL-LNCO]                            **
********************************************************************************/

// system
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class K2BodyTrackingComponent : ExComponent{

        static public Mesh jointMesh = null;

        private K2ManagerComponent m_kinectM = null;
        private GameObject m_parentBodies = null;

        public List<K2BodiesGO> bodiesGO = null;

        #region ex_functions

        protected override bool initialize() {

            add_signal("body");

            // get kinect manager
            m_kinectM = initC.get_component<K2ManagerComponent>("kinect_manager");
            if (m_kinectM == null) {
                log_error("No kinect manager component set.");
                return false;
            }

            // init bodies
            jointMesh = Ex.PrimitivesMesh.CubeBuilder.generate(1f);
            m_parentBodies = GO.generate_empty_scene_object("grabbers_bodies", transform, true);
            bodiesGO = new List<K2BodiesGO>(m_kinectM.cameras_nb());
            for (int ii = 0; ii < m_kinectM.cameras_nb(); ++ii) {
                bodiesGO.Add(new K2BodiesGO(GO.generate_empty_scene_object("cam_" + ii, m_parentBodies.transform, true), jointMesh));
            }

            // apply calibration matrices
            for (int ii = 0; ii < m_kinectM.cameras_nb(); ++ii) {
                var calibM = m_kinectM.calibration_matrix(ii);
                bodiesGO[ii].parent.transform.localRotation = calibM.GetRotation();
                bodiesGO[ii].parent.transform.localPosition = calibM.GetPosition();
            }

            return true;
        }

        protected override void set_visibility(bool visibility) {
            m_parentBodies.SetActive(visibility);
        }

        public override void update_from_current_config() {

            if (!currentC.get<bool>("global_transform_do_not_apply")) {
                currentC.update_transform("global_transform", transform, false);
            }

            set_visibility(is_visible());
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }


        protected override void pre_update() {

            int mode = m_kinectM.mode();

            if (mode != 0 && mode != 1) {
                return;
            }

            for (int idCamera = 0; idCamera < m_kinectM.cameras_nb(); ++idCamera) {

                var data = m_kinectM.camera_data(idCamera);
                if (!data.update_bodies(bodiesGO[idCamera])) {
                    continue;
                }

                if (idCamera != 0) {
                    continue;
                }

                var kinectBodies = data.generate_bodies_data(bodiesGO[idCamera]);

                int maxJointsDetected = 0;
                K2BodyData currentBody = null;
                foreach (var body in kinectBodies.bodies) {

                    if (!body.Value.tracked) {
                        continue;
                    }

                    int trackedJointsCount = 0;
                    int inferredJointsCount = 0;
                    int notTrackedJointsCount = 0;
                    foreach (var joint in body.Value.joints) {
                        if(joint.Value.state == K2.TrackingState.tracked) {
                            ++trackedJointsCount;
                        }
                        if (joint.Value.state == K2.TrackingState.inferred) {
                            ++inferredJointsCount;
                        }
                        if (joint.Value.state == K2.TrackingState.not_tracked) {
                            ++notTrackedJointsCount;
                        }
                    }
                    if(maxJointsDetected < trackedJointsCount) {
                        maxJointsDetected = trackedJointsCount;
                        currentBody = body.Value;
                    }
                }

                //Debug.Log("J " + maxJointsDetected + " " + (currentBody!= null));
                invoke_signal("body", currentBody);
            }            
        }

        #endregion
    }
}
