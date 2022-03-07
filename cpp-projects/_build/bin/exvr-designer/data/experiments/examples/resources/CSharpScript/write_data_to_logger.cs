// system
using System;
using System.Reflection;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class Data {
        public int someData1 = 0;
        public float someData2 = 0f;
        public string someData3 = "sample";

        public string to_string() {
            return string.Format("{0},{1},{2}",
                Converter.to_string(someData1),
                Converter.to_string(someData2),
                someData3
            );
        }

        public string header() {
            return "someData1,someData2,someData3";
        }
    }

    public class WriteDataToLoggerComponent : BaseCompiledCSharpComponent{

        public Data data = new Data();
        private LoggerComponent logger = null;

        public override void start_experiment() {
            logger = init_config().get_component<LoggerComponent>("Logger");
            logger.write(data.header(), true);
        }

        public void write_current_data_to_logger() {
            logger.write(data.to_string(), true);
        }
    }
}

