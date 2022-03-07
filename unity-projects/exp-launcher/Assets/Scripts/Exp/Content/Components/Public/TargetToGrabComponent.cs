
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

namespace Ex{


    public class TargetToGrabComponent : ExComponent{


        private GameObject m_workingZoneGO = null;
        private LineRenderer m_workingZoneLR = null;

        private GameObject m_targetLinesGO = null;
        private LineRenderer m_targetLinesLR = null;

        private GameObject m_startingLineGO = null;
        private LineRenderer m_startingLineLR = null;

        private GameObject m_originTargetLineGO = null;
        private LineRenderer m_originTargetLineLR = null;

        private GameObject m_targetPositionsLineGO = null;
        private LineRenderer m_targetPositionsLineLR = null;

        private float m_maxDistance = 0f;
        private float m_startLineDistance = 0f;

        protected override bool initialize() {

            // get target transformation
            if (components().get_from_name(initC.get<string>("shape_component")) == null) {
                log_error("Cannot find component " + initC.get<string>("shape_component") + ".");
                return false;
            }

            // working zone
            m_workingZoneGO = GO.generate_empty_scene_object("working zone", transform, true);
            var mf = m_workingZoneGO.AddComponent<MeshFilter>();

            Vector3[] vertices = {
                new Vector3(-0.5f, 0, 0),
                new Vector3(0.5f, 0, 0),
                new Vector3(0.5f, 0f, 1f),
                new Vector3(-0.5f, 0f, 1f)
            };

            mf.mesh.vertices = vertices;
            GO.generate_empty_scene_object("00 ", transform, true).transform.position = new Vector3(0f, 0, 0);
            GO.generate_empty_scene_object("working zone 1 ", transform, true).transform.position = vertices[0];
            GO.generate_empty_scene_object("working zone 2 ", transform, true).transform.position = vertices[1];
            GO.generate_empty_scene_object("working zone 3 ", transform, true).transform.position = vertices[2];
            GO.generate_empty_scene_object("working zone 4 ", transform, true).transform.position = vertices[3];

            m_workingZoneLR = m_workingZoneGO.AddComponent<LineRenderer>();
            m_workingZoneLR.material = new Material(Shader.Find("Sprites/Default"));
            m_workingZoneLR.material.color = Color.red;
            m_workingZoneLR.widthMultiplier = 0.003f;
            m_workingZoneLR.positionCount = 5;
            m_workingZoneLR.enabled = false;

            // target lines
            m_targetLinesGO = GO.generate_empty_scene_object("target lines", m_workingZoneGO.transform, true);
            m_targetLinesLR = m_targetLinesGO.AddComponent<LineRenderer>();
            m_targetLinesLR.material = new Material(Shader.Find("Sprites/Default"));
            m_targetLinesLR.material.color = Color.yellow;
            m_targetLinesLR.widthMultiplier = 0.003f;
            m_targetLinesLR.positionCount = 5;
            m_targetLinesLR.enabled = false;

            // starting line
            m_startingLineGO = GO.generate_empty_scene_object("starting line", m_workingZoneGO.transform, true);
            m_startingLineLR = m_startingLineGO.AddComponent<LineRenderer>();
            m_startingLineLR.material = new Material(Shader.Find("Sprites/Default"));
            m_startingLineLR.material.color = Color.red;
            m_startingLineLR.widthMultiplier = 0.003f;
            m_startingLineLR.positionCount = 2;
            m_startingLineLR.enabled = false;

            // origin target line
            m_originTargetLineGO = GO.generate_empty_scene_object("origin target line", m_workingZoneGO.transform, true);
            m_originTargetLineLR = m_originTargetLineGO.AddComponent<LineRenderer>();
            m_originTargetLineLR.material = new Material(Shader.Find("Sprites/Default"));
            m_originTargetLineLR.material.color = Color.green;
            m_originTargetLineLR.widthMultiplier = 0.003f;
            m_originTargetLineLR.positionCount = 2;
            m_originTargetLineLR.enabled = false;

            // target positions line
            m_targetPositionsLineGO = GO.generate_empty_scene_object("target positions line", m_workingZoneGO.transform, true);
            m_targetPositionsLineLR = m_targetPositionsLineGO.AddComponent<LineRenderer>();
            m_targetPositionsLineLR.material = new Material(Shader.Find("Sprites/Default"));
            m_targetPositionsLineLR.material.color = Color.blue;
            m_targetPositionsLineLR.widthMultiplier = 0.003f;
            m_targetPositionsLineLR.enabled = false;
            m_targetPositionsLineLR.useWorldSpace = false;

            // store distances
            m_maxDistance       = initC.get<float>("max_distance_reachable");
            m_startLineDistance = initC.get<float>("start_distance");

            return true;
        }


        protected override void set_visibility(bool visibility) {
            update_visibility();
        }

        public override void update_from_current_config() {

            // apply transformation
            if (!currentC.get<bool>("working_transform_do_not_apply")) {
                currentC.update_transform("working_transform", m_workingZoneGO.transform);
            }

            // reset working zone
            reset_working_transform();

            // target
            reset_target_transform();

            // update current visibility 
            update_visibility();
        }

        protected override void update_parameter_from_gui(string updatedArgName) {
            update_from_current_config();
        }

        public void update_visibility() {
            m_targetLinesLR.enabled  = is_visible() && currentC.get<bool>("debug");
            m_workingZoneLR.enabled  = is_visible() && currentC.get<bool>("debug");
            m_originTargetLineLR.enabled = is_visible() && currentC.get<bool>("debug");
            m_targetPositionsLineLR.enabled = is_visible() && currentC.get<bool>("debug");
            m_startingLineLR.enabled = is_visible() && currentC.get<bool>("display_start_line");            
        }

        private void reset_working_transform() {

            // get zone vertices
            var zoneMesh = m_workingZoneGO.GetComponent<MeshFilter>().sharedMesh;
            Vector3[] ze = new Vector3[4];
            ze[0] = m_workingZoneGO.transform.TransformPoint(zoneMesh.vertices[0]);
            ze[1] = m_workingZoneGO.transform.TransformPoint(zoneMesh.vertices[1]);
            ze[2] = m_workingZoneGO.transform.TransformPoint(zoneMesh.vertices[2]);
            ze[3] = m_workingZoneGO.transform.TransformPoint(zoneMesh.vertices[3]);

            // mid points
            var midPoint1 = (ze[0] + ze[1]) * 0.5f;
            var midPoint2 = (ze[2] + ze[3]) * 0.5f;

            // vectors
            Vector3 leftV = (ze[0] - midPoint1).normalized;
            Vector3 rightV = (ze[1] - midPoint1).normalized;
            Vector3 vecMid = (midPoint2 - midPoint1).normalized;

            // max point
            Vector3 maxMidPoint = midPoint1 + vecMid * m_maxDistance;

            // square
            float midDistance   = m_maxDistance * 0.5f;
            Vector3 bottomLeft  = midPoint1   + leftV   * midDistance;
            Vector3 bottomRight = midPoint1   + rightV  * midDistance;
            Vector3 topLeft     = maxMidPoint + leftV   * midDistance;
            Vector3 topRight    = maxMidPoint + rightV  * midDistance;

            // debug line
            m_workingZoneLR.SetPosition(0, bottomLeft);
            m_workingZoneLR.SetPosition(1, bottomRight);
            m_workingZoneLR.SetPosition(2, topRight);
            m_workingZoneLR.SetPosition(3, topLeft);
            m_workingZoneLR.SetPosition(4, bottomLeft);

            // start line
            m_startingLineLR.SetPosition(0, bottomLeft  + (topLeft - bottomLeft).normalized * m_startLineDistance);
            m_startingLineLR.SetPosition(1, bottomRight + (topRight - bottomRight).normalized * m_startLineDistance);
        }

        private void reset_target_transform() {

            // get zone vertices
            var zoneMesh = m_workingZoneGO.GetComponent<MeshFilter>().sharedMesh;
            Vector3[] ze = new Vector3[4];
            ze[0] = m_workingZoneGO.transform.TransformPoint(zoneMesh.vertices[0]);
            ze[1] = m_workingZoneGO.transform.TransformPoint(zoneMesh.vertices[1]);
            ze[2] = m_workingZoneGO.transform.TransformPoint(zoneMesh.vertices[2]);
            ze[3] = m_workingZoneGO.transform.TransformPoint(zoneMesh.vertices[3]);

            float distanceTarget = currentC.get<float>("distance_percentage_target");

            // rays
            float maxRay = m_maxDistance;
            float ray = maxRay * distanceTarget * 0.01f;

            // mid points
            var midPoint1 = (ze[0] + ze[1]) * 0.5f;
            var midPoint2 = (ze[2] + ze[3]) * 0.5f;

            // vectors
            Vector3 leftV = (ze[0] - midPoint1).normalized;
            Vector3 rightV = (ze[1] - midPoint1).normalized;
            Vector3 vecMid = (midPoint2 - midPoint1).normalized;

            // max point
            Vector3 maxMidPoint = midPoint1 + vecMid * ray;

            // square
            Vector3 bottomLeft = midPoint1 + leftV * ray * 0.5f;
            Vector3 bottomRight = midPoint1 + rightV * ray * 0.5f;
            Vector3 topLeft = maxMidPoint + leftV * ray * 0.5f;
            Vector3 topRight = maxMidPoint + rightV * ray * 0.5f;

            //maxMidPoint = midPoint1 + vecMid * maxRay;
            Vector3 ringMidPoint = midPoint1 + vecMid * ray;

            Vector3 position = new Vector3();
            float angle = currentC.get<float>("angle_target");
            if (angle < 0) {
                var tanAlpha = Mathf.Tan(-angle * Mathf.Deg2Rad);
                var leftD = tanAlpha * maxRay * distanceTarget * 0.01f;
                position = ringMidPoint + leftV * leftD;
            } else {
                var tanAlpha = Mathf.Tan(angle * Mathf.Deg2Rad);
                var rightD = tanAlpha * maxRay * distanceTarget * 0.01f;
                position = ringMidPoint + rightV * rightD;
            }

            var v = (position - midPoint1).normalized * m_maxDistance * distanceTarget * 0.01f;
            position = midPoint1 + v;

            var targetTr = components().get_from_name(initC.get<string>("shape_component")).transform;
            targetTr.localPosition = position + new Vector3(0, currentC.get<float>("height_target"), 0);

            // update lines
            m_targetLinesLR.SetPosition(0, bottomLeft);
            m_targetLinesLR.SetPosition(1, bottomRight);
            m_targetLinesLR.SetPosition(2, topRight);
            m_targetLinesLR.SetPosition(3, topLeft);
            m_targetLinesLR.SetPosition(4, bottomLeft);

            m_originTargetLineLR.SetPosition(0, m_workingZoneGO.transform.position);
            m_originTargetLineLR.SetPosition(1, targetTr.position);
            
            m_targetPositionsLineLR.positionCount = 361;
            //m_targetPositionsLineLR.useWorldSpace = false;
            var pointCount = 360 + 1; // add extra point to make startpoint and endpoint the same to close the circle
            var points = new Vector3[pointCount];

            for (int i = 0; i < pointCount; i++) {
                var rad = Mathf.Deg2Rad * (i * 360f / 360);
                points[i] = new Vector3(Mathf.Sin(rad) * ray, 0, Mathf.Cos(rad) * ray);
            }
            m_targetPositionsLineLR.SetPositions(points);

        }

        protected override void update() {
            reset_target_transform();            
        }

        public Vector3 origin_position() {
            return m_workingZoneGO.transform.position;
        }
        
        public Vector3 target_position() {
            return components().get_from_name(initC.get<string>("shape_component")).transform.position;
        }

    }

}