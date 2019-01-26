#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/legacy/specprocess.h"
#include "../../shared/SpectralAudioProcessor.h"

class FrequencyMagnetProcessor : public SpectralAudioProcessor {
public:
	FrequencyMagnetProcessor(int numOverlaps, int numChans) : SpectralAudioProcessor(numOverlaps, numChans), m_freq(nullptr), m_bias(nullptr), m_width(nullptr) {}		
	
	void createParameters(AudioProcessorValueTreeState* valueTreeState) override;
	void prepareProcess(int spectralProcessorIndex) override;
	STFT* createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps) override;		
private:	
	// parameters
	float* m_freq;	
	float* m_bias;
	float* m_width;
};
