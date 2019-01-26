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
	// note: the label parameter for the frequency shift parameter is blank. This is due to 2 bugs in JUCE at the time of writing (2018). 
	// 1. The label is not appended to the Juce UI value as expected. But it IS appended to the non-Juce UI. This means we need a custom value to text function.
	// 2. On the non-Juce UI the value to text function is performed...then the label is appended after. Meaning we see it twice in the non-Juce UI. Hence we should add an empty label for now.	
	
	String shiftHertzLabel = " Hz";				
	valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"shift",
		"Frequency shift",
		NormalisableRange<float>(-500.f, 500.f), 0.0f, "",
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
