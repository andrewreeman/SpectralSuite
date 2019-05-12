#include "FrequencyMagnetProcessor.h"

void FrequencyMagnetProcessor::createParameters(PluginParameters* valueTreeState)
{				
	m_freq = valueTreeState->getRawParameterValue("freq");			
	m_bias = valueTreeState->getRawParameterValue("bias");
	m_width = valueTreeState->getRawParameterValue("width");
}

void FrequencyMagnetProcessor::prepareProcess(int spectralProcessIndex)
{
	auto mag = (frequencyMagnet*)m_spectralProcess[spectralProcessIndex];
	mag->setFrequency(*m_freq);
	mag->setWidthBias(*m_bias);
	mag->setWidth(*m_width);
}

STFT * FrequencyMagnetProcessor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps)
{
	return new frequencyMagnet(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate);
}