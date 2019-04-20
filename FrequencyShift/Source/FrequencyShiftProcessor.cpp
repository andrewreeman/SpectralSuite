/*
  ==============================================================================

    FrequencyShiftProcesor.cpp
    Created: 31 Dec 2018 11:48:29am
    Author:  rem_d

  ==============================================================================
*/

#include "FrequencyShiftProcessor.h"



void FrequencyShiftProcessor::createParameters(AudioProcessorValueTreeState* valueTreeState)
{	
	const String shiftHertzLabel = " Hz";		
	const float minRangeDefault = -500.0f;
	const float maxRangeDefault = 500.0f;
	const float increment = 10.0;

	valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"shiftMinRange",
		"Frequency shift minimum range",
		NormalisableRange<float>(-20000, -10, increment), minRangeDefault, "",
		AudioProcessorParameter::Category::genericParameter,
		[shiftHertzLabel](float v, float maxLabelLength) { return String(v, 2) + shiftHertzLabel; },
		[shiftHertzLabel](String text) { return text.dropLastCharacters(shiftHertzLabel.length()).getFloatValue(); }
	));

	valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"shiftMaxRange",
		"Frequency shift maximum range",
		NormalisableRange<float>(10, 20000, increment), maxRangeDefault, "",
		AudioProcessorParameter::Category::genericParameter,
		[shiftHertzLabel](float v, float maxLabelLength) { return String(v, 2) + shiftHertzLabel; },
		[shiftHertzLabel](String text) { return text.dropLastCharacters(shiftHertzLabel.length()).getFloatValue(); }
	));

	valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"shift",
		"Frequency shift",
		NormalisableRange<float>(minRangeDefault, maxRangeDefault, increment), 0.0f, "",
		AudioProcessorParameter::Category::genericParameter,
		[shiftHertzLabel](float v, float maxLabelLength) { return String(v, 2) + shiftHertzLabel; },
		[shiftHertzLabel](String text) { return text.dropLastCharacters(shiftHertzLabel.length()).getFloatValue(); }
	));

	m_shift = valueTreeState->getRawParameterValue("shift");			
}

void FrequencyShiftProcessor::prepareProcess(int spectralProcessIndex)
{
	auto shifter = (frequencyShifter*)m_spectralProcess[spectralProcessIndex];
	shifter->setShift(getShift());
}

STFT * FrequencyShiftProcessor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps)
{
	return new frequencyShifter(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate);
}

float FrequencyShiftProcessor::getShift()
{
	{
		if (m_shift != nullptr) {
			return *m_shift;
		}
		else {
			return 0.0;
		}
	}
}
