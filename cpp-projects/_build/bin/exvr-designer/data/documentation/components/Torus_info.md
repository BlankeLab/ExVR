Description:
------------

&nbsp;

This component creates a procedural torus model.
You can move it, change its scale, color and transparency.

&nbsp;

Parameters:
------------

&nbsp;
### Initialization config: 

&nbsp;

Name | Id | Type | Description |
-----| ---| ---- | ----------- |
**Outer radius**| "outer_radius" | float | outer radius of the torus |
**Inner radius**| "inner_radius" | float | inner radius of the torus |
**Sides count**| "sides_count" | int | number of sides |
**Rings count**| "rings_count" | int | number of rings|
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


