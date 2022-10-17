Description:
------------

&nbsp;

This component creates a procedural cylinder model.
You can move it, change its scale, color and transparency.

&nbsp;

Parameters:
------------

&nbsp;
### Initialization config: 

&nbsp;

Name | Id | Type | Description |
-----| ---| ---- | ----------- |
**Number sides**| "nb_sides" | int | the number of sizes for the cylinder to be generated |
**Radius**| "radius" | float | radius height of the cylinder|
**Length**| "length" | float | height of the cylinder|
**Is tranparent?**| "tranparent" | boolean | if true, the alpha channel of the color will be used for sepcifying the transparency of the model|
**Init transform**| "init_transform" | Transform | Init transform to be applied|
**Do not apply**| "init_transform_do_not_apply" | boolean | If checked, the init transform will not be applied|

&nbsp;
&nbsp;

### Condition config:

&nbsp;

Name | Id | Type | Description |
-----| ---| ---- | ----------- |
**Transform**| "transform" | Transform | Config transform to be applied|
**Do not apply**| "transform_do_not_apply" | boolean | If checked, the config transform will not be applied|
**Color**| "color" | Color | model material color| 
**Metallic**| "metallic" | float | model material metallic value |
**Smoothness**| "smoothness" | float | model material smoothness value|

&nbsp;


