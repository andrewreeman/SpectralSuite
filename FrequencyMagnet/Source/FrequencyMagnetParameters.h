#pragma once

#include "../../shared/PluginParameters.h"

class FrequencyMagnetParameters : public PluginParameters {
public:
	FrequencyMagnetParameters(AudioProcessor* processor);

	float* getFreqValuePointer() const { return getRawParameterValue("freq"); }
	float* getWidthValuePointer() const { return getRawParameterValue("width"); }
	float* getBiasValuePointer() const {  return getRawParameterValue("bias"); }
};
