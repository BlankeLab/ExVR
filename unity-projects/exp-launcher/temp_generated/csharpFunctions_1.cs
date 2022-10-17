using System;
using System.Collections.Generic;
using UnityEngine;
 namespace Ex.CSharpFunctions { public class Choose__image { 
static int countErrors = 0;


public class standard{

 public static object function(object input) {
object output = null;
try{

var split = Ex.Text.split((string)input, "-");
var imageId = split[0];
var order = split[1];

var line1 = string.Format("#{0}-{1}_#{0}-{2}", imageId, order[0], order[1]);
var line2 = string.Format("#{0}-{1}_#{0}-{2}", imageId, order[2], order[3]);
var buttonText = Ex.Text.join_with_new_line(new string[]{line1,line2});


var buttonC = ExVR.Components().get_first<ButtonsUIComponent>();
buttonC.set_text(buttonText);
}catch(System.Exception ex){
CSharpFunctionComponent.display_static_exception(countErrors++,ex);}
return output;
}
}
 }
}