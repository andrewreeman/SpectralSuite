#include "BaseProcessor.h"

#include "../../shared/SpectralAudioPlugin.h"
#include "../../shared/utilities.h"

class emptyProcess : public StandardFFTProcessor {
public:
    emptyProcess(int fftSize, int hopSize, int offset, int sampleRate) : StandardFFTProcessor(fftSize, hopSize, offset, sampleRate) {}
    
    void spectral_process(const PolarVector &in, PolarVector &out, int bins) override {
        for(int i=0; i<bins; ++i) {
            out[i] = in[i];
        }
    };
};

std::unique_ptr<StandardFFTProcessor> BaseProcessor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int channel, int channelCount)
{
    return std::make_unique<emptyProcess>(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate);
}

void BaseProcessor::prepareProcess(StandardFFTProcessor * spectralProcessor)
{
    auto processor = ((emptyProcess*)spectralProcessor);
    // TODO: set the parameters of the processor from the BaseParameters class
}
