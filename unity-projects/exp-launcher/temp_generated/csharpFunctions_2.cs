using System;
using System.Collections.Generic;
using UnityEngine;
 namespace Ex.CSharpFunctions { public class Log_data { 
static LoggerColumnsComponent logger = ExVR.Components().get_from_name<LoggerColumnsComponent>("Logger columns");public class standard { 

 public static object function(object input) {
object output = null;
 

var i = (IdAny)input;

// reaction time
// q1
// q2
// q3
// q4
// q5
// id image object
// id image context
// id image perpective

if(i.id == 0){ // word
	logger.update_column_value(0, Converter.to_string(i.value));
}else if(i.id == 1){ // questions
	var cn = logger.currentCondition.name;
	if(cn.Contains("q1")){
		logger.update_column_value(1, Converter.to_string(i.value));
	}else if(cn.Contains("q2")){
		logger.update_column_value(2, Converter.to_string(i.value));
	}else if(cn.Contains("q3")){
		logger.update_column_value(3, Converter.to_string(i.value));
	}else if(cn.Contains("q4")){
		logger.update_column_value(4, Converter.to_string(i.value));
	}else{
		logger.update_column_value(5, Converter.to_string(i.value));
	}

}else if(i.id == 2){ // word
	logger.update_column_value(6, Converter.to_string(i.value));
}else if(i.id == 3){ // foot/nofoot
	logger.update_column_value(7, Converter.to_string(i.value));
	logger.write_current_colums();
}

 
return output;
} 
}
 }
}