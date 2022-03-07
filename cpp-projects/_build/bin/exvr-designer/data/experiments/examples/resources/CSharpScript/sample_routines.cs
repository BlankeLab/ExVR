// system
using System;
using System.Reflection;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class Routine1ScriptComponent : BaseCompiledCSharpComponent{

        public override void start_routine() {
            var script = get_csharp_script<WriteDataToLoggerComponent>("Write data script");
            script.data.someData1 = 1;
            script.data.someData2 = 1f;
            script.data.someData3 = "routine1";
            script.write_current_data_to_logger();
        }
    }

    public class Routine2ScriptComponent : BaseCompiledCSharpComponent {

        public override void start_routine() {
            var script = get_csharp_script<WriteDataToLoggerComponent>("Write data script");
            script.data.someData1 = 2;
            script.data.someData2 = 2f;
            script.data.someData3 = "routine2";
            script.write_current_data_to_logger();
        }
    }

    public class Routine3ScriptComponent : BaseCompiledCSharpComponent {

        public override void start_routine() {
            var script = get_csharp_script<WriteDataToLoggerComponent>("Write data script");
            script.data.someData1 = 3;
            script.data.someData2 = 3f;
            script.data.someData3 = "routine3";
            script.write_current_data_to_logger();
        }
    }
}

