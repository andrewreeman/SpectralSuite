#include "FrequencyShiftFFTProcessor.h"

FrequencyShiftFFTProcessor::FrequencyShiftFFTProcessor(int size, int hops, int offset, int sRate, std::shared_ptr<PhaseBuffer> phaseBuffer)
    : PhaseVocoder(size, hops, offset, sRate, phaseBuffer), m_shift(0), m_binShift(0), m_shiftStartIndex(0), m_shiftEndIndex(0)
{
    recalculateInternalParameters();
}

void FrequencyShiftFFTProcessor::spectral_process(const PolarVector &in, PolarVector &out, int) {
    if (m_binShift == 0) {
        for (size_t i = 0; i < (size_t)in.size(); ++i) {
            out[i] = in[i];
        }
        return;
    }

    auto start = (size_t)m_shiftStartIndex;
    auto end = (size_t)m_shiftEndIndex;
    long binShift = m_binShift;
    
    std::fill(out.begin(), out.end(), Polar<FftDecimal>(0.f, 0.f));
    for (auto i = start; i < end; ++i) {
        out[i + (size_t)binShift] = in[i];
    }
}

bool FrequencyShiftFFTProcessor::setFFTSize(int newSize) {
    bool success = PhaseVocoder::setFFTSize(newSize);
    if(success) {
        recalculateInternalParameters();
    }
    return success;
}

void FrequencyShiftFFTProcessor::recalculateInternalParameters() {
    if(getSampleRate() == 0) {
        m_shiftStartIndex = 0;
        m_shiftEndIndex = getFFTSize();
        m_binShift = 0;
        return;
    }
        
    int halfFftSize = getFFTSize() / 2;
    float binWidth = (float)getFFTSize() / (float)getSampleRate();
    int shiftFrequency = m_shift;
    m_binShift = (int) ((float)shiftFrequency * (float)binWidth);
    
    m_shiftStartIndex = m_binShift > 0 ? 0 : abs(m_binShift);
    m_shiftEndIndex = m_binShift > 0 ? halfFftSize - m_binShift : halfFftSize;
}
