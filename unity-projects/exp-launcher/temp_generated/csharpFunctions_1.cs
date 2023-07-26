using System;
using System.Collections.Generic;
using UnityEngine;
 namespace Ex.CSharpFunctions { public class Move_slide { 
static int countErrors = 0;static ImageViewerComponent imageLeft = ExVR.Components().get_from_name<ImageViewerComponent>("HalfSlide1");
static ImageViewerComponent imageRight = ExVR.Components().get_from_name<ImageViewerComponent>("HalfSlide2");
static TextViewerComponent textV = ExVR.Components().get_from_name<TextViewerComponent>("Text viewer");public class standard{

 public static object function(object input) {
object output = null;
try{double expTime = (double)input;
float t = Converter.to_float(expTime * 0.0005);
imageLeft.set_pivot(new Vector2(1.0f +t, 0.5f));
imageRight.set_pivot(new Vector2(0.0f -t, 0.5f));

if(expTime > 10000.0){
	double size = 1.0 - (expTime-10000.0)/2000.0;
	if(size < 0){
		size = 0;
	}
	textV.set_scale_factor(Converter.to_float(size));
}else if(expTime > 5000.0){

	double size = (expTime-5000.0)/2000.0;
	if(size > 1.3){
		size = 1.3;
	}
	textV.set_scale_factor(Converter.to_float(size));

}else{
	textV.set_scale_factor(0.0f);
}
}catch(System.Exception ex){
CSharpFunctionComponent.display_static_exception(countErrors++,ex);}
return output;
}
}
 }
}