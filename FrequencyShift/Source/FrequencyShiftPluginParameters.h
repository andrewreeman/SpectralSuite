#pragma once

#include "../../shared/PluginParameters.h"

class FrequencyShiftPluginParameters : public PluginParameters {
public:
	FrequencyShiftPluginParameters(AudioProcessor* processor) : PluginParameters(processor) {
		
		const String shiftHertzLabel = " Hz";
		const float minRangeDefault = -500.0f;
		const float maxRangeDefault = 500.0f;
		const float increment = 10.0;

		createAndAddParameter(std::make_unique<AudioParameterFloat>(
			"shiftMinRange",
			"Frequency shift minimum range",
			NormalisableRange<float>(-20000, -10, increment), minRangeDefault, "",
			AudioProcessorParameter::Category::genericParameter,
			[shiftHertzLabel](float v, float maxLabelLength) { return String(v, 2) + shiftHertzLabel; },
			[shiftHertzLabel](String text) { return text.dropLastCharacters(shiftHertzLabel.length()).getFloatValue(); }
		));

		createAndAddParameter(std::make_unique<AudioParameterFloat>(
			"shiftMaxRange",
			"Frequency shift maximum range",
			NormalisableRange<float>(10, 20000, increment), maxRangeDefault, "",
			AudioProcessorParameter::Category::genericParameter,
			[shiftHertzLabel](float v, float maxLabelLength) { return String(v, 2) + shiftHertzLabel; },
			[shiftHertzLabel](String text) { return text.dropLastCharacters(shiftHertzLabel.length()).getFloatValue(); }
		));

		createAndAddParameter(std::make_unique<AudioParameterFloat>(
			"shift",
			"Frequency shift",
			NormalisableRange<float>(minRangeDefault, maxRangeDefault, increment), 0.0f, "",
			AudioProcessorParameter::Category::genericParameter,
			[shiftHertzLabel](float v, float maxLabelLength) { return String(v, 2) + shiftHertzLabel; },
			[shiftHertzLabel](String text) { return text.dropLastCharacters(shiftHertzLabel.length()).getFloatValue(); }
		));
	}
};
