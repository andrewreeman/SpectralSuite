#include "SpectralAudioProcessorInteractor.h"
#include "../../shared/PhaseVocoder.h"

SpectralAudioProcessorInteractor::SpectralAudioProcessorInteractor(int numOverlaps)
: m_numOverlaps(numOverlaps), m_sampleRate(48000), m_fftHopSize(0), m_numChans(2), m_isPreparingToPlay(false), m_isPlaying(false), m_setOverlapsCallCount(0)
{
    m_phaseBuffer = std::make_shared<PhaseBuffer>(m_numChans * m_numOverlaps, 1024);
}
  
int SpectralAudioProcessorInteractor::getFftSize()
{
    return m_fftSize;
}

void SpectralAudioProcessorInteractor::prepareToPlay(int fftSize, int sampleRate, int channelCount)
{
    auto wasPreparingToPlay = m_isPreparingToPlay;
    m_isPreparingToPlay = true;
    m_fftSize = fftSize;
    m_numChans = channelCount;
    
    if (m_sampleRate != sampleRate && !m_spectralProcess.empty()) {
        m_spectralProcess.clear();
    }
    
	m_sampleRate = sampleRate;
    setNumOverlaps(m_numOverlaps);
    m_isPreparingToPlay = false;
}

void SpectralAudioProcessorInteractor::process(std::vector<std::vector<float>>* input, std::vector<std::vector<float>>* output)
{	
    m_isPlaying = true;
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
        
        if (m_spectralProcess.size() <= chan)
        {
            continue;
        }
        
		for (auto& spectralProcessOverlap : m_spectralProcess.at(chan)) {
			prepareProcess(spectralProcessOverlap.get());
			spectralProcessOverlap->process(input->at(chan).data(), output->at(chan).data(), m_fftHopSize);			
		}
	}
    m_isPlaying = false;
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
    int newOverlapCallCount = (m_setOverlapsCallCount + 1) % 100;
    m_setOverlapsCallCount = newOverlapCallCount;
    
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
    
    if (m_setOverlapsCallCount != newOverlapCallCount) { /* race detected */ return;}

    m_numOverlaps = newOverlapCount;
    m_fftHopSize = getFftSize() / m_numOverlaps;
    
    for (auto& processesPerChannel : m_spectralProcess) {
        if (!processesPerChannel.empty())
        {
            if (m_setOverlapsCallCount != newOverlapCallCount) { /* race detected */ return;}
            processesPerChannel.clear();
        }
    }
    
    // TODO: sometimes this is called from the fft switcher thread and we being destroyed while creating new allocations
    // If spectral process is empty then we may be destroyed and should not clear the buffer. Using a lock instead may prevent us needing to do these hacky checks
    if (!m_spectralProcess.empty())
    {
        if (m_setOverlapsCallCount != newOverlapCallCount) { /* race detected */ return;}
        m_spectralProcess.clear();
    }
    
	for (int chan = 0; chan < m_numChans; ++chan) {
        if (m_setOverlapsCallCount != newOverlapCallCount) { /* race detected */ return;}
        m_spectralProcess.push_back(std::vector<std::unique_ptr<StandardFFTProcessor>>());
        
        for (int specProcess = 0; specProcess < m_numOverlaps; specProcess++) {
            // TODO: sometimes this is called from the fft switcher thread and we being destroyed while creating new allocations
            // If spectral process is empty then we should not continue here. Using a lock instead may prevent us needing to do these hacky checks
            if (m_spectralProcess.empty())
            {
                return;
            }
            
            if (m_setOverlapsCallCount != newOverlapCallCount) { /* race detected */ return;}

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

