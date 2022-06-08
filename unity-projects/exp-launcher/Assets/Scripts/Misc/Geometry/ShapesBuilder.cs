
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

namespace Ex.PrimitivesMesh{

    class GridBuilder{

        public static Mesh generate(int nbVerticesH, int nbVerticesV, float width, float height, float depth) {

            if (nbVerticesH <= 1 || nbVerticesV <= 1) {
                Debug.LogError("Invalid parameters.");
                return null;
            }

            int nbVerticesFace  = nbVerticesH * nbVerticesV;
            int nbVerticesTotal = nbVerticesFace * 2;
            Vector3[] vertices = new Vector3[nbVerticesTotal];
            float dx = width  / nbVerticesH;
            float dy = height / nbVerticesV;
            float dz = depth  / 2f;

            int idV = 0;
            for (int ii = 0; ii < nbVerticesH; ii++) {
                for (int jj = 0; jj < nbVerticesV; jj++, idV++) {
                    vertices[idV] = new Vector3(dx * ii, dy * jj, -dz);
                }
            }
            for (int ii = 0; ii < nbVerticesH; ii++) {
                for (int jj = 0; jj < nbVerticesV; jj++, idV++) {
                    vertices[idV] = new Vector3(dx * ii, dy * jj, +dz);
                }
            }

            int nbTrianglesFace = (nbVerticesH - 1) * (nbVerticesV - 1) * 2;
            int nbTriangles = nbTrianglesFace * 2 + (nbVerticesV - 1)*4 + (nbVerticesH - 1) * 4;
            int[] triangles = new int[nbTriangles * 6];

            int idT = 0;
            for (int ii = 0; ii < nbVerticesH - 1; ii++) {
                for (int jj = 0; jj < nbVerticesV - 1; jj++, idT += 6) {

                    int idTopLeft      = ii * nbVerticesV + jj;
                    int idTopRight     = (ii) * nbVerticesV + (jj + 1);
                    int idBottomLeft   = (ii + 1) * nbVerticesV + (jj);
                    int idBottomRight  = (ii + 1) * nbVerticesV + (jj + 1);

                    triangles[idT]     = idTopLeft;
                    triangles[idT+1]   = idTopRight;
                    triangles[idT+2]   = idBottomLeft;

                    triangles[idT + 3] = idTopRight;
                    triangles[idT + 4] = idBottomRight;
                    triangles[idT + 5] = idBottomLeft;
                }
            }


            for (int ii = 0; ii < nbVerticesH - 1; ii++) {
                for (int jj = 0; jj < nbVerticesV - 1; jj++, idT += 6) {

                    int idTopLeft = nbVerticesFace + ii * nbVerticesV + jj;
                    int idTopRight = nbVerticesFace + (ii) * nbVerticesV + (jj + 1);
                    int idBottomLeft = nbVerticesFace + (ii + 1) * nbVerticesV + (jj);
                    int idBottomRight = nbVerticesFace + (ii + 1) * nbVerticesV + (jj + 1);

                    triangles[idT]     = idTopLeft;
                    triangles[idT + 1] = idBottomLeft;
                    triangles[idT + 2] = idTopRight; 

                    triangles[idT + 3] = idTopRight;
                    triangles[idT + 4] = idBottomLeft;
                    triangles[idT + 5] = idBottomRight; 
                }
            }

            // w = 4
            // h = 3

            // F1
            // 0 3 6 9 
            // 1 4 7 10
            // 2 5 8 11
            // F2
            // 12 15 18 21 
            // 13 16 19 22
            // 14 17 20 23

            // 0------3
            // |\     |\             
            // | \    | \    
            // 4--\---7  \
            //  \  1------2
            //   \ |    \ |
            //    \|     \|
            //     5------6

            int id0 = 0;
            int id1 = (nbVerticesH - 1) * nbVerticesV;
            int id2 = nbVerticesTotal - nbVerticesV;
            int id3 = nbVerticesFace;

            int id4 = nbVerticesV - 1;
            int id5 = nbVerticesFace - 1;
            int id6 = nbVerticesTotal - 1;
            int id7 = nbVerticesFace + nbVerticesV - 1;

            for (int ii = 0; ii < nbVerticesH - 1; ++ii, idT += 6) {

                int idTopLeft = id0 + ii* nbVerticesV;
                int idTopRight = id3 + ii* nbVerticesV;
                int idBottomLeft = idTopLeft + nbVerticesV;
                int idBottomRight = idTopRight + nbVerticesV;

                triangles[idT] = idTopLeft;
                triangles[idT + 1] = idBottomLeft;
                triangles[idT + 2] = idTopRight;

                triangles[idT + 3] = idTopRight;
                triangles[idT + 4] = idBottomLeft;
                triangles[idT + 5] = idBottomRight;
            }

            for (int ii = 0; ii < nbVerticesH - 1; ++ii, idT += 6) {

                int idTopLeft = id4 + ii * nbVerticesV;
                int idTopRight = id7 + ii * nbVerticesV;
                int idBottomLeft = idTopLeft + nbVerticesV;
                int idBottomRight = idTopRight + nbVerticesV;

                triangles[idT] = idTopLeft;
                triangles[idT + 1] = idTopRight;
                triangles[idT + 2] = idBottomLeft;

                triangles[idT + 3] = idTopRight;
                triangles[idT + 4] = idBottomRight;
                triangles[idT + 5] = idBottomLeft;
            }

            for (int ii = 0; ii < nbVerticesV - 1; ++ii, idT += 6) {

                int idTopLeft     = id1 + ii;
                int idTopRight    = id2 + ii;                
                int idBottomLeft  = idTopLeft + 1;
                int idBottomRight = idTopRight + 1;

                triangles[idT]     = idTopLeft;
                triangles[idT + 1] = idBottomLeft;
                triangles[idT + 2] = idTopRight;

                triangles[idT + 3] = idTopRight;
                triangles[idT + 4] = idBottomLeft;
                triangles[idT + 5] = idBottomRight;
            }

            for (int ii = 0; ii < nbVerticesV - 1; ++ii, idT += 6) {

                int idTopLeft     = id0 + ii;
                int idTopRight    = id3 + ii;
                int idBottomLeft  = idTopLeft + 1;
                int idBottomRight = idTopRight + 1;

                triangles[idT]     = idTopLeft;
                triangles[idT + 1] = idTopRight;
                triangles[idT + 2] = idBottomLeft;

                triangles[idT + 3] = idTopRight;
                triangles[idT + 4] = idBottomRight;
                triangles[idT + 5] = idBottomLeft;
            }




                //triangles[idT] = id0;
                //triangles[idT + 1] = id1;
                //triangles[idT + 2] = id2;

                //triangles[idT + 3] = id0;
                //triangles[idT + 4] = id2;
                //triangles[idT + 5] = id3;

                //triangles[idT + 6] = id4;
                //triangles[idT + 7] = id6;
                //triangles[idT + 8] = id5;

                //triangles[idT + 9] = id4;
                //triangles[idT + 10] = id7;
                //triangles[idT + 11] = id6;

                //triangles[idT + 12] = id1;
                //triangles[idT + 13] = id5;
                //triangles[idT + 14] = id6;

                //triangles[idT + 15] = id1;
                //triangles[idT + 16] = id6;
                //triangles[idT + 17] = id2;

                //triangles[idT + 18] = id3;
                //triangles[idT + 19] = id7;
                //triangles[idT + 20] = id4;

                //triangles[idT + 21] = id3;
                //triangles[idT + 22] = id4;
                //triangles[idT + 23] = id0;



                Vector3[] normals   = new Vector3[vertices.Length];
            Vector2[] uvs       = new Vector2[vertices.Length];
            Vector4[] tangents  = new Vector4[vertices.Length];
            Vector4 tangent     = new Vector4(1f, 0f, 0f, -1f);
            Vector3 normal      = new Vector3(0f, 0f, -1f);

            idV = 0;
            for (int ii = 0; ii < nbVerticesH; ii++) {
                for (int jj = 0; jj < nbVerticesV; jj++, idV++) {
                    uvs[idV]      = new Vector2(1f * ii / (nbVerticesH-1), 1f * jj / (nbVerticesV-1));
                    tangents[idV] = tangent;
                    normals[idV]  = normal;
                }
            }

            for (int ii = 0; ii < nbVerticesH; ii++) {
                for (int jj = 0; jj < nbVerticesV; jj++, idV++) {
                    uvs[idV] = new Vector2(1f * ii / (nbVerticesH - 1), 1f * jj / (nbVerticesV - 1));
                    tangents[idV] = -tangent;
                    normals[idV] = -normal;
                }
            }

            Mesh mesh = new Mesh();
            mesh.vertices = vertices;
            mesh.uv = uvs;
            mesh.normals = normals;
            mesh.tangents = tangents;
            mesh.triangles = triangles;
            mesh.Optimize();

            return mesh;
        }
    }

    class QuadBuilder{

        public static Mesh generate(float width, float height) {

            float halfW = width * 0.5f;
            float halfH = height * 0.5f;

            Vector3[] vertices = {
                new Vector3(-halfW, -halfH, 0),
                new Vector3(halfW, -halfH, 0),
                new Vector3(-halfW, halfH, 0),
                new Vector3(halfW, halfH, 0)
            };

            int[] triangles = {
                0, 2, 1, // lower left
			    2, 3, 1, // upper right
            };

            Vector2[] uvs = {
                new Vector2(0, 0),
                new Vector2(1, 0),
                new Vector2(0, 1),
                new Vector2(1, 1),
            };

            Mesh mesh = new Mesh();
            mesh.vertices = vertices;
            mesh.triangles = triangles;
            mesh.uv = uvs;
            mesh.RecalculateNormals();
            mesh.RecalculateTangents();
            mesh.Optimize();

            return mesh;
        }
    }

    
    class CubeBuilder{

        public static Mesh generate(float size = 1f) {

            float side2 = size / 2.0f;
            Vector3[] vertices = {
                // Front
                new Vector3(-side2, -side2, side2),
                new Vector3( side2, -side2, side2),
                new Vector3( side2,  side2, side2),
                new Vector3( -side2,  side2, side2),
                // Right
                new Vector3(side2, -side2, side2), 
                new Vector3(side2, -side2, -side2), 
                new Vector3(side2,  side2, -side2),
                new Vector3(side2,  side2, side2),
                // Back
                new Vector3(-side2, -side2, -side2), 
                new Vector3( -side2,  side2, -side2), 
                new Vector3( side2,  side2, -side2),
                new Vector3( side2, -side2, -side2),
                // Left
                new Vector3(-side2, -side2, side2), 
                new Vector3( -side2,  side2, side2), 
                new Vector3( -side2,  side2, -side2),
                new Vector3( -side2, -side2, -side2),
                // Bottom
                new Vector3(-side2, -side2, side2), 
                new Vector3( -side2, -side2, -side2), 
                new Vector3( side2, -side2, -side2),
                new Vector3( side2, -side2, side2),
                // Top
                new Vector3(-side2,  side2, side2), 
                new Vector3( side2,  side2, side2), 
                new Vector3( side2,  side2, -side2),
                new Vector3( -side2,  side2, -side2)
            };

            Vector3[] normals = {
                // Front
                new Vector3(0.0f, 0.0f, 1.0f),
                new Vector3(0.0f, 0.0f, 1.0f), 
                new Vector3(0.0f, 0.0f, 1.0f),
                new Vector3(0.0f, 0.0f, 1.0f),
                // Right
                new Vector3(1.0f, 0.0f, 0.0f), 
                new Vector3(1.0f, 0.0f, 0.0f), 
                new Vector3(1.0f, 0.0f, 0.0f),
                new Vector3(1.0f, 0.0f, 0.0f),
                // Back
                new Vector3(0.0f, 0.0f, -1.0f), 
                new Vector3(0.0f, 0.0f, -1.0f), 
                new Vector3(0.0f, 0.0f, -1.0f),
                new Vector3(0.0f, 0.0f, -1.0f),
                // Left
                new Vector3(-1.0f, 0.0f, 0.0f), 
                new Vector3(-1.0f, 0.0f, 0.0f), 
                new Vector3(-1.0f, 0.0f, 0.0f),
                new Vector3(-1.0f, 0.0f, 0.0f),
                // Bottom
                new Vector3(0.0f, -1.0f, 0.0f), 
                new Vector3(0.0f, -1.0f, 0.0f), 
                new Vector3(0.0f, -1.0f, 0.0f),
                new Vector3(0.0f, -1.0f, 0.0f),
                // Top
                new Vector3(0.0f, 1.0f, 0.0f), 
                new Vector3(0.0f, 1.0f, 0.0f),
                new Vector3(0.0f, 1.0f, 0.0f),
                new Vector3(0.0f, 1.0f, 0.0f)
            };

            Vector2[] tex = {
                // Front
                new Vector2(0.0f, 0.0f), 
                new Vector2(1.0f, 0.0f), 
                new Vector2(1.0f, 1.0f),
                new Vector2(0.0f, 1.0f),
                // Right
                new Vector2(0.0f, 0.0f), 
                new Vector2(1.0f, 0.0f), 
                new Vector2(1.0f, 1.0f),
                new Vector2(0.0f, 1.0f),
                // Back
                new Vector2(0.0f, 0.0f), 
                new Vector2(1.0f, 0.0f), 
                new Vector2(1.0f, 1.0f),
                new Vector2(0.0f, 1.0f),
                // Left
                new Vector2(0.0f, 0.0f), 
                new Vector2(1.0f, 0.0f), 
                new Vector2(1.0f, 1.0f),
                new Vector2(0.0f, 1.0f),
                // Bottom
                new Vector2(0.0f, 0.0f), 
                new Vector2(1.0f, 0.0f), 
                new Vector2(1.0f, 1.0f),
                new Vector2(0.0f, 1.0f),
                // Top
                new Vector2(0.0f, 0.0f), 
                new Vector2(1.0f, 0.0f), 
                new Vector2(1.0f, 1.0f),
                new Vector2(0.0f, 1.0f)
            };

            int[] triangles = {
                0,1,2,0,2,3,
                4,5,6,4,6,7,
                8,9,10,8,10,11,
                12,13,14,12,14,15,
                16,17,18,16,18,19,
                20,21,22,20,22,23
            };

            Mesh mesh = new Mesh();
            mesh.vertices   = vertices;
            mesh.normals    = normals;
            mesh.uv         = tex;            
            mesh.triangles  = triangles;

            mesh.RecalculateTangents();
            mesh.RecalculateBounds();
            mesh.Optimize();

            return mesh;
        }
    }

    class SphereBuilder{
        public static Mesh generate(float radius = 1f, int nbLong = 24, int nbLat = 16) {

            Mesh mesh = new Mesh();

            #region Vertices
            Vector3[] vertices = new Vector3[(nbLong + 1) * nbLat + 2];
            float pi = Mathf.PI;
            float piX2 = pi * 2f;

            vertices[0] = Vector3.up * radius;
            for (int lat = 0; lat < nbLat; lat++) {
                float a1 = pi * (float)(lat + 1) / (nbLat + 1);
                float sin1 = Mathf.Sin(a1);
                float cos1 = Mathf.Cos(a1);

                for (int lon = 0; lon <= nbLong; lon++) {
                    float a2 = piX2 * (float)(lon == nbLong ? 0 : lon) / nbLong;
                    float sin2 = Mathf.Sin(a2);
                    float cos2 = Mathf.Cos(a2);

                    vertices[lon + lat * (nbLong + 1) + 1] = new Vector3(sin1 * cos2, cos1, sin1 * sin2) * radius;
                }
            }
            vertices[vertices.Length - 1] = Vector3.up * -radius;
            #endregion

            #region Normales		
            Vector3[] normales = new Vector3[vertices.Length];
            for (int n = 0; n < vertices.Length; n++) {
                normales[n] = vertices[n].normalized;
            }
            #endregion

            #region UVs
            Vector2[] uvs = new Vector2[vertices.Length];
            uvs[0] = Vector2.up;
            uvs[uvs.Length - 1] = Vector2.zero;
            for (int lat = 0; lat < nbLat; lat++) {
                for (int lon = 0; lon <= nbLong; lon++) {
                    uvs[lon + lat * (nbLong + 1) + 1] = new Vector2((float)lon / nbLong, 1f - (float)(lat + 1) / (nbLat + 1));
                }
            }
            #endregion

            #region Triangles
            int nbFaces = vertices.Length;
            int nbTriangles = nbFaces * 2;
            int nbIndexes = nbTriangles * 3;
            int[] triangles = new int[nbIndexes];

            //Top Cap
            int i = 0;
            for (int lon = 0; lon < nbLong; lon++) {
                triangles[i++] = lon + 2;
                triangles[i++] = lon + 1;
                triangles[i++] = 0;
            }

            //Middle
            for (int lat = 0; lat < nbLat - 1; lat++) {
                for (int lon = 0; lon < nbLong; lon++) {
                    int current = lon + lat * (nbLong + 1) + 1;
                    int next = current + nbLong + 1;

                    triangles[i++] = current;
                    triangles[i++] = current + 1;
                    triangles[i++] = next + 1;

                    triangles[i++] = current;
                    triangles[i++] = next + 1;
                    triangles[i++] = next;
                }
            }

            //Bottom Cap
            for (int lon = 0; lon < nbLong; lon++) {
                triangles[i++] = vertices.Length - 1;
                triangles[i++] = vertices.Length - (lon + 2) - 1;
                triangles[i++] = vertices.Length - (lon + 1) - 1;
            }
            #endregion

            mesh.vertices = vertices;
            mesh.normals = normales;
            mesh.uv = uvs;
            mesh.triangles = triangles;

            mesh.RecalculateTangents();
            mesh.RecalculateBounds();
            mesh.Optimize();

            return mesh;
        }
    }

    class ConeBuilder{

        // https://wiki.unity3d.com/index.php/ProceduralPrimitives#C.23_-_Cone
        public static Mesh generate(int nbSides = 18, float bottomRadius = .25f, float topRadius = .05f, float height = 1f) {

            
            int nbHeightSeg = 1; // Not implemented yet

            int nbVerticesCap = nbSides + 1;
            #region Vertices

            // bottom + top + sides
            Vector3[] vertices = new Vector3[nbVerticesCap + nbVerticesCap + nbSides * nbHeightSeg * 2 + 2];
            int vert = 0;
            float _2pi = Mathf.PI * 2f;

            // Bottom cap
            vertices[vert++] = new Vector3(0f, 0f, 0f);
            while (vert <= nbSides) {
                float rad = (float)vert / nbSides * _2pi;
                vertices[vert] = new Vector3(Mathf.Cos(rad) * bottomRadius, 0f, Mathf.Sin(rad) * bottomRadius);
                vert++;
            }

            // Top cap
            vertices[vert++] = new Vector3(0f, height, 0f);
            while (vert <= nbSides * 2 + 1) {
                float rad = (float)(vert - nbSides - 1) / nbSides * _2pi;
                vertices[vert] = new Vector3(Mathf.Cos(rad) * topRadius, height, Mathf.Sin(rad) * topRadius);
                vert++;
            }

            // Sides
            int v = 0;
            while (vert <= vertices.Length - 4) {
                float rad = (float)v / nbSides * _2pi;
                vertices[vert] = new Vector3(Mathf.Cos(rad) * topRadius, height, Mathf.Sin(rad) * topRadius);
                vertices[vert + 1] = new Vector3(Mathf.Cos(rad) * bottomRadius, 0, Mathf.Sin(rad) * bottomRadius);
                vert += 2;
                v++;
            }
            vertices[vert] = vertices[nbSides * 2 + 2];
            vertices[vert + 1] = vertices[nbSides * 2 + 3];
            #endregion

            #region Normales

            // bottom + top + sides
            Vector3[] normales = new Vector3[vertices.Length];
            vert = 0;

            // Bottom cap
            while (vert <= nbSides) {
                normales[vert++] = Vector3.down;
            }

            // Top cap
            while (vert <= nbSides * 2 + 1) {
                normales[vert++] = Vector3.up;
            }

            // Sides
            v = 0;
            while (vert <= vertices.Length - 4) {
                float rad = (float)v / nbSides * _2pi;
                float cos = Mathf.Cos(rad);
                float sin = Mathf.Sin(rad);

                normales[vert] = new Vector3(cos, 0f, sin);
                normales[vert + 1] = normales[vert];

                vert += 2;
                v++;
            }
            normales[vert] = normales[nbSides * 2 + 2];
            normales[vert + 1] = normales[nbSides * 2 + 3];
            #endregion

            #region UVs
            Vector2[] uvs = new Vector2[vertices.Length];

            // Bottom cap
            int u = 0;
            uvs[u++] = new Vector2(0.5f, 0.5f);
            while (u <= nbSides) {
                float rad = (float)u / nbSides * _2pi;
                uvs[u] = new Vector2(Mathf.Cos(rad) * .5f + .5f, Mathf.Sin(rad) * .5f + .5f);
                u++;
            }

            // Top cap
            uvs[u++] = new Vector2(0.5f, 0.5f);
            while (u <= nbSides * 2 + 1) {
                float rad = (float)u / nbSides * _2pi;
                uvs[u] = new Vector2(Mathf.Cos(rad) * .5f + .5f, Mathf.Sin(rad) * .5f + .5f);
                u++;
            }

            // Sides
            int u_sides = 0;
            while (u <= uvs.Length - 4) {
                float t = (float)u_sides / nbSides;
                uvs[u] = new Vector3(t, 1f);
                uvs[u + 1] = new Vector3(t, 0f);
                u += 2;
                u_sides++;
            }
            uvs[u] = new Vector2(1f, 1f);
            uvs[u + 1] = new Vector2(1f, 0f);
            #endregion

            #region Triangles
            int nbTriangles = nbSides + nbSides + nbSides * 2;
            int[] triangles = new int[nbTriangles * 3 + 3];

            // Bottom cap
            int tri = 0;
            int i = 0;
            while (tri < nbSides - 1) {
                triangles[i] = 0;
                triangles[i + 1] = tri + 1;
                triangles[i + 2] = tri + 2;
                tri++;
                i += 3;
            }
            triangles[i] = 0;
            triangles[i + 1] = tri + 1;
            triangles[i + 2] = 1;
            tri++;
            i += 3;

            // Top cap
            //tri++;
            while (tri < nbSides * 2) {
                triangles[i] = tri + 2;
                triangles[i + 1] = tri + 1;
                triangles[i + 2] = nbVerticesCap;
                tri++;
                i += 3;
            }

            triangles[i] = nbVerticesCap + 1;
            triangles[i + 1] = tri + 1;
            triangles[i + 2] = nbVerticesCap;
            tri++;
            i += 3;
            tri++;

            // Sides
            while (tri <= nbTriangles) {
                triangles[i] = tri + 2;
                triangles[i + 1] = tri + 1;
                triangles[i + 2] = tri + 0;
                tri++;
                i += 3;

                triangles[i] = tri + 1;
                triangles[i + 1] = tri + 2;
                triangles[i + 2] = tri + 0;
                tri++;
                i += 3;
            }
            #endregion

            Mesh mesh = new Mesh();
            mesh.vertices = vertices;
            mesh.normals = normales;
            mesh.uv = uvs;
            mesh.triangles = triangles;

            mesh.RecalculateBounds();
            mesh.RecalculateTangents();
            mesh.Optimize();

            return mesh;
        }
    }

    class TubeBuilder{

        public static Mesh generate(int nbSides = 24, float height = 1f, 
            float bottomRadius = 0.5f, float bottomThickness = 0.15f, 
            float topRadius = 0.5f, float topThickness = 0.15f) { 

            // Outter shell is at radius1 + radius2 / 2, inner shell at radius1 - radius2 / 2            
            int nbVerticesCap   = nbSides * 2 + 2;
            int nbVerticesSides = nbSides * 2 + 2;
            #region Vertices

            // bottom + top + sides
            Vector3[] vertices = new Vector3[nbVerticesCap * 2 + nbVerticesSides * 2];
            int vert = 0;
            float _2pi = Mathf.PI * 2f;

            // Bottom cap
            int sideCounter = 0;
            while (vert < nbVerticesCap) {
                sideCounter = sideCounter == nbSides ? 0 : sideCounter;

                float r1 = (float)(sideCounter++) / nbSides * _2pi;
                float cos = Mathf.Cos(r1);
                float sin = Mathf.Sin(r1);
                vertices[vert] = new Vector3(cos * (bottomRadius - bottomThickness * .5f), 0f, sin * (bottomRadius - bottomThickness * .5f));
                vertices[vert + 1] = new Vector3(cos * (bottomRadius + bottomThickness * .5f), 0f, sin * (bottomRadius + bottomThickness * .5f));
                vert += 2;
            }

            // Top cap
            sideCounter = 0;
            while (vert < nbVerticesCap * 2) {
                sideCounter = sideCounter == nbSides ? 0 : sideCounter;

                float r1 = (float)(sideCounter++) / nbSides * _2pi;
                float cos = Mathf.Cos(r1);
                float sin = Mathf.Sin(r1);
                vertices[vert] = new Vector3(cos * (topRadius - topThickness * .5f), height, sin * (topRadius - topThickness * .5f));
                vertices[vert + 1] = new Vector3(cos * (topRadius + topThickness * .5f), height, sin * (topRadius + topThickness * .5f));
                vert += 2;
            }

            // Sides (out)
            sideCounter = 0;
            while (vert < nbVerticesCap * 2 + nbVerticesSides) {
                sideCounter = sideCounter == nbSides ? 0 : sideCounter;

                float r1 = (float)(sideCounter++) / nbSides * _2pi;
                float cos = Mathf.Cos(r1);
                float sin = Mathf.Sin(r1);

                vertices[vert] = new Vector3(cos * (topRadius + topThickness * .5f), height, sin * (topRadius + topThickness * .5f));
                vertices[vert + 1] = new Vector3(cos * (bottomRadius + bottomThickness * .5f), 0, sin * (bottomRadius + bottomThickness * .5f));
                vert += 2;
            }

            // Sides (in)
            sideCounter = 0;
            while (vert < vertices.Length) {
                sideCounter = sideCounter == nbSides ? 0 : sideCounter;

                float r1 = (float)(sideCounter++) / nbSides * _2pi;
                float cos = Mathf.Cos(r1);
                float sin = Mathf.Sin(r1);

                vertices[vert] = new Vector3(cos * (topRadius - topThickness * .5f), height, sin * (topRadius - topThickness * .5f));
                vertices[vert + 1] = new Vector3(cos * (bottomRadius - bottomThickness * .5f), 0, sin * (bottomRadius - bottomThickness * .5f));
                vert += 2;
            }
            #endregion

            #region Normales

            // bottom + top + sides
            Vector3[] normales = new Vector3[vertices.Length];
            vert = 0;

            // Bottom cap
            while (vert < nbVerticesCap) {
                normales[vert++] = Vector3.down;
            }

            // Top cap
            while (vert < nbVerticesCap * 2) {
                normales[vert++] = Vector3.up;
            }

            // Sides (out)
            sideCounter = 0;
            while (vert < nbVerticesCap * 2 + nbVerticesSides) {
                sideCounter = sideCounter == nbSides ? 0 : sideCounter;

                float r1 = (float)(sideCounter++) / nbSides * _2pi;

                normales[vert] = new Vector3(Mathf.Cos(r1), 0f, Mathf.Sin(r1));
                normales[vert + 1] = normales[vert];
                vert += 2;
            }

            // Sides (in)
            sideCounter = 0;
            while (vert < vertices.Length) {
                sideCounter = sideCounter == nbSides ? 0 : sideCounter;

                float r1 = (float)(sideCounter++) / nbSides * _2pi;

                normales[vert] = -(new Vector3(Mathf.Cos(r1), 0f, Mathf.Sin(r1)));
                normales[vert + 1] = normales[vert];
                vert += 2;
            }
            #endregion

            #region UVs
            Vector2[] uvs = new Vector2[vertices.Length];

            vert = 0;
            // Bottom cap
            sideCounter = 0;
            while (vert < nbVerticesCap) {
                float t = (float)(sideCounter++) / nbSides;
                uvs[vert++] = new Vector2(0f, t);
                uvs[vert++] = new Vector2(1f, t);
            }

            // Top cap
            sideCounter = 0;
            while (vert < nbVerticesCap * 2) {
                float t = (float)(sideCounter++) / nbSides;
                uvs[vert++] = new Vector2(0f, t);
                uvs[vert++] = new Vector2(1f, t);
            }

            // Sides (out)
            sideCounter = 0;
            while (vert < nbVerticesCap * 2 + nbVerticesSides) {
                float t = (float)(sideCounter++) / nbSides;
                uvs[vert++] = new Vector2(t, 0f);
                uvs[vert++] = new Vector2(t, 1f);
            }

            // Sides (in)
            sideCounter = 0;
            while (vert < vertices.Length) {
                float t = (float)(sideCounter++) / nbSides;
                uvs[vert++] = new Vector2(t, 0f);
                uvs[vert++] = new Vector2(t, 1f);
            }
            #endregion

            #region Triangles
            int nbFace = nbSides * 4;
            int nbTriangles = nbFace * 2;
            int nbIndexes = nbTriangles * 3;
            int[] triangles = new int[nbIndexes];

            // Bottom cap
            int i = 0;
            sideCounter = 0;
            while (sideCounter < nbSides) {
                int current = sideCounter * 2;
                int next = sideCounter * 2 + 2;

                triangles[i++] = next + 1;
                triangles[i++] = next;
                triangles[i++] = current;

                triangles[i++] = current + 1;
                triangles[i++] = next + 1;
                triangles[i++] = current;

                sideCounter++;
            }

            // Top cap
            while (sideCounter < nbSides * 2) {
                int current = sideCounter * 2 + 2;
                int next = sideCounter * 2 + 4;

                triangles[i++] = current;
                triangles[i++] = next;
                triangles[i++] = next + 1;

                triangles[i++] = current;
                triangles[i++] = next + 1;
                triangles[i++] = current + 1;

                sideCounter++;
            }

            // Sides (out)
            while (sideCounter < nbSides * 3) {
                int current = sideCounter * 2 + 4;
                int next = sideCounter * 2 + 6;

                triangles[i++] = current;
                triangles[i++] = next;
                triangles[i++] = next + 1;

                triangles[i++] = current;
                triangles[i++] = next + 1;
                triangles[i++] = current + 1;

                sideCounter++;
            }


            // Sides (in)
            while (sideCounter < nbSides * 4) {
                int current = sideCounter * 2 + 6;
                int next = sideCounter * 2 + 8;

                triangles[i++] = next + 1;
                triangles[i++] = next;
                triangles[i++] = current;

                triangles[i++] = current + 1;
                triangles[i++] = next + 1;
                triangles[i++] = current;

                sideCounter++;
            }
            #endregion

            Mesh mesh = new Mesh();
            mesh.vertices = vertices;
            mesh.normals = normales;
            mesh.uv = uvs;
            mesh.triangles = triangles;

            mesh.RecalculateTangents();
            mesh.RecalculateBounds();
            mesh.Optimize();

            return mesh;
        }
    }

    class CylinderBuilder{
        // Credits: https://github.com/doukasd/Unity-Components/blob/master/ProceduralCylinder/Assets/Scripts/Procedural/ProceduralCylinder.cs
        public static Mesh generate(int radialSegments = 8, int heightSegments = 2, float radius = 0.5f, float length = 1f) {

            Mesh mesh = new Mesh();
            mesh.name = "Procedural cylinder";

            // calculate how many vertices we need
            int numVertexColumns = radialSegments + 1;  //+1 for welding
            int numVertexRows = heightSegments + 1;

            // calculate sizes
            int numVertices = numVertexColumns * numVertexRows;
            int numUVs = numVertices;                                   //always
            int numSideTris = radialSegments * heightSegments * 2;      //for one cap
            int numCapTris = radialSegments - 2;                        //fact
            int trisArrayLength = (numSideTris + numCapTris * 2) * 3;   //3 places in the array for each tri

            // initialize arrays
            Vector3[] vertices = new Vector3[numVertices];
            Vector2[] uvs = new Vector2[numUVs];
            int[] triangles = new int[trisArrayLength];

            // precalculate increments to improve performance
            float heightStep = length / heightSegments;
            float angleStep = 2 * Mathf.PI / radialSegments;
            float uvStepH = 1.0f / radialSegments;
            float uvStepV = 1.0f / heightSegments;

            for (int ii = 0; ii < numVertexRows; ii++) {
                for (int jj = 0; jj < numVertexColumns; jj++) {

                    // calculate angle for that vertex on the unit circle
                    float angle = jj * angleStep;

                    // "fold" the sheet around as a cylinder by placing the first and last vertex of each row at the same spot
                    if (jj == numVertexColumns - 1) {
                        angle = 0;
                    }

                    // position current vertex
                    vertices[ii * numVertexColumns + jj] = new Vector3(radius * Mathf.Cos(angle), ii * heightStep, radius * Mathf.Sin(angle));

                    // calculate UVs
                    uvs[ii * numVertexColumns + jj] = new Vector2(jj * uvStepH, ii * uvStepV);

                    // create the tris				
                    if (ii == 0 || jj >= numVertexColumns - 1) {
                        //nothing to do on the first and last "floor" on the tris, capping is done below
                        //also nothing to do on the last column of vertices
                        continue;
                    } else {
                        // create 2 tris below each vertex
                        // 6 seems like a magic number. For every vertex we draw 2 tris in this for-loop, therefore we need 2*3=6 indices in the Tris array
                        // offset the base by the number of slots we need for the bottom cap tris. Those will be populated once we draw the cap
                        int baseIndex = numCapTris * 3 + (ii - 1) * radialSegments * 6 + jj * 6;

                        // 1st tri - below and in front
                        triangles[baseIndex + 0] = ii * numVertexColumns + jj;
                        triangles[baseIndex + 1] = ii * numVertexColumns + jj + 1;
                        triangles[baseIndex + 2] = (ii - 1) * numVertexColumns + jj;

                        // 2nd tri - the one it doesn't touch
                        triangles[baseIndex + 3] = (ii - 1) * numVertexColumns + jj;
                        triangles[baseIndex + 4] = ii * numVertexColumns + jj + 1;
                        triangles[baseIndex + 5] = (ii - 1) * numVertexColumns + jj + 1;
                    }
                }
            }

            //draw caps
            bool leftSided = true;
            int leftIndex = 0;
            int rightIndex = 0;
            int middleIndex = 0;
            int topCapVertexOffset = numVertices - numVertexColumns;
            for (int i = 0; i < numCapTris; i++) {
                int bottomCapBaseIndex = i * 3;
                int topCapBaseIndex = (numCapTris + numSideTris) * 3 + i * 3;

                if (i == 0) {
                    middleIndex = 0;
                    leftIndex = 1;
                    rightIndex = numVertexColumns - 2;
                    leftSided = true;
                } else if (leftSided) {
                    middleIndex = rightIndex;
                    rightIndex--;
                } else {
                    middleIndex = leftIndex;
                    leftIndex++;
                }
                leftSided = !leftSided;

                //assign bottom tris
                triangles[bottomCapBaseIndex + 0] = rightIndex;
                triangles[bottomCapBaseIndex + 1] = middleIndex;
                triangles[bottomCapBaseIndex + 2] = leftIndex;

                //assign top tris
                triangles[topCapBaseIndex + 0] = topCapVertexOffset + leftIndex;
                triangles[topCapBaseIndex + 1] = topCapVertexOffset + middleIndex;
                triangles[topCapBaseIndex + 2] = topCapVertexOffset + rightIndex;
            }

            //assign vertices, uvs and tris
            mesh.vertices = vertices;
            mesh.uv = uvs;
            mesh.triangles = triangles;

            mesh.RecalculateNormals();
            //mesh.
            mesh.RecalculateTangents();
            mesh.Optimize();

            return mesh;
        }
    }

    class TorusBuilder{

        public static Mesh generate(float outerRadius, float innerRadius, int nsides, int nrings) {

            int faces = nsides * nrings;
            int nVerts = nsides * (nrings + 1);   // One extra ring to duplicate first ring

            Vector3[] points = new Vector3[nVerts];
            Vector3[] normals = new Vector3[nVerts];
            Vector2[] textCoords = new Vector2[nVerts];
            int[] elements = new int[6*faces];


            // Generate the vertex data
            float twoPi = Mathf.PI * 2f;
            float ringFactor = twoPi / nrings;
            float sideFactor = twoPi / nsides;
            int idx = 0;
            for (int ring = 0; ring <= nrings; ring++) {
                float u = ring * ringFactor;
                float cu = Mathf.Cos(u);
                float su = Mathf.Sin(u);
                for (int side = 0; side < nsides; side++) {
                    float v = side * sideFactor;
                    float cv = Mathf.Cos(v);
                    float sv = Mathf.Sin(v);
                    float r = (outerRadius + innerRadius * cv);

                    points[idx] = new Vector3(r * cu, r * su, innerRadius * sv);
                    normals[idx] = new Vector3(cv * cu * r, cv * su * r, sv * r);
                    normals[idx].Normalize();

                    textCoords[idx] = new Vector2(u / twoPi, v / twoPi);       
                    idx++;
                }
            }

            idx = 0;
            for (int ring = 0; ring < nrings; ring++) {
                int ringStart = ring * nsides;
                int nextRingStart = (ring + 1) * nsides;
                for (int side = 0; side < nsides; side++) {
                    int nextSide = (side + 1) % nsides;
                    // The quad
                    elements[idx] = (ringStart + side);
                    elements[idx + 1] = (nextRingStart + side);
                    elements[idx + 2] = (nextRingStart + nextSide);
                    elements[idx + 3] = ringStart + side;
                    elements[idx + 4] = nextRingStart + nextSide;
                    elements[idx + 5] = (ringStart + nextSide);
                    idx += 6;
                }
            }

            Mesh mesh = new Mesh();
            mesh.vertices = points;
            mesh.normals = normals;
            mesh.uv = textCoords;
            mesh.triangles = elements;
            mesh.RecalculateTangents();

            return mesh;
        }
    }
}
