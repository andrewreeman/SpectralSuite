/*
  ==============================================================================

    FrequencyShiftPluginParameters.cpp
    Created: 12 May 2019 8:23:21am
    Author:  rem_d

  ==============================================================================
*/

#include "FrequencyShiftPluginParameters.h"

FrequencyShiftPluginParameters::FrequencyShiftPluginParameters(AudioProcessor * processor) : PluginParameters(processor) {

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

void FrequencyShiftPluginParameters::updateValue(Slider* frequencyShiftSlider, double valueToUpdateTo)
{
	AudioParameterFloat* shiftParam = (AudioParameterFloat*)getParameter("shift");		
	double currentValue = valueToUpdateTo;	

	const AudioParameterFloat* lowestValueParam = (AudioParameterFloat*)getParameter("shiftMinRange");
	const AudioParameterFloat* highestValueParam = (AudioParameterFloat*)getParameter("shiftMaxRange");

	const float lowestValue = lowestValueParam->get();
	const float highestValue = highestValueParam->get();

	if (currentValue < lowestValue) {
		currentValue = lowestValue;
	}

	if (currentValue > highestValue) {
		currentValue = highestValue;
	}

	shiftParam->range.start = lowestValue;
	shiftParam->range.end = highestValue;
	frequencyShiftSlider->setRange(shiftParam->range.start, shiftParam->range.end, shiftParam->range.interval);
	shiftParam->setValueNotifyingHost(shiftParam->convertTo0to1(currentValue));
}
