#pragma once

#include "../../shared/PluginParameters.h"

class FrequencyShiftPluginParameters : public PluginParameters {
public:
	FrequencyShiftPluginParameters(AudioProcessor* processor);
	void updateValue(Slider* frequencyShiftSlider, double valueToUpdateTo);
};
