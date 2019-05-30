#include "FrequencyMagnetProcessor.h"

void FrequencyMagnetProcessor::prepareProcess(STFT* spectralProcess)
{
	auto mag = (frequencyMagnet*)spectralProcess;
	mag->setFrequency(*m_freq);
	mag->setWidthBias(*m_bias);
	mag->setWidth(*m_width);
}

std::unique_ptr<STFT> FrequencyMagnetProcessor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps)
{
	return std::make_unique<frequencyMagnet>(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate);
}