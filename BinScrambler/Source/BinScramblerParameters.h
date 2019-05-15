#pragma once

#include "../../shared/PluginParameters.h"

class BinScramblerParameters : public PluginParameters {
public:
	BinScramblerParameters(AudioProcessor* processor);

	float* getScrambleValuePointer() const { return getRawParameterValue("scramble"); }
	float* getScatterValuePointer() const { return getRawParameterValue("scatter");	}
	float* getRateValuePointer() const { return getRawParameterValue("rate"); }
};
