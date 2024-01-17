#pragma once


#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/StandardFFTProcessor.h"
#include "../../shared/SpectralAudioProcessorInteractor.h"
#include "NewThingParameters.h"

class NewThingProcessor : public SpectralAudioProcessorInteractor {
public:
    
    NewThingProcessor(int numOverlaps, std::shared_ptr<NewThingParameters> params) : SpectralAudioProcessorInteractor(numOverlaps), m_params(params) {}
    
    void prepareProcess(StandardFFTProcessor* spectralProcessor) override;
    std::unique_ptr<StandardFFTProcessor> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int channel, int channelCount) override;
            

private:
    std::shared_ptr<NewThingParameters> m_params;
};
