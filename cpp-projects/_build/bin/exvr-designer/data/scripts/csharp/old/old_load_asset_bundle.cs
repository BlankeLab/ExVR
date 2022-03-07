

// system
using System;

// unity
using UnityEngine;


// IMPORTANT: replace TemplateComponent with your component class name 
public class TemplateComponent : MonoBehaviour{
     
    public static TemplateComponent add_component_to_gameobject(GameObject host) {
        return host.AddComponent<TemplateComponent>();
    }

    #region internal // do not modify this part

    public Expy.Component pCompo = null;
    public Expy.ComponentInitConfig initC = null;
    public Expy.ComponentConfig currC     = null;    
    
    public GameObject load_asset_bundle(string pathAssetBundle, string nameSubObject) {

        if(pathAssetBundle.Length == 0 || nameSubObject.Length == 0) {
            return null;
        }

        AssetBundle assetBundle = AssetBundle.LoadFromFile(pathAssetBundle);
        if (assetBundle == null) {
            pCompo.log_error("Failed to load AssetBundle!");
            return null;
        }

        var assets = assetBundle.LoadAsset<GameObject>(nameSubObject);
        if (assets == null) {
            pCompo.log_error("Sub oject doesn't exist in loaded AssetBundle!");
            System.Threading.Thread.Sleep(50);
            assetBundle.Unload(false);
            return null;
        }

        GameObject bundle = Instantiate(assets);
        bundle.transform.SetParent(transform);
        bundle.transform.name = nameSubObject;

        System.Threading.Thread.Sleep(50); // for avoiding crash when loading cab file
        assetBundle.Unload(false);

        return bundle;
    }

    #endregion internal // do not modify this part

    public bool update_parameter_from_gui(string configName, Expy.XML.Arg arg) {

        if (arg.Name == "arg1") {
            // ...
        }

        return true;
    }
    
    public bool initialize(Expy.Component parentComponent, Expy.ComponentInitConfig initConfig){

        pCompo = parentComponent; // parent component for calling common functions
        initC  = initConfig;      // initial configuration for getting arguments

        string pathAssetBundle = "";// initC.get<string>("path_bundle");
        string nameSubObject = "";// initC.get<string>("name_sub_object");

        GameObject bundle = load_asset_bundle(pathAssetBundle, nameSubObject);
        if(bundle == null) {
            pCompo.log("No asset bundle loaded.");
        }

        pCompo.log_error("succes");
        // string pathAssetBundle = initC.get<string>("path_bundle");

        return true;
    }
    
    public void start(Expy.ComponentConfig currentConfig) {
        currC = currentConfig; // update the current configuration for getting arguments
    }
    
    public void stop(){
        
    }
    
    public void update(){
        //Debug.Log("update-from-script");
    }
    
    public void clean(){
        
    }    
    
    public void enable(){
        
    }
    
    public void disable(){
        
    }
}