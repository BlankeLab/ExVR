using System;
using System.Collections.Generic;
using UnityEngine;
 namespace Ex.CSharpFunctions { public class Choose__cue { 
static int countErrors = 0;

static string content = ExVR.Resources().get_text_file_data("words_fr_pilot").content;
static string[] words = Ex.Text.split_lines(content);
	
public class standard{

 public static object function(object input) {
object output = null;
try{int id = Converter.to_int(input)-1;
output = "error";
if(id < words.Length){
   output = words[id];
	ExVR.Log().message("id " + id + " " + output + " " + words.Length);
}else{
	ExVR.Log().error("id " + id + " " + output + " " + words.Length);
}




}catch(System.Exception ex){
CSharpFunctionComponent.display_static_exception(countErrors++,ex);}
return output;
}
}
 }
}