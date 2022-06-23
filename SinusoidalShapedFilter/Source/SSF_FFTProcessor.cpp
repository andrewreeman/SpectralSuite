#include "SSF_FFTProcessor.h"

SSF_FFTProcessor::SSF_FFTProcessor(int size, int hops, int offset, int sRate, std::shared_ptr<PhaseBuffer> phaseBuffer, SharedTable wavetable)
    : PhaseVocoder(size, hops, offset, sRate, phaseBuffer), m_waveTable(wavetable), m_freq(0.0),
    m_width(0.0), m_phase(0.0)
{
    recalculateInternalParameters();
}

void SSF_FFTProcessor::spectral_process(const PolarVector &in, PolarVector &out, int bins) {
    float sinusoid, wavetableIndex;    

    for(int i=0; i<bins; ++i){
        wavetableIndex = (float)(i * m_indexScale) + m_theta;
        sinusoid = std::max(0.f,  m_waveTable->get_value(wavetableIndex));
        
        out[i].m_mag = in[i].m_mag * std::pow(sinusoid, m_sinusoidExponent);
        out[i].m_phase = in[i].m_phase;
    }
}

bool SSF_FFTProcessor::setFFTSize(int newSize) {
    bool result = PhaseVocoder::setFFTSize(newSize);
    recalculateInternalParameters();
    return result;
}

void SSF_FFTProcessor::recalculateInternalParameters() {
    m_indexScale = m_freq + 1.f;
	m_theta = std::pow(m_phase, 3) * (float)m_halfSize;
	m_sinusoidExponent = (m_width * m_width * 8.f) + 1.f;
}

void SSF_FFTProcessor::setFrequency(float freq) {
    if(m_freq != freq) {
        m_freq = freq;
        recalculateInternalParameters();
    }
}

void SSF_FFTProcessor::setWidth(float width) {
    if(m_width != width) {
        m_width = width;
        recalculateInternalParameters();
    }
}

void SSF_FFTProcessor::setPhase(float phase) {
    if(m_phase != phase) {
        m_phase = phase;
        recalculateInternalParameters();
    }
}
