#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/StandardFFTProcessor.h"
#include "../../shared/SpectralAudioProcessorInteractor.h"
#include "SpectralGateParameters.h"

class SpectralGateInteractor : public SpectralAudioProcessorInteractor {
public:
	SpectralGateInteractor(int numOverlaps, std::shared_ptr<SpectralGateParameters> params) : SpectralAudioProcessorInteractor(numOverlaps) {
		m_cutOffParameter = params->getCutOffValuePointer();
		m_balanceParameter = params->getBalanceValuePointer();		
	}

	void prepareProcess(StandardFFTProcessor* spectralProcessor) override;
	std::unique_ptr<StandardFFTProcessor> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int chan, int numChans) override;
private:
	float* m_cutOffParameter;	
	float* m_balanceParameter;
};
