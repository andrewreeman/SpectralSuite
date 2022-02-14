#include "SSFParameters.h"

SSFParameters::SSFParameters(AudioProcessor * processor) : PluginParameters(processor) {
	const String freqHertzLabel = "Hz";
	createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"freq",
		"Frequency",
		NormalisableRange<float>(0.0f, 10.0f), 0.1f, "",
		AudioProcessorParameter::Category::genericParameter,
		[freqHertzLabel](float v, int) { return String(v, 2) + freqHertzLabel; },
		[freqHertzLabel](const String& text) { return text.dropLastCharacters(freqHertzLabel.length()).getFloatValue(); }
	));

//	const String piLabel = " pi";
	createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"phase",
		"Phase",
		NormalisableRange<float>(0.0f, 1.0f), 0.01f, "",
		AudioProcessorParameter::Category::genericParameter//,
//		[piLabel](float v, int) { return String(v, 2) + piLabel; },
//		[piLabel](const String& text) { return text.dropLastCharacters(piLabel.length()).getFloatValue(); }
	));

	createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"width",
		"Width",
		NormalisableRange<float>(0.0f, 1.0f), 0.8f, "",
		AudioProcessorParameter::Category::genericParameter
	));
    
}



