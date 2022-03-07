

### Content:

&nbsp;
  
```csharp
// system
using System;
using System.Collections.Generic;

// unity
using UnityEngine;

namespace Ex{

    public class AudioSourceComponent : ExComponent{

        public GameObject audioSourceGO = null;
        public AudioSource audioSource = null;

	// Update components with current config parameters 
	// (Call it if you modify the component config using scripts)
        public override void update_from_current_config();
        
	// Start the sound
        public void start_sound();
		
	// Stop the sound
        public void stop_sound();

	// Pause the sound
        public override void pause();
		
	// Unpause the sound
        public override void play();

	// Set current play time (seconds)
        public void set_time(float time);
    }
}
```


### Samples:

```csharp

	// retrieve component 
	var audioComponent = (AudioSourceComponent)get("audio_souce_component_name");
	// or 
	var audioComponent = get<AudioSourceComponent>("audio_souce_component_name");

	// modify config
	// # enable spatialization
	audioComponent.current_config().set<bool>("spatialized", true);
	// # modify volume
	audioComponent.current_config().set<float>("volume", 0.6f);

	// update component from modified config (mandatory to apply changes)
	audioComponent.update_from_current_config();


```
