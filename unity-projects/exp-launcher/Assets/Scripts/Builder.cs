
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


namespace Ex {

    public class Builder : MonoBehaviour{



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



            //Program.tests();

            //// tests
            //var pc = gameObject.AddComponent<PointCloud>();
            //pc.set_rendering(PointCloud.RenderingType.ParabloidFrag);

            //Mesh mesh = gameObject.GetComponent<MeshFilter>().mesh;
            //mesh.MarkDynamic();

            //Converter.unit_tests();
        }


        //bool cloudInit = false;

        //void Update() {

        //    if (!cloudInit) {
        //        Mesh mesh = gameObject.GetComponent<MeshFilter>().mesh;
        //        mesh.Clear();
        //        mesh.indexFormat = UnityEngine.Rendering.IndexFormat.UInt32;

        //        int sizePts = 100000;
        //        Vector3[] vertices = new Vector3[sizePts];
        //        Color[] colors = new Color[sizePts];
        //        System.Collections.Generic.List<int> commonIndices = new System.Collections.Generic.List<int>(sizePts);
        //        for (int ii = 0; ii < sizePts; ++ii) {
        //            var x = UnityEngine.Random.Range(-0.5f, 0.5f);
        //            var y = UnityEngine.Random.Range(-0.5f, 0.5f);
        //            var z = UnityEngine.Random.Range(-0.5f, 0.5f);
        //            vertices[ii] = new Vector3(x, y, z);

        //            var r = UnityEngine.Random.Range(0f, 1f);
        //            var g = UnityEngine.Random.Range(0f, 1f);
        //            var b = UnityEngine.Random.Range(0f, 1f);
        //            colors[ii] = new Color(r, g, b);

        //            commonIndices.Add(ii);
        //        }
        //        mesh.SetVertices(vertices, 0, sizePts);
        //        mesh.SetColors(colors, 0, sizePts);
        //        mesh.SetIndices(commonIndices, 0, sizePts, MeshTopology.Points, 0, true);

        //        cloudInit = true;
        //    }

        //    var pc = gameObject.GetComponent<PointCloud>();

        //    System.Collections.Generic.List<GameObject> cubes = new System.Collections.Generic.List<GameObject>();
        //    cubes.Add(transform.Find("Cube1").gameObject);
        //    cubes.Add(transform.Find("Cube2").gameObject);
        //    cubes.Add(transform.Find("Cube3").gameObject);

        //    System.Collections.Generic.List<OBBFInfo> infos = new System.Collections.Generic.List<OBBFInfo>();
        //    infos.Add(new OBBFInfo());
        //    infos.Add(new OBBFInfo());
        //    infos.Add(new OBBFInfo());

        //    infos[0].color = new Color(1, 0, 0, 0.2f);
        //    infos[1].color = new Color(0, 1, 0, 0.2f);
        //    infos[2].color = new Color(0, 0, 1, 0.2f);

        //    cubes[0].GetComponent<MeshRenderer>().material.SetColor("_Color", infos[0].color);
        //    cubes[1].GetComponent<MeshRenderer>().material.SetColor("_Color", infos[1].color);
        //    cubes[2].GetComponent<MeshRenderer>().material.SetColor("_Color", infos[2].color);

        //    infos[0].display = true;
        //    infos[1].display = true;
        //    infos[2].display = true;

        //    infos[0].enabled = true;
        //    infos[1].enabled = true;
        //    infos[2].enabled = true;

        //    infos[0].transform = Converter.to_transform_value(cubes[0].transform);
        //    infos[1].transform = Converter.to_transform_value(cubes[2].transform);
        //    infos[2].transform = Converter.to_transform_value(cubes[1].transform);

        //    pc.set_filtering_obb_infos(infos);
        //    pc.set_rendering(PointCloud.RenderingType.ParabloidGeo);
        //}

        void OnApplicationQuit() {


            ExVR.ExpLog().builder(string.Format("Application ending after {0} seconds -> {1}", Time.time, GlobalVariables.wantToLeave));

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

//unsafe class Program {
//    public static unsafe void tests() {

//        int nbValues = 1000000;
//        System.Collections.Generic.List<double> values = new System.Collections.Generic.List<double>(nbValues);
//        for(int ii = 0; ii < nbValues; ++ii) {
//            values.Add((double)UnityEngine.Random.Range(0f, 1000f));
//        }

//        System.Collections.Generic.List<string> valuesStr = new System.Collections.Generic.List<string>(values.Count);
//        for(int ii = 0; ii < values.Count; ++ii) {
//            valuesStr.Add(null);
//        }

//        const int buffer_length = 2000;
//        var buffer = stackalloc char[buffer_length];
//        //var buffer = stackalloc char[buffer_length];
//        //var buffer = new char[buffer_length];

//        System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
//        sw.Start();
//        for (int ii = 0; ii < values.Count; ++ii) {
//            var res = RyuCsharp.Ryu.d2s_buffered_n(values[ii], buffer);
//            valuesStr[ii] = new string(buffer, 0, res);
//            Empty(buffer, res);
//        }
//        sw.Stop();
//        UnityEngine.Debug.LogError("d2s_buffered_n: " + sw.ElapsedMilliseconds);

//        //sw.Restart();
//        //for (int ii = 0; ii < values.Count; ++ii) {
//        //    valuesStr[ii] = new string(buffer, 0, RyuCsharp.Ryu.d2exp_buffered_n(values[ii], 5, buffer));
//        //    Empty(buffer, buffer_length);
//        //}
//        //sw.Stop();
//        //UnityEngine.Debug.LogError("d2exp_buffered_n: " + sw.ElapsedMilliseconds);

//        //sw.Restart();
//        //for (int ii = 0; ii < values.Count; ++ii) {
//        //    valuesStr[ii] = new string(buffer, 0, RyuCsharp.Ryu.d2fixed_buffered_n(values[ii], 5, buffer));
//        //    Empty(buffer, buffer_length);
//        //}
//        //sw.Stop();
//        //UnityEngine.Debug.LogError("d2fixed_buffered_n: " + sw.ElapsedMilliseconds);

//        sw.Restart();
//        for (int ii = 0; ii < values.Count; ++ii) {
//            valuesStr[ii] = Ex.Converter.to_string(values[ii]);
//        }
//        sw.Stop();
//        UnityEngine.Debug.LogError("converter: " + sw.ElapsedMilliseconds);

//        sw.Restart();
//        for (int ii = 0; ii < values.Count; ++ii) {            
//            valuesStr[ii] = values[ii].ToString();
//        }
//        sw.Stop();
//        UnityEngine.Debug.LogError("ToString: " + sw.ElapsedMilliseconds);

//        sw.Restart();
//        for (int ii = 0; ii < values.Count; ++ii) {
//            valuesStr[ii] = values[ii].ToString("G15");
//        }
//        sw.Stop();
//        UnityEngine.Debug.LogError("ToString format g15: " + sw.ElapsedMilliseconds);

//        //test(3);
//        //test(3.14);
//        //test(3.1415926);
//        //test(998);
//        //test(1218);
//        //test(19971218);
//        //test(ulong.MaxValue);
//        //test(long.MinValue);
//        //test(0.1);
//        //test(0.00314);
//        //test(0.0000000998);
//        //test(-0.0000000998);


//        //test(3.1415926e100);
//        //test(double.MaxValue);
//        //test(double.MinValue);
//    }

//    //public static void test(double val) {
//    //    const int buffer_length = 2000;

//    //    var buffer = stackalloc char[2000];


//    //    var str1 = new string(buffer, 0, RyuCsharp.Ryu.d2s_buffered_n(val, buffer));
//    //    double val1;
//    //    var eq1 = RyuCsharp.Ryu.s2d_n(buffer, str1.Length, &val1);
//    //    Empty(buffer, buffer_length);

//    //    var str2 = new string(buffer, 0, RyuCsharp.Ryu.d2exp_buffered_n(val, 10, buffer));
//    //    double val2;
//    //    var eq2 = RyuCsharp.Ryu.s2d_n(buffer, str2.Length, &val2);
//    //    Empty(buffer, buffer_length);

//    //    var str3 = new string(buffer, 0, RyuCsharp.Ryu.d2fixed_buffered_n(val, 10, buffer));
//    //    double val3;
//    //    var eq3 = RyuCsharp.Ryu.s2d_n(buffer, str3.Length, &val3);
//    //    Empty(buffer, buffer_length);

//    //    UnityEngine.Debug.Log($"Value: {val}, d2s: [{str1} -- s2d: {val1}], d2exp(10): [{str2} -- s2d: {val2}], d2fixed(10): [{str3} -- s2d: {val3}]");
//    //}

//    //public static void Empty(char* buffer, int length) {
//    //    for (int i = 0; i < length; i++) {
//    //        buffer[i] = default;
//    //    }
//    //}
//}


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





// https://eyeofrablog.wordpress.com/2017/06/11/windows-keylogger-part-1-attack-on-user-land/
// https://github.com/VollRahm/NotEnoughHotkeys/blob/18315edd8dea200145c81d2127d68b517ac5df3f/src/NotEnoughHotkeys/NEHSubprocess/RawInputLib/RawInput.cs#L16
//List<IntPtr> windowHandles = new List<IntPtr>();
//var processes = System.Diagnostics.Process.GetProcesses();
//Debug.LogError("processes " + processes.Length);

//foreach (var window in processes) {
//    //window.Refresh();
//    //if (window.MainWindowHandle != IntPtr.Zero) {

//    //}
//    windowHandles.Add(window.MainWindowHandle);
//    Debug.LogError("window.ProcessName " + window.ProcessName);
//}
//var current = System.Diagnostics.Process.GetCurrentProcess();


//uint ipdw = 0;
//var res = Native.GetWindowThreadProcessId(Native.GetForegroundWindow(), out ipdw);

//Debug.LogError("ipdw " + ipdw + " " + res);
//KeyboardHandler k = new KeyboardHandler(current.MainWindowHandle);
//k.LoadDevices();

//Native.RAWINPUTDEVICELIST[] list = new NotEnoughHotkeys.RawInputLib.Native.RAWINPUTDEVICELIST[10];
//Native.GetRawInputDeviceList(list, ref numDevices, 10);

//foreach(var l in list) {
//    Debug.LogError("l " + l.hDevice);
//}
