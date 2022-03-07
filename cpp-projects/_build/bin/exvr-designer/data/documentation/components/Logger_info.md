Description:
------------

&nbsp;

This component provides an interface for writing inside a logging file.
The logging file will be created (if necessary) at the beginning of the experiment. 
The content can be streamed during the experiment of written in a block a the end.

&nbsp;

Parameters:
------------

&nbsp;
### Initialization:

Name | Id | Type | Description |
-----| ---| ---- | ----------- |
**Directory to use**| "directory" | Directory resource | for specifying the directory parent of the logging file. (**MANDATORY**) |
**Stream to file**| "stream_to_file" | boolean | if selected the content will be streamed to the file, elese it will be stored to a list and written at the end of the experiment |
**Don't write if file already exists**| "dont_write_if_file_exists" | boolean | don't write anything in the loggig file if this one already exists at the begging of the experiment. |
**Insert info at each new routine**| "insert_new_routine_infos" | boolean | [TODO] ... |
**Replace if file exists**| "insert_new_routine_infos" | boolean | if the logging file already exists, delete it and creates a new one |
**Add to end of file if exists**| "insert_new_routine_infos" | boolean | if the logging file already exists on the disk, append content to the end |
**Logging file name**| "base_file_name" | string | base name of the logging file |
**Add current date to name**| "add_date_to_file_name" | boolean | add current date at the end of the name of the logging file following **[Date format]** format |
**Date format**| "date_time_format" | boolean | date format used in the name of the logging file |
**File extension**| "file_extension" | string | extension of the logging file to be used |
**Add header line**| "add_header_line" | boolean | add an header line just after the creation of the logging file |
**Header line**| "header_line" | string | header line to be added if "Add header line" is checked |

&nbsp;
