#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/legacy/specprocess.h"
#include "../../shared/SpectralAudioProcessor.h"
#include "SSFParameters.h"

class SSFProcessor : public SpectralAudioProcessor {
public:
	SSFProcessor(int numOverlaps, std::shared_ptr<SSFParameters> params) : SpectralAudioProcessor(numOverlaps) {
		m_freq = params->getFreqValuePointer();
		m_phase = params->getPhaseValuePointer();
		m_width = params->getWidthValuePointer();		
	}		
	
	//void createParameters(PluginParameters* valueTreeState) override;
	void prepareProcess(STFT* spectralProcessor) override;
	std::unique_ptr<STFT> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps) override;		
private:	
	// parameters
	float* m_freq;	
	float* m_phase;
	float* m_width;
};
