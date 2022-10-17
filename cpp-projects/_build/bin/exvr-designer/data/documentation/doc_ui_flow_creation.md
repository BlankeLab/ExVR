
Previous page: [Index](doc_general.md)
<br />

# Flow creation

![Image](images/img_ui_flow_creation_00.png "")

<br />
<br />
<br />
The flow creation panel is the place where you can define the structure of your experiment by placing elements on the arrow.
You have 3 differents available elements:

For adding a new element, first you need to chosse a node:

![Image](images/img_ui_flow_creation_01.png "")
<br />


Then click on the black ball, it will display 2 differents choices:

![Image](images/img_ui_flow_creation_02.png "")
<br />

You can either validate a choice by clicking on the corresponding button or canceling it by clicking anywhere on the grey part.

![Image](images/img_ui_flow_creation_03.png "")

<br />

Here a [routine](doc_ui_element_selection.md#routine) has been created.
The [routine](doc_ui_element_selection.md#routine) is the base container used in experiments, it contains conditions which number depends from its position inside loops. When created outside a loop they only contains a "default" condition.

<br />

You can easily move any [routine](doc_ui_element_selection.md#routine) by selecting it: 

![Image](images/img_ui_flow_creation_04.png "")

<br />

Then clicking on the right or left arrow: 

![Image](images/img_ui_flow_creation_05.png "")
![Image](images/img_ui_flow_creation_06.png "")

The cross button can remove any element.

<br />

If you create a [loop](doc_ui_element_selection.md#loop):

![Image](images/img_ui_flow_creation_07.png "")

<br />

You can move either the [routine](doc_ui_element_selection.md#routine) to put it inside, or extending the [loop](doc_ui_element_selection.md#loop): my moving its left or right node to englobe the [routine](doc_ui_element_selection.md#routine).

![Image](images/img_ui_flow_creation_08.png "")
![Image](images/img_ui_flow_creation_09.png "")

<br />

You can move a node [loop](doc_ui_element_selection.md#loop): to englobe another one, but [loops](doc_ui_element_selection.md#loop) nodes cannot overlap.

![Image](images/img_ui_flow_creation_10.png "")
![Image](images/img_ui_flow_creation_11.png "")
![Image](images/img_ui_flow_creation_12.png "")

<br />

It's possible to transform a [routine](doc_ui_element_selection.md#routine) into a [randomizer](doc_ui_element_selection.md#randomizer).

![Image](images/img_ui_flow_creation_13.png "")

<br />

It behaves exactly like an empty [routine](doc_ui_element_selection.md#routine) and it is used mostly for creating complexes randomizations or as an indicator to separate sections in the flow.

![Image](images/img_ui_flow_creation_15.png "")

<br />

If you right click on a flow element, a panel will be displayed with several options depending the element type:

![Image](images/img_ui_flow_creation_16.png "")

* All 
	* **Unselect:** (only if current element is selected): unselect the current element (the buttons for moving or removing the element will disappear and the [Element selection](doc_ui_element_selection.md) panel will be updated accordingly.)
	* **Duplicate:** will copy the element with exactly the same content (same actions for [routines](doc_ui_element_selection.md#routine) and same sets for [loops](doc_ui_element_selection.md#loop)) and change its name by adding "(copy)" at the end.
	* **Remove:** will erase the element
	* **Remove everything before:** will remove every elements in the flow before the selected one (a confirmation will be asked)
	* **Remove everything after:** will remove every elements in the flow after the selected one (a confirmation will be asked)
	* **Move to:** will move the selected element to the provided id position in the flow (see [no_selection_panel](doc_ui_element_selection.md#no_selection) for looking at the current if for each node)
* Only routines
	* **Clean current condition**: will delete every action and visual scripting node from the currently selected condition
	* **Clean all conditions**: will clean every condition 
	* **Set duration for all conditions**: will open a dialog box for choosing a new duration that will be applied to all conditions



<br />

If something is written in the "Informations" panel of an element (see: [Element selection](doc_ui_element_selection.md)), the moment you hover it with mouse, a ToolTip text will be displayed showing this text.

![Image](images/img_ui_flow_creation_17.png "")

<br />

You can zoom using this bar or adapt the view to the scene by clicking on the button.

![Image](images/img_ui_flow_creation_14.png "")

<br />

Next page: [Element selection](doc_ui_element_selection.md)
