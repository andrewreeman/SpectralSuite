#pragma once

#include "../../shared/PluginParameters.h"
#include "FreqSlider.h"

class FrequencyMagnetParameters : public PluginParameters {
public:
	FrequencyMagnetParameters(AudioProcessor* processor);

	float* getFreqValuePointer() const { return getRawParameterValue("freq"); }
	float* getWidthValuePointer() const { return getRawParameterValue("width"); }
	float* getBiasValuePointer() const {  return getRawParameterValue("bias"); }
	Value getUseLegacyLogicValue() const { return getParameterAsValue("useLegacyHighFrequencyShift"); }
 
    void setFrequency(float frequency);
    void setWidth(float width);
    void updateValue(FreqSlider* frequencyMagSlider, double valueToUpdateTo);
};
