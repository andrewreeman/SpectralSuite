#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/specprocess.h"
#include "../../shared/SpectralAudioProcessorInteractor.h"
#include "../../shared/PhaseBuffer.h"
#include "PlaygroundParameters.h"

class PlaygroundProcessor : public SpectralAudioProcessorInteractor {
public:
    
    PlaygroundProcessor(int numOverlaps, std::shared_ptr<PlaygroundParameters> params) : SpectralAudioProcessorInteractor(numOverlaps), m_params(params) {}
    
    ~PlaygroundProcessor(){
        if(m_fftThread->isThreadRunning()) {
            m_fftThread->stopThread(-1);
        }
    }
    
    void prepareProcess(STFT* spectralProcessor) override;
    std::unique_ptr<STFT> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int channel, int channelCount) override;
    void switchOverlapCount() override;
            

private:
    std::shared_ptr<PlaygroundParameters> m_params;
    std::vector<std::shared_ptr<PhaseBuffer>> m_phaseBuffers;
    std::shared_ptr<FftThread> m_fftThread;
};