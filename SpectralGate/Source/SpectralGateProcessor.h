#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/legacy/specprocess.h"
#include "../../shared/SpectralAudioProcessor.h"
#include "SpectralGateParameters.h"

class SpectralGateProcessor : public SpectralAudioProcessor {
public:
	SpectralGateProcessor(int numOverlaps, std::shared_ptr<SpectralGateParameters> params) : SpectralAudioProcessor(numOverlaps) {
		m_cutOff = params->getCutOffValuePointer();
		m_balance = params->getBalanceValuePointer();		
	}

	void prepareProcess(STFT* spectralProcessor) override;
	std::unique_ptr<STFT> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps) override;
private:	
	// parameters
	float* m_cutOff;	
	float* m_balance;
};
