#include "SpectralGateInteractor.h"
#include "SpectralGateFFTProcessor.h"

void SpectralGateInteractor::prepareProcess(StandardFFTProcessor* spectralProcessor)
{
	auto gate = (SpectralGateFFTProcessor*)spectralProcessor;
	gate->setCutOff(*m_cutOffParameter);
	gate->setBalance(*m_balanceParameter);
    gate->setTilt(*m_tiltParameter);
    gate->setTiltEnabled(m_params->getTiltEnabled());
}

std::unique_ptr<StandardFFTProcessor> SpectralGateInteractor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int chan, int numChans)
{
    auto phaseBuffer = this->getPhaseBuffer();
	return std::make_unique<SpectralGateFFTProcessor>(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate, phaseBuffer);
}
