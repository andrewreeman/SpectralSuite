#pragma once

#include "../../shared/StandardFFTProcessor.h"
#include "../../shared/wavetable.h"

class SSF_FFTProcessor : public StandardFFTProcessor {
public:
using SharedTable = std::shared_ptr<Table<float> >;
    SSF_FFTProcessor(int size, int hops, int offset, int sRate, SharedTable wavetable);
        
    void spectral_process(const PolarVector &in, PolarVector &out, int bins) override;
    bool setFFTSize(int newSize) override;

    void setFrequency(float freq);
    void setWidth(float width);
    void setPhase(float phase);

private:
    void recalculateInternalParameters();

    SharedTable m_waveTable;
    float m_freq;
    float m_width;
    float m_phase;
    
    float m_indexScale;
    float m_theta;
    float m_sinusoidExponent;
};

