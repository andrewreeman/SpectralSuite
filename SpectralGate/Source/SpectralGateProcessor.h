#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/legacy/specprocess.h"
#include "../../shared/SpectralAudioProcessor.h"
#include "SpectralGateParameters.h"

class SpectralGateProcessor : public SpectralAudioProcessor {
public:
	SpectralGateProcessor(int numOverlaps, int numChans, std::shared_ptr<SpectralGateParameters> params) : SpectralAudioProcessor(numOverlaps, numChans) {
		m_cutOff = params->getCutOffValuePointer();
		m_balance = params->getBalanceValuePointer();		
	}
	
	//void createParameters(PluginParameters* parameters) override;
	void prepareProcess(int spectralProcessorIndex) override;
	STFT* createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps) override;		
private:	
	// parameters
	float* m_cutOff;	
	float* m_balance;
};
