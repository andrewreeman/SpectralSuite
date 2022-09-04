#pragma once

#include "../../shared/PluginParameters.h"

class SpectralGateParameters : public PluginParameters {
public:	
	SpectralGateParameters(AudioProcessor* processor);

	float* getCutOffValuePointer() const { return getRawParameterValue("cutoff");  }
	float* getBalanceValuePointer() const { return getRawParameterValue("balance"); }
    float* getTiltPointer() const { return getRawParameterValue("tilt"); }
    bool getTiltEnabled() const;
};
