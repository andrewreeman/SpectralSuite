#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/StandardFFTProcessor.h"
#include "../../shared/SpectralAudioProcessorInteractor.h"
#include "SSF_FFTProcessor.h"
#include "SSFParameters.h"

class SSFInteractor : public SpectralAudioProcessorInteractor {
public:
	SSFInteractor(int numOverlaps, std::shared_ptr<SSFParameters> params);
	
	//void createParameters(PluginParameters* valueTreeState) override;
	void prepareProcess(StandardFFTProcessor* spectralProcessor) override;
	std::unique_ptr<StandardFFTProcessor> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int chan, int numChans) override;
    void onFftSizeChanged() override;
    
private:	
	// parameters
	float* m_freq;	
	float* m_phase;
	float* m_width;
    SSF_FFTProcessor::SharedTable m_wavetable;
    
};
