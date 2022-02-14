#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/specprocess.h"
#include "../../shared/SpectralAudioProcessorInteractor.h"
#include "SpectralGateParameters.h"

class SpectralGateInteractor : public SpectralAudioProcessorInteractor {
public:
	SpectralGateInteractor(int numOverlaps, std::shared_ptr<SpectralGateParameters> params) : SpectralAudioProcessorInteractor(numOverlaps) {
		m_cutOffParameter = params->getCutOffValuePointer();
		m_balanceParameter = params->getBalanceValuePointer();		
	}

	void prepareProcess(STFT* spectralProcessor) override;
	std::unique_ptr<STFT> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int chan, int numChans) override;
private:
	float* m_cutOffParameter;	
	float* m_balanceParameter;
};
