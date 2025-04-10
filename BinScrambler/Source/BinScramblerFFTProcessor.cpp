#include "BinScramblerFFTProcessor.h"
#include "../../shared/utilities.h"

void BinScramblerFFTProcessor::spectral_process(const PolarVector &in, PolarVector &out, int bins) {

    const int phase = m_phase;
    const int maxPhase = m_maxPhase;

    for(size_t i=0; i<(size_t)bins; ++i){
        out[i].m_mag = utilities::interp_lin<FftDecimal>(in[(size_t)m_AInd->at(i)].m_mag, in[(size_t)m_BInd->at(i)].m_mag, (FftDecimal)phase/(FftDecimal)maxPhase);
        out[i].m_phase = utilities::interp_lin<FftDecimal>(in[(size_t)m_AInd->at(i)].m_phase, in[(size_t)m_BInd->at(i)].m_phase, (FftDecimal)phase/(FftDecimal)maxPhase);
    }
}
