
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
using System;
using System.IO;
using System.Collections.Generic;

// unity
using UnityEngine;

#if UNITY_EDITOR
using UnityEditor;
namespace Ex{

    [CustomEditor(typeof(Ex.ResourcesManager), true)]
    public class ResourcesManagerEditor : Editor{

        public override void OnInspectorGUI() {

            if (!Application.isPlaying) {
                return;
            }

            Ex.ResourcesManager manager = (Ex.ResourcesManager)target;
            EditorGUILayout.LabelField("### Resources per type:");
            foreach (Ex.ResourcesManager.ResourceType r in Enum.GetValues(typeof(Ex.ResourcesManager.ResourceType))) {

                var resources = manager.get_resources_from_type(r);
                if(resources.Count > 0) {
                    EditorGUILayout.LabelField(String.Format("{0} -> count: {1}", r.ToString(), resources.Count));
                    foreach(var resource in resources) {
                        EditorGUILayout.LabelField(String.Format("\t[{0}] [{1}] ({2}) ", resource.alias, resource.path, resource.key));
                    }
                }
            }
        }
    }
}
#endif

namespace Ex { 

    public class ResourcesManager : MonoBehaviour {

        public enum ResourceType : int {  
            Audio = 1, Image = 2, Mesh = 4, Text = 8, Video = 16, 
            UnityAssetBundle = 32, CSharpScript = 64, PythonScript = 128, Cloud = 256,
            ScanerVideo = 512, Plot = 1024, Directory = 2048, VolumetricVideo = 4096
        };

        public void log_message(string message) {
            ExVR.Log().message(string.Format("[RESOURCE] {0}", message), false, "", "", 0, true, false);
        }

        public void log_warning(string warning) {
            ExVR.Log().warning(string.Format("[RESOURCE] {0}", warning), true, "", "", 0, true, false);
        }

        public void log_error(string error) {
            ExVR.Log().error(string.Format("[RESOURCE] {0}", error), true, "", "", 0, true, false);
        }

        private Dictionary<ResourceType, Dictionary<string, ExResource>> m_pathMappingResources = null; // type - path -> resource        
        private Dictionary<ResourceType, Dictionary<string, ExResource>> m_aliasMappingResources = null; // type - alias -> resource

        public List<ExResource> get_resources_from_type(ResourceType type) {
            List<ExResource> resources = new List<ExResource>(m_pathMappingResources[type].Count);
            foreach(var resource in m_pathMappingResources[type]) {
                resources.Add(resource.Value);
            }
            return resources;
        }

        public void initialize() {

            // init dictionnaries
            m_pathMappingResources = new Dictionary<ResourceType, Dictionary<string, ExResource>>();
            m_aliasMappingResources = new Dictionary<ResourceType, Dictionary<string, ExResource>>();
            foreach (ResourceType type in Enum.GetValues(typeof(ResourceType))) {
                m_pathMappingResources[type]    = new Dictionary<string, ExResource>();
                m_aliasMappingResources[type]   = new Dictionary<string, ExResource>();
            }

            // add defaults
            add_default_resources();
        }

        public void add_default_resources() {

            // images
            var dImage = generate_resource(ResourceType.Image, -1, "default_texture", "");

            //var dImage = new ImageResource(-1, "default_texture", "");
            dImage.read_data();
            dImage.initialize();
            dImage.doNotRemove = true;
            m_aliasMappingResources[ResourceType.Image]["default_texture"] = dImage;

            dImage.transform.SetParent(transform);
            // ...
        }

        public void clean() {

            foreach(var resourcesType in m_aliasMappingResources) {
                foreach (var resource in resourcesType.Value) {
                    resource.Value.clean();
                    Destroy(resource.Value.gameObject);
                }
            }
        }

        //private ExResource generate_resource(ResourceType type, int key, string alias, string path) {
        private ExResource generate_resource(ResourceType type, int key, string alias, string path) {

            GameObject resourceGO = GO.generate_empty_object(alias, null, true);

            ExResource resource = null;
            // resource file exists
            switch (type) {
                case ResourceType.Image:
                    (resource = resourceGO.AddComponent<ImageResource>()).create(key, alias, path); break;
                //return new ImageResource(key, alias, path);
                case ResourceType.Text:
                    (resource = resourceGO.AddComponent<TextResource>()).create(key, alias, path); break;
                //return new TextResource(key, alias, path);
                case ResourceType.UnityAssetBundle:
                    (resource = resourceGO.AddComponent<AssetBundleResource>()).create(key, alias, path); break;
                //return new AssetBundleResource(key, alias, path);
                case ResourceType.Video:
                    (resource = resourceGO.AddComponent<VideoResource>()).create(key, alias, path); break;
                //return new VideoResource(key, alias, path);
                case ResourceType.Audio:
                    (resource = resourceGO.AddComponent<AudioResource>()).create(key, alias, path); break;
                //return new AudioResource(key, alias, path);
                case ResourceType.CSharpScript:
                    (resource = resourceGO.AddComponent<CSharpScriptResource>()).create(key, alias, path); break;
                //return new CSharpScriptResource(key, alias, path);
                case ResourceType.Cloud:
                    (resource = resourceGO.AddComponent<CloudResource>()).create(key, alias, path); break;
                //return new CloudResource(key, alias, path);
                case ResourceType.ScanerVideo:
                    (resource = resourceGO.AddComponent<ScanerVideoResource>()).create(key, alias, path); break;
                //return new ScanerVideoResource(key, alias, path);
                case ResourceType.Plot:
                    (resource = resourceGO.AddComponent<PlotResource>()).create(key, alias, path); break;
                //return new PlotResource(key, alias, path);
                case ResourceType.Directory:
                    (resource = resourceGO.AddComponent<DirectoryResource>()).create(key, alias, path); break;
                //return new DirectoryResource(key, alias, path);
                case ResourceType.VolumetricVideo:
                    (resource = resourceGO.AddComponent<VolumetricVideoResource>()).create(key, alias, path); break;

                    //return new VolumetricVideoResource(key, alias, path);
            }
            //return null;
            return resource;
        }

        private bool add_resource_from_xml(XML.Resource resource, ResourceType type) {

            bool exists = (type == ResourceType.Directory) ? Directory.Exists(resource.Path) : File.Exists(resource.Path);
            if (!exists) {
                log_error(string.Format("Cannot load resource file of type [{0}] with path: {1}", resource.Type, resource.Path));
                return false;
            }

            // load resource if not mapped
            if (!m_pathMappingResources[type].ContainsKey(resource.Path)) {

                ExResource resourceData = generate_resource(type, resource.Key, resource.Alias, resource.Path);

                if (resourceData != null) {
                    m_pathMappingResources[type][resource.Path] = resourceData;
                    m_aliasMappingResources[type][resource.Alias] = resourceData;
                    if (!resourceData.read_data()) {
                        log_error(string.Format("Cannot read data from resource file of type [{0}] with path: {1}.", resource.Type, resource.Path));                        
                        resourceData.clean();
                        Destroy(resourceData.gameObject);
                        return false;
                    }
                    if (!resourceData.initialize()) {
                        log_error(string.Format("Cannot initialize resource file of type [{0}] with path: {1}.", resource.Type, resource.Path));
                        resourceData.clean();
                        Destroy(resourceData.gameObject);
                        return false;
                    }
                    resourceData.transform.SetParent(transform);
                }
            } else {
                log_error(string.Format("Resource file of type [{0}] with path: {1} already added.", resource.Type, resource.Path));
            }

            return true;
        }

        public bool generate_from_xml(XML.Experiment xmlExperiment) {


            XML.Resources xmlResources = xmlExperiment.Resources;

            // fill dictionnay for commodity
            var newResourcesPerPathPerType = new Dictionary<ResourceType, Dictionary<string, XML.Resource>>();
            foreach (var resource in xmlResources.Resource) {

                var type = (ResourceType)Enum.Parse(typeof(ResourceType), resource.Type);
                if (!newResourcesPerPathPerType.ContainsKey(type)) {
                    newResourcesPerPathPerType[type] = new Dictionary<string, XML.Resource>();
                }

                // set all path to absolute
                if (!Path.IsPathRooted(resource.Path)) {
                    resource.Path = Path.GetFullPath(ExVR.Paths().designerDataTempDir + "/" + resource.Path);
                }

                newResourcesPerPathPerType[type][resource.Path] = resource;
            }

            // look for resources not available anymore
            List<Tuple<ResourceType, string, ExResource>> resourcesToRemove = new List<Tuple<ResourceType, string, ExResource>>();
            foreach(var mappingPerType in m_pathMappingResources) {
                foreach(var resource in mappingPerType.Value) {

                    // look for resource in dictionnaries
                    bool found = false;
                    if (newResourcesPerPathPerType.ContainsKey(mappingPerType.Key)){
                        if (newResourcesPerPathPerType[mappingPerType.Key].ContainsKey(resource.Key)) {
                            found = true;
                        }
                    }
                    
                    // if not found remove from it
                    if (!found) {
                        resourcesToRemove.Add(new Tuple<ResourceType, string, ExResource>(mappingPerType.Key, resource.Key, resource.Value));
                    }
                }
            }

            // remove theses resources
            if (resourcesToRemove.Count > 0) {

                log_message(string.Format("Remove unused resources: {0}", resourcesToRemove.Count));
                foreach (var resource in resourcesToRemove) {
                    m_aliasMappingResources[resource.Item1].Remove(resource.Item3.alias);
                    m_pathMappingResources[resource.Item1].Remove(resource.Item2);
                    resource.Item3.clean();
                    Destroy(resource.Item3.gameObject);
                }
            }

            // clean resources depending reload code
            foreach (ResourceType type in Enum.GetValues(typeof(ResourceType))) {

                // check reload code
                if((xmlResources.ReloadCode & (int)type) == 0) {
                    continue;
                }

                // clean data
                List<ExResource> resourcesToKeep = new List<ExResource>();
                foreach (var resourcesPerType in m_aliasMappingResources[type]) {
                    if (!resourcesPerType.Value.doNotRemove) {
                        resourcesPerType.Value.clean();
                        Destroy(resourcesPerType.Value.gameObject);
                    } else {
                        resourcesToKeep.Add(resourcesPerType.Value);
                    }
                }

                // remove from dictionnary
                if(m_pathMappingResources[type].Count > 0) {
                    log_message(string.Format("Clean [{0}] [{1}]", m_pathMappingResources[type].Count, type));
                    m_pathMappingResources[type].Clear();
                    m_aliasMappingResources[type].Clear();
                }

                // add again non removables resources in dictionnaries
                foreach(ExResource resourceToKeep in resourcesToKeep) {
                    m_aliasMappingResources[type][resourceToKeep.alias] = resourceToKeep;
                }
            }

            // remove already added resources
            foreach(var currentResourcesPerType in m_pathMappingResources) {

                if (!newResourcesPerPathPerType.ContainsKey(currentResourcesPerType.Key)) {
                    continue;
                }

                var newResourcesPerPath = newResourcesPerPathPerType[currentResourcesPerType.Key];
                foreach (var currentResourcesPerPath in currentResourcesPerType.Value) {
                    if (newResourcesPerPath.ContainsKey(currentResourcesPerPath.Key)) {
                        newResourcesPerPath.Remove(currentResourcesPerPath.Key);
                    }
                }
            }

            // generate C# script resource (we need to compile every C# script before generating others resources)
            if (newResourcesPerPathPerType.ContainsKey(ResourceType.CSharpScript)) {
                if (newResourcesPerPathPerType[ResourceType.CSharpScript].Count > 0) {
                    log_message(string.Format("Generate [{0}] [{1}]", newResourcesPerPathPerType[ResourceType.CSharpScript].Count, ResourceType.CSharpScript));

                    // ### retrieve c# scripts resources load them
                    List<string> pathToRemove = new List<string>();
                    foreach (var resource in newResourcesPerPathPerType[ResourceType.CSharpScript]) {
                        add_resource_from_xml(resource.Value, ResourceType.CSharpScript);
                        pathToRemove.Add(resource.Value.Path);
                    }

                    // remove c# scripts resources from main resources list
                    foreach (var path in pathToRemove) {
                        newResourcesPerPathPerType[ResourceType.CSharpScript].Remove(path);
                    }
                }
            }

            // ###  compile/recompile scripts
            if ((xmlResources.ReloadCode & (int)ResourceType.CSharpScript) != 0 || CSharpScriptResource.get_compiled_assembly() == null) {
                
                List<string> scriptsFiles = new List<string>();

                // retrieve resources scripts
                foreach (var csharpData in m_pathMappingResources[ResourceType.CSharpScript]) {
                    scriptsFiles.Add(csharpData.Value.path);
                    //log_message("-> " + csharpData.Value.path);
                }

                // retrieve ui functions scripts                
                foreach (var tempFile in CSharpFunctionComponent.generate_files_from_scripts_functions(xmlExperiment.Components)) {
                    scriptsFiles.Add(tempFile);
                    //log_message("-> " + tempFile);
                }

                // compile assembly from scripts
                if (scriptsFiles.Count > 0) {
                    log_message(string.Format("Compile C# scripts [{0}]...", scriptsFiles.Count));                    
                    if(CSharpScriptResource.compile_assembly_from_scripts_files(scriptsFiles.ToArray()) == null) {
                        return false;
                    }
                }
            }


            // generate others resources
            foreach (var resourcesPerPath in newResourcesPerPathPerType) {
                if (resourcesPerPath.Value.Count > 0){
                    log_message(string.Format("Generate [{0}] [{1}]", resourcesPerPath.Value.Count, resourcesPerPath.Key));
                    foreach (var resource in resourcesPerPath.Value) {
                        if (!add_resource_from_xml(resource.Value, resourcesPerPath.Key)) {
                            continue;
                        }
                    }
                }
            }

            return true;
        }

        public ExResource get_resource_file_data(int key) {

            foreach(var resourcesPerType in m_aliasMappingResources) {
                foreach(var resource in resourcesPerType.Value) {
                    if(resource.Value.key == key) {
                        return resource.Value;
                    }
                }
            }

            log_error(string.Format("Cannot read resource with key [{0}] from resources.", key));
            return null;
        }

        public ExResource get_resource_file_data(ResourceType type, int key) {

            foreach (var resource in m_aliasMappingResources[type]) {
                if(resource.Value.key == key) {
                    return resource.Value;
                }
            }
            log_error(string.Format("Cannot read resource with key [{0}] and type [{1}] from resources.", key, type.ToString()));
            return null;
        }

        public ExResource get_resource_file_data(ResourceType type, string alias) {
            if (!m_aliasMappingResources[type].ContainsKey(alias)) {
                log_error(string.Format("Cannot read resource with alias [{0}] and type [{1}] from resources.", alias, type.ToString()));
                return null;
            }
            return m_aliasMappingResources[type][alias];
        }

        public ScanerVideoResource get_scaner_video_file_data(string alias) {
            if (!m_aliasMappingResources[ResourceType.ScanerVideo].ContainsKey(alias)) {
                log_error(string.Format("Cannot read scaner video with alias [{0}] from resources.", alias ));
                return null;
            }
            return ((ScanerVideoResource)m_aliasMappingResources[ResourceType.ScanerVideo][alias]);
        }

        public CloudResource get_cloud_file_data(string alias) {
            if (!m_aliasMappingResources[ResourceType.Cloud].ContainsKey(alias)) {
                log_error(string.Format("Cannot read cloud with alias [{0}] from resources.", alias));
                return null;
            }
            return ((CloudResource)m_aliasMappingResources[ResourceType.Cloud][alias]);
        }

        public TextResource get_text_file_data(string alias, bool showError = true) {
            if (!m_aliasMappingResources[ResourceType.Text].ContainsKey(alias)) {
                if (showError) {
                    log_error(string.Format("Cannot read text with alias [{0}] from resources.", alias));
                }
                return null;
            }
            return ((TextResource)m_aliasMappingResources[ResourceType.Text][alias]);
        }
        public AudioResource get_audio_file_data(string alias) {
            if (!m_aliasMappingResources[ResourceType.Audio].ContainsKey(alias)) {
                log_error(string.Format("Cannot read audio with alias [{0}] from resources.", alias));
                return null;
            }
            return ((AudioResource)m_aliasMappingResources[ResourceType.Audio][alias]);
        }

        public VideoResource get_video_file_data(string alias) {
            if (!m_aliasMappingResources[ResourceType.Video].ContainsKey(alias)) {
                log_error(string.Format("Cannot read video with alias [{0}] from resources.", alias));
                return null;
            }
            return ((VideoResource)m_aliasMappingResources[ResourceType.Video][alias]);
        }

        public PlotResource get_plot_file_data(string alias) {
            if (!m_aliasMappingResources[ResourceType.Plot].ContainsKey(alias)) {
                log_error(string.Format("Cannot read plot with alias [{0}] from resources.", alias));
                return null;
            }
            return ((PlotResource)m_aliasMappingResources[ResourceType.Plot][alias]);
        }

        public ImageResource get_image_file_data(string alias, bool displayError = true) {
            if (!m_aliasMappingResources[ResourceType.Image].ContainsKey(alias)) {
                if (displayError) {
                    log_error(string.Format("Cannot read image with alias [{0}] from resources.", alias));
                }
                return null;
            }
            return ((ImageResource)m_aliasMappingResources[ResourceType.Image][alias]);
        }

        public AssetBundleResource get_asset_bundle_file_data(string alias) {
            if (!m_aliasMappingResources[ResourceType.UnityAssetBundle].ContainsKey(alias)) {
                log_error(string.Format("Cannot read asset bundle with alias [{0}] from resources.", alias));
                return null;
            }
            return ((AssetBundleResource)m_aliasMappingResources[ResourceType.UnityAssetBundle][alias]);
        }

        public VolumetricVideoResource get_volumetric_video_file_data(string alias) {
            if (!m_aliasMappingResources[ResourceType.VolumetricVideo].ContainsKey(alias)) {
                log_error(string.Format("Cannot read volumetric video with alias [{0}] from resources.", alias));
                return null;
            }
            return ((VolumetricVideoResource)m_aliasMappingResources[ResourceType.VolumetricVideo][alias]);
        }


        public GameObject instantiate_asset_bundle(string alias, string nameSubObject, Transform parent = null) {

            var assetBundle = get_asset_bundle_file_data(alias);
            if(assetBundle == null) {
                return null;
            }

            GameObject assets;
            if(nameSubObject.Length == 0) {
                assets = assetBundle.bundle.LoadAsset<GameObject>(assetBundle.bundle.GetAllAssetNames()[0]);
            } else {
                assets = assetBundle.bundle.LoadAsset<GameObject>(nameSubObject);                
            }
            
            // load asset from asset bundle
            if (assets == null) {
                log_error(string.Format("Sub object [{0}] doesn't exist in loaded AssetBundle with alias: {1}", nameSubObject, alias));
                System.Threading.Thread.Sleep(50);
                return null;
            }

            // instantiate gameobject from assets
            var go = Instantiate(assets) as GameObject;
            if (parent != null) {
                go.transform.SetParent(parent);
            }            
            go.layer = Layers.Default;

            // force layer option ?
            foreach(Transform child in go.transform.GetComponentInChildren<Transform>()) {
                child.gameObject.layer = Layers.Default;
            }

            if (go.GetComponent<RectTransform>() == null) {
                go.transform.localPosition = new Vector3(0, 0, 0);
                go.transform.localEulerAngles = new Vector3(0, 0, 0);
                go.transform.localScale = new Vector3(1, 1, 1);
            }

            go.name = nameSubObject.Length == 0 ? alias : nameSubObject;
            go.SetActive(false);

            return go;
        }
    }
}
