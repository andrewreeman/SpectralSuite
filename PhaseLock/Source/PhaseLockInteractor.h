#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/specprocess.h"
#include "../../shared/SpectralAudioProcessorInteractor.h"
#include "PhaseLockParameters.h"

class PhaseLockInteractor : public SpectralAudioProcessorInteractor {
public:
	
	PhaseLockInteractor(int numOverlaps, std::shared_ptr<PhaseLockParameters> params) : SpectralAudioProcessorInteractor(numOverlaps), m_params(params) {}
	
	void prepareProcess(STFT* spectralProcessor) override;
	std::unique_ptr<STFT> createSpectralProcess(int index, int fftSize, int hopSize,
        int sampleRate, int numOverlaps, int chans, int numChans) override;            

private:	
	std::shared_ptr<PhaseLockParameters> m_params;    
};
