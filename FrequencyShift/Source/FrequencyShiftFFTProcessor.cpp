#include "FrequencyShiftFFTProcessor.h"

FrequencyShiftFFTProcessor::FrequencyShiftFFTProcessor(const int size, const int hops, const int offset, const int sRate, const std::shared_ptr<PhaseBuffer> &phaseBuffer)
    : PhaseVocoder(size, hops, offset, sRate, phaseBuffer), m_shift(0), m_scale(1.f), m_binShift(0), m_shiftStartIndex(0), m_shiftEndIndex(0)
{
    recalculateInternalParameters();
}

void FrequencyShiftFFTProcessor::spectral_process(const PolarVector &in, PolarVector &out, int) {
    if (m_binShift == 0 && approximatelyEqual(m_scale, 0.f)) {
        for (size_t i = 0; i < in.size(); ++i) {
            out[i] = in[i];
        }
        return;
    }

    const auto start = static_cast<size_t>(m_shiftStartIndex);
    const auto end = static_cast<size_t>(m_shiftEndIndex);
    const auto scaleEnd = in.size();
    const long binShift = m_binShift;

    std::fill(out.begin(), out.end(), Polar(0.f, 0.f));
    if (!approximatelyEqual(m_scale, 0.f)) {
        for (size_t i = 0; i < in.size(); ++i) {
            if (const auto newIndex = static_cast<size_t>(static_cast<float>(i) * m_scale); newIndex < scaleEnd) {
                out[newIndex] = in[i];
            }
        }
    }

    if (m_binShift != 0) {
        for (size_t i = start; i < end; ++i) {
            out[i + static_cast<size_t>(binShift)] = in[i];
        }
    }
}

bool FrequencyShiftFFTProcessor::setFFTSize(const int newSize) {
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
    float binWidth = static_cast<float>(getFFTSize()) / static_cast<float>(getSampleRate());
    int shiftFrequency = m_shift;
    m_binShift = static_cast<int>(static_cast<float>(shiftFrequency) * binWidth);
    
    m_shiftStartIndex = m_binShift > 0 ? 0 : abs(m_binShift);
    m_shiftEndIndex = m_binShift > 0 ? halfFftSize - m_binShift : halfFftSize;
}
