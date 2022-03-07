
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

    public class Builder : MonoBehaviour{

        bool cloudInit = false;
        //public Vector4 obbSize = new Vector4(1, 1, 1, 1);
        //public Vector4 obbPos = new Vector4(1, 1, 1, 1);
        //public Matrix4x4 obbOrientation = Matrix4x4.identity;


        public static Builder autoRef = null;
        public GoExplorer GO() { return go; }
        private GoExplorer go = null;

        [Header("Managers", order = 0)]
        [Rename("GUI settings")]
        public GuiSettingsManager guiSettingsManager = null;
        [Rename("Paths")]
        public PathsManager pathsManager = null;
        [Rename("VR")]
        public DisplayManager displayManager = null;
        [Rename("Coroutines")]
        public CoroutinesManager coroutinesManager = null;
        [Rename("Network")]
        public NetworkManager networkManager = null;
        [Rename("Events")]
        public EventsManager eventsManager = null;
        [Rename("Python")]
        public PythonManager pythonManager = null;
        [Rename("Logger")]
        public LoggerManager loggerManager = null;
        [Rename("Time")]
        public TimeManager timeManager = null;
        [Rename("Memory")]
        public MemoryManager memoryManager = null;
        [Rename("Debug")]
        public DebugManager debugManager = null;

        [Rename("Global resources")]
        public GlobalResourcesManager globalResourcesManager = null;

        [Header("Experiment content")]
        [Rename("Experiment")]
        public Experiment experiment = null;

        public void Awake() {

#if EXVR
            Debug.Log("[EXVR-EXP]");
#else
            Debug.LogError("EXVR symbol not present.");
#endif
#if CLOSED_COMPONENTS
            Debug.Log("Closed components included.");
#endif
#if LNCO_COMPONENTS
            Debug.Log("LNCO components included.");
#endif
            autoRef = this; // for static access
            go      = GetComponent<GoExplorer>();

            // init managers
            // # time
            timeManager.start_program();
            // # setting            
            guiSettingsManager.initialize();
            // # paths
            pathsManager.initialize();
            // # events
            eventsManager.initialize();
            // # logger
            loggerManager.initialize();
            // # IPC
            ExVR.ExpLog().builder("Start IPC communication");
            networkManager.initialize();                        
            // # memory
            memoryManager.initialize();
            // # debug
            debugManager.initialize();

            // # global resources
            ExVR.ExpLog().builder("Initialize global resources");
            globalResourcesManager.initialize();
            // # graphics
            ExVR.ExpLog().builder("Initialize VR manager");
            displayManager.initialize();

            // # scripting            
            pythonManager.initialize();

            // config
            ConfigUtility.initialize();

            // init experiment
            ExVR.ExpLog().builder("Initialize experiment generator.");
            experiment.initialize();

            // indicates to the GUI that unity is ready
            ExVR.ExpLog().builder("Send ready signal to GUI.");
            networkManager.set_launcher_idle_state();


            //// tests
            //var pc = gameObject.AddComponent<PointCloud>();
            //pc.set_rendering(PointCloud.RenderingType.ParabloidFrag);

            //Mesh mesh = gameObject.GetComponent<MeshFilter>().mesh;
            //mesh.MarkDynamic();


        }
        //void Update() {

        //    if (!cloudInit) {
        //        Mesh mesh = gameObject.GetComponent<MeshFilter>().mesh;
        //    mesh.Clear();
        //    mesh.indexFormat = UnityEngine.Rendering.IndexFormat.UInt32;

        //    int sizePts = 100000;
        //    Vector3[] vertices = new Vector3[sizePts];
        //    Color[] colors = new Color[sizePts];
        //    System.Collections.Generic.List<int> commonIndices = new System.Collections.Generic.List<int>(sizePts);
        //    for (int ii = 0; ii < sizePts; ++ii) {
        //        var x = UnityEngine.Random.Range(-0.5f, 0.5f);
        //        var y = UnityEngine.Random.Range(-0.5f, 0.5f);
        //        var z = UnityEngine.Random.Range(-0.5f, 0.5f);
        //        vertices[ii] = new Vector3(x, y, z);

        //        var r = UnityEngine.Random.Range(0f, 1f);
        //        var g = UnityEngine.Random.Range(0f, 1f);
        //        var b = UnityEngine.Random.Range(0f, 1f);
        //        colors[ii] = new Color(r, g, b);

        //        commonIndices.Add(ii);
        //    }
        //    mesh.SetVertices(vertices, 0, sizePts);
        //    mesh.SetColors(colors, 0, sizePts);
        //    mesh.SetIndices(commonIndices, 0, sizePts, MeshTopology.Points, 0, true);

        //    cloudInit = true;
        //    }

        //    var pc = gameObject.GetComponent<PointCloud>();
        //    pc.set_filtering_obb_transform(transform.Find("Cube"));
        //}

        void OnApplicationQuit() {

            ExVR.Log().message(string.Format("Application ending after {0} seconds -> {1}", Time.time, GlobalVariables.wantToLeave));

            // force destroy experiment (mostly for C++DLL component to be clean correctly in Editor mode)
            ExVR.ExpLog().builder("Quit application");
            experiment.clean_experiment();
                   
            ExVR.ExpLog().builder("Clean resources");
            ExVR.Resources().clean();

            ExVR.ExpLog().builder("Clean network communication");
            networkManager.clean();
            ExVR.ExpLog().write();
        }
    }
}


//Debug.LogError("UnityEngine.XR.XRSettings.loadedDeviceName " + UnityEngine.XR.XRSettings.loadedDeviceName);
//Debug.LogError("Screen.currentResolution " + Screen.currentResolution);
//Debug.LogError("Screen.fullScreen " + Screen.fullScreen);

//System.AppDomain.CurrentDomain.FirstChanceException += (sender, eventArgs) => {
//    Debug.LogError("[FirstChance]" + eventArgs.Exception.ToString());
//};
// UnityEngine.Debug.Log("QualitySettings.antiAliasing " + QualitySettings.antiAliasing);
// UnityEngine.Debug.Log("UnityEngine.XR.XRSettings.eyeTextureResolutionScale " + UnityEngine.XR.XRSettings.eyeTextureResolutionScale);



//static bool WantsToQuit() {
//    ExVR.Log().message("WantsToQuit");
//    Debug.Log("Player prevented from quitting.");
//    return GlobalVariables.wantToLeave;
//}

//void OnApplicationFocus() {
//    Debug.Log("FOCUS");
//}
//void OnDestroy() {
//    Debug.Log("DESTROY");
//    //ExVR.Log().message("DESTROY");
//}
//            if (!GlobalVariables.wantToLeave) {
//#if UNITY_EDITOR
//                UnityEditor.EditorApplication.isPlaying = true;
//#endif
//                return;
//            }

//private void OnDestroy(){
//    //UnityEngine.Debug.Break();
//    string str = UnityEngine.StackTraceUtility.ExtractStackTrace();
//    ExVR.Log().message("OnDestroy. " + str);
//    //experiment.destroy_experiment();
//}
//Application.wantsToQuit += () => {
//    ExVR.Log().message("Application wantsToQuit -> " + GlobalVariables.wantToLeave);                
//    return true;
//};
//Application.quitting += () => {
//    ExVR.Log().message("Application quiting");
//};

// companyName
// consoleLogPath
// dataPath
// isFocused
// isPlaying
// persistentDataPath
// systemLanguage
// streamingAssetsPath
// unityVersion
// version

