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


        // public void start_routine() {}

        double bpm = 0.0;
        double lastPeak = 0.0;
        double lastTimePeak = 0.0;

        // # slots
        public void slot1(object value) { //bpm
            bpm = (double)value;
        }

        public void slot2(object value) { // peak
            lastPeak = (double)value;
        }

        public void slot3(object value) { // peak time
            lastTimePeak = ellapsed_time_routine_ms();
        }

        public void update() {

            double predictedFrequency = bpm;

            // implement viviana code here
            // you can use invoke_signal1/invoke_signal2/invoke_signal3/invoke_signal4 with a int/real node for debbugging/
            // or using ExVR.Log().message("text"); / // or using ExVR.Log().message("" + value_to_display);

            invoke_signal1(predictedFrequency);
        }

    }
}
