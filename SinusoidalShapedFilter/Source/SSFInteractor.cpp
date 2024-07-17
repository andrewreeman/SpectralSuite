#include "SSFInteractor.h"
#include "SSF_FFTProcessor.h"
#include "../../shared/wavetable.h"

SSFInteractor::SSFInteractor(int numOverlaps, std::shared_ptr<SSFParameters> params)
    : SpectralAudioProcessorInteractor(numOverlaps)
{
    m_freq = params->getFreqValuePointer();
    m_phase = params->getPhaseValuePointer();
    m_width = params->getWidthValuePointer();
}

void SSFInteractor::prepareProcess(StandardFFTProcessor* spectralProcess) {
	auto ssf = (SSF_FFTProcessor*)spectralProcess;
	ssf->setFrequency(*m_freq);
	ssf->setPhase(*m_phase);
	ssf->setWidth(*m_width);
}

std::unique_ptr<StandardFFTProcessor> SSFInteractor::createSpectralProcess(
    int index, int fftSize, int hopSize, int sampleRate,
    int numOverlaps, int chan, int numChans
)
{
    if(m_wavetable == nullptr) {
        m_wavetable = std::make_shared<Table<float> >(getFftSize() / 2, 1, 1);
    }

    int offset = hopSize * (index%numOverlaps);
	return std::make_unique<SSF_FFTProcessor>(fftSize, hopSize, offset, (int)sampleRate, this->getPhaseBuffer(), m_wavetable);
}

void SSFInteractor::onFftSizeChanged(){
    if(m_wavetable == nullptr) {
        m_wavetable = std::make_shared<Table<float> >(getFftSize() / 2, 1, 1);
    }
    
    m_wavetable->resize(getFftSize() / 2);
}
