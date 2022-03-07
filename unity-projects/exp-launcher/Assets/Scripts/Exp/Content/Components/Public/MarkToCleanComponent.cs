
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
using UnityEngine.Profiling;

namespace Ex{

    public class MarkToCleanComponent : ExComponent{

        private GameObject m_fullZoneGO = null;
        private GameObject m_markToCleanGO = null;        

        private GameObject m_debugLineFullZoneGO = null;
        private LineRenderer m_debugFullZoneLR = null;

        private Vector3 bottomLeft = new Vector3();
        private Vector3 bottomRight = new Vector3();
        private Vector3 topLeft = new Vector3();
        private Vector3 topRight = new Vector3();

        private float m_maxDistance = 0f;

        private ErasableTexture m_markToClean = null;

        protected override bool initialize() {

            // slots
            add_slot("erase", (colliders) => {
                erase_mark((List<GameObject>)colliders);
            });
            add_slot("reset", (nullArg) =>    {
                reset_working_transform();
                reset_mark();
            });

            // debug line renderer
            m_debugLineFullZoneGO = new GameObject("Debug line full zone");
            m_debugLineFullZoneGO.transform.SetParent(transform);
            m_debugLineFullZoneGO.transform.localPosition = new Vector3(0, 0, 0);
            m_debugLineFullZoneGO.transform.localScale = new Vector3(1, 1, 1);
            m_debugLineFullZoneGO.transform.localEulerAngles = new Vector3(0, 0, 0);
            m_debugLineFullZoneGO.layer = Layers.Default;
            m_debugLineFullZoneGO.SetActive(false);
            m_debugFullZoneLR = m_debugLineFullZoneGO.AddComponent<LineRenderer>();
            m_debugFullZoneLR.material = new Material(Shader.Find("Sprites/Default"));
            m_debugFullZoneLR.material.color = Color.red;
            m_debugFullZoneLR.widthMultiplier = 0.01f;
            m_debugFullZoneLR.positionCount = 5;

            // generate zone
            m_fullZoneGO = ExVR.GlobalResources().instantiate_prebab("Components/MarkToClean/Zone", transform, "Mark full zone");
            m_fullZoneGO.GetComponent<MeshRenderer>().enabled = false;

            // generate mark
            m_markToCleanGO = new GameObject();
            m_markToCleanGO.transform.SetParent(m_fullZoneGO.transform);
            m_markToClean = m_markToCleanGO.AddComponent<ErasableTexture>();
            m_markToClean.init(initC.get<int>("width_collider_resolution"), initC.get<int>("height_collider_resolution"));
            m_markToCleanGO.SetActive(false);


            // store distance
            m_maxDistance = initC.get<float>("max_distance_reachable");

            return true;
        }        

        protected override void start_routine() {            
            reset_working_transform();
            reset_mark();            
        }

        protected override void set_visibility(bool visibility) {
            
            m_fullZoneGO.SetActive(visibility);
            m_markToCleanGO.SetActive(is_visible());
            reset_debug_state();
        }

        protected override void update_parameter_from_gui(string updatedArgName) {

            if (updatedArgName == "working_transform") {
                reset_working_transform();
                reset_mark();
            } else  if(updatedArgName == "distance_percentage_mark") {
                reset_mark();
            } else if(updatedArgName == "debug"){
                reset_debug_state();
            }
        }

        public void reset_working_transform() {

            // set transformation           
            if (!currentC.get<bool>("working_transform_do_not_apply")) {
                currentC.update_transform("working_transform", m_fullZoneGO.transform, true);
            }


            // get zone vertices
            var zoneMesh = m_fullZoneGO.GetComponent<MeshFilter>().sharedMesh;
            Vector3[] zoneEdges = new Vector3[4];
            zoneEdges[0] = m_fullZoneGO.transform.TransformPoint(zoneMesh.vertices[0]);
            zoneEdges[1] = m_fullZoneGO.transform.TransformPoint(zoneMesh.vertices[1]);
            zoneEdges[2] = m_fullZoneGO.transform.TransformPoint(zoneMesh.vertices[2]);
            zoneEdges[3] = m_fullZoneGO.transform.TransformPoint(zoneMesh.vertices[3]);

            // mid points
            var midPoint1 = (zoneEdges[0] + zoneEdges[2]) * 0.5f;
            var midPoint2 = (zoneEdges[1] + zoneEdges[3]) * 0.5f;

            // vectors
            Vector3 leftV = (zoneEdges[0] - midPoint1).normalized;
            Vector3 rightV = (zoneEdges[2] - midPoint1).normalized;
            var vecMid = (midPoint2 - midPoint1).normalized;

            // max point
            var maxMidPoint = midPoint1 + vecMid * m_maxDistance;

            // square
            bottomLeft = midPoint1 + leftV * m_maxDistance * 0.5f;
            bottomRight = midPoint1 + rightV * m_maxDistance * 0.5f;
            topLeft = maxMidPoint + leftV * m_maxDistance * 0.5f;
            topRight = maxMidPoint + rightV * m_maxDistance * 0.5f;

            // debug line
            m_debugFullZoneLR.SetPosition(0, bottomLeft);
            m_debugFullZoneLR.SetPosition(1, bottomRight);
            m_debugFullZoneLR.SetPosition(2, topRight);
            m_debugFullZoneLR.SetPosition(3, topLeft);
            m_debugFullZoneLR.SetPosition(4, bottomLeft);
        }

        public void reset_mark() {

            var zoneMesh = m_fullZoneGO.GetComponent<MeshFilter>().sharedMesh;
            Vector3[] zoneEdges = new Vector3[4];
            zoneEdges[0] = zoneMesh.vertices[0];
            zoneEdges[1] = zoneMesh.vertices[1];
            zoneEdges[2] = zoneMesh.vertices[2];
            zoneEdges[3] = zoneMesh.vertices[3];
            m_markToClean.reset_mark(m_fullZoneGO.transform, zoneEdges, m_maxDistance, currentC.get<float>("distance_percentage_mark"));
        }

        public void reset_debug_state() {

            // zone
            m_debugFullZoneLR.enabled = currentC.get<bool>("debug") && is_visible();
            // mark
            m_markToClean.set_debug(currentC.get<bool>("debug") && is_visible());
        }

        public void erase_mark(List<GameObject> colliders) {            

            Profiler.BeginSample("[ExVR][MarkToCleanComponent] erase_mark");
            m_markToClean.apply_colliders(colliders, currentC.get<float>("distance_erase"), currentC.get<float>("alpha_decrease"));
            Profiler.EndSample();
        }
    }
}

