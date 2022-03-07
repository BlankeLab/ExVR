// system
using System;
using System.Reflection;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class TemplateComponent : BaseCompiledCSharpComponent{

        // ### internal settings, do not remove
        public static MonoBehaviour add_component_to_gameobject(GameObject host) {
            return auto_add(host, MethodBase.GetCurrentMethod().DeclaringType);
        }
        
        // ### Ex component flow functions, uncomment to use
        // # main functions
        public bool initialize() {return true;}
        public void start_experiment() {}

        // # slots
        public void slot1(object value) {

            
            string keyPressed = ((KeyCode)value).ToString();
            if (keyPressed == "Keypad5") {
                ExVR.Log().message(keyPressed + " pressed");
                //invoke_signal1((object)keyPressed);// + " pressed at "  + ellapsed_time_routine_ms());
                //invoke_signal2(0);
                invoke_signal2("" + keyPressed + " pressed at " + ellapsed_time_routine_ms());
                invoke_signal1(0);                
            }
        }
    }
}
