#include "FrequencyShiftInteractor.h"
#include "FrequencyShiftFFTProcessor.h"

void FrequencyShiftInteractor::prepareProcess(StandardFFTProcessor* spectralProcessor)
{
	const auto shifter = dynamic_cast<FrequencyShiftFFTProcessor *>(spectralProcessor);
	shifter->setShift(getShift());
    shifter->setScale(getScale());
}

std::unique_ptr<StandardFFTProcessor> FrequencyShiftInteractor::createSpectralProcess(const int index, int fftSize, int hopSize, int sampleRate, const int numOverlaps, int, int)
{
    
    auto phaseBuffer = this->getPhaseBuffer();
	return std::make_unique<FrequencyShiftFFTProcessor>(fftSize, hopSize, hopSize * (index%numOverlaps), sampleRate, phaseBuffer);
}

float FrequencyShiftInteractor::getShift() const {
    if (m_shift != nullptr) {
        return *m_shift;
    }
    return 0.0;
}

float FrequencyShiftInteractor::getScale() const {
    if (m_scale != nullptr) {
        return *m_scale;
    }
    return 1.f;
}

