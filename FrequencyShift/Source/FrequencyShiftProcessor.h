/*
  ==============================================================================

    FrequencyShiftProcesor.h
    Created: 31 Dec 2018 11:48:29am
    Author:  rem_d

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/legacy/specprocess.h"
#include "../../shared/SpectralAudioProcessor.h"

class FrequencyShiftProcessor : public SpectralAudioProcessor {
public:
	FrequencyShiftProcessor(int numOverlaps, int numChans) : SpectralAudioProcessor(numOverlaps, numChans), m_shift(nullptr) {}		
	
	void createParameters(AudioProcessorValueTreeState* valueTreeState) override;
	void prepareProcess(int spectralProcessorIndex) override;
	STFT* createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps) override;
	
	float getShift();

private:	
	// parameters
	float* m_shift;	
};
