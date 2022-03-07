
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
#if UNITY_EDITOR
using UnityEditor;
#endif

namespace Ex{


    public class TubeGenerator{
        public string goName = "Tube";
        public int nbSides = 24;
        public float height = 1f;
        public float bottomRadius = 0.5f;
        public float bottomThickness = 0.15f;
        public float topRadius = 0.5f;
        public float topRadiusThickness = 0.15f;

        public void generate() {
            GameObject tube = new GameObject(goName);
            var mf = tube.AddComponent<MeshFilter>();
            var mr = tube.AddComponent<MeshRenderer>();
            mr.sharedMaterial = Resources.Load(string.Format("Materials/Procedural/Tube")) as Material;
            mf.mesh = PrimitivesMesh.TubeBuilder.generate(nbSides, height, bottomRadius, bottomThickness, topRadius, topRadiusThickness);
        }
    }

    public class GridGenerator{

        public int nbVerticesH = 10;
        public int nbVerticesV = 10;
        public float width = 1f;
        public float height = 0.6f;
        public float depth = 0.001f;
        public bool isFlag = false;
        public string goName = "Grid";

        public void generate() {

            GameObject grid = new GameObject(goName);
            var mf = grid.AddComponent<MeshFilter>();
            var smr = grid.AddComponent<SkinnedMeshRenderer>();
            smr.sharedMaterial = Resources.Load(string.Format("Materials/Procedural/Grid")) as Material;
            var mesh = PrimitivesMesh.GridBuilder.generate(nbVerticesH, nbVerticesV, width, height, depth);
            mf.mesh = mesh;
            smr.sharedMesh = mesh;

            if (isFlag) {

                var cloth = grid.AddComponent<Cloth>();
                var coeff = cloth.coefficients;

                int idV = 0;
                for (int ii = 0; ii < nbVerticesH; ii++) {
                    for (int jj = 0; jj < nbVerticesV; jj++, idV++) {
                        if (idV < nbVerticesH) {
                            coeff[idV].maxDistance = (1.0f * idV) / nbVerticesH;
                        } else {
                            coeff[idV].maxDistance = 0.0f;
                        }
                    }
                }

                for (int ii = 0; ii < nbVerticesH; ii++) {
                    for (int jj = 0; jj < nbVerticesV; jj++, idV++) {
                        coeff[idV].maxDistance = 0.0f;
                    }
                }

                cloth.coefficients = coeff;
            }
        }
    }

#if UNITY_EDITOR
    public class EditorGeometryGeneratorMenu : EditorWindow{

        bool displayGrid = false;
        GridGenerator grid = new GridGenerator();

        bool displayTube = false;
        TubeGenerator tube = new TubeGenerator();

        bool displayMesh = false;
        string meshAssetBasePath = "Assets/Resources/Meshes/Procedural";
        string meshName = "MeshName.asset";

        bool displayMaterial = false;
        string materialToInstanciate = "Materials/DefaultModel";
        string materialAssetBasePath = "Assets/Resources/Materials/Procedural";
        string materialName = "MaterialName.mat";

        [MenuItem("Geometry/Generator")]
        public static void ShowWindow() {
            //Show existing window instance. If one doesn't exist, make one.
            GetWindow(typeof(EditorGeometryGeneratorMenu));
        }

        void OnGUI() {
            // TODO
            //GUILayout.BeginHorizontal();
            //{   //Create a row of buttons
            //    for (int i = 0; i < options.Length; ++i) {
            //        GUI.backgroundColor = i == selected ? highlightCol : bgCol;
            //        if (GUILayout.Button(options[i], buttonStyle)) {
            //            selected = i; //Tab click
            //        }
            //    }
            //}
            //GUILayout.EndHorizontal();


            GUILayout.BeginVertical();
            GUILayout.Label("Shapes", EditorStyles.boldLabel);
            displayTube = GUILayout.Toggle(displayTube, "Tube");
            if (displayTube) {
                GUILayout.Label("Tube settings", EditorStyles.boldLabel);
                tube.nbSides = EditorGUILayout.IntField("Nb sides", tube.nbSides);
                tube.height = EditorGUILayout.FloatField("Height", tube.height);
                tube.bottomRadius = EditorGUILayout.FloatField("bottomRadius", tube.bottomRadius);
                tube.bottomThickness = EditorGUILayout.FloatField("bottomThickness", tube.bottomThickness);
                tube.topRadius = EditorGUILayout.FloatField("topRadius", tube.topRadius);
                tube.topRadiusThickness = EditorGUILayout.FloatField("topRadiusThickness", tube.topRadiusThickness);
                tube.goName = EditorGUILayout.TextField("GO name", tube.goName);

                if (GUILayout.Button("Generate")) {
                    tube.generate();
                }
            }
            displayGrid = GUILayout.Toggle(displayGrid, "Grid");
            if (displayGrid) {
                GUILayout.Label("Grid settings", EditorStyles.boldLabel);
                grid.nbVerticesH = EditorGUILayout.IntField("Nb vertices H", grid.nbVerticesH);
                grid.nbVerticesV = EditorGUILayout.IntField("Nb vertices V", grid.nbVerticesV);
                grid.width = EditorGUILayout.FloatField("Width", grid.width);
                grid.height = EditorGUILayout.FloatField("Height", grid.height);
                grid.depth = EditorGUILayout.FloatField("Depth", grid.depth);
                grid.goName = EditorGUILayout.TextField("GO name", grid.goName);

                if (GUILayout.Button("Generate")) {
                    grid.generate();
                }
            }

            GUILayout.Space(10);
            GUILayout.Label("Assets", EditorStyles.boldLabel);

            displayMesh = GUILayout.Toggle(displayMesh, "Generate mesh asset from selection");
            if (displayMesh) {
                GUILayout.Label("Settings", EditorStyles.boldLabel);
                meshAssetBasePath = EditorGUILayout.TextField("Path to save", meshAssetBasePath);
                meshName = EditorGUILayout.TextField("Mesh name", meshName);

                if (GUILayout.Button("Generate")) {
                    var tr = Selection.activeTransform;
                    if (tr == null) {
                        Debug.LogError("No GameObject selected.");
                        return;
                    }

                    var mf = tr.gameObject.GetComponent<MeshFilter>();
                    if (mf != null) {
                        AssetDatabase.CreateAsset(mf.sharedMesh, meshAssetBasePath + "/" + meshName);
                        AssetDatabase.SaveAssets();
                        AssetDatabase.Refresh();
                    } else {
                        Debug.LogError("No MeshFilter available inside GameObject.");
                    }
                }
            }

            displayMaterial = GUILayout.Toggle(displayMaterial, "Add generated material to selection");
            if (displayMaterial) {
                GUILayout.Label("Settings", EditorStyles.boldLabel);
                materialToInstanciate = EditorGUILayout.TextField("Material to instanciate", materialToInstanciate);
                materialAssetBasePath = EditorGUILayout.TextField("Path to save", materialAssetBasePath);
                materialName = EditorGUILayout.TextField("Material name", materialName);

                if (GUILayout.Button("Generate")) {

                    var tr = Selection.activeTransform;
                    if (tr == null) {
                        Debug.LogError("No GameObject selected.");
                        return;
                    }

                    var mr = tr.gameObject.GetComponent<MeshRenderer>();
                    if (mr != null) {
                        Material mat = Resources.Load(materialToInstanciate) as Material;  
                        if(mat == null){
                            Debug.LogError("Invalid material to instanciate.");
                            return;
                        }
                        var nMat = Instantiate(mat);
                        AssetDatabase.CreateAsset(nMat, materialAssetBasePath + "/" + materialName);
                        AssetDatabase.SaveAssets();
                        AssetDatabase.Refresh();

                        mr.material = nMat;
                    } else {
                        Debug.LogError("No MeshRenderer available inside GameObject.");
                    }
                }
            }
     

            GUILayout.EndVertical();
        }
    }
#endif
}

