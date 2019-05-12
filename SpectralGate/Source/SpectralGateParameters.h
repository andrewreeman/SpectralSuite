#pragma once

#include "../../shared/PluginParameters.h"

class SpectralGateParameters : public PluginParameters {
public:
	SpectralGateParameters(AudioProcessor* processor) : PluginParameters(processor) {
		createAndAddParameter(std::make_unique<AudioParameterFloat>(
			"cutoff",
			"Cutoff",
			NormalisableRange<float>(0.0f, 1.0f), 0.6f, "",
			AudioProcessorParameter::Category::genericParameter
		));
		
		createAndAddParameter(std::make_unique<AudioParameterFloat>(
			"balance",
			"Weak/Strong Balance",
			NormalisableRange<float>(0.0f, 1.0f), 0.7f, "",
			AudioProcessorParameter::Category::genericParameter
		));		
	}
};
