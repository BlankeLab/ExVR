// system
using System;
using System.Reflection;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class TestComponent : BaseCompiledCSharpComponent{

        // ### internal settings, do not remove
        public static MonoBehaviour add_component_to_gameobject(GameObject host) {
            return auto_add(host, MethodBase.GetCurrentMethod().DeclaringType);
        }
        
        // ### Ex component flow functions, uncomment to use
        // # main functions
        public bool initialize() {

            GameObject a = new GameObject();
            a.transform.SetParent(transform);
            a.name = "bouh";
            return true;
        }
        public void start_experiment() {}
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
        // public void slot1(object value){}
        // public void slot2(object value){}
        // public void slot3(object value){}
        // public void slot4(object value){}        
    }
}


/**
namespace Ex{    

    public class BaseCompiledCSharpComponent : MonoBehaviour{

        // ExComponent class associated to the script (see ExComponent code snippet) 
        public ExComponent p = null;
        
        // init configuration for this component 
        public ComponentInitConfig init_config();
        // current configuration of the routine for this component
        public ComponentConfig current_config();
        // current routine associated to this component
        public Routine current_routine();
        // current condition of the associated routine
        public Condition current_condition();
        
        // states
        public bool is_initialized();
        public bool is_visible();
        public bool is_updating();
        public bool is_closed();

        // special commands
        public void next();
        public void previous();
        public void close();
        public void stop();
        
        // timers
        public long ellapsed_time_exp_ms();
        public long ellapsed_time_routine_ms();
        
        // invoke signals
        public void invoke_signal1(object value);
        public void invoke_signal2(object value);
        public void invoke_signal3(object value);
        public void invoke_signal4(object value);
        
    }
}
*/
