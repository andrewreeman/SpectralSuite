#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/legacy/specprocess.h"
#include "../../shared/SpectralAudioProcessor.h"
#include "SSFParameters.h"

class SSFProcessor : public SpectralAudioProcessor {
public:
	SSFProcessor(int numOverlaps, int numChans, std::shared_ptr<SSFParameters> params) : SpectralAudioProcessor(numOverlaps, numChans) {
		m_freq = params->getFreqValuePointer();
		m_phase = params->getPhaseValuePointer();
		m_width = params->getWidthValuePointer();		
	}		
	
	//void createParameters(PluginParameters* valueTreeState) override;
	void prepareProcess(int spectralProcessorIndex) override;
	STFT* createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps) override;		
private:	
	// parameters
	float* m_freq;	
	float* m_phase;
	float* m_width;
};
