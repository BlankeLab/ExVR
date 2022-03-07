
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

    public class ErasableTexture : MonoBehaviour{


        private Mesh m_mesh = null;
        private int m_widthColliderDef = 250;
        private int m_heightColliderDef = 250;

        private Texture2D m_alphaTexture = null;
        private List<Color> m_alphaColors = null;
        private List<bool> m_alphaMask = null;

        private Vector3 bottomLeft = new Vector3();
        private Vector3 topRight = new Vector3();
        private Vector3 bottomRight = new Vector3();
        private Vector3 topLeft = new Vector3();

        private Vector3 pp1 = new Vector3();
        private Vector3 pp2 = new Vector3();

        private Vector3 proj = new Vector3();
        private Vector3 hitP = new Vector3();
        //private Vector3 np = new Vector3();

        private Vector3 P1 = new Vector3();
        private Vector3 P2 = new Vector3();
        private Vector3 P3 = new Vector3();
        private Vector3 P4 = new Vector3();

        public void init(int width, int height) {

            // update settings
            m_widthColliderDef = width;
            m_heightColliderDef = height;

            // init gameobject
            gameObject.layer = Layers.Default;
            gameObject.name = "Mark to clean";
            gameObject.transform.localScale = new Vector3(1, 1, 1);
            gameObject.transform.localEulerAngles = new Vector3(0, 0, 0);
            gameObject.transform.localPosition = new Vector3(0, 0, 0);

            // generate quad mesh (TODO: add utility in shapes builder)
            var quadGO = GameObject.CreatePrimitive(PrimitiveType.Quad);
            m_mesh = quadGO.GetComponent<MeshFilter>().mesh;
            Destroy(quadGO);

            // add components
            var mf = gameObject.AddComponent<MeshFilter>();
            var mr = gameObject.AddComponent<MeshRenderer>();
            var mc = gameObject.AddComponent<MeshCollider>();
            mf.sharedMesh = m_mesh;
            mc.sharedMesh = m_mesh;
            mr.material = ExVR.GlobalResources().instantiate_mat("Components/MarkToClean/AlphaMark");

            // generate textures and colors
            m_alphaTexture = new Texture2D(m_widthColliderDef, m_heightColliderDef);
            m_alphaColors = new List<Color>(m_widthColliderDef * m_heightColliderDef);
            m_alphaMask = new List<bool>(m_widthColliderDef * m_heightColliderDef);
            Color initCol = new Color(1, 0, 0, 1);
            for (int ii = 0; ii < m_widthColliderDef * m_heightColliderDef; ++ii) {
                m_alphaColors.Add(initCol);
                m_alphaMask.Add(false);
            }

            // debug line renderer
            LineRenderer lineRenderer = gameObject.AddComponent<LineRenderer>();
            lineRenderer.material = new Material(Shader.Find("Sprites/Default"));
            lineRenderer.material.color = Color.blue;
            lineRenderer.widthMultiplier = 0.01f;
            lineRenderer.positionCount = 5;
        }

        /// <summary>
        /// Reset the mark and update its position
        /// </summary>
        public void reset_mark(Transform baseTransform, Vector3[] zoneEdges, float maxDistance, float distancePercentage) {

            // reset alpha texture
            Color initCol = new Color(0, 0, 0, 0);
            for (int ii = 0; ii < m_widthColliderDef * m_heightColliderDef; ++ii) {
                m_alphaColors[ii] = initCol;
                m_alphaMask[ii] = false;
            }
            m_alphaTexture.SetPixels(0, 0, m_widthColliderDef, m_heightColliderDef, m_alphaColors.ToArray());
            m_alphaTexture.Apply();

            // set alpha texture
            GetComponent<MeshRenderer>().material.SetTexture("_MaskTex", m_alphaTexture);

            // rays
            float maxRay = maxDistance;
            float ray = maxRay * distancePercentage * 0.01f;

            // mid points
            Vector3 midPoint1 = (zoneEdges[0] + zoneEdges[2]) * 0.5f;
            Vector3 midPoint2 = (zoneEdges[1] + zoneEdges[3]) * 0.5f;

            // vectors
            Vector3 leftV = (zoneEdges[0] - midPoint1).normalized;
            Vector3 rightV = (zoneEdges[2] - midPoint1).normalized;
            Vector3 vecMid = (midPoint2 - midPoint1).normalized;

            // max point
            Vector3 maxMidPoint = midPoint1 + vecMid * ray;

            // square
            Vector3 bottomLeft = midPoint1 + leftV * ray * 0.5f;
            Vector3 bottomRight = midPoint1 + rightV * ray * 0.5f;
            Vector3 topLeft = maxMidPoint + leftV * ray * 0.5f;
            Vector3 topRight = maxMidPoint + rightV * ray * 0.5f;

            float d1 = (bottomLeft - bottomRight).magnitude;
            float d2 = (topLeft - topRight).magnitude;

            if (d1 > d2) {
                bottomLeft = midPoint1 + leftV * d2 * 0.5f;
                bottomRight = midPoint1 + rightV * d2 * 0.5f;
            } else {
                maxMidPoint = midPoint1 + vecMid * d1;
                topLeft = maxMidPoint + leftV * d1 * 0.5f;
                topRight = maxMidPoint + rightV * d1 * 0.5f;
            }

            Vector3[] newVertices = new Vector3[4];
            newVertices[0] = bottomLeft;
            newVertices[1] = bottomRight;
            newVertices[2] = topRight;
            newVertices[3] = topLeft;
            m_mesh.vertices = newVertices;

            int[] newTriangles = new int[6] {
                0,1,2, 0,2,3
            };
            m_mesh.triangles = newTriangles;
            Vector2[] newUV = new Vector2[4] {
                new Vector2(0,0), new Vector2(1,0), new Vector2(1,1), new Vector2(0,1)
            };
            m_mesh.uv = newUV;
            m_mesh.uv2 = newUV;

            m_mesh.RecalculateNormals();

            GetComponent<MeshFilter>().sharedMesh = m_mesh;
            GetComponent<MeshCollider>().sharedMesh = m_mesh;
            transform.localScale = new Vector3(1, 1, 1);

            // update lines
            LineRenderer lineRenderer = GetComponent<LineRenderer>();
            lineRenderer.SetPosition(0, baseTransform.TransformPoint(bottomLeft));
            lineRenderer.SetPosition(1, baseTransform.TransformPoint(bottomRight));
            lineRenderer.SetPosition(2, baseTransform.TransformPoint(topRight));
            lineRenderer.SetPosition(3, baseTransform.TransformPoint(topLeft));
            lineRenderer.SetPosition(4, baseTransform.TransformPoint(bottomLeft));
        }


        /// <summary>
        /// Decrease alpha values of the mark parts close to the colliders
        /// </summary>
        public void apply_colliders(List<GameObject> colliders, float distance = 0.015f, float alphaDecrease = 0.01f) {

            if (colliders == null) {
                return;
            }
            if (colliders.Count == 0) {
                return;
            }

            Vector3 normal  = transform.TransformDirection(m_mesh.normals[0]);
            Vector3 point   = transform.TransformPoint(m_mesh.vertices[0]);
            Plane plane     = new Plane(normal, point);

            // retrieve positions
            bottomLeft  = transform.TransformPoint(m_mesh.vertices[0]);
            bottomRight = transform.TransformPoint(m_mesh.vertices[1]);
            topRight = transform.TransformPoint(m_mesh.vertices[2]);
            topLeft     = transform.TransformPoint(m_mesh.vertices[3]);


            var vH = bottomRight - bottomLeft;
            var vV = topLeft - bottomLeft;

            float squarMaxDist = (distance * distance);

            int nbHits = 0;
            foreach (GameObject collider in colliders) {
                if (collider == null) {
                    return;
                }

                // retrieve closest point on the mark plane from collider projection
                var cp = collider.transform.position;
                proj = plane.ClosestPointOnPlane(cp);

                if ((proj - cp).magnitude < 0.05f) { // check if not too far from plane

                    var dir = proj - cp;
                    RaycastHit hit;
                    bool validHit = Physics.Raycast(cp, dir, out hit, Mathf.Infinity);
                    hitP = hit.point;
                    if (validHit) {

                        if (!Maths.point_in_triangle(hit.point, bottomLeft, topLeft, topRight) && !Maths.point_in_triangle(hit.point, topRight, bottomRight, bottomLeft)) {
                            continue;
                        }

                        pp1 = bottomLeft + Vector3.Dot(hit.point - bottomLeft, vH) / Vector3.Dot(vH, vH) * (vH);
                        pp2 = bottomLeft + Vector3.Dot(hit.point - bottomLeft, vV) / Vector3.Dot(vV, vV) * (vV);

                        float x = (bottomLeft - pp1).magnitude / vH.magnitude;
                        float y = (bottomLeft - pp2).magnitude / vV.magnitude;

                        int ii = (int)((x) * m_widthColliderDef);
                        int jj = (int)((y) * m_heightColliderDef);
                        int id = jj * m_widthColliderDef + ii;

                        var lH = vH.magnitude / m_widthColliderDef;
                        var lV = vV.magnitude / m_heightColliderDef;

                        var v1 = (pp1 - hit.point).normalized;
                        var v2 = (pp2 - hit.point).normalized;

                        int diffX = m_widthColliderDef / 15;
                        int diffY = m_heightColliderDef / 15;

                        P1 = hit.point - v1 * diffX * lH;
                        P2 = hit.point + v1 * diffX * lH;

                        P3 = hit.point - v2 * diffY * lV;
                        P4 = hit.point + v2 * diffY * lV;

                        var v2_xLV = v2 * lV;
                        var jj_mDiffY = jj - diffY;

                        var diffX_x2 = 2 * diffX;
                        var diffY_x2 = 2 * diffY;

                        var v1_xLH = v1 * lH;

                        for (int kk = 0; kk < diffX_x2; ++kk) {

                            var kk_mDiffX = kk - diffX;
                            var h1 = v1_xLH * kk_mDiffX;
                            var ii_pKK_mDiffY = ii + kk_mDiffX;

                            for (int ll = 0; ll < diffY_x2; ++ll) {
                        
                                if ((v2_xLV * (ll - diffY) + h1).sqrMagnitude < squarMaxDist) { 

                                    id = (jj_mDiffY + ll) * m_widthColliderDef + ii_pKK_mDiffY;
                                    if (id >= 0 && id < m_alphaColors.Count) {

                                        Color c = m_alphaColors[id];
                                        if (c.a < 1) {
                                            c.a += alphaDecrease;
                                        }
                                        m_alphaColors[id] = c;
                                        ++nbHits;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // if new hits, update texture
            if (nbHits > 0){
                m_alphaTexture.SetPixels(0, 0, m_widthColliderDef, m_heightColliderDef, m_alphaColors.ToArray());
                m_alphaTexture.Apply();
            }
        }

        /// <summary>
        /// Set debug line renderer state
        /// </summary>
        public void set_debug(bool debug) {
            GetComponent<LineRenderer>().enabled = debug;
        }
    }

}