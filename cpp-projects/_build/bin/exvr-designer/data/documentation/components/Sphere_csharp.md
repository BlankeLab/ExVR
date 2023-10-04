

### Content:

&nbsp;

```
// unity
using UnityEngine;

namespace Ex{
    public class SphereComponent : ExComponent{
        public override void update_from_current_config();
    }
}
```


### Samples:

```

    // retrieve component 
    var modelComponent = (SphereComponent)get("sphere_component_name");
    // or 
    var modelComponent = get<SphereComponent>("sphere_component_name");
    
    // change model current condition parameters
    // # retrieve config
    var modelConfig = modelComponent.current_config();    
    
    // # change material color
    modelConfig.set_color("color", Color.blue);
    // # change material metallic value
    modelConfig.set<float>("metallic", 0.5f);
    // # change material smoothness value
    modelConfig.set<float>("smoothness", 0.2f);   
    
    // you can move/resize the model using:
    // # a list of 3 Vector3 (postion,rotation,scale)
    List<Vector3> listV = new List<Vector3>();
    listV.Add(new Vector3(0,2f,0));  // position
    listV.Add(new Vector3(30f,0,0)); // rotation
    listV.Add(Vector3.one);          // scale
    modelConfig.set_transform("transform", listV);
    
    // # with the TransformValue class
    TransformValue trV = new TransformValue();
    trV.position = new Vector3(0,2f,0);
    trV.rotation = Quaternion.Euler(new Vector3(30f,0,0));
    trV.scale    = Vector3.one;
    modelConfig.set_transform("transform", trV);
    
    // # with a Transform
    Transform tr = someGameObject.transform;
    modelConfig.set_transform("transform", tr);

    // apply changes
    modelComponent.update_from_current_config();
    

```

### Configs parameters:

&nbsp;
```
    // initialization config (init_config() or initC for getting the config)    
    initC.get<float>("radius");
    initC.get<int>("nb_log");
    initC.get<int>("nb_lat");
    initC.get<bool>("tranparent");

    // condition config (current_config() or currentC for getting the config)
    TransformValue trV = currentC.get_transform_value("transform");
    currentC.get_color("color");
    currentC.get<float>("metallic");
    currentC.get<float>("smoothness");
```
