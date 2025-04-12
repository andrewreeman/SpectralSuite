#pragma once

#include "JuceHeader.h"
#include "../../shared/StandardFFTProcessor.h"
#include "../../shared/SpectralAudioProcessorInteractor.h"
#include "FrequencyShiftPluginParameters.h"

class FrequencyShiftInteractor : public SpectralAudioProcessorInteractor {
public:
	FrequencyShiftInteractor(const int numOverlaps, const std::shared_ptr<FrequencyShiftPluginParameters> &params) : SpectralAudioProcessorInteractor(numOverlaps) {
		m_shift = params->getShiftValuePointer();
		m_scale = params->getScaleValuePointer();
	}
		
	void prepareProcess(StandardFFTProcessor* spectralProcessor) override;
	std::unique_ptr<StandardFFTProcessor> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int chan, int numChans) override;
	
	float getShift() const;
	float getScale() const;

private:	
	// parameters
	float* m_shift;
	float* m_scale;
};
