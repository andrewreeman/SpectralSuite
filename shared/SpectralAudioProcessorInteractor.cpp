#include "SpectralAudioProcessorInteractor.h"
#include "../../shared/PhaseVocoder.h"

SpectralAudioProcessorInteractor::SpectralAudioProcessorInteractor(int numOverlaps)
: m_numOverlaps(numOverlaps), m_sampleRate(48000), m_fftHopSize(0), m_numChans(2), m_isPreparingToPlay(false)
{
    m_phaseBuffer = std::make_shared<PhaseBuffer>(m_numChans * m_numOverlaps, 1024);
}
  
int SpectralAudioProcessorInteractor::getFftSize()
{
    return m_fftSize;
}

void SpectralAudioProcessorInteractor::prepareToPlay(int fftSize, int sampleRate, int channelCount)
{
    m_isPreparingToPlay = true;
    m_fftSize = fftSize;
    m_numChans = channelCount;
	m_sampleRate = sampleRate;
    setNumOverlaps(m_numOverlaps);
    m_isPreparingToPlay = false;
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
        if (input->at(chan).size() == 0)
        {
            continue;
        }
        
		for (auto& spectralProcessOverlap : m_spectralProcess.at(chan)) {
			prepareProcess(spectralProcessOverlap.get());
			spectralProcessOverlap->process(input->at(chan).data(), output->at(chan).data(), m_fftHopSize);			
		}
	}
}

void SpectralAudioProcessorInteractor::setFftSize(int fftSize)
{
    if(fftSize == m_fftSize) {
        this->onFftSizeChanged();
        return;
    }
    
    if(fftSize > 32768 && m_numOverlaps > 2) {
        setNumOverlaps(2);
    }
    else if(fftSize <= 32768 && m_numOverlaps == 2) {
        setNumOverlaps(4);
    }
    
	m_fftHopSize = fftSize / m_numOverlaps;
    m_fftSize = fftSize;
    m_phaseBuffer->requestResize(fftSize);
    
	for (auto& spectralProcessChannel : m_spectralProcess) {
		for(int i=0; i<spectralProcessChannel.size(); ++i) {			
			
			if (fftSize == spectralProcessChannel[i]->getFFTSize()) { continue; }

			spectralProcessChannel[i]->setFFTSize(fftSize);
			spectralProcessChannel[i]->setHopSize(m_fftHopSize);	

			// TODO: casting was the only way to ensure the correct setOffset method is called, even though PhaseVocoder overrides it
			((PhaseVocoder*) spectralProcessChannel[i].get())->setOffset(m_fftHopSize * (i%m_numOverlaps));
			//spectralProcessChannel[i]->setOffset(m_fftHopSize * (i%m_numOverlaps));
		}
	}
    
	this->onFftSizeChanged();
}

void SpectralAudioProcessorInteractor::setNumOverlaps(int newOverlapCount) {
    if (newOverlapCount < 1 ||newOverlapCount > 8) {
        return;
    }
    
    if (m_spectralProcess.size() == m_numChans && m_numChans > 0) {
       if (m_spectralProcess.at(0).size() == newOverlapCount)
       {
           if (m_numOverlaps != newOverlapCount)
           {
               m_numOverlaps = newOverlapCount;
           }
           
           return;
       }
   }

    m_numOverlaps = newOverlapCount;
    m_fftHopSize = getFftSize() / m_numOverlaps;
    
    // TODO: we
    for (auto& processesPerChannel : m_spectralProcess) {
        processesPerChannel.clear();
    }
    
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

void SpectralAudioProcessorInteractor::setWindowType(FftWindowType windowType) {
    for(auto& processes : m_spectralProcess) {
        for(auto& p : processes) {
            if (isPreparingToPlay()) { return; }
            p->setWindowType(windowType);
        }
    }
}

