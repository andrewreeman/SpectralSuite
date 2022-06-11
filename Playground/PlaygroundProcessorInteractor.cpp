#include "PlaygroundProcessorInteractor.h"

#include "../../shared/SpectralAudioPlugin.h"
#include "../../shared/utilities.h"
#include "PlaygroundFFTProcessor.h"

std::unique_ptr<StandardFFTProcessor> PlaygroundProcessor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int channel, int channelCount)
{
    if(m_phaseBuffers.size() == 0) {
        for(int c = 0; c<channelCount; ++c) {
            auto phaseBuffer = std::make_shared<PhaseBuffer>(numOverlaps, fftSize / 2);
            m_phaseBuffers.push_back(phaseBuffer);
        }
        
        m_fftThread = std::make_shared<FftThread>();
    }

    auto& phaseBuffer = m_phaseBuffers[channel];

    return std::make_unique<PlaygroundFFTProcessor>(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate, phaseBuffer, m_fftThread);
}

void PlaygroundProcessor::prepareProcess(StandardFFTProcessor * spectralProcessor)
{
    auto processor = ((PlaygroundFFTProcessor*)spectralProcessor);
    auto usePvoc = m_params->getUsePvoc().getValue();
    processor->setUsePvoc(usePvoc);
    
    auto doRotate = m_params->getDoWindowRotate().getValue();
    processor->setDoWindowRotate(doRotate);
    
    if(m_params->getThreaded().getValue()) {
        if(!m_fftThread->isThreadRunning()){
            m_fftThread->startThread();
        }
        processor->setUseThreadForLargeFft(true);
    }
    else {
        if(m_fftThread->isThreadRunning()){
            m_fftThread->signalThreadShouldExit();
        }
        processor->setUseThreadForLargeFft(false);
    }
    
    if(m_params->getUseLinearHann().getValue()) {
        processor->setWindowType(PlaygroundFFTProcessor::WindowType::HANN_LINEAR);
    }
    else {
        processor->setWindowType(PlaygroundFFTProcessor::WindowType::HANN);
    }
}

void PlaygroundProcessor::switchOverlapCount() {
    int newNumOverlaps = (int)m_params->getNumberOfOverlaps().getValue();
    setNumOverlaps(newNumOverlaps);    
}
