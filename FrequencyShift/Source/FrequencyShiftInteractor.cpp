#include "FrequencyShiftInteractor.h"
#include "FrequencyShiftFFTProcessor.h"


void FrequencyShiftInteractor::prepareProcess(STFT* spectralProcessor)
{
	auto shifter = (FrequencyShiftFFTProcessor*)spectralProcessor;
	shifter->setShift(getShift());
}

std::unique_ptr<STFT> FrequencyShiftInteractor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int chan, int numChans)
{
	return std::make_unique<FrequencyShiftFFTProcessor>(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate);
}

float FrequencyShiftInteractor::getShift()
{
	{
		if (m_shift != nullptr) {
			return *m_shift;
		}
		else {
			return 0.0;
		}
	}
}
