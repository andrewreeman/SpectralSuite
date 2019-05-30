#include "SpectralAudioProcessor.h"

int SpectralAudioProcessor::getFftSize()
{
	return m_fftHopSize * m_numOverlaps;		
}

void SpectralAudioProcessor::prepareToPlay(int fftSize, int sampleRate, int channelCount)
{	
	m_sampleRate = sampleRate;
	m_fftHopSize = fftSize / m_numOverlaps; //SpectralAudioPlugin::FFT_OVERLAPS;
	m_spectralProcess.clear();
	
	for (int chan = 0; chan < channelCount; ++chan) {		 
		m_spectralProcess.push_back(std::vector<std::unique_ptr<STFT>>());
		
		for (int specProcess = 0; specProcess < m_numOverlaps; specProcess++) {
			m_spectralProcess.at(chan).push_back(createSpectralProcess(specProcess, fftSize, m_fftHopSize, sampleRate, m_numOverlaps));
		}			
	}
				
}

void SpectralAudioProcessor::process(std::vector<std::vector<float>>* input, std::vector<std::vector<float>>* output)
{	
	//need to take pointer to vector of vector of float
	jassert(input != nullptr);
	jassert(output != nullptr);
	jassert(input->size() > 0);
	jassert(input->at(0).size() > 0);
	jassert(input->size() == output->size());
	jassert(input->size() == m_spectralProcess.size());

	for (int chan = 0; chan < input->size(); chan++) {
		for (auto& spectralProcessOverlap : m_spectralProcess.at(chan)) {
			prepareProcess(spectralProcessOverlap.get());
			spectralProcessOverlap->process(input->at(chan).data(), output->at(chan).data(), m_fftHopSize);			
		}
	}
}

void SpectralAudioProcessor::setFftSize(int fftSize)
{
	m_fftHopSize = fftSize / m_numOverlaps;
	for (auto& spectralProcessChannel : m_spectralProcess) {
		for(int i=0; i<spectralProcessChannel.size(); ++i) {			
			
			if (fftSize == spectralProcessChannel[i]->getFFTSize()) { continue; }

			spectralProcessChannel[i]->setFFTSize(fftSize);
			spectralProcessChannel[i]->setHopSize(m_fftHopSize);
			spectralProcessChannel[i]->setOffset(m_fftHopSize * (i%m_numOverlaps));
		}
	}

	this->onFftSizeChanged();
}
