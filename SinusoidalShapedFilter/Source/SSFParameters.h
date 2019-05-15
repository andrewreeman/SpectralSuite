#pragma once

#include "../../shared/PluginParameters.h"

class SSFParameters : public PluginParameters {
public:
	SSFParameters(AudioProcessor* processor);
	
	float* getFreqValuePointer() const { return getRawParameterValue("freq"); }
	float* getPhaseValuePointer() const { return getRawParameterValue("phase"); }
	float* getWidthValuePointer() const { return getRawParameterValue("width"); }
};