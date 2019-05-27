#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/legacy/specprocess.h"
#include "../../shared/SpectralAudioProcessor.h"
#include "FrequencyShiftPluginParameters.h"

class FrequencyShiftProcessor : public SpectralAudioProcessor {
public:
	FrequencyShiftProcessor(int numOverlaps, int numChans, std::shared_ptr<FrequencyShiftPluginParameters> params) : SpectralAudioProcessor(numOverlaps, numChans) {
		m_shift = params->getShiftValuePointer();// valueTreeState->getRawParameterValue("shift");
	}
	
	//void createParameters(PluginParameters* valueTreeState) override;
	void prepareProcess(STFT* spectralProcessor) override;
	std::unique_ptr<STFT> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps) override;
	
	float getShift();

private:	
	// parameters
	float* m_shift;	
};
