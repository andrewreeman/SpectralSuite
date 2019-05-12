#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/legacy/specprocess.h"
#include "../../shared/SpectralAudioProcessor.h"

class SSFProcessor : public SpectralAudioProcessor {
public:
	SSFProcessor(int numOverlaps, int numChans) : SpectralAudioProcessor(numOverlaps, numChans), m_freq(nullptr), m_phase(nullptr), m_width(nullptr) {}		
	
	void createParameters(PluginParameters* valueTreeState) override;
	void prepareProcess(int spectralProcessorIndex) override;
	STFT* createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps) override;		
private:	
	// parameters
	float* m_freq;	
	float* m_phase;
	float* m_width;
};
