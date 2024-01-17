#pragma once


#include "../../shared/PluginParameters.h"
#include "../JuceLibraryCode/JuceHeader.h"

class NewThingParameters : public PluginParameters {
public:
    NewThingParameters(AudioProcessor* processor);
    
    float* getMixParameter() const {
        return getRawParameterValue("mix");
    }
    
};
