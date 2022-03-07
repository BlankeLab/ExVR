// system
using System;
using System.Reflection;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    //public struct BundleInfo{
    //    bool isTargeted = false;
    //    GameObject bundle = null;
    //    // ..
    //    // ..
    //}

    public class TemplateComponent : BaseCompiledCSharpComponent{

        //Dictionary<string, BundleInfo> objectsInfos = new Dictionary<string, BundleInfo>();

        // ### internal settings, do not remove
        public static MonoBehaviour add_component_to_gameobject(GameObject host) {
            return auto_add(host, MethodBase.GetCurrentMethod().DeclaringType);
        }

        public GameObject get_parent_asset_bundle_component(GameObject target) {

            //var assetCompo = target.GetComponent<AssetBundleCom>();
            //if(assetCompo != null) {
            //    return assetCompo;
            //}

            //var goParent = target.transform.parent;
            //if(goParent != null) {
            //    return get_parent_asset_bundle_component(goParent);
            //}

            return null;
        } 


        // ### Ex component flow functions, uncomment to use
        // # main functions
        public bool initialize() {return true;}
        public void start_experiment() {



            List<string> namesObjects = new List<string>(new string[] {
                "object1_name",
                "object2_name",
                "object3_name",
            });

            //foreach(var objName in namesObjects) {
            //    var info = new BundleInfo();
            //    info.bundle = (((AssetBundel)ExComponent.get(objName)).bundle);
            //    //info.scaling = info.bundle.transform.lcaos
            //    objectsInfos[objName] = info;
            //}

            //var bundle = ...;


            //objectsInfos[objName].isTargeted = true;



            //Ray a = new Ray(
            //    ExVR.VR().eyeCamera.transform.position,
            //    ExVR.VR().eyeCamera.transform.forward
            //);

            //RaycastHit hit;
            //Physics.Raycast(a, out hit);
            //hit.collider.gameObject;

        }
        // public void start_routine() {}
        // public void update() {}
        // public void stop_routine() {}
        // public void stop_experiment(){}
        // public void play(){}
        // public void pause(){}
        // public void set_update_state(bool doUpdate) { }        
        // public void set_visibility(bool visible) { }

        // # for advanced users 
        // public void clean(){}
        // public void pre_update() {}
        // public void post_update() {}
        // public void update_parameter_from_gui(XML.Arg arg);
        // public void update_from_current_config(){}

        // # slots
        public void slot1(object value) {
            int key = (int)value;

            // ...
            float nValue = 15f;
        }
        // public void slot2(object value){}
        // public void slot3(object value){}
        // public void slot4(object value){}

    }
}
