using System;
using System.Collections.Generic;
using UnityEngine;
 namespace Ex.CSharpFunctions { public class Alterate_calibration { 
static int countErrors = 0;

//static Vector3 calibInitLocalPosition = ExVR.Display().cameras().get_camera_rig_transform().Find("Cameras").localPosition;
static Vector3 calibInitLocalPosition = ExVR.Display().cameras().get_calibration_transform().localPosition;public class standard{

 public static object function(object input) {
object output = null;
try{double factor = Converter.to<double>(input);
double value = 1.0;
double calibrationOffset = factor * value;

//var t = ExVR.Components().get_from_name<CameraTargetComponent>("Camera target");


ExVR.Display().cameras().get_calibration_transform().localPosition = calibInitLocalPosition + new Vector3(0,(float)calibrationOffset,0);
ExVR.Display().cameras().get_camera_rig_transform().Find("Cameras").localPosition =  new Vector3(0,(float)calibrationOffset,0);

//t.cameraOffsetPosition = new Vector3(0,(float)calibrationOffset,0);
}catch(System.Exception ex){
CSharpFunctionComponent.display_static_exception(countErrors++,ex);}
return output;
}
}
 }
}