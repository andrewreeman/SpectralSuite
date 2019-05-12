#include "FrequencyMagnetParameters.h"

FrequencyMagnetParameters::FrequencyMagnetParameters(AudioProcessor * processor) : PluginParameters(processor) {

	const String freqHertzLabel = "Hz";
	createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"freq",
		"Frequency",
		NormalisableRange<float>(20.0f, 2000.0f), 800.0f, "",
		AudioProcessorParameter::Category::genericParameter,
		[freqHertzLabel](float v, int) { return String(v, 2) + freqHertzLabel; },
		[freqHertzLabel](const String& text) { return text.dropLastCharacters(freqHertzLabel.length()).getFloatValue(); }
	));
	
	createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"bias",
		"Width bias",
		NormalisableRange<float>(0.0f, 1.0f), 0.01f, "",
		AudioProcessorParameter::Category::genericParameter
	));

	createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"width",
		"Width",
		NormalisableRange<float>(0.0f, 1.0f), 0.5f, "",
		AudioProcessorParameter::Category::genericParameter
	));
}
