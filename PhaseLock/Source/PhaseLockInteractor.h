#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/StandardFFTProcessor.h"
#include "../../shared/SpectralAudioProcessorInteractor.h"
#include "PhaseLockParameters.h"

class PhaseLockInteractor : public SpectralAudioProcessorInteractor {
public:
	
	PhaseLockInteractor(int numOverlaps, std::shared_ptr<PhaseLockParameters> params) : SpectralAudioProcessorInteractor(numOverlaps), m_params(params) {}
	
	void prepareProcess(StandardFFTProcessor* spectralProcessor) override;
	std::unique_ptr<StandardFFTProcessor> createSpectralProcess(int index, int fftSize, int hopSize,
        int sampleRate, int numOverlaps, int chans, int numChans) override;            
    
    void process(SpectralAudioPlugin* plugin, std::vector<std::vector<float>>* input, std::vector<std::vector<float>>* output) override;
private:
	std::shared_ptr<PhaseLockParameters> m_params;    
};
