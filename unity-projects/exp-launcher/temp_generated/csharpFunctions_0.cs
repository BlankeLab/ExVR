using System;
using System.Collections.Generic;
using UnityEngine;
 namespace Ex.CSharpFunctions { public class Sound_controller { 

static AudioSourceComponent audioC = ExVR.Components().get_from_name<AudioSourceComponent>("Audio source");
static ParallelPortWriterComponent paraC = ExVR.Components().get_from_name<ParallelPortWriterComponent>("Parallel port writer");

static float startDistanceM = 1.0425f;
static float speedMMs = 0.00075f;
static float incrementMs = 190f;
static float condition = 1f;
static bool pulseSent= false;
static bool back = false;
static bool receiding = false;

static void process(IdAny idV){

	if(idV.id == 0){
		string conditionName = (string)idV.value;
		condition = Converter.to_float(	conditionName.Substring(1));

		back = (condition % 2) == 0;
		//receiding = (condition % 2) == 0;

		pulseSent = false;
	}else if(idV.id == 1){
		float currentTimeMs = Converter.to_float((double)idV.value);
		start_audio_tactile(currentTimeMs);
	}
}

static void start_audio_tactile(float currentTimeMs){	

	TransformValue tv = new TransformValue();
	if(!back){
		if(receiding){
	//		tv.position = new Vector3(0f,0.02f, -0.01f - speedMMs * currentTimeMs);
		}else{

		}
			tv.position = new Vector3(2f,0.02f, startDistanceM - speedMMs * currentTimeMs);
	}else{
		if(receiding){
//			tv.position = new Vector3(0f,0.02f, 0.01f + speedMMs * currentTimeMs);
		}else{

		}
			tv.position = new Vector3(2f,0.02f, -startDistanceM + speedMMs * currentTimeMs);
	}
	tv.scale = new Vector3(0.05f,0.05f,0.05f);
	audioC.current_config().set_transform("transform", tv);
	audioC.update_from_current_config();

	if(currentTimeMs > (condition*incrementMs) && !pulseSent){
		pulseSent = true;
		paraC.send_pulse(248);
	}
}public class T1 { 

 public static object function(object input) {
object output = null;
process((IdAny)input); 
return output;
} 
}public class T2 { 

 public static object function(object input) {
object output = null;
process((IdAny)input); 
return output;
} 
}public class T3 { 

 public static object function(object input) {
object output = null;
process((IdAny)input); 
return output;
} 
}public class T4 { 

 public static object function(object input) {
object output = null;
process((IdAny)input); 
return output;
} 
}public class T5 { 

 public static object function(object input) {
object output = null;
process((IdAny)input); 
return output;
} 
}public class T6 { 

 public static object function(object input) {
object output = null;
process((IdAny)input); 
return output;
} 
}
 }
}