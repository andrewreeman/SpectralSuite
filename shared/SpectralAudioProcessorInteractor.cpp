#include "SpectralAudioProcessorInteractor.h"

int SpectralAudioProcessorInteractor::getFftSize()
{
    return m_fftSize;
}

void SpectralAudioProcessorInteractor::prepareToPlay(int fftSize, int sampleRate, int channelCount)
{
    m_fftSize = fftSize;
    m_numChans = channelCount;
	m_sampleRate = sampleRate;
    setNumOverlaps(m_numOverlaps);
}

void SpectralAudioProcessorInteractor::process(std::vector<std::vector<float>>* input, std::vector<std::vector<float>>* output)
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

void SpectralAudioProcessorInteractor::setFftSize(int fftSize)
{
	m_fftHopSize = fftSize / m_numOverlaps;
    m_fftSize = fftSize;
    
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

void SpectralAudioProcessorInteractor::setNumOverlaps(int newOverlapCount) {
    if(newOverlapCount < 1 ){ return; }
    
    m_numOverlaps = newOverlapCount;
    m_fftHopSize = getFftSize() / m_numOverlaps;
    m_spectralProcess.clear();
        
	for (int chan = 0; chan < m_numChans; ++chan) {
        m_spectralProcess.push_back(std::vector<std::unique_ptr<StandardFFTProcessor>>());
        
        for (int specProcess = 0; specProcess < m_numOverlaps; specProcess++) {
            m_spectralProcess.at(chan).push_back(
                createSpectralProcess(specProcess, m_fftSize, m_fftHopSize, m_sampleRate, m_numOverlaps, chan, m_numChans)
            );
        }
	}
}


