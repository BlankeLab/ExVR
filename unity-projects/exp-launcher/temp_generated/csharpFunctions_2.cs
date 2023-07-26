using System;
using System.Collections.Generic;
using UnityEngine;
 namespace Ex.CSharpFunctions { public class Downsize__plane { 
static int countErrors = 0;public class standard{

 public static object function(object input) {
object output = null;
try{var time = Converter.to<float>((double)input);
time *= 0.01f;

if(time < 10.0f){	
	output = new Vector3(10.0f- time,10.0f- time,1);
}else{
		output = new Vector3(0f,0f,1);
}


}catch(System.Exception ex){
CSharpFunctionComponent.display_static_exception(countErrors++,ex);}
return output;
}
}
 }
}