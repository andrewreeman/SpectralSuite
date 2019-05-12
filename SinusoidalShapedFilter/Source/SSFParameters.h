#pragma once

#include "../../shared/PluginParameters.h"

class SSFParameters : public PluginParameters {
public:
	SSFParameters(AudioProcessor* processor) : PluginParameters(processor) {
		String freqHertzLabel = "Hz";
		createAndAddParameter(std::make_unique<AudioParameterFloat>(
			"freq",
			"Frequency",
			NormalisableRange<float>(0.0f, 10.0f), 0.1f, "",
			AudioProcessorParameter::Category::genericParameter,
			[freqHertzLabel](float v, float maxLabelLength) { return String(v, 2) + freqHertzLabel; },
			[freqHertzLabel](String text) { return text.dropLastCharacters(freqHertzLabel.length()).getFloatValue(); }
		));
		
		String piLabel = " pi";
		createAndAddParameter(std::make_unique<AudioParameterFloat>(
			"phase",
			"Phase",
			NormalisableRange<float>(0.0f, 2.0f), 0.01f, "",
			AudioProcessorParameter::Category::genericParameter,
			[piLabel](float v, float maxLabelLength) { return String(v, 2) + piLabel; },
			[piLabel](String text) { return text.dropLastCharacters(piLabel.length()).getFloatValue(); }
		));		

		createAndAddParameter(std::make_unique<AudioParameterFloat>(
			"width",
			"Width",
			NormalisableRange<float>(0.0f, 1.0f), 0.8f, "",
			AudioProcessorParameter::Category::genericParameter
		));		
	}
};
