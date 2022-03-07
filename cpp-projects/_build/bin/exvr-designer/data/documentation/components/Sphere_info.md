Description:
------------

&nbsp;

This component creates a procedural sphere model.
You can move it, change its scale, color and transparency.

&nbsp;

Parameters:
------------

&nbsp;
### Initialization:

Name | Id | Type | Description |
-----| ---| ---- | ----------- |
**Sphere radius**| "radius" | float | radius of the sphere |
**Long nb triangles**| "nb_log" | int | number of triangles for the longitude |
**Lat nb triangles**| "nb_lat" | int | number of triangles for the latitude |
**Is tranparent?**| "tranparent" | boolean | if true, the alpha channel of the color will be used for sepcifying the transparency of the model|

&nbsp;

### Condition:

Name | Id | Type | Description |
-----| ---| ---- | ----------- |
**Transform**| "transform" | Transform | model transform |
**Color**| "color" | Color | model material color| 
**Metallic**| "metallic" | float | model material metallic value |
**Smoothness**| "smoothness" | float | model material smoothness value|

&nbsp;


