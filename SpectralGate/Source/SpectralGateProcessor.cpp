#include "SpectralGateProcessor.h"

void SpectralGateProcessor::prepareProcess(STFT* spectralProcessor)
{
	auto gate = (spectralGate*)spectralProcessor;
	gate->setCutOff(*m_cutOff);
	gate->setBalance(*m_balance);
}

std::unique_ptr<STFT> SpectralGateProcessor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps)
{
	return std::make_unique<spectralGate>(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate);
}