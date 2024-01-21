#pragma once


#include "../../shared/PluginParameters.h"
#include "../JuceLibraryCode/JuceHeader.h"

class NEWNAMEParameters : public PluginParameters {
public:
    NEWNAMEParameters(AudioProcessor* processor);
    
    float* getMixParameter() const {
        return getRawParameterValue("mix");
    }
    
};
