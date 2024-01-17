#pragma once


#include "../../shared/PluginParameters.h"
#include "../JuceLibraryCode/JuceHeader.h"

class BaseParameters : public PluginParameters {
public:
    BaseParameters(AudioProcessor* processor);
    
    float* getMixParameter() const {
        return getRawParameterValue("mix");
    }
    
};
