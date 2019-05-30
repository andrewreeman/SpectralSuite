#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/legacy/specprocess.h"
#include "../../shared/SpectralAudioProcessor.h"
#include "FrequencyMagnetParameters.h"

class FrequencyMagnetProcessor : public SpectralAudioProcessor {
public:
	FrequencyMagnetProcessor(int numOverlaps, std::shared_ptr<FrequencyMagnetParameters> params) : SpectralAudioProcessor(numOverlaps) {
		m_freq = params->getFreqValuePointer();// valueTreeState->getRawParameterValue("freq");
		m_bias = params->getBiasValuePointer(); // ->getRawParameterValue("bias");
		m_width = params->getWidthValuePointer();// valueTreeState->getRawParameterValue("width");
	}
		
	void prepareProcess(STFT* spectralProcessor) override;
	std::unique_ptr<STFT> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps) override;		
private:	
	// parameters
	float* m_freq;	
	float* m_bias;
	float* m_width;
};
