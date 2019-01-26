#include "SpectralAudioProcessor.h"

int SpectralAudioProcessor::getFftSize()
{
	return m_fftHopSize * m_numOverlaps;		
}

void SpectralAudioProcessor::prepareToPlay(int fftSize, int sampleRate)
{	
	m_sampleRate = sampleRate;
	m_fftHopSize = fftSize / m_numOverlaps; //SpectralAudioPlugin::FFT_OVERLAPS;
	m_spectralProcess.clear();
	for (int i = 0; i < m_numOverlaps * m_numChans; ++i) {
		m_spectralProcess.add(createSpectralProcess(i, fftSize, m_fftHopSize, sampleRate, m_numOverlaps));
	}
}

void SpectralAudioProcessor::process(const float* input, float* output, const float* inputR, float* outputR)
{
	float params[] = {0.0};
	for (int i = 0; i < m_spectralProcess.size(); ++i) {
		prepareProcess(i);
		
		// This is a very ugly and inflexible way of doing stereo processing
		if (i < m_numOverlaps) {
			m_spectralProcess[i]->process(&input[0], &output[0], m_fftHopSize, params);
		}
		else {
			m_spectralProcess[i]->process(&inputR[0], &outputR[0], m_fftHopSize, params);
		}
	}
}

void SpectralAudioProcessor::setFftSize(int fftSize)
{
	m_fftHopSize = fftSize / m_numOverlaps;
	for (int i = 0; i < m_spectralProcess.size(); i++) {
		auto spectralProcess = m_spectralProcess[i];
		if (fftSize == spectralProcess->getFFTSize()) { continue; }

		spectralProcess->setFFTSize(fftSize);
		spectralProcess->setHopSize(m_fftHopSize);
		spectralProcess->setOffset(m_fftHopSize * (i%m_numOverlaps));
	}

	this->onFftSizeChanged();
}
