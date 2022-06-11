#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/StandardFFTProcessor.h"
#include "../../shared/SpectralAudioProcessorInteractor.h"
#include "FrequencyShiftPluginParameters.h"

class FrequencyShiftInteractor : public SpectralAudioProcessorInteractor {
public:
	FrequencyShiftInteractor(int numOverlaps, std::shared_ptr<FrequencyShiftPluginParameters> params) : SpectralAudioProcessorInteractor(numOverlaps) {
		m_shift = params->getShiftValuePointer();// valueTreeState->getRawParameterValue("shift");
	}
		
	void prepareProcess(StandardFFTProcessor* spectralProcessor) override;
	std::unique_ptr<StandardFFTProcessor> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int chan, int numChans) override;
	
	float getShift();

private:	
	// parameters
	float* m_shift;	
};
