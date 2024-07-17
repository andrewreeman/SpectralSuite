#include "FrequencyShiftInteractor.h"
#include "FrequencyShiftFFTProcessor.h"
#include "../../shared/PhaseBuffer.h"


void FrequencyShiftInteractor::prepareProcess(StandardFFTProcessor* spectralProcessor)
{
	auto shifter = (FrequencyShiftFFTProcessor*)spectralProcessor;
	shifter->setShift(getShift());
}

std::unique_ptr<StandardFFTProcessor> FrequencyShiftInteractor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int chan, int numChans)
{
    
    auto phaseBuffer = this->getPhaseBuffer();
	return std::make_unique<FrequencyShiftFFTProcessor>(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate, phaseBuffer);
}

float FrequencyShiftInteractor::getShift()
{
    if (m_shift != nullptr) {
        return *m_shift;
    }
    else {
        return 0.0;
    }
}

