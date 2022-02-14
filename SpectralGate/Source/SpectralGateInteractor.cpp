#include "SpectralGateInteractor.h"
#include "SpectralGateFFTProcessor.h"

void SpectralGateInteractor::prepareProcess(STFT* spectralProcessor)
{
	auto gate = (SpectralGateFFTProcessor*)spectralProcessor;
	gate->setCutOff(*m_cutOffParameter);
	gate->setBalance(*m_balanceParameter);
}

std::unique_ptr<STFT> SpectralGateInteractor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int chan, int numChans)
{
	return std::make_unique<SpectralGateFFTProcessor>(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate);
}
