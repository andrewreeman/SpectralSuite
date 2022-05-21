#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/StandardFFTProcessor.h"
#include "../../shared/SpectralAudioProcessorInteractor.h"
#include "FrequencyMagnetParameters.h"


class FrequencyMagnetInteractor : public SpectralAudioProcessorInteractor {
public:
	FrequencyMagnetInteractor(int numOverlaps, std::shared_ptr<FrequencyMagnetParameters> params) : SpectralAudioProcessorInteractor(numOverlaps) {
		m_freq = params->getFreqValuePointer();
		m_bias = params->getBiasValuePointer();
		m_width = params->getWidthValuePointer();
		m_params = params;
	}
		
	void prepareProcess(StandardFFTProcessor* spectralProcessor) override;
	std::unique_ptr<StandardFFTProcessor> createSpectralProcess(
        int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int chan, int numChans
    ) override;
    
    void receivedMidi(MidiBuffer& midi) override;
    
private:	
	// parameters
	float* m_freq;	
	float* m_bias;
	float* m_width;
	std::shared_ptr<FrequencyMagnetParameters> m_params;
};
