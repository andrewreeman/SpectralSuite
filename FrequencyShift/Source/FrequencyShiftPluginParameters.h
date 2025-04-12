#pragma once

#include "../../shared/PluginParameters.h"

class FrequencyShiftPluginParameters final : public PluginParameters {
public:
	explicit FrequencyShiftPluginParameters(AudioProcessor* processor);
	void updateValue(Slider* frequencyShiftSlider, double valueToUpdateTo) const;
	
	float* getShiftValuePointer() const { return getRawParameterValue("shift"); }
};
