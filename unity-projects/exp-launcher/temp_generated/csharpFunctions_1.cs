using System;
using System.Collections.Generic;
using UnityEngine;
 namespace Ex.CSharpFunctions { public class Select_image { 


static ImageViewerComponent selector = ExVR.Components().get_from_name<ImageViewerComponent>("Selector");
static ImageViewerComponent im1 = ExVR.Components().get_from_name<ImageViewerComponent>("Image1");
static ImageViewerComponent im2 = ExVR.Components().get_from_name<ImageViewerComponent>("Image2");
static ImageViewerComponent im3 = ExVR.Components().get_from_name<ImageViewerComponent>("Image3");
static ImageViewerComponent im4 = ExVR.Components().get_from_name<ImageViewerComponent>("Image4");

static List<Vector2> pivots = new List<Vector2>(new Vector2[]{
	new Vector2(1.025f,-0.025f),
	new Vector2(-0.025f,-0.025f),
	new Vector2(1.025f,1.025f),
	new Vector2(-0.025f,1.025f)
});public class standard { 
static int currentId = 0;
 public static object function(object input) {
object output = null;


var i = (IdAny)input;

if(i.id == 0){
	var rnd = new System.Random();
	currentId  = rnd.Next(0, 4);  
}else if(i.id == 1){
	if(currentId == 0){
		currentId = 3;
	}else{
		currentId = (currentId - 1)%4;
	}
}else {
   currentId = (currentId + 1)%4;
}


selector.current_config().set_vector2("pivot", pivots[currentId]);
selector.update_from_current_config();

output = currentId; 
return output;
} 
}
 }
}