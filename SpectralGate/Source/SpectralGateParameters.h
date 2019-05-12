#pragma once

#include "../../shared/PluginParameters.h"

class SpectralGateParameters : public PluginParameters {
public:	
	SpectralGateParameters(AudioProcessor* processor) : PluginParameters(processor) {
		const String dbSuffix = " dB";	
		createAndAddParameter(std::make_unique<AudioParameterFloat>(
			"cutoff",
			"Cutoff",
			NormalisableRange<float>(0.0f, 1.0f), 
			0.6f, "",
			AudioProcessorParameter::Category::genericParameter,
			[dbSuffix](float value, float maxLabelLength) { 
				return String(Decibels::gainToDecibels(value), 0) + dbSuffix; 
			},
			[dbSuffix](String text) {
				return  Decibels::decibelsToGain(text.dropLastCharacters(dbSuffix.length()).getFloatValue()); 
			}
		));
		
		createAndAddParameter(std::make_unique<AudioParameterFloat>(
			"balance",
			"Weak/Strong Balance",
			NormalisableRange<float>(0.0f, 1.0f), 0.7f, "",
			AudioProcessorParameter::Category::genericParameter
		));		
	}
};
