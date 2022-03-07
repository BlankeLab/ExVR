
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
using System.IO;

// unity
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;

////
//// Résumé :
////     Build assetBundle without any special option.
//None = 0,
////
//// Résumé :
////     Don't compress the data when creating the asset bundle.
//UncompressedAssetBundle = 1,
////
//// Résumé :
////     Includes all dependencies.
//CollectDependencies = 2,
////
//// Résumé :
////     Forces inclusion of the entire asset.
//CompleteAssets = 4,
////
//// Résumé :
////     Do not include type information within the AssetBundle.
//DisableWriteTypeTree = 8,
////
//// Résumé :
////     Builds an asset bundle using a hash for the id of the object stored in the asset
////     bundle.
//DeterministicAssetBundle = 16,
////
//// Résumé :
////     Force rebuild the assetBundles.
//ForceRebuildAssetBundle = 32,
////
//// Résumé :
////     Ignore the type tree changes when doing the incremental build check.
//IgnoreTypeTreeChanges = 64,
////
//// Résumé :
////     Append the hash to the assetBundle name.
//AppendHashToAssetBundleName = 128,
////
//// Résumé :
////     Use chunk-based LZ4 compression when creating the AssetBundle.
//ChunkBasedCompression = 256,
////
//// Résumé :
////     Do not allow the build to succeed if any errors are reporting during it.
//StrictMode = 512,
////
//// Résumé :
////     Do a dry run build.
//DryRunBuild = 1024,
////
//// Résumé :
////     Disables Asset Bundle LoadAsset by file name.
//DisableLoadAssetByFileName = 4096,
////
//// Résumé :
////     Disables Asset Bundle LoadAsset by file name with extension.
//DisableLoadAssetByFileNameWithExtension = 8192,
////
//// Résumé :
////     Removes the Unity Version number in the Archive File & Serialized File headers
////     during the build.
//AssetBundleStripUnityVersion = 32768


public class AssetBundleSettingsWindow : EditorWindow {
    
    public string path;
    string assetBundleName = "default";

    void OnGUI() {

        GUILayout.Label(string.Format("Prefab {0} asset bundle settings", path), EditorStyles.boldLabel);
        assetBundleName = EditorGUILayout.TextField("Assetbundle name", assetBundleName);

        if (GUILayout.Button("Apply")) {
            Debug.Log("APPLY: " + path + " " + assetBundleName);
            AssetImporter.GetAtPath(path).SetAssetBundleNameAndVariant(assetBundleName, "");
            Close();
        }

    }
}

public class AssetBundleBuilder : MonoBehaviour{

    private static readonly string assetBundleDirectory = "../assetsBundles";
    
    [MenuItem("AssetBundles/Choose bundle name for current selection")]
    static void ChangeBundleName() {
        AssetBundleSettingsWindow window = (AssetBundleSettingsWindow)EditorWindow.GetWindow(typeof(AssetBundleSettingsWindow));
        window.path = AssetDatabase.GetAssetPath(Selection.activeInstanceID);
        window.Show();
    }

    [MenuItem("AssetBundles/Remove unused asset bundles names")]
    static void RemoveUnusedAssetBundleNames() {
        AssetDatabase.RemoveUnusedAssetBundleNames();
    }


    [MenuItem("AssetBundles/Build AssetBundles")]
    static void BuildAllAssetBundles() {
        BuildAllAssetBundles(BuildAssetBundleOptions.None);
    }

    [MenuItem("AssetBundles/Force rebuild AssetBundles")]
    static void ForceRebuildAllAssetBundles() {
        BuildAllAssetBundles(BuildAssetBundleOptions.ForceRebuildAssetBundle);
    }

    [MenuItem("AssetBundles/Clean AssetBundles dir")]
    static void CleanAllAssetBundles() {

        Debug.Log("Start cleaning directory: " + assetBundleDirectory);
        string[] files = Directory.GetFiles(assetBundleDirectory);
        foreach (var file in files) {
            Debug.Log("Delete: " + file);
            File.Delete(file);
        }
    }

    static void BuildAllAssetBundles(BuildAssetBundleOptions options) {

        Debug.Log("Start generating assets bundles to: " + assetBundleDirectory);
        if (!Directory.Exists(assetBundleDirectory)) {
            Debug.Log("Directory \"" + assetBundleDirectory + "\" is created.");
            Directory.CreateDirectory(assetBundleDirectory);
        }

        
        Debug.Log("Asset bundle build list:");
        var manifest = BuildPipeline.BuildAssetBundles(assetBundleDirectory, options, EditorUserBuildSettings.activeBuildTarget);
        if (manifest != null) {
            foreach (var ab in manifest.GetAllAssetBundles()) {
                Debug.Log(ab);
            }
        } else {
            Debug.Log("Manifest is null.");
        }

        string[] files = Directory.GetFiles(assetBundleDirectory);
        foreach (var file in files) {

            if(file == "assetsbundles") {
                continue;
            }

            if (file.Contains(".manifest")) {   // do not touch manifest files
                continue;
            }

            if (file.Contains(".unity3d")) {    // do not touch other unity3d files
                continue;
            }

            // rename bundle file into bundle.unity3d
            string newName = file + ".unity3d";
            if (File.Exists(newName)) {
                File.Delete(newName);
            }
            File.Move(file, newName);
        }
        Debug.Log("End building asset bundles.");
    }
}
#endif