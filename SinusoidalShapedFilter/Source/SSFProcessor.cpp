#include "SSFProcessor.h"

void SSFProcessor::prepareProcess(STFT* spectralProcess)
{
	auto ssf = (sinusoidalShapedFilter*)spectralProcess;
	ssf->setFrequency(*m_freq);
	ssf->setPhase(*m_phase);
	ssf->setWidth(*m_width);
}

std::unique_ptr<STFT> SSFProcessor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps)
{
	return std::make_unique<sinusoidalShapedFilter>(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate);
}