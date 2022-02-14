#include "BinScramblerFFTProcessor.h"
#include "../../shared/utilities.h"

void BinScramblerFFTProcessor::spectral_process(const PolarVector &in, PolarVector &out, int bins) {

    const int phase = m_phase;
    const int maxPhase = m_maxPhase;

    for(int i=0; i<bins; ++i){
        out[i].m_mag = utilities::interp_lin<FftDecimal>(in[m_AInd->at(i)].m_mag, in[m_BInd->at(i)].m_mag, (FftDecimal)phase/(FftDecimal)maxPhase);
        out[i].m_phase = utilities::interp_lin<FftDecimal>(in[m_AInd->at(i)].m_phase, in[m_BInd->at(i)].m_phase, (FftDecimal)phase/(FftDecimal)maxPhase);
    }
}
