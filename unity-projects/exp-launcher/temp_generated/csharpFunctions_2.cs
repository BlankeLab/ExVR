using System;
using System.Collections.Generic;
using UnityEngine;
 namespace Ex.CSharpFunctions { public class Log_data { 
static int countErrors = 0;static LoggerColumnsComponent logger = ExVR.Components().get_from_name<LoggerColumnsComponent>("Logger columns");public class standard{

 public static object function(object input) {
object output = null;
try{ 
var i = (IdAny)input;

if(i.id == 0){ // word
	logger.update_column_value(0, Converter.to_string(i.value));
}else if(i.id == 1){ // reaction time bip
	logger.update_column_value(1, Converter.to_string(i.value));
}else if(i.id == 2){ // questions
	var cn = logger.currentCondition.name;
	if(cn.Contains("q1")){
		logger.update_column_value(2, Converter.to_string(i.value));
	}else if(cn.Contains("q2")){
		logger.update_column_value(3, Converter.to_string(i.value));
	}else if(cn.Contains("q3")){
		logger.update_column_value(4, Converter.to_string(i.value));
	}else if(cn.Contains("q4")){
		logger.update_column_value(5, Converter.to_string(i.value));
	}
}else if(i.id == 3){ // recog
    logger.update_column_value(6, Converter.to_string(i.value));
}else if(i.id == 4){ // q5
    logger.update_column_value(7, Converter.to_string(i.value));
}else if(i.id == 5){ // word
	logger.update_column_value(8, Converter.to_string(i.value));
}else if(i.id == 6){ // foot/nofoot
	logger.update_column_value(9, Converter.to_string(i.value));
}else if(i.id == 7){ // confidence
	logger.update_column_value(10, Converter.to_string(i.value));
}else if(i.id == 19){ // write current
		logger.write_current_colums();
}else if(i.id == 20){ // reset values
		var resetV = Converter.to_string(i.value);
		logger.update_column_value(0, resetV);
		logger.update_column_value(1, resetV);
		logger.update_column_value(2, resetV);
		logger.update_column_value(3, resetV);
		logger.update_column_value(4, resetV);
		logger.update_column_value(5, resetV);
		logger.update_column_value(6, resetV);
		logger.update_column_value(7, resetV);
		logger.update_column_value(8, resetV);
		logger.update_column_value(9, resetV);
		logger.update_column_value(10, resetV);
}
}catch(System.Exception ex){
CSharpFunctionComponent.display_static_exception(countErrors++,ex);}
return output;
}
}
 }
}