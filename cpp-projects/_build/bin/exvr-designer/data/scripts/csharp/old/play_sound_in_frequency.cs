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


        public void start_routine() {
            bpm = 0.0;
            lastTimePlaying = 0.0;
            startPlaying = false;
        }

        // # slots

        double bpm = 0.0;
        bool startPlaying = false;
        double lastTimePlaying = 0.0;

        public void slot1(object value) {
            bpm =  ((double)value);
            
            if (!startPlaying) {
                invoke_signal1(0);
                lastTimePlaying = 0;
            }
            startPlaying = true;
        }


        

        public void update() {

            if (startPlaying) {
                var diffTime = (ellapsed_time_routine_ms() - lastTimePlaying)/1000.0; // seconds

                if (diffTime > 1.0/(bpm/60)) {
                    lastTimePlaying = ellapsed_time_routine_ms();
                    invoke_signal1(0);
                } 
            }
        }
    }
}
