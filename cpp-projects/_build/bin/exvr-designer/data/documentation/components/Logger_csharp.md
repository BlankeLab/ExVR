

### Content:

&nbsp;

```
// system
using System;
using System.Collections.Generic;
using System.IO;

namespace Ex{

    public class LoggerComponent : ExComponent{

        public string parent_directory_path();

        public string file_path();

        public string file_extension();

        // write each string of the list as a new line in the logging file
        public void write_lines(List<string> values);  

        // write try to convert the input object to a string and write it in the logging file, if line is set to true, creates a new line
        public void write(object value, bool line = true);

        // if streamed mode choosen in parameters, it forces the writing of the content inside the logging file
        public void write_to_file();
    }
}
```


### Samples:

```

    // retrieve component 
    var loggerComponent = (LoggerComponent)get("logger_component_name");
    // or 
    var loggerComponent = get<LoggerComponent>("logger_component_name");
    
    // write inside logging file
    string text1 = "hello ";
    string text2 = "world";
    
    loggerComponent.write(text1, false);
    loggerComponent.write(text2, false);
    // -> will write "hello world";
    
    loggerComponent.write(text1);
    loggerComponent.write(text2);
    loggerComponent.write("from");
    loggerComponent.write(1988);
    // -> will write "hello \nworld\nfrom\n1988";
    
    List<string> lines = new List<string>();
    lines.Add("hello");
    lines.Add("world");
    loggerComponent.write_lines(lines);
    // -> will write "hello\nworld";

```

### Configs parameters:

&nbsp;
```
    // initialization config (init_config() or initC for getting the config)    
    initC.get_resource_path(ResourcesManager.ResourceType.Directory, "directory");
    initC.get<bool>("stream_to_file");
    initC.get<bool>("add_date_to_file_name");
    initC.get<bool>("dont_write_if_file_exists");
    initC.get<bool>("replace_if_file_exists");
    initC.get<bool>("add_to_end_if_file_exists");
    initC.get<bool>("insert_new_routine_infos");
    initC.get<string>("base_file_name");
    initC.get<string>("date_time_format");
    initC.get<string>("file_extension");
    initC.get<bool>("add_header_line");
    initC.get<string>("header_line");
```
