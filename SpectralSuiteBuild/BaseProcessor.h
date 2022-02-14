#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/specprocess.h"
#include "../../shared/SpectralAudioProcessorInteractor.h"
#include "BaseParameters.h"

class BaseProcessor : public SpectralAudioProcessorInteractor {
public:
    
    BaseProcessor(int numOverlaps, std::shared_ptr<BaseParameters> params) : SpectralAudioProcessorInteractor(numOverlaps), m_params(params) {}
    
    void prepareProcess(STFT* spectralProcessor) override;
    std::unique_ptr<STFT> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int channel, int channelCount) override;
            

private:
    std::shared_ptr<BaseParameters> m_params;
};
