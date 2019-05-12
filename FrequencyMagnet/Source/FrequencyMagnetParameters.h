#pragma once

#include "../../shared/PluginParameters.h"

class FrequencyMagnetParameters : public PluginParameters {
public:
	FrequencyMagnetParameters(AudioProcessor* processor) : PluginParameters(processor) {
		String freqHertzLabel = "Hz";
		createAndAddParameter(std::make_unique<AudioParameterFloat>(
			"freq",
			"Frequency",
			NormalisableRange<float>(20.0f, 2000.0f), 800.0f, "",
			AudioProcessorParameter::Category::genericParameter,
			[freqHertzLabel](float v, float maxLabelLength) { return String(v, 2) + freqHertzLabel; },
			[freqHertzLabel](String text) { return text.dropLastCharacters(freqHertzLabel.length()).getFloatValue(); }
		));

		//m_freq = valueTreeState->getRawParameterValue("freq");

		createAndAddParameter(std::make_unique<AudioParameterFloat>(
			"bias",
			"Width bias",
			NormalisableRange<float>(0.0f, 1.0f), 0.01f, "",
			AudioProcessorParameter::Category::genericParameter
			));

		//m_bias = valueTreeState->getRawParameterValue("bias");

		createAndAddParameter(std::make_unique<AudioParameterFloat>(
			"width",
			"Width",
			NormalisableRange<float>(0.0f, 1.0f), 0.5f, "",
			AudioProcessorParameter::Category::genericParameter
			));

		//m_width = valueTreeState->getRawParameterValue("width");
	}
};
