#pragma once

#include "../../shared/PhaseVocoder.h"
#include "../../shared/PhaseBuffer.h"

class FrequencyMagnetFFTProcessor : public PhaseVocoder {
public:
    FrequencyMagnetFFTProcessor(int size, int hops, int offset, int sRate, std::shared_ptr<PhaseBuffer> phaseBuffer)
        : PhaseVocoder(size, hops, offset, sRate, phaseBuffer), m_freq(0.5), m_width(0.5), m_widthBias(0.5), m_useLegacyHighFrequencyShift(false) {}

    virtual void spectral_process(const PolarVector &in, PolarVector &out, int bins) override;

    void setFrequency(float freq) { m_freq = freq; }
    void setWidth(float width) { m_width = width; }
    void setWidthBias(float widthBias) { m_widthBias = widthBias; };
    void setUseLegacyHighFrequencyShift(bool useLegacy) { m_useLegacyHighFrequencyShift = useLegacy; }

private:
    float m_freq;
    float m_width;
    float m_widthBias;
    bool m_useLegacyHighFrequencyShift;
};
