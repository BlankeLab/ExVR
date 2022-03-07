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
            times = new List<double>();
        }



        List<double> times = null;
        bool goNext = false;
        double timeGoNext = 0;
        int timeMsAfterStartingSoundForGoingNext = 500;

        // # slots
        public void slot1(object value) {
            double lastTime = ellapsed_time_routine_ms();
            times.Add(lastTime);

            ExVR.Log().message("peak: " + lastTime);
        }

        public void update() {

            double currentTime = ellapsed_time_routine_ms();
            for (int ii = 0; ii < times.Count; ++ii) { 
                if(currentTime - times[ii] > current_config().get<int>("delai")) {
                    ExVR.Log().message("play peak: " + (currentTime - times[ii]) + " " + currentTime);
                    times.RemoveAt(ii);
                    invoke_signal1(0);
                    goNext = true;
                    timeGoNext = ellapsed_time_routine_ms();
                    break;
                }
            }

            if (goNext) {
                if (ellapsed_time_routine_ms() - timeGoNext > timeMsAfterStartingSoundForGoingNext) {
                    goNext = false;
                    invoke_signal2(0);
                }
            }
        }

    }
}
