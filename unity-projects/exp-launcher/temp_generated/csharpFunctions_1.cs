using System;
using System.Collections.Generic;
using UnityEngine;
 namespace Ex.CSharpFunctions { public class Choose__image_NEW { 
static int countErrors = 0;static string readrandim = ExVR.Resources().get_text_file_data("order_images").content;
static string[] imagesorder = Ex.Text.split_lines(readrandim);
static string order = ExVR.Resources().get_text_file_data("order_bloc_1").content;
static string[] indxcue = Ex.Text.split_lines(order);



public class standard{

 public static object function(object input) {
object output = null;
try{int trial = Converter.to_int(input);
int imageId = Converter.to_int( indxcue[trial])+1;
var orderim = imagesorder[trial];

	ExVR.Log().message("trial " + trial + " " +  "imageid" + imageId + " "+ "order" + orderim);


var line1 = string.Format("#{0}-{1}_#{0}-{2}", imageId, orderim[0], orderim[1]);
var line2 = string.Format("#{0}-{1}_#{0}-{2}", imageId, orderim[2], orderim[3]);
var buttonText = Ex.Text.join_with_new_line(new string[]{line1,line2});


var buttonC = ExVR.Components().get_first<ButtonsUIComponent>();
buttonC.set_text(buttonText);

// TODO improve output for log
output = orderim;
}catch(System.Exception ex){
CSharpFunctionComponent.display_static_exception(countErrors++,ex);}
return output;
}
}
 }
}